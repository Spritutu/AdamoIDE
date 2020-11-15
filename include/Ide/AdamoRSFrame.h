#if !defined(AFX_ADAMORSFRAME_H__A8939786_C8DF_4B21_ACC3_CA9DB02E9AFC__INCLUDED_)
#define AFX_ADAMORSFRAME_H__A8939786_C8DF_4B21_ACC3_CA9DB02E9AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoRSFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSFrame frame

class CAdamoRSFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAdamoRSFrame)
protected:
	CAdamoRSFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoRSFrame();

	// Generated message map functions
	//{{AFX_MSG(CAdamoRSFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
        virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSFRAME_H__A8939786_C8DF_4B21_ACC3_CA9DB02E9AFC__INCLUDED_)
