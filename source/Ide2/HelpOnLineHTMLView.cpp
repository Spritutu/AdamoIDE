// DGEcMailBoxView.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrame.h"
#include "HelpOnLineHtmlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView

IMPLEMENT_DYNCREATE(CHtmlHelpOnLineView, CHtmlView)

CHtmlHelpOnLineView::CHtmlHelpOnLineView()
{
}

CHtmlHelpOnLineView::~CHtmlHelpOnLineView()
{
}


BEGIN_MESSAGE_MAP(CHtmlHelpOnLineView, CHtmlView)
	//{{AFX_MSG_MAP(CHelpOnLineView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView drawing

void CHtmlHelpOnLineView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView diagnostics

#ifdef _DEBUG
void CHtmlHelpOnLineView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlHelpOnLineView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

#endif //_DEBUG

int CHtmlHelpOnLineView::ViewHelpPage (CString strHelpPage)
{
	Navigate2 (strHelpPage);
	return 0;
}
