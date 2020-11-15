// AdamoDGCards.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoShowMatrixSupergrid.h"
#include "idemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTablesSuperGrid

CShowMatrixSuperGrid::CShowMatrixSuperGrid()
{
}

BEGIN_MESSAGE_MAP(CShowMatrixSuperGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CTablesSuperGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** FillColumns : configura le colonne del supergridctrl
*/
void CShowMatrixSuperGrid::FillColumns ()
{
    CString str;
    CRect rc;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
    GetWindowRect (rc);
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = rc.right;
    str=LOADSTRING (IDS_ADAMOSHOWGLOBALSSUPERGRID_1);
	lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn (0,&lvColumn);
}

/*
** OnCreate :
*/
int CShowMatrixSuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_image.Create (IDB_SHOWGLOBALS, 16, 1, RGB(0, 128, 128)))
        return -1;
	SetImageList (&m_image, LVSIL_SMALL);
    SetBkColor (GetSysColor (COLOR_WINDOW));
    m_cxImage=m_cyImage=16;
	return 0;
}

/*
** FillGrid :
*/
void CShowMatrixSuperGrid::FillGrid () 
{
    /* riempiamolo di variabili globali */
    CString str;
    stGlobalDescriptor* gd;

    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CVariableContainer& vc=pPrj->GetVariableContainer ();
        CMapStringToPtr&    map=vc.GetVarsContainer ();
        for (POSITION pos = map.GetStartPosition(); pos; )   {
            map.GetNextAssoc (pos, str, (void*&) gd);
			if (pPrj->GetVariableContainer().IsLegacyVariable (str))   {
				switch (gd->nType)   {
					case GLOBALVARIABLE :
						break;
					case GLOBALARRAY :
						break;
					case GLOBALMATRIX :
						FillMatrix (gd);
						break;
				}
			}
        }
    }
}

/*
** InitializeGrid :
*/
void CShowMatrixSuperGrid::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
    FillGrid ();
}

/*
** GetIcon :
*/
int CShowMatrixSuperGrid::GetIcon(const CTreeItem* pItem)
{
    int n = 0;

    if (pItem != NULL)
		n = GetData (pItem)->GetImage();
	return n;
}

/*
** GetCellRGB :
*/
COLORREF CShowMatrixSuperGrid::GetCellRGB(void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** FillMatrix :
*/
void CShowMatrixSuperGrid::FillMatrix (stGlobalDescriptor* gd)
{
    CTreeItem *pRoot, *pItem;
    CString str;
    int i=0, n;

    CShowMatrixItemInfo* lp = new CShowMatrixItemInfo ();
	lp->SetImage (4);
	lp->SetItemText (gd->strName);
    pRoot=InsertRootItem (lp);
    while (i<gd->mat.nRows)   {
        lp = new CShowMatrixItemInfo ();
	    lp->SetImage (4);
        str.Format ("[%d]", i);
        lp->SetItemText (str);
	    pItem=InsertItem (pRoot, lp);
        n=0;
        while (n<gd->mat.nCols)   {
            lp = new CShowMatrixItemInfo ();
	        lp->SetImage (GetSGImage (gd->mat.anTypes[n]));
            lp->SetItemText (gd->mat.strColumnName[n]);
            lp->SetRow (i);
            lp->SetCol (n);
            lp->SetGD (gd);
	        InsertItem (pItem, lp);
            n++;
        }
        i++;
    }
}

/*
** GetSGImage :
*/
int CShowMatrixSuperGrid::GetSGImage (int n)
{
    int i=0;

    switch (n)   {
        case LUA_TSTRING :
            i=0;
            break;
        case LUA_TNUMBER :
            i=1;
            break;
        case LUA_TBOOLEAN :
            i=2;
            break;
    }
    return i;
}
/*
** GetResults : 
*/
int CShowMatrixSuperGrid::GetResults (CString* strName)
{

    bool b=false;

    int nIndex=GetSelectedItem ();
    if (nIndex!=-1)   {
        CTreeItem* pTreeItem=GetTreeItem (nIndex);
		while (GetParentItem (pTreeItem))
			pTreeItem = GetParentItem (pTreeItem);
        CShowMatrixItemInfo* pData=(CShowMatrixItemInfo*) GetData (pTreeItem);
		*strName = pData->GetItemText ();
		b = true;
    }
    return b;
}

BOOL CShowMatrixSuperGrid::OnVkReturn(void)
{
    int nIndex = GetSelectedItem();
    if(nIndex!=-1)   {
	    CTreeItem *pSelItem = GetTreeItem(nIndex);
	    if (pSelItem != NULL)   {
		    BOOL bRedraw=0;
		    if (ItemHasChildren(pSelItem))   {
			    SetRedraw(0);
			    int nScrollIndex=0;
			    if (IsCollapsed(pSelItem))   {		
				    if (OnItemExpanding(pSelItem, nIndex))   {
					    nScrollIndex = Expand(pSelItem, nIndex);
					    OnItemExpanded(pSelItem, nIndex);
					    bRedraw=1;
				    }
			    }	
			    else   {
			       if (OnCollapsing(pSelItem))   {
					    Collapse(pSelItem);
					    OnItemCollapsed(pSelItem);
					    bRedraw=1;
			       }
			    }
			    SetRedraw(1);
			    if (bRedraw)   {
				    CRect rc;
				    GetItemRect(nIndex,rc,LVIR_BOUNDS);
				    InvalidateRect(rc);
				    UpdateWindow();
				    EnsureVisible(nScrollIndex,1);
			    }
		    }
	    }
    }
    return 1;
}

void CShowMatrixSuperGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
}

