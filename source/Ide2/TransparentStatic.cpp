// TransparentStatic.cpp : implementation file
//

#include "stdafx.h"
#include "TransparentStatic.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CTransparentStatic

IMPLEMENT_DYNAMIC(CTransparentStatic, CStatic)
CTransparentStatic::CTransparentStatic() : m_nOffsetY (0), m_nOffsetX (0), 
    m_rgbForeColor (RGB(255, 255, 255)), 
    m_rgbBackColor (RGB(0, 0, 0))
{
}

CTransparentStatic::~CTransparentStatic()
{
}


BEGIN_MESSAGE_MAP(CTransparentStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTransparentStatic message handlers

void CTransparentStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Where to draw text
	CRect client_rect;
	GetClientRect(client_rect);

	// Get the caption
	CString szText;
	GetWindowText(szText);

	// Get the font
	CFont *pFont, *pOldFont;
	pFont = GetFont();
	pOldFont = dc.SelectObject(pFont);

	// Map "Static Styles" to "Text Styles"
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

	DWORD dwStyle = GetStyle(), dwText = 0;

	MAP_STYLE(	SS_RIGHT,			DT_RIGHT					);
	MAP_STYLE(	SS_CENTER,			DT_CENTER					);
	MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER | DT_SINGLELINE	);
	MAP_STYLE(	SS_NOPREFIX,		DT_NOPREFIX					);
	MAP_STYLE(	SS_WORDELLIPSIS,	DT_WORD_ELLIPSIS			);
	MAP_STYLE(	SS_ENDELLIPSIS,		DT_END_ELLIPSIS				);
	MAP_STYLE(	SS_PATHELLIPSIS,	DT_PATH_ELLIPSIS			);

	NMAP_STYLE(	SS_LEFTNOWORDWRAP |
				SS_CENTERIMAGE |
				SS_WORDELLIPSIS |
				SS_ENDELLIPSIS |
				SS_PATHELLIPSIS,	DT_WORDBREAK				);

	// Set transparent background
	dc.SetBkMode(TRANSPARENT);

    dc.SetTextColor(m_rgbBackColor);

	// Draw the text
	dc.DrawText(szText, client_rect, dwText);
    
    client_rect.left+=m_nOffsetX;
    client_rect.top+=m_nOffsetY;
    client_rect.right+=m_nOffsetX;
    client_rect.bottom+=m_nOffsetY;

    dc.SetTextColor(m_rgbForeColor);
	dc.DrawText(szText, client_rect, dwText);

	// Select old font
	dc.SelectObject(pOldFont);

}
