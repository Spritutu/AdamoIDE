// AdamoDGView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "adamotemplates.h"
#include "AdamoDGView.h"
#include "AdamoDGComm.h"
#include "AdamoDGHardware.h"
#include "AdamoDGCards.h"
#include "AdamoDGSSCNET.h"
#include "AdamoDGSSCNETView.h"
#include "AdamoDGEthercat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT m_nRefreshMetrics;

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGView

IMPLEMENT_DYNCREATE(CAdamoDGView, CView)

CAdamoDGView::CAdamoDGView() : m_bmpHardware (NULL), m_bmpComm (NULL), m_bmpSSCNET (NULL), m_bmpEthercat (NULL), m_bExistSSCNET (false), m_bExistDGComm (false), m_bExistEthercat (false), m_pMachine (NULL)
{
}

CAdamoDGView::~CAdamoDGView()
{
}


BEGIN_MESSAGE_MAP(CAdamoDGView, CView)
	//{{AFX_MSG_MAP(CAdamoDGView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE ()
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
	ON_NOTIFY (TCN_SELCHANGE, IDC_TABDGCONTROL, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGView drawing

void CAdamoDGView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGView diagnostics

#ifdef _DEBUG
void CAdamoDGView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDGView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGView message handlers
void CAdamoDGView::Activate()
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
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enDiagnosticTemplate);
    pTmpl->SetActiveView (this);
    if (m_bExistSSCNET)   {
        ((CMainFrame *)AfxGetMainWnd ())->LoadSSCNETToolbar ();
        CAdamoDGSSCNETView *pSSCNETView=(CAdamoDGSSCNETView *)GetView (RUNTIME_CLASS (CAdamoDGSSCNETView));
        if (pSSCNETView)
            pSSCNETView->InizializzaInformazioni ();
    }
    if (m_bExistEthercat)   {
        ((CMainFrame *)AfxGetMainWnd ())->LoadEthercatToolbar ();
	}
    if (m_bExistDGComm)  
        ((CMainFrame *)AfxGetMainWnd ())->LoadCommToolbar ();
	/* gestione delle password */
    ePswLevel PswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	OnChangePsw (m_ePswLevel, PswLevel);
}

/*
** OnCloseRSView :
*/
void CAdamoDGView::OnCloseDGView () 
{
    CAdamoDGComm* pDGComm=(CAdamoDGComm *)GetView (RUNTIME_CLASS (CAdamoDGComm));
    if (pDGComm)
        pDGComm->OnCloseDGView ();
}

/*
** OnInitialUpdate :
*/
void CAdamoDGView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
    SetMachine ();
    /* aggiungiamo i view della diagnostica */
    AddView (RUNTIME_CLASS (CAdamoDGHardware), LOADSTRING (IDS_ADAMODGVIEW_3), 0);
    if (((CMainFrame *)AfxGetMainWnd ())->GetPswLevel () == ePswKinema)   {
        AddView (RUNTIME_CLASS (CAdamoDGComm), LOADSTRING (IDS_ADAMODGVIEW_4), 1);
        m_bExistDGComm=true;
    }
    bool b=WaitConnection (m_pMachine);
    if (b&&ExistSSCNET ())   {
        AddView (RUNTIME_CLASS (CAdamoDGSSCNETView), LOADSTRING (IDS_ADAMODGVIEW_5), 2);
        m_bExistSSCNET=true;
    }
    if (b&&ExistEthercat ())   {
        AddView (RUNTIME_CLASS (CAdamoDGEthercat), LOADSTRING (IDS_ADAMODGVIEW_6), 3);
        m_bExistEthercat=true;
    }
    m_wndTabControl.SetCurSel (0);
}

/*
** CreateImageList :
*/
void CAdamoDGView::CreateImageList() 
{
    m_bmpHardware=new CBitmap;
    m_bmpComm=new CBitmap;
    m_bmpSSCNET=new CBitmap;
	m_bmpEthercat=new CBitmap;
    m_bmpHardware->LoadBitmap (IDB_CAPDGHARDWARE);
    m_bmpComm->LoadBitmap (IDB_CAPDGCOMM);
    m_bmpSSCNET->LoadBitmap (IDB_CAPDGSSCNET);
    m_bmpEthercat->LoadBitmap (IDB_CAPDGETHERCAT);
    m_tbiml.Create (16, 16, ILC_COLORDDB|ILC_MASK, 0, 1);
    m_tbiml.Add(m_bmpHardware, RGB(0, 128,128));
	m_tbiml.Add(m_bmpComm, RGB(0, 128,128));
	m_tbiml.Add(m_bmpSSCNET, RGB(0, 128,128));
	m_tbiml.Add(m_bmpEthercat, RGB(0, 128,128));
}

/*
** OnDestroy :
*/
void CAdamoDGView::OnDestroy () 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    _delete (m_bmpHardware);
    _delete (m_bmpComm);
    _delete (m_bmpSSCNET);
	_delete (m_bmpEthercat);
}

/*
** OnAdamoTimer :
*/
void CAdamoDGView::OnAdamoTimer ()
{
    CView* pView=(CView*) GetActiveView ();
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGHardware)))   {
        ((CAdamoDGHardware*)pView)->OnAdamoTimer ();
    }
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGSSCNETView)))   {
        ((CAdamoDGSSCNETView*)pView)->OnAdamoTimer ();
    }
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGEthercat)))   {
        ((CAdamoDGEthercat*)pView)->OnAdamoTimer ();
    }
}

/*
** IsDGDeviceOpened :
*/
bool CAdamoDGView::IsDGDeviceOpened ()
{
    bool b=false;
    CView* pView=(CView*) GetActiveView ();
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGHardware)))
        b=((CAdamoDGHardware *) pView)->GetCurTabSel ()==1;            
    return b;
}

/*
** IsEcDeviceOpened :
*/
bool CAdamoDGView::IsEcDeviceOpened ()
{
    bool b=false;
    CView* pView=(CView*) GetActiveView ();
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGEthercat)))
        b=true;            
    return b;
}

/*
** SetDGMode :
*/
void CAdamoDGView::SetDGMode (int nMode)
{
    CView* pView=(CView*) GetActiveView ();
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGHardware)))
        ((CAdamoDGHardware *) pView)->SetDGMode (nMode);
}

/*
** SetEcMode :
*/
void CAdamoDGView::SetEcMode (int nMode)
{
    CView* pView=(CView*) GetActiveView ();
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGEthercat)))
        ((CAdamoDGEthercat *) pView)->SetEcMode (nMode);
}

/*
** ExistSSCNET :
*/
bool CAdamoDGView::ExistSSCNET ()
{
    CCmdGetCardInfo CI;
    int i, nCount;
    bool b=false;

    if (m_pMachine)   {
        if (m_pMachine->GetNumCards (&nCount) == S_FALSE)
            nCount = 0;
		bitres (nCount, 31);
        for (i=0; i<nCount; i++)   {
            if (m_pMachine->GetCardInfo (i, &CI) == S_OK)   {
                if (CI.eTipo==enOmikron)   {
                    b=true; break;
                }
            }
        }
    }
    return b;
}

/*
** ExistEthercat :
*/
bool CAdamoDGView::ExistEthercat ()
{
	bool b=false;
	CProject* pPrj=GETWS()->GetActiveProject();
	if (pPrj)
		b=pPrj->GetSlaveArray ().GetSize ()>0;
	return b;
}

/*
** SetMachine :
*/
void CAdamoDGView::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

int CAdamoDGView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	/* creiamo il tab control */
	m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, IDC_TABDGCONTROL);
	/* creiamo le iconcine */
    CreateImageList ();
	/* settiamo il corretto look */
	OnRefreshMetrics (((CIdeApp *) AfxGetApp ())->GetTheme (), 0);
	/* settiamo il look giusto */
	GestionePswCanOpen ();
    return 0;
}

/*
** OnSelChange :
*/
void CAdamoDGView::OnSelChange(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    CView* pView=(CView*)GetActiveView ();
    if (pView->IsKindOf (RUNTIME_CLASS(CAdamoDGSSCNETView)))
        ((CAdamoDGSSCNETView *)pView)->InizializzaInformazioni ();
}

/*
** OnChangePsw :
*/
void CAdamoDGView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	CView *pView;
	int nIndex;

	m_ePswLevel = newPsw;
    if (newPsw==ePswKinema)   {
        if (!GetView (RUNTIME_CLASS (CAdamoDGComm)))   {
            AddView (RUNTIME_CLASS (CAdamoDGComm), LOADSTRING (IDS_ADAMODGVIEW_4), 1);
            ((CMainFrame *)AfxGetMainWnd ())->LoadCommToolbar ();
        }
    }
    else   {
        DeleteView (RUNTIME_CLASS (CAdamoDGComm));
        ((CMainFrame *)AfxGetMainWnd ())->RemoveCommToolbar ();
    }
	nIndex=0;
	while (pView=(CView *)GetView (nIndex))   {
		if (pView->IsKindOf (RUNTIME_CLASS (CAdamoDGSSCNETView)))
			((CAdamoDGSSCNETView *)pView)->OnChangePsw (oldPsw, newPsw);
		if (pView->IsKindOf (RUNTIME_CLASS (CAdamoDGEthercat)))
			((CAdamoDGEthercat *)pView)->OnChangePsw (oldPsw, newPsw);
		if (pView->IsKindOf (RUNTIME_CLASS (CAdamoDGHardware)))
			((CAdamoDGHardware *)pView)->OnChangePsw (oldPsw, newPsw);
		nIndex++;
	}
	GestionePswCanOpen ();
	if (m_bExistEthercat)
		GestionePswEthercat ();
}

/*
** GestionePswCanOpen :
*/
void CAdamoDGView::GestionePswCanOpen ()
{
	CXTPDockingPaneManager*  pPaneManager = ((CMainFrame *) AfxGetMainWnd ())->GetPaneManager();
	if (pPaneManager)   {
	    if (m_ePswLevel < ePswService)
			pPaneManager->ShowPane (ID_VIEW_DG_SDO);
		else
			pPaneManager->ClosePane (ID_VIEW_DG_SDO);
	}
}

/*
** GestionePswEthercat :
*/
void CAdamoDGView::GestionePswEthercat ()
{
	CXTPDockingPaneManager*  pPaneManager = ((CMainFrame *) AfxGetMainWnd ())->GetPaneManager();
	if (pPaneManager)   {
	    if (m_ePswLevel < ePswService)
			pPaneManager->ShowPane (ID_VIEW_EC_MAILBOX);
		else
			pPaneManager->ClosePane (ID_VIEW_EC_MAILBOX);
	}
}


/*
** AddView :
*/
BOOL CAdamoDGView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
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
CView* CAdamoDGView::GetView (int nItem)
{
	CView* pView = NULL;
	if (nItem >= 0 && nItem < m_wndTabControl.GetItemCount ())
		pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nItem)->GetHandle()));
	return pView;
}

/*
** GetView :
*/
CView* CAdamoDGView::GetView (CRuntimeClass* pClass)
{
	CView* pView = NULL;
	for (int i = 0; i<m_wndTabControl.GetItemCount (); i++)   {
		pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(i)->GetHandle()));
		if (pView->IsKindOf (pClass))
			return pView;
	}
	return pView;
}

/*
** DeleteView :
*/
void CAdamoDGView::DeleteView (CRuntimeClass* pClass)
{
	CView* pView;
	for (int i = 0; i<m_wndTabControl.GetItemCount (); i++)   {
		pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(i)->GetHandle()));
		if (pView->IsKindOf (pClass))   {
			m_wndTabControl.DeleteItem (i);
			break;
		}
	}
}

/*
** GetActiveView :
*/
CView* CAdamoDGView::GetActiveView ()
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** SetTabStyle :
*/
void CAdamoDGView::SetTabStyle () 
{
	m_wndTabControl.GetImageManager()->SetIcons (m_tbiml, NULL, 0, CSize(16, 16), xtpImageNormal);
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
LRESULT CAdamoDGView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_theme != wParam)   {
		SetTabStyle ();
		m_wndTabControl.Invalidate ();
	}
	m_theme = (AppTheme) wParam;
	return 0;
}

/*
** OnSize :
*/
void CAdamoDGView::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_wndTabControl.GetSafeHwnd())
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
}
