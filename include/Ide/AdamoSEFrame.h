#if !defined(AFX_ADAMOSEFRAME_H__CEFA18BF_DCC1_4086_B056_3D09B538FA10__INCLUDED_)
#define AFX_ADAMOSEFRAME_H__CEFA18BF_DCC1_4086_B056_3D09B538FA10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoSEFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEFrame frame

class CAdamoSEFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAdamoSEFrame)
protected:
	CAdamoSEFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSEFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoSEFrame();

	// Generated message map functions
	//{{AFX_MSG(CAdamoSEFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
    virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSEFRAME_H__CEFA18BF_DCC1_4086_B056_3D09B538FA10__INCLUDED_)
