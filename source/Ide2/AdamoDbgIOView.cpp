// AdamoDbgIOView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoDbgIOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdamoDbgIOView* CAdamoDbgIOView::m_pThis=NULL;

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgIOView

IMPLEMENT_DYNCREATE(CAdamoDbgIOView, CView)

CAdamoDbgIOView::CAdamoDbgIOView()
{
    m_pThis=this;
}

CAdamoDbgIOView::~CAdamoDbgIOView()
{
    m_pThis=NULL;
}


BEGIN_MESSAGE_MAP(CAdamoDbgIOView, CView)
	//{{AFX_MSG_MAP(CAdamoDbgIOView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgIOView drawing

void CAdamoDbgIOView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

////////////////////////////////////////////////////////////////////////////
// CAdamoDbgIOView diagnostics

#ifdef _DEBUG
void CAdamoDbgIOView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDbgIOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgIOView message handlers
void CAdamoDbgIOView::OnAdamoTimer ()
{
    m_grid.RinfrescaDispositivi ();
}

void CAdamoDbgIOView::OnDestroy() 
{
	CView::OnDestroy();
    m_grid.ResetContent ();
}

int CAdamoDbgIOView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* creiamo il grid */
	m_grid.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);
	return 0;
}

void CAdamoDbgIOView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_grid.GetSafeHwnd())
        m_grid.SetWindowPos (NULL, 0, 0, cx, cy, SWP_SHOWWINDOW);
}

void CAdamoDbgIOView::UpdateData ()
{
    m_grid.ResetContent ();
	m_grid.InitializeGrid ();
}
