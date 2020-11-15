#pragma once

/* include files */
#include "stdafx.h"
#include "ParEdit.h"
#include "AdamoSynRTAxData.h"

class CAdamoSynRTAx : public CDialog, public CAdamoRTSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynRTAx)
	friend class CAdamoSynRTAxData;
	public:
		CAdamoSynRTAx(void);
		~CAdamoSynRTAx(void);
		BOOL Create ();
		BOOL Destroy ();
		virtual eRSCtrl KindOf ()	{
			return RSAx;
		}
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
		DECLARE_MESSAGE_MAP()
	private :
		void	 CreateFont ();
		void     InitButtons ();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnRadioAbs();
		afx_msg void OnRadioJog();
		afx_msg void OnRadioStep();
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		LONG OnRSAxButtonDown(UINT wParam, LONG lParam);
		LONG OnRSAxButtonUp(UINT wParam, LONG lParam);
		void ManageMinusButton (int nOp);
		void ManageStopButton ();
		void ManagePlusButton (int nOp);
		void ManageOpenLoop ();
		void ManageCloseLoop ();
		int GetSpeed (double* fpVel, int nShow);
		int GetLimits (bool* bLimitPos, bool* bLimitNeg, 
			double* fpLimitePositivo, double* fpLimiteNegativo);
		int GetTargetPosition (double* fpTargetPosition);
		int GetStep (double* fpStep);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);
	private :
		CWnd     *m_pParent;
		CFont    *m_pFont;
		CBitmap  *m_bmp;
		CBrush   *m_bkg;
		CxSkinButton m_butRadioJog;
		CxSkinButton m_butRadioStep;
		CxSkinButton m_butRadioAbs;
		CMDAxButton  m_butMeno;
		CMDAxButton  m_butStop;
		CMDAxButton  m_butPiu;
		CMDAxButton  m_butOpenLoop;
		CMDAxButton  m_butCloseLoop;
	    CParsedEdit  m_editAbs;
	    CParsedEdit  m_editVel;
		CAdamoSynRTAxData m_stData;
		bool		 m_bInitialized;
		double       m_fpQuotaTeorica;
		double       m_fpErroreAnello;
		double       m_fpCoppia;
		double       m_fpVelocita;
		double       m_fpAccelerazione;
		double       m_fpQuotaTeoricaVecchia;
		double       m_fpVelocitaVecchia;
		double       m_fpCoppiaVecchia;
		double       m_fpAccelerazioneVecchia;
		bool		 m_bLetto;
	public:
		virtual BOOL OnInitDialog();
};
