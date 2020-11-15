// AdamoSynRTAxData.cpp : implementation file
//

#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynRTAx.h"
#include "AdamoSynRTAxData.h"


// CAdamoSynRTAxData

IMPLEMENT_DYNAMIC(CAdamoSynRTAxData, CStatic)

CAdamoSynRTAxData::CAdamoSynRTAxData() : m_pRTAx (NULL), m_pFontHeader (NULL), m_pFontNumbers (NULL)
{
	CreateFonts ();
}

CAdamoSynRTAxData::~CAdamoSynRTAxData()
{
	_delete (m_pFontHeader);
}

BEGIN_MESSAGE_MAP(CAdamoSynRTAxData, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CAdamoSynRTAxData message handlers
void CAdamoSynRTAxData::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CPaintDC dc (this);
	CRect rect, rcText;
	CString str;

	GetClientRect(&rect);
	rcText = rect;
	dc.SetTextColor (RGB (255, 0, 0));
	dc.FillSolidRect (rect, RGB (0, 0, 0));
	rcText.OffsetRect (CSize (4, 4));
	CFont *pFont = dc.SelectObject (m_pFontHeader);
	dc.DrawText (m_pRTAx->GetPath (), rcText, DT_SINGLELINE);
	rcText = rect; 
	rcText.OffsetRect (CSize (4, 20));
	str.Format ("%8.2f", m_pRTAx->m_fpQuotaTeorica);
	dc.SelectObject (m_pFontNumbers);
	dc.DrawText (str, rcText, DT_SINGLELINE);
	dc.SelectObject (pFont);
}

BOOL CAdamoSynRTAxData::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
}

int CAdamoSynRTAxData::CreateFonts()
{
	m_pFontHeader = GETSYNCONT ()->CreateFont ("Calibri", 16, FW_NORMAL);
	m_pFontNumbers = GETSYNCONT ()->CreateFont ("Calibri", 24, FW_NORMAL);
	return 0;
}
