#include "stdafx.h"
#include "ODE.h"
#include "SystemMemory.h"
#include "DataType.h"
#include <iostream>

#ifndef DRAWSTUFF_TEXTURE_PATH
#define DRAWSTUFF_TEXTURE_PATH "./../ode-0.13/drawstuff/textures"
#endif

#define GRAVITY 9.81
#define MAX_JOINT_NUM 3		// 최대 조인트 갯수 설정

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958

//ODE Global Variable
dsFunctions g_Fn;

static dWorldID g_World;
static dSpaceID g_Space;
static dJointGroupID g_Contactgroup;	// 충돌과 관련된 변수
static dGeomID g_Gound;					// 중력과 관련된 변수

//My Robot Gloval Variable
Object g_oObj[MAX_JOINT_NUM + 1];
static dJointID g_oJoint[MAX_JOINT_NUM + 1];

double g_tar_q[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
double g_cur_q[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };

// 출력: -180도에서 180도 사이로 정규화된 각도
void joint_mode_angle(double &angle) {

	// -180에서 180 사이로 조정
	while (angle > 180*DEG2RAD) {
		angle -= 360 * DEG2RAD;
	}
	while (angle < -180*DEG2RAD) {
		angle += 360 * DEG2RAD;
	}


}
//float mobile_mode_angle(float angle)
//{
	//// 360으로 나눈 나머지 계산
	//angle = fmod(angle, 360.0f);

	//// -180에서 180 사이로 조정
	//if (angle > 180.0f) {
	//	angle -= 360.0f;
	//}
	//else if (angle < -180.0f) {
	//	angle += 360.0f;
	//}

	//return angle;
//}

void InitDrawStuff() {

	g_Fn.version = DS_VERSION;
	g_Fn.start = &StartDrawStuff;
	g_Fn.step = &SimLoopDrawStuff;
	g_Fn.command = &CommandDrawStuff;
	g_Fn.stop = StopDrawStuff;
	g_Fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;
}


void InitODE() {

	//TO DO
	//setDrawStuff
	dInitODE();
	g_World = dWorldCreate();
	g_Space = dHashSpaceCreate(0);
	g_Contactgroup = dJointGroupCreate(0);
	g_Gound = dCreatePlane(g_Space, 0, 0, 1, 0);
	dWorldSetGravity(g_World, 0, 0, -9.8);		// x,y,z 중력 방향 및 크기 결정
	dWorldSetERP(g_World, 1.0);
	dWorldSetCFM(g_World, 1e-5);
}



void RunODE(size_t width, size_t height) {

	//TO DO
	InitDrawStuff();							// ODE를 실행하는 시뮬레이션 창에 그래픽을 초기화 해주는 함수
	InitODE();

	InitRobot();

	//run simulation
	dsSimulationLoop(0, 0, width, height, &g_Fn);	//ODE의 창의 크기를 조절 해주는 함수
}



void ReleaseODE() {

	dJointGroupDestroy(g_Contactgroup);
	dSpaceDestroy(g_Space);
	dWorldDestroy(g_World);
	dCloseODE();
}



void StartDrawStuff() {							// 애니메이션 시작 시점을 설정하는 함수

	//TO DO
	float dPos[3] = { 0.0, 0.0, 4.0 };			// X Y Z 좌표
	float dRot[3] = { 90.0, -90.0, 0.0 };		// X축 Y축 Z축 기준 회전 // 조교님 설정 -90, 0, 0
	dsSetViewpoint( dPos, dRot );

}


void SimLoopDrawStuff(int pause) 
{

	//TO DO
	//
	//Robot Contorl
	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);
	g_tar_q[1] = jointData.Q_tar[0];
	g_tar_q[2] = jointData.Q_tar[1];

	joint_mode_angle(g_tar_q[1]);
	joint_mode_angle(g_tar_q[2]);

	jointData.Q_cur[0] = g_cur_q[1];
	jointData.Q_cur[1] = g_cur_q[2];
	SET_SYSTEM_MEMORY("JointData", jointData);
	if (g_tar_q[0] >= 360.0 * DEG2RAD) g_tar_q[0] -= 360.0*DEG2RAD;
	if (g_tar_q[0] <= -360.0 * DEG2RAD) g_tar_q[0] += 360.0*DEG2RAD;

	PControl();

	dReal length, radius;

	//Close Loop Setting
	double dt = 0.1;				// 사실과 같아지지만 연산량이 많아짐, dt만큼 코드를 한스텝 돌려라
	dWorldStep(g_World, dt);		// 0.1초 마다 원드 업데이트 반영

	dsSetColor(1, 0, 0);			// 1은 픽셀값 255를 의미
	dGeomCapsuleGetParams(g_oObj[0].geom, &radius, &length);	// 강체 생성
	// 파라미터로 캡슐에 대한 정보, 반지름, 길이에 대한 정보를 넘김
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[0].body), // 0번째 링크 설정
		dBodyGetRotation(g_oObj[0].body), 
		(float)length, (float)radius); // 강체에 대한 정보들을 파라미터로 넘겨줌
	
	dsSetColor(0, 1, 0);
	dGeomCapsuleGetParams(g_oObj[1].geom, &radius, &length);
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[1].body),
		dBodyGetRotation(g_oObj[1].body),
		(float)length, (float)radius);
	
	dsSetColor(0, 0, 1);
	dGeomCapsuleGetParams(g_oObj[2].geom, &radius, &length);
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[2].body),
		dBodyGetRotation(g_oObj[2].body),
		(float)length, (float)radius);
}



void CommandDrawStuff(int cmd) {

	//TO DO

}



void StopDrawStuff() {

	//TO DO

}


void InitRobot()
{

	//TO DO
   //Body 설정
	dMass mass;
	dMatrix3 R;

	// 질량중심점
	dReal x[MAX_JOINT_NUM] = { 0.0, 0.5, 1.5 };		 // 첫 번째, 두 번째, 세 번째 링크의 질량 중심점
	dReal y[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };	     // 세로로가 하나의 x,y,z 중심
	dReal z[MAX_JOINT_NUM] = { 0.5, 1.0, 1.0 };

	// 링크 자세
	dReal ori_x[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal ori_y[MAX_JOINT_NUM] = { 0.0, 1.0, 1.0 };
	dReal ori_z[MAX_JOINT_NUM] = { 1.0, 0.0, 0.0 };
	dReal ori_q[MAX_JOINT_NUM] = { 0.0, 1.5708, 1.5708 };

	// 한 덩이 링크 길이
	dReal length[MAX_JOINT_NUM] = { 1.0, 1.0, 1.0 };

	// 한 덩이 무게
	dReal weight[MAX_JOINT_NUM] = { 0.5, 1.0, 0.5 };

	// 캡슐의 반지름
	dReal r[MAX_JOINT_NUM];
	for (int i = 0; i < MAX_JOINT_NUM; i++)
		r[i] = 0.125;

	// Body 생성
	for (int i = 0; i < MAX_JOINT_NUM; i++)
	{
		g_oObj[i].body = dBodyCreate(g_World);
		dBodySetPosition(g_oObj[i].body, x[i], y[i], z[i]); // body를 좌표 공간에 놓음
		dMassSetZero(&mass);
		dMassSetCapsuleTotal(&mass, weight[i], 1, r[i], length[i]); // 캡슐을 만듬
		dBodySetMass(g_oObj[i].body, &mass);
		g_oObj[i].geom = dCreateCapsule(g_Space, r[i], length[i]);  // 캡슐을 생성
		dGeomSetBody(g_oObj[i].geom, g_oObj[i].body);
		dRFromAxisAndAngle(R, ori_x[i], ori_y[i], ori_z[i], ori_q[i]); //x,y,z 축을 기준으로 어느정도 회전할건지 결정
		dBodySetRotation(g_oObj[i].body, R);
	}

	//각 조인트의 회전축의 위치
	dReal c_x[MAX_JOINT_NUM] = { 0.0, 0.0, 1.0 }; // 0,0,1의 방향벡터
	dReal c_y[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal c_z[MAX_JOINT_NUM] = { 0.0, 1.0, 1.0 };

	//회전축의 로테이션 방향
	dReal axis_x[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal axis_y[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal axis_z[MAX_JOINT_NUM] = { 1.0, 1.0, 1.0 };

	//고정 축 설정
	g_oJoint[0] = dJointCreateFixed(g_World, 0);
	dJointAttach(g_oJoint[0], 0, g_oObj[0].body);
	dJointSetFixed(g_oJoint[0]);

	//조인트 설정
	for (int i = 1; i < MAX_JOINT_NUM; i++)
	{
		g_oJoint[i] = dJointCreateHinge(g_World, 0);
		dJointAttach(g_oJoint[i], g_oObj[i].body, g_oObj[i - 1].body);
		dJointSetHingeAnchor(g_oJoint[i], c_x[i], c_y[i], c_z[i]);
		dJointSetHingeAxis(g_oJoint[i], axis_x[i], axis_y[i], axis_z[i]);
	}

}

void PControl()
{
	//TO DO
	dReal Kp = 1.0, fMax = 5000.0;
	dReal a_error_q[MAX_JOINT_NUM];

	for (int i = 1; i < MAX_JOINT_NUM; i++)
	{
		g_cur_q[i] = dJointGetHingeAngle(g_oJoint[i]);
		a_error_q[i] = g_tar_q[i] - g_cur_q[i];
		dJointSetHingeParam(g_oJoint[i], dParamVel, Kp*a_error_q[i]);
		dJointSetHingeParam(g_oJoint[i], dParamFMax, fMax);
	}
}

