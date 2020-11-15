// OscProp.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "OscProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COscProp dialog

extern struct    {
    char* pVettStr;
    int   nValue;
} stFreq[NMAXFREQUENCY];

extern struct    {
    char* pVettStr;
    int   nValue;
} stOscBuf[NMAXOSCBUFFER];

COscProp::COscProp(CWnd* pParent /*=NULL*/)
	: CDialog(COscProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(COscProp)
	m_nMode = -1;
	m_nTriggerAnd = -1;
	//}}AFX_DATA_INIT
}


void COscProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COscProp)
	DDX_Radio(pDX, IDC_MODECONTINOUS, m_nMode);
	DDX_Radio(pDX, IDC_TRIGGERAND, m_nTriggerAnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COscProp, CDialog)
	//{{AFX_MSG_MAP(COscProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COscProp message handlers

BOOL COscProp::OnInitDialog() 
{
    int i;
	CDialog::OnInitDialog();
    for (i=0; i<NMAXFREQUENCY; i++)
        ((CComboBox*)GetDlgItem (IDC_COMBOFREQUENCY))->AddString (stFreq[i].pVettStr);
    ((CComboBox*)GetDlgItem (IDC_COMBOFREQUENCY))->SetCurSel (m_nFrequency);
    for (i=0; i<NMAXOSCBUFFER; i++)
        ((CComboBox*)GetDlgItem (IDC_COMBOBUFFER))->AddString (stOscBuf[i].pVettStr);
    ((CComboBox*)GetDlgItem (IDC_COMBOBUFFER))->SetCurSel (m_nDimBuffer);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COscProp::OnOK() 
{
    m_nFrequency=((CComboBox*)GetDlgItem (IDC_COMBOFREQUENCY))->GetCurSel ();
    m_nDimBuffer=((CComboBox*)GetDlgItem (IDC_COMBOBUFFER))->GetCurSel ();
    CDialog::OnOK();
}
