// GraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RobotExp_4.h"
#include "GraphDlg.h"
#include "afxdialogex.h"
#include "SystemMemory.h"
#include "DataType.h"

// CGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NTGRAPH_POS, m_ntgPos);
	DDX_Control(pDX, IDC_NTGRAPH_VEL, m_ntgVel);
	DDX_Control(pDX, IDC_NTGRAPH_TORQ, m_ntgTorque);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitNTGraph();

	return TRUE;	//return True unless you set the focus to a control
					//예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// CGraphDlg 메시지 처리기입니다.
void CGraphDlg::InitNTGraph()
{
	m_ntgPos.ClearGraph();
	m_ntgVel.ClearGraph();
	m_ntgTorque.ClearGraph();

	m_ntgPos.SetFrameStyle(0);
	m_ntgVel.SetFrameStyle(0);
	m_ntgTorque.SetFrameStyle(0);

	m_ntgPos.SetPlotAreaColor(WHITE);
	m_ntgVel.SetPlotAreaColor(WHITE);
	m_ntgTorque.SetPlotAreaColor(WHITE);

	m_ntgPos.SetShowGrid(TRUE);
	m_ntgVel.SetShowGrid(TRUE);
	m_ntgTorque.SetShowGrid(TRUE);

	m_ntgPos.SetFormatAxisBottom(_T("%.2f"));
	m_ntgVel.SetFormatAxisBottom(_T("%.2f"));
	m_ntgTorque.SetFormatAxisBottom(_T("%.2f"));

	m_ntgPos.SetCaption(_T("위치"));
	m_ntgVel.SetCaption(_T("속도"));
	m_ntgTorque.SetCaption(_T("토크"));

	m_ntgPos.SetXLabel(_T("Time[s]"));
	m_ntgVel.SetXLabel(_T("Time[s]"));
	m_ntgTorque.SetXLabel(_T("Time[s]"));

	m_ntgPos.SetYLabel(_T("Degree[deg]"));
	m_ntgVel.SetYLabel(_T("Velocity[deg/s]"));
	m_ntgTorque.SetYLabel(_T("Torque[Nm]"));

	m_ntgPos.AddElement();
	m_ntgPos.SetElementWidth(3);
	m_ntgPos.SetElementLineColor(RED); // Target
	m_ntgPos.AddElement();
	m_ntgPos.SetElementWidth(3);
	m_ntgPos.SetElementLineColor(BLUE); // Current

	m_ntgPos.SetRange(0.0, 10.0, -360.0, 360.0);
	m_ntgPos.SetYGridNumber(4);

	m_ntgVel.AddElement();
	m_ntgVel.SetElementWidth(4);
	m_ntgVel.SetElementLineColor(RED); // Target
	m_ntgVel.AddElement();
	m_ntgVel.SetElementWidth(3);
	m_ntgVel.SetElementLineColor(BLUE); // Velocity

	m_ntgVel.SetRange(0.0, 10.0, -30.0, 30.0);
	m_ntgVel.SetYGridNumber(4);

	m_ntgTorque.AddElement();
	m_ntgTorque.SetElementWidth(4);
	m_ntgTorque.SetElementLineColor(RED); // Target
	m_ntgTorque.AddElement();
	m_ntgTorque.SetElementWidth(3);
	m_ntgTorque.SetElementLineColor(BLUE); // Torque

	m_ntgTorque.SetRange(0.0, 10.0, -0.5, 0.5);
	m_ntgTorque.SetYGridNumber(4);

	SetTimer(1, 100, NULL);
}



void CGraphDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_dCnt += 0.1;
	DataType_t jointData;
	ControlData_t graphData;
	double tar_ode_angle;
	GET_SYSTEM_MEMORY("JointData", jointData);
	//GET_SYSTEM_MEMORY("VelData", );

	if (m_dCnt >= 10.0)
	{
		m_ntgPos.SetRange(m_dCnt - 10.0, m_dCnt, -360.0, 360.0);
		m_ntgVel.SetRange(m_dCnt - 10.0, m_dCnt, -30.0, 30.0);
		m_ntgTorque.SetRange(m_dCnt - 10.0, m_dCnt, -0.5, 0.5);
	}

	//if (jointData.Q_tar[0] * RAD2DEG < 0)
	//	tar_ode_angle = jointData.Q_tar[0] * RAD2DEG;
	//else
	//	tar_ode_angle = jointData.Q_tar[0] * RAD2DEG;


	m_ntgPos.PlotXY(m_dCnt, jointData.Q_tar[0] * RAD2DEG, 1);
	m_ntgPos.PlotXY(m_dCnt, CurPos, 2);

	m_ntgVel.PlotXY(m_dCnt, TarVelo, 1);
	m_ntgVel.PlotXY(m_dCnt, CurVelo, 2);

	m_ntgTorque.PlotXY(m_dCnt, TarTorq, 1);
	m_ntgTorque.PlotXY(m_dCnt, CurTorq, 2);

	CDialogEx::OnTimer(nIDEvent);
}
