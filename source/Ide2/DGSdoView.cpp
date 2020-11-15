// DGSdoView.cpp : implementation file
//

#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DGSdoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COscView

IMPLEMENT_DYNCREATE(CDGSdoView, CView)

CDGSdoView::CDGSdoView() : m_pFlatTabCtrl (NULL), m_pSdoRead (NULL), m_pSdoWrite (NULL), m_nCurSel(0)
{
}

CDGSdoView::~CDGSdoView()
{
}

BEGIN_MESSAGE_MAP(CDGSdoView, CView)
	//{{AFX_MSG_MAP(COscView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDGSdoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COscView diagnostics

#ifdef _DEBUG
void CDGSdoView::AssertValid() const
{
	CView::AssertValid();
}

void CDGSdoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COscView message handlers

void CDGSdoView::CreateTab ()
{
    RECT rc;

    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTPTabControl();
	if (!m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		rc, this, IDC_FLAT_DG_TABCTRL))  {
		TRACE0( "Failed to create flattab control\n" );
		return;
	}
    m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
    m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorVisualStudio2012);
    m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
	m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (TRUE);
    m_pSdoRead=new CDlgSdoRead ();
    if (!m_pSdoRead->Create (IDD_DLG_SDO_READ, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create triggers window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(0, LOADSTRING (IDS_ADAMODGSDOVIEW_1), m_pSdoRead->GetSafeHwnd ());
    m_pSdoWrite=new CDlgSdoWrite();
    if (!m_pSdoWrite->Create (IDD_DLG_SDO_WRITE, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create samples window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(1, LOADSTRING (IDS_ADAMODGSDOVIEW_2), m_pSdoWrite->GetSafeHwnd ());
	m_pFlatTabCtrl->SetCurSel(0);
}

void CDGSdoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_pFlatTabCtrl->MoveWindow (0, 0, cx, cy);
}

int CDGSdoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CreateTab ();
	return 0;
}

void CDGSdoView::OnDestroy()
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    m_pFlatTabCtrl->DeleteAllItems ();
    m_pFlatTabCtrl->DestroyWindow ();
    _delete (m_pFlatTabCtrl);
    m_pSdoRead->DestroyWindow ();
    m_pSdoWrite->DestroyWindow ();
    _delete (m_pSdoRead);
    _delete (m_pSdoWrite);
}

BOOL CDGSdoView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    return CView::OnNotify(wParam, lParam, pResult);
}
