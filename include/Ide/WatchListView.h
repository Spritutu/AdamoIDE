#if !defined(AFX_WATCHLISTVIEW_H__603D3A15_C7A1_4DFF_AFEF_34D03444A19E__INCLUDED_)
#define AFX_WATCHLISTVIEW_H__603D3A15_C7A1_4DFF_AFEF_34D03444A19E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WatchListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWatchListView view

class CWatchListView : public CListView
{
protected:
	CWatchListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWatchListView)

// Attributes
public:
	void Redraw();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWatchListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWatchListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void UpdateRow(int iItem);
	void AddEmptyRow();
	void AddEditItem(LVITEM &item);
	CStringArray m_exps;
	//{{AFX_MSG(CWatchListView)
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATCHLISTVIEW_H__603D3A15_C7A1_4DFF_AFEF_34D03444A19E__INCLUDED_)
