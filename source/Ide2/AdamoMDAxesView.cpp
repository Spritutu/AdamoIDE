// AdamoMDAxesView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "AdamoMDAxesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxesView

/* global variables */
CAdamoMDAxesView* CAdamoMDAxesView::m_pThis;

IMPLEMENT_DYNCREATE(CAdamoMDAxesView, CView)

CAdamoMDAxesView::CAdamoMDAxesView() : m_pFlatTabCtrl (NULL), m_pAllAxesView (NULL), m_pCmdBar (NULL), m_pMachine (NULL), m_nIndex (0), m_ePswLevel (ePswUser)
{
    m_pThis=this;
    for (int i=0; i<NMAXAXIS; i++)
        m_pSingleAxesView[i]=NULL;
}

CAdamoMDAxesView::~CAdamoMDAxesView()
{
}

BEGIN_MESSAGE_MAP(CAdamoMDAxesView, CView)
	//{{AFX_MSG_MAP(CAdamoMDAxesView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxesView drawing

void CAdamoMDAxesView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

void CAdamoMDAxesView::CreateTab ()
{
    CAdamoAxis* pAx;
    POSITION pos;
    RECT rc;
    int n;

    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTPTabControl();
	if (!m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		rc, this, IDC_FLAT_TABCTRL))  {
		TRACE0( "Failed to create flattab control\n" );
		return;
	}
    m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
    m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorVisualStudio2012);
    m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
    m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (TRUE);
    m_pFlatTabCtrl->GetPaintManager()->m_clientFrame =xtpTabFrameNone;
	m_pFlatTabCtrl->GetPaintManager ()->GetColorSet ()->m_clrHeaderFace = RGB (180,180,180);
	m_pFlatTabCtrl->GetPaintManager ()->m_bFillBackground = TRUE;
    pos=m_listAxes.GetHeadPosition();
    pAx=(CAdamoAxis*) m_listAxes.GetNext (pos);
    m_pCmdBar=new CAdamoMDCommandBar;
    m_pCmdBar->SetMachine (GETWS()->GetActiveProject ()->GetMachine ());
    if (!m_pCmdBar->Create (IDD_DLGCOMMANDBAR, this))   {
		TRACE0( "Failed to create command bar\n" );
		return;
	}
    m_pCmdBar->SetAx (pAx, 0);
    CreateViews ();
    n=m_listAxes.GetCount ()-1;
    for (pos=m_listAxes.GetTailPosition(); pos;)   {
        pAx=(CAdamoAxis*) m_listAxes.GetPrev (pos);
	    m_pFlatTabCtrl->InsertItem (0, pAx->GetName (), m_pSingleAxesView[n]->GetSafeHwnd ());
        n--;
    }
	m_pFlatTabCtrl->InsertItem(0, LOADSTRING (IDS_ADAMOMDAXESVIEW_1), m_pAllAxesView->GetSafeHwnd());
    m_pFlatTabCtrl->Invalidate ();
}
 
/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxesView diagnostics

#ifdef _DEBUG
void CAdamoMDAxesView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoMDAxesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxesView message handlers

void CAdamoMDAxesView::CreateViews()
{
   	CRect rectDummy(0, 0, 0, 0);
    int n=0;

    m_pAllAxesView=new CAdamoMDAxStatus;
    m_pAllAxesView->SettaListaAssi (&m_listAxes);
    m_pAllAxesView->SettaCommandBar (m_pCmdBar);
    m_pAllAxesView->SetMachine (GETWS()->GetActiveProject ()->GetMachine ());
    if (!m_pAllAxesView->Create(IDD_DLG_AXSTATUS, m_pFlatTabCtrl))
		TRACE0( "Failed to create CAllAxesView.\n" );
    POSITION pos=m_listAxes.GetHeadPosition ();
    while (n<m_listAxes.GetCount ())   {
        CAdamoAxis* pAx=(CAdamoAxis*)m_listAxes.GetNext (pos);
        m_pSingleAxesView[n]=new CAdamoMDAxSettings;
        m_pSingleAxesView[n]->SetMachine (GETWS()->GetActiveProject ()->GetMachine ());
        m_pSingleAxesView[n]->SettaCommandBar (m_pCmdBar);
        m_pSingleAxesView[n]->SetAx (pAx);
        if (!m_pSingleAxesView[n]->Create (IDD_DLGAXSETTINGS, m_pFlatTabCtrl))
            TRACE0( "Failed to create CSingleAxesView.\n" );
        n++;
    }
}

void CAdamoMDAxesView::OnDestroy() 
{
    int n=0;
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    SAFE_DELETE (m_pFlatTabCtrl)
    SAFE_DELETE (m_pAllAxesView);
    while (n<m_listAxes.GetCount ())   {
        SAFE_DELETE (m_pSingleAxesView[n]);
        n++;
    }
    m_pCmdBar->DestroyWindow ();
    SAFE_DELETE (m_pCmdBar);
    DeleteListAxes ();
}

void CAdamoMDAxesView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_pFlatTabCtrl->MoveWindow (0, 0, cx, cy-150);
    m_pCmdBar->MoveWindow (0, cy-150, cx, cy);
}

int CAdamoMDAxesView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    /* creiamo la visualizzazione degli assi */
    m_pMachine=GETWS()->GetActiveProject ()->GetMachine ();
    CreateAxesList ();
    if (m_listAxes.GetCount ())
	    CreateTab ();
	return 0;
}

void CAdamoMDAxesView::CreateAxesList ()
{
    CProject* pPrj=GETWS()->GetActiveProject ();
	if (pPrj)   {
		CAdamoDatiFile *pFile=new CAdamoDatiFile;
		bool bIsPhysical=pPrj->GetMachineType ()==enPhysical;
		if (pFile->Open (pPrj->GetParamsPathName (), TRUE))   {
			pFile->ReadData ();
			Iterator ((CAdamoGroup*)pPrj->GetMachineModule(), pFile, bIsPhysical);
			pFile->Close ();
		}
		delete pFile;
	}
}

void CAdamoMDAxesView::Iterator(CAdamoGroup *pGrp, CAdamoDatiFile *pFile, bool bIsPhysical)
{
    pGrp->Init();
    pGrp->DevInit();
    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase *)pGrp->DevGetNext();
		if (pDev->GetType ()== enAsse)   {
			char szBuffer[MAXHWLENGTH];
			bool bIsDeviceConnected=!bIsPhysical || (bIsPhysical && !pFile->GetDeviceHW (pDev->GetDeviceID (), enAsse, szBuffer)) ||
                ((CAdamoAxis *)pDev)->GetAxisType () == enAsseVirtuale;
			if (((CAdamoAxis *)pDev)->GetDisplay () && m_ePswLevel <= ((CAdamoAxis *)pDev)->GetRAcces () && bIsDeviceConnected)
            InsertAx ((CAdamoAxis *)pDev);
    }
    }
    while(pGrp->More())   {
        CObject *obj = (CObject*)pGrp->GetNext();
        Iterator((CAdamoGroup*)obj, pFile, bIsPhysical);
    }
}

void CAdamoMDAxesView::InsertAx (CAdamoAxis *pDev)
{
    for (POSITION pos2, pos=m_listAxes.GetHeadPosition(); pos2=pos;)   {
        CAdamoAxis* pListDev=(CAdamoAxis*) m_listAxes.GetNext (pos);
        if (pDev->GetName().Compare (pListDev->GetName ())<0)   {
            m_listAxes.InsertBefore (pos2, pDev);
            return;
        }
    }
    m_listAxes.AddTail (pDev);
}

void CAdamoMDAxesView::DeleteListAxes ()
{
    m_listAxes.RemoveAll ();
}

void CAdamoMDAxesView::OnAdamoTimer ()
{
    if (m_nIndex==0)
        m_pAllAxesView->OnAdamoTimer ();
    else
        m_pSingleAxesView[m_nIndex-1]->OnAdamoTimer ();
}

void CAdamoMDAxesView::OnViewSettings (stMDSettings* MDS)
{
    int n=0, nCurFolder=-1;
    bool bNotFirstTime=MDS->nAxStatus!=0;
    m_pCmdBar->OnViewSettings (MDS);
    m_pAllAxesView->OnViewSettings (MDS);
    for (POSITION pos=m_listAxes.GetHeadPosition(); pos;)   {
        CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext (pos);
        if (pAx->GetDeviceID ()==MDS->nFolderIDAxSettings&&bNotFirstTime)
            nCurFolder=n;    
        m_pSingleAxesView[n++]->OnViewSettings (MDS);
    }
    if (MDS->nFolderIDAxSettings==-1||nCurFolder==-1)
	    m_pFlatTabCtrl->SetCurSel(0);
    else    
	    m_pFlatTabCtrl->SetCurSel(nCurFolder);
}

void CAdamoMDAxesView::GetViewSettings (stMDSettings* MDS)
{
    int n=0, nCurAxID=-1, nIndex;

    m_pCmdBar->GetViewSettings (MDS);
    m_pAllAxesView->GetViewSettings (MDS);
    nIndex=m_pFlatTabCtrl->GetCurSel ();
    for (POSITION pos=m_listAxes.GetHeadPosition(); pos;)   {
        CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext (pos);
        if ((n+1)==nIndex)
            nCurAxID=pAx->GetDeviceID();
        m_pSingleAxesView[n++]->GetViewSettings (MDS);
    }
    MDS->nFolderIDAxSettings=nCurAxID;
}

BOOL CAdamoMDAxesView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ( IDC_FLAT_TABCTRL == (UINT)wParam)   {
		NMHDR* pNMHDR = (NMHDR*)lParam;
		switch(pNMHDR->code)   {
		    case TCN_SELCHANGING:
			    break;
		    case TCN_SELCHANGE:
                m_nIndex=m_pFlatTabCtrl->GetCurSel();
                if (m_nIndex==0)   {
                    m_pAllAxesView->UpdateAxesData ();
                    Invalidate ();
                }
                else
					m_pCmdBar->SetAx (m_pSingleAxesView[m_nIndex-1]->GetAx(), m_pSingleAxesView[m_nIndex-1]->GetMode() ? 1 : 2);
 			    break;
		}
    }
    return CView::OnNotify(wParam, lParam, pResult);
}

void CAdamoMDAxesView::OnClose ()
{
}

void CAdamoMDAxesView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    int n=0;

    /* selezioniamo il primo tab */
    if (m_pFlatTabCtrl->GetItemCount ())
        m_pFlatTabCtrl->SetCurSel (0);
    /* cancelliamo i tab gia' esistenti */
    while (n<m_listAxes.GetCount ())   {
        SAFE_DELETE (m_pSingleAxesView[n]);
        m_pFlatTabCtrl->DeleteItem (1);
        n++;
    }
    DeleteListAxes ();
    /* ora assegnamo la nuova password */
    m_ePswLevel=newPsw;
    /* ricreiamo la lista degli assi in base alla nuova password */
    CreateAxesList ();
    /* creiam ora i nuovi tab di visualizzazione */
    if (m_listAxes.GetCount ())   {
	    UpdateTab ();
        m_pAllAxesView->OnChangePsw (oldPsw, newPsw);
        for (n=0; n<m_listAxes.GetCount (); n++)   {
            m_pSingleAxesView[n]->OnChangePsw (oldPsw, newPsw);
        }
    }
    m_pFlatTabCtrl->Invalidate ();
}

void CAdamoMDAxesView::UpdateTab ()
{
    int n=0;

    POSITION pos=m_listAxes.GetHeadPosition();
    CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext (pos);
    m_pCmdBar->SetAx (pAx, 0);
    pos=m_listAxes.GetHeadPosition ();
    while (n<m_listAxes.GetCount ())   {
        CAdamoAxis* pAx=(CAdamoAxis*)m_listAxes.GetNext (pos);
        m_pSingleAxesView[n]=new CAdamoMDAxSettings;
        m_pSingleAxesView[n]->SetMachine (GETWS()->GetActiveProject ()->GetMachine ());
        m_pSingleAxesView[n]->SettaCommandBar (m_pCmdBar);
        m_pSingleAxesView[n]->SetAx (pAx);
        if (!m_pSingleAxesView[n]->Create (IDD_DLGAXSETTINGS, m_pFlatTabCtrl))
            TRACE0( "Failed to create CSingleAxesView.\n" );
        n++;
    }
    n=m_listAxes.GetCount ()-1;
    for (pos=m_listAxes.GetTailPosition(); pos;)   {
        pAx=(CAdamoAxis*) m_listAxes.GetPrev (pos);
	    m_pFlatTabCtrl->InsertItem (1, pAx->GetName (), m_pSingleAxesView[n]->GetSafeHwnd ());
        n--;
    }
    m_pAllAxesView->SettaListaAssi (&m_listAxes);
}
