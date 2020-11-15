// UserStrings.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "UserStrings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserStrings dialog


CUserStrings::CUserStrings(CWnd* pParent /*=NULL*/)
	: CDialog(CUserStrings::IDD, pParent), m_bModify (false)
{
	//{{AFX_DATA_INIT(CUserStrings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUserStrings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserStrings)
    /*
	DDX_Control(pDX, IDC_EDITDESCRIZIONE, m_strDescrizione);
	DDX_Control(pDX, IDC_EDITID, m_strID);
	DDX_Control(pDX, IDC_EDITMESSAGGIO, m_strMessaggio);
	DDX_Control(pDX, IDC_EDITVALORE, m_strValore);
    */
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserStrings, CDialog)
	//{{AFX_MSG_MAP(CUserStrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserStrings message handlers

BOOL CUserStrings::OnInitDialog()
{
	CDialog::OnInitDialog();
    CenterWindow ();
	m_strDescrizione.SubclassEdit (IDC_EDITDESCRIZIONE, this, PES_NUMBERS|PES_HEXNUMBERS|PES_LETTERS|PES_BLANK|PES_ALL);
	m_strID.SubclassEdit (IDC_EDITID, this, PES_NUMBERS|PES_HEXNUMBERS|PES_LETTERS|PES_UNDERLINE);
	m_strMessaggio.SubclassEdit (IDC_EDITMESSAGGIO, this, PES_NUMBERS|PES_HEXNUMBERS|PES_LETTERS|PES_BLANK|PES_ALL);
	m_strValore.SubclassEdit (IDC_EDITVALORE, this, PES_NUMBERS);
    m_strID.SetWindowText (m_strValoreID);
    m_strValore.SetWindowText (m_strValoreNumero);
    m_strMessaggio.SetWindowText (m_strValoreMessaggio);
    m_strDescrizione.SetWindowText (m_strValoreDescrizione);
    if (m_bModify)   {
        m_strID.EnableWindow (FALSE);
        m_strValore.EnableWindow (FALSE);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserStrings::OnOK ()
{
    CString str;
    m_strValore.GetWindowText (str);
    if (!str.IsEmpty())   {
        m_strID.GetWindowText (m_strValoreID);
        m_strValore.GetWindowText (m_strValoreNumero);    
        m_strMessaggio.GetWindowText (m_strValoreMessaggio);    
        m_strDescrizione.GetWindowText (m_strValoreDescrizione);
        CDialog::OnOK ();
    }
    else   {
        AfxMessageBox(LOADSTRING (IDS_ADAMOUSERSTRINGS_1));
        m_strValore.SetFocus ();
    }
}
