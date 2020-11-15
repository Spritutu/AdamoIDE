#include "StdAfx.h"
#include "ListViewErrorsCustomColor.h"

extern UINT m_nRefreshMetrics;

BEGIN_MESSAGE_MAP(CListViewCustomHeader, CHeaderCtrl)
    //{{AFX_MSG_MAP(MyCustomCListCtrl)
    //}}AFX_MSG_MAP
    // ON_WM_DRAWITEM()                             /* WM_DRAWITEM NON-AVAILABLE */
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
END_MESSAGE_MAP()

CListViewCustomHeader::CListViewCustomHeader ()
{
}

/*
** OnRefreshMetrics :
*/
LRESULT CListViewCustomHeader::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	SetSkin ((AppTheme) wParam);
	Invalidate ();
	return 0;
}

/*
** OnRefreshMetrics :
*/
LRESULT CListViewCustomHeader::SetSkin (AppTheme nSkin)
{
	switch (nSkin)   {
		case themeVS2010 :
			m_bckColor = RGB(0x4a,0x61,0x84);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2012Light :
			m_bckColor = RGB(0xf5,0xf5,0xf5);
			m_foreColor = RGB(0x00,0x00,0x00);
			break;
		case themeVS2012Dark :
			m_bckColor = RGB(0x00,0x00,0x00);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2015Light :
			m_bckColor = RGB(0xf5,0xf5,0xf5);
			m_foreColor = RGB(0x00,0x00,0x00);
			break;
		case themeVS2015Dark :
			m_bckColor = RGB(0x00,0x00,0x00);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2015Blue :
			m_bckColor = RGB(0x4a,0x61,0x84);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
	}
	return 0;
}

void CListViewCustomHeader::OnNMCustomdraw (NMHDR *pNMHDR, LRESULT *pResult)
{
	 LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = CDRF_DODEFAULT;

    if (pNMCD->dwDrawStage == CDDS_PREPAINT)        {
        CDC* pDC = CDC::FromHandle(pNMCD->hdc);
        CRect rect(0, 0, 0, 0);
        GetClientRect(&rect);
        pDC->FillSolidRect(&rect, m_bckColor);
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else 
		if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)        {
			CImageList* pimgList = GetImageList ();
			CRect rc (pNMCD->rc);
			int nCount = pimgList->GetImageCount ();
			HDITEM hditem;
			TCHAR buffer[MAX_PATH] = { 0 };
			SecureZeroMemory(&hditem, sizeof(HDITEM));
			hditem.mask = HDI_TEXT | HDI_IMAGE;
			hditem.pszText = buffer;
			hditem.cchTextMax = MAX_PATH;
			GetItem(pNMCD->dwItemSpec, &hditem);
			CDC* pDC = CDC::FromHandle(pNMCD->hdc);
			pDC->SetTextColor(m_foreColor);
			pDC->SetBkColor(m_bckColor);
			CString str(buffer);
			pimgList->DrawEx (pDC, hditem.iImage, CPoint (rc.left, rc.top + 4), CSize (16, 16), CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
			rc.left += 20;
			rc.top += 4;
			pDC->DrawText(str, rc, DT_VCENTER | DT_TOP);
			*pResult = CDRF_SKIPDEFAULT;
        }
}

BEGIN_MESSAGE_MAP(CListViewErrorsCustomColor, CListCtrl)
    ON_WM_ERASEBKGND()
	ON_WM_CREATE ()
END_MESSAGE_MAP()

CListViewErrorsCustomColor::CListViewErrorsCustomColor(void) : m_nHighlight (HIGHLIGHT_ROW)
{
}

CListViewErrorsCustomColor::~CListViewErrorsCustomColor(void)
{
}

void CListViewErrorsCustomColor::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();
    CHeaderCtrl* pHeader = NULL;
    pHeader = GetHeaderCtrl();
    if (pHeader != NULL)   {
        VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd)); // m_HeaderCtrl is the new wrapper object
    }
}

int CListViewErrorsCustomColor::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
    CHeaderCtrl* pHeader = NULL;
    pHeader = GetHeaderCtrl();
    if (pHeader != NULL)   {
        VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd)); // m_HeaderCtrl is the new wrapper object
    }
	return 0;
}

void CListViewErrorsCustomColor::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);
    int nItem = lpDrawItemStruct->itemID;
    CImageList* pImageList;
    
    // Save dc state
    int nSavedDC = pDC->SaveDC();
    
    // Get item image and state info
    LV_ITEM lvi;
    lvi.mask = LVIF_IMAGE | LVIF_STATE;
    lvi.iItem = nItem;
    lvi.iSubItem = 0;
    lvi.stateMask = 0xFFFF;     // get all state flags
    GetItem(&lvi);
    
    // Should the item be highlighted
    BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)
        || ( (lvi.state & LVIS_SELECTED)
        && ((GetFocus() == this)
        || (GetStyle() & LVS_SHOWSELALWAYS)
        )
        )
        );
    
    // Get rectangles for drawing
    CRect rcBounds, rcLabel, rcIcon;
    GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
    GetItemRect(nItem, rcLabel, LVIR_LABEL);
    GetItemRect(nItem, rcIcon, LVIR_ICON);
    CRect rcCol( rcBounds ); 
    
    CString sLabel = GetItemText( nItem, 0 );
    
    // Labels are offset by a certain amount  
    // This offset is related to the width of a space character
    int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;
    
    CRect rcHighlight;
    CRect rcWnd;
    int nExt;
    switch( m_nHighlight )    {
		case 0: 
			nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
			rcHighlight = rcLabel;
			if( rcLabel.left + nExt < rcLabel.right )
				rcHighlight.right = rcLabel.left + nExt;
			break;
		case 1:
			rcHighlight = rcBounds;
			rcHighlight.left = rcLabel.left;
			break;
		case 2:
			GetClientRect(&rcWnd);
			rcHighlight = rcBounds;
			rcHighlight.left = rcLabel.left;
			rcHighlight.right = rcWnd.right;
			break;
		default:
			rcHighlight = rcLabel;
    }
    
    // Draw the background color
	bHighlight = false;
	CBrush *br;
	switch (lvi.iImage)   {
        case TE_HARDWARE :
			pDC->SetTextColor (RGB (255, 255, 255));
            br = new CBrush (RGB (128, 0, 0));
            break;
        case TE_INTERPRETER :
			pDC->SetTextColor (RGB (255, 255, 255));
			br = new CBrush (RGB (255, 128, 0));
            break;
		default :
			//br = new CBrush(::GetSysColor(COLOR_WINDOW));
			br = new CBrush(RGB (192, 192, 192));
	}
    pDC->FillRect(rcHighlight, br);
    
    // Set clip region
    rcCol.right = rcCol.left + GetColumnWidth(0);
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rcCol);
    pDC->SelectClipRgn(&rgn);
    rgn.DeleteObject();
    
    // Draw state icon
    if (lvi.state & LVIS_STATEIMAGEMASK)
    {
        int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
        pImageList = GetImageList(LVSIL_STATE);
        if (pImageList)
        {
            pImageList->Draw(pDC, nImage,
                CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
        }
    }
    
    // Draw normal and overlay icon
    pImageList = GetImageList(LVSIL_SMALL);
    if (pImageList)
    {
        UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
        pImageList->Draw(pDC, lvi.iImage, 
            CPoint(rcIcon.left, rcIcon.top),
            (bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask );
    }
    
    // Draw item label - Column 0
    rcLabel.left += offset/2;
    rcLabel.right -= offset;
    
    pDC->DrawText(sLabel,-1,rcLabel,
        DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP |
        DT_VCENTER | DT_END_ELLIPSIS);
    
    // Draw labels for remaining columns
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH;
    
    if( m_nHighlight == 0 )     // Highlight only first column
    {
        pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
        pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
    }
    
    rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right :
    rcBounds.right;
    rgn.CreateRectRgnIndirect(&rcBounds);
    pDC->SelectClipRgn(&rgn);
    
    for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
    {
        rcCol.left = rcCol.right;
        rcCol.right += lvc.cx;
        
        // Draw the background if needed
        if( m_nHighlight == HIGHLIGHT_NORMAL )
            pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));
        
        sLabel = GetItemText(nItem, nColumn);
        if (sLabel.GetLength() == 0)
            continue;
        
        // Get the text justification
        UINT nJustify = DT_LEFT;
        switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
        {
        case LVCFMT_RIGHT:
            nJustify = DT_RIGHT;
            break;
        case LVCFMT_CENTER:
            nJustify = DT_CENTER;
            break;
        default:
            break;
        }
        
        rcLabel = rcCol;
        rcLabel.left += offset;
        rcLabel.right -= offset;
        
        pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
            DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
    }
    
    // Draw focus rectangle if item has focus
    if (lvi.state & LVIS_FOCUSED && (GetFocus() == this))
        pDC->DrawFocusRect(rcHighlight);
    
    // Restore dc
    pDC->RestoreDC( nSavedDC );
	delete br;
}

BOOL CListViewErrorsCustomColor::OnEraseBkgnd(CDC* pDC) 
{
	CBrush *br;
	CRect rect;
	CListViewErrorsCustomColor::GetClientRect(rect);
	br = new CBrush (RGB (245, 245, 245));
	pDC->FillRect(&rect,br);
	br->DeleteObject();
	delete br;
	return FALSE;
}
