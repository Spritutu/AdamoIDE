/*********************************************************
* Multi-Column Tree View
* Version: 1.1
* Date: October 22, 2003
* Author: Michal Mecinski
* E-mail: mimec@mimec.w.pl
* WWW: http://www.mimec.w.pl
*
* Copyright (C) 2003 by Michal Mecinski
*********************************************************/

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "ColumnTreeView.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef TVS_NOHSCROLL
#define TVS_NOHSCROLL 0x8000	// IE 5.0 or higher required
#endif

IMPLEMENT_DYNCREATE(CColumnTreeView, CPaneView)

BEGIN_MESSAGE_MAP(CColumnTreeView, CPaneView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
    ON_WM_DESTROY ()
	ON_NOTIFY(HDN_ITEMCHANGED, IDC_HEADER_CTRL, OnHeaderItemChanged)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE_CTRL, OnTreeCustomDraw)
END_MESSAGE_MAP()


CColumnTreeView::CColumnTreeView() : m_bNoHeader(false), m_pBoldFont (NULL)
{
}

CColumnTreeView::~CColumnTreeView()
{
}


BOOL CColumnTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CColumnTreeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (m_Tree.m_hWnd)
		return;

	// create tree and header controls as children
	m_Tree.Create(WS_CHILD | WS_VISIBLE | TVS_NOHSCROLL | TVS_NOTOOLTIPS, CRect(), this, IDC_TREE_CTRL);
	m_Header.Create(WS_CHILD | WS_VISIBLE | HDS_FULLDRAG | HDS_BUTTONS, CRect(), this, IDC_HEADER_CTRL);

	// set correct font for the header
	CFont* pFont = m_Tree.GetFont();
	m_Header.SetFont(pFont);

	// check if the common controls library version 6.0 is available
	BOOL bIsComCtl6 = FALSE;

	HMODULE hComCtlDll = LoadLibrary("comctl32.dll");

	if (hComCtlDll)
	{
		typedef HRESULT (CALLBACK *PFNDLLGETVERSION)(DLLVERSIONINFO*);

		PFNDLLGETVERSION pfnDllGetVersion = (PFNDLLGETVERSION)GetProcAddress(hComCtlDll, "DllGetVersion");

		if (pfnDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

			HRESULT hRes = (*pfnDllGetVersion)(&dvi);

			if (SUCCEEDED(hRes) && dvi.dwMajorVersion >= 6)
				bIsComCtl6 = TRUE;
		}

		FreeLibrary(hComCtlDll);
	}

	// calculate correct header's height
	CDC* pDC = GetDC();
	pDC->SelectObject(pFont);
	CSize szExt = pDC->GetTextExtent("A");
	m_cyHeader = szExt.cy + (bIsComCtl6 ? 7 : 4) + 2;
	ReleaseDC(pDC);

	// offset from column start to text start
	m_xOffset = bIsComCtl6 ? 9 : 6;
    m_pBoldFont=((CMainFrame*)AfxGetMainWnd ())->GetProgramFontBold();
	m_xPos = 0;
	UpdateColumns();
}


void CColumnTreeView::OnPaint()
{
	// do nothing
	CPaintDC dc(this);
}

BOOL CColumnTreeView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CColumnTreeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
    UpdateLastColumn (cx);
	UpdateScroller();
	RepositionControls();
}

void CColumnTreeView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int cx = rcClient.Width();

	int xLast = m_xPos;

	switch (nSBCode)
	{
	case SB_LINELEFT:
		m_xPos -= 15;
		break;
	case SB_LINERIGHT:
		m_xPos += 15;
		break;
	case SB_PAGELEFT:
		m_xPos -= cx;
		break;
	case SB_PAGERIGHT:
		m_xPos += cx;
		break;
	case SB_LEFT:
		m_xPos = 0;
		break;
	case SB_RIGHT:
		m_xPos = m_cxTotal - cx;
		break;
	case SB_THUMBTRACK:
		m_xPos = nPos;
		break;
	}

	if (m_xPos < 0)
		m_xPos = 0;
	else if (m_xPos > m_cxTotal - cx)
		m_xPos = m_cxTotal - cx;

	if (xLast == m_xPos)
		return;

	SetScrollPos(SB_HORZ, m_xPos);
	RepositionControls();
}


void CColumnTreeView::OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateColumns();

	m_Tree.Invalidate();
}

void CColumnTreeView::OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMCUSTOMDRAW* pNMCustomDraw = (NMCUSTOMDRAW*)pNMHDR;
	NMTVCUSTOMDRAW* pNMTVCustomDraw = (NMTVCUSTOMDRAW*)pNMHDR;
	Color_Font cf;

	switch (pNMCustomDraw->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		break;

	case CDDS_ITEMPOSTPAINT:
		{
            CPen pFore;
			HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
			CRect rcItem = pNMCustomDraw->rc;
            CFont* pF;
            
            UINT uMask=GetTreeCtrl().GetItemState (hItem, TVIS_BOLD);

			if (rcItem.IsRectEmpty())
			{
				// nothing to paint
				*pResult = CDRF_DODEFAULT;
				break;
			}

			CDC dc;
			dc.Attach(pNMCustomDraw->hdc);

            if (uMask&TVIS_BOLD)
                pF=(CFont*)dc.SelectObject (m_pBoldFont);

			CRect rcLabel;
			m_Tree.GetItemRect(hItem, &rcLabel, TRUE);

			COLORREF crTextBk = pNMTVCustomDraw->clrTextBk;
			COLORREF crText = pNMTVCustomDraw->clrText;
			
			int n = m_mapColorFont.GetCount();  
			if (m_mapColorFont.Lookup(hItem, cf)){
				if(cf.color != (COLORREF) - 1) {
					crText = cf.color;
					//if(crText == RGB(255,0,0))
					//	AfxMessageBox(m_Tree.GetItemText(hItem));
					
				}
			}
			
			COLORREF crWnd = GetSysColor(COLOR_WINDOW);

			// clear the original label rectangle
			dc.FillSolidRect(&rcLabel, crWnd);

			int nColsCnt = m_Header.GetItemCount();

			// draw horizontal lines...
			int xOffset = 0, i;
			//for (int i=0; i<nColsCnt; i++)
			//{
			//	xOffset += m_arrColWidths[i];
			//	rcItem.right = xOffset-1;
			//	dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_RIGHT);
			//}
			// ...and the vertical ones
			//dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_BOTTOM);

			CString strText = m_Tree.GetItemText(hItem);
			CString strSub;
			AfxExtractSubString(strSub, strText, 0, '\t');

			// calculate main label's size
			CRect rcText(0,0,0,0);
			dc.DrawText(strSub, &rcText, DT_NOPREFIX | DT_CALCRECT);
			rcLabel.right = min(rcLabel.left + rcText.right + 4, m_arrColWidths[0] - 4);

			if (rcLabel.Width() < 0)
				crTextBk = crWnd;
			if (crTextBk != crWnd)	  // draw label's background
            {
                CRect rcSolidLine (rcLabel);
                rcSolidLine.right=m_arrColWidths[0];
			    for (i=1; i<nColsCnt; i++)
			    {
                    rcSolidLine.right+=m_arrColWidths[i];   
                }
				dc.FillSolidRect(&rcSolidLine, crTextBk);
            }
            dc.SetTextColor (crText);
			// draw focus rectangle if necessary
			//if (pNMCustomDraw->uItemState & CDIS_FOCUS)
			//	dc.DrawFocusRect(&rcLabel);

			// draw main label
			rcText = rcLabel;
			rcText.DeflateRect(2, 1);
			dc.DrawText(strSub, &rcText, DT_NOPREFIX | DT_END_ELLIPSIS);

			xOffset = m_arrColWidths[0];
			dc.SetBkMode(TRANSPARENT);

			// draw other columns text
			for (i=1; i<nColsCnt; i++)
			{
				if (AfxExtractSubString(strSub, strText, i, '\t'))
				{
					rcText = rcLabel;
					rcText.left = xOffset;
					rcText.right = xOffset + m_arrColWidths[i];
					rcText.DeflateRect(m_xOffset, 1, 2, 1);
					dc.DrawText(strSub, &rcText, DT_NOPREFIX | DT_END_ELLIPSIS);
				}
				xOffset += m_arrColWidths[i];
			}

            if (uMask&TVIS_BOLD)
                dc.SelectObject (pF);

			dc.Detach();
		}
		*pResult = CDRF_DODEFAULT;
		break;

	default:
		*pResult = CDRF_DODEFAULT;
	}
}


void CColumnTreeView::UpdateColumns()
{
	m_cxTotal = 0;

	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	int nCnt = m_Header.GetItemCount();
	if (nCnt > 16)
		nCnt = 16;

	// get column widths from the header control
	for (int i=0; i<nCnt; i++)
	{
		if (m_Header.GetItem(i, &hditem))
		{
			m_cxTotal += m_arrColWidths[i] = hditem.cxy;
			if (i==0)
				m_Tree.m_cxFirstCol = hditem.cxy;
		}
	}

	UpdateScroller();
	RepositionControls();
}

void CColumnTreeView::UpdateScroller()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int cx = rcClient.Width();

	int lx = m_xPos;

	if (m_xPos > m_cxTotal - cx)
		m_xPos = m_cxTotal - cx;
	if (m_xPos < 0)
		m_xPos = 0;

	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	scrinfo.nPage = cx;
	scrinfo.nMin = 0;
	scrinfo.nMax = m_cxTotal;
	scrinfo.nPos = m_xPos;
	SetScrollInfo(SB_HORZ, &scrinfo);
}

void CColumnTreeView::RepositionControls()
{
	// reposition child controls
	if (m_Tree.m_hWnd)
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		int cx = rcClient.Width();
		int cy = rcClient.Height();

		// move to a negative offset if scrolled horizontally
		int x = 0;
		if (cx < m_cxTotal)
		{
			x = GetScrollPos(SB_HORZ);
			cx += x;
		}

        if(m_bNoHeader)
            m_Tree.MoveWindow(-x, 0, cx, cy);
        else   {
		    m_Header.MoveWindow(-x, 0, cx, m_cyHeader);
		    m_Tree.MoveWindow(-x, m_cyHeader, cx, cy-m_cyHeader);
        }
	}
}

void CColumnTreeView::SetNoHeader(BOOL bNoHeader)
{
    m_bNoHeader = bNoHeader;    
    if(bNoHeader)
        m_Header.ShowWindow(SW_HIDE);
    else
        m_Header.ShowWindow(SW_SHOW);
	Invalidate();
}

void CColumnTreeView::SetColumnImages ()
{
    m_hIL=new CImageList;
    m_bmpDeviceTree=new CBitmap;
    m_bmpDeviceDesc=new CBitmap;
    m_bmpDeviceTree->LoadBitmap(IDB_DEVICETREE);
    m_bmpDeviceDesc->LoadBitmap (IDB_CAPCOMMENT);
    m_hIL->Create (16, 16, ILC_MASK, 0, 1);
    m_hIL->Add(m_bmpDeviceTree, RGB(0, 128,128));
    m_hIL->Add(m_bmpDeviceDesc, RGB(0, 128,128));
    m_Header.SetImageList(m_hIL);
}

void CColumnTreeView::OnDestroy()
{
    _delete (m_hIL);
    _delete (m_bmpDeviceTree);
    _delete (m_bmpDeviceDesc);
}

void CColumnTreeView::UpdateLastColumn (int cx)
{
    if (m_Header.m_hWnd)   {
        HDITEM item;
        item.mask = HDI_WIDTH;
        m_Header.GetItem (1, &item);
        item.cxy=cx-400-1;
        if (item.cxy>100)   {
            m_Header.SetItem (1, &item);
            m_Header.Invalidate ();
        }
    }
}

/*
** SetItemColor: set il colre all'item
*/
void CColumnTreeView::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if(!m_mapColorFont.Lookup(hItem, cf))
		cf.logfont.lfFaceName[0] = '\0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
	Invalidate();
}

/*
** GetItemColor: ritorna il colore dell'item 
*/
COLORREF CColumnTreeView::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if(!m_mapColorFont.Lookup(hItem, cf))
		return (COLORREF) - 1;
	return cf.color;

}
