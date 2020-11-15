// PaneColorSet.cpp: implementation of the CPaneColorSet class.
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
#include "PaneColorSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaneColorSet::CPaneColorSet()
{
	RefreshMetrics();
}

CPaneColorSet::~CPaneColorSet()
{
}

void CPaneColorSet::RefreshMetrics()
{
	switch (((CIdeApp*)AfxGetApp ())->m_theme)
	{
		case themeVS2012Light:
		case themeVS2012Dark:
		{
			m_clrText		= XTPIniColor(_T("DockingPane.Inactive"), _T("TextColor"),
									  ::GetSysColor(COLOR_WINDOWTEXT));
			m_clrBack		= XTPIniColor(_T("DockingPane.Inactive"), _T("CaptionColor"),
									  ::GetSysColor(COLOR_WINDOW));
			m_clrBorderLite = XTPIniColor(_T("DockingPane.Tabs"), _T("ButtonSelected"),
										  ::GetSysColor(COLOR_WINDOW));
			m_clrBorderDark = XTPIniColor(_T("DockingPane.Tabs"), _T("ButtonSelected"),
										  ::GetSysColor(COLOR_3DSHADOW));
		}
		break;

		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue:
		{
			m_clrText		= XTPIniColor(_T("DockingPane.Inactive"), _T("TextColor"),
									  ::GetSysColor(COLOR_WINDOWTEXT));
			m_clrBack		= XTPIniColor(_T("DockingPane.Inactive"), _T("CaptionColor"),
									  ::GetSysColor(COLOR_WINDOW));
			m_clrBorderLite = XTPIniColor(_T("DockingPane.Tabs"), _T("ButtonSelected"),
										  ::GetSysColor(COLOR_WINDOW));
			m_clrBorderDark = XTPIniColor(_T("DockingPane.Tabs"), _T("ButtonBorder"),
										  ::GetSysColor(COLOR_3DSHADOW));
		}
		break;

		default:
		{
			m_clrText		= ::GetSysColor(COLOR_WINDOWTEXT);
			m_clrBack		= ::GetSysColor(COLOR_WINDOW);
			m_clrBorderLite = ::GetSysColor(COLOR_WINDOW);
			m_clrBorderDark = ::GetSysColor(COLOR_3DSHADOW);
		}
		break;
	}
}

void CPaneColorSet::DrawBorders(CDC* pDC, CRect rWindow, CXTPDockingPane* pPane /*=NULL*/)
{
	COLORREF pclrDark[4] = { m_clrBorderDark, m_clrBorderDark, m_clrBorderDark,
							 m_clrBorderDark }; // outside: left, top, right, bottom
	COLORREF pclrLite[4] = { m_clrBorderLite, m_clrBorderLite, m_clrBorderLite,
							 m_clrBorderLite }; // inside:  left, top, right, bottom

	if (pPane && (theApp.m_theme >= themeVS2015Light))
	{
		// set top border color to inside color.
		pclrDark[1] = pclrLite[1];

		// if floating or a side panel, set left, right and bottom border color to inside color.
		if (pPane->IsFloating() || pPane->IsSidePanel())
		{
			pclrDark[0] = pclrLite[0];
			pclrDark[2] = pclrLite[2];
			pclrDark[3] = pclrLite[3];
		}

		// if tabs are visible, set bottom border to inside color.
		else if (pPane->IsTabsVisible())
		{
			pclrDark[3] = pclrLite[3];
		}
	}

	CXTPDrawHelpers::DrawBorder(pDC, rWindow, pclrDark, pclrLite);
}
