// DlgSdoWrite.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "DlgSdoWrite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSdoWrite dialog


CDlgSdoWrite::CDlgSdoWrite(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSdoWrite::IDD, pParent), m_bInitialized (false), m_bSDOMonitoring (false)
{
	//{{AFX_DATA_INIT(CDlgSdoWrite)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSdoWrite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSdoWrite)
	DDX_Control(pDX, IDC_BTN_SDO_RESET_WRITE, m_btnResetWriteSdo);
	DDX_Control(pDX, IDC_BTN_SDO_WRITE_DELETE, m_btnDeleteSdo);
	DDX_Control(pDX, IDC_BTN_SDO_WRITE, m_btnWriteSdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSdoWrite, CDialog)
	//{{AFX_MSG_MAP(CDlgSdoWrite)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SDO_WRITE_DELETE, OnBtnSdoWriteDelete)
	ON_BN_CLICKED(IDC_BTN_SDO_WRITE, OnBtnSdoWrite)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SDO_RESET_WRITE, OnBtnSdoResetWrite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSdoWrite message handlers

BOOL CDlgSdoWrite::OnInitDialog() 
{
	RECT rc;

	CDialog::OnInitDialog();
    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_ctrlSdoWrite.Create (rc, this, IDC_GRID_SDO_WRITE, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_ctrlSdoWrite.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    m_ctrlSdoWrite.SetEditable(true);
    m_ctrlSdoWrite.SetVirtualMode(false);
    m_ctrlSdoWrite.SetFixedRowCount();
    m_ctrlSdoWrite.SetFixedColumnCount(0);
    m_ctrlSdoWrite.SetRowCount(MAXSDOWRITESROWS);
    m_ctrlSdoWrite.SetColumnCount(MAXSDOWRITECOLS);
    m_ctrlSdoWrite.SetGridLines (GVL_NONE);
    m_ctrlSdoWrite.SetColumnResize (FALSE);
    m_ctrlSdoWrite.SetFixedColumnSelection (FALSE);
    m_ctrlSdoWrite.EnableSelection (FALSE);
    m_ctrlSdoWrite.InitControl ();
    m_nTimer=SetTimer (ID_SDOWRITETIMER, 500, NULL);
	m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSdoWrite::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        if (cx!=0&&cy!=0)   {
            m_ctrlSdoWrite.MoveWindow (0, 0, cx-100, cy, TRUE);
            m_ctrlSdoWrite.SetColumnSize ();
            m_btnWriteSdo.MoveWindow (cx-100, 0, 100, 29);
            m_btnDeleteSdo.MoveWindow (cx-100, 30, 100, 29);
            m_btnResetWriteSdo.MoveWindow (cx-100, 60, 100, 29);
        }
    }
}

void CDlgSdoWrite::OnDestroy() 
{
	CDialog::OnDestroy();
    KillTimer (m_nTimer);
    m_ctrlSdoWrite.CloseControl ();    	
}

void CDlgSdoWrite::OnBtnSdoWriteDelete() 
{
	// TODO: Add your control notification handler code here
    CCellID ci=m_ctrlSdoWrite.GetFocusCell ();
    for (int i=0; i<MAXSDOWRITECOLS; i++)
        m_ctrlSdoWrite.SetItemText (ci.row, i, "");
    m_ctrlSdoWrite.SetItemBkColour (ci.row, 6, RGB(0xFF, 0xFF, 0xE0));
    m_ctrlSdoWrite.Invalidate ();
}

void CDlgSdoWrite::OnBtnSdoWrite() 
{
	// TODO: Add your control notification handler code here
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

CAdamoMachine *CDlgSdoWrite::GetMachine() 
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            return pActivePrj->GetMachine ();
    }
    return NULL;
}

void CDlgSdoWrite::StartSDOMonitoring ()
{
    m_bSDOMonitoring=true;        
}

int CDlgSdoWrite::GetSDORequests (stSDORequest *pRequests)
{
    CString str;
    int n=0;

    m_nCount=0;
    while (n<MAXSDOWRITESROWS)   {
        str=m_ctrlSdoWrite.GetItemText (n+1, 1);
        if (str!="")
            if (GetRowData (n+1, &pRequests[m_nCount]))
                if (++m_nCount==NMAX_READ_WRITE_SDO)
                    break;
        m_ctrlSdoWrite.SetItemBkColour (n, 6, RGB(0xFF, 0xFF, 0xE0));
        n++;
    }
    return m_nCount;
}

bool CDlgSdoWrite::GetRowData (int nRow, stSDORequest *pRequests)
{
    CString str;
    int l, t;
    bool b=false;
    char *p;

    /* canale */
    str=m_ctrlSdoWrite.GetItemText (nRow, 0);
    pRequests->m_nCanale=1;
    if (str!="")
        pRequests->m_nCanale=atoi (str);
    /* ricevitore */
    str=m_ctrlSdoWrite.GetItemText (nRow, 1);
    if (str!="")   {
        pRequests->m_nNode=atoi (str);
        /* indice */
        str=m_ctrlSdoWrite.GetItemText (nRow, 2);
        str.TrimLeft (); str.TrimRight ();
        if (str!="")   {
            str.TrimLeft (); str.TrimRight ();
            l=strtol (str, &p, 0);
            if (*p=='\0')   {
                pRequests->m_nIndex=l;
                /* sottoindice */
                str=m_ctrlSdoWrite.GetItemText (nRow, 3);
                str.TrimLeft (); str.TrimRight ();
                if (str!="")   {
                    l=strtol (str, &p, 0);
                    if (*p=='\0')   {
                        pRequests->m_nSubIndex=l;
                        str=m_ctrlSdoWrite.GetItemText (nRow, 4);
                        str.TrimLeft (); str.TrimRight ();
                        if (str!="")   {
                            /* tipo di dato da interpretare */
                            if (GetWriteData (nRow, str, &l, &t, pRequests->m_data))   {
                                pRequests->m_nSDOType=SDOTYPE_WRITE;
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

int CDlgSdoWrite::GetWriteData (int nRow, CString &str, int *l, int *t, BYTE *p)
{
    CString strValue;
    int n=0;
    char *pE;

    strValue=m_ctrlSdoWrite.GetItemText (nRow, 5);
    strValue.TrimLeft (); strValue.TrimRight ();
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_28))   {
        if (str!="")   {
            BYTE *buf=new BYTE [strValue.GetLength ()];
            ((CMainFrame *)AfxGetMainWnd ())->ScanHexBuffer ((BYTE *)strValue.GetBuffer (0), strValue.GetLength (), buf, l);
            strValue.ReleaseBuffer ();
            if (*l&&*l<=NMAX_READ_WRITE_SDO_DATA)   {
                memcpy (p, buf, *l);
                n=*l;
                *t=TYPE_VETTORE_CARATTERI;
            }
            _delete (buf);
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_29))   {
        *l=strValue.GetLength ();
        if (*l)  {
            n=*l;
            strcpy ((char *)p, strValue);
            *t=TYPE_VETTORE_STRINGA;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_30))   {
        double fp;
        *l=sizeof (float);
        fp=strtod (strValue, &pE);
        if (*pE=='\0')   {
            *((float *) p)=(float)fp;
            n=*l;
            *t=TYPE_VETTORE_FLOAT;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_31))   {
        double fp;
        *l=sizeof (double);
        fp=strtod (strValue, &pE);
        if (*pE=='\0')   {
            *((double *) p)=fp;
            n=*l;
            *t=TYPE_VETTORE_DOUBLE;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_32))   {
        long lValue;
        *l=sizeof (char);
        lValue=strtol (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((char *) p)=(char)lValue;
            n=*l;
            *t=TYPE_VETTORE_CHAR;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_33))   {
        long lValue;
        *l=sizeof (BYTE);
        lValue=strtol (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((BYTE *) p)=(BYTE)lValue;
            n=*l;
            *t=TYPE_VETTORE_BYTE;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_34))   {
        long lValue;
        *l=sizeof (short int);
        lValue=strtol (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((short int *) p)=(short)lValue;
            n=*l;
            *t=TYPE_VETTORE_SHORT;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_35))   {
        unsigned long lValue;
        *l=sizeof (WORD);
        lValue=strtoul (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((WORD *) p)=(WORD)lValue;
            n=*l;
            *t=TYPE_VETTORE_WORD;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_36))   {
        long lValue;
        *l=sizeof (int);
        lValue=strtol (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((int *) p)=lValue;
            n=*l;
            *t=TYPE_VETTORE_INT;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_37))   {
        unsigned long lValue;
        *l=sizeof (DWORD);
        lValue=strtoul (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((DWORD *) p)=lValue;
            n=*l;
            *t=TYPE_VETTORE_DWORD;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_41))   {
        int nDimType = sizeof (DWORD) + sizeof (BYTE) + 1;
        if (nDimType < strValue.GetLength ())
            nDimType = strValue.GetLength ();
        BYTE *buf=new BYTE [nDimType];
        memset (buf, 0, nDimType);

        CString str = strValue;
        if ((str[0] == '0' && str[1] == 'x') || (str[0] == '0' && str[1] == 'X'))
            str = str.Right (str.GetLength () - 2);
        ((CMainFrame *)AfxGetMainWnd ())->ScanHexBuffer ((BYTE *)str.GetBuffer (0), str.GetLength (), buf, l);
        str.ReleaseBuffer ();
        if (*l&&*l<=NMAX_READ_WRITE_SDO_DATA)   {
            memcpy (p, buf, *l);
            *l = n=sizeof (DWORD) + sizeof (BYTE);
            *t = TYPE_VETTORE_40BIT;
        }
        _delete (buf);
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_42))   {
        int nDimType = sizeof (DWORD) + sizeof (WORD) + 1;
        if (nDimType < strValue.GetLength ())
            nDimType = strValue.GetLength ();
        BYTE *buf=new BYTE [nDimType];
        memset (buf, 0, nDimType);

        CString str = strValue;
        if ((str[0] == '0' && str[1] == 'x') || (str[0] == '0' && str[1] == 'X'))
            str = str.Right (str.GetLength () - 2);
        ((CMainFrame *)AfxGetMainWnd ())->ScanHexBuffer ((BYTE *)str.GetBuffer (0), str.GetLength (), buf, l);
        str.ReleaseBuffer ();
        if (*l&&*l<=NMAX_READ_WRITE_SDO_DATA)   {
            memcpy (p, buf, *l);
            *l = n=sizeof (DWORD) + sizeof (WORD);
            *t = TYPE_VETTORE_48BIT;
        }
        _delete (buf);
    }
    return n;
}

void CDlgSdoWrite::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    int nRequests;

    if (m_bSDOMonitoring)	 {
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
    }
	CDialog::OnTimer(nIDEvent);
}

void CDlgSdoWrite::ShowResults(stSDOResults *pResults) 
{
    CString str;
    int n=0;

    while (n<m_nCount)   {
        if (!pResults[n].m_pe)
            m_ctrlSdoWrite.SetItemBkColour (m_stSDOGridInfo[n].nRow, 6, RGB (0, 255, 0));
        else
            m_ctrlSdoWrite.SetItemBkColour (m_stSDOGridInfo[n].nRow, 6, RGB (255, 0, 0));
        n++;
    }
    m_ctrlSdoWrite.Invalidate ();
}

void CDlgSdoWrite::OnBtnSdoResetWrite() 
{
    int n=0;
	// TODO: Add your control notification handler code here
    m_bSDOMonitoring=false;
    while (n<MAXSDOWRITESROWS)   {
        m_ctrlSdoWrite.SetItemBkColour (n, 6, RGB(0xFF, 0xFF, 0xE0));
        n++;
    }
    Invalidate ();
}

/*
** Evitiamo di uscire come degli stolti
*/
void CDlgSdoWrite::OnOK ()
{
}

void CDlgSdoWrite::OnCancel ()
{
}

