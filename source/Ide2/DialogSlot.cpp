// DialogSlot.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "DialogSlot.h"
#include "mainframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSlot dialog


CDialogSlot::CDialogSlot(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSlot::IDD, pParent)   {
	//{{AFX_DATA_INIT(CDialogSlot)
	m_szSlotName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSlot)
	DDX_Text(pDX, IDC_EDITSLOT, m_szSlotName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSlot, CDialog)
	//{{AFX_MSG_MAP(CDialogSlot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSlot message handlers

BOOL CDialogSlot::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    GetDlgItem (IDC_EDITSLOT)->SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFontBold ());
    GetDlgItem (IDC_SLOT)->SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
