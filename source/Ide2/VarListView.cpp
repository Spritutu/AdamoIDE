// VarListView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "VarListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVarListView

IMPLEMENT_DYNCREATE(CVarListView, CListView)

CVarListView::CVarListView()
{
}

CVarListView::~CVarListView()
{
}


BEGIN_MESSAGE_MAP(CVarListView, CListView)
	//{{AFX_MSG_MAP(CVarListView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarListView drawing

void CVarListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CVarListView diagnostics

#ifdef _DEBUG
void CVarListView::AssertValid() const
{
	CListView::AssertValid();
}

void CVarListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVarListView message handlers

int CVarListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	GetListCtrl().ModifyStyleEx(0, WS_EX_STATICEDGE);
	CString str;
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;

    str=LOADSTRING (IDS_ADAMOVARLISTVIEW_1);
	lvc.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
	lvc.cx = 70;
	lvc.fmt = LVCFMT_LEFT;
	GetListCtrl().InsertColumn(0,&lvc);

	lvc.iSubItem = 1;
    str=LOADSTRING (IDS_ADAMOVARLISTVIEW_2);
	lvc.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
	lvc.cx = 60;
	lvc.fmt = LVCFMT_LEFT;
	GetListCtrl().InsertColumn(1,&lvc);

	lvc.iSubItem = 2;
    str=LOADSTRING (IDS_ADAMOVARLISTVIEW_3);
	lvc.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
	lvc.cx = 300;
	lvc.fmt = LVCFMT_LEFT;
	GetListCtrl().InsertColumn(2,&lvc);

	return 0;
}

BOOL CVarListView::PreCreateWindow(CREATESTRUCT& cs) 
{	
	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}
