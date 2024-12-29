#pragma once
#include "NTGraph.h"
#include "SystemMemory.h"
#include "SharedMemory.h"
#include "DataType.h"


// CGraphDlg 대화 상자입니다.
#define RED       RGB(127, 0, 0)
#define GREEN     RGB(0, 127, 0)
#define BLUE      RGB(0, 0, 127)
#define BLACK     RGB(0, 0, 0)
#define WHITE     RGB(255, 255, 255)
#define GRAY      RGB(192, 192, 192)

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGraphDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void InitNTGraph();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CNTGraph m_ntgPos;
	CNTGraph m_ntgVel;
	CNTGraph m_ntgTorque;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	double m_dCnt = 0.0;
};
