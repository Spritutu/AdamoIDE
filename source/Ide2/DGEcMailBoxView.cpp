// DGEcMailBoxView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgEcMailBoxRead.h"
#include "DlgEcMailBoxWrite.h"
#include "DGEcMailBoxView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDGEcMailBoxView

IMPLEMENT_DYNCREATE(CDGEcMailBoxView, CView)

CDGEcMailBoxView::CDGEcMailBoxView()
{
}

CDGEcMailBoxView::~CDGEcMailBoxView()
{
}


BEGIN_MESSAGE_MAP(CDGEcMailBoxView, CView)
	//{{AFX_MSG_MAP(CDGEcMailBoxView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGEcMailBoxView drawing

void CDGEcMailBoxView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDGEcMailBoxView diagnostics

#ifdef _DEBUG
void CDGEcMailBoxView::AssertValid() const
{
	CView::AssertValid();
}

void CDGEcMailBoxView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDGEcMailBoxView message handlers
void CDGEcMailBoxView::CreateTab ()
{
    RECT rc;

    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTPTabControl();
	if (!m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		rc, this, IDC_FLAT_EC_MB_TABCTRL))  {
		TRACE0( "Failed to create flattab control\n" );
		return;
	}
    m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
    m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorVisualStudio2012);
    m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
	m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (TRUE);
    m_pEcMailBoxRead=new CDlgEcMailBoxRead ();
    if (!m_pEcMailBoxRead->Create (IDD_DLG_EC_MAILBOX_READ, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create triggers window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(0, LOADSTRING (IDS_EC_MAILBOX_READ), m_pEcMailBoxRead->GetSafeHwnd ());
    m_pEcMailBoxWrite=new CDlgEcMailBoxWrite();
    if (!m_pEcMailBoxWrite->Create (IDD_DLG_EC_MAILBOX_WRITE, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create samples window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(1, LOADSTRING (IDS_EC_MAILBOX_WRITE), m_pEcMailBoxWrite->GetSafeHwnd ());
	m_pFlatTabCtrl->SetCurSel(0);
}

int CDGEcMailBoxView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CreateTab ();
	return 0;
}

void CDGEcMailBoxView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_pFlatTabCtrl->MoveWindow (0, 0, cx, cy);
}

void CDGEcMailBoxView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    m_pFlatTabCtrl->DeleteAllItems ();
    m_pFlatTabCtrl->DestroyWindow ();
    _delete (m_pFlatTabCtrl);
    m_pEcMailBoxRead->DestroyWindow ();
    m_pEcMailBoxWrite->DestroyWindow ();
    _delete (m_pEcMailBoxRead);
    _delete (m_pEcMailBoxWrite);
}

void CDGEcMailBoxView::LoadEthercatXMLFile (CPtrArray &slaveArray)
{
	m_pEcMailBoxRead->LoadEthercatXMLFile (slaveArray);
	m_pEcMailBoxWrite->LoadEthercatXMLFile (slaveArray);
}
