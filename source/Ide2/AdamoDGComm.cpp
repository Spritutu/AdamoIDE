// AdamoDGComm.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoDGComm.h"
#include "AdamoDGSearch.h"
#include "AdamoDGFilters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGComm

IMPLEMENT_DYNCREATE(CAdamoDGComm, CTreeView)

CAdamoDGComm::CAdamoDGComm() : m_bEnabled (false), m_fnt (NULL), m_pDoc (NULL), m_pDlgSearch (NULL)
{
}

CAdamoDGComm::~CAdamoDGComm()
{
	_delete (m_fnt);
}


BEGIN_MESSAGE_MAP(CAdamoDGComm, CTreeView)
	//{{AFX_MSG_MAP(CAdamoDGComm)
	ON_COMMAND(IDM_DG_START_TRACE, OnDGStartTrace)
	ON_COMMAND(IDM_DG_STOP_TRACE, OnDGStopTrace)
	ON_UPDATE_COMMAND_UI(IDM_DG_START_TRACE, OnUpdateDGStartTrace)
	ON_UPDATE_COMMAND_UI(IDM_DG_STOP_TRACE, OnUpdateDGStopTrace)
	ON_COMMAND(IDM_DG_REFRESH, OnDGRefresh)
	ON_WM_DESTROY()
	ON_COMMAND(IDM_DG_CLEAR_FILTER, OnDGClearFilter)
	ON_UPDATE_COMMAND_UI(IDM_DG_CLEAR_FILTER, OnUpdateDGClearFilter)
	ON_COMMAND(IDM_DG_FILTER_ERRORS, OnDGFilterErrors)
	ON_UPDATE_COMMAND_UI(IDM_DG_FILTER_ERRORS, OnUpdateDGFilterErrors)
	ON_COMMAND(IDM_DG_FIND, OnDGFind)
	ON_UPDATE_COMMAND_UI(IDM_DG_FIND, OnUpdateDGFind)
	ON_COMMAND(IDM_DG_INTERPRETA, OnDGInterpreta)
	ON_UPDATE_COMMAND_UI(IDM_DG_INTERPRETA, OnUpdateDGInterpreta)
	ON_UPDATE_COMMAND_UI(IDM_DG_REFRESH, OnUpdateDGRefresh)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGComm drawing

void CAdamoDGComm::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGComm diagnostics

#ifdef _DEBUG
void CAdamoDGComm::AssertValid() const
{
	CTreeView::AssertValid();
}

void CAdamoDGComm::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGComm message handlers

void CAdamoDGComm::OnDGStartTrace() 
{
	// TODO: Add your command handler code here
    CAdamoMachine* pMachine;
	
    CProject* pPrj=GETWS()->GetActiveProject();
    if (m_pDoc)   {
        if (pPrj)   {
            pMachine = pPrj->GetMachine ();
            if (pMachine)   {
                pMachine->StartCommTrace ();
                m_pDoc->Reset ();
                GetTreeCtrl().DeleteAllItems ();
                m_bEnabled=true;
            }
        }
    }
}

void CAdamoDGComm::OnDGStopTrace() 
{
	// TODO: Add your command handler code here
    CAdamoMachine* pMachine;
	
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)   {
        pMachine = pPrj->GetMachine ();
        if (pMachine)   {
            pMachine->StopCommTrace ();        
            OnDGRefresh();
            m_bEnabled=false;
        }
    }
}

void CAdamoDGComm::OnUpdateDGStartTrace(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable (!m_bEnabled);
}

void CAdamoDGComm::OnUpdateDGStopTrace(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable (m_bEnabled);
}

/*
** OnInitialUpdate : inizializzazione della view
*/
void CAdamoDGComm::OnInitialUpdate() 
{
    CAdamoMachine* pMachine;
	CTreeView::OnInitialUpdate();
	
	/* vediamo se il trace e' gia' stato abilitato */
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)   {
        pMachine = pPrj->GetMachine ();
        if (pMachine)
            m_bEnabled=pMachine->IsCommTraceEnabled ()!=0;
    }
    /* settiamo il font fisso nel tree */
    m_fnt=((CMainFrame*)AfxGetMainWnd())->CreateFont ("Courier", 14, FW_NORMAL);
    GetTreeCtrl().SetFont (m_fnt);
    /* ora creiamo la image list per le iconcine */
    m_imgList.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    m_bmpTree.LoadBitmap(IDB_DG_TREE);
    m_imgList.Add(&m_bmpTree, RGB(0, 128, 128));
    /* set image */
	GetTreeCtrl().SetImageList (&m_imgList, TVSIL_NORMAL);
    /* settiamo il puntatore al oggetto document corretto */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enDiagnosticTemplate);
    if (pTmpl)
        m_pDoc=(CAdamoDGDocument*)pTmpl->GetDocument ("");
    /* rinfreschiamo l'albero se c'e' gia' un file */
    OnDGRefresh();
}

/*
** OnDGRefresh : comando di refresh dei dati dal file di trace al tree-view
*/
void CAdamoDGComm::OnDGRefresh() 
{
    if (m_pDoc)
        m_pDoc->UpdateTree (this);
}

/*
** OnDestroy : distruzione finestra.
*/
void CAdamoDGComm::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	// TODO: Add your message handler code here
	_delete (m_fnt);
}

void CAdamoDGComm::OnDGClearFilter() 
{
	// TODO: Add your command handler code here
	
}

void CAdamoDGComm::OnUpdateDGClearFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ();
}

void CAdamoDGComm::OnDGFilterErrors() 
{
	// TODO: Add your command handler code here
    CAdamoDGFilters dlgFilters (this);

    dlgFilters.SetTreeView (this);
    dlgFilters.DoModal ();
}

void CAdamoDGComm::OnUpdateDGFilterErrors(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ();
}

void CAdamoDGComm::OnDGFind() 
{
	// TODO: Add your command handler code here
    if (!m_pDlgSearch)   {
        m_pDlgSearch = new CAdamoDGSearch(this);
        if (m_pDlgSearch != NULL)   {
            m_pDlgSearch->Create(CAdamoDGSearch::IDD);
            m_pDlgSearch->SetDGView (this);
        }
    }
}

void CAdamoDGComm::OnUpdateDGFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ();
}

void CAdamoDGComm::OnDGInterpreta() 
{
	// TODO: Add your command handler code here
	
}

void CAdamoDGComm::OnUpdateDGInterpreta(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ();
}

void CAdamoDGComm::OnUpdateDGRefresh(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
	pCmdUI->Enable ();
}

void CAdamoDGComm::OnDblClick (NMHDR* pNMHDR, LRESULT* pResult)
{
    if (m_pDoc)   {
        HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
        if (!GetTreeCtrl().ItemHasChildren (ht))   {
            if (GetTreeCtrl().GetParentItem (ht)==NULL)
                /* esplodiamo l'albero */
                m_pDoc->ExplodeTree (this, ht);
        }
        else   {
            /* chiudiamo i rami */
           HTREEITEM hItem, hItemToDelete;
           hItem = hItemToDelete = GetTreeCtrl().GetChildItem (ht);
           while (hItem != NULL)   {
              hItem = GetTreeCtrl().GetNextSiblingItem(hItem);
              GetTreeCtrl().DeleteItem (hItemToDelete);
              hItemToDelete=hItem;
           }
        }
    }
}

/*
** OnCloseSearchDialog :
*/
void CAdamoDGComm::OnCloseDGView ()
{
    _delete (m_pDlgSearch);
}

/*
** Find :
*/
int CAdamoDGComm::FindNext (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase)
{
    /* prendiamo l'item selezionato */
    HTREEITEM htSel=GetTreeCtrl ().GetSelectedItem (), ht, htStorico, htParent, htChild;

    if (htSel)   {
        /* comiciamo la ricerca */
        htStorico=htSel;
        /* scorriamolo e verifichiamo se troviamo */
        if (htParent=GetTreeCtrl().GetParentItem (htSel))   {
            while (htSel)   {
                if (Trovato (htSel, str, bMatchWholeWord, bUpperCase))
                    return 0;
                htSel=GetTreeCtrl().GetNextItem (htSel, TVGN_NEXT);
                if (htSel==htStorico)
                    return 1;
            }
        }
        ht=htParent ? htParent : htSel;
        ht=GetTreeCtrl().GetNextItem (ht, TVGN_NEXT);
        if (!ht)
            ht=GetTreeCtrl().GetRootItem ();
        do   {
            if (Trovato (ht, str, bMatchWholeWord, bUpperCase))
                break;
            if (htChild=GetTreeCtrl ().GetChildItem (ht))   {
                while (htChild)   {
                    if (Trovato (htChild, str, bMatchWholeWord, bUpperCase))
                        return 0;
                    htChild=GetTreeCtrl ().GetNextItem (htChild, TVGN_NEXT);
                    if (htChild==htStorico)
                        return 0;
                }
            }
            ht=GetTreeCtrl ().GetNextItem (ht, TVGN_NEXT);
            if (ht==htStorico)
                return 0;
            if (!ht)
                ht=GetTreeCtrl().GetRootItem ();
        } while (true);
    }
    return 0;
}

/*
** FindPrev :
*/
int CAdamoDGComm::FindPrev (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase)
{
    /* prendiamo l'item selezionato */
    HTREEITEM htSel=GetTreeCtrl ().GetSelectedItem (), ht, htStorico, htParent, htChild, htIterator, htLast;

    if (htSel)   {
        htLast=FindLastElement ();
        /* comiciamo la ricerca */
        htStorico=htSel;
        /* scorriamolo e verifichiamo se troviamo */
        if (htParent=GetTreeCtrl().GetParentItem (htSel))   {
            while (htSel)   {
                if (Trovato (htSel, str, bMatchWholeWord, bUpperCase))
                    return 0;
                htSel=GetTreeCtrl().GetNextItem (htSel, TVGN_PREVIOUS);
                if (htSel==htStorico)
                    return 1;
            }
        }
        ht=htParent ? htParent : htSel;
        ht=GetTreeCtrl().GetNextItem (ht, TVGN_PREVIOUS);
        if (!ht)
            ht=htLast;
        do   {
            if (Trovato (ht, str, bMatchWholeWord, bUpperCase))
                break;
            if (htChild=GetTreeCtrl ().GetChildItem (ht))   {
                htIterator=htChild;
                while (htIterator)   {
                    htChild=htIterator;
                    htIterator=GetTreeCtrl().GetNextItem (htIterator, TVGN_NEXT);    
                }
                while (htChild)   {
                    if (Trovato (htChild, str, bMatchWholeWord, bUpperCase))
                        return 0;
                    htChild=GetTreeCtrl ().GetNextItem (htChild, TVGN_PREVIOUS);
                    if (htChild==htStorico)
                        return 0;
                }
            }
            ht=GetTreeCtrl ().GetNextItem (ht, TVGN_PREVIOUS);
            if (ht==htStorico)
                return 0;
            if (!ht)
                ht=htLast;
        } while (true);
    }
    return 0;
}

/*
** Trovato
*/
int CAdamoDGComm::Trovato (HTREEITEM ht, const char* str, bool bMatchWholeWord, bool bUpperCase)
{
    bool b=false;

    if (((CMainFrame*)AfxGetMainWnd ())->MatchString (str, GetTreeCtrl().GetItemText (ht), bMatchWholeWord, bUpperCase))   {
        /* abbiamo trovato un match */
        GetTreeCtrl ().Select (ht, TVGN_CARET);
        b=true;
    }
    return b;
}

/*
** PreCreateWindow :
*/
BOOL CAdamoDGComm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_SHOWSELALWAYS;
    cs.style |= TVS_FULLROWSELECT;
	return CTreeView::PreCreateWindow(cs);
}

/*
** FindLastElement :
*/
HTREEITEM CAdamoDGComm::FindLastElement ()
{
    HTREEITEM htIterator=GetTreeCtrl().GetRootItem (), ht;
    while (htIterator)   {
        ht=htIterator;
        htIterator=GetTreeCtrl().GetNextItem (htIterator, TVGN_NEXT);    
    }
    return ht;
}


