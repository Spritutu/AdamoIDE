// AdamoMDSplittedView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoMDSplittedView.h"
#include "AdamoMDIOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDSplittedView

IMPLEMENT_DYNCREATE(CAdamoMDSplittedView, CView)

CAdamoMDSplittedView::CAdamoMDSplittedView() : m_pIODlg (NULL)
{
}

CAdamoMDSplittedView::~CAdamoMDSplittedView()
{
}


BEGIN_MESSAGE_MAP(CAdamoMDSplittedView, CView)
	//{{AFX_MSG_MAP(CAdamoMDSplittedView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDSplittedView drawing

void CAdamoMDSplittedView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDSplittedView diagnostics

#ifdef _DEBUG
void CAdamoMDSplittedView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoMDSplittedView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDSplittedView message handlers

int CAdamoMDSplittedView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    m_pIODlg=new CAdamoMDIOViewDlg;
    if (!m_pIODlg->Create (IDD_DLGIOVIEW, this))   {
		TRACE0( "Failed to create triggers window\n" );
		return -1;
	}
	return 0;
}


void CAdamoMDSplittedView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pIODlg)   {
        m_pIODlg->DestroyWindow ();
        _delete (m_pIODlg);
    }
}

void CAdamoMDSplittedView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_pIODlg->MoveWindow (0, 0, cx, cy);      
}

void CAdamoMDSplittedView::OnAdamoTimer() 
{
	// TODO: Add your message handler code here and/or call default
	m_pIODlg->OnAdamoTimer ();
}

void CAdamoMDSplittedView::OnViewSettings (stMDSettings* MDS)
{
    m_pIODlg->OnViewSettings (MDS);
}

void CAdamoMDSplittedView::GetViewSettings (stMDSettings* MDS)
{
    m_pIODlg->GetViewSettings (MDS);
}

void CAdamoMDSplittedView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_pIODlg->OnChangePsw (oldPsw, newPsw);
}
