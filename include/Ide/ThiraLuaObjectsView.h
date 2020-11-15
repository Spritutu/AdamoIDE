#pragma once

#define ID_TREECTRL_VIEWLUAOBJECTS	1102

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView view

class CThiraLuaObjectsView: public CPaneView
{
protected:
	CThiraLuaObjectsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CThiraLuaObjectsView)
	
// Attributes
// Operations
	void LoadTree (CProject* pProject);
	void UpdateTree (CProject* pProject);
	void AddLibraryObjects (CString strLibrary, CString strModule, list<CAdamoObj*>* pLibraryObjects);
public:
	void DeleteLibraryObjects ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpOnLineView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CThiraLuaObjectsView();
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
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void RefreshMetrics(AppTheme nTheme);
private :
	CXTPTreeCtrl m_treeCtrl;
	CXTPImageList m_il;
	CProject* m_pProject;
	HTREEITEM m_htRoot[2];
	bool m_bInitialized;
private :
	void SetTreeStyle ();
	void LoadObjectsTree (CProject* pProject);
	CString ExtractModule (CString strPathModule);
	void DeleteAllData (bool bDeleteNodes);
	void DeleteData (HTREEITEM hItem);
	int GetImageIndex (int nType);
	void DeleteObjectsTree ();
	int GetTreeType (HTREEITEM ht, int& nLevel);
	HTREEITEM GetTreeItemLibrary (CString strLibrary);
	void LoadObjectsLibraryTree(CProject* pPrj);
	void AddRootNodes ();
	void DeleteAllNodes ();
	int CreateTree ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
