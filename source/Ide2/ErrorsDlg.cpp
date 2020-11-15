// ErrorsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "ErrorsDlg.h"
#include "AdamoErr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#include "adamoint.h"
#include "adamoglobal.h"

/////////////////////////////////////////////////////////////////////////////
// CErrorsDlg dialog


CErrorsDlg::CErrorsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorsDlg::IDD, pParent), m_pData (NULL), m_bFreeze (false), m_tbiml (NULL), m_hciml (NULL), m_bmpTblEH (NULL),
      m_bmpTblEI (NULL), m_bmpTblEC (NULL), m_bmpTblEISO (NULL), m_bmpHdrDate (NULL), m_bmpHdrError (NULL), m_bmpHdrTask (NULL), m_bmpHdrPath (NULL),
      m_bmpHdrLine (NULL), m_bmpHdrDevice (NULL), m_bmpHdrDeviceType (NULL), m_bmpHdrIdentifier (NULL), m_bmpHdrPriority (NULL),
      m_bmpHdrDisk (NULL)
{
	//{{AFX_DATA_INIT(CErrorsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CErrorsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorsDlg)
	DDX_Control(pDX, IDC_FREEZE, m_ctrlButFreeze);
	DDX_Control(pDX, IDC_LISTERRORS, m_ctrlListErrors);
	DDX_Control(pDX, IDC_TABERRORS, m_ctrlTabErrors);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CErrorsDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorsDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABERRORS, OnSelchangeTaberrors)
	ON_BN_CLICKED(IDC_GOTOLINE, OnGotoLine)
	ON_BN_CLICKED(IDC_CLEARALL, OnClearAll)
	ON_BN_CLICKED(IDC_FREEZE, OnFreeze)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CErrorsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorsDlg message handlers

BOOL CErrorsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    CString str;

    /* settiamo il font */	
    m_ctrlTabErrors.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    /* settiamo la dimensione massima dei tab del controllo */
	TC_ITEM TabCtrlItem;
	m_ctrlTabErrors.SetMinTabWidth(156);
    /* creiamo ora icone e image list del tab control */
    m_tbiml=new CImageList;
    m_bmpTblEH=new CBitmap;    
    m_bmpTblEI=new CBitmap;
    m_bmpTblEC=new CBitmap;
    m_bmpTblEISO=new CBitmap;
    m_bmpTblEH->LoadBitmap(IDB_CAPHW);
    m_bmpTblEI->LoadBitmap(IDB_CAPINTERRORS);
    m_bmpTblEC->LoadBitmap(IDB_CAPCYCERRORS);
    m_bmpTblEISO->LoadBitmap(IDB_CAPISOERRORS);
    m_tbiml->Create (16, 16, ILC_MASK, 0, 1);
    m_tbiml->Add(m_bmpTblEH, RGB(0, 128,128));
    m_tbiml->Add(m_bmpTblEI, RGB(0, 128,128));
    m_tbiml->Add(m_bmpTblEC, RGB(0, 128,128));
    m_tbiml->Add(m_bmpTblEISO, RGB(0, 128,128));
    m_ctrlTabErrors.SetImageList(m_tbiml);
	TabCtrlItem.mask = TCIF_TEXT|TCIF_IMAGE;
    str=LOADSTRING (IDS_ADAMOERRORSTYPE_1);
	TabCtrlItem.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    TabCtrlItem.iImage=0;
	m_ctrlTabErrors.InsertItem( 0, &TabCtrlItem );
	TabCtrlItem.mask = TCIF_TEXT|TCIF_IMAGE;
    str=LOADSTRING (IDS_ADAMOERRORSTYPE_2);
	TabCtrlItem.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    TabCtrlItem.iImage=1;
	m_ctrlTabErrors.InsertItem( 1, &TabCtrlItem );
	TabCtrlItem.mask = TCIF_TEXT|TCIF_IMAGE;
    str=LOADSTRING (IDS_ADAMOERRORSTYPE_3);
	TabCtrlItem.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    TabCtrlItem.iImage=2;
	m_ctrlTabErrors.InsertItem( 2, &TabCtrlItem );
	TabCtrlItem.mask = TCIF_TEXT|TCIF_IMAGE;
    str=LOADSTRING (IDS_ADAMOERRORSTYPE_4);
	TabCtrlItem.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    TabCtrlItem.iImage=3;
	m_ctrlTabErrors.InsertItem( 3, &TabCtrlItem );
    /* settiamo il font */
    m_ctrlListErrors.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    /* settiamo lo stile full row al listview */
    DWORD dw=m_ctrlListErrors.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_ctrlListErrors.SetExtendedStyle (dw);
    /* caption della dialog */
    SetWindowText (LOADSTRING (IDS_ADAMOERRORSDLG_1));
	return TRUE;
}

void CErrorsDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    DeleteTabResource ();
    DeleteHeaderResource ();
}

void CErrorsDlg::SetData (CPtrList* pData)
{
    m_pData=pData;
}

void CErrorsDlg::Refresh ()
{
    if (m_pData)   {
        if (!m_bFreeze)   {
            /* svuotiamo prima di tutto il listview */
            m_ctrlListErrors.DeleteAllItems ();
            if (m_pData->GetCount ())   {
                /* prendiamo l'ultimo errore */
                stAdamoErrore* pTail=(stAdamoErrore*)m_pData->GetTail ();
                /* settiamo il tab corretto da settare */
                m_ctrlTabErrors.SetCurSel (pTail->nTypeError);
                LoadErrorsInListBox (pTail->nTypeError);
                /* selezioniamo sempre la prima voce */
                m_ctrlListErrors.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
            }   
        }   
    }   
}

void CErrorsDlg::OnSelchangeTaberrors(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int nCurSel=m_ctrlTabErrors.GetCurSel ();
    LoadErrorsInListBox (nCurSel);
	*pResult = 0;
}

void CErrorsDlg::FillList (int nTypeError)
{
    CProject* pPrj;
    CAdamoDatiFile* pADF=NULL;
    POSITION pos;
    int i, nInterpreter, nCycle, nHardware;

    /* riempiamo ora il listview */
    pADF=new CAdamoDatiFile;
    pPrj=GETWS()->GetActiveProject();
    if (pPrj&&pADF)   {
        if (pADF->Open (pPrj->GetParamsPathName(), TRUE))   {
            pADF->ReadData ();
            if (m_pData->GetCount ())  {
                for (i=0, pos=m_pData->GetTailPosition(), nHardware=0, nInterpreter=0, nCycle=0; pos; i++)    {
                    stAdamoErrore* p=(stAdamoErrore*) m_pData->GetPrev (pos);
                    if (nTypeError==p->nTypeError)    {
                        switch (nTypeError)   {
                            case TE_HARDWARE :
                                FillHardware (p, nHardware++, pADF);
                                break;
                            case TE_ISO :
                                FillISO (p, nInterpreter++);
                                break;
                            case TE_INTERPRETER :
                                FillInterpreter (p, nInterpreter++);
                                break;
                            case TE_CYCLE :
                                FillCycle (p, nCycle++);
                                break;
                        }
                    }
                }
            }
            pADF->Close ();
        }
    }
    if (pADF)
        _delete (pADF);
}

void CErrorsDlg::OnOK() 
{
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGERRORS, 0, 0);
}

void CErrorsDlg::OnCancel() 
{
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGERRORS, 0, 0);
}

void CErrorsDlg::OnGotoLine() 
{
    int n;
    char szBufLine[256];
    char szBufPath[256];

    if ((n=m_ctrlListErrors.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
        m_ctrlListErrors.GetItemText (n, 2, szBufLine, 256);
        stAdamoErrore* pAdamoErrore=(stAdamoErrore*) m_ctrlListErrors.GetItemData (n);
        strcpy (szBufPath, pAdamoErrore->strErrore);
        if (szBufPath!=""&&szBufLine!="")   {
            char* p=strchr (szBufPath, '@');
            if (p)   {
                *p='\0';
	            ((CMainFrame*)AfxGetMainWnd())->GotoFileLine(szBufPath, atoi (szBufLine), true);
                OnOK ();
            }
        }
    }
}

void CErrorsDlg::OnClearAll() 
{
    for (POSITION pos=m_pData->GetHeadPosition(); pos;)   {
        stAdamoErrore* p=(stAdamoErrore*) m_pData->GetNext(pos);
        delete p;
    }
    m_pData->RemoveAll ();
    m_ctrlListErrors.DeleteAllItems ();
}

void CErrorsDlg::OnFreeze() 
{
	// TODO: Add your control notification handler code here
    if (m_bFreeze)   {
        m_bFreeze=false;
        m_ctrlButFreeze.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_1));
        Refresh ();
    }
    else   {
        m_bFreeze=true;
        m_ctrlButFreeze.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_2));
    }
}

void CErrorsDlg::LoadHeaderInterpreter ()
{
    CRect rect;

    m_ctrlListErrors.GetWindowRect(&rect);
	m_ctrlListErrors.InsertColumn(0, LOADSTRING (IDS_ADAMOERRORSDLG_2), LVCFMT_LEFT,
		rect.Width() * 15/100, 0);
	m_ctrlListErrors.InsertColumn(1, LOADSTRING (IDS_ADAMOERRORSDLG_3), LVCFMT_LEFT,
		rect.Width() * 15/100, 1);
	m_ctrlListErrors.InsertColumn(2, LOADSTRING (IDS_ADAMOERRORSDLG_4), LVCFMT_LEFT,
		rect.Width() * 10/100, 2);
	m_ctrlListErrors.InsertColumn(3, LOADSTRING (IDS_ADAMOERRORSDLG_5), LVCFMT_LEFT,
		rect.Width() * 40/100, 3);
	m_ctrlListErrors.InsertColumn(4, LOADSTRING (IDS_ADAMOERRORSDLG_6), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
    CHeaderCtrl* hc=m_ctrlListErrors.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrPath=new CBitmap;
    m_bmpHdrLine=new CBitmap;
    m_bmpHdrDate->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrPath->LoadBitmap(IDB_CAPPATH);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrLine->LoadBitmap(IDB_CAPLINE);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPath, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrLine, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
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

void CErrorsDlg::LoadHeaderISO ()
{
    CRect rect;

    m_ctrlListErrors.GetWindowRect(&rect);
	m_ctrlListErrors.InsertColumn(0, LOADSTRING (IDS_ADAMOERRORSDLG_2), LVCFMT_LEFT,
		rect.Width() * 15/100, 0);
	m_ctrlListErrors.InsertColumn(1, LOADSTRING (IDS_ADAMOERRORSDLG_3), LVCFMT_LEFT,
		rect.Width() * 15/100, 1);
	m_ctrlListErrors.InsertColumn(2, LOADSTRING (IDS_ADAMOERRORSDLG_4), LVCFMT_LEFT,
		rect.Width() * 10/100, 2);
	m_ctrlListErrors.InsertColumn(3, LOADSTRING (IDS_ADAMOERRORSDLG_5), LVCFMT_LEFT,
		rect.Width() * 40/100, 3);
	m_ctrlListErrors.InsertColumn(4, LOADSTRING (IDS_ADAMOERRORSDLG_6), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
    CHeaderCtrl* hc=m_ctrlListErrors.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrPath=new CBitmap;
    m_bmpHdrLine=new CBitmap;
    m_bmpHdrDate->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrPath->LoadBitmap(IDB_CAPPATH);
    m_bmpHdrError->LoadBitmap(IDB_CAPISOERRORS);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrLine->LoadBitmap(IDB_CAPLINE);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPath, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrLine, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
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

void CErrorsDlg::DeleteTabResource ()
{
    if (m_bmpTblEH)
        delete m_bmpTblEH;
    if (m_bmpTblEI)
        delete m_bmpTblEI;
    if (m_bmpTblEC)
        delete m_bmpTblEC;
    if (m_bmpTblEISO)
        delete m_bmpTblEISO;
    if (m_tbiml)
        delete m_tbiml;
}

void CErrorsDlg::DeleteHeaderResource ()
{
    CHeaderCtrl* hc=m_ctrlListErrors.GetHeaderCtrl();
    int i, n=hc->GetItemCount();

    for (i=0; i<n; i++)
        m_ctrlListErrors.DeleteColumn (0);
    if (m_bmpHdrDate)
        _delete (m_bmpHdrDate);
    if (m_bmpHdrError)
        _delete (m_bmpHdrError);
    if (m_bmpHdrTask)
        _delete (m_bmpHdrTask);
    if (m_bmpHdrPath)
        _delete (m_bmpHdrPath);
    if (m_bmpHdrLine)
        _delete (m_bmpHdrLine);
    if (m_bmpHdrDevice)
        _delete (m_bmpHdrDevice);
    if (m_bmpHdrDeviceType)
        _delete (m_bmpHdrDeviceType);
    if (m_bmpHdrIdentifier)
        _delete (m_bmpHdrIdentifier);
    if (m_bmpHdrPriority)
        _delete (m_bmpHdrPriority);
    if (m_bmpHdrDisk)
        _delete (m_bmpHdrDisk);
    if (m_hciml)
        _delete (m_hciml);
}

void CErrorsDlg::LoadHeaderHardware ()
{
    CRect rect;

    m_ctrlListErrors.GetWindowRect(&rect);
	m_ctrlListErrors.InsertColumn(0, LOADSTRING (IDS_ADAMOERRORSDLG_2), LVCFMT_LEFT,
		rect.Width() * 15/100, 0);
	m_ctrlListErrors.InsertColumn(1, LOADSTRING (IDS_ADAMOERRORSDLG_7), LVCFMT_LEFT,
		rect.Width() * 15/100, 1);
	m_ctrlListErrors.InsertColumn(2, LOADSTRING (IDS_ADAMOERRORSDLG_8), LVCFMT_LEFT,
		rect.Width() * 10/100, 2);
	m_ctrlListErrors.InsertColumn(3, LOADSTRING (IDS_ADAMOERRORSDLG_5), LVCFMT_LEFT,
		rect.Width() * 40/100, 3);
	m_ctrlListErrors.InsertColumn(4, LOADSTRING (IDS_ADAMOERRORSDLG_6), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 4);
    CHeaderCtrl* hc=m_ctrlListErrors.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrDevice=new CBitmap;
    m_bmpHdrDeviceType=new CBitmap;
    m_bmpHdrDate->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrDevice->LoadBitmap(IDB_CAPDEVICE);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrDeviceType->LoadBitmap(IDB_CAPTYPEDEVICE);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDevice, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDeviceType, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
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

void CErrorsDlg::LoadHeaderCycle ()
{
    CRect rect;

    m_ctrlListErrors.GetWindowRect(&rect);
	m_ctrlListErrors.InsertColumn(0, LOADSTRING (IDS_ADAMOERRORSDLG_2), LVCFMT_LEFT,
		rect.Width() * 12/100, 0);
	m_ctrlListErrors.InsertColumn(1, LOADSTRING (IDS_ADAMOERRORSDLG_9), LVCFMT_LEFT,
		rect.Width() * 8/100, 1);
	m_ctrlListErrors.InsertColumn(2, LOADSTRING (IDS_ADAMOERRORSDLG_5), LVCFMT_LEFT,
		rect.Width() * 40/100, 2);
	m_ctrlListErrors.InsertColumn(3, LOADSTRING (IDS_ADAMOERRORSDLG_10), LVCFMT_LEFT,
		rect.Width() * 12/100, 3);
	m_ctrlListErrors.InsertColumn(4, LOADSTRING (IDS_ADAMOERRORSDLG_11), LVCFMT_LEFT,
		(rect.Width() * 8/100)-2, 4);
	m_ctrlListErrors.InsertColumn(5, LOADSTRING (IDS_ADAMOERRORSDLG_6), LVCFMT_LEFT,
		(rect.Width() * 20/100)-2, 5);
    CHeaderCtrl* hc=m_ctrlListErrors.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_bmpHdrDate=new CBitmap;
    m_bmpHdrError=new CBitmap;
    m_bmpHdrTask=new CBitmap;
    m_bmpHdrIdentifier=new CBitmap;
    m_bmpHdrPriority=new CBitmap;
    m_bmpHdrDisk=new CBitmap;
    m_bmpHdrDate->LoadBitmap(IDB_CAPTIME);
    m_bmpHdrError->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpHdrTask->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrIdentifier->LoadBitmap(IDB_CAPID);
    m_bmpHdrPriority->LoadBitmap(IDB_CAPPRIORITY);
    m_bmpHdrDisk->LoadBitmap(IDB_CAPDISK);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrDate, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrIdentifier, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrError, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrPriority, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDisk, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrTask, RGB(0, 128,128));
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

void CErrorsDlg::FillInterpreter (stAdamoErrore* p, int i)
{
    CString strModule, strLine, strError;
    char szBuffer[256];

    CString strHour=COleDateTime(p->dt).Format ("%H:%M:%S");
    strcpy (szBuffer, p->strErrore);
    ((CMainFrame*)AfxGetMainWnd ())->ParseStrError (szBuffer, &strModule, &strLine, &strError);
    LVITEM lvi;
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=i;
    lvi.iSubItem=0;
    lvi.iImage=p->nTypeError;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    lvi.lParam=(long)p;
    m_ctrlListErrors.InsertItem (&lvi);
    strHour.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=1;
    lvi.pszText=strModule.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
    strModule.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=2;
    lvi.pszText=strLine.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
    strLine.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=3;
    lvi.pszText=strError.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
    strError.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=4;
    lvi.pszText=p->szTaskName;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
}

void CErrorsDlg::FillISO (stAdamoErrore* p, int i)
{
    CString strModule, strLine, strError;
    char szBuffer[256];

    CString strHour=COleDateTime(p->dt).Format ("%H:%M:%S");
    strcpy (szBuffer, p->strErrore);
    ((CMainFrame*)AfxGetMainWnd ())->ParseStrError (szBuffer, &strModule, &strLine, &strError);
    LVITEM lvi;
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=i;
    lvi.iSubItem=0;
    lvi.iImage=p->nTypeError;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    lvi.lParam=(long)p;
    m_ctrlListErrors.InsertItem (&lvi);
    strHour.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=1;
    lvi.pszText=strModule.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
    strModule.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=2;
    lvi.pszText=strLine.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
    strLine.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=3;
    lvi.pszText=strError.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
    strError.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=4;
    lvi.pszText=p->szTaskName;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
}

void CErrorsDlg::FillHardware (stAdamoErrore* p, int i, CAdamoDatiFile* pADF)
{
    char szBuffer[256];

    CString strHour=COleDateTime(p->dt).Format ("%H:%M:%S");
    LVITEM lvi;

    /* data / ora */
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=i;
    lvi.iSubItem=0;
    lvi.iImage=p->nTypeError;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    lvi.lParam=(long)p;
    m_ctrlListErrors.InsertItem (&lvi);
    strHour.ReleaseBuffer ();

    /* nome del dispositivo */
    memset (&lvi, 0, sizeof (LVITEM));
    if (p->nTypeDevice<HARDWARE_ERROR)
        pADF->GetDeviceName (p->nDevice, p->nTypeDevice, szBuffer);
    else
        ((CMainFrame*)AfxGetMainWnd ())->GetDeviceErrorString (p->nDevice, p->nTypeDevice, szBuffer);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);

    /* tipo del dispositivo */
    memset (&lvi, 0, sizeof (LVITEM));
	((CMainFrame*)AfxGetMainWnd ())->GetTypeDeviceErrorString (p->nTypeDevice, szBuffer);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);

    /* Errore */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, p->strErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=3;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
}

void CErrorsDlg::FillCycle (stAdamoErrore* p, int i)
{
    char szBuffer[256];

    CString strHour=COleDateTime(p->dt).Format ("%H:%M:%S");
    LVITEM lvi;

    /* data / ora */
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    lvi.iItem=i;
    lvi.iSubItem=0;
    lvi.iImage=p->nTypeError;
    lvi.pszText=strHour.GetBuffer(64);
    lvi.cchTextMax=NMAXLENGTHERRORE;
    lvi.lParam=(long)p;
    m_ctrlListErrors.InsertItem (&lvi);
    strHour.ReleaseBuffer ();

    /* ID */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", p->nErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=1;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);

    /* Errore */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, p->strErrore);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=2;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);

    /* priorita */
    memset (&lvi, 0, sizeof (LVITEM));
    sprintf (szBuffer, "%d", p->nTypeDevice);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=3;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);

    /* disk */
    memset (&lvi, 0, sizeof (LVITEM));
    if (p->nDevice)
        strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_12));
    else
        strcpy (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_13));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=4;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);

    /* task */
    memset (&lvi, 0, sizeof (LVITEM));
    strcpy (szBuffer, p->szTaskName);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=i;
    lvi.iSubItem=5;
    lvi.pszText=szBuffer;
    lvi.cchTextMax=NMAXLENGTHERRORE;
    m_ctrlListErrors.SetItem (&lvi);
}

void CErrorsDlg::LoadErrorsInListBox (int nSel)
{ 
    DeleteHeaderResource ();
    switch (nSel)   {
        case 0 :
            LoadHeaderHardware ();
            break;
        case 1 :
            LoadHeaderInterpreter ();
            break;
        case 2 :
            LoadHeaderCycle ();
            break;
        case 3 :
            LoadHeaderISO ();
            break;
    }
    m_ctrlListErrors.DeleteAllItems ();
    FillList (nSel);
}

void CErrorsDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGERRORS, 0, 0);
}

void CErrorsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
