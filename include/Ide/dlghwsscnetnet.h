#if !defined(AFX_DLGHWSSCNETNET_H__A7B7B9A3_CB28_4156_9A59_91F50420013E__INCLUDED_)
#define AFX_DLGHWSSCNETNET_H__A7B7B9A3_CB28_4156_9A59_91F50420013E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlghwsscnetnet.h : header file
//

#include "ConfigDoc.h"
#include "afxwin.h"
#include "SSCNETConf.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHWSSCNETNet dialog

class CDlgHWSSCNETNet : public CDialog
{
    DECLARE_DYNAMIC (CDlgHWSSCNETNet)
// Construction
public:
	CDlgHWSSCNETNet(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
    void SetModuleNumber (int nModuleNumber)    { m_nModuleNumber=nModuleNumber; }
    int  GetModuleNumber ()                     { return m_nModuleNumber; }
    void Save ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }		
// Dialog Data
	//{{AFX_DATA(CDlgHWSSCNETNet)
	enum { IDD = IDD_DLG_HW_SSCNETNET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHWSSCNETNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHWSSCNETNet)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void SetConfigDoc ();
    void InitTitles ();
    void InitData ();
    void SaveData ();
    void InitPsw ();
private :
    CConfigDoc     *m_pConfigDoc;
    CSSCNETConf     m_stSSCNETConf;
    int             m_nKinemaAddress;
	int				m_nModuleNumber;
    ePswLevel       m_ePswLevel;
	CProject	   *m_pPrj;
public:
	afx_msg void OnBnClickedCheckEnableWarnings();
	afx_msg void OnBnClickedCheckDisableNetwork();
    afx_msg void OnBnClickedSscnetTypeJ3();
    afx_msg void OnBnClickedSscnetTypeJ4();
    afx_msg void OnBnClickedBtnMonoax();
    afx_msg void OnBnClickedBtnBiax();
    afx_msg void OnBnClickedBtnTriax();
    afx_msg void OnBnClickedBtnDelete();
    afx_msg void OnBnClickedBtnJ3J4();
    afx_msg void OnBnClickedBtnJ4J3();
    afx_msg void OnBnClickedBtnRestore();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWSSCNETNET_H__A7B7B9A3_CB28_4156_9A59_91F50420013E__INCLUDED_)
