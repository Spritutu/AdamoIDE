// DlgFindErrors.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "adamoglobal.h"
#include "DlgFindErrors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFindErrors dialog


CDlgFindErrors::CDlgFindErrors(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindErrors::IDD, pParent), m_pSEView (NULL)
{
	//{{AFX_DATA_INIT(CDlgFindErrors)
	m_bMatchCase = FALSE;
	m_bMatchWords = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgFindErrors::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFindErrors)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchText);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchCase);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchWords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFindErrors, CDialog)
	//{{AFX_MSG_MAP(CDlgFindErrors)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFindErrors message handlers

BOOL CDlgFindErrors::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFindErrors::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGFINDERRORS, 0, 0);
}

void CDlgFindErrors::OnOK ()
{
    m_pParentWnd->SendMessage (WM_CLOSEDIALOGFINDERRORS, 0, 0);
}

void CDlgFindErrors::OnCancel ()
{
    m_pParentWnd->SendMessage (WM_CLOSEDIALOGFINDERRORS, 0, 0);
}

void CDlgFindErrors::OnNext() 
{
	// TODO: Add your control notification handler code here
    UpdateData();
    // try to find text
    CString strFind;
    m_ctrlSearchText.GetWindowText(strFind);
    StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
    Find (strFind.LockBuffer(), 0);
    strFind.UnlockBuffer();
}

void CDlgFindErrors::OnPrev() 
{
	// TODO: Add your control notification handler code here
    UpdateData();
    // try to find text
    CString strFind;
    m_ctrlSearchText.GetWindowText(strFind);
    StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
    Find (strFind.LockBuffer(), 1);
    strFind.UnlockBuffer();
}

void CDlgFindErrors::StoreSearchString(CIdeApp *pApp, LPCSTR szSearch)
{
   if (pApp != NULL)   {
      for (int i=0;i<pApp->m_strFindErrorsstrings.GetSize();i++)   {
         if (strcmp(pApp->m_strFindErrorsstrings.GetAt(i), szSearch) == 0)
            return;
      }
      pApp->m_strFindErrorsstrings.Add(szSearch);
      m_ctrlSearchText.InsertString(0, szSearch);
   }
}

void CDlgFindErrors::Find (const char* sz, int nDirection)
{
    BeginWaitCursor ();
    if (m_pSEView)
        m_pSEView->Find (sz, nDirection, m_bMatchWords!=0, m_bMatchCase!=0);
    EndWaitCursor ();
}

