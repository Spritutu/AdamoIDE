// AdamoRenameMachine.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRenameMachine.h"


// CAdamoRenameMachine dialog

IMPLEMENT_DYNAMIC(CAdamoRenameMachine, CDialog)

CAdamoRenameMachine::CAdamoRenameMachine(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoRenameMachine::IDD, pParent)
{

}

CAdamoRenameMachine::~CAdamoRenameMachine()
{
}

void CAdamoRenameMachine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdamoRenameMachine, CDialog)
END_MESSAGE_MAP()


// CAdamoRenameMachine message handlers

BOOL CAdamoRenameMachine::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem (IDC_ACTUAL_MACHINE_NAME)->SetWindowText (m_strActualMachine);
    m_editMachine.SubclassEdit(IDC_EDIT_NEW_MACHINE_NAME , this, PES_LETTERS | PES_UNDERLINE | PES_HEXNUMBERS | PES_NUMBERS);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoRenameMachine::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_editMachine.GetWindowText (m_strNewMachine);
	if (m_strNewMachine != "")
		CDialog::OnOK();
	else
		AfxMessageBox (LOADSTRING (IDS_MACHINENAME_MISSING));
}
