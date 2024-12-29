clc
clear all

%%Motor Spec
Va = 48;                    % Input Voltage
La = 0.658*10^-3;           % Inductance (H)
Ra = 1.76;                  % resister (ohm)
J_m = 99.5*10^-7;           % Rotor Inertia(Kgm^2)
t = 3.76*10^-3;             % time constant(s)
b = J_m/t;                  % 모터의 마찰계수 
Kt = 68.3*10^-3;            % 토크상수 % Nm/A
Ke = Kt;
Tl = 0; % 부하토크 0        % Load Torque

%%Gear Parameter( 무부하 시스템)
N1  = 1;                % ratio of gear1
N2  = 81;               % ratio of gear2
a   = 0.72;             % Max efficiency (기어의 효율)
n   = N1/N2;            % ratio of gear1,gear2

J_g = 5*10^-7;          % moment of inertia (gear)
J_eq = J_m + (1/a)*n^2*J_g; % inertia of geared motor
B_eq = J_eq/t;              % geared motor의 마찰계수

%%Gear Parameter( 부하 시스템 )
bar_m   = 0.175;        % bar mass
bar_L   = 0.300;        % bar length
bar_H   = 0.025;        % bar height
circular_m  = 0.340;    % circular mass
circular_R  = 0.050;    % circular radius
circular_L  = 0.300;    % circular length

J_bar       = 1/3*bar_m*(bar_L^2 + 1/4*bar_H^2);
J_circular  = 1/2*circular_m*circular_R^2 + circular_m*circular_L^2;

J_load      = J_bar + J_circular; % total load inertia
B_load      = J_load / t;

J_eq_1      = J_eq + 1/a*n^2*J_load;     % J_eq + J_load
B_eq_1      = J_eq_1 / t;

%% 전류 제어기(Voltage limit : -48~48V)
current_ref = 25;    % change variable 1 -> 5 -> 10 -> 25 [A]

PI = 3.14;          % define pi
fcc = 200;          % mcu 제어주기: 2000Hz -> fcc:200Hz
wcc = 2*PI*fcc;     % calculate cross angle frequency
Kp_c = La*wcc;      % calculate P gain
Ki_c = Ra*wcc;      % calculate I gain
Ka_c = 1/Kp_c;      % calculate anti-windup gain

%% 속도 제어기(Current limit : -27.3~27.3A)
speed_ref = 5;      % change variable 1 -> 2 -> 3 -> 5

wcs = wcc/10;       % calculate cross angle frequency
Kp_s = (1/n)*J_eq_1*wcs/Kt;     % calculate P gain
Ki_s = (1/n)*B_eq_1*wcs/Kt;     % calculate I gain
Ka_s = 1/Kp_s;      % calculate anti-windup gain

%% 위치 제어기
degree_ref = 90;      % change variable 1 -> 10 -> 45 -> 90

wcp = wcs/10;       % calculate cross angle frequency
Kp_p = wcp;         % calculate P gain
Kd_p = wcp/wcs;     % calculate D gain

%% Bring up result of Simulink
result = sim("HW4_Motor_Controller_2020741002_parkjeongho.slx");
sim_t = result.tout;

%% values from Current Control simulink              
current_measurement = result.current(:, 2);     
current_reference = result.current(:, 1);    

% Draw Figure - current reference, current measurement
figure('units','pixels' , 'pos',[0,200,500,500] , 'color',[1,1,1])          
% set pixel units, setting window size, background color

    plot(sim_t, current_reference,'--b', 'DisplayName', 'Reference Current','LineWidth', 1);
    hold on;
    plot(sim_t, current_measurement,'r', 'DisplayName', 'Measurement Current','LineWidth', 1);          
    xlim([0 (3/wcc)*3]);
    ylim([0 current_ref*1.2]);

    legend;         
    title('Current Controller' , 'fontsize',20);    % Title of figure 
    xlabel('t(sec)' , 'fontsize',15);               % Description of graph output's x-axis
    ylabel('current(A)' , 'fontsize',15);           % Description of graph output's y-axis

    % x축 tick 설정 (시정수 값의 3배 눈금으로 나타냄)
    x_tick_values = [get(gca, 'XTick'), 3/wcc];
    x_tick_values_sorted = sort(x_tick_values);

    xticks(x_tick_values_sorted);  

    % y축 tick 설정 (Referenc 값의 95%를 눈금으로 나타냄)
    y_tick_95_percent = 0.95 * current_ref;
    y_tick_values = [get(gca, 'YTick'), y_tick_95_percent]; 
    y_tick_values_sorted = sort(y_tick_values); 

    yticks(y_tick_values_sorted);
    grid on; %격자 생성

%% values from Speed Control simulink               
speed_measurement = result.speed(:,2);      
speed_reference = result.speed(:,1); 

% Draw Figure - speed reference, speed measurement
figure('units','pixels' , 'pos',[500,200,500,500] , 'color',[1,1,1]) 
% set pixel units, setting window size, background color

    plot(sim_t, speed_reference, '--b', 'DisplayName', 'Reference Speed', 'LineWidth', 1); 
    hold on; 
    plot(sim_t, speed_measurement, 'r', 'DisplayName', 'Measurement Speed', 'LineWidth', 1); 

    xlim([0 (3/wcs)*3]); 
    ylim([0 speed_ref*1.2])

    title('Speed Controller', 'fontsize',20); 
    xlabel('t(sec)', 'fontsize',15); 
    ylabel('Speed(rad/s)', 'fontsize',15); 
    legend; 

    % x축 tick 설정
    x_tick_values = [get(gca, 'XTick'), 3/wcs];
    x_tick_values_sorted = sort(x_tick_values);

    xticks(x_tick_values_sorted);

    % y축 tick 설정
    y_tick_95_percent = 0.95 * speed_ref;
    y_tick_values = [get(gca, 'YTick'), y_tick_95_percent]; 
    y_tick_values_sorted = sort(y_tick_values);

    yticks(y_tick_values_sorted);
    grid on; %격자 생성


%% values from Position Control simulink         
degree_measurement = result.degree(:,2);      
degree_reference = result.degree(:,1); 

% Draw Figure - degree reference, degree measurement
figure('units','pixels' , 'pos',[1000,200,500,500] , 'color',[1,1,1])         
% set pixel units, setting window size, background color

    plot(sim_t, degree_reference, '--b', 'DisplayName', 'Reference Position', 'LineWidth', 1);
    hold on;
    plot(sim_t, degree_measurement, 'r', 'DisplayName', 'Measurement Position', 'LineWidth', 1); 

    xlim([0 (3/wcp)*3]); 
    ylim([0 degree_ref*1.2])

    title('Position Controller', 'fontsize',20);
    xlabel('t(sec)', 'fontsize',15); 
    ylabel('Position(degree)', 'fontsize',15); 
    legend; 

    % x축 tick 설정
    x_tick_values = [get(gca, 'XTick'), 3/wcp];
    x_tick_values_sorted = sort(x_tick_values);

    xticks(x_tick_values_sorted);

    % y축 tick 설정
    y_tick_95_percent = 0.95 * degree_ref;
    y_tick_values = [get(gca, 'YTick'), y_tick_95_percent]; 
    y_tick_values_sorted = sort(y_tick_values); 

    yticks(y_tick_values_sorted);
    grid on; 
