/*
** AdamoHelpOptions.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "AdamoHelpOptions.h"
#include "adamoglobal.h"

BEGIN_MESSAGE_MAP(CAdamoHelpOptions, CListCtrl)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/* cobstruction / destruction */
CAdamoHelpOptions::CAdamoHelpOptions (int nMode) : m_nHighlight (HIGHLIGHT_ROW), m_nMode (nMode)
{
	m_nLastItems[0]=m_nLastItems[1]=m_nLastItems[2]=m_nLastItems[3]=0;
}

/*
** OnInitialUpdate :
*/
void CAdamoHelpOptions::SetCtrl ()
{
    CRect rect;
    GetWindowRect(&rect);
    m_il.Create(16, 16, ILC_MASK|ILC_COLOR8, 0, 16);
    CBitmap bm;
    bm.LoadBitmap(IDB_HELP_OPTIONS);
    m_il.Add(&bm, RGB(0, 128, 128));
    SetImageList(&m_il, LVSIL_SMALL);
	if (m_nMode == 0)   {
	InsertColumn(0, "", LVCFMT_LEFT,
		rect.Width() * 30/100, 0);
	InsertColumn(1, "", LVCFMT_LEFT,
		(rect.Width() * 70/100) - 20, 1);
	}
	else
		InsertColumn(0, "", LVCFMT_LEFT,
			rect.Width(), 0);
    DWORD dw=GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
	SetExtendedStyle (dw);
	SetFont (((CMainFrame *)AfxGetMainWnd ())->GetProgramFont ());
}

/*
** SetData :
*/
void CAdamoHelpOptions::SetData (CStringList* strOptions, CStringList* strDescOptions, int nType)
{
	m_strOptions = strOptions;
	m_strDescOptions = strDescOptions;
	m_nType = nType;
}

/*
** LoadData :
*/
void CAdamoHelpOptions::LoadData ()
{
    LVITEM lv;
	CString strOption, strDescription;
	POSITION pos = m_strOptions->GetHeadPosition ();
	POSITION pos1 = m_strDescOptions->GetHeadPosition ();
	int nItem;

	for (nItem = 0; pos; )   {
		strOption = m_strOptions->GetNext (pos);
		if (m_nMode == 0)
		strDescription = m_strDescOptions->GetNext (pos1);
		memset (&lv, 0, sizeof (LVITEM));
		lv.mask=LVIF_TEXT | LVIF_IMAGE;
		lv.iItem=nItem;
		lv.iSubItem=0;
		lv.iImage=m_nType;
		lv.pszText=strOption.GetBuffer ();
		lv.cchTextMax=256;
		InsertItem (&lv);
		if (m_nMode == 0)   {
		memset (&lv, 0, sizeof (LVITEM));
		lv.mask=LVIF_TEXT;
		lv.iItem=nItem;
		lv.iSubItem=1;
		lv.iImage=0;
		lv.pszText=strDescription.GetBuffer ();
		lv.cchTextMax=256;
		SetItem (&lv);
		}
		strOption.ReleaseBuffer ();
		strDescription.ReleaseBuffer ();
		nItem++;
	}
}

/*
** PreCreateWindow :
*/
BOOL CAdamoHelpOptions::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT;
    cs.style |= LVS_SINGLESEL;
    cs.style |= LVS_SHOWSELALWAYS;
	cs.style |= LVS_NOCOLUMNHEADER;
	cs.style |= LVS_OWNERDRAWFIXED;
	return CListCtrl::PreCreateWindow(cs);
}

/*
** PreTranslateMessage :
*/
BOOL CAdamoHelpOptions::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message==WM_KEYDOWN)   {
        switch (pMsg->wParam)   {
			case VK_ESCAPE :
				DestroyHelpOptionsWindow ();
				return TRUE;
				break;
		}
	}
	return CListCtrl::PreTranslateMessage (pMsg);
}

/*
** SelectLastItem :
*/
void CAdamoHelpOptions::SelectLastItem ()
{
	SetItemState (m_nLastItems[m_nType], LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

/*
** OnKillFocus :
*/
void CAdamoHelpOptions::OnKillFocus (CWnd* pNewWnd) 
{
}

/*
** DestroyHelpOptionsWindow :
*/
void CAdamoHelpOptions::DestroyHelpOptionsWindow ()
{
	CWnd* pWnd = GetParent ();
	DestroyWindow ();
	pWnd->SendMessage (WM_DESTROY_HELP_OPTIONS_WND, 0, 0L);
}

/*
** SelectWord :
*/
void CAdamoHelpOptions::SelectWord (CString strSelected)
{
	bool bFound = false;

	strSelected.MakeUpper ();
	/* scorriamo il list view alla ricerca della stringa */
	for (int i = 0; i < GetItemCount (); i++)   {
		CString str = GetItemText (i, 0);
		str.MakeUpper ();
		if (str == strSelected)
			break;
		else
		if (str.Left (strSelected.GetLength ()) == strSelected)  {
			/* selezioniamo l'item */
			SetItemState (i, LVIS_SELECTED, LVIS_SELECTED);
			EnsureVisible (i, FALSE);
			bFound = true; break;
		}
	}
	if (!bFound)
		DestroyHelpOptionsWindow ();
}

/*
** GetSelectedOption :
*/
CString CAdamoHelpOptions::GetSelectedOption ()
{
	CString str;
	int n;

	if ((n=GetNextItem (-1, LVIS_SELECTED))!=-1)   {
		str = GetItemText (n, 0);
		m_nLastItems[m_nType] = n;
	}
	return str;
}

/*
** MoveSelectedOptionUp :
*/
void CAdamoHelpOptions::MoveSelectedOptionUp()
{
	int n;

	if ((n=GetNextItem (-1, LVIS_SELECTED))!=-1)   {
		if (n > 0)   {
			n--;
			SetItemState (n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			EnsureVisible (n, FALSE);
		}
	}
}

/*
** MoveSelectedOptionDown :
*/
void CAdamoHelpOptions::MoveSelectedOptionDown()
{
	int n;

	if ((n=GetNextItem (-1, LVIS_SELECTED))!=-1)   {
		if (n < GetItemCount () - 1)   {
			n++;
			SetItemState (n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			EnsureVisible (n, FALSE);
		}
	}
}

void CAdamoHelpOptions::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
	CBrush *br;
	GetClientRect(&rcWnd);
	rcHighlight = rcBounds;
	rcHighlight.left = rcLabel.left;
	rcHighlight.right = rcWnd.right;
	if (lvi.state & LVIS_SELECTED )   {
		br = new CBrush (RGB (162, 212, 255));
		pDC->FillRect(rcHighlight, br);
	}
	else	 {
		br = new CBrush(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(rcHighlight, br);
	}
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