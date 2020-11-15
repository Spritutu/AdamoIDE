// PaneWnd.cpp : implementation file
//
// (c)1998-2019 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "PaneView.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneWnd
/* external references */
extern UINT m_nRefreshMetrics;

CPaneView::CPaneView()
	: m_pPane(NULL)
	, m_rcMargin(0, 0, 0, 0)
{
}

CPaneView::~CPaneView()
{
}

BEGIN_MESSAGE_MAP(CPaneView, CView)
	//{{AFX_MSG_MAP(CPaneWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaneWnd message handlers

void CPaneView::OnPaint()
{
	CPaintDC dc(this);

	if (!m_rcMargin.IsRectNull())
	{
		CXTPClientRect rClient(this);
		theApp.m_csPane.DrawBorders(&dc, rClient, m_pPane);
	}
}

LRESULT CPaneView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	RefreshMetrics(AppTheme(wParam));
	return 0;
}

void CPaneView::RefreshMetrics(AppTheme nTheme)
{
	switch (nTheme)
	{
		case themeVS6:
		case themeVS2003:
		case themeVS2005:
		case themeVS2008:
		case themeVS2010:
		case themeVS2012Light:
		case themeVS2012Dark: m_rcMargin = CRect(0, 0, 0, 0); break;

		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue:
			m_rcMargin = CRect(XTP_DPI_X(1), 0, XTP_DPI_X(1), XTP_DPI_Y(1));
			break;
	}
}

void CPaneView::MoveChildren(CXTPToolBar* pToolBar, CWnd* pChildWnd, int cx, int cy)
{
	int x = m_rcMargin.left;
	int y = m_rcMargin.top;

	cx -= (x + m_rcMargin.right);
	cy -= (y + m_rcMargin.bottom);

	if (pToolBar->GetSafeHwnd())
	{
		CSize sz = pToolBar->CalcDockingLayout(cx, LM_HORZDOCK | LM_HORZ | LM_COMMIT);

		pToolBar->MoveWindow(x, y, cx, sz.cy);
		pToolBar->Invalidate(FALSE);

		y += sz.cy;
		cy -= sz.cy;
	}

	if (pChildWnd->GetSafeHwnd())
	{
		pChildWnd->MoveWindow(x, y, cx, cy);
		pChildWnd->Invalidate(FALSE);
	}
}