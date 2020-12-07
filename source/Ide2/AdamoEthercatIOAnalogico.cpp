// AdamoEthercatIOAnalogico.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoEthercatIOAnalogico.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIOAnalogico dialog

IMPLEMENT_DYNAMIC(CAdamoEthercatIOAnalogico, CDialog)

CAdamoEthercatIOAnalogico::CAdamoEthercatIOAnalogico(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoEthercatIOAnalogico::IDD, pParent), m_pDoc (NULL), m_nNode (-1), m_bInitialized (false), m_nTab (-1), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CAdamoEthercatIOAnalogico)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoEthercatIOAnalogico::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoEthercatIOAnalogico)
	DDX_Control(pDX, IDC_ST_OUTANAG, m_stOutAnag);
	DDX_Control(pDX, IDC_ST_INPANAG, m_stInpAnag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoEthercatIOAnalogico, CDialog)
	//{{AFX_MSG_MAP(CAdamoEthercatIOAnalogico)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIOAnalogicoGrid message handlers

/* construction / destruction */
CAdamoEthercatIOAnalogicoGrid::CAdamoEthercatIOAnalogicoGrid () : m_pEth (NULL)
{
}

/*
** FillGridCaption :
*/
void CAdamoEthercatIOAnalogicoGrid::FillGridCaption ()
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
	Item.row = 0;
	Item.col = 3;
    Item.iImage = 2;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_VARIABILE);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 4;
    Item.iImage = 3;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_POSIZIONE);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 5;
    Item.iImage = 4;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_LLINF);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 6;
    Item.iImage = 5;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_LLSUP);
    SetItem(&Item);
}

/*
** OnConnected :
*/
int CAdamoEthercatIOAnalogicoGrid::OnConnected (int nRow, int nCol, const char *szHWAddress)
{
	CAdamoHWConfigFile *pHWFile;
	double fpLimInf, fpLimPos;
	int nHWAddress;
	char szBuffer[256];

    pHWFile=m_pPrj->GetHWConfigFile ();
    if (pHWFile)   {
		nHWAddress=((CMainFrame *) AfxGetMainWnd ())->GetPinFromHWAddress (szHWAddress)+1;
		if (m_eType==enInputAnalogico)   {
			if (nHWAddress!=-1)
				if (pHWFile->GetEthercatInpAnalogHW (m_pEth->m_nNode-1, nHWAddress, &fpLimInf, &fpLimPos)==-1)   {
					pHWFile->SetEthercatInpAnalogHW (m_pEth->m_nNode-1, nHWAddress, 2, 0.0, 0.0);
					pHWFile->SetEthercatInpAnalogType (m_pEth->m_nNode-1, nHWAddress, m_pEth->GetInpAnalogType (nHWAddress));
					fpLimInf=fpLimPos=0.0;
		}
		}
		else   {
			if (nHWAddress!=-1)
				if (pHWFile->GetEthercatOutAnalogHW (m_pEth->m_nNode-1, nHWAddress, &fpLimInf, &fpLimPos)==-1)   {
					pHWFile->SetEthercatOutAnalogHW (m_pEth->m_nNode-1, nHWAddress, 2, 0.0, 0.0);
					pHWFile->SetEthercatOutAnalogType (m_pEth->m_nNode-1, nHWAddress, m_pEth->GetOutAnalogType (nHWAddress));
					fpLimInf=fpLimPos=0.0;
		}
		}
		sprintf (szBuffer, "%8.3f", fpLimInf);
		this->SetItemText (nRow, 5, szBuffer);
		sprintf (szBuffer, "%8.3f", fpLimPos);
		this->SetItemText (nRow, 6, szBuffer);
	}
	return 0;
}

/*
** OnRemoveConnection :
*/
int CAdamoEthercatIOAnalogicoGrid::OnRemoveConnection (int nRow, int nCol, const char *szHWAddress)
{
	CAdamoHWConfigFile *pHWFile;
	int nHWAddress;

    pHWFile=m_pPrj->GetHWConfigFile ();
    if (pHWFile)   {
		this->SetItemText (nRow, 5, "");
		this->SetItemText (nRow, 6, "");
		nHWAddress=((CMainFrame *) AfxGetMainWnd ())->GetPinFromHWAddress (m_stConfigHW[nRow-1].m_strHWAddr)+1;
		if (m_eType==enInputAnalogico)   {
			pHWFile->RemoveEthercatInpAnalogHW (m_pEth->m_nNode-1, nHWAddress);
		}
		else
		if (m_eType==enOutputAnalogico)   {
			pHWFile->RemoveEthercatOutAnalogHW (m_pEth->m_nNode-1, nHWAddress);
		}
	}
	return 0;
}

/*
** OnEditCell :
*/
void CAdamoEthercatIOAnalogicoGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (GetItemText (nRow, 1)!="" && (nCol==5 || nCol==6))
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

/*
** OnEndEditCell :
*/
void CAdamoEthercatIOAnalogicoGrid::OnEndEditCell (int nRow, int nCol, CString str)
{
	CAdamoHWConfigFile *pHWFile;
	double fp;
	int nHWAddress;
	char *p;

    pHWFile=m_pPrj->GetHWConfigFile ();
    if (pHWFile && str!="" && (nCol==5 || nCol==6))   {
		nHWAddress=((CMainFrame *) AfxGetMainWnd ())->GetPinFromHWAddress (m_stConfigHW[nRow-1].m_strHWAddr)+1;
		str.TrimLeft (); str.TrimRight ();
		fp=strtod (str, &p);
		if (*p=='\0')   {
			if (nCol==5)   {
				if (m_eType==enInputAnalogico)   {
					pHWFile->SetEthercatInpAnalogHW (m_pEth->m_nNode-1, nHWAddress, 0, fp, 0.0);
				}
				else
					pHWFile->SetEthercatOutAnalogHW (m_pEth->m_nNode-1, nHWAddress, 0, fp, 0.0);
				m_pPrj->SetHWConfigModified ();
			}
			else
			if (nCol==6)   {
				if (m_eType==enInputAnalogico)   {
					pHWFile->SetEthercatInpAnalogHW (m_pEth->m_nNode-1, nHWAddress, 1, 0.0, fp);
				}
				else
					pHWFile->SetEthercatOutAnalogHW (m_pEth->m_nNode-1, nHWAddress, 1, 0.0, fp);
				m_pPrj->SetHWConfigModified ();
			}
		}
	}
	CGridCtrl::OnEndEditCell (nRow, nCol, str);
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIOAnalogico message handlers

/*
** Find :
*/
void CAdamoEthercatIOAnalogico::Find (const char *szBuffer)
{
	m_GridInpAna.Find (szBuffer);
	m_GridOutAna.Find (szBuffer);
}

/*
** RemoveCurrentConnection :
*/
void CAdamoEthercatIOAnalogico::RemoveCurrentConnection ()
{
	m_GridInpAna.RemoveCurrentConnection ();
	m_GridOutAna.RemoveCurrentConnection ();
}

/*
** OnInitDialog :
*/
BOOL CAdamoEthercatIOAnalogico::OnInitDialog () 
{
	int nRow;

	CDialog::OnInitDialog();
	m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    m_GridInpAna.SetConfigDoc   (m_pDoc);
    m_GridOutAna.SetConfigDoc   (m_pDoc);
    m_GridInpAna.SetProject   (m_pPrj);
    m_GridOutAna.SetProject   (m_pPrj);
	LoadSlaveInfo ();
	nRow=m_pSlave->GetInputVariables ();
	if (nRow<12)
		nRow=12;
	InitializeGrid (m_GridInpAna, IDC_GRID_HW_VAR_ANAG_INPUT, nRow, 6, 4);
	nRow=m_pSlave->GetOutputVariables ();
	if (nRow<12)
		nRow=12;
	InitializeGrid (m_GridOutAna, IDC_GRID_HW_VAR_ANAG_OUTPUT, nRow, 6, 4);
	InitializeGridData ();
    InitializeTitles ();
	InitPsw ();
	m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnSize :
*/
void CAdamoEthercatIOAnalogico::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

    if (m_bInitialized)   {
        m_stInpAnag.MoveWindow (10, 2, 100, 16);
	    m_GridInpAna.MoveWindow (10, 18, cx-20, 13*22+4);
        m_stOutAnag.MoveWindow (10, 334, 100, 16);
	    m_GridOutAna.MoveWindow (10, 350, cx-20, 13*22+4);
        m_GridInpAna.SetColumnWidth (0, 50);
        m_GridOutAna.SetColumnWidth (0, 50);
		m_GridInpAna.SetColumnWidth (1, 100);
        m_GridOutAna.SetColumnWidth (1, 100);
		m_GridInpAna.SetColumnWidth (2, 224);
        m_GridOutAna.SetColumnWidth (2, 224);
		m_GridInpAna.SetColumnWidth (3, 300);
        m_GridOutAna.SetColumnWidth (3, 300);
		m_GridInpAna.SetColumnWidth (4, 80);
        m_GridOutAna.SetColumnWidth (4, 80);
        m_GridInpAna.ExpandLastColumn ();
        m_GridOutAna.ExpandLastColumn ();
    }
}

/*
** InitializeTitles :
*/
void CAdamoEthercatIOAnalogico::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_TITLE_INPANA);
    m_stInpAnag.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_OUTANA);
    m_stOutAnag.SetWindowText (str);
}

/*
** InitializeGrid
*/
void CAdamoEthercatIOAnalogico::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode)
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
	((CAdamoEthercatIOAnalogicoGrid &)m_Grid).SetEthIOAnalogico (this);
}

/*
** InitializaGridData :
*/
void CAdamoEthercatIOAnalogico::InitializeGridData ()
{
    CString str;
	int i, n;

    i = 0; n = 0;
	for (POSITION pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))   {
			if (pVar->nInputOutput==0)   {
				str.Format ("E1R%dIAPin%d", m_nNode, pVar->nBitOffs);
				m_GridInpAna.SetGridData (i+1, 1, i+1, 2, str);
				str.Format ("IA%d", i+1);
				m_GridInpAna.SetItemText (i+1, 0, str);
				m_GridInpAna.SetItemText (i+1, 3, pVar->szName);
				str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
				m_GridInpAna.SetItemText (i+1, 4, str);
			    i++;
			}
		}
	}
    i = 0; n = 0;
	for (POSITION pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))   {
			if (pVar->nInputOutput==1)   {
				str.Format ("E1R%dOAPin%d", m_nNode, pVar->nBitOffs);
				m_GridOutAna.SetGridData (i+1, 1, i+1, 2, str);
				str.Format ("OA%d", i+1);
				m_GridOutAna.SetItemText (i+1, 0, str);
				m_GridOutAna.SetItemText (i+1, 3, pVar->szName);
				str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
				m_GridOutAna.SetItemText (i+1, 4, str);
			i++;
		}
	}
	}
    m_GridInpAna.SetType (enInputAnalogico);
    m_GridInpAna.UpdateData ();
    m_GridOutAna.SetType (enOutputAnalogico);
    m_GridOutAna.UpdateData ();
}

/*
** LoadSlaveInfo :
*/
void CAdamoEthercatIOAnalogico::LoadSlaveInfo ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
	if (pPrj)   {
		CPtrArray &rSlaveArray=pPrj->GetSlaveArray ();
		m_pSlave=(stAdamoEthSlave *)rSlaveArray[m_nNode-1];
	}
}

/*
** OnChangePsw :
*/
void CAdamoEthercatIOAnalogico::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitPsw 
*/
void CAdamoEthercatIOAnalogico::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
		m_GridInpAna.EnableWindow ();
		m_GridOutAna.EnableWindow ();
    }
    else   {
		m_GridInpAna.EnableWindow (false);
		m_GridOutAna.EnableWindow (false);
    }
}

/*
** GetInpAnalogType : 
*/
int CAdamoEthercatIOAnalogico::GetInpAnalogType (int nHWAddress)
{
	int nType = -1, n = 0;
	for (POSITION pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))   {
			if (pVar->nInputOutput==0)   {
				if (nHWAddress == pVar->nBitOffs)   {
					nType = pVar->nDataType;
					break;
				}
			}
		}
	}
	return nType;
}

/*
** GetOutAnalogType : 
*/
int CAdamoEthercatIOAnalogico::GetOutAnalogType (int nHWAddress)
{
	int nType = -1, n = 0;
	for (POSITION pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))   {
			if (pVar->nInputOutput==1)   {
				if (nHWAddress == pVar->nBitOffs)   {
					nType = pVar->nDataType;
					break;
				}
			}
		}
	}
	return nType;
}
