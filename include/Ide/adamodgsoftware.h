#if !defined(AFX_ADAMODGSOFTWARE_H__8FD6EA4A_7115_417D_9759_74F52A0F448D__INCLUDED_)
#define AFX_ADAMODGSOFTWARE_H__8FD6EA4A_7115_417D_9759_74F52A0F448D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGSoftware.h : header file
//

/* include files */
#include "CiLedRectangleX.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSoftware dialog

class CAdamoDGSoftware : public CDialog
{
// Construction
public:
	CAdamoDGSoftware(CWnd* pParent = NULL);   // standard constructor
    void RinfrescaInformazioni ();
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Dialog Data
	//{{AFX_DATA(CAdamoDGSoftware)
	enum { IDD = IDD_DLG_DG_SOFTWARE };
	CStatic	m_stDCCorrections;
	CStatic	m_stDynPacketSend;
	CStatic	m_stDynPacketsReceived;
	CStatic	m_stDynPacketsDifference;
	CStatic	m_stMaxTimersPeriod;
	CStatic	m_stTimersPeriod;
	CStatic	m_stMaxTimerBusy;
	CStatic	m_stAvgTimerBusy;
	CStatic	m_stTimersBusy;
	CStatic	m_stMaxTimerCan;
	CStatic	m_stAvgTimerCan;
	CStatic	m_stTimersCan;
	CStatic	m_stObjSDO;
	CStatic	m_stObjMS;
	CStatic	m_stObjLoop;
	CStatic	m_stObjEventi;
	CStatic	m_stObjAssi;
	CStatic	m_stStaticChunksUsati;
	CStatic	m_stStaticChunksLiberi;
	CStatic	m_stStaticBytesUsati;
	CStatic	m_stDynBytesUsati;
	CStatic	m_stStaticBytesLiberi;
	CStatic	m_stStaticByteTotali;
	CStatic	m_stDynCunksUsati;
	CStatic	m_stDynChunksLiberi;
	CStatic	m_stDynBytesLiberi;
	CStatic	m_stDynByteTotali;
	CStatic	m_stStaticHeapSize;
	CStatic	m_stDynamicHeapSize;
	CButton	m_btnDebugPar8;
	CButton	m_btnDebugPar7;
	CButton	m_btnDebugPar6;
	CButton	m_btnDebugPar4;
	CButton	m_btnDebugPar5;
	CButton	m_btnDebugPar3;
	CButton	m_btnDebugPar2;
	CButton	m_btnDebugPar1;
	CButton	m_btnResetTimer;
	CButton	m_btnResetBusy;
	CButton	m_btnResetCan;
	CiLedRectangleX m_ledCanOpen;
	CiLedRectangleX m_ledMatchAbsPos;
	CiLedRectangleX m_ledRestoreAbsPos;
	CiLedRectangleX m_ledConfigurazione;
	CiLedRectangleX m_ledEthercat;
	CiLedRectangleX m_ledDCEthercat;
	CiLedRectangleX m_ledProtectionPlc;
	CStatic	m_stCanOpen;
	CStatic	m_stMatchAbsPos;
	CStatic	m_stRestoreAbsPos;
	CStatic	m_stConfigurazione;
	CStatic	m_stEthercat;
	CStatic	m_stDCEthercat;
	CStatic	m_stProtectionPlc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGSoftware)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoDGSoftware)
	afx_msg void OnBtnDebugWrite1();
	afx_msg void OnBtnDebugWrite2();
	afx_msg void OnBtnDebugWrite3();
	afx_msg void OnBtnDebugWrite4();
	afx_msg void OnBtnDebugWrite5();
	afx_msg void OnBtnDebugWrite6();
	afx_msg void OnBtnDebugWrite7();
	afx_msg void OnBtnDebugWrite8();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnResetTimer();
	afx_msg void OnBtnResetBusy();
	afx_msg void OnBtnResetCan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void SetMachine ();
	void ManageInitialization (int nInitTest, int nInitStatus);
	void GestionePsw ();
private :
    CAdamoMachine*  m_pMachine;
    CBrush          m_brWhite;
    ePswLevel		m_ePswLevel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGSOFTWARE_H__8FD6EA4A_7115_417D_9759_74F52A0F448D__INCLUDED_)
