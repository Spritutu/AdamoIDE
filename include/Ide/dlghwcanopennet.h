#if !defined(AFX_DLGHWCANOPENNET_H__124E8896_B412_443B_AA22_449075C5F533__INCLUDED_)
#define AFX_DLGHWCANOPENNET_H__124E8896_B412_443B_AA22_449075C5F533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlghwcanopennet.h : header file
//

#include "ConfigDoc.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNet dialog

class CDlgHWCanOpenNet : public CDialog
{
    DECLARE_DYNAMIC (CDlgHWCanOpenNet )
// Construction
public:
	CDlgHWCanOpenNet(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
    void InitTitles ();
    void InitData ();
    void Save ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }		
// Dialog Data
	//{{AFX_DATA(CDlgHWCanOpenNet)
	enum { IDD = IDD_DLG_HW_CANOPENNET };
	CParsedEdit	m_ctrlEditLifeTime;
	CParsedEdit	m_ctrlEditGuardTime;
	CButton m_btnEnableCanOpenWarnings;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHWCanOpenNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHWCanOpenNet)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioBaudrate1000();
	afx_msg void OnRadioBaudrate500();
	afx_msg void OnRadioBaudrate250();
	afx_msg void OnRadioBaudrate125();
	afx_msg void OnChangeEditSyncfrequency();
	afx_msg void OnChangeEditTimeoutsdo();
	afx_msg void OnChangeEditNetguardtime();
	afx_msg void OnChangeEditNetlifetime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    private :
        void SetConfigDoc ();
        void SaveData ();
        void InitPsw ();
    private :
		CProject       *m_pPrj;
        CParsedEdit     m_editSDOTimeot;
        CParsedEdit     m_editSync;
        CConfigDoc     *m_pConfigDoc;
        int             m_nKinemaAddress;
        bool            m_bIsInitialized;
        ePswLevel       m_ePswLevel;
public:
	afx_msg void OnBnClickedEnableCanopenWarning();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWCANOPENNET_H__124E8896_B412_443B_AA22_449075C5F533__INCLUDED_)
