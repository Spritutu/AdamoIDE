#if !defined(AFX_HTMLFRAME_H__CB25CD60_A605_49C5_9986_3AD86CA986C1__INCLUDED_)
#define AFX_HTMLFRAME_H__CB25CD60_A605_49C5_9986_3AD86CA986C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HTMLFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHTMLFrame frame

class CHTMLFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CHTMLFrame)
protected:
	CHTMLFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHTMLFrame();

	// Generated message map functions
	//{{AFX_MSG(CHTMLFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLFRAME_H__CB25CD60_A605_49C5_9986_3AD86CA986C1__INCLUDED_)
