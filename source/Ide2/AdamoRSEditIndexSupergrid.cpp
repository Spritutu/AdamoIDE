// AdamoDGCards.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSEditIndexSupergrid.h"
#include "idemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTablesSuperGrid

CAdamoRSEditIndexSuperGrid::CAdamoRSEditIndexSuperGrid() : m_pRootItem (NULL)
{
}

BEGIN_MESSAGE_MAP(CAdamoRSEditIndexSuperGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CTablesSuperGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** FillColumns : configura le colonne del supergridctrl
*/
void CAdamoRSEditIndexSuperGrid::FillColumns ()
{
    CRect rc;
    CString str;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
    GetWindowRect (rc);
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = rc.right/2;
    str=LOADSTRING (IDS_ADAMORSEDITINDEXSUPERGRID_1);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn (0,&lvColumn);
	lvColumn.cx = (rc.right/2)-18;
    str=LOADSTRING (IDS_ADAMORSEDITINDEXSUPERGRID_2);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=1;
    InsertColumn(1,&lvColumn);
}

/*
** OnCreate :
*/
int CAdamoRSEditIndexSuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
        return -1;
    CBitmap* pBmp=new CBitmap;
    pBmp->LoadBitmap (IDB_RS_EDITINDEX);
    m_image.Add (pBmp, RGB(0, 128, 128));
	SetImageList (&m_image, LVSIL_SMALL);
    SetBkColor (GetSysColor (COLOR_WINDOW));
    m_cxImage=m_cyImage=16;
    delete pBmp;
	return 0;
}

/*
** FillGrid :
*/
void CAdamoRSEditIndexSuperGrid::FillGrid () 
{
    CStringList slSubIndex, slSubValue;
    int i=0;

    CAdamoRSDoc* pDoc=(CAdamoRSDoc*) ((CIdeApp *)AfxGetApp ())->GetTemplate (enResourcesTemplate)->GetDocument ("");
    if (pDoc)   {
        if (m_strTable!=""&&m_strIndex!="")   {
            pDoc->GetRSFile ()->GetSubIndexList (m_strTable, m_strIndex, slSubIndex);
            GetSubValueList (m_strIndexValue, slSubValue);
            CItemInfo* lp = new CItemInfo ();
	        lp->SetImage (2);
	        lp->SetItemText (m_strIndex);
            m_pRootItem=InsertRootItem (lp);
            if (slSubIndex.GetCount ())   {
                for (POSITION pos=slSubIndex.GetHeadPosition (), pos1=slSubValue.GetHeadPosition (); pos&&pos1;)   {
                    CItemInfo* lp = new CItemInfo ();
	                lp->SetImage (3);
	                lp->SetItemText (slSubIndex.GetNext (pos));
                    lp->AddSubItemText (slSubValue.GetNext (pos1));
                    InsertItem (m_pRootItem, lp, TRUE);
                    i++;
                }
            }
            Expand (m_pRootItem, 0);
        }
    }
}

/*
** InitializeGrid :
*/
void CAdamoRSEditIndexSuperGrid::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        m_strNomeFile=pPrj->GetResourcesPathName ();
        FillColumns ();
        FillGrid ();
    }
}

/*
** GetIcon :
*/
int CAdamoRSEditIndexSuperGrid::GetIcon(const CTreeItem* pItem)
{
    int n = 0;

    if (pItem != NULL)
		n = GetData (pItem)->GetImage();
	return n;
}

/*
** GetCellRGB :
*/
COLORREF CAdamoRSEditIndexSuperGrid::GetCellRGB(void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** GetResults : 
*/
CString CAdamoRSEditIndexSuperGrid::GetResults ()
{
    CTreeItem* pItem;
    CString str;

    m_strIndexValue="";
    if (m_pRootItem)   {
        for (POSITION pos=GetHeadPosition (m_pRootItem); pos; )   {
            pItem=GetNextChild (m_pRootItem, pos);
            CItemInfo* lp=pItem->GetNodeInfo ();
            if (lp)
                str.Format ("%s|", lp->GetSubItem (0));
                m_strIndexValue+=str;
        }
        m_strIndexValue.SetAt (m_strIndexValue.GetLength ()-1,'\0');
    }
    return m_strIndexValue;
}

/*
** OnVkReturn :
*/
BOOL CAdamoRSEditIndexSuperGrid::OnVkReturn (void)
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

/*
** GetSubValueList : 
*/
void CAdamoRSEditIndexSuperGrid::GetSubValueList (CString strIndexValue, CStringList& sl)
{
    CStringArray ar;
    int i;

    ar.SetSize (NMAX_DBKEYS);
    ScomponiInStringhe (ar, m_strIndexValue, '|');
    for (i=0; i<NMAX_DBKEYS; i++)
        sl.AddTail (ar[i]);
}

/*
** ScomponiInStringhe : 
*/
void CAdamoRSEditIndexSuperGrid::ScomponiInStringhe (CStringArray& ar, CString str, char ch)
{
    int nIni=0, nFin, i=0;

    if (str!="")   {
        while ((nFin=str.Find ('|', nIni))!=-1)   {
            ar[i]=str.Mid (nIni, nFin-nIni);
            i++; nIni=nFin+1;
        }
        ar[i]=str.Mid (nIni, str.GetLength ()-nIni);
    }
}

/*
** OnControlLButtonDown :
*/
void CAdamoRSEditIndexSuperGrid::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	if (ht.iSubItem!=0)   {
        CTreeItem *t=GetTreeItem (ht.iItem);
        if (t)
            if (t->GetNodeInfo ()->GetItemCount ()>0)
                EditLabelEx(ht.iItem, ht.iSubItem);
    }
}
