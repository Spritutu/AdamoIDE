/*
** SdoWriteGrid.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "SdoWriteGrid.h"
#include "GridCellCheck.h"
#include "GridCellCombo.h"

static CString* aszCanaleCan [NMAX_SCHEDE];
static CString* aszRicevitore [NMAX_CANNODES];
static CString* aszTipiDato [10];

CSdoWriteGrid::CSdoWriteGrid ()
{
}

/*
** InitControl :
*/
void CSdoWriteGrid::InitControl ()
{
    InitStrings ();
    InitColumnCaptions ();
    InitGrid ();
}

/*
** InitControl :
*/
void CSdoWriteGrid::CloseControl ()
{
    CloseStrings ();
}

/*
** InitControl :
*/
void CSdoWriteGrid::SetColumnSize ()
{
    RECT rc;
    int n;

    GetClientRect (&rc);
    SetColumnWidth (0, 90);
    SetColumnWidth (1, 90);
    SetColumnWidth (2, 90);
    SetColumnWidth (3, 90);
    SetColumnWidth (4, 100);
    SetColumnWidth (6, 30);
    n = rc.right>490 ? rc.right-490: 90;
    SetColumnWidth (5, n);
}

/*
** InitColumnCaptions :
*/
void CSdoWriteGrid::InitColumnCaptions ()
{
    GV_ITEM Item;
    int i;
    CString str;
    CBitmap bm;

    m_ImageList.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_SDOWRITESYMBOLS);
    m_ImageList.Add(&bm, RGB(0, 128, 128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<MAXSDOWRITECOLS; i++)   {
		Item.col=i;
        Item.iImage=i;
        str=LOADSTRING (IDS_ADAMODGSDOVIEW_21+i);
        Item.strText=str.GetBuffer (0);
        str.ReleaseBuffer ();
        SetItem(&Item);
    }
}

/*
** InitGrid :
*/
void CSdoWriteGrid::InitGrid ()
{
    for (int n=0; n<MAXSDOWRITESROWS-1; n++)
        InitGridLine (n+1);
}

/*
** InitGridLine :
*/
void CSdoWriteGrid::InitGridLine (int n)
{
    SetCellType(n, 0, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 1, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 2, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 3, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 4, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 5, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 6, RUNTIME_CLASS(CGridCell));
    ((CGridCellCombo*)GetCell(n, 0))->SetOptions(m_slCanaleCan);
    ((CGridCellCombo*)GetCell(n, 0))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 1))->SetOptions(m_slRicevitore);
    ((CGridCellCombo*)GetCell(n, 1))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 4))->SetOptions(m_slTipoDato);
    ((CGridCellCombo*)GetCell(n, 4))->SetStyle (CBS_DROPDOWNLIST);
}

/*
** InitStrings :
*/
void CSdoWriteGrid::InitStrings ()
{
    int n;

    m_slCanaleCan.SetSize(NMAX_SCHEDE);
    for (n=0; n<NMAX_SCHEDE; n++)   {
        aszCanaleCan[n]=new CString;
        aszCanaleCan[n]->Format ("%d", n+1);
        m_slCanaleCan[n]=*aszCanaleCan[n];
    }
    m_slRicevitore.SetSize(NMAX_CANNODES);
    for (n=0; n<NMAX_CANNODES; n++)   {
        aszRicevitore[n]=new CString;
        aszRicevitore[n]->Format ("%d", n+1);
        m_slRicevitore[n]=*aszRicevitore[n];
    }
    m_slTipoDato.SetSize (11);
    for (n=0; n<11; n++)   {
        aszTipiDato[n]=new CString;
        if (n < 9)
            *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_29+n);
            if (n == 9)
                *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_41);
        else
                if (n == 10)
            *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_42);
        m_slTipoDato[n]=*aszTipiDato[n];
    }
}

/*
** CloseStrings :
*/
void CSdoWriteGrid::CloseStrings ()
{
    int n;

    for (n=0; n<NMAX_SCHEDE; n++)
        _delete (aszCanaleCan[n]);
    for (n=0; n<NMAX_CANNODES; n++)
        _delete (aszRicevitore[n]);
    for (n=0; n<11; n++)
        _delete (aszTipiDato[n]);
}       

/*
** OnEditCell :
*/
void CSdoWriteGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nCol!=MAXSDOREADCOLS-1)
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}
