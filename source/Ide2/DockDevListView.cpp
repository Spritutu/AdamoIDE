// DockDevListView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "NewDev.h"
#include "MainFrame.h"
#include "ConfigDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockDevListView

IMPLEMENT_DYNCREATE(CDockDevListView, CPaneView)

CDockDevListView::CDockDevListView() : m_pDragImage(NULL), m_bDragging(FALSE), m_bIsHighlight(false), m_bInitialized (false)
{
}

CDockDevListView::~CDockDevListView()
{
	if(m_pDragImage)
		delete m_pDragImage;
}


BEGIN_MESSAGE_MAP(CDockDevListView, CPaneView)
	//{{AFX_MSG_MAP(CDockDevListView)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDockDevListView drawing

void CDockDevListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDockDevListView diagnostics

#ifdef _DEBUG
void CDockDevListView::AssertValid() const
{
	CPaneView::AssertValid();
}

void CDockDevListView::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDockDevListView message handlers

void CDockDevListView::OnInitialUpdate() 
{
	CPaneView::OnInitialUpdate();
    if (!m_bInitialized)   {
		m_wndReport.Create ( WS_CHILD | WS_VISIBLE, CRect(0,0, 0, 0), this, ID_REPCTRL_CONFDEVLIST);
		m_wndReport.SetOwner(this);		
		m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureNone;
        InitView ();
        m_bInitialized=true;
    }
}

void CDockDevListView::InitView ()
{
	CXTPReportRecord* pRecord, *pChild, *pChild_1;
	CXTPReportRecordItem* pItem;
	CXTPReportColumn* pColumn;
	CString str;	
	int i = 0, nIconIndex;

	/* creiamo innanzitutto la colonna */
	pColumn = new CXTPReportColumn(0, LOADSTRING (IDS_ADAMODEVICE_0), 0, TRUE);
	m_wndReport.AddColumn(pColumn);
	pColumn->SetTreeColumn(TRUE);
	/* aggiorniamo le icons */
	UpdateImages ();
	/* aggiungiamo il primo record, la macchina */
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	pItem = new CXTPReportRecordItemText (LOADSTRING (IDS_ADAMODEVICE_10));
	pItem->SetIconIndex (0);
	pRecord->AddItem (pItem);
	/* aggiungiamo il secondo record, i gruppi */
	pChild = new CXTPReportRecord();
	pItem = new CXTPReportRecordItemText (LOADSTRING (IDS_ADAMODEVICE_11));
	pItem->SetIconIndex (2);
	pChild->AddItem (pItem);
	pRecord->GetChilds()->Add(pChild);
	while (i < 8)   {
		switch (i)   {
			case 0 :
				str = LOADSTRING (IDS_ADAMODEVICE_1); nIconIndex = 6 ;break;
			case 1 :
				str = LOADSTRING (IDS_ADAMODEVICE_2); nIconIndex = 7; break;
			case 2 :
				str = LOADSTRING (IDS_ADAMODEVICE_3); nIconIndex = 4; break;
			case 3 :
				str = LOADSTRING (IDS_ADAMODEVICE_4); nIconIndex = 5; break;
			case 4 :
				str = LOADSTRING (IDS_ADAMODEVICE_5); nIconIndex = 3; break;
			case 5 :
				str = LOADSTRING (IDS_ADAMODEVICE_6); nIconIndex = 8; break;
			case 6 :
				str = LOADSTRING (IDS_ADAMODEVICE_7); nIconIndex = 9; break;
			case 7 :
				str = LOADSTRING (IDS_ADAMODEVICE_8); nIconIndex = 10; break;
		}
		pChild_1 = new CXTPReportRecord();
		pItem = new CXTPReportRecordItemText (str);
		pItem->SetIconIndex (nIconIndex);
		pChild_1->AddItem (pItem);
		pChild->GetChilds()->Add(pChild_1);
		i++;
	}
	m_wndReport.Populate ();
	/* nascondiamo l'header */
	m_wndReport.ShowHeader (FALSE);
	/* esplodiamo tutti i rami */
	m_wndReport.ExpandAll (TRUE);
}

BOOL CDockDevListView::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_ICON;

	return CPaneView::PreCreateWindow(cs);
}

void CDockDevListView::OnBegindrag(NMHDR* pnmhdr, LRESULT* pResult) 
{
}

void CDockDevListView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPaneView::OnMouseMove(nFlags, point);
}

void CDockDevListView::OnLButtonUp(UINT nFlags, CPoint point)
{	
	CPaneView::OnLButtonUp(nFlags, point);
}

CView* CDockDevListView::GetActiveView()
{
	CMDIFrameWnd *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	if ( !pChild )
		return NULL;

	CView *pView = pChild->GetActiveView();
	if ( pView && pView->IsKindOf(RUNTIME_CLASS(CConfTreeView)) )
		return pView;

	return NULL;
}

void CDockDevListView::RefreshMetrics(AppTheme nTheme)
{
	switch (nTheme)
	{
		case themeVS2010: m_wndReport.SetTheme(xtpReportThemeVisualStudio2012Blue); break;
		case themeVS2012Light: m_wndReport.SetTheme(xtpReportThemeVisualStudio2012Light); break;
		case themeVS2012Dark: m_wndReport.SetTheme(xtpReportThemeVisualStudio2012Dark); break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: m_wndReport.SetTheme(xtpReportThemeVisualStudio2015); break;
	}	
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureNone;
	m_wndReport.GetPaintManager()->SetGridStyle (FALSE, xtpReportLineStyleNone);
	m_wndReport.GetPaintManager()->SetGridStyle (TRUE, xtpReportLineStyleNone);
}

void CDockDevListView::UpdateImages ()
{
	m_wndReport.GetImageManager()->RemoveAll();
	VERIFY(m_wndReport.GetImageManager()->SetIcons (IDB_CONF_TREE, 0, 0, CSize (16, 16)));
}

void CDockDevListView::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_wndReport.m_hWnd) )
		m_wndReport.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

BEGIN_MESSAGE_MAP(CAdamoDDLVCXTPReportControl, CXTPReportControl)
	ON_WM_LBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_MOUSEMOVE ()
END_MESSAGE_MAP()

void CAdamoDDLVCXTPReportControl::OnLButtonDown (UINT nFlags, CPoint point)
{
    if (((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ()<=ePswBuilder)  {
		__super::OnLButtonDown (nFlags, point);
		m_ButtonDown = true;
		m_point = point;
	}
}

void CAdamoDDLVCXTPReportControl::OnLButtonUp (UINT nFlags, CPoint point)
{
	__super::OnLButtonUp (nFlags, point);
	m_ButtonDown = false;
	if (m_Dragging)   {
		if (m_pRecord)   {
			CView* pActiveView = ((CDockDevListView *) GetParent ())->GetActiveView ();
			((CConfTreeView*)pActiveView)->DropItemOnList (GetDraggedDevice ());
		}
		m_Dragging = false;
		m_pRecord = NULL;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		ReleaseCapture ();
	}
}

void CAdamoDDLVCXTPReportControl::OnMouseMove (UINT nFlags, CPoint point)
{
	if (!m_Dragging)   {
		__super::OnMouseMove (nFlags, point);
		if (m_ButtonDown)   {
			CPoint p = point - m_point;
			if (abs (p.x) > 4 || abs (p.y) > 4)   {
				CXTPReportSelectedRows* pSelectedRows = GetSelectedRows ();
				if (pSelectedRows->GetCount () > 0)   {
					CXTPReportRow* pRow = pSelectedRows->GetAt (0);
					if (pRow)   {
						m_pRecord = pRow->GetRecord ();
						m_Dragging = true;
						OnDrag (m_pRecord, point);
						SetCapture ();
					}
				}
			}
		}
	}
	else
		OnDrag (m_pRecord, point);
}

void CAdamoDDLVCXTPReportControl::OnDrag (CXTPReportRecord* pRecord, CPoint point)
{
	CXTPReportHitTestInfo pInfo;
	CPoint pt (point);
	ClientToScreen (&pt);
	CWnd* pDropWnd = WindowFromPoint (pt);
	ASSERT (pDropWnd);

	CView* pActiveView = ((CDockDevListView *) GetParent ())->GetActiveView ();
	if (pActiveView->IsKindOf (RUNTIME_CLASS(CConfTreeView)))   {
		CXTPReportControl* pXTPReportControl = ((CConfTreeView*)pActiveView)->GetReportControl ();
		CRect rcDockDevList;
		CRect rcConfTreeView;
		GetWindowRect (&rcDockDevList);
		pXTPReportControl->GetWindowRect (&rcConfTreeView);
		int xMouse = point.x - rcConfTreeView.left + rcDockDevList.left;
		int yMouse = point.y - rcConfTreeView.top + rcDockDevList.top;
		CPoint ptMouse (xMouse, yMouse);
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CAdamoCXTPReportControl)))   {
				HWND hwnd_1 = pDropWnd->GetSafeHwnd ();
				HWND hwnd_2 = pXTPReportControl->GetSafeHwnd ();
				if (hwnd_1 == hwnd_2)   {
					SetCursor(AfxGetApp()->LoadCursor (IDC_DROPCURSOR));
					pXTPReportControl->HitTest (ptMouse, &pInfo);
					if (pInfo.m_pRow)
						pXTPReportControl->SetFocusedRow (pInfo.m_pRow);
				}
				else
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		}
	}
}

/*
** GetDraggedDevice :
*/
int CAdamoDDLVCXTPReportControl::GetDraggedDevice ()
{
	int nIndex = -1;

	CString str = ((CXTPReportRecordItemText *)m_pRecord->GetItem (0))->GetValue ();
	if (str == LOADSTRING (IDS_ADAMODEVICE_10))
		nIndex = 0;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_11))
		nIndex = 1;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_1))
		nIndex = 7;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_2))
		nIndex = 8;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_3))
		nIndex = 5;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_4))
		nIndex = 6;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_5))
		nIndex = 2;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_6))
		nIndex = 4;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_7))
		nIndex = 3;
	else
	if (str == LOADSTRING (IDS_ADAMODEVICE_8))
		nIndex = 9;
	return nIndex;
}
