// AdamoRSValues.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSValues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSValues dialog


CAdamoRSValues::CAdamoRSValues(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoRSValues::IDD, pParent), m_bSelected (false)
{
	//{{AFX_DATA_INIT(CAdamoRSValues)
	//}}AFX_DATA_INIT
}


void CAdamoRSValues::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoRSValues)
	DDX_Control(pDX, IDC_CHECK_RS_FORCE_VALUE, m_checkEnableValue);
	DDX_Control(pDX, IDC_RADIO_RS_SELECTED, m_radioSelected);
	DDX_Control(pDX, IDC_RADIO_RS_NOT_SELECTED, m_radioNotSelected);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoRSValues, CDialog)
	//{{AFX_MSG_MAP(CAdamoRSValues)
	ON_BN_CLICKED(IDC_CHECK_RS_FORCE_VALUE, OnCheckRsForceValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSValues message handlers

BOOL CAdamoRSValues::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    GetDlgItem (IDC_EDIT_RS_VALUE)->EnableWindow (FALSE);
    m_editOption.SubclassEdit(IDC_EDIT_RS_OPTIONS, this, PES_LETTERS|PES_HEXNUMBERS);
    m_editValue.SubclassEdit(IDC_EDIT_RS_VALUE, this, PES_LETTERS|PES_HEXNUMBERS);
    m_radioNotSelected.SetCheck (TRUE);
    m_editOption.SetWindowText (m_strOption);
    m_editValue.SetWindowText (m_strValue);
    m_bSelected ? m_radioSelected.SetCheck (TRUE) : m_radioNotSelected.SetCheck (TRUE);
    !m_bSelected ? m_radioSelected.SetCheck (FALSE) : m_radioNotSelected.SetCheck (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoRSValues::OnCheckRsForceValue() 
{
	// TODO: Add your control notification handler code here
    GetDlgItem (IDC_EDIT_RS_VALUE)->EnableWindow (m_checkEnableValue.GetCheck ());
}

void CAdamoRSValues::OnOK() 
{
	// TODO: Add extra validation here
    m_editOption.GetWindowText (m_strOption);
    m_editValue.GetWindowText (m_strValue);
    m_strOption.TrimLeft(); m_strOption.TrimRight ();
    m_strValue.TrimLeft(); m_strValue.TrimRight ();
    m_bSelected=m_radioSelected.GetCheck ()!=0;
    if (m_strOption!="")
	    CDialog::OnOK();
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMORSVALUES_1), MB_ICONSTOP);
}
