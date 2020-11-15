// OscView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "OscView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COscView

IMPLEMENT_DYNCREATE(COscView, CView)

COscView::COscView() : m_pFlatTabCtrl (NULL), m_pTriggers (NULL), m_pSamples (NULL),
                       m_pOscilloscope (NULL), m_nCurSel(0)
{
}

COscView::~COscView()
{
}


BEGIN_MESSAGE_MAP(COscView, CView)
	//{{AFX_MSG_MAP(COscView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COscView drawing

void COscView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COscView diagnostics

#ifdef _DEBUG
void COscView::AssertValid() const
{
	CView::AssertValid();
}

void COscView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COscView message handlers
void COscView::CreateTab ()
{
    RECT rc;

	memset (&rc, 0, sizeof (RECT));
    m_pFlatTabCtrl = new CXTPTabControl ();
	if (!m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		rc, this, IDC_FLAT_TABCTRL))  {
		TRACE0( "Failed to create flattab control\n" );
		return;
	}
    //m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
    //m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorOffice2003);
    //m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
    //m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (FALSE);

	m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
    m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorVisualStudio2012);
    m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
    m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (TRUE);
    m_pFlatTabCtrl->GetPaintManager()->m_clientFrame =xtpTabFrameNone;
	m_pFlatTabCtrl->GetPaintManager ()->GetColorSet ()->m_clrHeaderFace = RGB (180,180,180);
	m_pFlatTabCtrl->GetPaintManager ()->m_bFillBackground = TRUE;

    m_pTriggers=new CDlgTriggers ();
    if (!m_pTriggers->Create (IDD_DLGTRIGGERS, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create triggers window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(0, LOADSTRING (IDS_ADAMOOSCVIEW_1), m_pTriggers->GetSafeHwnd ());
    m_pSamples=new CDlgSamples ();
    if (!m_pSamples->Create (IDD_DLGSAMPLES, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create samples window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(1, LOADSTRING (IDS_ADAMOOSCVIEW_2), m_pSamples->GetSafeHwnd ());
    m_pOscilloscope=new CDlgOscilloscope ();
    if (!m_pOscilloscope->Create (IDD_DLGOSCILLOSCOPE, m_pFlatTabCtrl))   {
		TRACE0( "Failed to create oscilloscope window\n" );
		return;
	}
	m_pFlatTabCtrl->InsertItem(2, LOADSTRING (IDS_ADAMOOSCVIEW_3), m_pOscilloscope->GetSafeHwnd ());
    m_pOscilloscope->SettaDlgTriggers (m_pTriggers);
    m_pOscilloscope->SettaDlgSamples (m_pSamples);
    m_pSamples->SettaDlgOscilloscope (m_pOscilloscope);
    m_pTriggers->SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_pSamples->SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
	m_pFlatTabCtrl->SetCurSel(0);
}

void COscView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_pFlatTabCtrl->MoveWindow (0, 0, cx, cy);
}

int COscView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	CreateTab ();
	return 0;
}

void COscView::OnDestroy()
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    m_pFlatTabCtrl->DestroyWindow ();
	m_pTriggers->DestroyWindow ();
    m_pSamples->DestroyWindow ();
    m_pOscilloscope->DestroyWindow ();
    _delete (m_pFlatTabCtrl);
    _delete (m_pTriggers);
    _delete (m_pSamples);
    _delete (m_pOscilloscope);
}

BOOL COscView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ( IDC_FLAT_TABCTRL == (UINT)wParam)   {
		NMHDR* pNMHDR = (NMHDR*)lParam;
		switch(pNMHDR->code)   {
		    case TCN_SELCHANGING:
			    break;
		    case TCN_SELCHANGE:
				if (m_pSamples)
					m_pSamples->EndEdit ();
                m_nCurSel=m_pFlatTabCtrl->GetCurSel();
                if (m_nCurSel==2)
                    m_pOscilloscope->InitOscilloscope ();
 			    break;
		}
    }
    return CView::OnNotify(wParam, lParam, pResult);
}


