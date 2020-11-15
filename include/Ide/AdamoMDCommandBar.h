//{{AFX_INCLUDES()
#include "iknobx.h"
//}}AFX_INCLUDES
#if !defined(AFX_ADAMOMDCOMMANDBAR_H__0D7A6979_2134_4532_9E37_7E5E3FC41C50__INCLUDED_)
#define AFX_ADAMOMDCOMMANDBAR_H__0D7A6979_2134_4532_9E37_7E5E3FC41C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDCommandBar.h : header file
//
                         
#include "AdamoMDAxButton.h"
#include "ide2.h"
#include "AdamoAxis.h"
#include "paredit.h"
#include "xSkinButton.h"
#include "transparentstatic.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDCommandBar dialog

class CAdamoMDCommandBar : public CDialog
{
// Construction
public:
	CAdamoMDCommandBar(CWnd* pParent = NULL);   // standard constructor
    void SetAx (CAdamoAxis *pAx, int nMode);
    void SetMachine (CAdamoMachine* pMachine) { m_pMachine=pMachine; }
    void OnViewSettings (stMDSettings* MDS);
    void GetViewSettings (stMDSettings* MDS);
    void PushPlusButton ();
    void ReleasePlusButton ();
    void PushMinusButton ();
    void ReleaseMinusButton ();
    void PushStopButton ();
    void ReleaseStopButton ();
    void SetVel ();
    void SetJogMode ();
    void SetAbsMode ();
    void SetStepMode ();
    void PushOpenLoopButton ();
    void PushCloseLoopButton ();
    void ReleaseOpenLoopButton ();
    void ReleaseCloseLoopButton ();
    int  GetSpeed (double* fpVel, int nShow);
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void UpdateKnob (int nMode);

// Dialog Data
	//{{AFX_DATA(CAdamoMDCommandBar)
	enum { IDD = IDD_DLGCOMMANDBAR };
	CiKnobX	m_knobSpeed;
    CxSkinButton m_butRadioJog;
    CxSkinButton m_butRadioStep;
    CxSkinButton m_butRadioAbs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDCommandBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoMDCommandBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRadioAbs();
	afx_msg void OnRadioJog();
	afx_msg void OnRadioStep();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPositionChangeIknobSpeed();
	afx_msg void OnMouseUpIknobSpeed(short Button, short Shift, long X, long Y);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
    LONG OnMDButtonDown(UINT wParam, LONG lParam);
    LONG OnMDButtonUp(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
private :
    CMDAxButton m_butMeno;
    CMDAxButton m_butStop;
    CMDAxButton m_butPiu;
    CMDAxButton m_butOpenLoop;
    CMDAxButton m_butCloseLoop;
    CParsedEdit m_editAbs;
    CParsedEdit m_editVel;
    CAdamoAxis* m_pAx;
    CAdamoMachine* m_pMachine;
    stMDSettings* m_pSettings;
    CBrush      m_bkg;
    int         m_nMode;
    ePswLevel   m_ePswLevel;
	CFont*		m_stFont;
private :
    void ManageMinusButton (int nOp);
    void ManageStopButton ();
    void ManagePlusButton (int nOp);
    void ManageOpenLoop ();
    void ManageCloseLoop ();
    int  GetLimits (bool* bLimitPos, bool* bLimitNeg, 
        double* fpLimitePositivo, double* fpLimiteNegativo);
    int GetTargetPosition (double* fpTargetPosition);
    int GetStep (double* fpStep);
    void OnOK ();
    void OnCancel ();
    BOOL PreTranslateMessage(MSG* pMsg);
    void StoreData (CAdamoAxis* pAx);
    void InitButtons ();
    void SendAxDataDouble (int nType, double fpValue);
    void GestionePassword ();
    void UpdateData (CAdamoAxis* pAx, int nMode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDCOMMANDBAR_H__0D7A6979_2134_4532_9E37_7E5E3FC41C50__INCLUDED_)
