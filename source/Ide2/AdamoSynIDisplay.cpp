#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynIDisplay.h"

CAdamoSynIDisplay::CAdamoSynIDisplay(void)
{
}

CAdamoSynIDisplay::~CAdamoSynIDisplay(void)
{
}

/*
** Create :
*/
int CAdamoSynIDisplay::Create ()
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
int CAdamoSynIDisplay::Destroy ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynIDisplay::OnLoad ()
{
	return 0;
}

/*
** OnSave :
*/
int CAdamoSynIDisplay::OnSave ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynIDisplay::OnDownload ()
{
	return 0;
}

/*
** OnTimer :
*/
int CAdamoSynIDisplay::OnTimer ()
{
	CAdamoData ad;
	if (GetDeviceType () != enBase && GetLogicAddress () != -1)   {
		GETSYNCONT ()->RequestRTData (GetDeviceType (), GetLogicAddress (), GetData (), LUA_TNUMBER, ad);
		if (ad.nType == LUA_TNUMBER)
			put_Value (ad.AdamoData.fp);
	}
	else   {
		CAdamoRSVarControl &rsVar = GetObjVar ();
		if (rsVar.GetObjVar ().m_strName != "")   {
			GETSYNCONT ()->RequestVarData (&rsVar, LUA_TNUMBER, ad);
			if (ad.nType == LUA_TNUMBER)
				put_Value (ad.AdamoData.fp);
		}
	}
	return 0;
}

/*
** UpdateProperties :
*/
int CAdamoSynIDisplay::UpdateProperties ()
{
	CiSevenSegmentAnalogX *pDD = (CiSevenSegmentAnalogX *) this;
	/* general */
	pDD->put_ShowOffSegments (IsOffSegment ());
	pDD->put_Transparent (IsTransparent());
	pDD->put_ShowSign (IsSign());
	pDD->put_AutoSegmentOffColor (IsAutoSegmentOffColor());
	pDD->put_Precision (GetPrecision ());
	pDD->put_LeadingStyle (GetLeadingStyle ());
	pDD->put_BorderStyle (GetDisplayStyle ());
	pDD->put_BackGroundColor (GetBackGroundColor ());
	/* display */
	pDD->put_DigitCount (GetDisplayCount ());
	pDD->put_DigitSpacing (GetDisplaySpacing ());
	pDD->put_SegmentSize (GetDisplaySize ());
	pDD->put_SegmentSeperation (GetDisplaySeparation ());
	pDD->put_SegmentMargin (GetDisplayMargin ());
	pDD->put_SegmentColor (GetDisplayColor ());
	pDD->put_SegmentOffColor (GetDisplayOffColor ());
	return 0;
}
