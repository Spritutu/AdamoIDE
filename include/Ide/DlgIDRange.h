#if !defined(AFX_DLGIDRANGE_H__B3009AEA_DD5B_43A1_8C6A_C0CDB5A2DC3A__INCLUDED_)
#define AFX_DLGIDRANGE_H__B3009AEA_DD5B_43A1_8C6A_C0CDB5A2DC3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIDRange.h : header file
//

/* include files */
#include "paredit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgIDRange dialog

class CDlgIDRange : public CDialog
{
// Construction
public:
	CDlgIDRange(CWnd* pParent = NULL);   // standard constructor
    int GetDaID ()   { return nDa; }
    int GetAID ()    { return nA; }
// Dialog Data
	//{{AFX_DATA(CDlgIDRange)
	enum { IDD = IDD_DLGRANGEIDERRORS };
	CParsedEdit	m_edtAID;
	CParsedEdit	m_edtDaID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIDRange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void OnOK ();
	// Generated message map functions
	//{{AFX_MSG(CDlgIDRange)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    int nDa;
    int nA;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIDRANGE_H__B3009AEA_DD5B_43A1_8C6A_C0CDB5A2DC3A__INCLUDED_)
