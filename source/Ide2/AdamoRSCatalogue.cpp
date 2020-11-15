// AdamoRSCatalogue.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "ColumnTreeView.h"
#include "AdamoRSDoc.h"
#include "adamorscatalogue.h"
#include "paredit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
** PreTranslateMessage :
*/
BOOL CAdamoRSTree::PreTranslateMessage(MSG* pMsg) 
{
    /* vediamo se e' stato schiacciato un tasto sensibile */
	if (pMsg->message == WM_KEYDOWN)   {
		switch(pMsg->wParam)   {
		    case VK_F2:
                /* rename di una label */    
			    m_pRSCatalogue->OnRename();
			    break;
		    case VK_DELETE:
                /* cancellazione di un  nodo */
                m_pRSCatalogue->OnRSCatDeleteResource ();
			    break;
            case VK_CONTROL :
                m_bCtrlPressed=true;
                break;
		    case VK_RETURN:
                /* espansione o collasso di un nodo */
                if (!m_pRSCatalogue->m_pEdit)   {
                    m_pRSCatalogue->OnExpandRequest ();
			        break;
                }
		    default:
			    /* comportamento standard */
			    return CXTPTreeCtrl::PreTranslateMessage(pMsg);
		}
		return TRUE;
	}
	if (pMsg->message == WM_KEYUP)   {
		switch(pMsg->wParam)   {
            case VK_CONTROL :
                m_bCtrlPressed=false;
                break;
		    default:
			    /* comportamento standard */
			    return CXTPTreeCtrl::PreTranslateMessage(pMsg);
        }
		return TRUE;
    }
    if (pMsg->message == WM_LBUTTONDBLCLK)   {
        if (m_bCtrlPressed)   {
            CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
            CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
            if (pView)   {
                pView->EditRSResource ();
                m_bCtrlPressed=false;
                return TRUE;
            }
        }
    }
    /* chiamiamo la funzione base */
	return CXTPTreeCtrl::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSCatalogue

IMPLEMENT_DYNCREATE(CAdamoRSCatalogue, CPaneView)

CAdamoRSCatalogue::CAdamoRSCatalogue() : m_treeCtrl (this), m_pEdit (NULL), m_bInitialized (false)
{
}

CAdamoRSCatalogue::~CAdamoRSCatalogue()
{
}

BEGIN_MESSAGE_MAP(CAdamoRSCatalogue, CPaneView)
	//{{AFX_MSG_MAP(CAdamoRSCatalogue)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_NOTIFY(NM_RCLICK, ID_TREECTRL_RS_CATALOGUE, OnRClick)
	ON_NOTIFY(TVN_BEGINLABELEDIT, ID_TREECTRL_RS_CATALOGUE, OnBeginlabeledit)
	ON_NOTIFY(TVN_ENDLABELEDIT, ID_TREECTRL_RS_CATALOGUE, OnEndlabeledit)
	ON_NOTIFY(TVN_SELCHANGED, ID_TREECTRL_RS_CATALOGUE, OnSelChanged)
	ON_NOTIFY(NM_DBLCLK, ID_TREECTRL_RS_CATALOGUE, OnDblClick)
    ON_COMMAND(ID_RESOURCES_ADDARESOURCE, OnRSCatAddResource)
    ON_COMMAND(ID_RESOURCES_DELETEARESOURCE, OnRSCatDeleteResource)
    ON_COMMAND(ID_RESOURCES_ADDAFOLDER, OnRSCatAddFolder)
    ON_COMMAND(ID_BITMAPS_IMPORTIMAGE, OnRSCatAddBitmap)
    ON_COMMAND(ID_BITMAPS_DELETEIMAGE, OnRSCatDeleteBitmap)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSCatalogue drawing

void CAdamoRSCatalogue::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSCatalogue diagnostics

#ifdef _DEBUG
void CAdamoRSCatalogue::AssertValid() const
{
	CPaneView::AssertValid();
}

void CAdamoRSCatalogue::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSCatalogue message handlers
void CAdamoRSCatalogue::OnInitialUpdate () 
{
    if (!m_bInitialized)   {
        InitView ();
        m_bInitialized=true;
    }
}

/*
** InitView :
*/
void CAdamoRSCatalogue::InitView ()
{
    CBitmap bm;
	/* creiamo il tree */
	m_treeCtrl.Create (WS_CHILD | WS_VISIBLE, CRect(0,0, 0, 0), this, ID_TREECTRL_RS_CATALOGUE);
    /* cambiamo lo stile al tree */
	DWORD dwStyle = GetWindowLong(GetTreeCtrl(), GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_EDITLABELS | TVS_FULLROWSELECT;
	SetWindowLong(GetTreeCtrl(), GWL_STYLE, dwStyle);
    /* creiamo la image-list */
    m_il.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    /* carichiamo i bitmap del tree */
    bm.LoadBitmap(IDB_RS_CATALOGUE);
    m_il.Add(&bm, RGB(128, 0, 128));
    /* set image */
	GetTreeCtrl().SetImageList (&m_il, TVSIL_NORMAL);
    /* aggiungiamo i tre nodi principali */
    InsertRootItems ();
    /* settiamo il font del programma */
    GetTreeCtrl().SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
}

/*
** AddCatalogueElement :
*/
bool CAdamoRSCatalogue::AddCatalogueElement (CString strName, CString strTrad, eResources eType, int nKind)
{
    char szBuffer[256], szBufferKey[256], *token, *p, *pKey, *qKey;

    bool b=false;
    if (eType>=0&&eType<=3)   {
        HTREEITEM ht=m_ahItem[(int)eType];
        if (strTrad=="")
            strcpy (szBuffer, strName);
        else
            strcpy (szBuffer, strTrad);
        strcpy (szBufferKey, strName);
        pKey=szBufferKey;
        qKey=strchr (pKey, '/');
        if (qKey)
            *qKey='\0';
        token=strtok (szBuffer, "//");
        while (token)   {
            int nIndex;
            if (nKind==RS_RESOURCE)
                switch (eType)   {
                    case eParametriche :
                        nIndex=0;
                        break;
                    case eUtensili :
                        nIndex=1;
                        break;
                    case eTestine :
                        nIndex=2;
                        break;
                    case eDialogs :
                        nIndex=3;
                        break;
                }
            else
                if (nKind==RS_FOLDER||nKind==RS_LOAD)
                    nIndex=5;
            p=strtok (NULL, "//");
            if (nKind==RS_LOAD)
                if (p)
                    nIndex=5;
                else
                    switch (eType)   {
                        case eParametriche :
                            nIndex=0;
                            break;
                        case eUtensili :
                            nIndex=1;
                            break;
                        case eTestine :
                            nIndex=2;
                            break;
                        case eDialogs :
                            nIndex=3;
                            break;
                    }
            ht=AddTreeItem (token, ht, nIndex);
            MettiLaChiave (ht, pKey);
            token=p;
            if (qKey)   {
                pKey=qKey+2;
                qKey=strchr (pKey, '/');
                if (qKey)
                    *qKey='\0';
            }
        }
        b=true;
    }
    return b;
}

/*
** RemoveCatalogueElement :
*/
bool CAdamoRSCatalogue::RemoveCatalogueElement ()
{   
    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem ();
    GetTreeCtrl().DeleteItem (htSel);
    return false;
}

/*
** OnRClick :
*/
void CAdamoRSCatalogue::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CPoint pt, p;
    UINT nFlags;

	GetCursorPos(&pt); p=pt;
	GetTreeCtrl().ScreenToClient(&pt);
	HTREEITEM hItem = GetTreeCtrl().HitTest(pt, &nFlags);
    if (hItem==m_ahItem[4]||GetTreeCtrl().GetParentItem(hItem)==m_ahItem[4])
        OnContextMenuBitmap (p);
    else
	    OnContextMenuResource (p);
	*pResult = 0;
}

/*
** OnDblClick :
*/
void CAdamoRSCatalogue::OnDblClick (NMHDR* pNMHDR, LRESULT* pResult)
{
    HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
    if (ht)   {
        CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
        CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
        if (pView)   {
            if (!GetTreeCtrl().ItemHasChildren (ht))
                pView->EditRSResource ();
        }
    }
}

/*
** OnContextMenuResource :
*/
void CAdamoRSCatalogue::OnContextMenuResource (CPoint point)
{
	CMenu menu;                                
	VERIFY(menu.LoadMenu(IDR_RS_FLOAT));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

/*
** OnContextMenuBitmap :
*/
void CAdamoRSCatalogue::OnContextMenuBitmap (CPoint point) 
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_RS_BITMAP));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

/*
** OnRSCatAddResource :
*/
void CAdamoRSCatalogue::OnRSCatAddResource()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->AddRSResource ();    
}

/*
** OnRSCatDeleteResource :
*/
void CAdamoRSCatalogue::OnRSCatDeleteResource()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->DeleteRSResource ();    
}

/*
** OnRenameRSResource :
*/
void CAdamoRSCatalogue::OnRenameRSResource(CString strPrev, CString strNew, eResources e)
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->RenameRSResource (strPrev, strNew, e);
}

/*
** OnRenameRSImage :
*/
bool CAdamoRSCatalogue::OnRenameRSImage (CString strImage, CString strID)
{
    bool b=false;
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        b=pView->RenameRSImage (strImage, strID);
    return b;
}

/*
** GetValidName :
*/
int CAdamoRSCatalogue::GetValidName (CString& str, eResources* e)
{
    CString strNew, strItem, strFormat;
    int n=-1, nCount=1;
    bool bFound=false;

    /* prendiamo la selezione corrente */
    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem ();
    if (htSel)   {
        /* andiamo a vedere se e' una parametrica, un utensile o una testina */
        HTREEITEM ht=htSel;
        while (!IsRSCatRoot (ht, e))
            ht=GetTreeCtrl().GetParentItem (ht);
        /* prendiamo un nome di base valido */
        strFormat=strNew=GetNewName (*e);
        /* vediamo se ha figli */
        if (GetTreeCtrl().ItemHasChildren (htSel))   {
            /* ha figli, vediamo se il nome proposto va bene */
            while (true)   {
                bFound=false;
                ht=GetTreeCtrl().GetChildItem(htSel);
                do   {
                    strItem=GetTreeCtrl().GetItemText (ht);
                    if (strItem==strFormat)   {
                        bFound=true; break;
                    }
                    ht=GetTreeCtrl().GetNextSiblingItem(ht);
                }   while (ht);
                if (!bFound)   {
                    str=strFormat; break;
                }
                /* proponiamone un altro */
                strFormat.Format ("%s(%d)", strNew, nCount++);
            }
        }
        else
            /* se non ne ha, il nome proposto va per forza bene */
            str=strNew;
        n=0;
    }
    str=BuildCompleteRSPath (str, *e, htSel);
    return n;
}

/*
** IsRSCatRoot : 
*/
bool CAdamoRSCatalogue::IsRSCatRoot (HTREEITEM ht, eResources* e)
{
    bool b=false;
    for (int i=0; i<NROOTITEMS; i++)
        if (ht==m_ahItem[i])   {
            *e=(eResources)i; b=true; break;
        }
    return b;
}

/*
** GetNewName :
*/
CString CAdamoRSCatalogue::GetNewName (eResources e)
{
    int nId;

    switch (e)   {
        case eParametriche :
            nId=IDS_ADAMORSCATALOGUE_1;
            break;
        case eUtensili :
            nId=IDS_ADAMORSCATALOGUE_2;
            break;
        case eTestine :
            nId=IDS_ADAMORSCATALOGUE_3;
            break;
        case eDialogs :
            nId=IDS_ADAMORSCATALOGUE_4;
            break;
    }
    return LOADSTRING (nId);
}

/*
** BuildRSPath :
*/
CString CAdamoRSCatalogue::BuildRSPath (HTREEITEM ht, eResources e)
{
    CString str;

    while (ht!=m_ahItem[(int)e])   {
        if (str=="")
            str=(char *)GetTreeCtrl().GetItemData(ht);
        else
            str=CString ((char *)GetTreeCtrl().GetItemData(ht))+"//"+str;
        ht=GetTreeCtrl().GetParentItem (ht);    
    }
    return str;    
}

/*
** BuildCompleteRSPath :
*/
CString CAdamoRSCatalogue::BuildCompleteRSPath (CString str, eResources e, HTREEITEM ht)
{
    CString strPath, strRet;
    strPath=BuildRSPath (ht, e);
    if (strPath=="")
        strRet=str;
    else
        strRet=strPath+"//"+str;
    return strRet;
}

/*
** AddTreeItem :
*/
HTREEITEM CAdamoRSCatalogue::AddTreeItem (CString token, HTREEITEM ht, int nImage)
{
    CString strItem;
    HTREEITEM htNode, htRet;    
    bool bFound;

    if (GetTreeCtrl().ItemHasChildren (ht))   {
        /* ha figli, cerchiamo se esiste gia' il nome */
        while (true)   {
            bFound=false;
            htNode=GetTreeCtrl().GetChildItem(ht);
            do   {
                strItem=GetTreeCtrl().GetItemText (htNode);
                if (strItem==token)   {
                    htRet=htNode; bFound=true; break;
                }
                htNode=GetTreeCtrl().GetNextSiblingItem(htNode);
            }   while (htNode);
            if (!bFound)   {
                htRet=GetTreeCtrl().InsertItem (token, nImage, nImage, ht);
                break;
            }
            else   {
                int nImageSet, nSelectedImageSet;
                GetTreeCtrl().GetItemImage (htRet, nImageSet, nSelectedImageSet);
                if (nImageSet==5)
                    GetTreeCtrl().SetItemImage (htRet, nImage, nImage);
                break;
            }
        }
    }
    else
        htRet=GetTreeCtrl().InsertItem (token, nImage, nImage, ht);
    return htRet;
}

/*
** GetDeleteName :
*/
bool CAdamoRSCatalogue::GetDeleteName (CString& strName, eResources* eType)
{
    CString strPath;
    bool b=false;

    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem ();
    if (!GetTreeCtrl().ItemHasChildren (htSel))   {
        HTREEITEM ht=htSel;
        while (!IsRSCatRoot (ht, eType))
            ht=GetTreeCtrl().GetParentItem (ht);
        strPath=BuildRSPath (htSel, *eType);
        if (strPath!="")   {
            strName=strPath; b=true;
        }
    }
    return b;
}

/*
** OnRename :
*/
void CAdamoRSCatalogue::OnRename()
{
    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem ();

    GetTreeCtrl().EditLabel(htSel);
}

/*
** OnBeginlabeledit :
*/
void CAdamoRSCatalogue::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
    LRESULT l=TRUE;

    HTREEITEM htSel=pTVDispInfo->item.hItem;
    if (!GetTreeCtrl().ItemHasChildren (htSel))
	    l = FALSE;
    *pResult = l;
    m_pEdit=new CParsedEdit;
    m_pEdit->SubclassEdit(GetTreeCtrl().GetEditControl ()->GetSafeHwnd (), PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE);
}

/*
** OnEndlabeledit :
*/
void CAdamoRSCatalogue::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    HTREEITEM ht,htSel;
	TV_DISPINFO *ptvinfo;
    LRESULT l=FALSE;
    CString str, str1;
    eResources eType;

	ptvinfo = (TV_DISPINFO *)pNMHDR;
	if (ptvinfo->item.pszText != NULL) 	{
        if (!strchr (ptvinfo->item.pszText, '//'))   {
            /* c'e' del testo */
		    ptvinfo->item.mask = TVIF_TEXT;
            /* andiamo a cercare il path completo e il tipo di risorsa */
            htSel=ht=ptvinfo->item.hItem;
            while (!IsRSCatRoot (ht, &eType))
                ht=GetTreeCtrl().GetParentItem (ht);
            if (eType==eBitmaps)   {
                if (!ExistItem (htSel, ptvinfo->item.pszText))  {
                    HTREEITEM h=GetTreeCtrl().GetSelectedItem ();
                    if (OnRenameRSImage (GetTreeCtrl().GetItemText (h), ptvinfo->item.pszText))
                        GetTreeCtrl().SetItem (&ptvinfo->item);
                }
            }
            else   {
                if (!ExistItem (htSel, ptvinfo->item.pszText))  {
                    str=BuildRSPath (htSel, eType);
		            GetTreeCtrl().SetItem(&ptvinfo->item);
                    SostituisciLaChiave (ptvinfo->item.hItem, ptvinfo->item.pszText);
                    str1=BuildRSPath (ptvinfo->item.hItem, eType);
                    OnRenameRSResource (str, str1, eType);
                    l=TRUE;
                }
            }
	    }
}
    _delete (m_pEdit);
	*pResult = l;
}

/*
** OnExpandRequest : 
*/
void CAdamoRSCatalogue::OnExpandRequest ()
{
    if (m_treeCtrl.IsCtrlPressed ())   {
        CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
        CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
        if (pView)
            pView->EditRSResource ();
        m_treeCtrl.CtrlPressed (false);
    }
    else   {
        HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
        if (ht)   {
            if (GetTreeCtrl().ItemHasChildren (ht))
                GetTreeCtrl().Expand  (ht, TVE_TOGGLE);
            else   {
                CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
                CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
                if (pView)
                    pView->EditRSResource ();
            }
        }
    }
}

/*
** IsAValidResource :
*/
bool CAdamoRSCatalogue::IsAValidResource ()
{
    eResources e;
    bool b=false;
    HTREEITEM htValidHandle;
    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem ();
    if (htSel)   {
        b=!IsRSCatRoot (htSel, &e);
        if (b)   {
            b=false;
            while (htSel)   {
                htValidHandle=htSel;
                htSel=GetTreeCtrl().GetParentItem (htSel);
                if (!htSel)
                    break;
            }
            for (int i=0; i<NROOTMACHINEITEMS; i++)
                if (htValidHandle==m_ahItem[i])   {
                    b=true; break;
                }    
        }
    }
    return b;
}

/*
** GetSelectedName :
*/
bool CAdamoRSCatalogue::GetSelectedName (CString& strName, eResources* eType)
{
    CString strPath;

    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem (), ht=htSel;
    while (!IsRSCatRoot (ht, eType))
        ht=GetTreeCtrl().GetParentItem (ht);
    strPath=BuildRSPath (htSel, *eType);
    strName=strPath;
    return false;
}

/*
** Clear :
*/
void CAdamoRSCatalogue::Clear ()
{
    DeleteAllData ();
    GetTreeCtrl().DeleteAllItems ();
    InsertRootItems ();
}

/*
** InsertRootItems :
*/
void CAdamoRSCatalogue::InsertRootItems ()
{
    /* aggiungiamo i tre nodi principali */
    m_ahItem[0]=GetTreeCtrl().InsertItem (LOADSTRING (IDS_ADAMORSCATALOGUE_5), 0, 0);
    m_ahItem[1]=GetTreeCtrl().InsertItem (LOADSTRING (IDS_ADAMORSCATALOGUE_6), 1, 1);
    m_ahItem[2]=GetTreeCtrl().InsertItem (LOADSTRING (IDS_ADAMORSCATALOGUE_7), 2, 2);
    m_ahItem[3]=GetTreeCtrl().InsertItem (LOADSTRING (IDS_ADAMORSCATALOGUE_8), 3, 3);
    m_ahItem[4]=GetTreeCtrl().InsertItem (LOADSTRING (IDS_ADAMORSCATALOGUE_9), 4, 4);
}

/*
** OnRSCatAddFolder :
*/
void CAdamoRSCatalogue::OnRSCatAddFolder()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->AddRSFolder ();    
}

/*
** OnRSCatAddBitmap :
*/
void CAdamoRSCatalogue::OnRSCatAddBitmap()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)   {
        pView->AddRSBitmap ();
    }
}

/*
** OnRSCatDeleteBitmap :
*/
void CAdamoRSCatalogue::OnRSCatDeleteBitmap()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)   {
        pView->DeleteRSBitmap ();
    }
}

/*
** GetValidFolderName :
*/
int CAdamoRSCatalogue::GetValidFolderName (CString& str, eResources* e)
{
    CString strNew, strItem, strFormat;
    int n=-1, nCount=1;
    bool bFound=false;

    /* prendiamo la selezione corrente */
    HTREEITEM htSel=GetTreeCtrl().GetSelectedItem ();
    if (htSel)   {
        /* andiamo a vedere se e' una parametrica, un utensile o una testina */
        HTREEITEM ht=htSel;
        while (!IsRSCatRoot (ht, e))
            ht=GetTreeCtrl().GetParentItem (ht);
        /* prendiamo un nome di base valido */
        strFormat=strNew=LOADSTRING (IDS_ADAMORSCATALOGUE_10);
        /* vediamo se ha figli */
        if (GetTreeCtrl().ItemHasChildren (htSel))   {
            /* ha figli, vediamo se il nome proposto va bene */
            while (true)   {
                bFound=false;
                ht=GetTreeCtrl().GetChildItem(htSel);
                do   {
                    strItem=GetTreeCtrl().GetItemText (ht);
                    if (strItem==strFormat)   {
                        bFound=true; break;
                    }
                    ht=GetTreeCtrl().GetNextSiblingItem(ht);
                }   while (ht);
                if (!bFound)   {
                    str=strFormat; break;
                }
                /* proponiamone un altro */
                strFormat.Format ("%s(%d)", strNew, nCount++);
            }
        }
        else
            /* se non ne ha, il nome proposto va per forza bene */
            str=strNew;
        n=0;
    }
    str=BuildCompleteRSPath (str, *e, htSel);
    return n;
}

/*
** AddImageElement :
*/
bool CAdamoRSCatalogue::AddImageElement (CString strName)
{
    GetTreeCtrl().InsertItem (strName, 4, 4, m_ahItem[4]);
    return false;
}

/*
** GetSelectedImageID :
*/
CString CAdamoRSCatalogue::GetSelectedImageID ()
{
    CString str;

    HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
    if (ht)
        str=GetTreeCtrl().GetItemText (ht);
    return str;
}

/*
** DeleteImageItem :
*/
void CAdamoRSCatalogue::DeleteImageItem (CString strID)
{
    CString strItem;
    bool bFound=false;

    HTREEITEM ht=GetTreeCtrl().GetChildItem (m_ahItem[4]);
    if (ht)   {
        do   {
            strItem=GetTreeCtrl().GetItemText (ht);
            if (strItem==strID)   {
                bFound=true; break;
            }
            ht=GetTreeCtrl().GetNextSiblingItem(ht);
        }   while (ht);
    }
    if (bFound)
        GetTreeCtrl ().DeleteItem (ht);
}

/*
** GetResourceType :
*/
eResources CAdamoRSCatalogue::GetResourceType (HTREEITEM ht)
{
    int i=0;
    bool b=false;
    HTREEITEM htValidHandle=NULL;

    if (ht)   {
        while (ht)   {
            htValidHandle=ht;
            ht=GetTreeCtrl().GetParentItem (ht);
            if (!ht)
                break;
        }
        for (i=0; i<NROOTITEMS; i++)
            if (htValidHandle==m_ahItem[i])
                break;
    }
    return (eResources)i;
}

/*
** OnSelChanged :
*/
void CAdamoRSCatalogue::OnSelChanged (NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pnmtv = (NM_TREEVIEW FAR *) pNMHDR;
    HTREEITEM htSel=pnmtv->itemNew.hItem;

    eResources e=GetResourceType (htSel);
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (e==eBitmaps)   {
        if (pView&&GetTreeCtrl().GetParentItem (htSel))
            pView->SelectRSBitmap (GetTreeCtrl().GetItemText (htSel));
    }
}

/*
** DeleteData :
*/
void CAdamoRSCatalogue::DeleteData (HTREEITEM hItem)
{
    HTREEITEM ht;

    if (hItem)   {
        ht=GetTreeCtrl().GetChildItem(hItem);
        do   {
            if (GetTreeCtrl().ItemHasChildren (ht))
                DeleteData (ht);
            else   {
                char *p = (char *)GetTreeCtrl().GetItemData (ht);
                if (p)
                    _delete (p);
            }
            ht=GetTreeCtrl().GetNextSiblingItem (ht);
        }   while (ht);
    }
}

/*
** DeleteAllData :
*/
void CAdamoRSCatalogue::DeleteAllData ()
{
	HTREEITEM hti=GetTreeCtrl().GetRootItem ();
	while (hti)   { 			
		if (GetTreeCtrl().ItemHasChildren(hti))
			//itero l'abero
			DeleteData (hti);
		hti=GetTreeCtrl().GetNextItem (hti, TVGN_NEXT);
	}	
}

/*
** MettiLaChiave :
*/
void CAdamoRSCatalogue::MettiLaChiave (HTREEITEM ht, CString strName)
{
    char *p=new char [strName.GetLength ()+1];
    strcpy (p, strName);
    GetTreeCtrl ().SetItemData (ht, (DWORD)p);
}

/*
** SostituisciLaChiave :
*/
void CAdamoRSCatalogue::SostituisciLaChiave (HTREEITEM ht, CString strName)
{
    char *p=(char *)GetTreeCtrl ().GetItemData (ht);
    if (p)   {
        delete p;
        MettiLaChiave (ht, strName);
    }
}

/*
** ExistItem :
*/
bool CAdamoRSCatalogue::ExistItem (HTREEITEM ht, CString strName)
{
    HTREEITEM htParent = GetTreeCtrl().GetParentItem(ht);
    ht=GetTreeCtrl().GetChildItem (htParent);
    CString strItem;
    bool bFound = false;

    do   {
        strItem=GetTreeCtrl().GetItemText (ht);
        if (strItem==strName)   {
            bFound=true; break;
        }
        ht=GetTreeCtrl().GetNextSiblingItem (ht);
    }   while (ht);
    return bFound;
}

/*
** OnSize :
*/
void CAdamoRSCatalogue::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_treeCtrl.GetSafeHwnd ()) )
		m_treeCtrl.SetWindowPos (NULL, 0, 0, cx, cy, 0);
}

/*
** RefreshMetrics :
*/
void CAdamoRSCatalogue::RefreshMetrics(AppTheme nTheme)
{
	CPaneView::RefreshMetrics(nTheme);
	switch (nTheme)
	{
		case themeVS2010: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Blue); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);

			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2012Light: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Light); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);

			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2012Dark: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Dark); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);
			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: 
			m_treeCtrl.SetTheme (xtpControlThemeVisualStudio2015);
			m_treeCtrl.ModifyStyle (TVS_HASLINES, 0);
			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
	}	
}
