// AdamoSynLabel.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynImage.h"

#pragma comment(lib, "UxTheme.lib")

IMPLEMENT_DYNAMIC(CAdamoSynImage, CStatic)

CAdamoSynImage::CAdamoSynImage() : m_pParent (NULL), m_pImage (NULL)
{

}

CAdamoSynImage::~CAdamoSynImage()
{
}

BEGIN_MESSAGE_MAP(CAdamoSynImage, CStatic)
END_MESSAGE_MAP()

/*
** Create :
*/
BOOL CAdamoSynImage::Create ()
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_LEFT | SS_OWNERDRAW;
	if (m_objStyle.GetRSBorder ().HasABorder)
		dwStyle |= SS_SUNKEN;
	BOOL b = CStatic::Create (GetValue (), dwStyle, GetObjStyle ().GetRect (), m_pParent, GetID ());
    m_pImage = new CxImage;
    GETSYNCONT ()->CaricaImmagine (m_strID, &m_pImage);
	return b;
}

/*
** Destroy :
*/
BOOL CAdamoSynImage::Destroy ()
{
    _delete (m_pImage);
	return FALSE;
}

/*
** OnEraseBkgnd :
*/
void CAdamoSynImage::OnEraseBackGround(CDC *pDC)
{
    //m_pImage->Draw(pDC->GetSafeHdc(), GetObjStyle().GetRect (), 0, false);
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

/*
** OnSave :
*/
int CAdamoSynImage::OnSave ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynImage::OnLoad ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynImage::OnDownload ()
{
	return 0;
}

/*
** OnDownload :
*/
void CAdamoSynImage::DrawItem(LPDRAWITEMSTRUCT)
{
}
