// DlgIDRange.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgIDRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIDRange dialog


CDlgIDRange::CDlgIDRange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIDRange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIDRange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgIDRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIDRange)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIDRange, CDialog)
	//{{AFX_MSG_MAP(CDlgIDRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIDRange message handlers

BOOL CDlgIDRange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_edtDaID.SubclassEdit(IDC_EDITDAIDERROR, this, PES_NUMBERS);
	m_edtAID.SubclassEdit(IDC_EDITAIDERROR, this, PES_NUMBERS);
    CenterWindow ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIDRange::OnOK ()
{
    CString strDa, strA;
    m_edtDaID.GetWindowText (strDa);
    m_edtAID.GetWindowText (strA);
    strDa.TrimLeft(); strDa.TrimRight ();
    strA.TrimLeft(); strA.TrimRight ();
    if (strDa==""||strA=="")
        AfxMessageBox (LOADSTRING (IDS_ADAMODLGIDRANGE_1), MB_ICONSTOP);
    else   {
        nDa=atoi (strDa);
        nA=atoi (strA);
        if (nDa>nA)
            AfxMessageBox (LOADSTRING (IDS_ADAMODLGIDRANGE_2), MB_ICONSTOP);
        else
            CDialog::OnOK ();
    }
}

