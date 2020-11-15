// HTMLView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoTemplates.h"
#include "HTMLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLView

IMPLEMENT_DYNCREATE(CHTMLView, CHtmlView)

CHTMLView::CHTMLView()
{
	//{{AFX_DATA_INIT(CHTMLView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHTMLView::~CHTMLView()
{
}

void CHTMLView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHTMLView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHTMLView, CHtmlView)
	//{{AFX_MSG_MAP(CHTMLView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLView diagnostics

#ifdef _DEBUG
void CHTMLView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHTMLView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTMLView message handlers

void CHTMLView::OnInitialUpdate() 
{
}

void CHTMLView::Activate()
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
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enHTMLTemplate);
    pTmpl->SetActiveView (this);
}
