// AdamoDGSearch.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoDGComm.h"
#include "AdamoDGSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSearch dialog


CAdamoDGSearch::CAdamoDGSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDGSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoDGSearch)
	m_bMatchCase = FALSE;
	m_bMatchWords = FALSE;
 	//}}AFX_DATA_INIT
}


void CAdamoDGSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoDGSearch)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchText);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchCase);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchWords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoDGSearch, CDialog)
	//{{AFX_MSG_MAP(CAdamoDGSearch)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSearch message handlers

/*
** StoreSearchString :
*/
void CAdamoDGSearch::StoreSearchString (CIdeApp *pApp, LPCSTR szSearch)
{
   if (pApp != NULL)   {
      for (int i=0;i<pApp->m_strFindDiagnostic.GetSize();i++)   {
         if (strcmp(pApp->m_strFindDiagnostic.GetAt(i), szSearch) == 0)
            return;
      }
      pApp->m_strFindDiagnostic.Add(szSearch);
      m_ctrlSearchText.InsertString(0, szSearch);
   }
}

BOOL CAdamoDGSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoDGSearch::OnOK ()
{
	m_pDGComm->OnCloseDGView ();
}

void CAdamoDGSearch::OnCancel ()
{
	m_pDGComm->OnCloseDGView ();
}

void CAdamoDGSearch::OnNext() 
{
    UpdateData();
    // try to find text
    CString strFind;
    m_ctrlSearchText.GetWindowText(strFind);
    StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
    Find (strFind.LockBuffer(), 0);
    strFind.UnlockBuffer();
}

void CAdamoDGSearch::OnPrev() 
{
    UpdateData();
    // try to find text
    CString strFind;
    m_ctrlSearchText.GetWindowText(strFind);
    StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
    Find (strFind.LockBuffer(), 1);
    strFind.UnlockBuffer();
}

void CAdamoDGSearch::Find (const char* sz, int nDirection)
{
    BeginWaitCursor ();
    if (m_pDGComm)
        if (nDirection==0)
            m_pDGComm->FindNext (sz, nDirection, m_bMatchWords!=0, m_bMatchCase!=0);
        else
            m_pDGComm->FindPrev (sz, nDirection, m_bMatchWords!=0, m_bMatchCase!=0);
    EndWaitCursor ();
}
