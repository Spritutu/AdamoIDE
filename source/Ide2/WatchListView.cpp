// WatchListView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "WatchListView.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWatchListView

IMPLEMENT_DYNCREATE(CWatchListView, CListView)

CWatchListView::CWatchListView()
{
}

CWatchListView::~CWatchListView()
{
}


BEGIN_MESSAGE_MAP(CWatchListView, CListView)
	//{{AFX_MSG_MAP(CWatchListView)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWatchListView drawing

void CWatchListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CWatchListView diagnostics

#ifdef _DEBUG
void CWatchListView::AssertValid() const
{
	CListView::AssertValid();
}

void CWatchListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWatchListView message handlers

void CWatchListView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( pDispInfo->item.pszText )
		AddEditItem(pDispInfo->item);

	*pResult = 0;
}

void CWatchListView::AddEditItem(LVITEM &item)
{
	if ( item.iItem == GetListCtrl().GetItemCount()-1 )
	{
		if ( strlen(item.pszText) == 0 )
			return;

		m_exps.Add(item.pszText);
		AddEmptyRow();
		UpdateRow(item.iItem);
	}
	else
	{
		if ( strlen(item.pszText) == 0 )
		{
			GetListCtrl().DeleteItem(item.iItem);
			m_exps.RemoveAt(item.iItem);
			return;
		}
		else
		{
			m_exps[item.iItem] = item.pszText;
			UpdateRow(item.iItem);
		}
	}
}

void CWatchListView::AddEmptyRow()
{
	int nItem = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), "");
	GetListCtrl().SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);
}

BOOL CWatchListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_EDITLABELS|LVS_NOSORTHEADER|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_REPORT;
	
	return CListView::PreCreateWindow(cs);
}

void CWatchListView::UpdateRow(int iItem)
{
	GetListCtrl().SetItemText(iItem, 0, m_exps[iItem]);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if ( pFrame->GetMode() == CMainFrame::modeDebugBreak )
	{
		GetListCtrl().SetItemText(iItem, 1, pFrame->GetDebugger()->Eval(m_exps[iItem]));
	}
}

void CWatchListView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SetFocus();

	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	GetListCtrl().SubItemHitTest(&lvhti);

	if (lvhti.flags & LVHT_ONITEMLABEL)
		GetListCtrl().EditLabel(lvhti.iItem);
	else    
	    CListView::OnLButtonDblClk(nFlags, point);
}

void CWatchListView::Redraw()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	for ( int i=0; i<m_exps.GetSize(); ++i )
		GetListCtrl().SetItemText(i, 1, pFrame->GetDebugger()->Eval(m_exps[i]));
}

int CWatchListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    CString str;
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetListCtrl().ModifyStyleEx(0, WS_EX_STATICEDGE);
	
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
    str=LOADSTRING (IDS_ADAMOWATCHLISTVIEW_1);
	lvc.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
	lvc.cx = 70;
	lvc.fmt = LVCFMT_LEFT;
	GetListCtrl().InsertColumn(0,&lvc);

	lvc.iSubItem = 1;
    str=LOADSTRING (IDS_ADAMOWATCHLISTVIEW_2);
	lvc.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
 	lvc.cx = 300;
	lvc.fmt = LVCFMT_LEFT;
	GetListCtrl().InsertColumn(2,&lvc);

	AddEmptyRow();
    
	return 0;
}
