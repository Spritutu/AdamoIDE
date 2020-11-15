// AdamoMDIOView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoMDIOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOView

IMPLEMENT_DYNCREATE(CAdamoMDIOView, CView)

CAdamoMDIOView::CAdamoMDIOView()
{
}

CAdamoMDIOView::~CAdamoMDIOView()
{
}


BEGIN_MESSAGE_MAP(CAdamoMDIOView, CView)
	//{{AFX_MSG_MAP(CAdamoMDIOView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOView diagnostics

#ifdef _DEBUG
void CAdamoMDIOView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoMDIOView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOView message handlers

void CAdamoMDIOView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
}

void CAdamoMDIOView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_grid.GetSafeHwnd())
        m_grid.SetWindowPos (NULL, -1, -1, cx, cy, SWP_SHOWWINDOW);
}

int CAdamoMDIOView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_grid.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);
	m_grid.InitializeGrid ();
	return 0;
}

void CAdamoMDIOView::OnDestroy() 
{
	CView::OnDestroy();
    m_grid.ResetContent ();
}

void CAdamoMDIOView::OnAdamoTimer() 
{
	// TODO: Add your message handler code here and/or call default
	m_grid.RinfrescaDispositivi ();
}

void CAdamoMDIOView::OnViewSettings (stMDSettings* MDS)
{

}

void CAdamoMDIOView::GetViewSettings (stMDSettings* MDS)
{

}
