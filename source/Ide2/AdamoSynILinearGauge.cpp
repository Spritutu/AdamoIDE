#include "stdafx.h"
#include "font.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynILinearGauge.h"

CAdamoSynILinearGauge::CAdamoSynILinearGauge(void)
{
}

CAdamoSynILinearGauge::~CAdamoSynILinearGauge(void)
{
}

/*
** Create :
*/
int CAdamoSynILinearGauge::Create ()
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
int CAdamoSynILinearGauge::Destroy ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynILinearGauge::OnLoad ()
{
	return 0;
}

/*
** OnSave :
*/
int CAdamoSynILinearGauge::OnSave ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynILinearGauge::OnDownload ()
{
	return 0;
}

/*
** OnTimer :
*/
int CAdamoSynILinearGauge::OnTimer ()
{
	CAdamoData ad;
	if (GetDeviceType () != enBase && GetLogicAddress () != -1)   {
		GETSYNCONT ()->RequestRTData (GetDeviceType (), GetLogicAddress (), GetData (), LUA_TNUMBER, ad);
		if (ad.nType == LUA_TNUMBER)
			put_Position (ad.AdamoData.fp);
	}
	else   {
		CAdamoRSVarControl &rsVar = GetObjVar ();
		if (rsVar.GetObjVar ().m_strName != "")   {
			GETSYNCONT ()->RequestVarData (&rsVar, LUA_TNUMBER, ad);
			if (ad.nType == LUA_TNUMBER)
				put_Position (ad.AdamoData.fp);
		}
	}
	return 0;
}

/*
** UpdateProperties :
*/
int CAdamoSynILinearGauge::UpdateProperties ()
{
	COleFont fnt;
	CY cy;
	CiLinearGaugeX *pLG = (CiLinearGaugeX *) this;
	/* general */
	pLG->put_PositionMin  (GetPosMin ());
	pLG->put_PositionMax  (GetPosMax ());
	pLG->put_Transparent  (IsTransparent ());
	pLG->put_ReverseScale (IsReverseScale ());
	pLG->put_Orientation  (GetOrientation ());
	pLG->put_OrientationTickMarks (GetOrientationTicks ());
	pLG->put_BorderStyle  (GetBorderStyle ());
	pLG->put_BackGroundColor (GetBackGroundColor ());
	/* pointers */
	pLG->put_PointerStyle (GetPointerStyle ());
	pLG->put_PointerSize  (GetPointerSize ());
	pLG->put_PointerOffSet(GetPointerOffset ());
	pLG->put_PointerColor (GetPointerColor ());
	pLG->SetPointersStyle3D (0, GetPointer3D ());
	pLG->SetPointersDrawScaleSide (0, GetDrawScaleSide ());
	/* ticks */
	pLG->put_ShowTicksAxis (IsTickAxes ());
	pLG->put_TickMargin (GetTickMargin ());
	/* labels */
	pLG->put_ShowTickLabels (IsShowLabels());
	pLG->put_TickLabelMargin (GetLabelMargin  ());
	CAdamoRSStyle style = GetObjStyle ();
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
	pLG->put_ShowTicksMajor (IsTickMajor());
	pLG->put_TickMajorCount (GetTickMajorNumber ());
	pLG->put_TickMajorLength (GetTickMajorLenght ());
	pLG->put_TickMajorColor (GetTickMajorColor ());
	/* ticks minor */
	pLG->put_ShowTicksMinor (IsTickMinor());
	pLG->put_TickMinorCount (GetTickMinorNumber ());
	pLG->put_TickMinorLength (GetTickMinorLenght ());
	pLG->put_TickMinorColor (GetTickMinorColor ());
	/* sections */
	pLG->put_SectionCount (GetSectionsNumber ());
	pLG->put_SectionColor1 (GetSection1Color ());
	pLG->put_SectionColor2 (GetSection2Color ());
	pLG->put_SectionColor3 (GetSection3Color ());
	pLG->put_SectionColor4 (GetSection4Color ());
	pLG->put_SectionColor5 (GetSection5Color ());
	pLG->put_SectionEnd1 (GetEndSection1 ());
	pLG->put_SectionEnd2 (GetEndSection2 ());
	pLG->put_SectionEnd3 (GetEndSection3 ());
	pLG->put_SectionEnd4 (GetEndSection4 ());
	return 0;
}
