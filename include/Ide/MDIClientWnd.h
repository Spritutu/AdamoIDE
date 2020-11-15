#if !defined(AFX_MDICLIENTWND_H__A19DDD02_2C26_4675_A107_262E4B931144__INCLUDED_)
#define AFX_MDICLIENTWND_H__A19DDD02_2C26_4675_A107_262E4B931144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDIClientWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd window

class CMDIClientWnd : public CWnd//CXTPTabClientWnd
{
// Construction
public:
	CMDIClientWnd();

// Attributes
public:
protected:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIClientWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDIClientWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMDIClientWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICLIENTWND_H__A19DDD02_2C26_4675_A107_262E4B931144__INCLUDED_)
