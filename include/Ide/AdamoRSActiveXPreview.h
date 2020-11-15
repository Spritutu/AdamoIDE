#pragma once

#include "AdamoRSObjects.h"
#include "iangulargaugex.h"
#include "CiLinearGaugeX.h"
#include "CiLedRectangleX.h"
#include "CiLedDiamondX.h"
#include "CiLedRoundX.h"
#include "CiSevenSegmentAnalogX.h"
#include "CiLabelX.h"
#include "paneview.h"

#define ACIVEX_PREVIEW_ID 100

// visualizzazione CAdamoRSActiveXPreview

class CAdamoRSActiveXPreview : public CPaneView
{
	DECLARE_DYNCREATE(CAdamoRSActiveXPreview)

protected:
	CAdamoRSActiveXPreview();           // costruttore protetto utilizzato dalla creazione dinamica
	virtual ~CAdamoRSActiveXPreview();

public:
	void SetRSControl (CAdamoRSBase *pRSControl);
	void UpdateProperties ();
	void UpdateSize ();
	virtual void OnDraw(CDC* pDC);      // sottoposto a override per la creazione di questa visualizzazione
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
private :
	CAdamoRSBase    *m_pRSObj;
	CWnd			*m_pRSCtrl;
	COLORREF		m_brBackColor;
private :
	void CreateObj (CAdamoRSBase *pRSObj);
	void CreateIAngularGauge (CAdamoRSBase *pRSObj);
	void CreateILinearGauge (CAdamoRSBase *pRSObj);
	void CreateILed (CAdamoRSBase *pRSObj);
	void CreateISevenSegment (CAdamoRSBase *pRSObj);
	void CreateILabel (CAdamoRSBase *pRSObj);
	void Clear ();
	void UpdateIAngularGaugeProperties ();
	void UpdateILedProperties ();
	void UpdateILedRectangular ();
	void UpdateILedDiamond ();
	void UpdateILedCircular ();
	void UpdateILinearGaugeProperties ();
	void UpdateIDisplayProperties ();
	void UpdateILabelProperties ();
	void ResizeControl ();
	void RefreshMetrics(AppTheme nTheme);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


