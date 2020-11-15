/* @doc
 * @module ReplaceDlg.cpp | Implementation of a modeless search/replace dialog for use with the scintilla edit control
 */
//

#include "stdafx.h"
#include "ReplaceDlg.h"
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
CReplaceDlg::CReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplaceDlg)
	m_bMatchcase = FALSE;
	m_bMatchregex = FALSE;
	m_bMatchwords = FALSE;
	m_bSelection = FALSE;
	//}}AFX_DATA_INIT
   m_pEdit = NULL;
   m_bForwardSearch = TRUE;
}
/////////////////////////////////////
// @mfunc set/get data to/from controls in dialog
// @rvalue void | not used
//
void CReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplaceDlg)
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchtext);
	DDX_Control(pDX, IDC_REPLACETEXT, m_ctrlReplacetext);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchcase);
	DDX_Check(pDX, IDC_MATCHREGEX, m_bMatchregex);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchwords);
	DDX_Check(pDX, IDC_MATCHSELECTION, m_bSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CReplaceDlg)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_REPLACE, OnReplace)
	ON_BN_CLICKED(IDC_REPLACEALL, OnReplaceall)
	ON_BN_CLICKED(IDC_MATCHSELECTION, OnMatchselection)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////
// @mfunc init dialog members
// @rvalue void | not used
//
BOOL CReplaceDlg::InitDialog() 
{
// get app with old searchstrings
   CIdeApp *app = (CIdeApp*)AfxGetApp();

   CenterWindow(GetParent());
   int i;

// set selected text to dropdown
   if (m_pEdit != NULL && app != NULL)   {
      m_ctrlSearchtext.ResetContent();
//    get old find strings and add to combobox
      for (i= app->m_strFindstrings.GetSize()-1;i>=0;i--)   {
         m_ctrlSearchtext.AddString(app->m_strFindstrings.GetAt(i));
      }
      m_ctrlReplacetext.ResetContent();
//    get old replace strings and add to combobox
      for (i= app->m_strReplacestrings.GetSize()-1;i>=0;i--)   {
         m_ctrlReplacetext.AddString(app->m_strReplacestrings.GetAt(i));
      }
//    set new searchtext and check if already present in combobox
	  CString strFind;
	  strFind = m_pEdit->GetSelectedText ();
	  if (strFind.GetLength () == 0)   {
		  CString strFind = m_pEdit->GetCaretText();
		  if (strFind.FindOneOf(_T("\r\n")) < 0)
			 m_pEdit->SelectCurrentWord ();
	  }
      m_ctrlSearchtext.SetWindowText(strFind);
      m_ctrlReplacetext.SetWindowText(strFind);
      m_ctrlSearchtext.SetEditSel (-1, -1);
      m_ctrlSearchtext.SetFocus ();
   }
   return FALSE;
}

/////////////////////////////////////
// @mfunc store new searchstring to global array of searchstrings. Never store one twice.
// @rvalue void | not used
//
void CReplaceDlg::StoreSearchString(
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
/////////////////////////////////////
// @mfunc store new searchstring to global array of searchstrings. Never store one twice.
// @rvalue void | not used
//
void CReplaceDlg::StoreReplaceString(
                                     CIdeApp *app,    //@parm application handle
                                     LPCSTR szReplace) //@parm text to store
{
   if (app != NULL)   {
//    try to find string in old stringlist
      for (int i=0;i<app->m_strReplacestrings.GetSize();i++)   {
         if (strcmp(app->m_strReplacestrings.GetAt(i), szReplace) == 0)
            return;
      }
//    if we get here no match was found and we add the new string
      app->m_strReplacestrings.Add(szReplace);
      m_ctrlReplacetext.InsertString(0, szReplace);
   }
}
/////////////////////////////////////
// @mfunc goto next occurance of searchstring
// @rvalue void | not used
//
void CReplaceDlg::OnNext() 
{
   m_bForwardSearch = TRUE;
   UpdateData();
// set search flags
	int nSearchFlags = 0;
   if (m_bMatchwords)
      nSearchFlags |= SCFIND_WHOLEWORD;
   if (m_bMatchcase)
      nSearchFlags |= SCFIND_MATCHCASE;
   if (m_bMatchregex)
      nSearchFlags |= SCFIND_REGEXP;
// try to find text
   if (m_pEdit != NULL)   {
//    if we dont operate on the selection 
      if (!m_bSelection)   {
         CString strFind;
         m_ctrlSearchtext.GetWindowText(strFind);
         StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
         m_pEdit->SetSearchflags(nSearchFlags);
         if(!m_pEdit->SearchForward(strFind.LockBuffer()))   {
            MessageBeep(0);
         }
         SetFocus ();
         strFind.UnlockBuffer();
      }
   }
}
/////////////////////////////////////
// @mfunc goto previous occurance of searchstring
// @rvalue void | not used
//
void CReplaceDlg::OnPrev() 
{
   m_bForwardSearch = FALSE;
   UpdateData();
// set search flags
	int nSearchFlags = 0;
   if (m_bMatchwords)
      nSearchFlags |= SCFIND_WHOLEWORD;
   if (m_bMatchcase)
      nSearchFlags |= SCFIND_MATCHCASE;
   if (m_bMatchregex)
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
// @mfunc replace current searchstring
// @rvalue void | not used
//
void CReplaceDlg::OnReplace() 
{
   UpdateData(TRUE);
// set search flags
	int nSearchFlags = 0;
   if (m_bMatchwords)
      nSearchFlags |= SCFIND_WHOLEWORD;
   if (m_bMatchcase)
      nSearchFlags |= SCFIND_MATCHCASE;
   if (m_bMatchregex)
      nSearchFlags |= SCFIND_REGEXP;
   if (m_pEdit != NULL)   {
      m_pEdit->SetSearchflags(nSearchFlags);
      long lStart = m_pEdit->GetSelectionStart();
      long lEnd = m_pEdit->GetSelectionEnd();
      CString strReplace;
      m_ctrlReplacetext.GetWindowText(strReplace);
      m_pEdit->ReplaceSearchedText(strReplace);
      StoreReplaceString ((CIdeApp*)AfxGetApp(), strReplace);
      if (m_bForwardSearch)   {
         m_pEdit->GotoPosition(lEnd);
         OnNext();
      }
      else   {
         m_pEdit->GotoPosition(lStart);
         OnPrev();
      }
   }
}
/////////////////////////////////////
// @mfunc replace all occurences of the search string
// @rvalue void | not used
//
void CReplaceDlg::OnReplaceall() 
{
   UpdateData(TRUE);
// set search flags
	int nSearchFlags = 0;
   if (m_bMatchwords)
      nSearchFlags |= SCFIND_WHOLEWORD;
   if (m_bMatchcase)
      nSearchFlags |= SCFIND_MATCHCASE;
   if (m_bMatchregex)
      nSearchFlags |= SCFIND_REGEXP;
   if (m_pEdit != NULL)   {
      CString strFind;
      m_ctrlSearchtext.GetWindowText(strFind);
      CString strReplace;
      m_ctrlReplacetext.GetWindowText(strReplace);
      m_pEdit->SetSearchflags(nSearchFlags);
      m_pEdit->ReplaceAll(strFind, strReplace, m_bSelection);
      StoreSearchString ((CIdeApp*)AfxGetApp(), strFind);
      StoreReplaceString ((CIdeApp*)AfxGetApp(), strReplace);
   }
   SetFocus ();
}

/////////////////////////////////////
// @mfunc Handle "selection only" state - disable/enable some controls
// @rvalue void | not used
//
void CReplaceDlg::OnMatchselection() 
{
	UpdateData();
// if we should operate on the selection disable next/prev/replace - only replace all works now
   if (m_bSelection)   {
      CWnd *pWnd = GetDlgItem(IDC_NEXT);
      if (pWnd != NULL)
         pWnd->EnableWindow(FALSE);
      pWnd = GetDlgItem(IDC_PREV);
      if (pWnd != NULL)
         pWnd->EnableWindow(FALSE);
      pWnd = GetDlgItem(IDC_REPLACE);
      if (pWnd != NULL)
         pWnd->EnableWindow(FALSE);
   }
   else   {
      CWnd *pWnd = GetDlgItem(IDC_NEXT);
      if (pWnd != NULL)
         pWnd->EnableWindow(TRUE);
      pWnd = GetDlgItem(IDC_PREV);
      if (pWnd != NULL)
         pWnd->EnableWindow(TRUE);
      pWnd = GetDlgItem(IDC_REPLACE);
      if (pWnd != NULL)
         pWnd->EnableWindow(TRUE);
	}
}

void CReplaceDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGREPLACE, 0, 0);
}

void CReplaceDlg::OnOK() 
{
	OnClose ();
}

void CReplaceDlg::OnCancel() 
{
	OnClose ();
}
