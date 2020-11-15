// AdamoRSEditTableIndex.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "adamorsedittableindex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSEditTableIndex dialog


CAdamoRSEditTableIndex::CAdamoRSEditTableIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoRSEditTableIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoRSEditTableIndex)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoRSEditTableIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoRSEditTableIndex)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_STATIC_FRAME, m_stFrame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoRSEditTableIndex, CDialog)
	//{{AFX_MSG_MAP(CAdamoRSEditTableIndex)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_BTN_CLOSEDIALOG, OnBtnCloseDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSEditTableIndex message handlers

int CAdamoRSEditTableIndex::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_eisg.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0),this,0x1001);	
	m_eisg.SetExtendedStyle(LVS_SHOWSELALWAYS|LVS_EX_GRIDLINES);
	m_eisg.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	return 0;
}

BOOL CAdamoRSEditTableIndex::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rc;
	m_stFrame.GetWindowRect(rc);
	ScreenToClient(rc);
	m_stFrame.ShowWindow(SW_HIDE);
	if (m_eisg.GetSafeHwnd())
        m_eisg.MoveWindow(rc);
    m_eisg.SetDBData (m_strTable, m_strIndex, m_strIndexValue);
	m_eisg.InitializeGrid ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnOK :
*/
void CAdamoRSEditTableIndex::OnOK ()
{
}

/*
** OnBtnCloseDialog :
*/
void CAdamoRSEditTableIndex::OnBtnCloseDialog() 
{
    EndDialog (IDOK);	
}
