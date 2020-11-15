// AdamoSynLabel.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynIPicture.h"

#pragma comment(lib, "UxTheme.lib")

IMPLEMENT_DYNAMIC(CAdamoSynIPicture, CStatic)

CAdamoSynIPicture::CAdamoSynIPicture() : m_pParent (NULL), m_pImage (NULL)
{
    for (int i = 0; i<NMAX_IPICTURE_IMAGES; i++)
        m_paImage[i] = NULL;
}

CAdamoSynIPicture::~CAdamoSynIPicture()
{
}

BEGIN_MESSAGE_MAP(CAdamoSynIPicture, CStatic)
END_MESSAGE_MAP()

/*
** Create :
*/
BOOL CAdamoSynIPicture::Create ()
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_OWNERDRAW;
	if (m_objStyle.GetRSBorder ().HasABorder)
		dwStyle |= SS_SUNKEN;
	BOOL b = CStatic::Create (GetValue (), dwStyle, GetObjStyle ().GetRect (), m_pParent, GetID ());
    for (int i = 0; i<NMAX_IPICTURE_IMAGES; i++)
        if (m_strID[i] != "")   {
            m_paImage[i] = new CxImage;
            GETSYNCONT ()->CaricaImmagine (m_strID[i], &m_paImage[i]);
        }
	return b;
}

/*
** Destroy :
*/
BOOL CAdamoSynIPicture::Destroy ()
{
    for (int i = 0; i<NMAX_IPICTURE_IMAGES; i++)
        _delete (m_paImage[i]);
	return FALSE;
}

/*
** OnEraseBkgnd :
*/
void CAdamoSynIPicture::OnEraseBackGround(CDC* pDC)
{
    if (m_pImage)   {
        if (IsStretched ())
            m_pImage->Draw(pDC->GetSafeHdc(), GetObjStyle().GetRect (), 0, false);
        else   {
            if (IsTransparent ())   {
                CBitmap bmp;
                bmp.Attach (m_pImage->MakeBitmap (pDC->GetSafeHdc ()));
                GETSYNCONT ()->DrawTransparentBitmap (pDC->GetSafeHdc (), (HBITMAP)bmp.GetSafeHandle (), (short)GetObjStyle().GetRect ().left, (short)GetObjStyle().GetRect ().top, GetTransparentColor ());
            }
            else
                m_pImage->Draw(pDC->GetSafeHdc(), (short)GetObjStyle().GetRect ().left, (short)GetObjStyle().GetRect ().top, m_pImage->GetWidth(), m_pImage->GetHeight(), 0, false);
        }
    }
}

/*
** OnSave :
*/
int CAdamoSynIPicture::OnSave ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynIPicture::OnLoad ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynIPicture::OnDownload ()
{
	return 0;
}

/*
** OnDownload :
*/
void CAdamoSynIPicture::DrawItem(LPDRAWITEMSTRUCT)
{
}

/*
** OnTimer :
*/
int CAdamoSynIPicture::OnTimer ()
{
	CAdamoData ad;
	if (GetDeviceType () != enBase && GetLogicAddress () != -1)   {
		GETSYNCONT ()->RequestRTData (GetDeviceType (), GetLogicAddress (), GetData (), LUA_TNUMBER, ad);
        if (ad.nType == LUA_TNUMBER)  {
			if (SetCurrentImage ((int)ad.AdamoData.fp))
                m_pParent->InvalidateRect (GetObjStyle ().GetRect ());
        }
	}
	else   {
		CAdamoRSVarControl &rsVar = GetObjVar ();
		if (rsVar.GetObjVar ().m_strName != "")   {
			GETSYNCONT ()->RequestVarData (&rsVar, LUA_TNUMBER, ad);
            if (ad.nType == LUA_TNUMBER)   {
    			if (SetCurrentImage ((int)ad.AdamoData.fp))
                    m_pParent->InvalidateRect (GetObjStyle ().GetRect ());
            }
		}
	}
	return 0;
}

/*
** SetCurrentImage :
*/
int CAdamoSynIPicture::SetCurrentImage (int nIndex)
{
    int nB = 0;
    if (nIndex >=0 && nIndex < NMAX_IPICTURE_IMAGES)
        if (m_paImage[nIndex] != NULL)
            if (m_pImage != m_paImage[nIndex])   {
                m_pImage = m_paImage[nIndex];
                nB = 1;
            }
        else
            m_pImage = NULL;
    else
        m_pImage = NULL;
    return nB;
}
