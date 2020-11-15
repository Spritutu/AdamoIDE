#if !defined(AFX_ADAMORSEDITTABLEINDEX_H__FD2567B4_746B_483D_9807_4EA70122140D__INCLUDED_)
#define AFX_ADAMORSEDITTABLEINDEX_H__FD2567B4_746B_483D_9807_4EA70122140D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamorsedittableindex.h : header file
//

#include "AdamoRSEditIndexSupergrid.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSEditTableIndex dialog

class CAdamoRSEditTableIndex : public CDialog
{
// Construction
public:
	CAdamoRSEditTableIndex(CWnd* pParent = NULL);   // standard constructor
    void SetDBData (CString strTable, CString strIndex, CString strIndexValue)   { m_strTable=strTable; m_strIndex=strIndex; m_strIndexValue=strIndexValue; }
    CString GetResult ()                            { return m_eisg.GetResults (); }
// Dialog Data
	//{{AFX_DATA(CAdamoRSEditTableIndex)
	enum { IDD = IDD_DLG_RS_EDITTABLEINDEX };
	CStatic	m_stFrame;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSEditTableIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoRSEditTableIndex)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
    afx_msg void OnBtnCloseDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoRSEditIndexSuperGrid m_eisg;
    CString m_strTable;
    CString m_strIndex;
    CString m_strIndexValue;
private :
    void OnOK ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSEDITTABLEINDEX_H__FD2567B4_746B_483D_9807_4EA70122140D__INCLUDED_)
