// GridCellNumeric.cpp: implementation of the CGridCellNumeric class.
//
// Written by Andrew Truckle [ajtruckle@wsatkins.co.uk]
//
//////////////////////////////////////////////////////////////////////

/* include files */
#include "stdafx.h"
#include "GridCellToggleButton.h"
#include "GridCtrl.h"

/* constants definition */
#define MAXTOGGLEDIMENSION 50

IMPLEMENT_DYNCREATE(CGridCellToggleButton, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellToggleButton::CGridCellToggleButton () : m_bToggled (false)
{
}

BOOL CGridCellToggleButton::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
    m_Rect=rect;
    //rect.left=rect.right-MAXTOGGLEDIMENSION;
	rect.right=rect.left + MAXTOGGLEDIMENSION;
    pDC->DrawFrameControl(rect, DFC_BUTTON, 
	    (m_bToggled)? DFCS_BUTTONPUSH | DFCS_PUSHED  : DFCS_BUTTONPUSH);
    pDC->SetBkMode (TRANSPARENT);
    pDC->DrawText ("...", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    return bResult;
}

void CGridCellToggleButton::OnClick(CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();
    CRect rc (m_Rect);
    //rc.left=rc.right-MAXTOGGLEDIMENSION;
	rc.right=rc.left + MAXTOGGLEDIMENSION;
	// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
	// uncheck if the user clicked in the box
	if (rc.PtInRect(PointCellRelative))	{
        m_bToggled=true;
        CCellID id=GetGrid()->GetCellFromPt (PointCellRelative);
        GetGrid()->SetSelectedRange (id.row, id.col, id.row, id.col, TRUE, TRUE);
        NM_GRIDVIEW nmgv;
        nmgv.hdr.hwndFrom = GetGrid()->m_hWnd;
        nmgv.hdr.idFrom   = GetGrid()->GetDlgCtrlID();
        nmgv.hdr.code     = GVN_SELCHANGING;
        nmgv.iRow=id.row;
        nmgv.iColumn=id.col;
        GetGrid()->GetOwner()->SendMessage (WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
        GV_CHECKNOTIFY cngv;
        cngv.hdr.hwndFrom = GetGrid()->m_hWnd;
        cngv.hdr.idFrom   = GetGrid()->GetDlgCtrlID();
        cngv.hdr.code     = GVN_BUTTONPRESSED;
        cngv.iCheck       = true;
        GetGrid()->GetOwner()->SendMessage (WM_NOTIFY, cngv.hdr.idFrom, (LPARAM)&cngv);
        GetGrid()->InvalidateRect(m_Rect);
    }
}

void CGridCellToggleButton::ResetToggle ()
{
    m_bToggled=false;
    GetGrid()->InvalidateRect(m_Rect);
}

BOOL CGridCellToggleButton::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    return FALSE;
}