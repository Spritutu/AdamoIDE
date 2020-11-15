/*
** SSCNETParGrid.cpp :
*/

#include "stdafx.h"
#include "mainframe.h"
#include "SSCNETParGrid.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* constructors / destructors */
CSSCNETParGrid::CSSCNETParGrid () : m_nGD (0), m_pConfigDoc (NULL), m_pPrj (NULL)
{
}

/*
** SetGridData :
*/
void CSSCNETParGrid::SetGridData (int nDataNumber, CString strParNumber, CString strParSymbol, CString strParUnit, CString strParName, bool bIsEditable, bool bIsHex)
{
    m_SSCNETGridData[m_nGD].m_nNumber=nDataNumber;
    m_SSCNETGridData[m_nGD].m_strParNumber=strParNumber;
    m_SSCNETGridData[m_nGD].m_strParSymbol=strParSymbol;
    m_SSCNETGridData[m_nGD].m_strParUnit=strParUnit;
    m_SSCNETGridData[m_nGD].m_strParName=strParName;
    m_SSCNETGridData[m_nGD].m_bIsEditable=bIsEditable;
    m_SSCNETGridData[m_nGD].m_bIsHex=bIsHex;
    m_nGD++;
}

/*
** InitializeData :
*/
void CSSCNETParGrid::InitializeData ()
{
    CString str;

    for (int i=0; i<m_nGD; i++)   {
        SetItemText (i+1, 0, m_SSCNETGridData[i].m_strParNumber);
        SetItemText (i+1, 1, m_SSCNETGridData[i].m_strParSymbol);
        if (m_SSCNETGridData[i].m_bIsHex)
            str.Format ("0x%04x", 0);
        else
            str.Format ("%d", 0);
        SetItemText (i+1, 2, str);
        SetItemText (i+1, 3, m_SSCNETGridData[i].m_strParUnit);
        SetItemText (i+1, 4, m_SSCNETGridData[i].m_strParName);
        if (m_SSCNETGridData[i].m_bIsEditable)
            GetCell (i+1, 2)->SetBackClr (RGB (0xE0, 0xE0, 0xFF));
    }
}

/*
** InitParData :
*/
void CSSCNETParGrid::InitParData (int nPar)
{
    m_nPar=nPar;
}

/*
** OnEditCell :
*/
void CSSCNETParGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (m_SSCNETGridData[nRow-1].m_bIsEditable && nCol==2)   {
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
        m_pPrj->SetHWConfigModified ();
    }
}

/*
** GetParValue :
*/
void CSSCNETParGrid::GetParValue (int nPar, WORD *pw)
{
    CString str;
    char *p;

    str=GetItemText (nPar+1, 2);
    str.TrimLeft (); str.TrimRight ();
    long l=strtol (str, &p, 0);
    if (*p=='\0')
        *pw=(WORD) l;
}

/*
** SetParValue :
*/
void CSSCNETParGrid::SetParValue (int nPar, WORD w)
{
    CString str;

    if (m_SSCNETGridData[nPar].m_bIsHex)
        str.Format ("0x%04x", w);
    else
        str.Format ("%d", w);
    SetItemText (nPar+1, 2, str);
}

/*
** OnEndEditCell :
*/
void CSSCNETParGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
    WORD w=0;
    CString strFormatted;
    char *p;

    str.TrimLeft (); str.TrimRight ();
    long l=strtol (str, &p, 0);
    if (*p=='\0')
        w=(WORD) l;
    if (m_SSCNETGridData[nRow-1].m_bIsHex)
        strFormatted.Format ("0x%04x", w);
    else
        strFormatted.Format ("%d", w);
    CGridCtrl::OnEndEditCell (nRow, nCol, strFormatted);
}

/*
** FillGridCaption :
*/
void CSSCNETParGrid::FillGridCaption ()
{
    GV_ITEM Item;
    CString str;

    m_ImageList.Create(MAKEINTRESOURCE(IDB_CONFIG_HW_GRID), 16, 1, RGB(0,128,128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
    Item.iImage = 10;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_OPCODE);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 2;
    Item.iImage = 11;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_VALUE);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 3;
    Item.iImage = 12;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_UM);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 4;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
    SetItem(&Item);
}

/*
** FillGridCaption :
*/
void CSSCNETParGrid::GetSelectedParams (WORD *pW, int *nNumPar)
{
    int nState;

    *nNumPar=0;
    for (int i=0; i<m_nGD; i++)   {
        nState=GetItemState (i+1, 1);
        if (nState & GVNI_SELECTED)   {
            *pW=m_SSCNETGridData[i].m_nNumber+1;
            GetParValue (i, pW+1);
            pW+=2; (*nNumPar)++;
        }
    }
}
