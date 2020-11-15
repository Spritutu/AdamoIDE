#if !defined(AFX_CANOPENDATA_H__EF0847CF_BF6D_4CA4_AEFC_012263591752__INCLUDED_)
#define AFX_CANOPENDATA_H__EF0847CF_BF6D_4CA4_AEFC_012263591752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// canopendata.h : header file
//

#include "ConfigDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCanOpenData dialog

class CCanOpenData : public CDialog
{
// Construction
public:
	CCanOpenData(CWnd* pParent = NULL);   // standard constructor
    void SetConfigDoc (CConfigDoc *pDoc)        { m_pConfigDoc=pDoc; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    void SetCanOpenNode (int nNode)             { m_nNode=nNode; }
    void Save ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

// Dialog Data
	//{{AFX_DATA(CCanOpenData)
	enum { IDD = IDD_DLG_HW_CANOPEN_DATA };
	CButton	m_ctrlPollingReceiver;
	CButton	m_ctrlDisabledReceiver;
	CButton	m_ctrlVirtualReceiver;
	CButton	    m_ctrlEnableGuarding;
	CButton	    m_ctrlEnableTPDO;
	CParsedEdit	m_ctrlTPDOSync;
	CParsedEdit	m_ctrlLifeTime;
	CComboBox	m_ctrlTPDO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanOpenData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCanOpenData)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeComboTpdo();
	afx_msg void OnDestroy();
	afx_msg void OnRadioTpdoAA();
	afx_msg void OnRadioTpdoCS();
	afx_msg void OnRadioTpdoCUS();
	afx_msg void OnRadioTpdoDEV();
	afx_msg void OnRadioTpdoRTRA();
	afx_msg void OnRadioTpdoRTRS();
	afx_msg void OnChangeEditTpdo();
	afx_msg void OnCheckTpdo();
	afx_msg void OnCheckGuarding();
	afx_msg void OnRadioGuarding();
	afx_msg void OnRadioHeartbeat();
	afx_msg void OnChangeEditLifeTime();
	afx_msg void OnCheckVirtualReceiver();
	afx_msg void OnCheckDisabledReceiver();
    afx_msg void OnCheckPollingReceiver();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfigDoc     *m_pConfigDoc;
	CProject	   *m_pPrj;
    int             m_nKinemaAddress;
    int             m_nNode;
    int             m_nSelected;
    bool            m_bIsInitialized;
    ePswLevel       m_ePswLevel;
private :
    void InitTitles ();
    void SetConfigDoc ();
    void EnableTPDOControls (bool bEnable=true);
    void EnableGuardingControls (bool bEnable=true);
    void LoadData ();
    void StoreData ();
    void InitPsw ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANOPENDATA_H__EF0847CF_BF6D_4CA4_AEFC_012263591752__INCLUDED_)
