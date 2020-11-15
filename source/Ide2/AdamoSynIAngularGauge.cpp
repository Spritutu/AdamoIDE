#include "stdafx.h"
#include "font.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynIAngularGauge.h"

CAdamoSynIAngularGauge::CAdamoSynIAngularGauge(void)
{
}

CAdamoSynIAngularGauge::~CAdamoSynIAngularGauge(void)
{
}

/*
** Create :
*/
int CAdamoSynIAngularGauge::Create ()
{
	BOOL b = __super::Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), m_pParent, GetID ());
	if (b)   {
		CRect rc = GetObjStyle ().GetRect ();
		UpdateProperties ();
		MoveWindow (rc);
	}
	return 0;
}

/*
** Destroy :
*/
int CAdamoSynIAngularGauge::Destroy ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynIAngularGauge::OnLoad ()
{
	return 0;
}

/*
** OnSave :
*/
int CAdamoSynIAngularGauge::OnSave ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynIAngularGauge::OnDownload ()
{
	return 0;
}

/*
** OnTimer :
*/
int CAdamoSynIAngularGauge::OnTimer ()
{
	CAdamoData ad;
	if (GetDeviceType () != enBase && GetLogicAddress () != -1)   {
		GETSYNCONT ()->RequestRTData (GetDeviceType (), GetLogicAddress (), GetData (), LUA_TNUMBER, ad);
		if (ad.nType == LUA_TNUMBER)
			SetPosition (ad.AdamoData.fp);
	}
	else   {
		CAdamoRSVarControl &rsVar = GetObjVar ();
		if (rsVar.GetObjVar ().m_strName != "")   {
			GETSYNCONT ()->RequestVarData (&rsVar, LUA_TNUMBER, ad);
			if (ad.nType == LUA_TNUMBER)
				SetPosition (ad.AdamoData.fp);
		}
	}
	return 0;
}

/*
** UpdateProperties :
*/
int CAdamoSynIAngularGauge::UpdateProperties ()
{
	COleFont fnt;
	CY cy;
	CiAngularGaugeX *pAG = (CiAngularGaugeX *) this;
	/* general */
	pAG->SetTransparent (IsTransparent ());
	pAG->SetPositionMin (GetPosMin ());
	pAG->SetPositionMax (GetPosMax ());
	pAG->SetShowInnerArcRadius (IsInnerRadius ());
	pAG->SetShowOuterArcRadius (IsOuterRadius ());
	pAG->SetReverseScale (IsReverseScale ());
	pAG->SetBorderStyle (GetBorderStyle ());
	pAG->SetBackGroundColor (GetBackGroundColor ());
	/* pointers */
	pAG->SetPointerStyle (GetPointerStyle ());
	pAG->SetPointerSize (GetPointerSize ());
	pAG->SetPointerMargin (GetPointerMargin ());
	pAG->SetPointersColor (0, GetPointerColor ());
	/* arc */
	pAG->SetArcStartDegrees (GetArcStartAngle ());
	pAG->SetArcRangeDegrees (GetArcAngle ());
	pAG->SetArcRadius (GetArcRadius ());
	/* hub */
	pAG->SetShowHub (IsHub ());
	pAG->SetHub3D (IsHub3D ());
	pAG->SetHubSize (GetHubSize ());
	pAG->SetHubColor (GetHubColor ());
	/* face */
	pAG->SetShowFace (IsFace ());
	pAG->SetFaceStyle (GetFaceStyle ());
	pAG->SetFaceBevelStyle (GetBevelStyle ());
	pAG->SetFaceBevelSize (GetBevelSize ());
	pAG->SetFaceColor (GetFaceColor ());
	/* tick */
	pAG->SetTickMargin (GetTickMargin ());
	pAG->SetTickLabelMargin (GetTickLabelMargin ());
	CAdamoRSStyle style = GetObjStyle ();
	fnt = pAG->GetTickLabelFont ();
	fnt.SetName (style.GetRSFont ().strFaceName);
	cy.int64 = style.GetRSFont ().nSize * 10000;
	fnt.SetSize (cy);
	fnt.SetItalic (style.GetRSFont ().bItalic);
	fnt.SetBold (style.GetRSFont ().bBold),
	fnt.SetUnderline (style.GetRSFont ().bUnderLine);
	fnt.SetStrikethrough (style.GetRSFont ().bStrikeOut);
	pAG->SetTickLabelFont (fnt);
	pAG->SetShowTicksMajor (IsTickMajor ());
	pAG->SetTickMajorCount (GetTickMajorNumber ());
	pAG->SetTickMajorLength (GetTickMajorLenght ());
	pAG->SetTickMajorColor (GetTickMajorColor ());
	pAG->SetShowTicksMinor (IsTickMinor ());
	pAG->SetTickMinorCount (GetTickMinorNumber ());
	pAG->SetTickMinorLength (GetTickMinorLenght ());
	pAG->SetTickMinorColor (GetTickMinorColor ());
	/* sections */
	pAG->SetSectionCount (GetSectionsNumber ());
	pAG->SetSectionColor1 (GetSection1Color ());
	pAG->SetSectionColor2 (GetSection2Color ());
	pAG->SetSectionColor3 (GetSection3Color ());
	pAG->SetSectionColor4 (GetSection4Color ());
	pAG->SetSectionColor5 (GetSection5Color ());
	pAG->SetSectionEnd1 (GetEndSection1 ());
	pAG->SetSectionEnd2 (GetEndSection2 ());
	pAG->SetSectionEnd3 (GetEndSection3 ());
	pAG->SetSectionEnd4 (GetEndSection4 ());
	return 0;
}
