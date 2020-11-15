// AdamoDGCards.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoDGCards.h"
#include "workspace.h"
#include "mainframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGCards

CAdamoDGCards::CAdamoDGCards() : m_nCount (0)
{
}

BEGIN_MESSAGE_MAP(CAdamoDGCards, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CAdamoDGCards)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** FillColumns : configura le colonne del supergridctrl
*/
void CAdamoDGCards::FillColumns ()
{
    CString str;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
    str=LOADSTRING (IDS_ADAMODGCARDS_1);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn(0,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_2);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=1;
    InsertColumn(1,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_3);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=2;
    InsertColumn(2,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_4);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=3;
    InsertColumn(3,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_5);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=4;
    InsertColumn(4,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_6);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=5;
    InsertColumn(5,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_7);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=6;
    InsertColumn(6,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGCARDS_8);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=7;
    InsertColumn(7,&lvColumn);
    SetGraphicalColumn (5, TRUE);
    SetGraphicalColumn (6, TRUE);
    SetGraphicalColumn (7, TRUE);
}

/*
** OnCreate :
*/
int CAdamoDGCards::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
        return -1;
    CBitmap* pBmp=new CBitmap;
    pBmp->LoadBitmap (IDB_DG_CARDS);
    m_image.Add (pBmp, RGB(0, 128, 128));
    SetMachine ();
	SetImageList(&m_image, LVSIL_SMALL);
    m_cxImage=m_cyImage=16;
    m_bmp24VoltOn.LoadBitmap (IDB_CAPEVENTS);
    m_bmp24VoltOff.LoadBitmap (IDB_BMP_EMPTY);
    delete pBmp;
	return 0;
}

/*
** RinfrescaInformazioni :
*/
void CAdamoDGCards::RinfrescaInformazioni () 
{
    CCmdGetCardInfo CI;
    int i;
    /* completiamo il refresh dei dispositivi andando a interrogare quelli che sono nel frattempo cambiati */
    if (m_pMachine)   {
        if (m_nCount==0)   {
            if (m_pMachine->GetNumCards (&m_nCount) == S_FALSE)
                m_nCount = 0;
			bitres (m_nCount, 31);
            if (m_nCount>0)   {
                for (i=0; i<m_nCount; i++)   {
                    if (m_pMachine->GetCardInfo (i, &CI) == S_OK)
                        AddCardLine (&CI);
                }
            }
        }
        else
            for (i=0; i<m_nCount; i++)   {
                if (m_pMachine->GetCardInfo (i, &CI) == S_OK)   {
                    CTreeItem* pItem=GetRootItem (i);
					if (pItem)   {
						CDGCardsItemInfo *pItemInfo=(CDGCardsItemInfo *) pItem->GetNodeInfo ();
						pItemInfo->SetCI (&CI);
					}
                }
            }
        /* rinfreschiamo il controllo */
        Invalidate (FALSE);
    }
}

/*
** GetCardName :
*/
CString CAdamoDGCards::GetCardName (CCmdGetCardInfo* pCI)
{
    CString str;
    switch (pCI->eTipo)   {
        case enYoupsilon :
            str=LOADSTRING (IDS_ADAMODGCARDS_10);
            break;
        case enOmikron :
            str=LOADSTRING (IDS_ADAMODGCARDS_11);
            break;
        default :
            break;
    }
    return str;
}

/*
** GetCardID :
*/
CString CAdamoDGCards::GetCardID (CCmdGetCardInfo* pCI)
{
    CString str;
    str.Format ("%d", pCI->Pci.nIDKinema);
    return str;
}

/*
** GetPosizionePCI :
*/
CString CAdamoDGCards::GetPosizionePCI (CCmdGetCardInfo* pCI)
{
    CString str;
    str.Format (LOADSTRING (IDS_ADAMODGCARDS_9), pCI->Pci.awBusPCI[0], pCI->Pci.awBusPCI[1], pCI->Pci.awBusPCI[2]);
    return str;
}

/*
** GetCardVersion :
*/
CString CAdamoDGCards::GetCardVersion (CCmdGetCardInfo* pCI)
{
    CString str;
    str.Format ("%d", pCI->Pci.nVersioneScheda);
    return str;
}

/*
** GetCardDate :
*/
CString CAdamoDGCards::GetCardDate (CCmdGetCardInfo* pCI)
{
    CTime t(pCI->Pci.acDataRilascioFPGA[2]+2000, pCI->Pci.acDataRilascioFPGA[1], pCI->Pci.acDataRilascioFPGA[0], 0, 0, 0);
    return t.Format ("%d %B %Y");
}

/*
** OnPaintColumn :
*/
void CAdamoDGCards::OnPaintColumn (LPDRAWITEMSTRUCT lpDrawItemStruct, int nColumn, CRect* rc)
{
    CTreeItem* pItem=(CTreeItem*)lpDrawItemStruct->itemData;
    if (pItem)   {
        CDGCardsItemInfo *pItemInfo=(CDGCardsItemInfo *) pItem->GetNodeInfo ();
        if (pItemInfo)   {
            switch (nColumn)   {
                case 5 :
                    break;
                case 6 :
                    Disegna24Volt (pItemInfo->GetCI ()->Pci.nPresenza24Volt, lpDrawItemStruct, rc);
                    break;
                case 7 :
                    DisegnaContatore (pItemInfo->GetCI ()->Pci.nContatore, lpDrawItemStruct, rc);
                    break;
            }
        }
    }
}

/*
** Disegna24Volt :
*/
void CAdamoDGCards::Disegna24Volt (int nPresenza24Volt, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc)
{
    if (nPresenza24Volt)
        ((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (lpDrawItemStruct->hDC, m_bmp24VoltOn, (short)rc->left, (short)rc->top, RGB (0, 128, 128));
    else
        ((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (lpDrawItemStruct->hDC, m_bmp24VoltOff, (short)rc->left, (short)rc->top, RGB (0, 128, 128));
}

/*
** DisegnaContatore :
*/
void CAdamoDGCards::DisegnaContatore (int nContatore, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc)
{
    char szBuffer[32];

    sprintf (szBuffer, "%d", nContatore);
    ::DrawText (lpDrawItemStruct->hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
}

/*
** SetMachine :
*/
void CAdamoDGCards::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** InitializeGrid :
*/
void CAdamoDGCards::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
}

/*
** OnControlLButtonDown :
*/
void CAdamoDGCards::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
}

/*
** GetIcon :
*/
int CAdamoDGCards::GetIcon(const CTreeItem* pItem)
{
    int n = 0;

    if (pItem != NULL)
		n = GetData (pItem)->GetImage();
	return n;
}

/*
** GetCellRGB :
*/
COLORREF CAdamoDGCards::GetCellRGB (void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** AddCardLine :
*/
void CAdamoDGCards::AddCardLine (CCmdGetCardInfo *pCI)
{
	CDGCardsItemInfo* lp = new CDGCardsItemInfo();
	lp->SetImage (8);
	lp->SetItemText (GetCardName (pCI));
	lp->AddSubItemText (GetCardID (pCI));
	lp->AddSubItemText (GetPosizionePCI (pCI));
	lp->AddSubItemText (GetCardVersion (pCI));
	lp->AddSubItemText (GetCardDate (pCI));
	lp->AddSubItemText ("");
	lp->AddSubItemText ("");
	lp->AddSubItemText ("");
    lp->SetCI (pCI);
	InsertRootItem (lp);
}

/*
** OnChangePsw :
*/
void CAdamoDGCards::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
}
