// AdamoRenameMachine.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoCopyPlant.h"


// CAdamoCopyPlant dialog

IMPLEMENT_DYNAMIC(CAdamoCopyPlant, CDialog)

CAdamoCopyPlant::CAdamoCopyPlant(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoCopyPlant::IDD, pParent)
{

}

CAdamoCopyPlant::~CAdamoCopyPlant()
{
}

void CAdamoCopyPlant::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdamoCopyPlant, CDialog)
END_MESSAGE_MAP()


// CAdamoCopyPlant message handlers

BOOL CAdamoCopyPlant::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
    m_editPlant.SubclassEdit(IDC_EDIT_COPY_PLANT , this, PES_LETTERS | PES_UNDERLINE | PES_HEXNUMBERS | PES_NUMBERS);
    m_editMachine.SubclassEdit(IDC_EDIT_COPY_MACHINE, this, PES_LETTERS | PES_UNDERLINE | PES_HEXNUMBERS | PES_NUMBERS);
	m_editPlant.SetWindowText (m_strPlant);
	m_editMachine.SetWindowText (m_strMachine);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoCopyPlant::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_editPlant.GetWindowText (m_strPlant);
	m_editMachine.GetWindowText (m_strMachine);
	if (m_strPlant != "" && m_strMachine != "")
		CDialog::OnOK();
	else
		if (m_strPlant == "")
			AfxMessageBox (LOADSTRING (IDS_PLANTNAME_MISSING));
		else
			AfxMessageBox (LOADSTRING (IDS_MACHINENAME_MISSING));
}
