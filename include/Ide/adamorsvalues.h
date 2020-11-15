#if !defined(AFX_ADAMORSVALUES_H__042D6C3F_B833_4FBA_98ED_64E7DA89061C__INCLUDED_)
#define AFX_ADAMORSVALUES_H__042D6C3F_B833_4FBA_98ED_64E7DA89061C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoRSValues.h : header file
//

/* include files */
#include "paredit.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSValues dialog

class CAdamoRSValues : public CDialog
{
// Construction
public:
	CAdamoRSValues(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdamoRSValues)
	enum { IDD = IDD_DLG_RS_VALUES };
	CButton	m_checkEnableValue;
	CButton	m_radioSelected;
	CButton	m_radioNotSelected;
	//}}AFX_DATA
	CParsedEdit	m_editValue;
	CParsedEdit	m_editOption;
    CString m_strOption;
    CString m_strValue;
    bool    m_bSelected;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSValues)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoRSValues)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckRsForceValue();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSVALUES_H__042D6C3F_B833_4FBA_98ED_64E7DA89061C__INCLUDED_)
