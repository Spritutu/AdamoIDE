// AdamoEthercatAssi.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoEthercatAssi.h"
#include "GridCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatAssi dialog

IMPLEMENT_DYNAMIC(CAdamoEthercatAssi, CDialog)

CAdamoEthercatAssi::CAdamoEthercatAssi(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoEthercatAssi::IDD, pParent), m_pDoc (NULL), m_nNode (-1), m_bInitialized (false), m_nTab (-1), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CAdamoEthercatAssi)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for (int i=0; i<AX_PER_SLAVE; i++)
		bAxConnected[i]=false;
}


void CAdamoEthercatAssi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoEthercatAssi)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_ST_ASSI, m_stAssi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoEthercatAssi, CDialog)
	//{{AFX_MSG_MAP(CAdamoEthercatAssi)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatAssi message handlers

/* construction / destruction */
CAdamoEthercatAssiGrid::CAdamoEthercatAssiGrid () : m_pEth (NULL)
{
}

CAdamoEthercatVariabiliAssiGrid::CAdamoEthercatVariabiliAssiGrid () : m_pEth (NULL), m_bOutput (false)
{
}

/*
** FillGridCaption :
*/
void CAdamoEthercatAssiGrid::FillGridCaption ()
{
    GV_ITEM Item;
    CString str;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_CONFIG_HW_GRID), 16, 1, RGB(0,128,128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
    Item.iImage = 0;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_NOME);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 2;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
    SetItem(&Item);
}

/*
** FillGridCaption :
*/
void CAdamoEthercatVariabiliAssiGrid::FillGridCaption ()
{
    GV_ITEM Item;
    CString str;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_CONFIG_HW_GRID), 16, 1, RGB(0,128,128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
    Item.iImage = 0;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_STATUS_VAR);
    SetItem(&Item);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 2;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_POSIZIONE);
    SetItem(&Item);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 3;
    Item.iImage = 2;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_CONTROL_VAR);
    SetItem(&Item);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 4;
    Item.iImage = 3;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_POSIZIONE);
    SetItem(&Item);
}

/*
** Find :
*/
void CAdamoEthercatAssi::Find (const char *szBuffer)
{
    m_GridAssi.Find (szBuffer);
}

/*
** RemoveCurrentConnection :
*/
void CAdamoEthercatAssi::RemoveCurrentConnection ()
{
	m_GridAssi.RemoveCurrentConnection ();
}

/*
** OnInitDialog :
*/
BOOL CAdamoEthercatAssi::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    m_GridAssi.SetConfigDoc (m_pDoc);
    m_GridAssi.SetProject (m_pPrj);
	LoadSlaveInfo ();
	InitializeStrings ();
	m_GridVarOutput.SetOutputGrid ();
	InitializeGrid (m_GridAssi, IDC_GRID_HW_ASSI, AX_PER_SLAVE, 2, 5);
	InitializeGridVariabili (m_GridVarInput, IDC_GRID_HW_VAR_INPUT, AX_PER_SLAVE, 4, 6);
	InitializeGridVariabili (m_GridVarOutput, IDC_GRID_HW_VAR_OUTPUT, AX_PER_SLAVE, 4, 7);
    InitializeGridData ();
	InitializeGridVariabiliData (m_GridVarInput);
	InitializeGridVariabiliData (m_GridVarOutput);
    InitializeTitles ();
	InitPsw ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** LoadSlaveInfo :
*/
void CAdamoEthercatAssi::LoadSlaveInfo ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
	if (pPrj)   {
		CPtrArray &rSlaveArray=pPrj->GetSlaveArray ();
		m_pSlave=(stAdamoEthSlave *)rSlaveArray[m_nNode-1];
	}
}

/*
** InitializeGrid
*/
void CAdamoEthercatAssi::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.SetMode (nMode);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.SetEditable(false);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(nCol+1);
    m_Grid.SetFixedRowCount ();
    m_Grid.SetRowCount (nRow+1);
    m_Grid.SetEditable ();
    m_Grid.EnableDragAndDrop ();
    m_Grid.FillGridCaption ();
	m_Grid.SetListMode ();
    m_Grid.SetColumnResize (TRUE);
    for (int i=1; i<nRow+1; i++)
        for (int n=1; n<nCol+1; n++)
            m_Grid.SetCellType (i, n, RUNTIME_CLASS(CConfigHWGridCell));
	((CAdamoEthercatAssiGrid &)m_Grid).SetEthAssi (this);
}

/*
** InitializeGridVariabili :
*/
void CAdamoEthercatAssi::InitializeGridVariabili (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.SetMode (nMode);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.SetEditable(true);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(nCol+1);
    m_Grid.SetFixedRowCount ();
    m_Grid.SetRowCount (nRow+1);
    m_Grid.SetEditable ();
    m_Grid.FillGridCaption ();
    m_Grid.SetColumnResize (FALSE);
	((CAdamoEthercatAssiGrid &)m_Grid).SetEthAssi (this);
	if (nID==IDC_GRID_HW_VAR_INPUT)
		m_Grid.SetItemText (0, 0, LOADSTRING (IDS_EC_GRID_ASSI_INPUT));
	else
		m_Grid.SetItemText (0, 0, LOADSTRING (IDS_EC_GRID_ASSI_OUTPUT));
}

/*
** InitializeGridData :
*/
void CAdamoEthercatAssi::InitializeGridData ()
{
	CString str;

	for (int i=0; i<AX_PER_SLAVE; i++)   {
		str.Format ("E1R%dAx%d", m_nNode, i+1);
		m_GridAssi.SetGridData (i+1, 1, i+1, 2, str);
		str.Format ("Ax%d", i+1);
		m_GridAssi.SetItemText (i+1, 0, str);
		m_GridAssi.SetItemData (i+1, 1, -1);
	}
    m_GridAssi.SetType (enAsse);
    m_GridAssi.UpdateData ();
}

/*
** InitializeGridVariabiliData :
*/
void CAdamoEthercatAssi::InitializeGridVariabiliData (CConfigHWGrid& m_Grid)
{
	CString str;
	int i;

	for (i=0; i<AX_PER_SLAVE; i++)   {
		str.Format ("Ax%d", i+1);
		m_Grid.SetItemText (i+1, 0, str);
	}
	m_GridVarInput.UpdateData ();
	m_GridVarOutput.UpdateData ();
}

/*
** InitializeTitles :
*/
void CAdamoEthercatAssi::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_TITLE_ASSI);
    m_stAssi.SetWindowText (str);
}

/*
** InitializeStrings :
*/
void CAdamoEthercatAssi::InitializeStrings ()
{
	POSITION pos;
	int i, n;

	m_slVarInput.SetSize (1024);
	i=0; n=0;
	for (pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (pVar->nInputOutput==0)   {
			if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))
				m_slVarInput[i++]=pVar->szName;
		}
	}
	m_slVarOutput.SetSize (1024);
	i=0; n=0;
	for (pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (pVar->nInputOutput==1)
			if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))
				m_slVarOutput[i++]=pVar->szName;
	}
}

/*
** OnSize :
*/
void CAdamoEthercatAssi::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_stAssi.MoveWindow (10, 2, 100, 16);
	    m_GridAssi.MoveWindow (10, 16, cx-20, 9*19+5);
        m_GridAssi.SetColumnWidth (0, 50);
        m_GridAssi.SetColumnWidth (1, 200);
        m_GridAssi.ExpandLastColumn ();
		int n=(cx-20-50)/8;
		if (n>0)   {
			m_GridVarInput.MoveWindow (10, 194, cx-20, 9*19+5);
			m_GridVarInput.SetColumnWidth (0, 50);
			m_GridVarInput.SetColumnWidth (1, n*2);
			m_GridVarInput.SetColumnWidth (2, n);
			m_GridVarInput.SetColumnWidth (3, n*2);
			m_GridVarInput.ExpandLastColumn ();
			m_GridVarOutput.MoveWindow (10, 372, cx-20, 9*19+5);
			m_GridVarOutput.SetColumnWidth (0, 50);
			m_GridVarOutput.SetColumnWidth (1, n*2);
			m_GridVarOutput.SetColumnWidth (2, n);
			m_GridVarOutput.SetColumnWidth (3, n*2);
			m_GridVarOutput.ExpandLastColumn ();
		}
    }
}

/*
** OnConnected :
*/
int CAdamoEthercatAssiGrid::OnConnected (int nRow, int nCol, const char *szHWAddress)
{
	/* marchiamolo come connesso */
	m_pEth->ConnectAx (nRow-1, true);
	m_pEth->OnConnected (nRow, nCol, szHWAddress);
	return 0;
}

/*
** OnConnected :
*/
int CAdamoEthercatAssiGrid::OnRemoveConnection (int nRow, int nCol, const char *szHWAddress)
{
	/* marchiamolo come connesso */
	int nID=GetItemData (nRow, 1);
	return m_pEth->OnRemoveConnection (nRow, nID);
}

/*
** OnEditCell :
*/
void CAdamoEthercatVariabiliAssiGrid::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
    if ((nCol==1 || nCol==3) && m_pEth->IsAxConnected(nRow-1))
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

/*
** OnEndEditCell :
*/
void CAdamoEthercatVariabiliAssiGrid::OnEndEditCell (int nRow, int nCol, CString str)
{
	CString strPosition;

	if (nCol==1 || nCol==3)   {
		stAdamoEthSlaveVariable *pVar;
		int nBase;

		if (nCol==1 || nCol==3)   {
			if (m_bOutput)   {
				pVar=m_pEth->GetSlave ()->FindOutputVariable (str);
				nBase=2;
			}
			else   {
				pVar=m_pEth->GetSlave ()->FindInputVariable (str);
				nBase=0;
			}
		}
		if (pVar)   {
			switch (nCol)   {
			case 1 :
				StoreEthercatInfo (pVar, nBase, m_pEth->GetGridAssi ()->GetItemData (nRow, 1));
				break;
			case 3 :
				StoreEthercatInfo (pVar, nBase+1, m_pEth->GetGridAssi ()->GetItemData (nRow, 1));
				break;
			}
			strPosition.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
			SetItemText (nRow, nCol+1, strPosition);
			m_pEth->GetProject ()->SetHWConfigModified ();
		}
		CGridCtrl::OnEndEditCell (nRow, nCol, str);
		Invalidate ();
	}
}

/*
** OnConnected :
*/
int CAdamoEthercatVariabiliAssiGrid::OnConnected (int nRow, int nCol, const char *szHWAddress)
{
	CString str;

	/* ora mettiamo i combo */
    SetCellType(nRow, 1, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(nRow, 3, RUNTIME_CLASS(CGridCellCombo));
	/* riempiamo i combo */
	if (m_bOutput)   {
		((CGridCellCombo*)GetCell(nRow, 1))->SetOptions(m_pEth->GetSLVarOutput ());
		((CGridCellCombo*)GetCell(nRow, 3))->SetOptions(m_pEth->GetSLVarOutput ());
	}
	else   {
		((CGridCellCombo*)GetCell(nRow, 1))->SetOptions(m_pEth->GetSLVarInput ());
		((CGridCellCombo*)GetCell(nRow, 3))->SetOptions(m_pEth->GetSLVarInput ());
	}
    ((CGridCellCombo*)GetCell(nRow, 1))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(nRow, 3))->SetStyle (CBS_DROPDOWNLIST);
	return 0;
}

/*
** OnConnected :
*/
int  CAdamoEthercatAssi::OnConnected (int nRow, int nCol, const char *szHWAddress)
{
	m_GridVarInput.OnConnected (nRow, nCol, szHWAddress);
	m_GridVarOutput.OnConnected (nRow, nCol, szHWAddress);
	return 0;
}

/*
** StoreEthercatInfo :
*/
void CAdamoEthercatVariabiliAssiGrid::StoreEthercatInfo (stAdamoEthSlaveVariable *pVar, int nBase, DWORD dw)
{
	m_pEth->StoreEthercatInfo (pVar, nBase, dw);
}

/*
** StoreEthercatInfo :
*/
void CAdamoEthercatAssi::StoreEthercatInfo (stAdamoEthSlaveVariable *pVar, int nBase, DWORD dw)
{
	if (m_pDoc)   {
		CAdamoHWConfigFile *pHWFile=m_pPrj->GetHWConfigFile ();
		if (pHWFile)
			pHWFile->SetVariableData (dw, nBase, pVar->nBitOffs, pVar->nBitSize, pVar->nDataType);
	}
}

/*
** UpdateData :
*/
void CAdamoEthercatVariabiliAssiGrid::UpdateData ()
{
	stAdamoEthSlaveVariable *pVar;
	CString str;
	int nBitOffs, nBitSize, nDataType, i, nID;
	bool bConfigured;

	/* prendiamo per prima cosa offset e lunghezza */
	CAdamoHWConfigFile *pHWFile=m_pEth->GetHWConfigFile ();
	if (pHWFile)   {
		for (i=0; i<AX_PER_SLAVE; i++)   {
			nID=m_pEth->GetGridAssi()->GetItemData (i+1, 1);
			if (nID!=-1)   {
				if (m_bOutput)   {
					pHWFile->GetVariableData (nID, 2, &bConfigured, &nBitOffs, &nBitSize, &nDataType);
					if (bConfigured)   {
						pVar=m_pEth->GetSlave ()->FindOutputVariable (nBitOffs, nBitSize);
						if (pVar)   {
							SetItemText (i+1, 1, pVar->szName);
							str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
							SetItemText (i+1, 2, str);
						}
					}
					pHWFile->GetVariableData (nID, 3, &bConfigured, &nBitOffs, &nBitSize, &nDataType);
					if (bConfigured)   {
						pVar=m_pEth->GetSlave ()->FindOutputVariable (nBitOffs, nBitSize);
						if (pVar)   {
							SetItemText (i+1, 3, pVar->szName);
							str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
							SetItemText (i+1, 4, str);
						}
					}
				}
				else   {
					pHWFile->GetVariableData (nID, 0, &bConfigured, &nBitOffs, &nBitSize, &nDataType);
					if (bConfigured)   {
						pVar=m_pEth->GetSlave ()->FindInputVariable (nBitOffs, nBitSize);
						if (pVar)   {
							SetItemText (i+1, 1, pVar->szName);
							str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
							SetItemText (i+1, 2, str);
						}
					}
					pHWFile->GetVariableData (nID, 1, &bConfigured, &nBitOffs, &nBitSize, &nDataType);
					if (bConfigured)   {
						pVar=m_pEth->GetSlave ()->FindInputVariable (nBitOffs, nBitSize);
						if (pVar)   {
							SetItemText (i+1, 3, pVar->szName);
							str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
							SetItemText (i+1, 4, str);
						}
					}
				}
			}
		}
	}
}

/*
** OnRemoveConnection :
*/
int CAdamoEthercatAssi::OnRemoveConnection (int nRow, int nID)
{
	CAdamoHWConfigFile *pHWConfigFile=GetHWConfigFile ();
	if (pHWConfigFile)   {
		pHWConfigFile->RemoveVariableData (nID);
		m_GridVarInput.SetItemText (nRow, 1, "");
		m_GridVarInput.SetItemText (nRow, 2, "");
		m_GridVarInput.SetItemText (nRow, 3, "");
		m_GridVarInput.SetItemText (nRow, 4, "");
		m_GridVarOutput.SetItemText (nRow, 1, "");
		m_GridVarOutput.SetItemText (nRow, 2, "");
		m_GridVarOutput.SetItemText (nRow, 3, "");
		m_GridVarOutput.SetItemText (nRow, 4, "");
		m_GridVarInput.Invalidate ();
		m_GridVarOutput.Invalidate ();
	}
	return 0;
}

/*
** OnChangePsw :
*/
void CAdamoEthercatAssi::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitPsw 
*/
void CAdamoEthercatAssi::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
		m_GridAssi.EnableWindow ();
		m_GridVarInput.EnableWindow ();
		m_GridVarOutput.EnableWindow ();
    }
    else   {
		m_GridAssi.EnableWindow (false);
		m_GridVarInput.EnableWindow (false);
		m_GridVarOutput.EnableWindow (false);
    }
}
