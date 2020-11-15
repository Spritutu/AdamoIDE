//{{AFX_INCLUDES()
#include "iangulargaugex.h"
#include "islidingscalex.h"
#include "iledbarx.h"
#include "iswitchpanelx.h"

//}}AFX_INCLUDES
#if !defined(AFX_ADAMOMDAXSETTINGS_H__228EAC3A_D6B2_4BA4_807B_90A6B2ACDC21__INCLUDED_)
#define AFX_ADAMOMDAXSETTINGS_H__228EAC3A_D6B2_4BA4_807B_90A6B2ACDC21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDAxSettings.h : header file
//

/* include files */
#include "BitmapSlider.h"
#include "paredit.h"
#include "label.h"
#include "xSkinButton.h"

/* constants definitions */
#define TEMPORARY_TUNING "_Tuning.ada"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxSettings dialog

/* report control paint manager */
class CXTPAxSettinsPaintManager : public CXTPReportPaintManager
{
public:
	CXTPAxSettinsPaintManager();
	virtual ~CXTPAxSettinsPaintManager();
	virtual int GetRowHeight(CDC* pDC, CXTPReportRow* pRow);
	virtual void DrawFocusedRow(CDC* pDC, CRect rcRow);
};

class CAdamoMDAxSettings : public CDialog
{
// Construction
	friend class CXTPAxSettingsRecordItemPositions;
	friend class CXTPAxSettingsRecordItemStatus;
public:
	CAdamoMDAxSettings(CWnd* pParent = NULL);   // standard constructor
	~CAdamoMDAxSettings();
    void SetMachine (CAdamoMachine* pMachine) { m_pMachine=pMachine; }
    void SettaCommandBar (CAdamoMDCommandBar* pCmdBar)   { m_pCmdBar=pCmdBar; }
    void SetAx (CAdamoAxis *pAx)   { m_pAx=pAx; Ax=*m_pAx; }
    bool GetMode ()                { return m_bSingle; }
    CAdamoAxis *GetAx ()   { return m_pAx; }
    void OnViewSettings (stMDSettings* MDS);
    void GetViewSettings (stMDSettings* MDS);
    void OnAdamoTimer ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

// Dialog Data
	//{{AFX_DATA(CAdamoMDAxSettings)
	enum { IDD = IDD_DLGAXSETTINGS };
	CxSkinButton	m_btnStopTaratura;
	CxSkinButton	m_btnStartTaratura;
  	CBitmapSlider	m_sliderAcc;
  	CBitmapSlider	m_sliderDec;
  	CBitmapSlider	m_sliderProp;
  	CBitmapSlider	m_sliderDer;
  	CBitmapSlider	m_sliderInteg;
  	CBitmapSlider	m_sliderFF;
  	CBitmapSlider	m_sliderFFA;
    CLabel          m_lblQP;
    CLabel          m_lblQN;
    CLabel          m_lblRitardo;
    CLabel          m_lblAcc;
    CLabel          m_lblDec;
    CLabel          m_lblK;
    CLabel          m_lblD;
    CLabel          m_lblI;
    CLabel          m_lblFFW;
    CLabel          m_lblFFWA;
    CLabel          m_lblQLPos;
    CLabel          m_lblQLNeg;
    CLabel          m_lblViewK;
    CLabel          m_lblViewD;
    CLabel          m_lblViewI;
    CLabel          m_lblViewFFW;
    CLabel          m_lblViewFFWA;
    CLabel          m_lblEA;
	CiAngularGaugeX	m_gaugeAccelerazione;
	CiAngularGaugeX	m_gaugeCoppia;
	CiAngularGaugeX	m_gaugeVelTeorica;
	CiLedBarX	m_ledD;
	CiLedBarX	m_ledK;
	CiLedBarX	m_ledI;
	CiLedBarX	m_ledFFWA;
	CiLedBarX	m_ledFFW;
	CiLedBarX	m_ledEA;
	CiSwitchPanelX	m_plx;
	//}}AFX_DATA
	CParsedEdit	m_editQP;
	CParsedEdit	m_editQN;
	CParsedEdit	m_editProp;
	CParsedEdit	m_editLimEAPos;
	CParsedEdit	m_editInteg;
	CParsedEdit	m_editLimEANeg;
	CParsedEdit	m_editFFWA;
	CParsedEdit	m_editFFW;
	CParsedEdit	m_editDer;
	CParsedEdit	m_editDec;
	CParsedEdit	m_editDelay;
	CParsedEdit	m_editAcc;
    CParsedEdit	m_editSetQuota;
    CParsedEdit	m_editSetDAC;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDAxSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoMDAxSettings)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocusEditAcc();
	afx_msg void OnSetFocusEditDec();
	afx_msg void OnSetFocusEditDer();
	afx_msg void OnSetFocusEditFFW();
	afx_msg void OnSetFocusEditFFWA();
	afx_msg void OnSetFocusEditInteg();
	afx_msg void OnSetFocusEditLimEANeg();
	afx_msg void OnSetFocusEditLimEAPos();
	afx_msg void OnSetFocusEditProp();
	afx_msg void OnKillFocusEditAcc();
	afx_msg void OnKillFocusEditDec();
	afx_msg void OnKillFocusEditDer();
	afx_msg void OnKillFocusEditFFW();
	afx_msg void OnKillFocusEditFFWA();
	afx_msg void OnKillFocusEditInteg();
	afx_msg void OnKillFocusEditLimEANeg();
	afx_msg void OnKillFocusEditLimEAPos();
	afx_msg void OnKillFocusEditProp();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPositionChange();
	afx_msg void OnBtnStartTaratura();
	afx_msg void OnBtnStopTaratura();
	afx_msg void OnBtnSetQuota();
	afx_msg void OnBtnSetDac();
	afx_msg void OnBtnStore();
	afx_msg void OnBtnTrace();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private :
	CXTPReportControl m_wndReport;
    CAdamoMachine* m_pMachine;
    CAdamoMDCommandBar* m_pCmdBar;
    CAdamoAxis* m_pAx;
    CAdamoAxis  Ax;
	CBrush      m_br;
	CFont*		m_pDigitFont;
    bool        m_bSingle;
    bool        m_bModified;
    int         m_nEditFocused;
    CBrush      m_bkg;
    double         m_fpBackQuotaReale;
    double         m_fpBackQuotaTeorica;
    double         m_fpBackEA;
    double         m_fpBackVelocitaTeorica;
    double         m_fpBackAccelerazione;
    double         m_fpBackCoppia;
    int            m_nBackStDAC;
    int            m_nBackStStato;
    int            m_nBackStTipoMovimento;
    int            m_nBackStK;
    int            m_nBackStD;
    int            m_nBackStI;
    int            m_nBackStFFW;
    int            m_nBackStFFWA;
    bool           m_bInitialized;
    CString        *m_aszConfrontoAssi[5];
    CString        *m_aszTipoMovimento[11];
    ePswLevel      m_ePswLevel;
	bool		   m_bTimerOk;
	bool		   m_bAnalogic;
private :
    void UpdateAxData ();
    BOOL PreTranslateMessage(MSG* pMsg);
    BOOL GetEditValueInt (int nId, int nRangeMin, int nRangeMax, int* n);
    BOOL GetEditValueDouble (int nId, double fpRangeMin, double fpRangeMax, double* fp);
    void SendAxDataInt (int nType, int nValue);
    void SendAxDataDouble (int nType, double fpValue);
    void OnOK ();
    void OnCancel ();
    void SetAcceleration (int n);
    void SetDeceleration (int n);
    void SetProp (double fp);
    void SetDer (double fp);
    void SetInteg (double fp);
    void SetFFW (double fp);
    void SetFFWA (double fp);
    void SetLimEAPos (double fp);
    void SetLimEANeg (double fp);
    void InitLabels ();
    void InitAngularGauges ();
    bool GetMoveParameters (double* pfpQuotaNeg, double* pfpQuotaPos, double* pfpRitardo, double *fpSpeed);
    bool ComponiTemporary (double fpQuotaPos, double fpQuotaNeg, double fpRitardo, double fpSpeed);
	bool ComponiInterpTemporary (double fpQuotaPos, double fpQuotaNeg, double fpRitardo, double fpSpeed);
    void LanciaTemporary ();
    void EliminaTemporary ();
    void TerminaTemporary ();
    void InitStrings ();
    void DestroyStrings ();
    bool GestionePassword ();
	void UpdateLabelUM ();
	void InitReportControl ();
	void FillColumns ();
	void FillGrid ();
	void GestioneTipoAsse ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDAXSETTINGS_H__228EAC3A_D6B2_4BA4_807B_90A6B2ACDC21__INCLUDED_)
