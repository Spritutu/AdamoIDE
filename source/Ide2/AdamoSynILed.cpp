#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynILed.h"

CAdamoSynILed::CAdamoSynILed(void)
{
}

CAdamoSynILed::~CAdamoSynILed(void)
{
}

/*
** Create :
*/
int CAdamoSynILed::Create ()
{
	CRect rc = GetObjStyle ().GetRect ();
	/* creiamo per prima cosa il componente */
	switch (GetLedType ())   {
		case eRectangular :
			((CiLedRectangleX *)this)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), m_pParent, GetID ());
			UpdateRectangularProperties  ();
			break;
		case eDiamond :
			((CiLedDiamondX *)this)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), m_pParent, GetID ());
			UpdateDiamondProperties  ();
			break;
		case eCircular :
			((CiLedRoundX *)this)->Create ("", WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), m_pParent, GetID ());
			UpdateCircularProperties  ();
			break;
	}
	MoveWindow (rc);
	return 0;
}

/*
** UpdateRectangularProperties :
*/
int CAdamoSynILed::UpdateRectangularProperties ()
{
	CiLedRectangleX *pLR = (CiLedRectangleX *) this;
	pLR->put_ShowReflection (GetShowReflection ());
	pLR->put_ActiveColor (GetActiveColor ());
	pLR->put_BevelStyle (GetBevelStyle ());
	pLR->put_BorderStyle (GetBorderStyle ());
	pLR->put_AutoInactiveColor (TRUE);
	return 0;
}

/*
** UpdateDiamondProperties :
*/
int CAdamoSynILed::UpdateDiamondProperties ()
{
	CiLedDiamondX *pLed = (CiLedDiamondX *) this;
	pLed->put_Transparent (GetTransparent ());
	pLed->put_ActiveColor (GetActiveColor ());
	pLed->put_BevelStyle (GetBevelStyle ());
	pLed->put_BorderStyle (GetBorderStyle ());
	pLed->put_BackGroundColor (GetBackgroundColor ());
	pLed->put_AutoInactiveColor (TRUE);
	return 0;
}

/*
** UpdateCircularProperties :
*/
int CAdamoSynILed::UpdateCircularProperties ()
{
	CiLedRoundX *pLed = (CiLedRoundX *) this;
	pLed->put_Transparent (GetTransparent ());
	pLed->put_ShowReflection (GetShowReflection ());
	pLed->put_ActiveColor (GetActiveColor ());
	pLed->put_BevelStyle (GetBevelStyle ());
	pLed->put_BorderStyle (GetBorderStyle ());
	pLed->put_BackGroundColor (GetBackgroundColor ());
	pLed->put_AutoInactiveColor (TRUE);
	return 0;
}

/*
** Destroy :
*/
int CAdamoSynILed::Destroy ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynILed::OnLoad ()
{
	return 0;
}

/*
** OnSave :
*/
int CAdamoSynILed::OnSave ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynILed::OnDownload ()
{
	return 0;
}

/*
** OnTimer :
*/
int CAdamoSynILed::OnTimer ()
{
	CAdamoData ad;
	if (GetDeviceType () != enBase && GetLogicAddress () != -1)   {
		GETSYNCONT ()->RequestRTData (GetDeviceType (), GetLogicAddress (), GetData (), LUA_TBOOLEAN, ad);
		if (ad.nType == LUA_TBOOLEAN)   {
			switch (m_eLedType)   {
				case eRectangular :
					((CiLedRectangleX *) this)->put_Active (ad.AdamoData.b);
					break;
				case eDiamond :
					((CiLedDiamondX *) this)->put_Active (ad.AdamoData.b);
					break;
				case eCircular :
					((CiLedRoundX *) this)->put_Active (ad.AdamoData.b);
					break;
			}
		}
	}
	else   {
		CAdamoRSVarControl &rsVar = GetObjVar ();
		if (rsVar.GetObjVar ().m_strName != "")   {
			GETSYNCONT ()->RequestVarData (&rsVar, LUA_TBOOLEAN, ad);
            if (ad.nType == LUA_TBOOLEAN)   {
			    switch (m_eLedType)   {
				    case eRectangular :
					    ((CiLedRectangleX *) this)->put_Active (ad.AdamoData.b);
					    break;
				    case eDiamond :
					    ((CiLedDiamondX *) this)->put_Active (ad.AdamoData.b);
					    break;
				    case eCircular :
					    ((CiLedRoundX *) this)->put_Active (ad.AdamoData.b);
					    break;
			    }
            }
		}
	}
	return 0;
}
