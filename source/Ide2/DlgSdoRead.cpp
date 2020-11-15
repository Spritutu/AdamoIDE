// DlgSdoRead.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "DlgSdoRead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSdoRead dialog


CDlgSdoRead::CDlgSdoRead(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSdoRead::IDD, pParent), m_bInitialized (false), m_bSDOMonitoring (false)
{
	//{{AFX_DATA_INIT(CDlgSdoRead)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSdoRead::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSdoRead)
	DDX_Control(pDX, IDC_BTN_SDO_RESET_READ, m_btnResetReadSdo);
	DDX_Control(pDX, IDC_BTN_SDO_READ_DELETE, m_btnDeleteSdo);
	DDX_Control(pDX, IDC_BTN_SDO_READ, m_btnReadSdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSdoRead, CDialog)
	//{{AFX_MSG_MAP(CDlgSdoRead)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SDO_READ_DELETE, OnBtnSdoReadDelete)
	ON_BN_CLICKED(IDC_BTN_SDO_READ, OnBtnSdoRead)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SDO_RESET_READ, OnBtnSdoResetRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSdoRead message handlers

BOOL CDlgSdoRead::OnInitDialog() 
{
	RECT rc;

	CDialog::OnInitDialog();
    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_ctrlSdoRead.Create (rc, this, IDC_GRID_SDO_READ, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_ctrlSdoRead.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    m_ctrlSdoRead.SetEditable(true);
    m_ctrlSdoRead.SetVirtualMode(false);
    m_ctrlSdoRead.SetFixedRowCount();
    m_ctrlSdoRead.SetFixedColumnCount(0);
    m_ctrlSdoRead.SetRowCount(MAXSDOREADSROWS);
    m_ctrlSdoRead.SetColumnCount(MAXSDOREADCOLS);
    m_ctrlSdoRead.SetGridLines (GVL_NONE);
    m_ctrlSdoRead.SetColumnResize (FALSE);
    m_ctrlSdoRead.SetFixedColumnSelection (FALSE);
    m_ctrlSdoRead.EnableSelection (FALSE);
    m_ctrlSdoRead.InitControl ();
    m_nTimer=SetTimer (ID_SDOREADTIMER, 500, NULL);
	m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSdoRead::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        if (cx!=0&&cy!=0)   {
            m_ctrlSdoRead.MoveWindow (0, 0, cx-100, cy, TRUE);
            m_ctrlSdoRead.SetColumnSize ();
            m_btnReadSdo.MoveWindow (cx-100, 0, 100, 29);
            m_btnDeleteSdo.MoveWindow (cx-100, 30,  100, 29);
            m_btnResetReadSdo.MoveWindow (cx-100, 60,  100, 29);
        }
    }
}

void CDlgSdoRead::OnDestroy() 
{
	CDialog::OnDestroy();
    KillTimer (m_nTimer);
    m_ctrlSdoRead.CloseControl ();    	
}

void CDlgSdoRead::OnBtnSdoReadDelete() 
{
	// TODO: Add your control notification handler code here
    CCellID ci=m_ctrlSdoRead.GetFocusCell ();
    for (int i=0; i<MAXSDOREADCOLS; i++)
        m_ctrlSdoRead.SetItemText (ci.row, i, "");
    m_ctrlSdoRead.SetItemBkColour (ci.row, 7, RGB(0xFF, 0xFF, 0xE0));
    m_ctrlSdoRead.Invalidate ();
}

void CDlgSdoRead::OnBtnSdoRead() 
{
    if (!m_bSDOMonitoring)   {
        CAdamoMachine *pMachine=GetMachine ();
        if (pMachine)   {
            stSDORequest *pSDORequest=new stSDORequest[NMAX_READ_WRITE_SDO];
            int nRequests=GetSDORequests (pSDORequest);
            if (nRequests)   {
                pMachine->StartSDOCommand (nRequests, pSDORequest);
                StartSDOMonitoring ();
            }
            _delete (pSDORequest);
        }
    }
}

CAdamoMachine *CDlgSdoRead::GetMachine() 
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            return pActivePrj->GetMachine ();
    }
    return NULL;
}

int CDlgSdoRead::GetSDORequests (stSDORequest *pRequests)
{
    CString str;
    int n=0;

    m_nCount=0;
    while (n<MAXSDOREADSROWS)   {
        str=m_ctrlSdoRead.GetItemText (n+1, 1);
        if (str!="")
            if (GetRowData (n+1, &pRequests[m_nCount]))
                if (++m_nCount==NMAX_READ_WRITE_SDO)
                    break;
        m_ctrlSdoRead.SetItemBkColour (n, 7, RGB(0xFF, 0xFF, 0xE0));
        n++;
    }
    return m_nCount;
}

bool CDlgSdoRead::GetRowData (int nRow, stSDORequest *pRequests)
{
    CString str;
    int l, t;
    bool b=false;
    char *p;

    /* canale */
    str=m_ctrlSdoRead.GetItemText (nRow, 0);
    pRequests->m_nCanale=1;
    if (str!="")
        pRequests->m_nCanale=atoi (str);
    /* ricevitore */
    str=m_ctrlSdoRead.GetItemText (nRow, 1);
    if (str!="")   {
        pRequests->m_nNode=atoi (str);
        /* indice */
        str=m_ctrlSdoRead.GetItemText (nRow, 2);
        str.TrimLeft (); str.TrimRight ();
        if (str!="")   {
            str.TrimLeft (); str.TrimRight ();
            l=strtol (str, &p, 0);
            if (*p=='\0')   {
                pRequests->m_nIndex=l;
                /* sottoindice */
                str=m_ctrlSdoRead.GetItemText (nRow, 3);
                str.TrimLeft (); str.TrimRight ();
                if (str!="")   {
                    l=strtol (str, &p, 0);
                    if (*p=='\0')   {
                        pRequests->m_nSubIndex=l;
                        str=m_ctrlSdoRead.GetItemText (nRow, 4);
                        str.TrimLeft (); str.TrimRight ();
                        if (str!="")   {
                            /* tipo di dato da interpretare */
                            if (GetReadData (nRow, str, &l, &t))   {
                                pRequests->m_nSDOType=SDOTYPE_READ;
                                pRequests->m_nLengthData=l;
                                m_stSDOGridInfo[m_nCount].nRow=nRow;
                                m_stSDOGridInfo[m_nCount].nType=t;
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

int CDlgSdoRead::GetReadData (int nRow, CString &str, int *l, int *t)
{
    int n=0;

    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_11))   {
        str=m_ctrlSdoRead.GetItemText (nRow, 5);
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
        *l=n=sizeof (short int);
        *t=TYPE_VETTORE_SHORT;
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_18))   {
        *l=n=sizeof (sizeof (WORD));
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
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_41))   {
        *l=n=sizeof (DWORD) + sizeof (BYTE);
        *t=TYPE_VETTORE_40BIT;
    }
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_42))   {
        *l=n=sizeof (DWORD) + sizeof (WORD);
        *t=TYPE_VETTORE_48BIT;
    }
    return n;
}

void CDlgSdoRead::StartSDOMonitoring ()
{
    m_bSDOMonitoring=true;        
}

void CDlgSdoRead::OnTimer(UINT nIDEvent) 
{
    int nRequests;
	// TODO: Add your message handler code here and/or call default
    if (m_bSDOMonitoring)   {
        CAdamoMachine *pMachine=GetMachine ();
        int nStatus=pMachine->GetSDOStatus ();
        if (nStatus == 1)   {
            stSDOResults *pResults=new stSDOResults[NMAX_READ_WRITE_SDO];
            if (pMachine->GetSDOData (&nRequests, pResults) != -1)   {
                if (nRequests==m_nCount)
                    ShowResults (pResults);
            }
            _delete (pResults);
            m_bSDOMonitoring=false;
        }
        else
            if (nStatus == -1)
                OnBtnSdoResetRead ();
    }
	CDialog::OnTimer(nIDEvent);
}

void CDlgSdoRead::ShowResults(stSDOResults *pResults) 
{
    eTVSL nVisualType;
    CString str;
    char *buf;
    int n=0;

    while (n<m_nCount)   {
        if (!pResults[n].m_pe)   {
            nVisualType=GetTipoDiVisualizzazione (m_stSDOGridInfo[n].nRow);
            switch (m_stSDOGridInfo[n].nType)   {
                case TYPE_VETTORE_CARATTERI :
                    buf=new char [pResults[n].m_nLengthData*3];
                    ((CMainFrame *)AfxGetMainWnd ())->PrintHexBuffer ((BYTE *)pResults[n].m_data, pResults[n].m_nLengthData, (BYTE *)buf);
                    buf[pResults[n].m_nLengthData*3-1]='\0';
                    str=buf;
                    _delete (buf);
                    break;
                case TYPE_VETTORE_STRINGA :
                    pResults[n].m_data[NMAX_READ_WRITE_SDO_DATA-1]='\0';
                    str.Format ("%s", pResults[n].m_data);
                    break;
                case TYPE_VETTORE_FLOAT :
                    str.Format ("%8.3f", *((float *)&pResults[n].m_data));
                    break;
                case TYPE_VETTORE_DOUBLE :
                    str.Format ("%8.3f", *((double *)&pResults[n].m_data));
                    break;
                case TYPE_VETTORE_CHAR :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%1X", *((char *)&pResults[n].m_data));
                            break;
                        case evtDec :
                            str.Format ("%d", *((char *)&pResults[n].m_data));
                            break;
                        case evtOct :
                            str.Format ("%o", *((char *)&pResults[n].m_data));
                            break;
                    }    
                    break;
                case TYPE_VETTORE_BYTE :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%1X", *((BYTE *)&pResults[n].m_data));
                            break;
                        case evtDec :
                            str.Format ("%u", *((BYTE *)&pResults[n].m_data));
                            break;
                        case evtOct :
                            str.Format ("%o", *((BYTE *)&pResults[n].m_data));
                            break;
                    }
                    break;
                case TYPE_VETTORE_SHORT :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%2X", *((short int *)&pResults[n].m_data));
                            break;
                        case evtDec :
                            str.Format ("%d", *((short int *)&pResults[n].m_data));
                            break;
                        case evtOct :
                            str.Format ("%o", *((short int *)&pResults[n].m_data));
                            break;
                    }
                    break;
                case TYPE_VETTORE_WORD :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%2X", *((WORD *)&pResults[n].m_data));
                            break;
                        case evtDec :
                            str.Format ("%u", *((WORD *)&pResults[n].m_data));
                            break;
                        case evtOct :
                            str.Format ("%o", *((WORD *)&pResults[n].m_data));
                            break;
                    }
                    break;
                case TYPE_VETTORE_INT :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%4X", *((int *)&pResults[n].m_data));
                            break;
                        case evtDec :
                            str.Format ("%d", *((int *)&pResults[n].m_data));
                            break;
                        case evtOct :
                            str.Format ("%o", *((int *)&pResults[n].m_data));
                            break;
                    }
                    break;
                case TYPE_VETTORE_DWORD :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%4X", *((DWORD *)&pResults[n].m_data));
                            break;
                        case evtDec :
                            str.Format ("%u", *((DWORD *)&pResults[n].m_data));
                            break;
                        case evtOct :
                            str.Format ("%o", *((DWORD *)&pResults[n].m_data));
                            break;
                    }
                    break;
                case TYPE_VETTORE_40BIT :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%02X%02X%02X%02X%02X", pResults[n].m_data[0], pResults[n].m_data[1], pResults[n].m_data[2],
                                pResults[n].m_data[3], pResults[n].m_data[4]);
                            break;
                        case evtDec :
                            str.Format ("0x%02X%02X%02X%02X%02X", pResults[n].m_data[0], pResults[n].m_data[1], pResults[n].m_data[2],
                                pResults[n].m_data[3], pResults[n].m_data[4]);
                            break;
                        case evtOct :
                            str.Format ("0x%02X%02X%02X%02X%02X", pResults[n].m_data[0], pResults[n].m_data[1], pResults[n].m_data[2],
                                pResults[n].m_data[3], pResults[n].m_data[4]);
                            break;
                    }
                    break;
                case TYPE_VETTORE_48BIT :
                    switch (nVisualType)   {
                        case evtHex :
                            str.Format ("0x%02X%02X%02X%02X%02X%02X", pResults[n].m_data[0], pResults[n].m_data[1], pResults[n].m_data[2],
                                pResults[n].m_data[3], pResults[n].m_data[4], pResults[n].m_data[5]);
                            break;
                        case evtDec :
                            str.Format ("0x%02X%02X%02X%02X%02X%02X", pResults[n].m_data[0], pResults[n].m_data[1], pResults[n].m_data[2],
                                pResults[n].m_data[3], pResults[n].m_data[4], pResults[n].m_data[5]);
                            break;
                        case evtOct :
                            str.Format ("0x%02X%02X%02X%02X%02X%02X", pResults[n].m_data[0], pResults[n].m_data[1], pResults[n].m_data[2],
                                pResults[n].m_data[3], pResults[n].m_data[4], pResults[n].m_data[5]);
                            break;
                    }
                    break;
            }
            m_ctrlSdoRead.SetItemText (m_stSDOGridInfo[n].nRow, 6, str);
            m_ctrlSdoRead.SetItemBkColour (m_stSDOGridInfo[n].nRow, 7, RGB (0, 255, 0));
        }
        else   {
            m_ctrlSdoRead.SetItemText (m_stSDOGridInfo[n].nRow, 6, "");
            m_ctrlSdoRead.SetItemBkColour (m_stSDOGridInfo[n].nRow, 7, RGB (255, 0, 0));
        }
        n++;
    }
    m_ctrlSdoRead.Invalidate ();
}

void CDlgSdoRead::OnBtnSdoResetRead() 
{
    int n=0;
	// TODO: Add your control notification handler code here
    m_bSDOMonitoring=false;
    while (n<MAXSDOREADSROWS)   {
        m_ctrlSdoRead.SetItemText (n, 6, "");
        m_ctrlSdoRead.SetItemBkColour (n, 7, RGB(0xFF, 0xFF, 0xE0));
        n++;
    }
    Invalidate ();
}

CDlgSdoRead::eTVSL CDlgSdoRead::GetTipoDiVisualizzazione (int nRow)
{
    eTVSL e;

    CString str=m_ctrlSdoRead.GetItemText (nRow, 5);
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
void CDlgSdoRead::OnOK ()
{
}

void CDlgSdoRead::OnCancel ()
{
}