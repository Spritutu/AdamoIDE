// AdamoRSActiveXPreview.cpp : file di implementazione
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoRSActiveXPreview.h"
#include "font.h"


// CAdamoRSActiveXPreview

IMPLEMENT_DYNCREATE(CAdamoRSActiveXPreview, CPaneView)

CAdamoRSActiveXPreview::CAdamoRSActiveXPreview() : m_pRSObj (NULL), m_pRSCtrl (NULL), m_brBackColor (RGB (255, 255, 255))
{
}

CAdamoRSActiveXPreview::~CAdamoRSActiveXPreview()
{
	Clear ();
}

BEGIN_MESSAGE_MAP(CAdamoRSActiveXPreview, CPaneView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND ()
END_MESSAGE_MAP()


// disegno di CAdamoRSActiveXPreview

void CAdamoRSActiveXPreview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: aggiungere qui il codice di disegno
}


// diagnostica di CAdamoRSActiveXPreview

#ifdef _DEBUG
void CAdamoRSActiveXPreview::AssertValid() const
{
	CPaneView::AssertValid();
}

#ifndef _WIN32_WCE
void CAdamoRSActiveXPreview::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif
#endif //_DEBUG


// gestori di messaggi CAdamoRSActiveXPreview
void CAdamoRSActiveXPreview::SetRSControl (CAdamoRSBase *pRSObj)
{
	if (m_pRSObj != pRSObj)   {
		Clear ();
		m_pRSObj = pRSObj;
		if (pRSObj)
			CreateObj (pRSObj);
	}
}

/*
** CreateObj :
*/
void CAdamoRSActiveXPreview::CreateObj (CAdamoRSBase *pRSObj)
{
	switch (pRSObj->KindOf ())   {
		case RSILed :
			CreateILed (pRSObj);
			UpdateProperties ();
			ResizeControl ();
			break;
		case RSIAngularGauge :
			CreateIAngularGauge (pRSObj);
			UpdateProperties ();
			ResizeControl ();
			break;
		case RSILinearGauge :
			CreateILinearGauge (pRSObj);
			UpdateProperties ();
			ResizeControl ();
			break;
		case RSIDisplay :
			CreateISevenSegment (pRSObj);
			UpdateProperties ();
			ResizeControl ();
			break;
		case RSILabel :
			CreateILabel (pRSObj);
			UpdateProperties ();
			ResizeControl ();
			break;
	}
}

/*
** CreateILed :
*/
void CAdamoRSActiveXPreview::CreateILed (CAdamoRSBase *pRSObj)
{
	switch (((CAdamoRSILed *)pRSObj)->GetLedType ())   {
		case eRectangular :
			m_pRSCtrl = new CiLedRectangleX;
			((CiLedRectangleX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
			break;
		case eDiamond :
			m_pRSCtrl = new CiLedDiamondX;
			((CiLedDiamondX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
			break;
		case eCircular :
			m_pRSCtrl = new CiLedRoundX;
			((CiLedRoundX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
			break;
	}
}

/*
** CreateIAngularGauge :
*/
void CAdamoRSActiveXPreview::CreateIAngularGauge (CAdamoRSBase *pRSObj)
{
	m_pRSCtrl = new CiAngularGaugeX;
	((CiAngularGaugeX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
}

/*
** CreateILinearGauge :
*/
void CAdamoRSActiveXPreview::CreateILinearGauge (CAdamoRSBase *pRSObj)
{
	m_pRSCtrl = new CiLinearGaugeX;
	((CiLinearGaugeX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
}

/*
** CreateISevenSegment :
*/
void CAdamoRSActiveXPreview::CreateISevenSegment (CAdamoRSBase *pRSObj)
{
	m_pRSCtrl = new CiSevenSegmentAnalogX;
	((CiSevenSegmentAnalogX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
}

/*
** CreateILabel :
*/
void CAdamoRSActiveXPreview::CreateILabel (CAdamoRSBase *pRSObj)
{
	m_pRSCtrl = new CiLabelX;
	((CiLabelX *)m_pRSCtrl)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), this, ACIVEX_PREVIEW_ID);
}

/*
** UpdateProperties :
*/
void CAdamoRSActiveXPreview::UpdateProperties ()
{
	if (m_pRSCtrl && m_pRSObj)   {
		switch (m_pRSObj->KindOf ())   {
			case RSILed :
				UpdateILedProperties ();
				break;
			case RSIAngularGauge :
				UpdateIAngularGaugeProperties ();
				break;
			case RSILinearGauge :
				UpdateILinearGaugeProperties ();
				break;
			case RSIDisplay :
				UpdateIDisplayProperties ();
				break;
			case RSILabel :
				UpdateILabelProperties ();
				break;
		}
	}
}

/*
** UpdateSize :
*/
void CAdamoRSActiveXPreview::UpdateSize ()
{
	if (m_pRSCtrl && m_pRSObj)
		ResizeControl ();
}

/*
** UpdateIAngularGaugeProperties
*/
void CAdamoRSActiveXPreview::UpdateIAngularGaugeProperties ()
{
	COleFont fnt;
	CY cy;
	CiAngularGaugeX *pAG = (CiAngularGaugeX *) m_pRSCtrl;
	CAdamoRSIAngularGauge *pRSAG = (CAdamoRSIAngularGauge *) m_pRSObj;
	/* general */
	pAG->SetTransparent (pRSAG->IsTransparent ());
	pAG->SetPositionMin (pRSAG->GetPosMin ());
	pAG->SetPositionMax (pRSAG->GetPosMax ());
	pAG->SetShowInnerArcRadius (pRSAG->IsInnerRadius ());
	pAG->SetShowOuterArcRadius (pRSAG->IsOuterRadius ());
	pAG->SetReverseScale (pRSAG->IsReverseScale ());
	pAG->SetBorderStyle (pRSAG->GetBorderStyle ());
	pAG->SetBackGroundColor (pRSAG->GetBackGroundColor ());
	/* pointers */
	pAG->SetPointerStyle (pRSAG->GetPointerStyle ());
	pAG->SetPointerSize (pRSAG->GetPointerSize ());
	pAG->SetPointerMargin (pRSAG->GetPointerMargin ());
	pAG->SetPointersColor (0, pRSAG->GetPointerColor ());
	/* arc */
	pAG->SetArcStartDegrees (pRSAG->GetArcStartAngle ());
	pAG->SetArcRangeDegrees (pRSAG->GetArcAngle ());
	pAG->SetArcRadius (pRSAG->GetArcRadius ());
	/* hub */
	pAG->SetShowHub (pRSAG->IsHub ());
	pAG->SetHub3D (pRSAG->IsHub3D ());
	pAG->SetHubSize (pRSAG->GetHubSize ());
	pAG->SetHubColor (pRSAG->GetHubColor ());
	/* face */
	pAG->SetShowFace (pRSAG->IsFace ());
	pAG->SetFaceStyle (pRSAG->GetFaceStyle ());
	pAG->SetFaceBevelStyle (pRSAG->GetBevelStyle ());
	pAG->SetFaceBevelSize (pRSAG->GetBevelSize ());
	pAG->SetFaceColor (pRSAG->GetFaceColor ());
	/* tick */
	pAG->SetTickMargin (pRSAG->GetTickMargin ());
	pAG->SetTickLabelMargin (pRSAG->GetTickLabelMargin ());
	CAdamoRSStyle style = pRSAG->GetObjStyle ();
	fnt = pAG->GetTickLabelFont ();
	fnt.SetName (style.GetRSFont ().strFaceName);
	cy.int64 = style.GetRSFont ().nSize * 10000;
	fnt.SetSize (cy);
	fnt.SetItalic (style.GetRSFont ().bItalic);
	fnt.SetBold (style.GetRSFont ().bBold),
	fnt.SetUnderline (style.GetRSFont ().bUnderLine);
	fnt.SetStrikethrough (style.GetRSFont ().bStrikeOut);
	pAG->SetTickLabelFont (fnt);
	pAG->SetShowTicksMajor (pRSAG->IsTickMajor ());
	pAG->SetTickMajorCount (pRSAG->GetTickMajorNumber ());
	pAG->SetTickMajorLength (pRSAG->GetTickMajorLenght ());
	pAG->SetTickMajorColor (pRSAG->GetTickMajorColor ());
	pAG->SetShowTicksMinor (pRSAG->IsTickMinor ());
	pAG->SetTickMinorCount (pRSAG->GetTickMinorNumber ());
	pAG->SetTickMinorLength (pRSAG->GetTickMinorLenght ());
	pAG->SetTickMinorColor (pRSAG->GetTickMinorColor ());
	/* sections */
	pAG->SetSectionCount (pRSAG->GetSectionsNumber ());
	pAG->SetSectionColor1 (pRSAG->GetSection1Color ());
	pAG->SetSectionColor2 (pRSAG->GetSection2Color ());
	pAG->SetSectionColor3 (pRSAG->GetSection3Color ());
	pAG->SetSectionColor4 (pRSAG->GetSection4Color ());
	pAG->SetSectionColor5 (pRSAG->GetSection5Color ());
	pAG->SetSectionEnd1 (pRSAG->GetEndSection1 ());
	pAG->SetSectionEnd2 (pRSAG->GetEndSection2 ());
	pAG->SetSectionEnd3 (pRSAG->GetEndSection3 ());
	pAG->SetSectionEnd4 (pRSAG->GetEndSection4 ());
}

/*
** UpdateILinearGaugeProperties :
*/
void CAdamoRSActiveXPreview::UpdateILinearGaugeProperties ()
{
	COleFont fnt;
	CY cy;
	CiLinearGaugeX *pLG = (CiLinearGaugeX *) m_pRSCtrl;
	CAdamoRSILinearGauge *pRSLG = (CAdamoRSILinearGauge *) m_pRSObj;
	/* general */
	pLG->put_PositionMin  (pRSLG->GetPosMin ());
	pLG->put_PositionMax  (pRSLG->GetPosMax ());
	pLG->put_Transparent  (pRSLG ->IsTransparent ());
	pLG->put_ReverseScale (pRSLG->IsReverseScale ());
	pLG->put_Orientation  (pRSLG->GetOrientation ());
	pLG->put_OrientationTickMarks (pRSLG->GetOrientationTicks ());
	pLG->put_BorderStyle  (pRSLG->GetBorderStyle ());
	pLG->put_BackGroundColor (pRSLG->GetBackGroundColor ());
	/* pointers */
	pLG->put_PointerStyle (pRSLG->GetPointerStyle ());
	pLG->put_PointerSize  (pRSLG->GetPointerSize ());
	pLG->put_PointerOffSet(pRSLG->GetPointerOffset ());
	pLG->put_PointerColor (pRSLG->GetPointerColor ());
	pLG->SetPointersStyle3D (0, pRSLG->GetPointer3D ());
	pLG->SetPointersDrawScaleSide (0, pRSLG->GetDrawScaleSide ());
	/* ticks */
	pLG->put_ShowTicksAxis (pRSLG->IsTickAxes ());
	pLG->put_TickMargin (pRSLG->GetTickMargin ());
	/* labels */
	pLG->put_ShowTickLabels (pRSLG->IsShowLabels());
	pLG->put_TickLabelMargin (pRSLG->GetLabelMargin  ());
	CAdamoRSStyle style = pRSLG->GetObjStyle ();
	fnt = pLG->get_TickLabelFont ();
	fnt.SetName (style.GetRSFont ().strFaceName);
	cy.int64 = style.GetRSFont ().nSize * 10000;
	fnt.SetSize (cy);
	fnt.SetItalic (style.GetRSFont ().bItalic);
	fnt.SetBold (style.GetRSFont ().bBold),
	fnt.SetUnderline (style.GetRSFont ().bUnderLine);
	fnt.SetStrikethrough (style.GetRSFont ().bStrikeOut);
	pLG->put_TickLabelFont (fnt);
	/* ticks major */
	pLG->put_ShowTicksMajor (pRSLG->IsTickMajor());
	pLG->put_TickMajorCount (pRSLG->GetTickMajorNumber ());
	pLG->put_TickMajorLength (pRSLG->GetTickMajorLenght ());
	pLG->put_TickMajorColor (pRSLG->GetTickMajorColor ());
	/* ticks minor */
	pLG->put_ShowTicksMinor (pRSLG->IsTickMinor());
	pLG->put_TickMinorCount (pRSLG->GetTickMinorNumber ());
	pLG->put_TickMinorLength (pRSLG->GetTickMinorLenght ());
	pLG->put_TickMinorColor (pRSLG->GetTickMinorColor ());
	/* sections */
	pLG->put_SectionCount (pRSLG->GetSectionsNumber ());
	pLG->put_SectionColor1 (pRSLG->GetSection1Color ());
	pLG->put_SectionColor2 (pRSLG->GetSection2Color ());
	pLG->put_SectionColor3 (pRSLG->GetSection3Color ());
	pLG->put_SectionColor4 (pRSLG->GetSection4Color ());
	pLG->put_SectionColor5 (pRSLG->GetSection5Color ());
	pLG->put_SectionEnd1 (pRSLG->GetEndSection1 ());
	pLG->put_SectionEnd2 (pRSLG->GetEndSection2 ());
	pLG->put_SectionEnd3 (pRSLG->GetEndSection3 ());
	pLG->put_SectionEnd4 (pRSLG->GetEndSection4 ());
}

/*
** UpdateIDisplayProperties
*/
void CAdamoRSActiveXPreview::UpdateIDisplayProperties ()
{
	CiSevenSegmentAnalogX *pDD = (CiSevenSegmentAnalogX *) m_pRSCtrl;
	CAdamoRSIDisplay *pRSDD = (CAdamoRSIDisplay *) m_pRSObj;
	/* general */
	pDD->put_ShowOffSegments (pRSDD->IsOffSegment ());
	pDD->put_Transparent (pRSDD->IsTransparent());
	pDD->put_ShowSign (pRSDD->IsSign());
	pDD->put_AutoSegmentOffColor (pRSDD->IsAutoSegmentOffColor());
	pDD->put_Precision (pRSDD->GetPrecision ());
	pDD->put_LeadingStyle (pRSDD->GetLeadingStyle ());
	pDD->put_BorderStyle (pRSDD->GetDisplayStyle ());
	pDD->put_BackGroundColor (pRSDD->GetBackGroundColor ());
	/* display */
	pDD->put_DigitCount (pRSDD->GetDisplayCount ());
	pDD->put_DigitSpacing (pRSDD->GetDisplaySpacing ());
	pDD->put_SegmentSize (pRSDD->GetDisplaySize ());
	pDD->put_SegmentSeperation (pRSDD->GetDisplaySeparation ());
	pDD->put_SegmentMargin (pRSDD->GetDisplayMargin ());
	pDD->put_SegmentColor (pRSDD->GetDisplayColor ());
	pDD->put_SegmentOffColor (pRSDD->GetDisplayOffColor ());
	/* eseguiamo il resize */
	CRect rc;
	GetWindowRect (rc);
	OnSize (0, rc.right - rc.left, rc.bottom - rc.top);
}

/*
** UpdateILabelProperties
*/
void CAdamoRSActiveXPreview::UpdateILabelProperties ()
{
	COleFont fnt;
	CY cy;
	CiLabelX *pL = (CiLabelX *) m_pRSCtrl;
	CAdamoRSILabel *pRSL = (CAdamoRSILabel *) m_pRSObj;
	/* general */
	pL->put_Caption (pRSL->GetValue ());
	pL->put_Transparent  (pRSL->IsTransparent ());
	pL->put_AutoSize (pRSL->IsAutoSize ()); 
	pL->put_BorderStyle (pRSL->GetStyle ());
	pL->put_Alignment (pRSL->GetAlignment ());
	pL->put_ShadowShow (pRSL->IsShadow ());
	pL->put_ShadowXOffset (pRSL->GetOffsetX ());
	pL->put_ShadowYOffset (pRSL->GetOffsetY ());
	pL->put_FontColor (pRSL->GetColor ());
	pL->put_BackGroundColor (pRSL->GetBackColor ());
	pL->put_ShadowColor (pRSL->GetShadowColor ());
	CAdamoRSStyle style = pRSL->GetObjStyle ();
	fnt = pL->get_Font ();
	fnt.SetName (style.GetRSFont ().strFaceName);
	cy.int64 = style.GetRSFont ().nSize * 10000;
	fnt.SetSize (cy);
	fnt.SetItalic (style.GetRSFont ().bItalic);
	fnt.SetBold (style.GetRSFont ().bBold),
	fnt.SetUnderline (style.GetRSFont ().bUnderLine);
	fnt.SetStrikethrough (style.GetRSFont ().bStrikeOut);
	pL->put_Font (fnt);
}

/*
** Clear :
*/
void CAdamoRSActiveXPreview::Clear ()
{
	if (m_pRSCtrl)   {
		m_pRSCtrl->DestroyWindow ();
		_delete (m_pRSCtrl);
	}
}

/*
** OnSize
*/
void CAdamoRSActiveXPreview::OnSize(UINT nType, int cx, int cy)
{
	CPaneView::OnSize(nType, cx, cy);
	
	// TODO: aggiungere qui il codice per la gestione dei messaggi.
	if (m_pRSObj)   {
		int x = 0, y = 0;
		CRect rcCtrl = ((CAdamoRSControl *)m_pRSObj)->GetObjStyle ().GetRect ();
		if (rcCtrl.Width () < cx)
			x = (cx - rcCtrl.Width ()) / 2;
		if (rcCtrl.Height () < cy)
			y = (cy - rcCtrl.Height ()) / 2;
		if (m_pRSCtrl)
			m_pRSCtrl->MoveWindow (x, y, rcCtrl.Width (), rcCtrl.Height ());
	}
}

/*
** OnDestroy :
*/
void CAdamoRSActiveXPreview::OnDestroy()
{
	CPaneView::OnDestroy();

	// TODO: aggiungere qui il codice per la gestione dei messaggi.
	Clear ();
}

/*
** Resize :
*/
void CAdamoRSActiveXPreview::ResizeControl ()
{
	CRect rc;

	GetClientRect (&rc);
	OnSize (0, rc.Width (), rc.Height ());
}

/*
** UpdateIAngularGaugeProperties
*/
void CAdamoRSActiveXPreview::UpdateILedProperties ()
{
	switch (((CAdamoRSILed *)m_pRSObj)->GetLedType ())   {
		case eRectangular :
			UpdateILedRectangular ();
			break;
		case eDiamond :
			UpdateILedDiamond ();
			break;
		case eCircular :
			UpdateILedCircular ();
			break;
	}
}

/*
** UpdateILedRectangular :
*/
void CAdamoRSActiveXPreview::UpdateILedRectangular ()
{
	CiLedRectangleX *pLR = (CiLedRectangleX *) m_pRSCtrl;
	CAdamoRSILed *pRSLR = (CAdamoRSILed *) m_pRSObj;
	pLR->put_ShowReflection (pRSLR->GetShowReflection ());
	pLR->put_ActiveColor (pRSLR->GetActiveColor ());
	pLR->put_BevelStyle (pRSLR->GetBevelStyle ());
	pLR->put_BorderStyle (pRSLR->GetBorderStyle ());
	pLR->put_AutoInactiveColor (TRUE);
}

/*
**
*/
void CAdamoRSActiveXPreview::UpdateILedDiamond ()
{
	CiLedDiamondX *pLed = (CiLedDiamondX *) m_pRSCtrl;
	CAdamoRSILed *pRSLed = (CAdamoRSILed *) m_pRSObj;
	pLed->put_Transparent (pRSLed->GetTransparent ());
	pLed->put_ActiveColor (pRSLed->GetActiveColor ());
	pLed->put_BevelStyle (pRSLed->GetBevelStyle ());
	pLed->put_BorderStyle (pRSLed->GetBorderStyle ());
	pLed->put_BackGroundColor (pRSLed->GetBackgroundColor ());
	pLed->put_AutoInactiveColor (TRUE);
}

/*
** UpdateILedCircular :
*/
void CAdamoRSActiveXPreview::UpdateILedCircular ()
{
	CiLedRoundX *pLed = (CiLedRoundX *) m_pRSCtrl;
	CAdamoRSILed *pRSLed = (CAdamoRSILed *) m_pRSObj;
	pLed->put_Transparent (pRSLed->GetTransparent ());
	pLed->put_ShowReflection (pRSLed->GetShowReflection ());
	pLed->put_ActiveColor (pRSLed->GetActiveColor ());
	pLed->put_BevelStyle (pRSLed->GetBevelStyle ());
	pLed->put_BorderStyle (pRSLed->GetBorderStyle ());
	pLed->put_BackGroundColor (pRSLed->GetBackgroundColor ());
	pLed->put_AutoInactiveColor (TRUE);
}

/*
** OnEraseBkgnd :
*/
BOOL CAdamoRSActiveXPreview::OnEraseBkgnd (CDC* pDC)
{
	CBrush backBrush(m_brBackColor);
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox (&rect);
	pDC->PatBlt (rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject (pOldBrush);
	return TRUE;
}

/*
** OnRefreshMetrics :
*/
void CAdamoRSActiveXPreview::RefreshMetrics(AppTheme nTheme)
{
	switch (nTheme)
	{
		case themeVS2010 :
			m_brBackColor = RGB(0xFF,0xFF,0xFF);
			break;
		case themeVS2012Light :
			m_brBackColor = RGB(0xf5,0xf5,0xf5);
			break;
		case themeVS2012Dark :
			m_brBackColor = RGB(0x00,0x00,0x00);
			break;
		case themeVS2015Light :
			m_brBackColor = RGB(0xf5,0xf5,0xf5);
			break;
		case themeVS2015Dark :
			m_brBackColor = RGB(0x00,0x00,0x00);
			break;
		case themeVS2015Blue :
			m_brBackColor = RGB(0xFF,0xFF,0xFF);
			break;
	}	
	Invalidate ();
	return;
}
