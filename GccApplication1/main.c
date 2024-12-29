/*
 * GccApplication1.c
 *
 * Created: 2019-08-28 오후 12:13:16
 * Author : CDSL
 */ 

#include "mcu_init.h"
#include "dataType.h"

#define F_CPU 16000000UL
#define dt 0.0005			// 0.5ms

#define Kt	0.0683			// 역기전력 상수

#define Kpc 0.827			// 전류제어기 P Gain  (La*wcc) => wcc=2pi*fcc
#define Kic 2.2117e+03		// 전류제어기 I Gain  (Ra/La)*Kp
#define Kac 1.21			// 전류제어기 anti wind up (1/Kpc)

#define Kpv 2.62			// 속도제어기 P Gain (J*wcv)/Kt 를 이용하여 계산			
#define Kiv 40.0			// 속도제어기 I Gain (B/J)*Kp => 매트랩에서 설계한 제어기의 값을 이용하면 진동 발생
//   --> 값이 큰 I Gain을 사용하면 정상상태 오차가 줄어들지만, 진동수가 커지게 된다. (불안정해짐)
#define Kav	0.381			// 속도제어기 anti wind up (1/Kpv)
 
#define Kpp 12.56			// 위치제어기 P Gain kpp=wcp=wcs/10=4pi		
#define Kdp 0.1				// 위치제어기 D Gain kpd=wcp/wcs=1/10


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


volatile int32_t g_Cnt, g_preCnt;

volatile double g_Pdes = 0., g_Ppre = 0.;
volatile double g_Pcur = 0., g_Pre_Pcur = 0.;
volatile double g_Perr = 0.;
volatile double g_Perr_prev = 0.;

volatile double g_Vcur=0., g_Vpre=0.;
volatile double g_Vdes=10.;
volatile double g_Verr=0.;
volatile double g_Vlimit = 1.;
volatile double g_Verr_sum=0.;


volatile double g_Ccur=0.;
volatile double g_Cdes=0.;
volatile double g_Cerr=0.;
volatile double g_Cerr_sum=0.;
volatile double g_Climit = 1.;

volatile double g_ADC;
volatile int g_SendFlag = 0;
volatile int g_Direction;

volatile int cur_control = 0;
volatile double g_vel_control=0.;
volatile double g_pos_control=0.;
volatile unsigned char g_TimerCnt;

volatile unsigned char checkSize;
volatile unsigned char g_buf[256], g_BufWriteCnt, g_BufReadCnt;

volatile Packet_t g_PacketBuffer;
volatile unsigned char g_PacketMode;
volatile unsigned char g_ID = 1;

void SetDutyCW(double v){
	
	while(TCNT1  == 0);

	int ocr = v * (200. / 24.) + 200;
	
	if(ocr > OCR_MAX)	ocr = OCR_MAX;
	else if(ocr < OCR_MIN)	ocr = OCR_MIN;
	//OCR1A = OCR1B = ocr;
	
	OCR1A = OCR3B = ocr + 8;		//1 H
	OCR1B = OCR3A = ocr - 8;		//1 L
}


void InitLS7366(){
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_MDR0 | WR_REG);
	SPI_MasterSend(X4_QUAD | FREE_RUN | DISABLE_INDEX | SYNCHRONOUS_INDEX |FILTER_CDF_1);
	PORTB = 0x01;
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_MDR1 | WR_REG);
	SPI_MasterSend(FOUR_BYTE_COUNT_MODE | ENABLE_COUNTING);
	PORTB = 0x01;
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_CNTR | CLR_REG);
	PORTB = 0x01;
}


// 전류센서 값을 ADC로 받아오는 함수
int getADC(char ch){

	ADMUX = (ADMUX & 0xf0) + ch;
	ADCSRA |= 0x40;
	while(!(ADCSRA & 0x10));
	return ADC;
}




ISR(USART0_RX_vect){

	g_buf[g_BufWriteCnt++] = UDR0;
}

//0.05s
void POSITION_CONTROL(){ // 위치 제어기
	// 위치 제어기
	g_Perr = g_Pdes - g_Pcur; // 위치 오차 계산
	
	// PD 제어 명령 생성
	g_pos_control = Kpp * g_Perr + Kdp * (g_Perr - g_Perr_prev)/(dt*100.); // Kpp: 위치 Kp, Kpd: 위치 Kd
	// D-term을 위한 오차 저장
	g_Perr_prev = g_Perr;
	
	//속도제한
	if(g_pos_control >= g_Vlimit)
	{
		g_pos_control = g_Vlimit;
	}
	else if(g_pos_control <= -g_Vlimit)
	{
		g_pos_control = -g_Vlimit;
	}

	if(g_Vlimit > 6140.*M_PI/30.)
	{
		g_Vlimit = (double)6140.*M_PI/30.;
	}
	else if(g_Vlimit < -6140.*M_PI/30.)
	{
		g_Vlimit = (double)-6140.*M_PI/30.;
	}
}

//0.005s
void VELOCITY_CONTROL(){ // 속도 제어기
	g_Vcur = (double)(g_Pcur - g_Pre_Pcur) / (dt*10.);
	g_Pre_Pcur = (double)g_Pcur;
	
	// 속도 제어기
	g_Verr = (double)(g_pos_control - g_Vcur); // 속도 오차 계산
	
	// 속도 제어 명령 생성 (PI 제어기)
	g_vel_control = (double)(Kpv * g_Verr + Kiv * g_Verr_sum * (dt*10.));
	g_Verr_sum += (double)g_Verr; // I-term 누적

	// I-term anti-windup (속도 제어 명령 제한)
	if(g_vel_control >= g_Climit)
	{
		g_Verr_sum -= (double)(g_vel_control - g_Climit)*Kav;
		g_vel_control = g_Climit;
	}
	else if(g_vel_control <= -g_Climit)
	{
		g_Verr_sum -= (double)(g_vel_control + g_Climit)*Kav;
		g_vel_control = -g_Climit;
	}
	
	if(g_Climit > 2.08)
	{
		g_Climit = 2.08;
	}
	else if(g_Climit < -2.08)
	{
		g_Climit = -2.08;
	}
}

//0.0005s
void CURRENT_CONTROL(){ //전류 제어기
	//전류 센서를 통하여 받아온 센서값을 고유 계산식을 이용하여 현재 전류값 계산
	g_Ccur = -( ((g_ADC / 1024. * 5.) - 2.5) * 10.);
	g_Cerr = (double)(g_vel_control - g_Ccur);

	// PI 제어기 // Kp gain,Ki gain 임의의 gain 값 수정해야함
	cur_control = (double)(g_Cerr * Kpc + g_Cerr_sum * Kic * dt); 
	cur_control += (double)g_Vcur * Kt; // 전향보상해주는 코드
	
	g_Cerr_sum += (double)g_Cerr;
	
	//I-term anti ([-24[v]~24[v]]
	if(cur_control >= 24){
		g_Cerr_sum -= (double)(cur_control - 24) * 1. / Kpc; // (1/Kpc = Kac)
		cur_control = 24.;
	}
	else if(cur_control <= -24){
		g_Cerr_sum -= (double)(cur_control + 24) * 1. / Kpc; // (1/Kpc = Kac)
		cur_control = -24.;
	}
}

//ISR(TIMER3_OVF_vect){
ISR(TIMER0_OVF_vect){
			
	TCNT0 = 256 - 125;
	//TCNT3 = 65536 - 125;		
	//Read LS7366
	int32_t cnt;
	
	PORTC = 0x01;
	
	g_ADC = getADC(0);
	
	PORTB = 0x00;
	SPI_MasterSend(SELECT_OTR | LOAD_REG);
	PORTB = 0x01;
			
	PORTB = 0x00;
	SPI_MasterSend(SELECT_OTR | RD_REG);
	cnt = SPI_MasterRecv();		cnt = cnt<< 8;
	cnt |= SPI_MasterRecv();	cnt = cnt<< 8;
	cnt |= SPI_MasterRecv();	cnt = cnt<< 8;
	cnt |= SPI_MasterRecv();
	PORTB = 0x01;
	g_Cnt = -cnt;
	
	PORTC = 0x03;
	
	g_Pcur = (double)(g_Cnt / (4096. * 81.)) * 2 * M_PI; // rad
	
	//TO DO
	// 위치제어기
	if((g_TimerCnt % 100) == 0){
	
		POSITION_CONTROL();	
		g_TimerCnt = 0;
		
	}
	// 속도제어기
	if((g_TimerCnt % 10) == 0){
		
		VELOCITY_CONTROL();
	}
	
	CURRENT_CONTROL();
	
	SetDutyCW(cur_control);
	
	/////////////////////////////////////////
	g_TimerCnt++;	
	g_SendFlag++;

}



int main(void){
	
	Packet_t packet;
	packet.data.header[0] = packet.data.header[1] = packet.data.header[2] = packet.data.header[3] = 0xFE;
	
	InitIO();
	
	//Uart
	InitUart0();
	
	//SPI
	InitSPI();
	
	//Timer
	InitTimer0();
	InitTimer1();
	InitTimer3();


	TCNT1 = TCNT3 = 0;
	SetDutyCW(0.);
	
	//ADC
	InitADC();
	
	//LS7366
	InitLS7366();
	
	//TCNT3 = 65536 - 125;
	TCNT0 = 256 - 125;
	sei();

	unsigned char check = 0;
	
    while (1) {
		for(;g_BufReadCnt != g_BufWriteCnt; g_BufReadCnt++){
			
			switch(g_PacketMode){
			case 0:
				
				if (g_buf[g_BufReadCnt] == 0xFF) {
					checkSize++;
					if (checkSize == 4) {
						g_PacketMode = 1;
					}
				}
				else {
					checkSize = 0;
				}
				break;
				
			case 1:

				g_PacketBuffer.buffer[checkSize++] = g_buf[g_BufReadCnt];
				
				if (checkSize == 8) {
					if(g_PacketBuffer.data.id == g_ID){

						g_PacketMode = 2;
					}
					else{
						g_PacketMode = 0;
						checkSize = 0;
					}
				}

				break;
			
			case 2:
				
				g_PacketBuffer.buffer[checkSize++] = g_buf[g_BufReadCnt];
				check += g_buf[g_BufReadCnt];
				
				if (checkSize == g_PacketBuffer.data.size) {

					if(check == g_PacketBuffer.data.check){

						switch(g_PacketBuffer.data.mode){

							case 2:
							g_Pdes = g_PacketBuffer.data.pos / 1000.;
							//g_Pdes += (360*DEG2RAD) * over_check;
							g_Vlimit = g_PacketBuffer.data.velo / 1000.;
							g_Climit = g_PacketBuffer.data.cur / 1000.;
							break;
							}
					}
					
					check = 0;
					g_PacketMode = 0;
					checkSize = 0;
				}
				else if(checkSize > g_PacketBuffer.data.size || checkSize > sizeof(Packet_t)) {
					TransUart0('f');
					check = 0;
					g_PacketMode = 0;
					checkSize = 0;
				}
			}
		}

		if(g_SendFlag > 19){
			g_SendFlag = 0;			
			
			packet.data.id = g_ID;
			packet.data.size = sizeof(Packet_data_t);
			packet.data.mode = 3;
			packet.data.check = 0;
			
			//packet.data.pos = g_Pdes * 1000; 
			//packet.data.velo = g_Vlimit * 1000;
			//packet.data.cur = g_Climit * 1000;  
			packet.data.pos = g_Pcur * 1000;
			packet.data.velo = g_Vcur * 1000;
			packet.data.cur = g_Ccur * 1000;
			
			for (int i = 8; i < sizeof(Packet_t); i++)
			packet.data.check += packet.buffer[i];
			
			for(int i=0; i<packet.data.size; i++){
				TransUart0(packet.buffer[i]);
			}
		}
	}
		
}