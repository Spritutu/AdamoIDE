#if !defined(AFX_KSFILEDIALOG_H__7A479919_ABD7_45CA_9BEF_9925D0C8506F__INCLUDED_)
#define AFX_KSFILEDIALOG_H__7A479919_ABD7_45CA_9BEF_9925D0C8506F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//CKSFileDialog dialog
//Author:Keyvan Saneinejad (with some code from MFC)
//March 23/2000
//
//
//

class CKSFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CKSFileDialog)

public:
	void KSCenterWindow(HWND hWnd);
	void SetStringFilter(LPCSTR filter);
	void SetDocumentPointer (CDocument* pDoc);
	void SetAppPointer(CWinApp* pApp);
	int DoModal();
	CKSFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_EXPLORER|OFN_ENABLESIZING| OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
						  BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	void SetMyParent(CWnd* pwnd);



protected:
	CWnd* m_pMyParent;
	void AppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt);
	//{{AFX_MSG(CKSFileDialog)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWinApp *m_pApp;
	CDocument *m_pDoc;
	CString m_StrFilter;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEDIALOG_H__7A479919_ABD7_45CA_9BEF_9925D0C8506F__INCLUDED_)


