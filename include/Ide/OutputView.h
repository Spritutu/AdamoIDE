#if !defined(AFX_OUTPUTVIEW_H__9099ECD2_26C4_4827_990E_D3EC165C87CD__INCLUDED_)
#define AFX_OUTPUTVIEW_H__9099ECD2_26C4_4827_990E_D3EC165C87CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputView.h : header file
//
#include "LuaEditor.h"

/////////////////////////////////////////////////////////////////////////////
// COutputView view

class COutputView : public CView
{
protected:
	COutputView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COutputView)

// Attributes
public:

// Operations
public:
	CWnd* GetItemWindow(int nIndex);
	CLuaEditor* GetEditor(int nOutput);
	void Write(int nOutput,CString strMsg);
	void Clear(int nOutput);
	int SetCurSel(int nItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COutputView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
   	BOOL	m_FTS_XT_BOTTOM;
	BOOL	m_FTS_XT_HASARROWS;
	BOOL	m_FTS_XT_HASHOMEEND;
	BOOL	m_FTS_XT_HSCROLL;

    int   m_dwTabStyle;
    CRect m_rcTabs;

  	CXTFlatTabCtrl*			m_pFlatTabCtrl;

    CLuaEditor m_sheetBuild;
    CLuaEditor m_sheetDebug;

    // Generated message map functions
protected:
	void UpdateActiveView();
	int CreateViews();
	void CreateTabs();
	void UpdateTabsStyle();
	//{{AFX_MSG(COutputView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTVIEW_H__9099ECD2_26C4_4827_990E_D3EC165C87CD__INCLUDED_)
