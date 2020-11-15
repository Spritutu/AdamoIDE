// AdamoRSView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSView.h"
#include "BuildResource.h"
#include "PreviewResource.h"
#include "ImageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT m_nRefreshMetrics;

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSView

IMPLEMENT_DYNCREATE(CAdamoRSView, CView)

CAdamoRSView::CAdamoRSView() : m_theme (themeNone)
{
}

CAdamoRSView::~CAdamoRSView()
{
}


BEGIN_MESSAGE_MAP(CAdamoRSView, CView)
	//{{AFX_MSG_MAP(CAdamoRSView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE ()
	ON_NOTIFY (TCN_SELCHANGE, IDC_TABCONTROL, OnSelChange)
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSView drawing

void CAdamoRSView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSView diagnostics

#ifdef _DEBUG
void CAdamoRSView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoRSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/*
**
*/
void CAdamoRSView::Activate()
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
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    pTmpl->SetActiveView (this);
}

/*
** OnCloseRSView :
*/
void CAdamoRSView::OnCloseRSView () 
{
	CAdamoRSDoc* pDoc = (CAdamoRSDoc*)GetDocument();
    if (pDoc)   {
        pDoc->SaveXMLRecord ();
    }
}

/*
** AddRSResource :
*/
void CAdamoRSView::AddRSResource ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->AddRSResource ();
    }
}

/*
** AddRSFolder :
*/
void CAdamoRSView::AddRSFolder ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->AddRSFolder ();
    }
}

/*
** EditRSResource :
*/
void CAdamoRSView::EditRSResource ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->EditRSResource ();
    }
}

/*
** DeleteRSResource :
*/
void CAdamoRSView::DeleteRSResource ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->DeleteRSResource ();
    }
}

/*
** RenameRSResource :
*/
void CAdamoRSView::RenameRSResource (CString strPrev, CString strNew, eResources e)
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->RenameRSResource (strPrev, strNew, e);
    }
}

/*
** RenameRSBitmap :
*/
bool CAdamoRSView::RenameRSImage (CString strImage, CString strID)
{
    bool b=false;
	CAdamoRSDoc* pDoc = (CAdamoRSDoc*)GetDocument();
    if (pDoc)
        b=pDoc->RinominaImmagine (strImage, strID);
    return b;
}

/*
** UpdateDiagramEntitySize :
*/
void CAdamoRSView::UpdateDiagramEntitySize ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->UpdateDiagramEntitySize ();
    }
}

/*
** UpdateDiagramSize :
*/
void CAdamoRSView::UpdateDiagramSize ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->UpdateDiagramSize ();
    }
}

/*
** UpdateDiagramSize :
*/
void CAdamoRSView::UpdateDiagramValue ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->UpdateDiagramValue ();
    }
}

/*
** UpdateDiagramSelected :
*/
void CAdamoRSView::UpdateDiagramSelected ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
        m_wndTabControl.SetCurSel (0);
        pView->UpdateDiagramSelected ();
    }
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSView message handlers
int CAdamoRSView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	/* creiamo il tab */
	m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
    /* aggiungiamo le view */
    AddView (RUNTIME_CLASS (CBuildResource), LOADSTRING (IDS_ADAMORSVIEW_1), 0);
    AddView (RUNTIME_CLASS (CPreviewResource), LOADSTRING (IDS_ADAMORSVIEW_2), 1);
    AddView (RUNTIME_CLASS (CImageResource), LOADSTRING (IDS_ADAMORSVIEW_3), 2);
    CreateImageList ();
	SetTabStyle ();
    m_wndTabControl.SetCurSel (0);
	CAdamoRSDoc* pDoc = (CAdamoRSDoc*)GetDocument();
	return 0;
}

/*
** AddRSBitmap :
*/
void CAdamoRSView::AddRSBitmap()
{
    CString str;
    CImageResource* pView=(CImageResource*)GetView (2);
    if (pView->IsKindOf (RUNTIME_CLASS (CImageResource)))   {
        str=pView->AddRsBimap ();
        if (str!="")   {
            CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
            if (rsCat)
                rsCat->AddImageElement (str);
            m_wndTabControl.SetCurSel (2);
        }
    }
}

/*
** SelectRSBitmap :
*/
void CAdamoRSView::SelectRSBitmap (CString strID)
{
    CImageResource* pView=(CImageResource*)GetView (2);
    if (pView->IsKindOf (RUNTIME_CLASS (CImageResource)))   {
        pView->SelectRSBitmap (strID);
		pView->Invalidate ();
        m_wndTabControl.SetCurSel (2);
    }
}

/*
** DeleteRSBitmap :
*/
void CAdamoRSView::DeleteRSBitmap()
{
    CString str;
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    if (rsCat)   {
        CString strID=rsCat->GetSelectedImageID ();
        CImageResource* pView=(CImageResource*)GetView (2);
        if (pView->IsKindOf (RUNTIME_CLASS (CImageResource)))   {
            pView->DeleteRsBimap (strID);
            rsCat->DeleteImageItem (strID);
        }
    }
}

/*
** GetFirstRSImage :
*/
int CAdamoRSView::GetFirstRSImage (CString& str)
{
	CAdamoRSDoc* pDoc = (CAdamoRSDoc*)GetDocument();
    int nB = -1;
    if (pDoc)
        nB=pDoc->GetFirstRSImage (str);
    return nB;
}

/*
** GetNextRSImage :
*/
int CAdamoRSView::GetNextRSImage (CString& str)
{
	CAdamoRSDoc* pDoc = (CAdamoRSDoc*)GetDocument();
    int nB=-1;
    if (pDoc)
        nB=pDoc->GetNextRSImage (str);
    return nB;
}

/*
** CloseTabMode :
*/
void CAdamoRSView::CloseTabMode ()
{
    CBuildResource* pView=(CBuildResource*)GetView (0);
    if (pView)   {
		m_wndTabControl.SetCurSel (0);
        pView->CloseTabMode ();
    }
}

/*
** CreateImageList :
*/
void CAdamoRSView::CreateImageList() 
{
    m_bmpDlgs=new CBitmap;
    m_bmpPreview=new CBitmap;
    m_bmpImages=new CBitmap;
    m_tbiml=new CXTPImageList;
    m_bmpDlgs->LoadBitmap (IDB_CAPDIALOGS);
    m_bmpPreview->LoadBitmap (IDB_CAPANTEPRIMA);
    m_bmpImages->LoadBitmap (IDB_CAPIMMAGINI);
    m_tbiml->Create (16, 16, ILC_MASK | ILC_COLOR24, 0, 1);
    m_tbiml->Add(m_bmpDlgs, RGB(0, 128,128));
    m_tbiml->Add(m_bmpPreview, RGB(0, 128,128));
    m_tbiml->Add(m_bmpImages, RGB(0, 128,128));
}

/*
** OnDestroy :
*/
void CAdamoRSView::OnDestroy()
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    _delete (m_bmpDlgs);
    _delete (m_bmpPreview);
    _delete (m_bmpImages);
    _delete (m_tbiml);
}

/*
** OnAdamoTimer :
*/
void CAdamoRSView::OnAdamoTimer ()
{
    CView* pPreView=(CView*)GetActiveView ();
    if (pPreView->IsKindOf (RUNTIME_CLASS(CPreviewResource)))
		((CPreviewResource *)pPreView)->OnAdamoTimer ();
}

/*
** SetTabStyle :
*/
void CAdamoRSView::SetTabStyle () 
{
	m_wndTabControl.GetImageManager()->SetIcons (*m_tbiml, NULL, 0, CSize(16, 16), xtpImageNormal);
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
** OnSize :
*/
void CAdamoRSView::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_wndTabControl.GetSafeHwnd())
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
}

/*
** AddView :
*/
BOOL CAdamoRSView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
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
CView* CAdamoRSView::GetView (int nItem)
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nItem)->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** GetActiveView :
*/
CView* CAdamoRSView::GetActiveView ()
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** OnRefreshMetrics :
*/
LRESULT CAdamoRSView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_theme != wParam)   {
		SetTabStyle ();
		m_wndTabControl.Invalidate ();
	}
	m_theme = (AppTheme) wParam;
	return 0;
}

/*
** OnSelChange :
*/
void CAdamoRSView::OnSelChange (NMHDR* pNMHDR, LRESULT* pResult)
{
    CView* pPreView=(CView*)GetActiveView ();
    if (pPreView->IsKindOf (RUNTIME_CLASS(CPreviewResource)))   {
        CBuildResource* pBuildView=(CBuildResource*)GetView (0);
        if (pBuildView)   {
			/* fare scattare il preview */
            CString str;
            pBuildView->GetXMLString (str);
            ((CPreviewResource*)pPreView)->LoadPreviewPage (((CAdamoRSDoc*)GetDocument())->GetCurResource (), str);
        }
    }
}
