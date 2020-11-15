#include "stdafx.h"
#include "MainFrame.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynButton.h"

IMPLEMENT_DYNAMIC(CAdamoSynButton, CWnd)

BEGIN_MESSAGE_MAP(CAdamoSynButton, CWnd)
END_MESSAGE_MAP()

CAdamoSynButton::CAdamoSynButton(void) : m_eType (eStandard), m_pFont (NULL), m_pParent (NULL), m_dwAction (0), m_bTransparency (false)
{
}

CAdamoSynButton::~CAdamoSynButton(void)
{
	_delete (m_pFont);
}

/*
** Create :
*/
int CAdamoSynButton::Create ()
{
	BOOL b = CButton::Create (GetValue (), WS_CHILD | WS_VISIBLE, GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b && m_eType == eBitmap)   {
		GETSYNCONT ()->CaricaImmagine (m_strImage, &m_bmp);
		COLORREF rgb = GetTransparencyRGB ();
		SetBitmaps (m_bmp, rgb, (HBITMAP)BTNST_AUTO_DARKER);
		if (IsTransparent ())
			DrawTransparent ();
	}
	if (b)
		CreateFont ();
	return b;
}

/*
** Destroy :
*/
int CAdamoSynButton::Destroy ()
{
	return 0;
}

/*
** GetTransparencyRGB :
*/
COLORREF CAdamoSynButton::GetTransparencyRGB ()
{
	CDC *pWndDC = GetDC (), dc;
	dc.CreateCompatibleDC (pWndDC);
	dc.SelectObject (&m_bmp);
	COLORREF rgb = dc.GetPixel (0, 0);
	dc.DeleteDC ();
	ReleaseDC (pWndDC);
	return rgb;
}

/*
** CreateFont :
*/
void CAdamoSynButton::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** OnSave :
*/
int CAdamoSynButton::OnSave ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynButton::OnLoad ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynButton::OnDownload ()
{
	return 0;
}

/*
** OnEraseBackGround :
*/
void CAdamoSynButton::OnEraseBackGround (CDC* pDC)
{
	SetBk (pDC);
}
