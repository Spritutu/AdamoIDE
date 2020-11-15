#if !defined(AFX_VARLISTVIEW_H__20E73880_3CF0_4EEC_93F9_8131A4D26D5A__INCLUDED_)
#define AFX_VARLISTVIEW_H__20E73880_3CF0_4EEC_93F9_8131A4D26D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VarListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVarListView view

class CVarListView : public CListView
{
protected:
	CVarListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CVarListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVarListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVarListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CVarListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VARLISTVIEW_H__20E73880_3CF0_4EEC_93F9_8131A4D26D5A__INCLUDED_)
