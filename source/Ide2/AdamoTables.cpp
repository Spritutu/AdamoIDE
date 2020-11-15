// AdamoTables.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "adamotables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoTables dialog


CAdamoTables::CAdamoTables(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoTables::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoTables)
	//}}AFX_DATA_INIT
}


void CAdamoTables::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoTables)
	DDX_Control(pDX, IDC_STNAME, m_stName);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_stFrame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoTables, CDialog)
	//{{AFX_MSG_MAP(CAdamoTables)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BTN_CLOSEDIALOG, OnBtnCloseDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoTables message handlers

int CAdamoTables::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndReport.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0), this, ID_REPCTRL_ADAMOTABLES);	
	return 0;
}

BOOL CAdamoTables::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rc;
	m_stFrame.GetWindowRect(rc);
	ScreenToClient(rc);
	m_stFrame.ShowWindow(SW_HIDE);
	if (m_wndReport.GetSafeHwnd())
		m_wndReport.MoveWindow(rc);
	m_wndReport.AddVariable (m_strVariableName, m_nGL, m_nLevel, m_nProg);
	m_wndReport.InitializeGrid ();
    SetWindowText (m_strCaption);
    m_stName.SetWindowText (m_strVariableName);
    m_nTimer=SetTimer (ID_TABLESTIMER, 500, NULL);
	return TRUE;
}

void CAdamoTables::OnDestroy() 
{
	CDialog::OnDestroy();
	
    KillTimer (m_nTimer);
}

void CAdamoTables::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_wndReport.RinfrescaInformazioni ();
	CDialog::OnTimer(nIDEvent);
}

void CAdamoTables::OnOK ()
{
}

void CAdamoTables::OnBtnCloseDialog() 
{
	// TODO: Add your control notification handler code here
    EndDialog (0);	
}
