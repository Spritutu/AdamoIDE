// CGridCellDesc.cpp: implementation of the CGridCellDesc class.
//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellDesc.h"
#include "inplaceedit.h"
#include "GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridCellDesc, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Create a control to do the editing
BOOL CGridCellDesc::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    m_bEditing = TRUE;
    
    // CInPlaceEdit auto-deletes itself
    m_pEditWnd = new CInPlaceEdit(GetGrid(), rect,NULL, nID, nRow, nCol,GetText(), nChar);

//    ((CInPlaceEdit*)m_pEditWnd)->SetControlType(CInPlaceEdit::ED_DESC);

    return TRUE;
}

// Cancel the editing.
void CGridCellDesc::EndEdit()
{
    if (m_pEditWnd)
        ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
}
