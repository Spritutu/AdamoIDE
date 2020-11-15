#if !defined(AFX_DLGECCONFIGSINGLESLAVE_H__35528969_050A_4A47_8FA2_DDB2BCF92CAA__INCLUDED_)
#define AFX_DLGECCONFIGSINGLESLAVE_H__35528969_050A_4A47_8FA2_DDB2BCF92CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEcConfigSingleSlave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEcConfigSingleSlave dialog

class CDlgEcConfigSingleSlave : public CDialog
{
// Construction
public:
	CDlgEcConfigSingleSlave(CWnd* pParent = NULL);   // standard constructor
	void SetEcSlave (stAdamoEthSlave *pEcSlave, int nTab)	{ m_pEcSlave=pEcSlave; m_nTab=nTab; }
	void SetProject (CProject *pPrj)						{ m_pPrj = pPrj; }
	bool IsEnabled ();
	void OnOK ();
	void OnCancel ();
// Dialog Data
	//{{AFX_DATA(CDlgEcConfigSingleSlave)
	enum { IDD = IDD_DLG_EC_CONFIG_SINGLE_SLAVE };
	CButton	m_ctrlRadioAssi;
	CButton	m_ctrlRadioIO;
	CButton	m_ctrlRadioIOAnag;
	CButton	m_ctrlTabType;
	CListCtrl	m_ctrlVarEnable;
	CButton	m_ctrlTabEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEcConfigSingleSlave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEcConfigSingleSlave)
	virtual BOOL OnInitDialog();
	afx_msg void OnTabEnable();
	afx_msg void OnEcTabAssi();
	afx_msg void OnEcTabIo();
	afx_msg void OnEcTabIoanag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void InitStrings ();
	void InitListView ();
	void InitControls ();
private :
	stAdamoEthSlave *m_pEcSlave;
	CProject		*m_pPrj;
	int m_nTab;
	CImageList m_iIcons;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGECCONFIGSINGLESLAVE_H__35528969_050A_4A47_8FA2_DDB2BCF92CAA__INCLUDED_)
