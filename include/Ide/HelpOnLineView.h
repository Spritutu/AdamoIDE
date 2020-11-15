#pragma once

#define ID_TREECTRL_VIEWHELPONLINE 1103

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView view

class CHelpOnLineView : public CPaneView
{
protected:
	CHelpOnLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHelpOnLineView)
	
// Attributes
// Operations
public:
	void LoadTree (CProject* pProject);
	void UpdateTree (CProject* pProject);
	void DeleteCustomFunction (CString strPath, CString strFunction);
	void DeleteLibraryFunctions ();
	void AddLibraryFunction (CString strLibrary, CString strModule, list<CAdamoFunction*>* pLibraryFunctions);
	void SetPath (CProject* pProject);
	void ViewHelpPage (CString strHelp);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpOnLineView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHelpOnLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	virtual void OnInitialUpdate();
	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpOnLineView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void RefreshMetrics(AppTheme nTheme);
private :
	void SetTreeStyle ();
	CString GetLanguageExtension ();
	void DeleteData (HTREEITEM hItem);
	void DeleteAllData (bool bDeleteNodes);
	void LoadStandardFunctionsTree (CProject* pProject);
	void LoadCustomFunctionsTree (CProject* pProject);
	void LoadLibraryFunctionsTree (CProject* pPrj);
	CString ExtractModule (CString strPathModule);
	int GetTreeType (HTREEITEM ht);
	void DeleteFunctionsTree ();
	HTREEITEM GetTreeItemLibrary (CString strLibrary);
	void AddRootNodes ();
	void DeleteAllNodes ();
	int CreateTree ();
private :
	CXTPTreeCtrl m_treeCtrl;
	CXTPImageList m_il;
	bool m_bInitialized;
	CString m_strPath;
	CProject* m_pProject;
	HTREEITEM m_htRoot[3];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
