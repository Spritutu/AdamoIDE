// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "OutputView.h"
#include "OutputWnd.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputView

IMPLEMENT_DYNCREATE(COutputView, CView)

COutputView::COutputView():m_pFlatTabCtrl(0)
{
	m_FTS_XT_BOTTOM = TRUE;
	m_FTS_XT_HASARROWS = TRUE;
	m_FTS_XT_HASHOMEEND = FALSE;
	m_FTS_XT_HSCROLL = TRUE;
	m_dwTabStyle	= -1;
}

COutputView::~COutputView()
{
  	SAFE_DELETE( m_pFlatTabCtrl );
}


BEGIN_MESSAGE_MAP(COutputView, CView)
	//{{AFX_MSG_MAP(COutputView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputView drawing

void COutputView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COutputView diagnostics

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CView::AssertValid();
}

void COutputView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COutputView message handlers

void COutputView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
    UpdateActiveView();	
	
//	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL COutputView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN;

	return TRUE;
}

int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	UpdateTabsStyle();
		
	return 0;
}

void COutputView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (m_pFlatTabCtrl && ::IsWindow(m_pFlatTabCtrl->m_hWnd))
	{
		m_pFlatTabCtrl->SetWindowPos(NULL, 0,0,cx,cy, SWP_FRAMECHANGED);
	}	
}

BOOL COutputView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (ID_OUTPUT_FLAT_TAB == (UINT)wParam)
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;

		switch(pNMHDR->code)
		{
		case TCN_SELCHANGING:
			break;

		case TCN_SELCHANGE:
            UpdateActiveView();
			break;
		}
	}
	
	NMHDR *lpnmhdr = (LPNMHDR) lParam; 

    if (lpnmhdr->hwndFrom == m_sheetBuild.m_hWnd ||
                        lpnmhdr->hwndFrom == m_sheetDebug.m_hWnd)
	{
		*pResult = ((CMainFrame*)AfxGetMainWnd())->OnSci(this, (SCNotification*)lParam);
		return TRUE;
	}

    return CView::OnNotify(wParam, lParam, pResult);
}

void COutputView::UpdateTabsStyle()
{
	int dwNewStyle = 0;
	UpdateData();
	if (m_FTS_XT_HASHOMEEND) 
	{
		dwNewStyle |= FTS_XT_HASHOMEEND;
		m_FTS_XT_HASARROWS = TRUE;
		UpdateData(FALSE);
	}

	if (m_FTS_XT_BOTTOM)     dwNewStyle |= FTS_XT_BOTTOM;
	if (m_FTS_XT_HASARROWS)  dwNewStyle |= FTS_XT_HASARROWS;
	if (m_FTS_XT_HSCROLL)    dwNewStyle |= FTS_XT_HSCROLL;
	dwNewStyle |= FTS_XT_TOOLTIPS;
	if (dwNewStyle != m_dwTabStyle)
	{
		m_dwTabStyle = dwNewStyle;
		CreateTabs();
	}
}

void COutputView::CreateTabs()
{
	SetRedraw(false);

	int iDefaultSel = 0;
	if (m_pFlatTabCtrl)
	{
		m_pFlatTabCtrl->GetWindowRect(m_rcTabs);
		ScreenToClient(m_rcTabs);

		iDefaultSel = m_pFlatTabCtrl->GetCurSel();
		m_pFlatTabCtrl->DestroyWindow();
		SAFE_DELETE( m_pFlatTabCtrl );
	}
	
	m_pFlatTabCtrl = new CXTFlatTabCtrl;

    // Create  the flat tab control.
	if (!m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|m_dwTabStyle,
		m_rcTabs, this, ID_OUTPUT_FLAT_TAB))
	{
		TRACE0( "Failed to create flattab control\n" );
		return;
	}

	CreateViews();

	// Insert tabs into the flat tab control.
	m_pFlatTabCtrl->InsertItem(0, _T("Build"),&m_sheetBuild);
	m_pFlatTabCtrl->InsertItem(1, _T("Debug"),&m_sheetDebug);

	m_pFlatTabCtrl->SetCurSel(iDefaultSel);

	m_pFlatTabCtrl->SetWindowPos(&wndTop, 0, 0, 0, 0, 
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	SetRedraw(true);

	RedrawWindow(0,0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

int COutputView::CreateViews()
{
	CRect rectDummy(0, 0, 0, 0);

    if ( ::IsWindow( m_sheetBuild.m_hWnd ) )
	{
		m_sheetBuild.DestroyWindow();
	}
	if (!m_sheetBuild.Create(this, ID_OUTPUT_CTRL1,WS_CHILD|WS_TABSTOP))
	{
		TRACE0( "Failed to create sheet1.\n" );
		return -1;
	}
	
	m_sheetBuild.SetReadOnly(TRUE);

    // Create the sheet2 list box.
	if ( ::IsWindow( m_sheetDebug.m_hWnd ) )
	{
		m_sheetDebug.DestroyWindow();
	}
	if (!m_sheetDebug.Create(this, ID_OUTPUT_CTRL2,WS_CHILD|WS_TABSTOP))
	{
		TRACE0( "Failed to create sheet2.\n" );
		return -1;
	}

	m_sheetBuild.SetReadOnly(TRUE);
    m_sheetDebug.SetReadOnly(TRUE);

    return TRUE;
}

void COutputView::UpdateActiveView()
{
    // any tab selected?
    const int iSel = m_pFlatTabCtrl->GetCurSel();
    if (iSel >= 0)
    {
        // is the corresponding child window a view?
        CWnd * const pWndActive = m_pFlatTabCtrl->GetItemWindow(iSel);
        if (pWndActive->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CView)))
        {
            // yes - make it the active view
            GetParentFrame()->SetActiveView((CView *)pWndActive);
        }
    }
}

int COutputView::SetCurSel(int nItem)
{
    return m_pFlatTabCtrl->SetCurSel(nItem);
}

void COutputView::Clear(int nOutput)
{
    switch(nOutput){
    case COutputWnd::outputBuild:
	    m_sheetBuild.SetReadOnly(FALSE);
	    m_sheetBuild.ClearAll();
	    m_sheetBuild.SetReadOnly(TRUE);
    break;
    case COutputWnd::outputDebug:
	    m_sheetDebug.SetReadOnly(FALSE);
	    m_sheetDebug.ClearAll();
	    m_sheetDebug.SetReadOnly(TRUE);
    break;
    }
}

void COutputView::Write(int nOutput,CString strMsg)
{
    switch(nOutput){
    case COutputWnd::outputBuild:
        m_sheetBuild.SetReadOnly(FALSE);
        m_sheetBuild.AddText(strMsg);
        m_sheetBuild.SetReadOnly(TRUE);
        m_sheetBuild.GotoLastLine();
    break;
    case COutputWnd::outputDebug:
        m_sheetDebug.SetReadOnly(FALSE);
        m_sheetDebug.AddText(strMsg);
        m_sheetDebug.SetReadOnly(TRUE);
        m_sheetDebug.GotoLastLine();
    break;
    }
    UpdateWindow ();
}

CLuaEditor* COutputView::GetEditor(int nOutput)
{
    switch(nOutput){
    case COutputWnd::outputBuild:
        return &m_sheetBuild;
    case COutputWnd::outputDebug:
        return &m_sheetDebug;
    break;
    }

    return NULL;
}

CWnd* COutputView::GetItemWindow(int nIndex)
{
    return m_pFlatTabCtrl->GetItemWindow(nIndex);
}
