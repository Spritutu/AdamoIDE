// ShowGlobalVars.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "showglobalvars.h"
#include "supergridctrl.h"
#include "idemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowGlobalVars dialog


CShowGlobalVars::CShowGlobalVars(CWnd* pParent /*=NULL*/)
	: CDialog(CShowGlobalVars::IDD, pParent), m_nRow (-1)
{
	//{{AFX_DATA_INIT(CShowGlobalVars)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowGlobalVars::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowGlobalVars)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_STATIC_FRAME, m_stFrame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowGlobalVars, CDialog)
	//{{AFX_MSG_MAP(CShowGlobalVars)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_BTN_CLOSEDIALOG, OnBtnCloseDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowGlobalVars message handlers

int CShowGlobalVars::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_sg.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_SHOWSELALWAYS, CRect(0,0,0,0),this,0x1002);
	m_sg.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_sg.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	return 0;
}

BOOL CShowGlobalVars::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rc;
	m_stFrame.GetWindowRect(rc);
	ScreenToClient(rc);
	m_stFrame.ShowWindow(SW_HIDE);
	if (m_sg.GetSafeHwnd())
	    m_sg.MoveWindow(rc);
	m_sg.InitializeGrid ();
	return TRUE;
}

void CShowGlobalVars::OnOK ()
{
	EndDialog (0);
}

void CShowGlobalVars::OnBtnCloseDialog() 
{
	/* recuperiamo i dati corretti */
    if (m_sg.GetResults (&m_strName, &m_nRow, &m_strCol, &m_nType))   {
        EndDialog (0);
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOSHOWGLOBALVARS_1));
}

void CShowGlobalVars::GetResults (CString* strName, int *nRow, CString *strColumn, int *nType)
{
    *nType=m_nType;
    *strName=m_strName;
    *nRow=m_nRow;
    *strColumn=m_strCol;
}

void CShowGlobalVars::SetResults (CString strName, int nRow, CString strCol, int nType)
{
    m_nType=nType;
    m_strName=strName;
    m_nRow=nRow;
    m_strCol=strCol;
}
