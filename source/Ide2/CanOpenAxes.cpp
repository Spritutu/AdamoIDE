// CanOpenDigitalIO.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "canopenaxes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanOpenDigitalIO dialog


CCanOpenAxes::CCanOpenAxes(CWnd* pParent /*=NULL*/)
	: CDialog(CCanOpenAxes::IDD, pParent), m_pConfigDoc (NULL), m_bInitialized (false), m_pPrj (NULL), m_pFont (NULL), m_ePswLevel (ePswUser)
{
	//{{AFX_DATA_INIT(CCanOpenDigitalIO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCanOpenAxes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
 	//{{AFX_DATA_MAP(CCanOpenAxes)
	DDX_Control(pDX, IDC_ST_CANOPEN_ASSE, m_stAxesTitle);
	DDX_Control(pDX, IDC_CHECK_ENABLE_DSP402, m_EnableDSP402);
	DDX_Control(pDX, IDC_ST_CANOPEN_DSP402, m_stDSP402Title);
	DDX_Control(pDX, IDC_ST_CANOPEN_VARCONTROL, m_stVarControl);
	DDX_Control(pDX, IDC_ST_CANOPEN_VARCOMMAND, m_stVarCommand);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanOpenAxes, CDialog)
	//{{AFX_MSG_MAP(CCanOpenDigitalIO)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_CANOPEN_16BIT, OnRadio16bit)
	ON_BN_CLICKED(IDC_RADIO_CANOPEN_32BIT, OnRadio32bit)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_DSP402, OnCheckDSP402)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanOpenDigitalIO message handlers

BOOL CCanOpenAxes::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    m_GridAxes.SetConfigDoc   (m_pConfigDoc);
    m_GridAxes.SetProject   (m_pPrj);
	InitializeGrid (m_GridAxes, IDC_GRID_HW_CANOPEN_AXES, 1, 0);
	InitializeCommandGridObjects (IDC_GRID_HW_CANOPEN_COMMAND_OBJECTS);
	InitializeControlGridObjects (IDC_GRID_HW_CANOPEN_CONTROL_OBJECTS);
	InitColumnGridCommandCaptions ();
	InitColumnGridControlCaptions ();
	InitializeGridData ();
    InitializeTitles ();
    InitData ();
    InitPsw ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitializeGrid
*/
void CCanOpenAxes::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_GridAxes.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_GridAxes.SetMode (nMode);
    m_GridAxes.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_GridAxes.SetEditable(false);
    m_GridAxes.SetVirtualMode(false);
    m_GridAxes.SetFixedColumnCount(1);
    m_GridAxes.SetColumnCount(3);
    m_GridAxes.SetFixedRowCount ();
    m_GridAxes.SetRowCount (nRow+1);
    m_GridAxes.SetEditable ();
    m_GridAxes.EnableDragAndDrop ();
    m_GridAxes.SetListMode ();
    m_GridAxes.FillGridCaption ();
    for (int i=1; i<nRow+1; i++)
        m_GridAxes.SetCellType (i, 1, RUNTIME_CLASS (CConfigHWGridCell));
}

/*
** InitializeGridObjects :
*/
void CCanOpenAxes::InitializeCommandGridObjects (int nID)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_gridCommandObjects.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_gridCommandObjects.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_gridCommandObjects.SetVirtualMode(false);
    m_gridCommandObjects.SetFixedColumnCount(1);
    m_gridCommandObjects.SetColumnCount(5);
    m_gridCommandObjects.SetFixedRowCount ();
    m_gridCommandObjects.SetRowCount (4);
    m_gridCommandObjects.SetEditable ();
    m_gridCommandObjects.SetListMode ();
	m_gridCommandObjects.SetProject (m_pPrj);
}

/*
** InitializeGridObjects :
*/
void CCanOpenAxes::InitializeControlGridObjects (int nID)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_gridControlObjects.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_gridControlObjects.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_gridControlObjects.SetVirtualMode(false);
    m_gridControlObjects.SetFixedColumnCount(1);
    m_gridControlObjects.SetColumnCount(3);
    m_gridControlObjects.SetFixedRowCount ();
    m_gridControlObjects.SetRowCount (3);
    m_gridControlObjects.SetEditable ();
    m_gridControlObjects.SetListMode ();
	m_gridControlObjects.SetProject (m_pPrj);
}

/*
** InitializeTitles :
*/
void CCanOpenAxes::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_TITLE_CANOPEN_DSP402);
    m_stDSP402Title.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_CANOPEN_AXES);
    m_stAxesTitle.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_CANOPEN_ENABLE_DSP402);
    m_EnableDSP402.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_CANOPEN_VARCONTROL);
    m_stVarControl.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_CANOPEN_VARCOMMAND);
    m_stVarCommand.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_CANOPEN_16BIT);
	GetDlgItem (IDC_RADIO_CANOPEN_16BIT)->SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_CANOPEN_32BIT);
	GetDlgItem (IDC_RADIO_CANOPEN_32BIT)->SetWindowText (str);
}

/*
** OnSize :
*/
void CCanOpenAxes::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
		int n=(cx-20-50)/8;
		if (n > 0)   {
			m_stAxesTitle.MoveWindow (10, 2, 100, 20);
			m_stDSP402Title.MoveWindow (cx-200, 2, 190, 20);
			m_EnableDSP402.MoveWindow (cx-200, 19, 190, 20);
			m_stVarControl.MoveWindow (10, 82, 190, 20);
			m_stVarCommand.MoveWindow (10, 182, 190, 20);
			GetDlgItem (IDC_RADIO_CANOPEN_16BIT)->MoveWindow (cx - 200, 200, 100, 25);
			GetDlgItem (IDC_RADIO_CANOPEN_32BIT)->MoveWindow (cx - 200, 225, 100, 20);
			m_GridAxes.MoveWindow (10, 22, cx-229, 2*19+10);
			m_GridAxes.ExpandLastColumn();
			((CWnd &)m_GridAxes).EnableScrollBarCtrl (SB_HORZ, FALSE);
			((CWnd &)m_GridAxes).EnableScrollBarCtrl (SB_VERT, FALSE);
			m_gridControlObjects.MoveWindow (10, 100, cx/2, 3*19+13);
			m_gridControlObjects.SetColumnWidth (1, ((cx/2) - 10 - m_gridControlObjects.GetColumnWidth (0))/2);
			m_gridControlObjects.ExpandLastColumn();
			((CWnd &)m_gridControlObjects).EnableScrollBarCtrl (SB_HORZ, FALSE);
			((CWnd &)m_gridControlObjects).EnableScrollBarCtrl (SB_VERT, FALSE);
			m_gridCommandObjects.MoveWindow (10, 200, cx-229, 4*19+16);
			m_gridCommandObjects.SetColumnWidth (0, 50);
			m_gridCommandObjects.SetColumnWidth (1, n*2);
			m_gridCommandObjects.SetColumnWidth (2, n);
			m_gridCommandObjects.SetColumnWidth (3, n*2);
			m_gridCommandObjects.ExpandColumnsToFit();
		}
	}
}

/*
** InitializaGridData :
*/
void CCanOpenAxes::InitializeGridData ()
{
    CString str;
    str.Format ("C%dR%dAx1", m_nKinemaAddress+1, m_nNode);
    m_GridAxes.SetGridData (1, 1, 1, 2, str);
    m_GridAxes.SetItemText (1, 0, "Ax0");
    m_GridAxes.SetType (enAsse);
    m_GridAxes.UpdateData ();
	m_gridCommandObjects.SetItemText (1, 0, LOADSTRING (IDS_CONFIG_HW_CANOPEN_AXES_POSITION));
	m_gridCommandObjects.SetItemText (2, 0, LOADSTRING (IDS_CONFIG_HW_CANOPEN_AXES_SPEED));
	m_gridCommandObjects.SetItemText (3, 0, LOADSTRING (IDS_CONFIG_HW_CANOPEN_AXES_TORQUE));
	m_gridControlObjects.SetItemText (1, 0, LOADSTRING (IDS_CONFIG_HW_CANOPEN_STATUS));
	m_gridControlObjects.SetItemText (2, 0, LOADSTRING (IDS_CONFIG_HW_CANOPEN_CONTROL));
}

/*
** RemoveCurrentConnection :
*/
void CCanOpenAxes::RemoveCurrentConnection ()
{
    m_GridAxes.RemoveCurrentConnection ();
}

/*
** Find :
*/
void CCanOpenAxes::Find (const char *szBuffer)
{
    m_GridAxes.Find (szBuffer);
}

/*
** OnDestroy :
*/
void CCanOpenAxes::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_GridAxes.DestroyWindow ();
}

/*
** OnOK :
*/
void CCanOpenAxes::OnOK ()
{
}

/*
** OnCancel :
*/
void CCanOpenAxes::OnCancel ()
{
}

/*
** CreateFont :
*/
void CCanOpenAxes::CreateFont ()
{
    m_pFont=((CMainFrame *) AfxGetMainWnd ())->CreateFont ("Tahoma", 18, FW_NORMAL);
    m_stDSP402Title.SetFont (m_pFont);
    m_stAxesTitle.SetFont (m_pFont);
}

/*
** Save :
*/
void CCanOpenAxes::Save ()
{
	CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
	/* verifichiamo se il flag di DSP402 e' settato */
	int nDSP402 = m_EnableDSP402.GetCheck ();
	pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsDSP402Node = nDSP402 != 0;
	if (nDSP402 != 0)   {
		/* siamo in modalita DSP402, assegniamo noi gli oggetti */
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord[0] = STATUS_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord[1] = STATUS_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nControlWord[0] = CONTROL_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nControlWord[1] = CONTROL_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nPositionActualValue[0] = POSITION_ACTUAL_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nPositionActualValue[1] = POSITION_ACTUAL_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetPosition[0] = POSITION_TARGET_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetPosition[1] = POSITION_TARGET_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nSpeedActualValue[0] = SPEED_ACTUAL_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nSpeedActualValue[1] = SPEED_ACTUAL_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetSpeed[0] = SPEED_TARGET_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetSpeed[1] = SPEED_TARGET_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTorqueActualValue[0] = TORQUE_ACTUAL_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTorqueActualValue[1] = TORQUE_ACTUAL_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetTorque[0] = TORQUE_TARGET_INDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetTorque[1] = TORQUE_TARGET_SUBINDEX;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nImageTypeInput = IDETHTYPE_DINT;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nImageTypeOutput = IDETHTYPE_DINT;
	}
	else   {
		AssignObjects (&m_gridControlObjects, 1, 1, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord[0]);
		AssignObjects (&m_gridControlObjects, 1, 2, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord[1]);
		AssignObjects (&m_gridControlObjects, 2, 1, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nControlWord[0]);
		AssignObjects (&m_gridControlObjects, 2, 2, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nControlWord[1]);
		AssignObjects (&m_gridCommandObjects, 1, 1, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nPositionActualValue[0]);
		AssignObjects (&m_gridCommandObjects, 1, 2, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nPositionActualValue[1]);
		AssignObjects (&m_gridCommandObjects, 1, 3, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetPosition[0]);
		AssignObjects (&m_gridCommandObjects, 1, 4, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetPosition[1]);
		AssignObjects (&m_gridCommandObjects, 2, 1, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nSpeedActualValue[0]);
		AssignObjects (&m_gridCommandObjects, 2, 2, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nSpeedActualValue[1]);
		AssignObjects (&m_gridCommandObjects, 2, 3, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetSpeed[0]);
		AssignObjects (&m_gridCommandObjects, 2, 4, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetSpeed[1]);
		AssignObjects (&m_gridCommandObjects, 3, 1, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTorqueActualValue[0]);
		AssignObjects (&m_gridCommandObjects, 3, 2, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTorqueActualValue[1]);
		AssignObjects (&m_gridCommandObjects, 3, 3, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetTorque[0]);
		AssignObjects (&m_gridCommandObjects, 3, 4, &pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetTorque[1]);
		int nValue, nCheck = GetCheckedRadioButton (IDC_RADIO_CANOPEN_16BIT, IDC_RADIO_CANOPEN_32BIT);
		switch (nCheck)   {
			case IDC_RADIO_CANOPEN_16BIT :
				nValue = IDETHTYPE_DINT;
				break;
			case IDC_RADIO_CANOPEN_32BIT :
				nValue = IDETHTYPE_DINT;
				break;
		}
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nImageTypeInput = nValue;
		pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nImageTypeOutput = nValue;
	}
}

/*
** AssignObjects :
*/
void CCanOpenAxes::AssignObjects (CGridCtrl *pGrid, int nRow, int nCol, int *value)
{
	CString str;
	char *p;

	str = pGrid->GetItemText (nRow, nCol);
	*value = strtol (str, &p, 0);
}

/*
** InitData :
*/
void CCanOpenAxes::InitData ()
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
	int nDSP402 = pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsDSP402Node;
	m_EnableDSP402.SetCheck (nDSP402 != 0);
	if (nDSP402 != 0)
		SetDSP402Objects ();
	else
		LoadDSP402Objects (pACF);
	int nTypeInput = pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nImageTypeInput;
	if (nTypeInput == IDETHTYPE_DINT)   {
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_16BIT))->SetCheck (false);
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_32BIT))->SetCheck (true);
	}
	else   {
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_16BIT))->SetCheck (true);
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_32BIT))->SetCheck (false);
	}
}

/*
** InitColumnGridCommandCaptions :
*/
void CCanOpenAxes::InitColumnGridCommandCaptions ()
{
    GV_ITEM Item;
    CString str;
    CBitmap bm;

	m_ImageList.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_SDOREADSYMBOLS);
    m_ImageList.Add(&bm, RGB(0, 128, 128));
	m_gridCommandObjects.SetImageList(&m_ImageList);
	/* prima colonna, indice valore attuale */
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col=1;
    Item.iImage=2;
    str=LOADSTRING (IDS_CONFIG_HW_CANOPEN_INDEX_ACTUAL_VALUE);
    Item.strText=str.GetBuffer (0);
    str.ReleaseBuffer ();
    m_gridCommandObjects.SetItem(&Item);
	/* seconda colonna, sottoindice valore attuale */
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col=2;
    Item.iImage=3;
    str=LOADSTRING (IDS_CONFIG_HW_CANOPEN_SUBINDEX_ACTUAL_VALUE);
    Item.strText=str.GetBuffer (0);
    str.ReleaseBuffer ();
    m_gridCommandObjects.SetItem(&Item);
	/* terza colonna, indice valore target */
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col=3;
    Item.iImage=2;
    str=LOADSTRING (IDS_CONFIG_HW_CANOPEN_INDEX_TARGET_VALUE);
    Item.strText=str.GetBuffer (0);
    str.ReleaseBuffer ();
    m_gridCommandObjects.SetItem(&Item);
	/* quarta colonna, sottoindice valore target */
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col=4;
    Item.iImage=3;
    str=LOADSTRING (IDS_CONFIG_HW_CANOPEN_SUBINDEX_TARGET_VALUE);
    Item.strText=str.GetBuffer (0);
    str.ReleaseBuffer ();
    m_gridCommandObjects.SetItem(&Item);
}

/*
** InitColumnGridControlCaptions :
*/
void CCanOpenAxes::InitColumnGridControlCaptions ()
{
    GV_ITEM Item;
    CString str;
    CBitmap bm;

	m_gridControlObjects.SetImageList(&m_ImageList);
	/* prima colonna, indice valore attuale */
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col=1;
    Item.iImage=2;
    str=LOADSTRING (IDS_CONFIG_HW_CANOPEN_INDEX_ACTUAL_VALUE);
    Item.strText=str.GetBuffer (0);
    str.ReleaseBuffer ();
    m_gridControlObjects.SetItem(&Item);
	/* seconda colonna, sottoindice valore attuale */
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col=2;
    Item.iImage=3;
    str=LOADSTRING (IDS_CONFIG_HW_CANOPEN_SUBINDEX_ACTUAL_VALUE);
    Item.strText=str.GetBuffer (0);
    str.ReleaseBuffer ();
    m_gridControlObjects.SetItem(&Item);
}

/*
** OnRadio16bit :
*/
void CCanOpenAxes::OnRadio16bit ()
{
	m_pPrj->SetHWConfigModified ();	
}

/*
** OnRadio32bit :
*/
void CCanOpenAxes::OnRadio32bit ()
{
	m_pPrj->SetHWConfigModified ();	
}

/*
** SetDSP402Objects :
*/
void CCanOpenAxes::SetDSP402Objects ()
{
	CString str;
	str.Format ("0x%x", STATUS_INDEX);
	m_gridControlObjects.GetCell (1, 1)->SetText (str);
	m_gridControlObjects.GetCell (1, 2)->SetText ("0");
	str.Format ("0x%x", CONTROL_INDEX);
	m_gridControlObjects.GetCell (2, 1)->SetText (str);
	m_gridControlObjects.GetCell (2, 2)->SetText ("0");
	str.Format ("0x%x", POSITION_ACTUAL_INDEX);
	m_gridCommandObjects.GetCell (1, 1)->SetText (str);
	m_gridCommandObjects.GetCell (1, 2)->SetText ("0");
	str.Format ("0x%x", POSITION_TARGET_INDEX);
	m_gridCommandObjects.GetCell (1, 3)->SetText (str);
	m_gridCommandObjects.GetCell (1, 4)->SetText ("0");
	str.Format ("0x%x", SPEED_ACTUAL_INDEX);
	m_gridCommandObjects.GetCell (2, 1)->SetText (str);
	m_gridCommandObjects.GetCell (2, 2)->SetText ("0");
	str.Format ("0x%x", SPEED_TARGET_INDEX);
	m_gridCommandObjects.GetCell (2, 3)->SetText (str);
	m_gridCommandObjects.GetCell (2, 4)->SetText ("0");
	str.Format ("0x%x", TORQUE_ACTUAL_INDEX);
	m_gridCommandObjects.GetCell (3, 1)->SetText (str);
	m_gridCommandObjects.GetCell (3, 2)->SetText ("0");
	str.Format ("0x%x", TORQUE_TARGET_INDEX);
	m_gridCommandObjects.GetCell (3, 3)->SetText (str);
	m_gridCommandObjects.GetCell (3, 4)->SetText ("0");
	m_gridControlObjects.Invalidate ();
	m_gridCommandObjects.Invalidate ();
	m_gridControlObjects.EnableWindow (FALSE);
	m_gridCommandObjects.EnableWindow (FALSE);
}

/*
** OnCheckDSP402 :
*/
void CCanOpenAxes::OnCheckDSP402 ()
{
	CString str;

	if (m_EnableDSP402.GetCheck () != 0)   {
		str = LOADSTRING (IDS_CONFIG_HW_CANOPEN_OVERWRITE_OBJECTS);
		if (AfxMessageBox (str, MB_YESNO) == IDYES)   {
			SetDSP402Objects ();			
		}
	}
	else   {
		m_gridControlObjects.EnableWindow ();
		m_gridCommandObjects.EnableWindow ();
	}
	m_pPrj->SetHWConfigModified ();	
}

/*
** LoadDSP402Objects :
*/
void CCanOpenAxes::LoadDSP402Objects (CAdamoHWConfigFile *pACF)
{
	CString str;

	pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord;
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord[0]);
	m_gridControlObjects.GetCell (1, 1)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nStatusWord[1]);
	m_gridControlObjects.GetCell (1, 2)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nControlWord[0]);
	m_gridControlObjects.GetCell (2, 1)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nControlWord[1]);
	m_gridControlObjects.GetCell (2, 2)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nPositionActualValue[0]);
	m_gridCommandObjects.GetCell (1, 1)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nPositionActualValue[1]);
	m_gridCommandObjects.GetCell (1, 2)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetPosition[0]);
	m_gridCommandObjects.GetCell (1, 3)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetPosition[1]);
	m_gridCommandObjects.GetCell (1, 4)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nSpeedActualValue[0]);
	m_gridCommandObjects.GetCell (2, 1)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nSpeedActualValue[1]);
	m_gridCommandObjects.GetCell (2, 2)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetSpeed[0]);
	m_gridCommandObjects.GetCell (2, 3)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetSpeed[1]);
	m_gridCommandObjects.GetCell (2, 4)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTorqueActualValue[0]);
	m_gridCommandObjects.GetCell (3, 1)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTorqueActualValue[1]);
	m_gridCommandObjects.GetCell (3, 2)->SetText (str);
	str.Format ("0x%x", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetTorque[0]);
	m_gridCommandObjects.GetCell (3, 3)->SetText (str);
	str.Format ("%d", pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).nTargetTorque[1]);
	m_gridCommandObjects.GetCell (3, 4)->SetText (str);
	m_gridControlObjects.Invalidate ();
	m_gridCommandObjects.Invalidate ();
	m_gridControlObjects.EnableWindow ();
	m_gridCommandObjects.EnableWindow ();
}

/*
** OnChangePsw :
*/
void CCanOpenAxes::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitPsw :
*/
void CCanOpenAxes::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
        m_EnableDSP402.EnableWindow ();
        m_gridControlObjects.EnableWindow ();
		m_gridCommandObjects.EnableWindow ();
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_16BIT))->EnableWindow ();
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_32BIT))->EnableWindow ();
    }
    else   {
        m_EnableDSP402.EnableWindow (false);
        m_gridControlObjects.EnableWindow (false);
		m_gridCommandObjects.EnableWindow (false);
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_16BIT))->EnableWindow (false);
		((CButton *)GetDlgItem (IDC_RADIO_CANOPEN_32BIT))->EnableWindow (false);
    }
}
