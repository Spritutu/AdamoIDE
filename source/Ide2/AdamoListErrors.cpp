// KosListErrors.cpp : implementation file
//

#include "stdafx.h"
#include "mainframe.h"
#include "ListViewErrorsCustomColor.h"
#include "AdamoListErrors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CAdamoListAllErrors, CView)
BEGIN_MESSAGE_MAP(CAdamoListAllErrors, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
    ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** construction / destruction **/
CAdamoListAllErrors::CAdamoListAllErrors () : m_nItem (0), m_lviml (NULL), m_hciml (NULL), m_bmpHdrErrorType (NULL), m_bmpHdrError (NULL), m_bmpHdrTask (NULL),
	m_bmpHdrDate (NULL), m_bmpHdrTime (NULL), m_bmpHdrModule (NULL), m_bInitialized (false)
{
}

CAdamoListAllErrors::~CAdamoListAllErrors ()
{
    delete m_lviml;
    delete m_hciml;
    delete m_bmpHdrErrorType;
    delete m_bmpHdrDate;
    delete m_bmpHdrTime;
    delete m_bmpHdrError;
    delete m_bmpHdrTask;
	delete m_bmpHdrModule;
}

/*
** AddMessage :
*/
void CAdamoListAllErrors::AddMessage (CString str, CString strModule, int nPriority, time_t dt)
{
	FillMessage (str, strModule, nPriority, dt, 0);
	m_nItem++;
}

/*
** AddError :
*/
void CAdamoListAllErrors::AddError (stThiraErrore *ae)
{
	/* aggiungiamo un errore */
	FillError (ae, -1);
	m_nItem++;
}

/*
** FillAll :
*/
void CAdamoListAllErrors::FillError (stThiraErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	/* settiamo l'item, se diverso da 0, accodiamo */
	if (nItem != 0)
		nItem = GetListCtrl ().GetItemCount ();
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
		lvi.lParam = (LPARAM) new stThiraErrore;
		memcpy ((void *)lvi.lParam, pE, sizeof (stThiraErrore));
	}
	else
		lvi.lParam = NULL;
    GetListCtrl ().InsertItem (&lvi);
	
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
    GetListCtrl ().SetItem (&lvi);

	/* module */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    strcpy (szBuffer, pE->m_strModule);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    GetListCtrl ().SetItem (&lvi);
	pE->m_strModule.ReleaseBuffer ();

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    GetListCtrl ().SetItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    GetListCtrl ().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    GetListCtrl ().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** FillMessage :
*/
void CAdamoListAllErrors::FillMessage (CString str, CString strModule, int nPriority, time_t dt, int nItem)
{
    COleDateTime odt (dt);
    LVITEM lvi;
    char szBuffer[256];

    /* tipo di errore */
    memset (&lvi, 0, sizeof (LVITEM));
	strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_26));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
	lvi.iImage = 5;
    GetListCtrl ().InsertItem (&lvi);

	/* errore */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText = str.GetBuffer ();
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);

	/* module */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    strcpy (szBuffer, strModule);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    GetListCtrl ().SetItem (&lvi);

	/* task */
	szBuffer[0] = '\0';
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

void CAdamoListAllErrors::CreateColumnHeader ()
{
    CRect rect;
    GetListCtrl().GetWindowRect(&rect);
	GetListCtrl().InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_11), LVCFMT_LEFT,0, 0);
	GetListCtrl().InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_4),  LVCFMT_LEFT, 0, 1);
	GetListCtrl().InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_27), LVCFMT_LEFT,0, 2);
	GetListCtrl().InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_7),  LVCFMT_LEFT, 0, 3);
	GetListCtrl().InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_1),  LVCFMT_LEFT, 0, 4);
	GetListCtrl().InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_2),  LVCFMT_LEFT, 0, 5);
    CHeaderCtrl* hc=GetListCtrl().GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrErrorType=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
	m_bmpHdrModule=new CBitmap;
    m_bmpHdrErrorType->LoadBitmap(IDB_CAPTYPEDEVICE);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrModule->LoadBitmap(IDB_CAPMODULES);
	m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrErrorType, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrModule, RGB(0, 128,128));
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
}

/*
** OnSize :
*/
void CAdamoListAllErrors::OnSize(UINT nType, int cx, int cy)
{
	int nCountPerPage = GetListCtrl ().GetCountPerPage ();
	int nItemCount = GetListCtrl ().GetItemCount ();
	bool bIsVisible = nItemCount > nCountPerPage;
	int nMargin = bIsVisible ? 18 : 0;

	CView::OnSize(nType, cx, cy);
	GetListCtrl ().MoveWindow (0, 0, cx, cy);
    GetListCtrl ().SetColumnWidth (0, (int)((double)cx / 100.0 * 14.0));
    GetListCtrl ().SetColumnWidth (1, (int)((double)cx / 100.0 * 46.0)+3-nMargin);
    GetListCtrl ().SetColumnWidth (2, (int)((double)cx / 100.0 * 10.0));
	GetListCtrl ().SetColumnWidth (3, (int)((double)cx / 100.0 * 10.0));
	GetListCtrl ().SetColumnWidth (4, (int)((double)cx / 100.0 * 10.0));
	GetListCtrl ().SetColumnWidth (5, (int)((double)cx / 100.0 * 10.0));
}

/*
** OnDraw :
*/
void CAdamoListAllErrors::OnDraw(CDC* pDC)
{
}

/*
** CreateImageList :
*/
void CAdamoListAllErrors::CreateImageList ()
{
    m_lviml=new CImageList;
	m_lviml->Create(MAKEINTRESOURCE(IDB_BMP_LIST_ERRORS), 16, 1, RGB(0,128,128));
    GetListCtrl ().SetImageList (m_lviml, LVSIL_SMALL);
}

/*
** OnInitialUpdate :
*/
void CAdamoListAllErrors::OnInitialUpdate ()
{
	CView::OnInitialUpdate();
	if (!m_bInitialized)   {
		DWORD dw=GetListCtrl().GetExtendedStyle ();
		dw|=LVS_EX_FULLROWSELECT;
		GetListCtrl().SetExtendedStyle (dw);
		GetListCtrl().GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().ShowScrollBar (SB_VERT);
		m_bInitialized = true;
	}
}

/*
** OnCreate :
*/
int CAdamoListAllErrors::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, 100);
	if (b)   {
		CreateImageList ();
		CreateColumnHeader ();
	}
	return b != 0;
}

/*
** DeleteErrors :
*/
void CAdamoListAllErrors::DeleteErrors ()
{
	m_list.DeleteAllItems ();
}

IMPLEMENT_DYNCREATE(CAdamoListInterpreterErrors, CView)
BEGIN_MESSAGE_MAP(CAdamoListInterpreterErrors, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
    ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** construction / destruction **/
CAdamoListInterpreterErrors::CAdamoListInterpreterErrors () : m_nItem (0), m_hciml (NULL), m_lviml (NULL), m_bmpHdrIdentifier (NULL), m_bmpHdrError (NULL), m_bmpHdrTask (NULL),
	m_bmpHdrPath (NULL), m_bmpHdrLine (NULL), m_bmpHdrDate (NULL), m_bmpHdrTime (NULL), m_bmpHdrModule (NULL), m_bInitialized (false)
{
}

CAdamoListInterpreterErrors::~CAdamoListInterpreterErrors ()
{
    delete m_lviml;
	delete m_hciml;
    delete m_bmpHdrTime;
    delete m_bmpHdrDate;
    delete m_bmpHdrIdentifier;
    delete m_bmpHdrError;
    delete m_bmpHdrTask;
    delete m_bmpHdrPath;
    delete m_bmpHdrLine;
	delete m_bmpHdrModule;
}

/*
** LoadHeaderInterpreter :
*/
void CAdamoListInterpreterErrors::CreateColumnHeader ()
{
    CRect rect;

    GetListCtrl().GetWindowRect(&rect);
	GetListCtrl().InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT, 0, 0);
	GetListCtrl().InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_27), LVCFMT_LEFT,0, 1);
	GetListCtrl().InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT, 0, 2);
	GetListCtrl().InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_8), LVCFMT_LEFT, 0, 3);
	GetListCtrl().InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_9), LVCFMT_LEFT, 0, 4);
	GetListCtrl().InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT, 0, 5);
	GetListCtrl().InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT, 0, 6);
	GetListCtrl().InsertColumn(7, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT, 0, 7);
    CHeaderCtrl* hc=GetListCtrl().GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrModule=new CBitmap;
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
    m_bmpHdrModule->LoadBitmap(IDB_CAPMODULES);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrModule, RGB(0, 128,128));
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
    hc->GetItem(7, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=7;
    hc->SetItem(7, &hd);
}

/*
** FillError :
*/
void CAdamoListInterpreterErrors::FillError (stThiraErrore* pE, int nItem)
{
	COleDateTime odt (pE->dt);
    LVITEM lvi;
	CString strModule, strLine, strError;
    char szBuffer[256];

	/* settiamo l'item, se diverso da 0, accodiamo */
	if (nItem != 0)
		nItem = GetListCtrl ().GetItemCount ();
	strcpy (szBuffer, pE->strErrore);
    ((CMainFrame*) AfxGetMainWnd ())->ParseStrError (szBuffer, &strModule, &strLine, &strError);
	
	/* errore */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.iImage = 1;
    lvi.pszText=strError.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().InsertItem (&lvi);
    strError.ReleaseBuffer ();

	/* Modulo */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=pE->m_strModule.GetBuffer ();
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    pE->m_strModule.ReleaseBuffer ();

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);

	/* module name */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=strModule.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    strModule.ReleaseBuffer ();

	/* linea */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=strLine.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    strLine.ReleaseBuffer ();

    /* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    GetListCtrl ().SetItem (&lvi);

	/* ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=7;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** AddMessage :
*/
void CAdamoListInterpreterErrors::AddMessage (CString str, CString strModule, int nPriority, DATE dt)
{
}

/*
** AddError :
*/
void CAdamoListInterpreterErrors::AddError (stThiraErrore *ae)
{
	/* aggiungiamo un errore */
	FillError (ae, -1);
	m_nItem++;
}

/*
** OnSize :
*/
void CAdamoListInterpreterErrors::OnSize(UINT nType, int cx, int cy)
{
	int nCountPerPage = GetListCtrl ().GetCountPerPage ();
	int nItemCount = GetListCtrl ().GetItemCount ();
	bool bIsVisible = nItemCount > nCountPerPage;
	int nMargin = bIsVisible ? 18 : 0;

	CView::OnSize(nType, cx, cy);
	GetListCtrl ().MoveWindow (0, 0, cx, cy);
    GetListCtrl ().SetColumnWidth (0, (int)((double)cx / 100.0 * 30.0)+2-nMargin);
    GetListCtrl ().SetColumnWidth (1, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (2, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (3, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (4, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (5, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (6, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (7, (int)((double)cx / 100.0 * 10.0));
}

/*
** OnDraw :
*/
void CAdamoListInterpreterErrors::OnDraw (CDC* pDC)
{
}

/*
** CreateImageList :
*/
void CAdamoListInterpreterErrors::CreateImageList ()
{
    m_lviml=new CImageList;
	m_lviml->Create(MAKEINTRESOURCE(IDB_BMP_LIST_ERRORS), 16, 1, RGB(0,128,128));
    GetListCtrl ().SetImageList (m_lviml, LVSIL_SMALL);
}

/*
** OnInitialUpdate :
*/
void CAdamoListInterpreterErrors::OnInitialUpdate ()
{
	CView::OnInitialUpdate();
	if (!m_bInitialized)   {
		DWORD dw=GetListCtrl().GetExtendedStyle ();
		dw|=LVS_EX_FULLROWSELECT;
		GetListCtrl().SetExtendedStyle (dw);
		GetListCtrl().GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().ShowScrollBar (SB_VERT);
		m_bInitialized = true;
	}
}

/*
** OnCreate :
*/
int CAdamoListInterpreterErrors::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, 101);
	if (b)   {
		CreateImageList ();
		CreateColumnHeader ();
	}
	return b != 0;
}

/*
** DeleteErrors :
*/
void CAdamoListInterpreterErrors::DeleteErrors ()
{
	m_list.DeleteAllItems ();
}

IMPLEMENT_DYNCREATE(CAdamoListPLCErrors, CView)
BEGIN_MESSAGE_MAP(CAdamoListPLCErrors, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
    ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** construction / destruction **/
CAdamoListPLCErrors::CAdamoListPLCErrors () : m_nItem (0), m_lviml (NULL), m_hciml (NULL), m_bmpHdrError (NULL), m_bmpHdrTask (NULL),
	m_bmpHdrDate (NULL), m_bmpHdrTime (NULL), m_bmpHdrIdentifier (NULL), m_bmpHdrPriority (NULL), m_bmpHdrDisk (NULL), m_bmpHdrModule (NULL), m_bInitialized (false)
{
}

CAdamoListPLCErrors::~CAdamoListPLCErrors ()
{
    delete m_lviml;
    delete m_hciml;
    delete m_bmpHdrDate;
    delete m_bmpHdrTime;
    delete m_bmpHdrError;
    delete m_bmpHdrTask;
    delete m_bmpHdrIdentifier;
    delete m_bmpHdrPriority;
    delete m_bmpHdrDisk;
	delete m_bmpHdrModule;
}

/*
** AddMessage :
*/
void CAdamoListPLCErrors::AddMessage (CString str, CString strModule, int nPriority, DATE dt)
{
}

/*
** AddError :
*/
void CAdamoListPLCErrors::AddError (stThiraErrore *ae)
{
	/* aggiungiamo un errore */
	FillError (ae, -1);
	m_nItem++;
}

/*
** FillAll :
*/
void CAdamoListPLCErrors::FillError (stThiraErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	/* settiamo l'item, se diverso da 0, accodiamo */
	if (nItem != 0)
		nItem = GetListCtrl ().GetItemCount ();
    /* Errore */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, pE->strErrore);
    lvi.mask=LVIF_TEXT | LVIF_IMAGE;;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.iImage = 2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().InsertItem (&lvi);

	/* Modulo */
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=pE->m_strModule.GetBuffer ();
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
	pE->m_strModule.ReleaseBuffer ();

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);

    /* priorita */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nTypeDevice);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);

    /* disk */
    memset (&lvi, 0, sizeof (LVITEM));
    if (pE->nDevice)
        strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_12));
    else
        strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_13));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);

    /* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXIDENTIFIER+1;
    GetListCtrl ().SetItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=7;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

void CAdamoListPLCErrors::CreateColumnHeader ()
{
    CRect rect;

    GetListCtrl().GetWindowRect(&rect);
	GetListCtrl().InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT, 0, 0);
	GetListCtrl().InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_27), LVCFMT_LEFT,0, 1);
	GetListCtrl().InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT, 0, 2);
	GetListCtrl().InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_5), LVCFMT_LEFT, 0, 3);
	GetListCtrl().InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_6), LVCFMT_LEFT, 0, 4);
	GetListCtrl().InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT, 0, 5);
	GetListCtrl().InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT, 0, 6);
	GetListCtrl().InsertColumn(7, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT, 0, 7);
    CHeaderCtrl* hc=GetListCtrl().GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrPriority=new CBitmap;
    m_bmpHdrDisk=new CBitmap;
    m_bmpHdrModule=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrPriority->LoadBitmap(IDB_CAPPRIORITY);
    m_bmpHdrDisk->LoadBitmap(IDB_CAPDISK);
    m_bmpHdrModule->LoadBitmap(IDB_CAPMODULES);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrModule, RGB(0, 128,128));
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
    hc->GetItem(7, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=7;
    hc->SetItem(7, &hd);
}

/*
** OnSize :
*/
void CAdamoListPLCErrors::OnSize(UINT nType, int cx, int cy)
{
	int nCountPerPage = GetListCtrl ().GetCountPerPage ();
	int nItemCount = GetListCtrl ().GetItemCount ();
	bool bIsVisible = nItemCount > nCountPerPage;
	int nMargin = bIsVisible ? 18 : 0;

	CView::OnSize(nType, cx, cy);
	GetListCtrl ().MoveWindow (0, 0, cx, cy);
    GetListCtrl ().SetColumnWidth (0, (int)((double)cx / 100.0 * 30.0)+2-nMargin);
    GetListCtrl ().SetColumnWidth (1, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (2, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (3, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (4, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (5, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (6, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (7, (int)((double)cx / 100.0 * 10.0));
}

/*
** OnDraw :
*/
void CAdamoListPLCErrors::OnDraw(CDC* pDC)
{
}

/*
** CreateImageList :
*/
void CAdamoListPLCErrors::CreateImageList ()
{
    m_lviml=new CImageList;
	m_lviml->Create(MAKEINTRESOURCE(IDB_BMP_LIST_ERRORS), 16, 1, RGB(0,128,128));
    GetListCtrl ().SetImageList (m_lviml, LVSIL_SMALL);
}

/*
** OnInitialUpdate :
*/
void CAdamoListPLCErrors::OnInitialUpdate ()
{
	CView::OnInitialUpdate();
	if (!m_bInitialized)   {
		DWORD dw=GetListCtrl().GetExtendedStyle ();
		dw|=LVS_EX_FULLROWSELECT;
		GetListCtrl().SetExtendedStyle (dw);
		GetListCtrl().GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().ShowScrollBar (SB_VERT);
		m_bInitialized = true;
	}
}

/*
** OnCreate :
*/
int CAdamoListPLCErrors::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, 102);
	if (b)   {
		CreateImageList ();
		CreateColumnHeader ();
	}
	return b != 0;
}

/*
** DeleteErrors :
*/
void CAdamoListPLCErrors::DeleteErrors ()
{
	m_list.DeleteAllItems ();
}

IMPLEMENT_DYNCREATE(CAdamoListISOErrors, CView)
BEGIN_MESSAGE_MAP(CAdamoListISOErrors, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
    ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** construction / destruction **/
CAdamoListISOErrors::CAdamoListISOErrors () : m_nItem (0), m_lviml (NULL), m_hciml (NULL), m_bmpHdrError (NULL), m_bmpHdrTask (NULL),
	m_bmpHdrDate (NULL), m_bmpHdrTime (NULL), m_bmpHdrIdentifier (NULL), m_bmpHdrPath (NULL), m_bmpHdrLine (NULL), m_bmpHdrModule (NULL), m_bInitialized (false)
{
}

CAdamoListISOErrors::~CAdamoListISOErrors ()
{
    delete m_lviml;
    delete m_hciml;
    delete m_bmpHdrDate;
    delete m_bmpHdrTime;
    delete m_bmpHdrError;
    delete m_bmpHdrTask;
    delete m_bmpHdrIdentifier;
    delete m_bmpHdrPath;
    delete m_bmpHdrLine;
	delete m_bmpHdrModule;
}

/*
** AddMessage :
*/
void CAdamoListISOErrors::AddMessage (CString str, CString strModule, int nPriority, DATE dt)
{
}

/*
** AddError :
*/
void CAdamoListISOErrors::AddError (stThiraErrore *ae)
{
	/* aggiungiamo un errore */
	FillError (ae, -1);
	m_nItem++;
}

/*
** FillAll :
*/
void CAdamoListISOErrors::FillError (stThiraErrore* pE, int nItem)
{
    COleDateTime odt (pE->dt);
    LVITEM lvi;
	CString strModule, strLine, strError;
	char szBuffer[256];

	/* settiamo l'item, se diverso da 0, accodiamo */
	if (nItem != 0)
		nItem = GetListCtrl ().GetItemCount ();
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
    GetListCtrl().InsertItem (&lvi);
    strError.ReleaseBuffer ();

	/* Modulo */
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=pE->m_strModule.GetBuffer ();
    lvi.cchTextMax=256;
    GetListCtrl().SetItem (&lvi);
	pE->m_strModule.ReleaseBuffer ();

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=256;
    GetListCtrl().SetItem (&lvi);
	
	/* module name */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=strModule.GetBuffer(64);
    lvi.cchTextMax=64;
    GetListCtrl().SetItem (&lvi);
    strModule.ReleaseBuffer ();

	/* linea */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=strLine.GetBuffer(64);
    lvi.cchTextMax=64;
    GetListCtrl().SetItem (&lvi);
    strLine.ReleaseBuffer ();

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    GetListCtrl ().SetItem (&lvi);

	/* ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=64;
    GetListCtrl().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=7;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=64;
    GetListCtrl().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** CreateColumnHeader :
*/
void CAdamoListISOErrors::CreateColumnHeader ()
{
    CRect rect;

    GetListCtrl().GetWindowRect(&rect);
	GetListCtrl().InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT, 0, 0);
	GetListCtrl().InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_27), LVCFMT_LEFT,0, 1);
	GetListCtrl().InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT, 0, 2);
	GetListCtrl().InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_8), LVCFMT_LEFT, 0, 3);
	GetListCtrl().InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_9), LVCFMT_LEFT, 0, 4);
	GetListCtrl().InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT, 0, 5);
	GetListCtrl().InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT, 0, 6);
	GetListCtrl().InsertColumn(7, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT, 0, 7);
    CHeaderCtrl* hc=GetListCtrl().GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrPath=new CBitmap;
    m_bmpHdrLine=new CBitmap;
    m_bmpHdrModule=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrPath->LoadBitmap(IDB_CAPPATH);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrLine->LoadBitmap(IDB_CAPLINE);
    m_bmpHdrModule->LoadBitmap(IDB_CAPMODULES);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrModule, RGB(0, 128,128));
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
    hc->GetItem(7, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=7;
    hc->SetItem(7, &hd);
}

/*
** OnSize :
*/
void CAdamoListISOErrors::OnSize(UINT nType, int cx, int cy)
{
	int nCountPerPage = GetListCtrl ().GetCountPerPage ();
	int nItemCount = GetListCtrl ().GetItemCount ();
	bool bIsVisible = nItemCount > nCountPerPage;
	int nMargin = bIsVisible ? 18 : 0;

	CView::OnSize(nType, cx, cy);
	GetListCtrl ().MoveWindow (0, 0, cx, cy);
    GetListCtrl ().SetColumnWidth (0, (int)((double)cx / 100.0 * 30.0)+2-nMargin);
    GetListCtrl ().SetColumnWidth (1, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (2, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (3, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (4, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (5, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (6, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (7, (int)((double)cx / 100.0 * 10.0));
}

/*
** OnDraw :
*/
void CAdamoListISOErrors::OnDraw(CDC* pDC)
{
}

/*
** CreateImageList :
*/
void CAdamoListISOErrors::CreateImageList ()
{
    m_lviml=new CImageList;
	m_lviml->Create(MAKEINTRESOURCE(IDB_BMP_LIST_ERRORS), 16, 1, RGB(0,128,128));
    GetListCtrl ().SetImageList (m_lviml, LVSIL_SMALL);
}

/*
** OnInitialUpdate :
*/
void CAdamoListISOErrors::OnInitialUpdate ()
{
	CView::OnInitialUpdate();
	if (!m_bInitialized)   {
		DWORD dw=GetListCtrl().GetExtendedStyle ();
		dw|=LVS_EX_FULLROWSELECT;
		GetListCtrl().SetExtendedStyle (dw);
		GetListCtrl().GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().ShowScrollBar (SB_VERT);
		m_bInitialized = true;
	}
}

/*
** OnCreate :
*/
int CAdamoListISOErrors::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, 103);
	if (b)   {
		CreateImageList ();
		CreateColumnHeader ();
	}
	return b != 0;
}

/*
** DeleteErrors :
*/
void CAdamoListISOErrors::DeleteErrors ()
{
	m_list.DeleteAllItems ();
}

IMPLEMENT_DYNCREATE(CAdamoListHardwareErrors, CView)
BEGIN_MESSAGE_MAP(CAdamoListHardwareErrors, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
    ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** construction / destruction **/
CAdamoListHardwareErrors::CAdamoListHardwareErrors () : m_nItem (0), m_lviml (NULL), m_hciml (NULL), m_bmpHdrError (NULL), m_bmpHdrTask (NULL),
	m_bmpHdrDate (NULL), m_bmpHdrTime (NULL), m_bmpHdrIdentifier (NULL), m_bmpHdrDeviceType (NULL), m_bmpHdrDevice (NULL), m_bmpHdrModule (NULL), m_bInitialized (false),
	m_df (NULL)
{
}

CAdamoListHardwareErrors::~CAdamoListHardwareErrors ()
{
    delete m_lviml;
    delete m_hciml;
    delete m_bmpHdrDate;
    delete m_bmpHdrTime;
    delete m_bmpHdrError;
    delete m_bmpHdrTask;
    delete m_bmpHdrIdentifier;
    delete m_bmpHdrDevice;
    delete m_bmpHdrDeviceType;
	delete m_bmpHdrModule;
}

/*
** AddMessage :
*/
void CAdamoListHardwareErrors::AddMessage (CString str, CString strModule, int nPriority, DATE dt)
{
}

/*
** AddError :
*/
void CAdamoListHardwareErrors::AddError (stThiraErrore *ae)
{
	/* aggiungiamo un errore */
	OpenDatiFile ();
	FillError (ae, -1);
	m_nItem++;
}

/*
** FillAll :
*/
void CAdamoListHardwareErrors::FillError (stThiraErrore* pE, int nItem)
{
	COleDateTime odt (pE->dt);
    LVITEM lvi;
    char szBuffer[256];

	/* settiamo l'item, se diverso da 0, accodiamo */
	if (nItem != 0)
		nItem = GetListCtrl ().GetItemCount ();
	/* Errore */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, pE->strErrore);
    lvi.mask=LVIF_TEXT | LVIF_IMAGE;
    lvi.iItem=nItem;
    lvi.iSubItem=0;
	lvi.iImage = 0;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().InsertItem (&lvi);

	/* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", pE->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);

	/* Module */
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=pE->m_strModule.GetBuffer ();
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
	pE->m_strModule.ReleaseBuffer ();

	/* task */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    strcpy (szBuffer, pE->szTaskName);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    GetListCtrl ().SetItem (&lvi);

	/* nome del dispositivo */
    memset (&lvi, 0, sizeof (LVITEM));
	szBuffer[0] = '\0';
    if (pE->nTypeDevice<HARDWARE_ERROR)
		if (m_df)
			m_df->GetDeviceName (pE->nDevice, pE->nTypeDevice, szBuffer);
		else;
    else
        GetDeviceErrorString (pE->nDevice, pE->nTypeDevice, szBuffer);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);

	/* tipo del dispositivo */
    memset (&lvi, 0, sizeof (LVITEM));
    GetTypeDeviceErrorString (pE->nTypeDevice, szBuffer);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=5;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);

	/* ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=6;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=7;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

/*
** CreateColumnHeader :
*/
void CAdamoListHardwareErrors::CreateColumnHeader ()
{
    CRect rect;

    GetListCtrl().GetWindowRect(&rect);
	GetListCtrl().InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT, 0, 0);
	GetListCtrl().InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_27), LVCFMT_LEFT,0, 1);
	GetListCtrl().InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_3), LVCFMT_LEFT, 0, 2);
	GetListCtrl().InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_7), LVCFMT_LEFT, 0, 3);
	GetListCtrl().InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_10), LVCFMT_LEFT,0, 4);
	GetListCtrl().InsertColumn(5, LOADSTRING (IDS_ADAMOSELISTVIEW_11), LVCFMT_LEFT,0, 5);
	GetListCtrl().InsertColumn(6, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT, 0, 6);
	GetListCtrl().InsertColumn(7, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT, 0, 7);
    CHeaderCtrl* hc=GetListCtrl().GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrDevice=new CBitmap;
    m_bmpHdrDeviceType=new CBitmap;
    m_bmpHdrModule=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrDevice->LoadBitmap(IDB_CAPDEVICE);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrDeviceType->LoadBitmap(IDB_CAPTYPEDEVICE);
    m_bmpHdrModule->LoadBitmap(IDB_CAPMODULES);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrModule, RGB(0, 128,128));
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
    hc->GetItem(7, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=7;
    hc->SetItem(7, &hd);
}

/*
** OnSize :
*/
void CAdamoListHardwareErrors::OnSize(UINT nType, int cx, int cy)
{
	int nCountPerPage = GetListCtrl ().GetCountPerPage ();
	int nItemCount = GetListCtrl ().GetItemCount ();
	bool bIsVisible = nItemCount > nCountPerPage;
	int nMargin = bIsVisible ? 18 : 0;

	CView::OnSize(nType, cx, cy);
	GetListCtrl ().MoveWindow (0, 0, cx, cy);
    GetListCtrl ().SetColumnWidth (0, (int)((double)cx / 100.0 * 30.0)+2-nMargin);
    GetListCtrl ().SetColumnWidth (1, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (2, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (3, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (4, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (5, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (6, (int)((double)cx / 100.0 * 10.0));
    GetListCtrl ().SetColumnWidth (7, (int)((double)cx / 100.0 * 10.0));
}

/*
** OnDraw :
*/
void CAdamoListHardwareErrors::OnDraw(CDC* pDC)
{
}

/*
** CreateImageList :
*/
void CAdamoListHardwareErrors::CreateImageList ()
{
    m_lviml=new CImageList;
	m_lviml->Create(MAKEINTRESOURCE(IDB_BMP_LIST_ERRORS), 16, 1, RGB(0,128,128));
    GetListCtrl ().SetImageList (m_lviml, LVSIL_SMALL);
}

/*
** OnInitialUpdate :
*/
void CAdamoListHardwareErrors::OnInitialUpdate ()
{
	CView::OnInitialUpdate();
	if (!m_bInitialized)   {
		DWORD dw=GetListCtrl().GetExtendedStyle ();
		dw|=LVS_EX_FULLROWSELECT;
		GetListCtrl().SetExtendedStyle (dw);
		GetListCtrl().GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().ShowScrollBar (SB_VERT);
		m_bInitialized = true;
	}
}

/*
** OnCreate :
*/
int CAdamoListHardwareErrors::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, 104);
	if (b)   {
		CreateImageList ();
		CreateColumnHeader ();
	}
	return b != 0;
}

/*
** OpenDatiFile :
*/
void CAdamoListHardwareErrors::OpenDatiFile ()
{
	if (!m_df)   {
		CString strPathName = GETWS ()->GetActiveProject ()->GetParamsPathName ();
		m_df=new CAdamoDatiFile;
		if (m_df)   {
			m_df->Open (strPathName, TRUE);
			m_df->ReadData ();
			m_df->CFile::Close ();
		}
	}
}

/*
** GetTypeDeviceErrorString :
*/
void CAdamoListHardwareErrors::GetTypeDeviceErrorString (int nTypeDevice, char* szBuffer)
{
    CString str;
    if (nTypeDevice<HARDWARE_ERROR)
        str=GetDeviceString (nTypeDevice);
    else
        switch (nTypeDevice)   {
            case HARDWARE_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_16);
                break;
            case RECEIVER_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_17);
                break;
            case OMIKRON_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_19);
                break;
            case SSCNET_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_21);
                break;
        }
    strcpy (szBuffer, str);
}

/*
** GetDeviceString :
*/
CString CAdamoListHardwareErrors::GetDeviceString (int nTypeDevice)
{
    CString str;
    switch (nTypeDevice)   {
        case enInputDigitale :
            str=LOADSTRING (IDS_ADAMODEVICE_1);
            break;
        case enOutputDigitale :
            str=LOADSTRING (IDS_ADAMODEVICE_2);
            break;
        case enInputAnalogico :
            str=LOADSTRING (IDS_ADAMODEVICE_3);
            break;
        case enOutputAnalogico :
            str=LOADSTRING (IDS_ADAMODEVICE_4);
            break;
        case enAsse :
            str=LOADSTRING (IDS_ADAMODEVICE_5);
            break;
        case enTimer :
            str=LOADSTRING (IDS_ADAMODEVICE_6);
            break;
        case enCounter :
            str=LOADSTRING (IDS_ADAMODEVICE_7);
            break;
        case enGenericCan :
            str=LOADSTRING (IDS_ADAMODEVICE_8);
            break;
    }
    return str; 
}                        

/*
** GetDeviceErrorString :
*/
void CAdamoListHardwareErrors::GetDeviceErrorString (int nDevice, int nTypeDevice, char* szBuffer)
{
    switch (nTypeDevice)   {
        case HARDWARE_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_16));
            break;
        case RECEIVER_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_17), nDevice);
            break;
        case OMIKRON_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_19));
            break;
        case SSCNET_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_21), nDevice);
            break;
    }
}

/*
** DeleteErrors :
*/
void CAdamoListHardwareErrors::DeleteErrors ()
{
	m_list.DeleteAllItems ();
}

IMPLEMENT_DYNCREATE(CAdamoListMessages, CView)
BEGIN_MESSAGE_MAP(CAdamoListMessages, CView)
	//{{AFX_MSG_MAP(CAdamoSEListView)
    ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/** construction / destruction **/
CAdamoListMessages::CAdamoListMessages () : m_nItem (0), m_lviml (NULL), m_hciml (NULL), m_bmpHdrError (NULL),
	m_bmpHdrDate (NULL), m_bmpHdrTime (NULL), m_bmpHdrPriority (NULL), m_bmpHdrModule (NULL),m_bInitialized (false)
{
}

CAdamoListMessages::~CAdamoListMessages ()
{
	delete m_lviml;
    delete m_hciml;
    delete m_bmpHdrDate;
    delete m_bmpHdrTime;
    delete m_bmpHdrError;
    delete m_bmpHdrPriority;
	delete m_bmpHdrModule;
}

/*
** AddMessage :
*/
void CAdamoListMessages::AddMessage (CString str, CString strModule, int nPriority, time_t dt)
{
	/* aggiungiamo un errore */
	FillMessage (str, strModule, nPriority, dt, -1);
	m_nItem++;
}

/*
** AddMessage :
*/
void CAdamoListMessages::AddError (stThiraErrore *ae)
{
}

/*
** FillMessage :
*/
void CAdamoListMessages::FillMessage (CString str, CString strModule, int nPriority, time_t dt, int nItem)
{
    COleDateTime odt (dt);
    LVITEM lvi;
    char szBuffer[256];

	/* settiamo l'item, se diverso da 0, accodiamo */
	if (nItem != 0)
		nItem = GetListCtrl ().GetItemCount ();
	/* messaggio */
	memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE;
    lvi.iItem=nItem;
	lvi.iImage = 5;
    lvi.iSubItem=0;
    strcpy (szBuffer, str);
    lvi.pszText = szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().InsertItem (&lvi);

    /* modulo */
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=1;
    lvi.pszText=strModule.GetBuffer ();
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
	strModule.ReleaseBuffer ();

	/* priorita */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", nPriority);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);

	/* data / ora */
    CString strHour=odt.Format ("%H:%M:%S");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=3;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
    strHour.ReleaseBuffer ();

    /* data / ora */
    CString strDate=odt.Format ("%d/%m/%Y");
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nItem;
    lvi.iSubItem=4;
    lvi.pszText=strDate.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    GetListCtrl ().SetItem (&lvi);
    strDate.ReleaseBuffer ();
}

void CAdamoListMessages::CreateColumnHeader ()
{
    CRect rect;

    GetListCtrl().GetWindowRect(&rect);
	GetListCtrl().InsertColumn(0, LOADSTRING (IDS_ADAMOSELISTVIEW_4), LVCFMT_LEFT, 0, 0);
	GetListCtrl().InsertColumn(1, LOADSTRING (IDS_ADAMOSELISTVIEW_27),LVCFMT_LEFT, 0, 1);
	GetListCtrl().InsertColumn(2, LOADSTRING (IDS_ADAMOSELISTVIEW_5), LVCFMT_LEFT, 0, 2);
	GetListCtrl().InsertColumn(3, LOADSTRING (IDS_ADAMOSELISTVIEW_1), LVCFMT_LEFT, 0, 3);
	GetListCtrl().InsertColumn(4, LOADSTRING (IDS_ADAMOSELISTVIEW_2), LVCFMT_LEFT, 0, 4);
    CHeaderCtrl* hc=GetListCtrl().GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrTime=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrPriority=new CBitmap;
    m_bmpHdrModule=new CBitmap;
    m_bmpHdrTime->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDate->LoadBitmap(IDB_CAPDATE);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrPriority->LoadBitmap(IDB_CAPPRIORITY);
    m_bmpHdrModule->LoadBitmap(IDB_CAPMODULES);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrModule, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPriority, RGB(0, 128,128));
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
}

/*
** OnSize :
*/
void CAdamoListMessages::OnSize(UINT nType, int cx, int cy)
{
	int nCountPerPage = GetListCtrl ().GetCountPerPage ();
	int nItemCount = GetListCtrl ().GetItemCount ();
	bool bIsVisible = nItemCount > nCountPerPage;
	int nMargin = bIsVisible ? 18 : 0;

	CView::OnSize(nType, cx, cy);
	GetListCtrl ().MoveWindow (0, 0, cx, cy);
    GetListCtrl ().SetColumnWidth (0, (int)((double)cx / 100.0 * 40.0)+2-nMargin);
    GetListCtrl ().SetColumnWidth (1, (int)((double)cx / 100.0 * 15.0));
    GetListCtrl ().SetColumnWidth (2, (int)((double)cx / 100.0 * 15.0));
    GetListCtrl ().SetColumnWidth (3, (int)((double)cx / 100.0 * 15.0));
    GetListCtrl ().SetColumnWidth (4, (int)((double)cx / 100.0 * 15.0));
}

/*
** OnDraw :
*/
void CAdamoListMessages::OnDraw(CDC* pDC)
{
}

/*
** CreateImageList :
*/
void CAdamoListMessages::CreateImageList ()
{
    m_lviml=new CImageList;
	m_lviml->Create(MAKEINTRESOURCE(IDB_BMP_LIST_ERRORS), 16, 1, RGB(0,128,128));
    GetListCtrl ().SetImageList (m_lviml, LVSIL_SMALL);
}

/*
** OnInitialUpdate :
*/
void CAdamoListMessages::OnInitialUpdate ()
{
	CView::OnInitialUpdate();
	if (!m_bInitialized)   {
		DWORD dw=GetListCtrl().GetExtendedStyle ();
		dw|=LVS_EX_FULLROWSELECT;
		GetListCtrl().SetExtendedStyle (dw);
		GetListCtrl().GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
		GetListCtrl().ShowScrollBar (SB_VERT);
		m_bInitialized = true;
	}
}

/*
** OnCreate :
*/
int CAdamoListMessages::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	BOOL b = m_list.Create (WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_OWNERDRAWFIXED, CRect (0, 0, 0, 0), this, 105);
	if (b)   {
		CreateImageList ();
		CreateColumnHeader ();
	}
	return b != 0;
}

/*
** DeleteMessages :
*/
void CAdamoListMessages::DeleteMessages ()
{
	m_list.DeleteAllItems ();
}
