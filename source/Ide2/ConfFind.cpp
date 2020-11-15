// ConfFind.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "adamoglobal.h"
#include "conftreeview.h"
#include "conffind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfFind dialog


CConfFind::CConfFind(CWnd* pParent /*=NULL*/)
	: CDialog(CConfFind::IDD, pParent), m_pCTV (NULL)
{
	//{{AFX_DATA_INIT(CConfFind)
	m_bMatchCase = FALSE;
	m_bMatchWords = FALSE;
	//}}AFX_DATA_INIT
}


void CConfFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfFind)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchText);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchCase);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchWords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfFind, CDialog)
	//{{AFX_MSG_MAP(CConfFind)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfFind message handlers

BOOL CConfFind::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CIdeApp *app = (CIdeApp*)AfxGetApp();
    CenterWindow(GetParent());
    m_ctrlSearchText.ResetContent();
    for (int i= app->m_strFindConf.GetSize()-1;i>=0;i--)   {
        m_ctrlSearchText.AddString(app->m_strFindConf.GetAt(i));
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfFind::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    m_pParentWnd->SendMessage (WM_CLOSEDIALOGFIND, 0, 0);
}

void CConfFind::OnOK() 
{
	OnClose ();
}

void CConfFind::OnCancel() 
{
	OnClose ();
}

void CConfFind::OnNext()
{
    CString strFind;
    int n=0;

    //UpdateData();
    //m_ctrlSearchText.GetWindowText(strFind);
    //StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
    //m_hBS=m_pCTV->GetTreeCtrl().GetSelectedItem ();
    //m_bs=(m_hRootNode==m_hBS&&strFind!=m_strLastFind);
    //m_strLastFind=strFind;
    //while (n++<=1)   {
    //    if (m_pCTV)   {
    //        m_hBS=m_pCTV->FindTreeNode (m_hRootNode, m_hBS, &m_bs);
    //        if (m_hBS)   {
    //            m_pCTV->GetTreeCtrl().EnsureVisible (m_hBS);
    //            m_pCTV->GetTreeCtrl().SelectItem (m_hBS);
    //            break;
    //        }
    //        else
    //            m_hBS=m_hRootNode;
    //    }
    //}
}

void CConfFind::StoreSearchString(CIdeApp *app, LPCSTR szSearch)
{
    for (int i=0;i<app->m_strFindConf.GetSize();i++)   {
        if (strcmp(app->m_strFindConf.GetAt(i), szSearch) == 0)
            return;
    }
    app->m_strFindConf.Add(szSearch);
    m_ctrlSearchText.InsertString(0, szSearch);
}
