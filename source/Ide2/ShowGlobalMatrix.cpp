// ShowGlobalMatrix.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "ShowGlobalMatrix.h"
#include "supergridctrl.h"
#include "idemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CShowGlobalMatrix dialog

CShowGlobalMatrix::CShowGlobalMatrix(CWnd* pParent /*=NULL*/)
	: CDialog(CShowGlobalMatrix::IDD, pParent)
{

}

CShowGlobalMatrix::~CShowGlobalMatrix()
{
}

void CShowGlobalMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_FRAME, m_stFrame);
}


BEGIN_MESSAGE_MAP(CShowGlobalMatrix, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CShowGlobalMatrix message handlers
int CShowGlobalMatrix::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_sg.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_SHOWSELALWAYS, CRect(0,0,0,0),this,0x1002);
	m_sg.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_sg.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	return 0;
}

BOOL CShowGlobalMatrix::OnInitDialog() 
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

void CShowGlobalMatrix::OnOK ()
{
	/* recuperiamo i dati corretti */
    if (m_sg.GetResults (&m_strName))   {
		CDialog::OnOK ();
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOSHOWGLOBALVARS_1));
}

void CShowGlobalMatrix::GetResults (CString* strName)
{
    *strName=m_strName;
}

void CShowGlobalMatrix::SetResults (CString strName)
{
    m_strName=strName;
}
