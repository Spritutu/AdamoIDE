#if !defined(AFX_DLGECCONFIGSLAVES_H__88B1AED3_DDDA_4AA9_A270_EDCBA2FE0A67__INCLUDED_)
#define AFX_DLGECCONFIGSLAVES_H__88B1AED3_DDDA_4AA9_A270_EDCBA2FE0A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEcConfigSlaves.h : header file
//
/* include files */
#include "AdamoIdeEthercat.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEcConfigSlaves dialog

class CDlgEcConfigSlaves : public CDialog
{
// Construction
public:
	CDlgEcConfigSlaves(CWnd* pParent = NULL);   // standard constructor
	void SetEcSlave (stAdamoEthSlave *pEcSlave)	{ m_pEcSlave=pEcSlave; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
// Dialog Data
	//{{AFX_DATA(CDlgEcConfigSlaves)
	enum { IDD = IDD_DLG_EC_CONFIG_SLAVES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEcConfigSlaves)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEcConfigSlaves)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	stAdamoEthSlave *m_pEcSlave;
	CProject		*m_pPrj;
	CXTFlatTabCtrl  *m_pFlatTabCtrl;
	int			m_nCurTabSel;
	bool		m_bInitialized;
private :
	void CreateTab ();
	void OnOK ();
	void OnCancel ();
    afx_msg LRESULT OnEnableTab (WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGECCONFIGSLAVES_H__88B1AED3_DDDA_4AA9_A270_EDCBA2FE0A67__INCLUDED_)
