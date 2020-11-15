// DlgEcMailBoxRead.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgEcMailBoxRead.h"
#include "gridcellcombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEcMailBoxRead dialog

static CString* aszRicevitore [NMAX_CANNODES];
static CString* aszTipiDato [9];
static CString* aszTipiDiVisualizzazione [3];

CDlgEcMailBoxRead::CDlgEcMailBoxRead(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEcMailBoxRead::IDD, pParent), m_bInitialized (false), m_bEcMonitoring (false), m_slaveArray (NULL)
{
	//{{AFX_DATA_INIT(CDlgEcMailBoxRead)
	//}}AFX_DATA_INIT
}


void CDlgEcMailBoxRead::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEcMailBoxRead)
	DDX_Control(pDX, IDC_BTN_EC_RESET_READ, m_btnEcMailBoxReset);
	DDX_Control(pDX, IDC_BTN_EC_READ_READ, m_btnEcMailBoxRead);
	DDX_Control(pDX, IDC_BTN_EC_DELETE_READ, m_btnEcMailBoxDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEcMailBoxRead, CDialog)
	//{{AFX_MSG_MAP(CDlgEcMailBoxRead)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EC_DELETE_READ, OnBtnEcDeleteRead)
	ON_BN_CLICKED(IDC_BTN_EC_RESET_READ, OnBtnEcResetRead)
	ON_BN_CLICKED(IDC_BTN_EC_READ_READ, OnBtnEcReadRead)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEcMailBoxRead message handlers

void CDlgEcMailBoxRead::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        if (cx!=0&&cy!=0)   {
            m_ctrlEcMailBoxRead.MoveWindow (0, 0, cx-100, cy, TRUE);
            m_ctrlEcMailBoxRead.SetColumnSize ();
			m_ctrlEcMailBoxRead.ExpandLastColumn ();
            m_btnEcMailBoxRead.MoveWindow (cx-100, 0, 100, 29);
            m_btnEcMailBoxDelete.MoveWindow (cx-100, 30,  100, 29);
            m_btnEcMailBoxReset.MoveWindow (cx-100, 60,  100, 29);
        }
    }
}

BOOL CDlgEcMailBoxRead::OnInitDialog() 
{
	RECT rc;

	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	InitButtons ();
    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_ctrlEcMailBoxRead.Create (rc, this, IDC_GRID_EC_MAILBOX_READ, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_ctrlEcMailBoxRead.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    m_ctrlEcMailBoxRead.SetEditable(true);
    m_ctrlEcMailBoxRead.SetVirtualMode(false);
    m_ctrlEcMailBoxRead.SetFixedRowCount();
    m_ctrlEcMailBoxRead.SetFixedColumnCount(0);
    m_ctrlEcMailBoxRead.SetRowCount(MAXECMAILBOXREADROWS);
    m_ctrlEcMailBoxRead.SetColumnCount(MAXECMAILBOXREADCOLS);
    m_ctrlEcMailBoxRead.SetGridLines (GVL_NONE);
    m_ctrlEcMailBoxRead.SetColumnResize (FALSE);
    m_ctrlEcMailBoxRead.SetFixedColumnSelection (FALSE);
    m_ctrlEcMailBoxRead.EnableSelection (FALSE);
    m_nTimer=SetTimer (ID_ECMAILBOXREADTIMER, 500, NULL);
	m_bInitialized=true;	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitButtons :
*/
void CDlgEcMailBoxRead::InitButtons ()
{
	m_btnEcMailBoxReset.SetWindowText (LOADSTRING (IDS_EC_MAILBOX_BUT_RESET));
	m_btnEcMailBoxRead.SetWindowText (LOADSTRING (IDS_EC_MAILBOX_BUT_READ));
	m_btnEcMailBoxDelete.SetWindowText (LOADSTRING (IDS_EC_MAILBOX_BUT_DELETE));
}

/* grid construction / destruction */
CEcMailBoxReadGrid::CEcMailBoxReadGrid ()
{
}

/*
** InitControl :
*/
void CEcMailBoxReadGrid::InitControl (CPtrArray &slaveArray)
{
	InitStrings (slaveArray);
    InitColumnCaptions ();
    InitGrid ();
}

/*
** CloseControl :
*/
void CEcMailBoxReadGrid::CloseControl ()
{
    CloseStrings ();
	m_ImageList.DeleteImageList ();
}

/*
** SetColumnSize  :
*/
void CEcMailBoxReadGrid::SetColumnSize ()
{
    RECT rc;
    int n;

    GetClientRect (&rc);
    SetColumnWidth (0, 90);
    SetColumnWidth (1, 90);
    SetColumnWidth (2, 90);
    SetColumnWidth (3, 100);
    SetColumnWidth (4, 90);
    SetColumnWidth (6, 30);
    n = rc.right>490 ? rc.right-490 : 90;
    SetColumnWidth (5, n);
}

/*
** InitColumnCaptions :
*/
void CEcMailBoxReadGrid::InitColumnCaptions ()
{
    GV_ITEM Item;
    int i;
    CString str;
    CBitmap bm;

    m_ImageList.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_SDOREADSYMBOLS);
    m_ImageList.Add(&bm, RGB(0, 128, 128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<MAXECMAILBOXREADCOLS; i++)   {
		Item.col=i;
        Item.iImage=i+1;
        str=LOADSTRING (IDS_ADAMOECMAILBOXVIEW_1+i);
        Item.strText=str.GetBuffer (0);
        str.ReleaseBuffer ();
        SetItem(&Item);
    }
}

/*
** InitGrid :
*/
void CEcMailBoxReadGrid::InitGrid ()
{
    for (int n=0; n<MAXECMAILBOXREADROWS-1; n++)
        InitGridLine (n+1);
}

/*
** InitGridLine :
*/
void CEcMailBoxReadGrid::InitGridLine (int n)
{
    SetCellType(n, 0, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 1, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 2, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 3, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 4, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 5, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 6, RUNTIME_CLASS(CGridCell));
    ((CGridCellCombo*)GetCell(n, 0))->SetOptions(m_slRicevitore);
    ((CGridCellCombo*)GetCell(n, 0))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 3))->SetOptions(m_slTipoDato);
    ((CGridCellCombo*)GetCell(n, 3))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 4))->SetOptions(m_slTipoDiVisualizzazione);
    ((CGridCellCombo*)GetCell(n, 4))->SetStyle (CBS_DROPDOWNLIST);
}

/*
** InitStrings :
*/
void CEcMailBoxReadGrid::InitStrings (CPtrArray &slaveArray)
{
    int n;
	CPtrArray &p=slaveArray;
    m_slRicevitore.SetSize(slaveArray.GetSize ());
    for (n=0; n<slaveArray.GetSize (); n++)   {
        aszRicevitore[n]=new CString;
        *aszRicevitore[n]=((stAdamoEthSlave *)slaveArray[n])->szName;
        m_slRicevitore[n]=*aszRicevitore[n];
    }
    m_slTipoDato.SetSize (10);
    for (n=0; n<9; n++)   {
        aszTipiDato[n]=new CString;
        *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_12+n);
        m_slTipoDato[n]=*aszTipiDato[n];
    }
    m_slTipoDiVisualizzazione.SetSize (3);
    for (n=0; n<3; n++)   {
        aszTipiDiVisualizzazione[n]=new CString;
        *aszTipiDiVisualizzazione[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_38+n);
        m_slTipoDiVisualizzazione[n]=*aszTipiDiVisualizzazione[n];
    }
}

/*
** CloseStrings :
*/
void CEcMailBoxReadGrid::CloseStrings ()
{
    int n;

    for (n=0; n<m_slRicevitore.GetSize (); n++)
        _delete (aszRicevitore[n]);
    for (n=0; n<9; n++)
        _delete (aszTipiDato[n]);
    for (n=0; n<3; n++)
        _delete (aszTipiDiVisualizzazione [n]);
}       

/*
** OnEditCell :
*/
void CEcMailBoxReadGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nCol!=MAXECMAILBOXREADCOLS-1)
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

/*
** OnDestroy :
*/
void CDlgEcMailBoxRead::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    KillTimer (m_nTimer);
    m_ctrlEcMailBoxRead.CloseControl ();    	
}

/*
** LoadEthercatXMLFile :
*/
void CDlgEcMailBoxRead::LoadEthercatXMLFile (CPtrArray &slaveArray)
{
	m_ctrlEcMailBoxRead.CloseControl ();
	m_ctrlEcMailBoxRead.InitControl (slaveArray);
	m_slaveArray=&slaveArray;
}

/*
** OnBtnEcDeleteRead :
*/
void CDlgEcMailBoxRead::OnBtnEcDeleteRead() 
{
	// TODO: Add your control notification handler code here
    CCellID ci=m_ctrlEcMailBoxRead.GetFocusCell ();
    for (int i=0; i<MAXSDOREADCOLS; i++)
        m_ctrlEcMailBoxRead.SetItemText (ci.row, i, "");
    m_ctrlEcMailBoxRead.SetItemBkColour (ci.row, 7, RGB(0xFF, 0xFF, 0xE0));
    m_ctrlEcMailBoxRead.Invalidate ();
}

/*
** OnBtnEcResetRead :
*/
void CDlgEcMailBoxRead::OnBtnEcResetRead() 
{
	// TODO: Add your control notification handler code here
	int n=0;

    m_bEcMonitoring=false;
    while (n<MAXECMAILBOXWRITEROWS)   {
        m_ctrlEcMailBoxRead.SetItemText (n, 5, "");
        m_ctrlEcMailBoxRead.SetItemBkColour (n, 6, RGB(0xFF, 0xFF, 0xE0));
        n++;
    }
    Invalidate ();
}

/*
** GetMachine :
*/
CAdamoMachine *CDlgEcMailBoxRead::GetMachine() 
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            return pActivePrj->GetMachine ();
    }
    return NULL;
}

/*
** OnBtnEcReadRead :
*/
void CDlgEcMailBoxRead::OnBtnEcReadRead() 
{
	// TODO: Add your control notification handler code here
	int nSlave, nIndex, nSubIndex, nLen;
    if (!m_bEcMonitoring)   {
        CAdamoMachine *pMachine=GetMachine ();
        if (pMachine)   {
			for (int i=0; i<MAXECMAILBOXREADROWS; i++)   {
				for (int n=0; n<MAXECMAILBOXREADCOLS; n++)   {
					int nState=m_ctrlEcMailBoxRead.GetItemState (i, n);
					if (nState & GVNI_FOCUSED)   {
						m_nMonitoredRow=i;
						if (GetEcMalBoxData (i, &nSlave, &nIndex, &nSubIndex, &nLen))   {
							pMachine->StartEcMailBoxCommand (EC_MAILBOX_TYPE_READ, nSlave, nIndex, nSubIndex, nLen, NULL);
							m_bEcMonitoring=true;
						}
						break;
					}
				}
			}
        }
    }
}

/*
** GetEcMalBoxData :
*/
bool CDlgEcMailBoxRead::GetEcMalBoxData (int nRow, int *nSlave, int *nIndex, int *nSubIndex, int *nLen)
{
    CString str;
    int l, t;
    bool b=false;
    char *p;

    /* slave */
    str=m_ctrlEcMailBoxRead.GetItemText (nRow, 0);
    if (str!="")   {
        *nSlave=GetSlaveIndex (str);
        /* indice */
        str=m_ctrlEcMailBoxRead.GetItemText (nRow, 1);
        str.TrimLeft (); str.TrimRight ();
        if (str!="")   {
            l=strtol (str, &p, 0);
            if (*p=='\0')   {
                *nIndex=l;
                /* sottoindice */
                str=m_ctrlEcMailBoxRead.GetItemText (nRow, 2);
                str.TrimLeft (); str.TrimRight ();
                if (str!="")   {
                    l=strtol (str, &p, 0);
                    if (*p=='\0')   {
                        *nSubIndex=l;
                        str=m_ctrlEcMailBoxRead.GetItemText (nRow, 3);
                        str.TrimLeft (); str.TrimRight ();
                        if (str!="")   {
                            /* tipo di dato da interpretare */
                            if (GetReadData (nRow, str, &l, &t))   {
                                *nLen=l;
                                b=true;
                            }
                        }
                    }
                }
            }
        }
    }
    return b;
}

int CDlgEcMailBoxRead::GetReadData (int nRow, CString &str, int *l, int *t)
{
    int n=0;

    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_11))   {
        str=m_ctrlEcMailBoxRead.GetItemText (nRow, 4);
        str.TrimLeft (); str.TrimRight ();
        if (str!="")
            *l=n=atoi (str);
        *t=TYPE_VETTORE_CARATTERI;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_12))   {
        *l=n=NMAX_READ_WRITE_SDO_DATA;
        *t=TYPE_VETTORE_STRINGA;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_13))   {
        *l=n=sizeof (float);
        *t=TYPE_VETTORE_FLOAT;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_14))   {
        *l=n=sizeof (double);
        *t=TYPE_VETTORE_DOUBLE;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_15))   {
        *l=n=sizeof (char);
        *t=TYPE_VETTORE_CHAR;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_16))   {
        *l=n=sizeof (BYTE);
        *t=TYPE_VETTORE_BYTE;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_17))   {
        *l=n=2; //sizeof (short int);
        *t=TYPE_VETTORE_SHORT;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_18))   {
        *l=n=2; //sizeof (sizeof (WORD));
        *t=TYPE_VETTORE_WORD;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_19))   {
        *l=n=sizeof (int);
        *t=TYPE_VETTORE_INT;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_20))   {
        *l=n=sizeof (DWORD);
        *t=TYPE_VETTORE_DWORD;
    }
    return n;
}

/*
** GetSlaveIndex :
*/
int CDlgEcMailBoxRead::GetSlaveIndex (CString str)
{
	int i, nPhysAddress=0;
	if (m_slaveArray)   {
		for (i=0; i<m_slaveArray->GetSize (); i++)   {
			if (!strcmp (((stAdamoEthSlave *)(*m_slaveArray)[i])->szName, str))   {
				nPhysAddress=((stAdamoEthSlave *)(*m_slaveArray)[i])->nPhysAddress;
				break;
			}
		}
	}
	return nPhysAddress;
}

/*
** OnTimer :
*/
void CDlgEcMailBoxRead::OnTimer(UINT nIDEvent) 
{
	LONG lResult;
    int nStatus, nLen;
	BYTE pData[NMAX_EC_RW_MAILBOX_DATA];
	// TODO: Add your message handler code here and/or call default

    if (m_bEcMonitoring)   {
        CAdamoMachine *pMachine=GetMachine ();
        nStatus=pMachine->GetEcMailBoxStatus ();
        if (nStatus == 1)   {
            if (!pMachine->GetEcMailBoxData (&lResult, &nLen, pData))
				ShowResults (lResult, nLen, pData);
            m_bEcMonitoring=false;
        }
    }
	CDialog::OnTimer(nIDEvent);
}

/*
** ShowResults :
*/
void CDlgEcMailBoxRead::ShowResults(LONG lResult, int nLen, BYTE *pData)
{
    eTVSL nVisualType;
    CString str;
    char *buf;
    int l, t, n=0;

	if (lResult==0)   {
        nVisualType=GetTipoDiVisualizzazione (m_nMonitoredRow);
        str=m_ctrlEcMailBoxRead.GetItemText (m_nMonitoredRow, 3);
        str.TrimLeft (); str.TrimRight ();
		GetReadData (m_nMonitoredRow, str, &l, &t);
        switch (t)   {
            case TYPE_VETTORE_CARATTERI :
                buf=new char [l*3];
                ((CMainFrame *)AfxGetMainWnd ())->PrintHexBuffer (pData, nLen, (BYTE *)buf);
                buf[nLen*3-1]='\0';
                str=buf;
                _delete (buf);
                break;
            case TYPE_VETTORE_STRINGA :
                pData[NMAX_EC_RW_MAILBOX_DATA-1]='\0';
                str.Format ("%s", pData);
                break;
            case TYPE_VETTORE_FLOAT :
                str.Format ("%8.3f", *((float *)pData));
                break;
            case TYPE_VETTORE_DOUBLE :
                str.Format ("%8.3f", *((double *)pData));
                break;
            case TYPE_VETTORE_CHAR :
                switch (nVisualType)   {
                    case evtHex :
                        str.Format ("0x%1X", *((char *)pData));
                        break;
                    case evtDec :
                        str.Format ("%d", *((char *)pData));
                        break;
                    case evtOct :
                        str.Format ("%o", *((char *)pData));
                        break;
                }    
                break;
            case TYPE_VETTORE_BYTE :
                switch (nVisualType)   {
                    case evtHex :
                        str.Format ("0x%1X", *((BYTE *)pData));
                        break;
                    case evtDec :
                        str.Format ("%u", *((BYTE *)pData));
                        break;
                    case evtOct :
                        str.Format ("%o", *((BYTE *)pData));
                        break;
                }
                break;
            case TYPE_VETTORE_SHORT :
                switch (nVisualType)   {
                    case evtHex :
                        str.Format ("0x%2X", *((short int *)pData));
                        break;
                    case evtDec :
                        str.Format ("%d", *((short int *)pData));
                        break;
                    case evtOct :
                        str.Format ("%o", *((short int *)pData));
                        break;
                }
                break;
            case TYPE_VETTORE_WORD :
                switch (nVisualType)   {
                    case evtHex :
                        str.Format ("0x%2X", *((WORD *)pData));
                        break;
                    case evtDec :
                        str.Format ("%u", *((WORD *)pData));
                        break;
                    case evtOct :
                        str.Format ("%o", *((WORD *)pData));
                        break;
                }
                break;
            case TYPE_VETTORE_INT :
                switch (nVisualType)   {
                    case evtHex :
                        str.Format ("0x%4X", *((int *)pData));
                        break;
                    case evtDec :
                        str.Format ("%d", *((int *)pData));
                        break;
                    case evtOct :
                        str.Format ("%o", *((int *)pData));
                        break;
                }
                break;
            case TYPE_VETTORE_DWORD :
                switch (nVisualType)   {
                    case evtHex :
                        str.Format ("0x%4X", *((DWORD *)pData));
                        break;
                    case evtDec :
                        str.Format ("%u", *((DWORD *)pData));
                        break;
                    case evtOct :
                        str.Format ("%o", *((DWORD *)pData));
                        break;
                }
                break;
        }
        m_ctrlEcMailBoxRead.SetItemText (m_nMonitoredRow, 5, str);
        m_ctrlEcMailBoxRead.SetItemBkColour (m_nMonitoredRow, 6, RGB (0, 255, 0));
    }
    else   {
        m_ctrlEcMailBoxRead.SetItemText (m_nMonitoredRow, 5, "");
        m_ctrlEcMailBoxRead.SetItemBkColour (m_nMonitoredRow, 6, RGB (255, 0, 0));
    }
    m_ctrlEcMailBoxRead.Invalidate ();
}

/*
** GetTipoDiVisualizzazione :
*/
CDlgEcMailBoxRead::eTVSL CDlgEcMailBoxRead::GetTipoDiVisualizzazione (int nRow)
{
    eTVSL e;

    CString str=m_ctrlEcMailBoxRead.GetItemText (nRow, 4);
    if (!strcmp (str, LOADSTRING (IDS_ADAMODGSDOVIEW_38)))   {
        e=evtHex;
    }
    else
    if (!strcmp (str, LOADSTRING (IDS_ADAMODGSDOVIEW_39)))   {
        e=evtDec;
    }
    else
    if (!strcmp (str, LOADSTRING (IDS_ADAMODGSDOVIEW_40)))   {
        e=evtOct;
    }
    return e;
}

/*
** Evitiamo di uscire come degli stolti
*/
void CDlgEcMailBoxRead::OnOK ()
{
}

void CDlgEcMailBoxRead::OnCancel ()
{
}
