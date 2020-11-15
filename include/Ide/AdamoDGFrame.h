#if !defined(AFX_ADAMODGFRAME_H__177DDB6D_3C8D_4DA4_BD66_B4AE477FFDB0__INCLUDED_)
#define AFX_ADAMODGFRAME_H__177DDB6D_3C8D_4DA4_BD66_B4AE477FFDB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGFrame frame

class CAdamoDGFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAdamoDGFrame)
protected:
	CAdamoDGFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
    void ActivateFrame(int nCmdShow);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoDGFrame();

	// Generated message map functions
	//{{AFX_MSG(CAdamoDGFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGFRAME_H__177DDB6D_3C8D_4DA4_BD66_B4AE477FFDB0__INCLUDED_)
