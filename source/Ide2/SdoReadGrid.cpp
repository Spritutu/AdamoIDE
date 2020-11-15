/*
** SdoReadGrid.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "SdoReadGrid.h"
#include "GridCellCheck.h"
#include "GridCellCombo.h"

static CString* aszCanaleCan [NMAX_SCHEDE];
static CString* aszRicevitore [NMAX_CANNODES];
static CString* aszTipiDato [10];
static CString* aszTipiDiVisualizzazione [3];

CSdoReadGrid::CSdoReadGrid ()
{
}

/*
** InitControl :
*/
void CSdoReadGrid::InitControl ()
{
    InitStrings ();
    InitColumnCaptions ();
    InitGrid ();
}

/*
** CloseControl :
*/
void CSdoReadGrid::CloseControl ()
{
    CloseStrings ();
}

/*
** InitControl :
*/
void CSdoReadGrid::SetColumnSize ()
{
    RECT rc;
    int n;

    GetClientRect (&rc);
    SetColumnWidth (0, 90);
    SetColumnWidth (1, 90);
    SetColumnWidth (2, 90);
    SetColumnWidth (3, 90);
    SetColumnWidth (4, 100);
    SetColumnWidth (5, 90);
    SetColumnWidth (7, 30);
    n = rc.right>580 ? rc.right-580 : 90;
    SetColumnWidth (6, n);
}

/*
** InitColumnCaptions :
*/
void CSdoReadGrid::InitColumnCaptions ()
{
    GV_ITEM Item;
    int i;
    CString str;
    CBitmap bm;

    m_ImageList.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_SDOREADSYMBOLS);
    m_ImageList.Add(&bm, RGB(0, 128, 128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<MAXSDOREADCOLS; i++)   {
		Item.col=i;
        Item.iImage=i;
        str=LOADSTRING (IDS_ADAMODGSDOVIEW_3+i);
        Item.strText=str.GetBuffer (0);
        str.ReleaseBuffer ();
        SetItem(&Item);
    }
}

/*
** InitGrid :
*/
void CSdoReadGrid::InitGrid ()
{
    for (int n=0; n<MAXSDOREADSROWS-1; n++)
        InitGridLine (n+1);
}

/*
** InitGridLine :
*/
void CSdoReadGrid::InitGridLine (int n)
{
    SetCellType(n, 0, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 1, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 2, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 3, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 4, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 5, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 6, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 7, RUNTIME_CLASS(CGridCell));
    ((CGridCellCombo*)GetCell(n, 0))->SetOptions(m_slCanaleCan);
    ((CGridCellCombo*)GetCell(n, 0))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 1))->SetOptions(m_slRicevitore);
    ((CGridCellCombo*)GetCell(n, 1))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 4))->SetOptions(m_slTipoDato);
    ((CGridCellCombo*)GetCell(n, 4))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 5))->SetOptions(m_slTipoDiVisualizzazione);
    ((CGridCellCombo*)GetCell(n, 5))->SetStyle (CBS_DROPDOWNLIST);
}

/*
** InitStrings :
*/
void CSdoReadGrid::InitStrings ()
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
            *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_12+n);
        else
            if (n == 9)
            *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_41);
            else
                if (n == 10)
                    *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_42);
        m_slTipoDato[n]=*aszTipiDato[n];
    }
    m_slTipoDiVisualizzazione.SetSize (3);
    for (n=0; n<3; n++)   {
        aszTipiDiVisualizzazione[n]=new CString;
        *aszTipiDiVisualizzazione[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_38+n);
        m_slTipoDiVisualizzazione[n]=*aszTipiDiVisualizzazione[n];
    }
}

/*
** CloseStrings :
*/
void CSdoReadGrid::CloseStrings ()
{
    int n;

    for (n=0; n<NMAX_SCHEDE; n++)
        _delete (aszCanaleCan[n]);
    for (n=0; n<NMAX_CANNODES; n++)
        _delete (aszRicevitore[n]);
    for (n=0; n<11; n++)
        _delete (aszTipiDato[n]);
    for (n=0; n<3; n++)
        _delete (aszTipiDiVisualizzazione [n]);
}       

/*
** OnEditCell :
*/
void CSdoReadGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nCol!=MAXSDOREADCOLS-1)
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}
