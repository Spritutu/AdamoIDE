// AdamoMDTabbedView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoMDView.h"
#include "AdamoMDSplittedView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdamoMDView* CAdamoMDView::m_pThis=NULL;
extern UINT m_nRefreshMetrics;

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDView

IMPLEMENT_DYNCREATE(CAdamoMDView, CView)

CAdamoMDView::CAdamoMDView() : m_nAxes (0), m_ePswLevel (ePswUser), m_theme (themeNone)
{
    m_pThis=this;
}

CAdamoMDView::~CAdamoMDView()
{
}


BEGIN_MESSAGE_MAP(CAdamoMDView, CView)
	//{{AFX_MSG_MAP(CAdamoMDView)
	ON_WM_CREATE()
	ON_WM_SIZE ()
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDView diagnostics

#ifdef _DEBUG
void CAdamoMDView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoMDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDView message handlers

int CAdamoMDView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	/* creiamo il tab control */
	m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, IDC_TABMDCONTROL);
    /* carichiamo i view di assi e io */
    CreateImageList ();
    if (ExistAxis ())
        AddView (RUNTIME_CLASS (CAdamoMDAxesView), LOADSTRING (IDS_ADAMOMDVIEW_1), 0);
    AddView (RUNTIME_CLASS (CAdamoMDSplittedView), LOADSTRING (IDS_ADAMOMDVIEW_2), 1);
	OnRefreshMetrics (((CIdeApp *) AfxGetApp ())->GetTheme (), 0);
	m_wndTabControl.SetCurSel (0);
	GestionePswOscilloscopio ();
	return 0;
}

bool CAdamoMDView::ExistAxis ()
{
    CProject* pPrj=GETWS()->GetActiveProject ();
	if (pPrj)   {
		CAdamoDatiFile *pFile=new CAdamoDatiFile;
		bool bIsPhysical=pPrj->GetMachineType ()==enPhysical;
		if (pFile->Open (pPrj->GetParamsPathName (), TRUE))   {
			pFile->ReadData ();
			m_nAxes=0;
			Iterator ((CAdamoGroup*)pPrj->GetMachineModule(), pFile, bIsPhysical);
			pFile->Close ();
		}
		delete pFile;
	}
    return m_nAxes!=0;
}

void CAdamoMDView::Iterator(CAdamoGroup *pGrp, CAdamoDatiFile *pFile, bool bIsPhysical)
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
            m_nAxes++;
    }
    }
    while(pGrp->More())   {
        CObject *obj = (CObject*)pGrp->GetNext();
        Iterator((CAdamoGroup*)obj, pFile, bIsPhysical);
    }
}

void CAdamoMDView::Activate()
{
	CFrameWnd* pFrame = GetParentFrame();

	if (pFrame != NULL)
		pFrame->ActivateFrame();
	else
		TRACE0("Error: Can not find a frame for document to activate.\n");

	CFrameWnd* pAppFrame;
	if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
	{
		ASSERT_KINDOF(CFrameWnd, pAppFrame);
		pAppFrame->ActivateFrame();
	}
    /* comunichiamo al template che  siamo stati attivati */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enManDiagTemplate);
    pTmpl->SetActiveView (this);
    /* verifichiamo se downloadare i parametri */
    ((CMainFrame*)AfxGetMainWnd ())->CheckParameters ();
    /* rinfreschiamo ora i dati */
    RefreshData ();
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	GestionePswOscilloscopio ();
}

/*
** RefreshData : la funzione rinfresca i dati del dialog delle variabili globali.
*/
void CAdamoMDView::RefreshData ()
{
    /* rinfreschiamo la view delle variabili globali */
	CAdamoMDSplittedView* pMDSplittedView = GetIOView ();
	if (pMDSplittedView)   {
		CAdamoMDIOViewDlg* pIOViewDlg = pMDSplittedView->GetIOViewDlg ();
		if (pIOViewDlg)
			pIOViewDlg->UpdateData ();
	}
}

/*
** OnClose :
*/
void CAdamoMDView::OnClose ()
{
    CAdamoMDAxesView* pAxesView=(CAdamoMDAxesView*)GetAxesView ();
    if (pAxesView)
        pAxesView->OnClose ();
}

/*
** GetAxesView :
*/
CAdamoMDAxesView* CAdamoMDView::GetAxesView () 
{
    CAdamoMDAxesView* pAxesView=NULL;
    if (m_nAxes) 
        pAxesView=(CAdamoMDAxesView*) GetView (0);
    return pAxesView;
}

/*
** GetIOView :
*/
CAdamoMDSplittedView* CAdamoMDView::GetIOView ()
{
    int nIndex = m_nAxes ? 1 : 0;
    return (CAdamoMDSplittedView*) GetView (nIndex);
}

/*
** CreateImageList :
*/
void CAdamoMDView::CreateImageList ()
{
    CBitmap bm;

    m_il.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_MDAXESVIEW);
    m_il.Add(&bm, RGB(0, 128, 128));
}

/*
** OnChangePsw :
*/
void CAdamoMDView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	/* cambio password per gli assi a seconda dei livelli di accesso stabiliti in configurazione */
    m_ePswLevel=newPsw;
	CView* pView = GetView (0);
	if (ExistAxis () && !pView->IsKindOf (RUNTIME_CLASS (CAdamoMDAxesView)))   {
        AddView (RUNTIME_CLASS (CAdamoMDAxesView), LOADSTRING (IDS_ADAMOMDVIEW_1), 0);
		m_wndTabControl.SetCurSel (0);
	}
    if (!ExistAxis () && pView->IsKindOf (RUNTIME_CLASS (CAdamoMDAxesView)))
        m_wndTabControl.DeleteItem (0);
    CAdamoMDAxesView* pAxesView=GetAxesView ();
    if (pAxesView)
        pAxesView->OnChangePsw (oldPsw, newPsw);
    CAdamoMDSplittedView* pIOView=GetIOView ();
    if (pIOView)
        pIOView->OnChangePsw (oldPsw, newPsw);
	/* Oscilloscopio */
	GestionePswOscilloscopio ();
}

/*
** GestionePswOscilloscopio :
*/
void CAdamoMDView::GestionePswOscilloscopio ()
{	
	CXTPDockingPaneManager*  pPaneManager = ((CMainFrame *) AfxGetMainWnd ())->GetPaneManager();
	if (pPaneManager)   {
	    if (m_ePswLevel < ePswService)
			pPaneManager->ShowPane (ID_VIEW_OSCILLOSCOPE);
		else
			pPaneManager->ClosePane (ID_VIEW_OSCILLOSCOPE);
	}
}

/*
** AddView :
*/
BOOL CAdamoMDView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc	 = GetDocument();
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = GetDocument()->GetDocTemplate();

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;

	int nTab = m_wndTabControl.GetItemCount();

	// Create with the right size (wrong position)
	CRect rect(0, 0, 0, 0);
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab),
					  &contextT))
	{
		TRACE0("Warning: couldn't create client tab for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);

	pWnd->SetOwner(this);

	return TRUE;
}

/*
** GetView :
*/
CView* CAdamoMDView::GetView (int nItem)
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nItem)->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** GetActiveView :
*/
CView* CAdamoMDView::GetActiveView ()
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** SetTabStyle :
*/
void CAdamoMDView::SetTabStyle () 
{
	m_wndTabControl.GetImageManager()->SetIcons (m_il, NULL, 0, CSize(16, 16), xtpImageNormal);
	switch (((CIdeApp *) AfxGetApp ())->GetTheme ())   {
		case themeVS2010 :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2010);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2010);
			break;
		case themeVS2012Light :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2012);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2012);
			break;
		case themeVS2012Dark :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2012Dark);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2012);
			break;
		case themeVS2015Light :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2015);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2015);
			break;
		case themeVS2015Dark :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2015);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2015);
			break;
		case themeVS2015Blue :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2015);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2015);
			break;
	}
}

/*
** OnRefreshMetrics :
*/
LRESULT CAdamoMDView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_theme != wParam)   {
		SetTabStyle ();
		m_wndTabControl.Invalidate ();
	}
	m_theme = (AppTheme) wParam;
	return 0;
}

/*
** OnDraw :
*/
void CAdamoMDView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/*
** OnSize :
*/
void CAdamoMDView::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_wndTabControl.GetSafeHwnd())
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
}
