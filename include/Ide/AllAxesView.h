#if !defined(AFX_ALLAXESVIEW_H__53D3AD38_4F8F_4FBA_8F8A_EA2FA67FE250__INCLUDED_)
#define AFX_ALLAXESVIEW_H__53D3AD38_4F8F_4FBA_8F8A_EA2FA67FE250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllAxesView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAllAxesView window

class CAllAxesView : public CWnd
{
// Construction
public:
	CAllAxesView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllAxesView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAllAxesView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAllAxesView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLAXESVIEW_H__53D3AD38_4F8F_4FBA_8F8A_EA2FA67FE250__INCLUDED_)
