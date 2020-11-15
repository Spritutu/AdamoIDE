#if !defined(AFX_DLGHWMODULE_H__124E8896_B412_443B_AA22_449075C5F533__INCLUDED_)
#define AFX_DLGHWMODULE_H__124E8896_B412_443B_AA22_449075C5F533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlghwcanopennet.h : header file
//

#include "ConfigHWGrid.h"
#include "afxcmn.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNet dialog

class CDlgHWModule : public CDialog
{
    DECLARE_DYNAMIC (CDlgHWModule )
// Construction
public:
	CDlgHWModule(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
    void Save ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void RemoveCurrentConnection ();
	void SetCurrentProject (CProject *pPrj);
// Dialog Data
	//{{AFX_DATA(CDlgHWCanOpenNet)
	enum { IDD = IDD_DLG_HW_MODULE };
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void SaveData ();
	void InitPsw ();
	void InitializeGrid ();
	void InitTitles();
	void CreateFont ();
	void InitData ();
	void GetIPAddress (BYTE *achIPAddress);
	int  GetIPPort ();
	void SetIPAddress (BYTE *achIPAddress, int nPort);
	void SetModuleType (eTypeModule TypeModule);
	eTypeModule GetModuleType ();
private :
	CConfigModuleGrid	m_Grid;
	CStatic				m_stMachineTitle;
	CStatic				m_stIpAddress;
	CStatic				m_stPort;
	CIPAddressCtrl		m_ctrlIPAddress;
    CParsedEdit			m_EditIPPort;
	int					m_nKinemaAddress;
	bool				m_bInitialized;
	bool				m_bIPCtrlInitialized;
	ePswLevel			m_ePswLevel;
	CFont				*m_pFontModule;
	CProject			*m_pPrj;
	bool				m_bIPCtrlToggled;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeHwModuleEditPort();
	afx_msg void OnBnClickedHwModuleConnect();
	CButton m_ctrlConnect;
	afx_msg void OnIpnFieldchangedHwModuleIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWCANOPENNET_H__124E8896_B412_443B_AA22_449075C5F533__INCLUDED_)
