// AdamoSynLabel.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynLabel.h"

#pragma comment(lib, "UxTheme.lib")

IMPLEMENT_DYNAMIC(CAdamoSynLabel, CStatic)

CAdamoSynLabel::CAdamoSynLabel() : m_pFont (NULL), m_pParent (NULL)
{

}

CAdamoSynLabel::~CAdamoSynLabel()
{
}

BEGIN_MESSAGE_MAP(CAdamoSynLabel, CStatic)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/*
** Create :
*/
BOOL CAdamoSynLabel::Create ()
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_LEFT;
	if (m_objStyle.GetRSBorder ().HasABorder)
		dwStyle |= SS_SUNKEN;
	BOOL b = CStatic::Create (GetValue (), dwStyle, GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b)
		CreateFont ();
	return b;
}

/*
** Destroy :
*/
BOOL CAdamoSynLabel::Destroy ()
{
	return FALSE;
}

/*
** CreateFont :
*/
void CAdamoSynLabel::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** CtlColor :
*/
HBRUSH CAdamoSynLabel::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	COLORREF rgb;
	bool bIsForeAutomatic;

	m_objStyle.GetForeColor (&bIsForeAutomatic, &rgb);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor (rgb);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

/*
** OnEraseBkgnd :
*/
BOOL CAdamoSynLabel::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

/*
** PreSubclassWindow :
*/
void CAdamoSynLabel::PreSubclassWindow() 
{
	SetWindowTheme (*this, L"", L"");
}

/*
** OnSave :
*/
int CAdamoSynLabel::OnSave ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynLabel::OnLoad ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynLabel::OnDownload ()
{
	return 0;
}
