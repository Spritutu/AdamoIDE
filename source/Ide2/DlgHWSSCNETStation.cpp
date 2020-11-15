// DlgHWSSCNETStation.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "AdamoTemplates.h"
#include "dlghwsscnetstation.h"

#define TYPE_PA     0
#define TYPE_PB     1
#define TYPE_PC     2
#define TYPE_PD     3
#define TYPE_PE     4
#define TYPE_PF     5
#define TYPE_PL     7

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHWSSCNETStation dialog
IMPLEMENT_DYNAMIC (CDlgHWSSCNETStation, CDialog)

CDlgHWSSCNETStation::CDlgHWSSCNETStation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWSSCNETStation::IDD, pParent), m_pFlatTabCtrl (NULL), m_pConfigDoc (NULL), m_bInitialized (false), m_nStation (0), m_nModule (-1), m_pFontSSCNET (NULL),
      m_pBasePar (NULL), m_pGainFilterPar (NULL), m_pExtensionPar (NULL), m_pIOPar (NULL), m_pIOExt2 (NULL), m_pIOExt3 (NULL), m_LinearMotorPar (NULL),
      m_pMachine (NULL), m_pIO (NULL), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgHWSSCNETStation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWSSCNETStation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHWSSCNETStation)
	DDX_Control(pDX, IDC_ST_ABS_ENCODER, m_stEncTitle);
	DDX_Control(pDX, IDC_CHECK_ENABLE_ABS_ENCODER, m_EnableAbsEncoder);
	DDX_Control(pDX, IDC_ST_STATION, m_stStationTitle);
	DDX_Control(pDX, IDC_ST_AXISTITLE, m_stAxisTitle);
	//DDX_Control(pDX, IDC_GRID_HW_SSCNET, m_GridSSCNETAxis);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHWSSCNETStation, CDialog)
	//{{AFX_MSG_MAP(CDlgHWSSCNETStation)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_ENABLE_ABS_ENCODER, OnCheckEnableAbsEncoder)
	ON_EN_CHANGE(IDC_EDIT_HOME_POSITION, OnChangeEditHomePosition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWSSCNETStation message handlers
/*
** SetConfigDoc :
*/
void CDlgHWSSCNETStation::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

BOOL CDlgHWSSCNETStation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_bInitialized==false)   {
		m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
		CreateFont ();
		InitializeGrid (m_GridSSCNETAxis, IDC_GRID_HW_SSCNET, 1, 0);
		InitializeTitles ();
		SetConfigDoc ();
		m_GridSSCNETAxis.SetConfigDoc (m_pConfigDoc);
		m_GridSSCNETAxis.SetProject (m_pPrj);
		InitializeGridData ();
		InitData ();
		CreateTab ();
		InitializeMachine ();
		InitPsw ();
		m_bInitialized=true;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnCancel :
*/
void CDlgHWSSCNETStation::OnCancel ()
{
}

/*
** OnOK :
*/
void CDlgHWSSCNETStation::OnOK ()
{
}

/*
** Find :
*/
void CDlgHWSSCNETStation::Find (const char *szBuffer)
{
    if (!m_GridSSCNETAxis.Find (szBuffer))
        if (m_pIO->Find (szBuffer))
            m_pFlatTabCtrl->SetCurSel(4);
}

void CDlgHWSSCNETStation::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_stStationTitle.MoveWindow (10, 10, 300, 40);
        m_stAxisTitle.MoveWindow (10, 50, 200, 20);
        m_stEncTitle.MoveWindow (cx-200, 50, 190, 20);
        m_EnableAbsEncoder.MoveWindow (cx-200, 67, 190, 20);
        m_EditHomePosition.MoveWindow (cx-200, 92, 190, 20);

        m_GridSSCNETAxis.MoveWindow (10, 70, cx-229, 2*19+10);
        m_GridSSCNETAxis.ExpandLastColumn();
        ((CWnd &)m_GridSSCNETAxis).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_GridSSCNETAxis).EnableScrollBarCtrl (SB_VERT, FALSE);

        m_pFlatTabCtrl->MoveWindow (10, 123, cx-20, cy-133);
    }
}

/*
** InitializeGrid
*/
void CDlgHWSSCNETStation::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.SetMode (nMode);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.SetEditable(false);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(3);
    m_Grid.SetFixedRowCount ();
    m_Grid.SetRowCount (nRow+1);
    m_Grid.SetEditable ();
    m_Grid.EnableDragAndDrop ();
    m_Grid.SetListMode ();
    m_Grid.FillGridCaption ();
    for (int i=1; i<nRow+1; i++)
        m_Grid.SetCellType (i, 1, RUNTIME_CLASS (CConfigHWGridCell));
}

/*
** InitializeTitles :
*/
void CDlgHWSSCNETStation::InitializeTitles ()
{
	CString strFormat, strFormat_1;
    CString str, str1;

    str.LoadString (IDS_CONFIG_HW_TITLE_SSCNET_AXIS);
    m_stAxisTitle.SetWindowText (str);
	strFormat.LoadString (IDS_CONFIG_HW_TITLE_SSCNET_STATION);
	strFormat_1.LoadString (IDS_CONFIG_HW_TITLE_SSCNET_MODULE_STATION );
	if (m_nModule == -1)
		str.Format (strFormat, m_nStation + 1);
	else
		str.Format (strFormat_1, m_nModule, m_nStation + 1);
	m_stStationTitle.SetWindowText (str);
    m_stEncTitle.SetWindowText (LOADSTRING (IDS_CONFIG_HW_TITLE_SSCNET_ENC));
    m_EnableAbsEncoder.SetWindowText (LOADSTRING (IDS_CONFIG_HW_SSCNET_ENC_ENABLE));
    m_EditHomePosition.SubclassEdit(IDC_EDIT_HOME_POSITION, this, PES_NUMBERS|PES_FLOATS);
}

/*
** InitializeGridData :
*/
void CDlgHWSSCNETStation::InitializeGridData ()
{
    CString str;
    str.Format ("M%dAx%d", m_nKinemaAddress+1, m_nStation+1);
    m_GridSSCNETAxis.SetGridData (1, 1, 1, 2, str);
    m_GridSSCNETAxis.SetItemText (1, 0, "Ax0");
    m_GridSSCNETAxis.SetType (enAsse);
    m_GridSSCNETAxis.UpdateData ();
}

/*
** CreateTab :
*/
void CDlgHWSSCNETStation::CreateTab ()
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    int n = 0;
    RECT rc;

    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTFlatTabCtrl();
	if (m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		&rc, this, IDC_FLAT_TABCTRL))  {
        m_pBasePar=new CSSCNETParameters ();
		m_pBasePar->SetProject (m_pPrj);
        InitBaseParData ();
        m_pBasePar->Create (IDD_DLG_SSCNETPAR, this);
    	m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_BASEPAR), m_pBasePar);

        m_pGainFilterPar=new CSSCNETParameters ();
		m_pGainFilterPar->SetProject (m_pPrj);
        InitGainFilterParData ();
        m_pGainFilterPar->Create (IDD_DLG_SSCNETPAR, this);
    	m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_GAINFILTERPAR), m_pGainFilterPar);

        m_pExtensionPar=new CSSCNETParameters ();
		m_pExtensionPar->SetProject (m_pPrj);
        InitExtensionParData ();
        m_pExtensionPar->Create (IDD_DLG_SSCNETPAR, this);
    	m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_EXTENSIONPAR), m_pExtensionPar);

        m_pIOPar=new CSSCNETParameters ();
		m_pIOPar->SetProject (m_pPrj);
        InitIOParData ();
        m_pIOPar->Create (IDD_DLG_SSCNETPAR, this);
	    m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_IOPAR), m_pIOPar);

        if (pACF->GetSSCNETNetworkType (m_nKinemaAddress))   {
            m_pIOExt2=new CSSCNETParameters ();
			m_pIOExt2->SetProject (m_pPrj);
            InitExtension2ParData ();
            m_pIOExt2->Create (IDD_DLG_SSCNETPAR, this);
    	    m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2PAR), m_pIOExt2);

            m_pIOExt3=new CSSCNETParameters ();
			m_pIOExt3->SetProject (m_pPrj);
            InitExtension3ParData ();
            m_pIOExt3->Create (IDD_DLG_SSCNETPAR, this);
    	    m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3PAR), m_pIOExt3);

            m_LinearMotorPar=new CSSCNETParameters ();
			m_LinearMotorPar->SetProject (m_pPrj);
            InitLinearMotorParData ();
            m_LinearMotorPar->Create (IDD_DLG_SSCNETPAR, this);
    	    m_pFlatTabCtrl->InsertItem(n++, LOADSTRING (IDS_CONFIG_HW_SSCNET_LMPAR), m_LinearMotorPar);
        }
        m_pIO=new CSSCNETIO ();
		m_pIO->SetProject (m_pPrj);
        m_pIO->SetKinemaAddress (m_nKinemaAddress); m_pIO->SetSSCNETStation (m_nStation);
        InitIOData ();
        m_pIO->Create (IDD_DLG_SSCNETIO, this);
    	m_pFlatTabCtrl->InsertItem(n, LOADSTRING (IDS_CONFIG_HW_SSCNET_DIGITALIO), m_pIO);

        m_pFlatTabCtrl->SetCurSel(0);
        LoadParValues ();
    }
}

/*
** CreateFont :
*/
void CDlgHWSSCNETStation::CreateFont ()
{
    m_pFontSSCNET=((CMainFrame *) AfxGetMainWnd ())->CreateFont ("Tahoma", 18, FW_NORMAL);
    m_stStationTitle.SetFont (m_pFontSSCNET);
}

void CDlgHWSSCNETStation::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    if (m_pFontSSCNET)   {
	    DeleteObject (m_pFontSSCNET);
        _delete (m_pFontSSCNET);
    }
    SaveData ();
    SaveParValues ();
    m_pFlatTabCtrl->DeleteAllItems ();
    m_pFlatTabCtrl->DestroyWindow ();
    _delete (m_pFlatTabCtrl);
    m_pBasePar->DestroyWindow ();
    m_pGainFilterPar->DestroyWindow ();
    m_pExtensionPar->DestroyWindow ();
    m_pIOPar->DestroyWindow ();
    m_pIO->DestroyWindow ();
    _delete (m_pBasePar);
    _delete (m_pGainFilterPar);
    _delete (m_pExtensionPar);
    _delete (m_pIOPar);
    _delete (m_pIO);
    m_GridSSCNETAxis.DestroyWindow ();
}

/*
** RemoveCurrentConnection :
*/
void CDlgHWSSCNETStation::RemoveCurrentConnection ()
{
    m_GridSSCNETAxis.RemoveCurrentConnection ();
    m_pIO->RemoveCurrentConnection ();
}

/*
** InitBaseParData :
*/
void CDlgHWSSCNETStation::InitBaseParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
    CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
    m_pBasePar->InitParData (SSCNET_BASEPAR);
    m_pBasePar->SetGridData (0, "PA01", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, true);
    m_pBasePar->SetGridData (1, "PA02", "REG", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_2), true, true);
    m_pBasePar->SetGridData (2, "PA03", "ABS", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_3), true, true);
    m_pBasePar->SetGridData (3, "PA04", "A0P1", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_4), true, true);
    m_pBasePar->SetGridData (4, "PA05", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, false);
    m_pBasePar->SetGridData (5, "PA06", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, false);
    m_pBasePar->SetGridData (6, "PA07", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, false);
    m_pBasePar->SetGridData (7, "PA08", "ATU", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_8), true, true);
    m_pBasePar->SetGridData (8, "PA09", "RSP", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_9), true, false);
    m_pBasePar->SetGridData (9, "PA10", "INP", strUnitPulse, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_10), true, false);
    m_pBasePar->SetGridData (10, "PA11", "", strUnitPerc, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, false);
    m_pBasePar->SetGridData (11, "PA12", "", strUnitPerc, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, false);
    m_pBasePar->SetGridData (12, "PA13", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, true);
    m_pBasePar->SetGridData (13, "PA14", "POL", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_14), true, false);
    m_pBasePar->SetGridData (14, "PA15", "ENR", strUnitPulseRev, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_15), true, false);
    m_pBasePar->SetGridData (15, "PA16", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, false);
    m_pBasePar->SetGridData (16, "PA17", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, true);
    m_pBasePar->SetGridData (17, "PA18", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_1), false, true);
    m_pBasePar->SetGridData (18, "PA19", "BLK", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARBASE_19), true, true);
}

/*
** InitGainFilterParData :
*/
void CDlgHWSSCNETStation::InitGainFilterParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
    CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
    m_pGainFilterPar->InitParData (SSCNET_GAINFILTERPAR);
    m_pGainFilterPar->SetGridData (0, "PB01", "FILT", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_1), true, true);
    m_pGainFilterPar->SetGridData (1, "PB02", "VRFT", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_2), true, true);
    m_pGainFilterPar->SetGridData (2, "PB03", "", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (3, "PB04", "FFC", strUnitPerc, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_4), true, false);
    m_pGainFilterPar->SetGridData (4, "PB05", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (5, "PB06", "GD2", strUnitTimes,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_6), true, false);
    m_pGainFilterPar->SetGridData (6, "PB07", "PG1", strUnitRadSec,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_7), true, false);
    m_pGainFilterPar->SetGridData (7, "PB08", "PG2", strUnitRadSec,  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_8), true, false);
    m_pGainFilterPar->SetGridData (8, "PB09", "VG2", strUnitRadSec,  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_9), true, false);
    m_pGainFilterPar->SetGridData (9, "PB10", "VIC", strUnitMS, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_10), true, false);
    m_pGainFilterPar->SetGridData (10, "PB11", "VDC", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_11), true, false);
    m_pGainFilterPar->SetGridData (11, "PB12", "", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (12, "PB13", "NH1", strUnitHertz,    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_13), true, false);
    m_pGainFilterPar->SetGridData (13, "PB14", "NHQ1", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_14), true, true);
    m_pGainFilterPar->SetGridData (14, "PB15", "NH2", strUnitHertz, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_15), true, false);
    m_pGainFilterPar->SetGridData (15, "PB16", "NHQ2", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_16), true, true);
    m_pGainFilterPar->SetGridData (16, "PB17", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (17, "PB18", "LPF", strUnitRadSec,    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_18), true, false);
    m_pGainFilterPar->SetGridData (18, "PB19", "VRF1", strUnitHertz, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_19), true, false);
    m_pGainFilterPar->SetGridData (19, "PB20", "VRF2", strUnitHertz,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_20), true, false);
    m_pGainFilterPar->SetGridData (20, "PB21", "", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (21, "PB22", "", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (22, "PB23", "VFBF", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_23), true, true);
    m_pGainFilterPar->SetGridData (23, "PB24", "MVS", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_24), true, true);
    m_pGainFilterPar->SetGridData (24, "PB25", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, true);
    m_pGainFilterPar->SetGridData (25, "PB26", "CDP", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_26), true, true);
    m_pGainFilterPar->SetGridData (26, "PB27", "CDL", "",  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_27), true, false);
    m_pGainFilterPar->SetGridData (27, "PB28", "CDT", strUnitMS,  LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_28), true, false);
    m_pGainFilterPar->SetGridData (28, "PB29", "GD2B", strUnitTimes, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_29), true, false);
    m_pGainFilterPar->SetGridData (29, "PB30", "PG2B", strUnitRadSec, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_30), true, false);
    m_pGainFilterPar->SetGridData (30, "PB31", "VG2B", strUnitRadSec, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_31), true, false);
    m_pGainFilterPar->SetGridData (31, "PB32", "VICB", strUnitMS,    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_32), true, false);
    m_pGainFilterPar->SetGridData (32, "PB33", "VRF1B", strUnitHertz, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_33), true, false);
    m_pGainFilterPar->SetGridData (33, "PB34", "VRF2B", strUnitHertz, LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_34), true, false);
    m_pGainFilterPar->SetGridData (34, "PB35", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (35, "PB36", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (36, "PB37", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (37, "PB38", "", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (38, "PB39", "", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (39, "PB40", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (40, "PB41", "", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (41, "PB42", "", "", LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (42, "PB43", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, true);
    m_pGainFilterPar->SetGridData (43, "PB44", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, false);
    m_pGainFilterPar->SetGridData (44, "PB45", "", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_PARGAINFILTER_0), false, true);
}

/*
** InitExtensionParData :
*/
void CDlgHWSSCNETStation::InitExtensionParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
    CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
    m_pExtensionPar->InitParData (SSCNET_EXTENSIONPAR);
    m_pExtensionPar->SetGridData (0, "PC01", "ERZ", strUnitRev,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_1), true, false);
    m_pExtensionPar->SetGridData (1, "PC02", "MBR", strUnitMS,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_2), true, false);
    m_pExtensionPar->SetGridData (2, "PC03", "ENRS", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_3), true, true);
    m_pExtensionPar->SetGridData (3, "PC04", "COP1", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_4), true, true);
    m_pExtensionPar->SetGridData (4, "PC05", "COP2", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_5), true, true);
    m_pExtensionPar->SetGridData (5, "PC06", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (6, "PC07", "ZSP", strUnitRMin,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_7), true, false);
    m_pExtensionPar->SetGridData (7, "PC08", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, false);
    m_pExtensionPar->SetGridData (8, "PC09", "MOD1", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_9), true, true);
    m_pExtensionPar->SetGridData (9, "PC10", "MOD2", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_10), true, true);
    m_pExtensionPar->SetGridData (10, "PC11", "MO1", strUnitMV,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_11), true, false);
    m_pExtensionPar->SetGridData (11, "PC12", "MO2", strUnitMV,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_12), true, false);
    m_pExtensionPar->SetGridData (12, "PC13", "MOSDL", strUnitPulse,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_13), true, false);
    m_pExtensionPar->SetGridData (13, "PC14", "MOSDH", strUnitPulse,     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_14), true, false);
    m_pExtensionPar->SetGridData (14, "PC15", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, false);
    m_pExtensionPar->SetGridData (15, "PC16", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (16, "PC17", "COP4", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_17), true, true);
    m_pExtensionPar->SetGridData (17, "PC18", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (18, "PC19", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (19, "PC20", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (20, "PC21", "BPS", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_21), true, true);
    m_pExtensionPar->SetGridData (21, "PC22", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (22, "PC23", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (23, "PC24", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (24, "PC25", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (25, "PC26", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (26, "PC27", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (27, "PC28", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (28, "PC29", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (29, "PC30", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (30, "PC31", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
    m_pExtensionPar->SetGridData (31, "PC32", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PAREXTENSION_0), false, true);
}

/*
** InitIOParData :
*/
void CDlgHWSSCNETStation::InitIOParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
    CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
    m_pIOPar->InitParData (SSCNET_IOPAR);
    m_pIOPar->SetGridData (0, "PD01", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (1, "PD02", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (2, "PD03", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (3, "PD04", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (4, "PD05", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (5, "PD06", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (6, "PD07", "DO1", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_7), true, true);
    m_pIOPar->SetGridData (7, "PD08", "DO2", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_8), true, true);
    m_pIOPar->SetGridData (8, "PD09", "DO3", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_9), true, true);
    m_pIOPar->SetGridData (9, "PD10", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (10, "PD11", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (11, "PD12", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (12, "PD13", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (13, "PD14", "DOP3", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_14), true, true);
    m_pIOPar->SetGridData (14, "PD15", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (15, "PD16", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (16, "PD17", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (17, "PD18", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (18, "PD19", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (19, "PD20", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (20, "PD21", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (21, "PD22", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (22, "PD23", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (23, "PD24", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (24, "PD25", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (25, "PD26", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (26, "PD27", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (27, "PD28", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (28, "PD29", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (29, "PD30", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (30, "PD31", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
    m_pIOPar->SetGridData (31, "PD32", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_PARIO_0), false, true);
}

/*
** InitExtension2ParData :
*/
void CDlgHWSSCNETStation::InitExtension2ParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
    CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
    CString strUnitKPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPULSE);
    m_pIOExt2->InitParData (SSCNET_IOEXT2);
    m_pIOExt2->SetGridData (0, "PE01", "FCT1", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_1), true, true);
    m_pIOExt2->SetGridData (1, "PE02", "", "",        LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (2, "PE03", "FCT2", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_2), true, true);
    m_pIOExt2->SetGridData (3, "PE04", "FBN", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_8), true, true);
    m_pIOExt2->SetGridData (4, "PE05", "FBD", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_3), true, true);
    m_pIOExt2->SetGridData (5, "PE06", "BC1", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_4), true, true);
    m_pIOExt2->SetGridData (6, "PE07", "BC2", strUnitRMin,     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_5), true, true);
    m_pIOExt2->SetGridData (7, "PE08", "DUF", strUnitKPulse,   LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_6), true, true);
    m_pIOExt2->SetGridData (8, "PE09", "", strUnitRadSec,      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_7), false, true);
    m_pIOExt2->SetGridData (9, "PE10", "FCT3", "",    LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_9), true, true);
    m_pIOExt2->SetGridData (10, "PE11", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (11, "PE12", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (12, "PE13", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (13, "PE14", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (14, "PE15", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (15, "PE16", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (16, "PE17", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (17, "PE18", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (18, "PE19", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (19, "PE20", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (20, "PE21", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (21, "PE22", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (22, "PE23", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (23, "PE24", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (24, "PE25", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (25, "PE26", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (26, "PE27", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (27, "PE28", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (28, "PE29", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (29, "PE30", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (30, "PE31", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (31, "PE32", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (32, "PE33", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (33, "PE34", "FBN2", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_10), true, true);
    m_pIOExt2->SetGridData (34, "PE35", "FBD2", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_11), true, true);
    m_pIOExt2->SetGridData (35, "PE36", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (36, "PE37", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (37, "PE38", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (38, "PE39", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (39, "PE40", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (40, "PE41", "EOP3", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_12), true, true);
    m_pIOExt2->SetGridData (41, "PE42", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (42, "PE43", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (43, "PE44", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (44, "PE45", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (45, "PE46", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (46, "PE47", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (47, "PE48", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (48, "PE49", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (49, "PE50", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (50, "PE51", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (51, "PE52", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (52, "PE53", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (53, "PE54", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (54, "PE55", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (55, "PE56", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (56, "PE57", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (57, "PE58", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (58, "PE59", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (59, "PE60", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (60, "PE61", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (61, "PE62", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (62, "PE63", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
    m_pIOExt2->SetGridData (63, "PE64", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT2_0), false, true);
}

/*
** InitExtension3ParData :
*/
void CDlgHWSSCNETStation::InitExtension3ParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
    CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
    CString strUnitKPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPULSE);
    m_pIOExt3->InitParData (SSCNET_IOEXT3);
    m_pIOExt3->SetGridData (0, "PF01", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (1, "PF02", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (2, "PF03", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (3, "PF04", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (4, "PF05", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (5, "PF06", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (6, "PF07", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (7, "PF08", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (8, "PF09", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (9, "PF10", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (10, "PF11", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (11, "PF12", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (12, "PF13", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (13, "PF14", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (14, "PF15", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (15, "PF16", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (16, "PF17", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (17, "PF18", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (18, "PF19", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (19, "PF20", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (20, "PF21", "DRT", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_1), true, true);
    m_pIOExt3->SetGridData (21, "PF22", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (22, "PF23", "OSCL1", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_2), true, true);
    m_pIOExt3->SetGridData (23, "PF24", "OSCL2", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_3), true, true);
    m_pIOExt3->SetGridData (24, "PF25", "CVAT", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_4), true, true);
    m_pIOExt3->SetGridData (25, "PF26", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (26, "PF27", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (27, "PF28", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (28, "PF29", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (29, "PF30", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (30, "PF31", "FRIC", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_5), true, true);
    m_pIOExt3->SetGridData (31, "PF32", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (32, "PF33", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (33, "PF34", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (34, "PF35", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (35, "PF36", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (36, "PF37", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (37, "PF38", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (38, "PF39", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (39, "PF40", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (40, "PF41", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (41, "PF42", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (42, "PF43", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (43, "PF44", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (44, "PF45", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (45, "PF46", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (46, "PF47", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (47, "PF48", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
    m_pIOExt3->SetGridData (48, "PF49", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_EXT3_0), false, true);
}

/*
** InitLinearMotorParData :
*/
void CDlgHWSSCNETStation::InitLinearMotorParData ()
{
    CString strUnitPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSE);
    CString strUnitPerc =LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PERC);
    CString strUnitPulseRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_PULSEREV);
    CString strUnitTimes=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_TIMES);
    CString strUnitRadSec=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RADSEC);
    CString strUnitMS=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MS);
    CString strUnitHertz=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_HERTZ);
    CString strUnitKppsPulseRadMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPPSPULSERADSEC);
    CString strUnitRev=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_REV);
CString strUnitRMin=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_RMIN);
    CString strUnitMV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MV);
CString strUnitMM001REV=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MM_001REV);
    CString strUnitKPulse=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_KPULSE);
CString strUnitKMC=LOADSTRING (IDS_CONFIG_HW_SSCNET_PARUNIT_MC);

    m_LinearMotorPar->InitParData (SSCNET_IOEXTLINEARMOTOR);
    m_LinearMotorPar->SetGridData (0, "PL01", "LIT1", "",           LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_1), true, true);
    m_LinearMotorPar->SetGridData (1, "PL02", "LIM", strUnitKMC,            LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_2), true, true);
    m_LinearMotorPar->SetGridData (2, "PL03", "LID", strUnitKMC,    LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_3), true, true);
    m_LinearMotorPar->SetGridData (3, "PL04", "LIT2", "",           LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_4), true, true);
    m_LinearMotorPar->SetGridData (4, "PL05", "LB1", strUnitMM001REV,      LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_5), true, true);
    m_LinearMotorPar->SetGridData (5, "PL06", "LB2", strUnitRMin,   LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_6), true, true);
    m_LinearMotorPar->SetGridData (6, "PL07", "LB3", strUnitPerc,   LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_7), true, true);
    m_LinearMotorPar->SetGridData (7, "PL08", "LIT3", "",           LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_8), true, true);
    m_LinearMotorPar->SetGridData (8, "PL09", "LPWM", strUnitPerc,  LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_9), true, true);
    m_LinearMotorPar->SetGridData (9, "PL10", "", "",      LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (10, "PL11", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (11, "PL12", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (12, "PL13", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (13, "PL14", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (14, "PL15", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (15, "PL16", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (16, "PL17", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (17, "PL18", "LTSTS", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_10), true, true);
    m_LinearMotorPar->SetGridData (18, "PL19", "IDLV", strUnitPerc,     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_11), true, true);
    m_LinearMotorPar->SetGridData (19, "PL20", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (20, "PL21", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (21, "PL22", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (22, "PL23", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (23, "PL24", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (24, "PL25", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (25, "PL26", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (26, "PL27", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (27, "PL28", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (28, "PL29", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (29, "PL30", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (30, "PL31", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (31, "PL32", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (32, "PL33", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (33, "PL34", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (34, "PL35", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (35, "PL36", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (36, "PL37", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (37, "PL38", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (38, "PL39", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (39, "PL40", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (40, "PL41", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (41, "PL42", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (42, "PL43", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (43, "PL44", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (44, "PL45", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (45, "PL46", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (46, "PL47", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (47, "PL48", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
    m_LinearMotorPar->SetGridData (48, "PL49", "", "",     LOADSTRING (IDS_CONFIG_HW_SSCNET_LM_0), false, true);
}

/*
** LoadParValues :
*/
void CDlgHWSSCNETStation::LoadParValues ()
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    int i;

    if (pACF)   {
        for (i=0; i<19; i++)
            m_pBasePar->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 0, i));
        for (i=0; i<45; i++)
            m_pGainFilterPar->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 1, i));
        for (i=0; i<32; i++)
            m_pExtensionPar->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 2, i));
        for (i=0; i<32; i++)
            m_pIOPar->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 3, i));
        if (pACF->GetSSCNETNetworkType (m_nKinemaAddress))   {
            for (i=0; i<64; i++)
                m_pIOExt2->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 4, i));
            for (i=0; i<48; i++)
                m_pIOExt3->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 5, i));
            for (i=0; i<48; i++)
                m_LinearMotorPar->SetParValue (i, pACF->GetSSCNETPar (m_nKinemaAddress, m_nStation, 8, i));
        }
    }
}

/*
** SaveParValues :
*/
void CDlgHWSSCNETStation::SaveParValues ()
{
    WORD w;
    int i;
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    if (pACF)   {
        for (i=0; i<19; i++)   {
            m_pBasePar->GetParValue (i, &w);
            pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 0, i, w);
        }
        for (i=0; i<45; i++)   {
            m_pGainFilterPar->GetParValue (i, &w);
            pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 1, i, w);
        }
        for (i=0; i<32; i++)   {
            m_pExtensionPar->GetParValue (i, &w);
            pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 2, i, w);
        }
        for (i=0; i<32; i++)   {
            m_pIOPar->GetParValue (i, &w);
            pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 3, i, w);
        }
        if (pACF->GetSSCNETNetworkType (m_nKinemaAddress))   {
            for (i=0; i<64; i++)   {
                m_pIOExt2->GetParValue (i, &w);
                pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 4, i, w);
            }
            for (i=0; i<48; i++)   {
                m_pIOExt3->GetParValue (i, &w);
                pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 5, i, w);
            }
            for (i=0; i<48; i++)   {
                m_LinearMotorPar->GetParValue (i, &w);
                pACF->SetSSCNETPar (m_nKinemaAddress, m_nStation, 8, i, w);
            }
        }
    }
}

/*
** Save :
*/
void CDlgHWSSCNETStation::Save ()
{
    SaveData ();
    SaveParValues ();
}

/*
** DownloadSSCNETParams :
*/
void CDlgHWSSCNETStation::DownloadSSCNETParams() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)   {
        WORD *pW=new WORD [NMAXMGA023PARAMETERS*2];
        int nSel=m_pFlatTabCtrl->GetCurSel (), nNumPar;
        switch (nSel)   {
            case 0 :
                m_pBasePar->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PA, pW, nNumPar);
                break;
            case 1 :
                m_pGainFilterPar->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PB, pW, nNumPar);
                break;
            case 2 :
                m_pExtensionPar->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PC, pW, nNumPar);
                break;
            case 3 :
                m_pIOPar->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PD, pW, nNumPar);
                break;
            case 4 :
                m_pIOExt2->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PE, pW, nNumPar);
                break;
            case 5 :
                m_pIOExt3->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PF, pW, nNumPar);
                break;
            case 6 :
                m_LinearMotorPar->GetSelectedParams (pW, &nNumPar);
                DefineType (TYPE_PL, pW, nNumPar);
                break;
        }
        m_pMachine->DownloadSSCNETParameters (m_nKinemaAddress, m_nStation, nNumPar, pW);
        _delete (pW);
    }
}

/*
** UploadSSCNETParams :
*/
void CDlgHWSSCNETStation::UploadSSCNETParams () 
{
	// TODO: Add your control notification handler code here
    int i;

    if (m_pMachine)   {
        CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
        if (pACF)   {
            if (pACF->GetSSCNETNetworkType (m_nKinemaAddress) == 0)   {
                WORD *pwSSCNETParameters=new WORD [NMAXMGA023PARAMETERSTYPES*NMAXMGA023PARAMETERS], *pw;
                pw=pwSSCNETParameters;
                if (m_pMachine->UploadSSCNETParameters (m_nKinemaAddress, 0, m_nStation, pwSSCNETParameters) == S_OK)   {
                    for (i=0; i<19; i++)
                        m_pBasePar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA023PARAMETERS;
                    for (i=0; i<45; i++)
                        m_pGainFilterPar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA023PARAMETERS;
                    for (i=0; i<32; i++)
                        m_pExtensionPar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA023PARAMETERS;
                    for (i=0; i<32; i++)
                        m_pIOPar->SetParValue (i, pwSSCNETParameters[i]);
                    Invalidate ();
                    m_pPrj->SetHWConfigModified ();
                }
                delete pw;
            }
            else   {
                WORD *pwSSCNETParameters=new WORD [NMAXMGA030PARAMETERSTYPES*NMAXMGA030PARAMETERS], *pw;
                pw=pwSSCNETParameters;
                if (m_pMachine->UploadSSCNETParameters (m_nKinemaAddress, 1, m_nStation, pwSSCNETParameters) == S_OK)   {
                    for (i=0; i<19; i++)
                        m_pBasePar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    for (i=0; i<45; i++)
                        m_pGainFilterPar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    for (i=0; i<32; i++)
                        m_pExtensionPar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    for (i=0; i<32; i++)
                        m_pIOPar->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    for (i=0; i<64; i++)
                        m_pIOExt2->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    for (i=0; i<48; i++)
                        m_pIOExt3->SetParValue (i, pwSSCNETParameters[i]);
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    pwSSCNETParameters+=NMAXMGA030PARAMETERS;
                    for (i=0; i<48; i++)
                        m_LinearMotorPar->SetParValue (i, pwSSCNETParameters[i]);
                    Invalidate ();
                    m_pPrj->SetHWConfigModified ();
                }
                delete pw;
            }
        }
    }
}

/*
** InitializeMachine :
*/
void CDlgHWSSCNETStation::InitializeMachine ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)
        m_pMachine=pPrj->GetMachine ();
}

/*
** DefineType :
*/
void CDlgHWSSCNETStation::DefineType (int nType, WORD *pW, int nNumParams)
{
    for (int i=0; i<nNumParams; i+=2)   {
        pW[i]|=nType<<8;
        pW+=2;
    }
}

void CDlgHWSSCNETStation::OnCheckEnableAbsEncoder()
{
	// TODO: Add your control notification handler code here
    if (m_EnableAbsEncoder.GetCheck ())
        m_EditHomePosition.EnableWindow (TRUE);
    else   {
        m_EditHomePosition.EnableWindow (FALSE);
        m_EditHomePosition.SetWindowText ("");
    }
    m_pPrj->SetHWConfigModified ();
}

void CDlgHWSSCNETStation::OnChangeEditHomePosition() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if (m_bInitialized)
		m_pPrj->SetHWConfigModified ();
}

/*
** InitData :
*/
void CDlgHWSSCNETStation::InitData ()
{
    CAdamoHWConfigFile *pCF=m_pPrj->GetHWConfigFile ();
    stHWSSCNETStation &HWSSCNETStation=pCF->GetSSCNETStation (m_nKinemaAddress, m_nStation);
    m_EnableAbsEncoder.SetCheck (HWSSCNETStation.m_bAbsoluteEncoder);
    if (HWSSCNETStation.m_bAbsoluteEncoder)   {
        CString str;
        str.Format ("%8.3f", HWSSCNETStation.m_fpHomePosition);
        m_EditHomePosition.SetWindowText (str);
        m_EditHomePosition.EnableWindow ();
    }
    else   {
        m_EditHomePosition.SetWindowText ("");
        m_EditHomePosition.EnableWindow (FALSE);
    }
}

/*
** SaveData :
*/
void CDlgHWSSCNETStation::SaveData ()
{
    CAdamoHWConfigFile *pCF=m_pPrj->GetHWConfigFile ();
    stHWSSCNETStation &HWSSCNETStation=pCF->GetSSCNETStation (m_nKinemaAddress, m_nStation);
    if (pCF)   {
        if (m_EnableAbsEncoder.GetCheck ())   {
            HWSSCNETStation.m_bAbsoluteEncoder=true;
            CString str;
            double fp;
            char *p;
            m_EditHomePosition.GetWindowText (str);
            str.TrimLeft (); str.TrimRight ();
            fp=strtod (str.GetBuffer (0), &p);
            str.ReleaseBuffer ();
            HWSSCNETStation.m_fpHomePosition=fp;
        }
        else   {
            HWSSCNETStation.m_bAbsoluteEncoder=false;
            HWSSCNETStation.m_fpHomePosition=0.0;
        }
    }
}

/*
** InitIOData  :
*/
void CDlgHWSSCNETStation::InitIOData  ()
{
}

/*
** OnChangePsw :
*/
void CDlgHWSSCNETStation::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitPsw :
*/
void CDlgHWSSCNETStation::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
        GetDlgItem (IDC_CHECK_ENABLE_ABS_ENCODER)->EnableWindow ();
        GetDlgItem (IDC_EDIT_HOME_POSITION)->EnableWindow ();
        m_pBasePar->EnableGrid ();
        m_pGainFilterPar->EnableGrid ();
        m_pExtensionPar->EnableGrid ();
        m_pIOPar->EnableGrid ();
    }
    else   {
        GetDlgItem (IDC_CHECK_ENABLE_ABS_ENCODER)->EnableWindow (false);
        GetDlgItem (IDC_EDIT_HOME_POSITION)->EnableWindow (false);
        m_pBasePar->EnableGrid (false);
        m_pGainFilterPar->EnableGrid (false);
        m_pExtensionPar->EnableGrid (false);
        m_pIOPar->EnableGrid (false);
    }
}
