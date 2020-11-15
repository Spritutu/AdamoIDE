#if !defined(AFX_SHOWGLOBALVARS_H__41A6DB6A_CFBD_4A1A_9236_65293CE29797__INCLUDED_)
#define AFX_SHOWGLOBALVARS_H__41A6DB6A_CFBD_4A1A_9236_65293CE29797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// showglobalvars.h : header file
//

#include "AdamoShowGlobalsSupergrid.h"

/////////////////////////////////////////////////////////////////////////////
// CShowGlobalVars dialog

class CShowGlobalVars : public CDialog
{
// Construction
public:
	CShowGlobalVars(CWnd* pParent = NULL);   // standard constructor
    void SetResults (CString  strName, int  nRow, CString  strCol, int  nType);
    void GetResults (CString* strName, int *nRow, CString *strCol, int *nType);
// Dialog Data
	//{{AFX_DATA(CShowGlobalVars)
	enum { IDD = IDD_DLG_SHOWGLOBALS };
		// NOTE: the ClassWizard will add data members here
	CStatic	m_stFrame;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowGlobalVars)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowGlobalVars)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCloseDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void OnOK ();
private :
    CShowGlobalsSuperGrid m_sg;
    CString m_strName;
    int     m_nRow, m_nType;
    CString m_strCol;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWGLOBALVARS_H__41A6DB6A_CFBD_4A1A_9236_65293CE29797__INCLUDED_)
