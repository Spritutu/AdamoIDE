// ScintillaView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "ScintillaView.h"

//#include "OutputWnd.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScintillaView

IMPLEMENT_DYNCREATE(CScintillaView, CView)

extern UINT m_nRefreshMetrics;

CScintillaView::CScintillaView()
{
}

CScintillaView::~CScintillaView()
{
}


BEGIN_MESSAGE_MAP(CScintillaView, CView)
	//{{AFX_MSG_MAP(CScintillaView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScintillaView drawing

void CScintillaView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CScintillaView diagnostics

#ifdef _DEBUG
void CScintillaView::AssertValid() const
{
	CView::AssertValid();
}

void CScintillaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScintillaView message handlers

BOOL CScintillaView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::PreCreateWindow(cs);
}

int CScintillaView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	static int nCtrlID = ID_OUTPUT_CTRL3;

	// TODO: Add your specialized creation code here
	m_view.Create(this, nCtrlID++);
	m_view.SetReadOnly(TRUE);

	return 0;
}

void CScintillaView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_view.m_hWnd) )
		m_view.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

void CScintillaView::Clear()
{
	m_view.SetReadOnly(FALSE);
	m_view.ClearAll();
	m_view.SetReadOnly(TRUE);
}

void CScintillaView::Write(CString strMsg)
{
	m_view.GotoLastLine();
	m_view.SetReadOnly(FALSE);
	m_view.AddText(strMsg);
	m_view.SetReadOnly(TRUE);
}

BOOL CScintillaView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *lpnmhdr = (LPNMHDR) lParam; 

	if (lpnmhdr->hwndFrom == m_view.m_hWnd)
	{
		*pResult = ((CMainFrame*)AfxGetMainWnd())->OnSci(this, (SCNotification*)lParam);
		return TRUE;
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

LRESULT CScintillaView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	GetEditor()->SetSkin ();
	return 0;
}
