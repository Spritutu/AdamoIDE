#if !defined(AFX_SCINTILLAVIEW_H__FE8FF881_86FA_4CCA_A16D_0E586E353DB4__INCLUDED_)
#define AFX_SCINTILLAVIEW_H__FE8FF881_86FA_4CCA_A16D_0E586E353DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScintillaView.h : header file
//

#include "LuaEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CScintillaView view

class CScintillaView : public CView
{
public:
	CScintillaView();           // protected constructor used by dynamic creation
	virtual ~CScintillaView();
protected:
	DECLARE_DYNCREATE(CScintillaView)

// Attributes
public:

protected:
	CLuaEditor m_view;

// Operations
public:
	void Clear();
	void Write(CString strMsg);

	CLuaEditor* GetEditor() { return &m_view; };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScintillaView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CScintillaView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCINTILLAVIEW_H__FE8FF881_86FA_4CCA_A16D_0E586E353DB4__INCLUDED_)
