#if !defined(AFX_ADAMODGETHERCAT_H__98762E91_B253_4AB0_BE11_27C7C8DE9199__INCLUDED_)
#define AFX_ADAMODGETHERCAT_H__98762E91_B253_4AB0_BE11_27C7C8DE9199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGEthercat.h : header file
//

#include "AdamoEthercatGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGEthercat view

class CAdamoDGEthercat : public CView
{
protected:
	CAdamoDGEthercat();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDGEthercat)

// Attributes
public:

// Operations
public:
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGEthercat)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public :
	void OnAdamoTimer ();
	void SetEcMode (int nMode)	{ m_bSingleSlave=nMode!=0; }
protected:
	virtual ~CAdamoDGEthercat();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDGEthercat)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnEthercatInit();
	afx_msg void OnBtnEthercatInitNetwork();
	afx_msg void OnBtnEthercatOperational();
	afx_msg void OnBtnEthercatProperational();
	afx_msg void OnBtnEthercatSafeOperational();
	afx_msg void OnUpdateBtnEthercatInit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatInitNetwork(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatOperational(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatProperational(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatSafeOperational(CCmdUI* pCmdUI);
	afx_msg void OnBtnEthercatReadyOn();
	afx_msg void OnBtnEthercatReadyOff();
	afx_msg void OnBtnEthercatServoOn();
	afx_msg void OnBtnEthercatServoOff();
	afx_msg void OnBtnEthercatResetAlarm();
	afx_msg void OnUpdateBtnEthercatReadyOn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatReadyOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatServoOn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatServoOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnEthercatResetAlarm(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CAdamoMachine		*m_pMachine;
    CAdamoEthercatGrid	*m_pEthercatGrid;
	bool				 m_bSingleSlave;
    ePswLevel		     m_ePswLevel;
private :
	void SetMachine ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGETHERCAT_H__98762E91_B253_4AB0_BE11_27C7C8DE9199__INCLUDED_)
