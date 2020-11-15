 // AdamoSEListView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoSEListView.h"
#include "AdamoErr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CAdamoSEListView::m_listGID = 1000;

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEListView

IMPLEMENT_DYNCREATE(CAdamoSEListView, CView)

CAdamoSEListView::CAdamoSEListView() : m_nErrorType (-1), m_hciml (NULL), m_lviml (NULL), m_bmpHdrDate (NULL),
    m_bmpHdrError (NULL), m_bmpHdrTask (NULL), m_bmpHdrPath (NULL), m_bmpHdrLine (NULL), m_bmpHdrDevice (NULL), m_bmpHdrTime (NULL),
    m_bmpHdrDeviceType (NULL), m_bmpHdrIdentifier (NULL), m_bmpHdrPriority (NULL), m_bmpHdrDisk (NULL), m_bmpHdrEvent (NULL), m_pSEDoc (NULL), m_nItem (-1)
{
	m_listID = m_listGID++;
}

CAdamoSEListView::~CAdamoSEListView()
{
}

BEGIN_MESSAGE_MAP(CAdamoSEListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CAdamoSEListView)
	ON_WM_ERASEBKGND ()
	ON_WM_CREATE ()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CAdamoSEListView, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
	ON_WM_CREATE ()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(IDM_SE_FIND, OnUpdateSeFind)
	ON_COMMAND(IDM_SE_FIND, OnSeFind)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEListView drawing

int CAdamoSEListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, m_listID);
	return b != 0;
}

void CAdamoSEListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEListView diagnostics

#ifdef _DEBUG
void CAdamoSEListView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoSEListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEListView message handlers
void CAdamoSEListView::CreateColumnHeader ()
{
    switch (m_nErrorType)   {
        case TE_HARDWARE :
            LoadHeaderHardware ();
            break;
        case TE_ISO :
            LoadHeaderISO ();
            break;
        case TE_INTERPRETER :
            LoadHeaderInterpreter ();
            break;
        case TE_CYCLE :
            LoadHeaderCycle ();
            break;
        case TE_EVENT :
            LoadHeaderEvents ();
            break;
        default :
            LoadAllErrors ();
            break;
    }
}

/*
** LoadHeaderCycle :
*/
void CAdamoSEListView::LoadHeaderCycle ()
{
    CRect rect;

    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT,
		rect.Width() * 36/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT,
		rect.Width() * 8/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_5), LVCFMT_LEFT,
		rect.Width() * 12/100, 2);
	m_list.InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_6), LVCFMT_LEFT,
		(rect.Width() * 8/100)-2, 3);
	m_list.InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
	m_list.InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT,
		rect.Width() * 8/100, 5);
	m_list.InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT,
		rect.Width() * 8/100, 6);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrPriority=new CBitmap;
    m_bmpHdrDisk=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrPriority->LoadBitmap(IDB_CAPPRIORITY);
    m_bmpHdrDisk->LoadBitmap(IDB_CAPDISK);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrIdentifier, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPriority, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDisk, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTime, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    hc->SetImageList(m_hciml);

    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->GetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hc->GetItem(3, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=3;
    hc->SetItem(3, &hd);
    hc->GetItem(4, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=4;
    hc->SetItem(4, &hd);
    hc->GetItem(5, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=5;
    hc->SetItem(5, &hd);
    hc->GetItem(6, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=6;
    hc->SetItem(6, &hd);
	for (int i = 0;i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}

/*
** LoadHeaderInterpreter :
*/
void CAdamoSEListView::LoadHeaderInterpreter ()
{
    CRect rect;

    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT,
		rect.Width() * 34/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT,
		rect.Width() * 8/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_8), LVCFMT_LEFT,
		rect.Width() * 14/100, 2);
	m_list.InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_9), LVCFMT_LEFT,
		rect.Width() * 8/100, 3);
	m_list.InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
	m_list.InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT,
		rect.Width() * 8/100, 5);
	m_list.InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT,
		rect.Width() * 8/100, 6);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrPath=new CBitmap;
    m_bmpHdrLine=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrPath->LoadBitmap(IDB_CAPPATH);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrLine->LoadBitmap(IDB_CAPLINE);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrIdentifier, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPath, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrLine, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTime, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    hc->SetImageList(m_hciml);

    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->GetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hc->GetItem(3, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=3;
    hc->SetItem(3, &hd);
    hc->GetItem(4, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=4;
    hc->SetItem(4, &hd);
    hc->GetItem(5, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=5;
    hc->SetItem(5, &hd);
    hc->GetItem(6, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=6;
    hc->SetItem(6, &hd);
	for (int i = 0;i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}

/*
** LoadHeaderISO :
*/
void CAdamoSEListView::LoadHeaderISO ()
{
    CRect rect;

    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT,
		rect.Width() * 34/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT,
		rect.Width() * 8/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_8), LVCFMT_LEFT,
		rect.Width() * 14/100, 2);
	m_list.InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_9), LVCFMT_LEFT,
		rect.Width() * 8/100, 3);
	m_list.InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
	m_list.InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT,
		rect.Width() * 8/100, 5);
	m_list.InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT,
		rect.Width() * 8/100, 6);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrPath=new CBitmap;
    m_bmpHdrLine=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrPath->LoadBitmap(IDB_CAPPATH);
    m_bmpHdrError->LoadBitmap(IDB_CAPISOERRORS);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrLine->LoadBitmap(IDB_CAPLINE);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrIdentifier, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPath, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrLine, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTime, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    hc->SetImageList(m_hciml);

    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->GetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hc->GetItem(3, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=3;
    hc->SetItem(3, &hd);
    hc->GetItem(4, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=4;
    hc->SetItem(4, &hd);
    hc->GetItem(5, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=5;
    hc->SetItem(5, &hd);
    hc->GetItem(6, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=6;
    hc->SetItem(6, &hd);
	for (int i = 0;i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}

/*
** LoadHeaderHardware :
*/
void CAdamoSEListView::LoadHeaderHardware ()
{
    CRect rect;

    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT,
		rect.Width() * 31/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT,
		rect.Width() * 8/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT,
		rect.Width() * 15/100, 2);
	m_list.InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_10), LVCFMT_LEFT,
		rect.Width() * 10/100, 3);
	m_list.InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_11), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
	m_list.InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT,
		rect.Width() * 8/100, 5);
	m_list.InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT,
		rect.Width() * 8/100, 6);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrDevice=new CBitmap;
    m_bmpHdrDeviceType=new CBitmap;
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrDevice->LoadBitmap(IDB_CAPDEVICE);
    m_bmpHdrDeviceType->LoadBitmap(IDB_CAPTYPEDEVICE);
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrIdentifier, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDevice, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDeviceType, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTime, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    hc->SetImageList(m_hciml);

    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->GetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hc->GetItem(3, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=3;
    hc->SetItem(3, &hd);
    hc->GetItem(4, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=4;
    hc->SetItem(4, &hd);
    hc->GetItem(5, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=5;
    hc->SetItem(5, &hd);
    hc->GetItem(6, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=6;
    hc->SetItem(6, &hd);
	for (int i = 0;i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}

/*
** LoadHeaderEvents :
*/
void CAdamoSEListView::LoadHeaderEvents ()
{
    CRect rect;

    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_22), LVCFMT_LEFT,
		rect.Width() * 60/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT,
		rect.Width() * 20/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT,
		rect.Width() * 20/100, 2);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrEvent=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrEvent->LoadBitmap(IDB_CAPEVENTS);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrEvent, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTime, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    hc->SetImageList(m_hciml);

    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->GetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
	for (int i = 0;i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}

void CAdamoSEListView::DeleteHeaderResource ()
{
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    int i, n=hc->GetItemCount();

    for (i=0; i<n; i++)
        m_list.DeleteColumn (0);
    _delete (m_bmpHdrTime);
    _delete (m_bmpHdrDate);
    _delete (m_bmpHdrError);
    _delete (m_bmpHdrTask);
    _delete (m_bmpHdrPath);
    _delete (m_bmpHdrLine);
    _delete (m_bmpHdrDevice);
    _delete (m_bmpHdrDeviceType);
    _delete (m_bmpHdrIdentifier);
    _delete (m_bmpHdrPriority);
    _delete (m_bmpHdrDisk);
    _delete (m_bmpHdrEvent);
    _delete (m_hciml);
}

void CAdamoSEListView::OnDestroy() 
{
	DeleteLVParam ();
	__super::OnDestroy();
    DeleteHeaderResource ();
    _delete (m_bmpErrHardWare);
    _delete (m_bmpErrInterpreter);
    _delete (m_bmpErrCycle);
    _delete (m_bmpErrISO);
    _delete (m_lviml);
	m_list.DestroyWindow ();
}

BOOL CAdamoSEListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT;
    cs.style |= LVS_SINGLESEL;
    cs.style |= LVS_SHOWSELALWAYS;
    cs.style |= LVS_NOSORTHEADER;
	return CView::PreCreateWindow(cs);
}

/*
** LoadAllErrors :
*/
void CAdamoSEListView::LoadAllErrors ()
{
    CRect rect;
    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_11), LVCFMT_LEFT,
		rect.Width() * 15/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT,
		rect.Width() * 45/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT,
		rect.Width() * 20/100, 2);
	m_list.InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT,
		rect.Width() * 10/100, 3);
	m_list.InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT,
		rect.Width() * 10/100, 4);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDeviceType=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrDeviceType->LoadBitmap(IDB_CAPTYPEDEVICE);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrDeviceType, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTime, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    hc->SetImageList(m_hciml);

    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->GetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hc->GetItem(3, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=3;
    hc->SetItem(3, &hd);
    hc->GetItem(4, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=4;
    hc->SetItem(4, &hd);
	for (int i = 0;i < m_list.GetHeaderCtrl()->GetItemCount(); ++i)
		m_list.SetColumnWidth(i,LVSCW_AUTOSIZE_USEHEADER);
}

/*
** FillHardware :
*/
void CAdamoSEListView::FillHardware (stAdamoErrore* pE, int nItem)
{
	COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	if (nItem != 0)
		nItem = m_list.GetItemCount ();
	/* Errore */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, pE->strErrore);
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.iImage=pE->nTypeError;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
	lvi.lParam = NULL;
    m_list.InsertItem (&lvi);

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* nome del dispositivo */
    memset (&lvi, 0, sizeof (LVITEM));
	GestisciSENomeDelDispositivo (pE, szBuffer);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* tipo del dispositivo */
    memset (&lvi, 0, sizeof (LVITEM));
	GestisciSETipoDelDispositivo (pE, szBuffer);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** FillInterpreter :
*/
void CAdamoSEListView::FillInterpreter (stAdamoErrore* pE, int nItem)
{
	COleDateTime odt (pE->dt);
    LVITEM lvi;
	CString strModule, strLine, strError;
    char szBuffer[256];

	if (nItem != 0)
		nItem = m_list.GetItemCount ();
	/* estrapoliamo i dati di modulo linea ed errore */
	strcpy (szBuffer, pE->strErrore);
    ((CMainFrame *) AfxGetMainWnd ())->ParseStrError (szBuffer, &strModule, &strLine, &strError);

	/* errore */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
    lvi.pszText=strError.GetBuffer(64);
    lvi.cchTextMax=64;
	lvi.iImage=pE->nTypeError;
	lvi.lParam = (LPARAM) new stAdamoErrore;
	memcpy ((void *)lvi.lParam, pE, sizeof (stAdamoErrore));
    m_list.InsertItem (&lvi);
    strError.ReleaseBuffer ();

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* module name */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=strModule.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strModule.ReleaseBuffer ();

	/* linea */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=strLine.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strLine.ReleaseBuffer ();

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    m_list.SetItem (&lvi);

	/* ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** FillISO :
*/
void CAdamoSEListView::FillISO (stAdamoErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
	CString strModule, strLine, strError;
	char szBuffer[256];

	if (nItem != 0)
		nItem = m_list.GetItemCount ();
	strcpy (szBuffer, pE->strErrore);
    ((CMainFrame *) AfxGetMainWnd ())->ParseStrError (szBuffer, &strModule, &strLine, &strError);
	/* errore */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.iImage=pE->nTypeError;
    lvi.pszText=strError.GetBuffer(64);
    lvi.cchTextMax=64;
	lvi.lParam = NULL;
    m_list.InsertItem (&lvi);
    strError.ReleaseBuffer ();

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);
	
	/* module name */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=strModule.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strModule.ReleaseBuffer ();

	/* linea */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=strLine.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strLine.ReleaseBuffer ();

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** FillCycle :
*/
void CAdamoSEListView::FillCycle (stAdamoErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	if (nItem != 0)
		nItem = m_list.GetItemCount ();
    /* Errore */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, pE->strErrore);
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.iImage=pE->nTypeError;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
	lvi.lParam = NULL;
    m_list.InsertItem (&lvi);

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

    /* priorita */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nTypeDevice);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

    /* disk */
    memset (&lvi, 0, sizeof (LVITEM));
    if (pE->nDevice)
        strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_12));
    else
        strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_13));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    m_list.SetItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** FillEvent :
*/
void CAdamoSEListView::FillEvent (stAdamoErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	if (nItem != 0)
		nItem = m_list.GetItemCount ();
	/* errore */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
    strcpy (szBuffer, pE->strErrore);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
	lvi.iImage = pE->nTypeError;
	lvi.lParam = NULL;
    m_list.InsertItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** FillAll :
*/
void CAdamoSEListView::FillAll (stAdamoErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	if (nItem != 0)
		nItem = m_list.GetItemCount ();
    /* tipo di errore */
    memset (&lvi, 0, sizeof (LVITEM));
    switch (pE->nTypeError)   {
        case TE_HARDWARE :
            strcpy (szBuffer, LOADSTRING (IDS_ADAMOERRORSTYPE_1));
            break;
        case TE_INTERPRETER :
            strcpy (szBuffer, LOADSTRING (IDS_ADAMOERRORSTYPE_2));
            break;
        case TE_CYCLE :
            strcpy (szBuffer, LOADSTRING (IDS_ADAMOERRORSTYPE_3));
            break;
        case TE_ISO :
            strcpy (szBuffer, LOADSTRING (IDS_ADAMOERRORSTYPE_4));
            break;
        case TE_EVENT :
            strcpy (szBuffer, LOADSTRING (IDS_ADAMOERRORSTYPE_5));
            break;
    }
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
	lvi.iImage = pE->nTypeError;
	if (pE->nTypeError == TE_INTERPRETER)   {
		lvi.lParam = (LPARAM) new stAdamoErrore;
		memcpy ((void *)lvi.lParam, pE, sizeof (stAdamoErrore));
	}
	else
		lvi.lParam = NULL;
    m_list.InsertItem (&lvi);
	
	/* errore */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    strcpy (szBuffer, pE->strErrore);
    if (pE->nTypeError==TE_INTERPRETER||pE->nTypeError==TE_ISO)   {
        char* p=strrchr (pE->strErrore, '@');
        if (p)
            strcpy (szBuffer, p+1);
    }
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_list.SetItem (&lvi);

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    m_list.SetItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    m_list.SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

void CAdamoSEListView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_lviml=new CImageList;
    m_bmpErrHardWare=new CBitmap;
    m_bmpErrInterpreter=new CBitmap;
    m_bmpErrCycle=new CBitmap;
    m_bmpErrISO=new CBitmap;
    m_bmpErrHardWare->LoadBitmap (IDB_SLOTSTATUSRUN);
    m_bmpErrInterpreter->LoadBitmap (IDB_SLOTSTATUSSUSPENDED);
    m_bmpErrCycle->LoadBitmap (IDB_SLOTSTATUSWAITEVENT);
    m_bmpErrISO->LoadBitmap (IDB_SLOTSTATUSZOMBIE);
    m_lviml->Create (16, 16, ILC_MASK, 0, 1);
    m_lviml->Add(m_bmpErrHardWare, RGB(0, 128,128));
    m_lviml->Add(m_bmpErrInterpreter, RGB(0, 128,128));
    m_lviml->Add(m_bmpErrCycle, RGB(0, 128,128));
    m_lviml->Add(m_bmpErrISO, RGB(0, 128,128));
    //m_list.SetImageList (m_lviml, LVSIL_SMALL);
    DWORD dw=m_list.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
	m_list.SetExtendedStyle (dw);
    m_list.GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_list.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
}

void CAdamoSEListView::OnUpdateSeFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ();
}

void CAdamoSEListView::OnSeFind() 
{
	// TODO: Add your command handler code here
    m_pSEView->SendMessage (WM_OPENDIALOGFINDERRORS, 0, 0L);
}

/*
** GestisciSENomeDelDispositivo
*/
void CAdamoSEListView::GestisciSENomeDelDispositivo (stAdamoErrore* pE, char* szBuffer)
{
    if (pE->nTypeDevice<HARDWARE_ERROR)
        m_pSEDoc->GetDeviceName (pE->nDevice, pE->nTypeDevice, szBuffer);
    else
        switch (pE->nTypeDevice)   {
            case HARDWARE_ERROR :
                sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_14));
                break;
            case RECEIVER_ERROR :
                sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_15), pE->nDevice);
                break;
            case OMIKRON_ERROR :
                sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_18));
                break;
            case SSCNET_ERROR :
                sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_20), pE->nDevice);
                break;
        }
}

/*
** GestisciSETipoDelDispositivo
*/
void CAdamoSEListView::GestisciSETipoDelDispositivo (stAdamoErrore* pE, char* szBuffer)
{
    if (pE->nTypeDevice<HARDWARE_ERROR)
        strcpy (szBuffer, ((CMainFrame*)AfxGetMainWnd ())->GetDeviceString (pE->nTypeDevice));
    else
        switch (pE->nTypeDevice)   {
            case HARDWARE_ERROR :
                strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_16));
                break;
            case RECEIVER_ERROR :
                strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_17));
                break;
            case OMIKRON_ERROR :
                strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_19));
                break;
            case SSCNET_ERROR :
                sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_21));
                break;
        };
}

/*
** AddError :
*/
void CAdamoSEListView::AddError (stAdamoErrore *p, int nItem)
{
	switch (m_nErrorType)   {
		case TE_HARDWARE :
			FillHardware (p, nItem);
			break;
		case TE_ISO :
			FillISO (p, nItem);
			break;
		case TE_INTERPRETER :
			FillInterpreter (p, nItem);
			break;
		case TE_CYCLE :
			FillCycle (p, nItem);
			break;
		case TE_EVENT :
			FillEvent (p, nItem);
			break;
		default :
			FillAll (p, nItem);
			break;
	}
}

/*
** OnSize :
*/
void CAdamoSEListView::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_list.MoveWindow (0, 0, cx, cy);
	switch (m_nErrorType)   {
		case TE_ALL :
			m_list.SetColumnWidth (0, (int)((double)cx / 100.0 * 15.0));
			m_list.SetColumnWidth (1, (int)((double)cx / 100.0 * 45.0));
			m_list.SetColumnWidth (2, (int)((double)cx / 100.0 * 20.0));
			m_list.SetColumnWidth (3, (int)((double)cx / 100.0 * 10.0));
			m_list.SetColumnWidth (4, LVSCW_AUTOSIZE_USEHEADER);
			break;
		case TE_HARDWARE :
			m_list.SetColumnWidth (0, (int)((double)cx / 100.0 * 34.0));
			m_list.SetColumnWidth (1, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (2, (int)((double)cx / 100.0 * 12.0));
			m_list.SetColumnWidth (3, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (4, (int)((double)cx / 100.0 * 20.0));
			m_list.SetColumnWidth (5, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (6, LVSCW_AUTOSIZE_USEHEADER);
			break;
		case TE_ISO :
			m_list.SetColumnWidth (0, (int)((double)cx / 100.0 * 34.0));
			m_list.SetColumnWidth (1, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (2, (int)((double)cx / 100.0 * 12.0));
			m_list.SetColumnWidth (3, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (4, (int)((double)cx / 100.0 * 20.0));
			m_list.SetColumnWidth (5, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (6, LVSCW_AUTOSIZE_USEHEADER);
			break;
		case TE_INTERPRETER :
			m_list.SetColumnWidth (0, (int)((double)cx / 100.0 * 34.0));
			m_list.SetColumnWidth (1, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (2, (int)((double)cx / 100.0 * 12.0));
			m_list.SetColumnWidth (3, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (4, (int)((double)cx / 100.0 * 20.0));
			m_list.SetColumnWidth (5, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (6, LVSCW_AUTOSIZE_USEHEADER);
			break;
		case TE_CYCLE :
			m_list.SetColumnWidth (0, (int)((double)cx / 100.0 * 34.0));
			m_list.SetColumnWidth (1, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (2, (int)((double)cx / 100.0 * 12.0));
			m_list.SetColumnWidth (3, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (4, (int)((double)cx / 100.0 * 20.0));
			m_list.SetColumnWidth (5, (int)((double)cx / 100.0 * 8.0));
			m_list.SetColumnWidth (6, LVSCW_AUTOSIZE_USEHEADER);
			break;
		case TE_EVENT :
			m_list.SetColumnWidth (0, (int)((double)cx / 100.0 * 58.0));
			m_list.SetColumnWidth (1, (int)((double)cx / 100.0 * 20.0));
			m_list.SetColumnWidth (2, LVSCW_AUTOSIZE_USEHEADER);
			break;
	}
}

/*
** GotoLine :
*/
void CAdamoSEListView::GotoLine () 
{
	LV_ITEM lv;
	stAdamoErrore *pE;
	int n = m_list.GetTopIndex();
	int nLast = n + m_list.GetCountPerPage();
	CString strModule, strLine, strError, strPath;
    char szBuffer[256];

	if (((CMainFrame *) AfxGetMainWnd ())->GetPswLevel () < ePswService)   {
		CProject *pPrj = GETWS ()->GetActiveProject ();
		if (pPrj)   {
			strPath = pPrj->GetProjectDir ();
			for (; n < nLast; n++)   {
				if (m_list.GetItemState (n, LVIS_SELECTED) == LVIS_SELECTED)   {
					lv.iItem = n;
					lv.iSubItem = 0;
					lv.mask = LVIF_PARAM;
					if (m_list.GetItem (&lv))   {
						pE = (stAdamoErrore *) lv.lParam;
						if (pE != NULL && pE->nTypeError == TE_INTERPRETER)   {
							strcpy (szBuffer, pE->strErrore);
							((CMainFrame *) AfxGetMainWnd ())->ParseStrError (szBuffer, &strModule, &strLine, &strError);
							((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strPath + "\\" + strModule, atoi (strLine), true);
						}
					}
				}
			}
		}
	}
}

/*
** DeleteLVParam :
*/
void CAdamoSEListView::DeleteLVParam ()
{
	LV_ITEM lv;
	stAdamoErrore *pE;

	for (int n = 0; n<m_list.GetItemCount (); n++)   {
		lv.iItem = n;
		lv.iSubItem = 0;
		lv.mask = LVIF_PARAM;
		if (m_list.GetItem (&lv))   {
			pE = (stAdamoErrore *) lv.lParam;
			if (pE != NULL)
				delete pE;
		}
	}
}

/*
** ClearAllItems :
*/
void CAdamoSEListView::ClearAllItems ()
{
	DeleteLVParam ();
	m_list.DeleteAllItems ();
}

/*
** OnEraseBkgnd
*/
BOOL CAdamoSEListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CBrush *br;
	CRect rect;
	GetClientRect(rect);
	br = new CBrush (RGB (245, 245, 245));
	pDC->FillRect(&rect,br);
	br->DeleteObject();
	delete br;
	return FALSE;
}

/*
** DrawItem :
*/
void CAdamoSEListCtrl::DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);
    int nItem = lpDrawItemStruct->itemID;
    CImageList* pImageList;
    
    // Save dc state
    int nSavedDC = pDC->SaveDC();
    
    // Get item image and state info
    LV_ITEM lvi;
    lvi.mask = LVIF_IMAGE | LVIF_STATE;
    lvi.iItem = nItem;
    lvi.iSubItem = 0;
    lvi.stateMask = 0xFFFF;     // get all state flags
    GetItem(&lvi);
    
    // Should the item be highlighted
    BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)
        || ( (lvi.state & LVIS_SELECTED)
        && ((GetFocus() == this)
        || (GetStyle() & LVS_SHOWSELALWAYS)
        )
        )
        );
    
    // Get rectangles for drawing
    CRect rcBounds, rcLabel, rcIcon;
    GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
    GetItemRect(nItem, rcLabel, LVIR_LABEL);
    GetItemRect(nItem, rcIcon, LVIR_ICON);
    CRect rcCol( rcBounds ); 
    
    CString sLabel = GetItemText( nItem, 0 );
    
    // Labels are offset by a certain amount  
    // This offset is related to the width of a space character
    int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;
    
    CRect rcHighlight;
    CRect rcWnd;
    int nExt;
    switch( m_nHighlight )    {
		case 0: 
			nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
			rcHighlight = rcLabel;
			if( rcLabel.left + nExt < rcLabel.right )
				rcHighlight.right = rcLabel.left + nExt;
			break;
		case 1:
			rcHighlight = rcBounds;
			rcHighlight.left = rcLabel.left;
			break;
		case 2:
			GetClientRect(&rcWnd);
			rcHighlight = rcBounds;
			rcHighlight.left = rcLabel.left;
			rcHighlight.right = rcWnd.right;
			break;
		default:
			rcHighlight = rcLabel;
    }
    
    // Draw the background color
	bHighlight = false;
	CBrush *br;
	//br = new CBrush(::GetSysColor(COLOR_WINDOW));
	br = new CBrush(m_bckColor);
    pDC->FillRect(rcHighlight, br);
    
    // Set clip region
    rcCol.right = rcCol.left + GetColumnWidth(0);
    CRgn rgn;
    rgn.CreateRectRgnIndirect(&rcCol);
    pDC->SelectClipRgn(&rgn);
    rgn.DeleteObject();
    
    // Draw state icon
    if (lvi.state & LVIS_STATEIMAGEMASK)
    {
        int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
        pImageList = GetImageList(LVSIL_STATE);
        if (pImageList)
        {
            pImageList->Draw(pDC, nImage,
                CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
        }
    }
    
    // Draw normal and overlay icon
    pImageList = GetImageList(LVSIL_SMALL);
    if (pImageList)
    {
        UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
        pImageList->Draw(pDC, lvi.iImage, 
            CPoint(rcIcon.left, rcIcon.top),
            (bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask );
    }
    
	// draw selection 
	if (lvi.state & LVIS_SELECTED)   {
        pDC->FillRect(rcHighlight, &CBrush(m_bckSelectionColor));
		pDC->SetTextColor (m_foreSelectionColor);
	}
	else
		pDC->SetTextColor (m_foreColor);

	// Draw item label - Column 0
    rcLabel.left += offset/2;
    rcLabel.right -= offset;
    pDC->DrawText(sLabel,-1,rcLabel,
        DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP |
        DT_VCENTER | DT_END_ELLIPSIS);
    
    // Draw labels for remaining columns
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH;
    
    if( m_nHighlight == 0 )     // Highlight only first column
    {
        pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
        pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
    }
    
    rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right :
    rcBounds.right;
    rgn.CreateRectRgnIndirect(&rcBounds);
    pDC->SelectClipRgn(&rgn);
    
    for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
    {
        rcCol.left = rcCol.right;
        rcCol.right += lvc.cx;
        
        // Draw the background if needed
        if( m_nHighlight == HIGHLIGHT_NORMAL )
            pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));
		if (lvi.state & LVIS_SELECTED)   {
			pDC->FillRect (rcCol, &CBrush(m_bckSelectionColor)); 
			pDC->SetTextColor (m_foreSelectionColor);
		}
        sLabel = GetItemText(nItem, nColumn);
        if (sLabel.GetLength() == 0)
            continue;
        
        // Get the text justification
        UINT nJustify = DT_LEFT;
        switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
        {
        case LVCFMT_RIGHT:
            nJustify = DT_RIGHT;
            break;
        case LVCFMT_CENTER:
            nJustify = DT_CENTER;
            break;
        default:
            break;
        }
        
        rcLabel = rcCol;
        rcLabel.left += offset;
        rcLabel.right -= offset;
        
        pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
            DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
    }
    
    // Draw focus rectangle if item has focus
    if (lvi.state & LVIS_FOCUSED && (GetFocus() == this))
        pDC->DrawFocusRect(rcHighlight);

    // Restore dc
    pDC->RestoreDC( nSavedDC );
	delete br;
}

/*
** PreSubclassWindow :
*/
void CAdamoSEListCtrl::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();
}

/*
** OnCreate :
*/
int CAdamoSEListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
    CHeaderCtrl* pHeader = NULL;
    pHeader = GetHeaderCtrl();
    if (pHeader != NULL)   {
        VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd)); // m_HeaderCtrl is the new wrapper object
    }
	SetSkin (((CIdeApp *) AfxGetApp ())->GetTheme ());
	return 0;
}

/*
** SetSkin :
*/
LRESULT CAdamoSEListCtrl::SetSkin (AppTheme nSkin)
{
	switch (nSkin)   {
		case themeVS2010 :
			m_bckColor = RGB(0xff,0xff,0xff);
			m_foreColor = RGB(0x00,0x00,0x00);
			m_foreSelectionColor = RGB(0xff,0xff,0xff);
			m_bckSelectionColor = RGB(0x32,0x98,0xfe);
			break;
		case themeVS2012Light :
			m_bckColor = RGB(0xf5,0xf5,0xf5);
			m_foreColor = RGB(0x00,0x00,0x00);
			m_foreSelectionColor = RGB(0xff,0xff,0xff);
			m_bckSelectionColor = RGB(0x32,0x98,0xfe);
			break;
		case themeVS2012Dark :
			m_bckColor = RGB(0x00,0x00,0x00);
			m_foreColor = RGB(0xff,0xff,0xff);
			m_foreSelectionColor = RGB(0x00,0x00,0x00);
			m_bckSelectionColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2015Light :
			m_bckColor = RGB(0xf5,0xf5,0xf5);
			m_foreColor = RGB(0x00,0x00,0x00);
			m_foreSelectionColor = RGB(0xff,0xff,0xff);
			m_bckSelectionColor = RGB(0x32,0x98,0xfe);
			break;
		case themeVS2015Dark :
			m_bckColor = RGB(0x00,0x00,0x00);
			m_foreColor = RGB(0xff,0xff,0xff);
			m_foreSelectionColor = RGB(0x00,0x00,0x00);
			m_bckSelectionColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2015Blue :
			m_bckColor = RGB(0xff,0xff,0xff);
			m_foreColor = RGB(0x00,0x00,0x00);
			m_foreSelectionColor = RGB(0xff,0xff,0xff);
			m_bckSelectionColor = RGB(0x32,0x98,0xfe);
			break;
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CAdamoSEHeaderListCtrl, CHeaderCtrl)
    //{{AFX_MSG_MAP(MyCustomCListCtrl)
    //}}AFX_MSG_MAP
    // ON_WM_DRAWITEM()                             /* WM_DRAWITEM NON-AVAILABLE */
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()

CAdamoSEHeaderListCtrl::CAdamoSEHeaderListCtrl () : m_bFirstTime (true)
{
}

/*
** SetSkin :
*/
LRESULT CAdamoSEHeaderListCtrl::SetSkin (AppTheme nSkin)
{
	switch (nSkin)   {
		case themeVS2010 :
			m_bckColor = RGB(0x4a,0x61,0x84);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2012Light :
			m_bckColor = RGB(0xf5,0xf5,0xf5);
			m_foreColor = RGB(0x00,0x00,0x00);
			break;
		case themeVS2012Dark :
			m_bckColor = RGB(0x00,0x00,0x00);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2015Light :
			m_bckColor = RGB(0xf5,0xf5,0xf5);
			m_foreColor = RGB(0x00,0x00,0x00);
			break;
		case themeVS2015Dark :
			m_bckColor = RGB(0x00,0x00,0x00);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
		case themeVS2015Blue :
			m_bckColor = RGB(0x4a,0x61,0x84);
			m_foreColor = RGB(0xff,0xff,0xff);
			break;
	}
	return 0;
}

void CAdamoSEHeaderListCtrl::OnNMCustomdraw (NMHDR *pNMHDR, LRESULT *pResult)
{
	 LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = CDRF_DODEFAULT;

	if (m_bFirstTime)   {
		SetSkin (((CIdeApp *) AfxGetApp ())->GetTheme ());
		m_bFirstTime = false;
	}
    if (pNMCD->dwDrawStage == CDDS_PREPAINT)        {
        CDC* pDC = CDC::FromHandle(pNMCD->hdc);
        CRect rect(0, 0, 0, 0);
        GetClientRect(&rect);
        pDC->FillSolidRect(&rect, m_bckColor);
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else 
		if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)        {
			CImageList* pimgList = GetImageList ();
			CRect rc (pNMCD->rc);
			int nCount = pimgList->GetImageCount ();
			HDITEM hditem;
			TCHAR buffer[MAX_PATH] = { 0 };
			SecureZeroMemory(&hditem, sizeof(HDITEM));
			hditem.mask = HDI_TEXT | HDI_IMAGE;
			hditem.pszText = buffer;
			hditem.cchTextMax = MAX_PATH;
			GetItem(pNMCD->dwItemSpec, &hditem);
			CDC* pDC = CDC::FromHandle(pNMCD->hdc);
			pDC->SetTextColor(m_foreColor);
			pDC->SetBkColor(m_bckColor);
			CString str(buffer);
			pimgList->DrawEx (pDC, hditem.iImage, CPoint (rc.left, rc.top + 4), CSize (16, 16), CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
			rc.left += 20;
			rc.top += 4;
			pDC->DrawText(str, rc, DT_VCENTER | DT_TOP);
			*pResult = CDRF_SKIPDEFAULT;
        }
}

int CAdamoSEHeaderListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHeaderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetSkin (((CIdeApp *) AfxGetApp ())->GetTheme ());
	return 0;
}