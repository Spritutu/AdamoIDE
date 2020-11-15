// NewPlant.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "NewPlant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPlant dialog


CNewPlant::CNewPlant(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPlant::IDD, pParent)
{
}


void CNewPlant::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewPlant, CDialog)
	//{{AFX_MSG_MAP(CNewPlant)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPlant message handlers
BOOL CNewPlant::OnInitDialog()
{
	CDialog::OnInitDialog();
    CenterWindow ();
	m_ctrlName.SubclassEdit (IDC_PLANT_NAME, this, PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE);
	m_ctrlDesc.SubclassEdit (IDC_PLANT_DESC, this, PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE|PES_BLANK);
	if (m_strName != "")
		m_ctrlName.EnableWindow (FALSE);
	m_ctrlName.SetWindowText (m_strName);
	m_ctrlDesc.SetWindowText (m_strDesc);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnOK : chiamata sul tasto di OK.
*/
void CNewPlant::OnOK ()
{
    GetDlgItemText (IDC_PLANT_NAME, m_strName); 
    GetDlgItemText (IDC_PLANT_DESC, m_strDesc);
    m_strName.TrimLeft (); m_strName.TrimRight ();
    m_strDesc.TrimLeft (); m_strDesc.TrimRight ();
    CDialog::OnOK ();
}
