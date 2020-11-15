#include "stdafx.h"
#include "font.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynILabel.h"

CAdamoSynILabel::CAdamoSynILabel(void)
{
}

CAdamoSynILabel::~CAdamoSynILabel(void)
{
}

/*
** Create :
*/
int CAdamoSynILabel::Create ()
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
int CAdamoSynILabel::Destroy ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynILabel::OnLoad ()
{
	return 0;
}

/*
** OnSave :
*/
int CAdamoSynILabel::OnSave ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynILabel::OnDownload ()
{
	return 0;
}

/*
** OnTimer :
*/
int CAdamoSynILabel::OnTimer ()
{
	CAdamoData ad;
	if (GetDeviceType () != enBase && GetLogicAddress () != -1)   {
		GETSYNCONT ()->RequestRTData (GetDeviceType (), GetLogicAddress (), GetData (), LUA_TSTRING, ad);
		if (ad.nType == LUA_TSTRING)
			put_Caption (ad.AdamoData.str);
	}
	else   {
		CAdamoRSVarControl &rsVar = GetObjVar ();
		if (rsVar.GetObjVar ().m_strName != "")   {
			GETSYNCONT ()->RequestVarData (&rsVar, LUA_TSTRING, ad);
			if (ad.nType == LUA_TSTRING)
			    put_Caption (ad.AdamoData.str);
		}
	}
	return 0;
}

/*
** UpdateProperties :
*/
int CAdamoSynILabel::UpdateProperties ()
{
	COleFont fnt;
	CY cy;
	CiLabelX *pL = (CiLabelX *) this;
	/* general */
	pL->put_Caption (GetValue ());
	pL->put_Transparent  (IsTransparent ());
	pL->put_AutoSize (IsAutoSize ()); 
	pL->put_BorderStyle (GetStyle ());
	pL->put_Alignment (GetAlignment ());
	pL->put_ShadowShow (IsShadow ());
	pL->put_ShadowXOffset (GetOffsetX ());
	pL->put_ShadowYOffset (GetOffsetY ());
	pL->put_FontColor (GetColor ());
	pL->put_BackGroundColor (GetBackColor ());
	pL->put_ShadowColor (GetShadowColor ());
	CAdamoRSStyle style = GetObjStyle ();
	fnt = pL->get_Font ();
	fnt.SetName (style.GetRSFont ().strFaceName);
	cy.int64 = style.GetRSFont ().nSize * 10000;
	fnt.SetSize (cy);
	fnt.SetItalic (style.GetRSFont ().bItalic);
	fnt.SetBold (style.GetRSFont ().bBold),
	fnt.SetUnderline (style.GetRSFont ().bUnderLine);
	fnt.SetStrikethrough (style.GetRSFont ().bStrikeOut);
	pL->put_Font (fnt);
	return 0;
}
