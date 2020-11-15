// CAdamoSynGroupBox.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynGroupBox.h"

#pragma comment(lib, "UxTheme.lib")

// CAdamoSynCheckBox

IMPLEMENT_DYNAMIC(CAdamoSynGroupBox, CWnd)

CAdamoSynGroupBox::CAdamoSynGroupBox() : m_pFont (NULL), m_pParent (NULL)
{
}

CAdamoSynGroupBox::~CAdamoSynGroupBox()
{
}

BEGIN_MESSAGE_MAP(CAdamoSynGroupBox, CWnd)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/*
** Create :
*/
BOOL CAdamoSynGroupBox::Create ()
{
	BOOL b = CButton::Create (GetValue (), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b)
		CreateFont ();
	return b;
}

/*
** Destroy :
*/
BOOL CAdamoSynGroupBox::Destroy ()
{
	return FALSE;
}

/*
** CreateFont :
*/
void CAdamoSynGroupBox::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** CtlColor :
*/
HBRUSH CAdamoSynGroupBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

/*
** OnEraseBkgnd :
*/
BOOL CAdamoSynGroupBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

/*
** PreSubclassWindow :
*/
void CAdamoSynGroupBox::PreSubclassWindow() 
{
	SetWindowTheme (*this, L"", L"");
}

/*
** OnSave :
*/
int CAdamoSynGroupBox::OnSave ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynGroupBox::OnLoad ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynGroupBox::OnDownload ()
{
	return 0;
}
