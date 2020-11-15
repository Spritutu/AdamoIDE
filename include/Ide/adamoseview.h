#if !defined(AFX_ADAMOSEVIEW_H__D9774FC4_300D_4514_8947_33E27E206108__INCLUDED_)
#define AFX_ADAMOSEVIEW_H__D9774FC4_300D_4514_8947_33E27E206108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoSEView.h : header file
//
#define IDC_TABCONTROL 2000

class CDlgFindErrors;

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEView view

class CAdamoSEView : public CView
{
protected:
	CAdamoSEView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoSEView)

// Attributes
public:

// Operations
    void Activate();
    void OnCloseSEView ();
	void AddError (stAdamoErrore *p, int nItem);
    int  Find (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase); 
    int  GetActiveErrorType ();
    int  GetActiveItemSelected ();
    void SetActiveItemSelected (int nItem);
    void Reload (int nMonth);
	bool FilterEnabled ()		{ return m_bEnableFilter; }
	void OnSeFilterErrors();
	void OnSeClearFilter();
	void OnSeGotoLine ();
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSEView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoSEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
    afx_msg LRESULT OnOpenDialogFindErrors (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCloseDialogFindErrors (WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CAdamoSEView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateSeRefresh(CCmdUI* pCmdUI);
	afx_msg void OnSeRefresh();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CXTPTabControl m_wndTabControl;
    CImageList m_tbiml;
    CBitmap* m_bmpTblEH;
    CBitmap* m_bmpTblEI;
    CBitmap* m_bmpTblEC;
    CBitmap* m_bmpTblEISO;
    CBitmap* m_bmpTblEEvent;
    CBitmap* m_bmpTblEAll;
    CDlgFindErrors* m_pDlgErrors;
	int m_nFilterDa;
	int m_nFilterA;
	bool m_bEnableFilter;
	int m_nMonth;
	AppTheme m_theme;
private :
    int  CreateImageList ();
	int  LoadErrors (int nMonth = -1);
	void SetIdFilter (int nFilterDa, int nFilterA)	{ m_nFilterDa = nFilterDa; m_nFilterA = nFilterA; }
	void EnableIdFilter ()							{ m_bEnableFilter = true; }
	void DisableIdFilter ()							{ m_bEnableFilter = false; }
	void OnSeUpdate ();
	void DeleteAllItems ();
	void SetTabStyle ();
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	CView* GetView (int nItem);
	CView* GetActiveView ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSEVIEW_H__D9774FC4_300D_4514_8947_33E27E206108__INCLUDED_)
