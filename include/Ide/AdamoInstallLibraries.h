/*
** AdamoInstallLibraries.h
*/

#pragma once

#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAdapters dialog

class CDlgInstallLibraries : public CDialog
{
// Construction
public:
	CDlgInstallLibraries(CWnd* pParent = NULL);   // standard constructor
	CStringList& GetListLibraries ()	{ return m_listLibraries; }
// Dialog Data
	//{{AFX_DATA(CAdamoAboutBox)
	enum { IDD = IDD_DLG_INSTALL_LIBRARY };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnOK ();
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdamoAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddToProject ();
	afx_msg void OnRemoveFromProject ();
	afx_msg void OnMoveUp ();
	afx_msg void OnMoveDown ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CListBox m_ctrlLibrary;
	CListBox m_ctrlProject;
	CButtonST m_btnAddToProject;
	CButtonST m_btnRemoveFromProject;
	CButtonST m_btnMoveUp;
	CButtonST m_btnMoveDown;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;
	CStringList m_listLibraries;
private :
	void LoadLibraryLib ();
	void LoadProjectLib ();
	bool ExistInProject (CString& strLib);
	void InitButtons ();
	void InitCaption ();
};
