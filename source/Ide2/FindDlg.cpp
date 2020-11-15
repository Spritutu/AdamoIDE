/* @doc
 * @module FindDlg.cpp | Implementation of a modeless find dialog for use with the scintilla edit control
 */

#include "stdafx.h"
#include "FindDlg.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////
// @mfunc constructor
// @rvalue void | not used
//
CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindDlg)
	m_bMatchcase = FALSE;
	m_bMacthregex = FALSE;
	m_bMatchwords = FALSE;
	//}}AFX_DATA_INIT
   m_pEdit = NULL;
}
/////////////////////////////////////
// @mfunc set/get data to/from controls in dialog
// @rvalue void | not used
//
void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchtext);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchcase);
	DDX_Check(pDX, IDC_MATCHREGEX, m_bMacthregex);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchwords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////
// @mfunc modeless handler for OK button - destroy myself and rely on PostNcDestroy to free memory
// @rvalue void | not used
//
void CFindDlg::OnOK() 
{
	OnClose ();
}

void CFindDlg::OnCancel() 
{
	OnClose ();
}

/////////////////////////////////////
// @mfunc init dialog members
// @rvalue void | not used
//
BOOL CFindDlg::InitDialog() 
{
    // get app with old searchstrings
    CIdeApp *app = (CIdeApp*)AfxGetApp();

    CenterWindow(GetParent());

    // set selected text to dropdown
    if (m_pEdit != NULL && app != NULL)   {
        m_ctrlSearchtext.ResetContent();
        //    get old find strings and add to combobox
        for (int i= app->m_strFindstrings.GetSize()-1;i>=0;i--)   {
         m_ctrlSearchtext.AddString(app->m_strFindstrings.GetAt(i));
        }
        //    set new searchtext and check if already present in combobox
		CString strFind;
		strFind = m_pEdit->GetSelectedText ();
		if (strFind.GetLength () == 0)   {
			strFind = m_pEdit->GetCaretText();
			if (strFind.FindOneOf(_T("\r\n")) < 0)
				m_pEdit->SelectCurrentWord ();
		}
		m_ctrlSearchtext.SetWindowText(strFind);
		m_ctrlSearchtext.SetEditSel (-1, -1);
		m_ctrlSearchtext.SetFocus ();
    }
	return FALSE;
}
/////////////////////////////////////
// @mfunc try to find string forward
// @rvalue void | not used
//
void CFindDlg::OnNext() 
{
   UpdateData();
// set search flags
	int nSearchFlags = 0;
   if (m_bMatchwords)
      nSearchFlags |= SCFIND_WHOLEWORD;
   if (m_bMatchcase)
      nSearchFlags |= SCFIND_MATCHCASE;
   if (m_bMacthregex)
      nSearchFlags |= SCFIND_REGEXP;
// try to find text
   if (m_pEdit != NULL)   {
      CString strFind;
      m_ctrlSearchtext.GetWindowText(strFind);
      StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
      ((CMainFrame *) AfxGetMainWnd ())->SetStringToSearch (strFind);
      m_pEdit->SetSearchflags(nSearchFlags);
      if(!m_pEdit->SearchForward(strFind.LockBuffer()))   {
         MessageBeep(0);
      }
      SetFocus ();
      strFind.UnlockBuffer();
   }
}

/////////////////////////////////////
// @mfunc try to find string backward
// @rvalue void | not used
//
void CFindDlg::OnPrev() 
{
   UpdateData();
// set search flags
	int nSearchFlags = 0;
   if (m_bMatchwords)
      nSearchFlags |= SCFIND_WHOLEWORD;
   if (m_bMatchcase)
      nSearchFlags |= SCFIND_MATCHCASE;
   if (m_bMacthregex)
      nSearchFlags |= SCFIND_REGEXP;
// try to find text
   if (m_pEdit != NULL)   {
      CString strFind;
      m_ctrlSearchtext.GetWindowText(strFind);
      StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
      m_pEdit->SetSearchflags(nSearchFlags);
      if(!m_pEdit->SearchBackward(strFind.LockBuffer()))   {
         MessageBeep(0);
      }
      SetFocus ();
      strFind.UnlockBuffer();
   }
}
/////////////////////////////////////
// @mfunc store new searchstring to global array of searchstrings. Never store one twice.
// @rvalue void | not used
//
void CFindDlg::StoreSearchString(
                                 CIdeApp *app,    //@parm application handle
                                 LPCSTR szSearch)  //@parm text to store
{
   if (app != NULL)   {
//    try to find string in old stringlist
      for (int i=0;i<app->m_strFindstrings.GetSize();i++)   {
         if (strcmp(app->m_strFindstrings.GetAt(i), szSearch) == 0)
            return;
      }
//    if we get here no match was found and we add the new string
      app->m_strFindstrings.Add(szSearch);
      m_ctrlSearchtext.InsertString(0, szSearch);
   }
}

void CFindDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    m_pParentWnd->SendMessage (WM_CLOSEDIALOGFIND, 0, 0);
}
