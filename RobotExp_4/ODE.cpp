#include "stdafx.h"
#include "ODE.h"
#include "SystemMemory.h"
#include "DataType.h"
#include <iostream>

#ifndef DRAWSTUFF_TEXTURE_PATH
#define DRAWSTUFF_TEXTURE_PATH "./../ode-0.13/drawstuff/textures"
#endif

#define GRAVITY 9.81
#define MAX_JOINT_NUM 3		// �ִ� ����Ʈ ���� ����

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958

//ODE Global Variable
dsFunctions g_Fn;

static dWorldID g_World;
static dSpaceID g_Space;
static dJointGroupID g_Contactgroup;	// �浹�� ���õ� ����
static dGeomID g_Gound;					// �߷°� ���õ� ����

//My Robot Gloval Variable
Object g_oObj[MAX_JOINT_NUM + 1];
static dJointID g_oJoint[MAX_JOINT_NUM + 1];

double g_tar_q[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
double g_cur_q[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };

// ���: -180������ 180�� ���̷� ����ȭ�� ����
void joint_mode_angle(double &angle) {

	// -180���� 180 ���̷� ����
	while (angle > 180*DEG2RAD) {
		angle -= 360 * DEG2RAD;
	}
	while (angle < -180*DEG2RAD) {
		angle += 360 * DEG2RAD;
	}


}
//float mobile_mode_angle(float angle)
//{
	//// 360���� ���� ������ ���
	//angle = fmod(angle, 360.0f);

	//// -180���� 180 ���̷� ����
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
	dWorldSetGravity(g_World, 0, 0, -9.8);		// x,y,z �߷� ���� �� ũ�� ����
	dWorldSetERP(g_World, 1.0);
	dWorldSetCFM(g_World, 1e-5);
}



void RunODE(size_t width, size_t height) {

	//TO DO
	InitDrawStuff();							// ODE�� �����ϴ� �ùķ��̼� â�� �׷����� �ʱ�ȭ ���ִ� �Լ�
	InitODE();

	InitRobot();

	//run simulation
	dsSimulationLoop(0, 0, width, height, &g_Fn);	//ODE�� â�� ũ�⸦ ���� ���ִ� �Լ�
}



void ReleaseODE() {

	dJointGroupDestroy(g_Contactgroup);
	dSpaceDestroy(g_Space);
	dWorldDestroy(g_World);
	dCloseODE();
}



void StartDrawStuff() {							// �ִϸ��̼� ���� ������ �����ϴ� �Լ�

	//TO DO
	float dPos[3] = { 0.0, 0.0, 4.0 };			// X Y Z ��ǥ
	float dRot[3] = { 90.0, -90.0, 0.0 };		// X�� Y�� Z�� ���� ȸ�� // ������ ���� -90, 0, 0
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
	double dt = 0.1;				// ��ǰ� ���������� ���귮�� ������, dt��ŭ �ڵ带 �ѽ��� ������
	dWorldStep(g_World, dt);		// 0.1�� ���� ���� ������Ʈ �ݿ�

	dsSetColor(1, 0, 0);			// 1�� �ȼ��� 255�� �ǹ�
	dGeomCapsuleGetParams(g_oObj[0].geom, &radius, &length);	// ��ü ����
	// �Ķ���ͷ� ĸ���� ���� ����, ������, ���̿� ���� ������ �ѱ�
	dsDrawCapsuleD(dBodyGetPosition(g_oObj[0].body), // 0��° ��ũ ����
		dBodyGetRotation(g_oObj[0].body), 
		(float)length, (float)radius); // ��ü�� ���� �������� �Ķ���ͷ� �Ѱ���
	
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
   //Body ����
	dMass mass;
	dMatrix3 R;

	// �����߽���
	dReal x[MAX_JOINT_NUM] = { 0.0, 0.5, 1.5 };		 // ù ��°, �� ��°, �� ��° ��ũ�� ���� �߽���
	dReal y[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };	     // ���ηΰ� �ϳ��� x,y,z �߽�
	dReal z[MAX_JOINT_NUM] = { 0.5, 1.0, 1.0 };

	// ��ũ �ڼ�
	dReal ori_x[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal ori_y[MAX_JOINT_NUM] = { 0.0, 1.0, 1.0 };
	dReal ori_z[MAX_JOINT_NUM] = { 1.0, 0.0, 0.0 };
	dReal ori_q[MAX_JOINT_NUM] = { 0.0, 1.5708, 1.5708 };

	// �� ���� ��ũ ����
	dReal length[MAX_JOINT_NUM] = { 1.0, 1.0, 1.0 };

	// �� ���� ����
	dReal weight[MAX_JOINT_NUM] = { 0.5, 1.0, 0.5 };

	// ĸ���� ������
	dReal r[MAX_JOINT_NUM];
	for (int i = 0; i < MAX_JOINT_NUM; i++)
		r[i] = 0.125;

	// Body ����
	for (int i = 0; i < MAX_JOINT_NUM; i++)
	{
		g_oObj[i].body = dBodyCreate(g_World);
		dBodySetPosition(g_oObj[i].body, x[i], y[i], z[i]); // body�� ��ǥ ������ ����
		dMassSetZero(&mass);
		dMassSetCapsuleTotal(&mass, weight[i], 1, r[i], length[i]); // ĸ���� ����
		dBodySetMass(g_oObj[i].body, &mass);
		g_oObj[i].geom = dCreateCapsule(g_Space, r[i], length[i]);  // ĸ���� ����
		dGeomSetBody(g_oObj[i].geom, g_oObj[i].body);
		dRFromAxisAndAngle(R, ori_x[i], ori_y[i], ori_z[i], ori_q[i]); //x,y,z ���� �������� ������� ȸ���Ұ��� ����
		dBodySetRotation(g_oObj[i].body, R);
	}

	//�� ����Ʈ�� ȸ������ ��ġ
	dReal c_x[MAX_JOINT_NUM] = { 0.0, 0.0, 1.0 }; // 0,0,1�� ���⺤��
	dReal c_y[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal c_z[MAX_JOINT_NUM] = { 0.0, 1.0, 1.0 };

	//ȸ������ �����̼� ����
	dReal axis_x[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal axis_y[MAX_JOINT_NUM] = { 0.0, 0.0, 0.0 };
	dReal axis_z[MAX_JOINT_NUM] = { 1.0, 1.0, 1.0 };

	//���� �� ����
	g_oJoint[0] = dJointCreateFixed(g_World, 0);
	dJointAttach(g_oJoint[0], 0, g_oObj[0].body);
	dJointSetFixed(g_oJoint[0]);

	//����Ʈ ����
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

