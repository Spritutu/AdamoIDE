// DlgEcMailBoxWrite.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgEcMailBoxWrite.h"
#include "gridcellcombo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEcMailBoxWrite dialog

static CString* aszRicevitore [NMAX_CANNODES];
static CString* aszTipiDato [9];

CDlgEcMailBoxWrite::CDlgEcMailBoxWrite(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEcMailBoxWrite::IDD, pParent), m_bInitialized (false), m_bEcMonitoring (false), m_slaveArray (NULL)
{
	//{{AFX_DATA_INIT(CDlgEcMailBoxWrite)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEcMailBoxWrite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEcMailBoxWrite)
	DDX_Control(pDX, IDC_BTN_EC_RESET_WRITE, m_btnEcMailBoxReset);
	DDX_Control(pDX, IDC_BTN_EC_READ_WRITE, m_btnEcMailBoxRead);
	DDX_Control(pDX, IDC_BTN_EC_DELETE_WRITE, m_btnEcMailBoxDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEcMailBoxWrite, CDialog)
	//{{AFX_MSG_MAP(CDlgEcMailBoxWrite)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_EC_RESET_WRITE, OnBtnEcResetWrite)
	ON_BN_CLICKED(IDC_BTN_EC_DELETE_WRITE, OnBtnEcDeleteWrite)
	ON_BN_CLICKED(IDC_BTN_EC_READ_WRITE, OnBtnEcReadWrite)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEcMailBoxWrite message handlers

void CDlgEcMailBoxWrite::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        if (cx!=0&&cy!=0)   {
            m_ctrlEcMailBoxWrite.MoveWindow (0, 0, cx-100, cy, TRUE);
            m_ctrlEcMailBoxWrite.SetColumnSize ();
			m_ctrlEcMailBoxWrite.ExpandLastColumn ();
            m_btnEcMailBoxRead.MoveWindow (cx-100, 0, 100, 29);
            m_btnEcMailBoxDelete.MoveWindow (cx-100, 30,  100, 29);
            m_btnEcMailBoxReset.MoveWindow (cx-100, 60,  100, 29);
        }
    }
}

BOOL CDlgEcMailBoxWrite::OnInitDialog() 
{
	RECT rc;

	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	InitButtons ();
    m_ctrlEcMailBoxWrite.Create (rc, this, IDC_GRID_EC_MAILBOX_WRITE, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_ctrlEcMailBoxWrite.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    m_ctrlEcMailBoxWrite.SetEditable(true);
    m_ctrlEcMailBoxWrite.SetVirtualMode(false);
    m_ctrlEcMailBoxWrite.SetFixedRowCount();
    m_ctrlEcMailBoxWrite.SetFixedColumnCount(0);
    m_ctrlEcMailBoxWrite.SetRowCount(MAXECMAILBOXWRITEROWS);
    m_ctrlEcMailBoxWrite.SetColumnCount(MAXECMAILBOXWRITECOLS);
    m_ctrlEcMailBoxWrite.SetGridLines (GVL_NONE);
    m_ctrlEcMailBoxWrite.SetColumnResize (FALSE);
    m_ctrlEcMailBoxWrite.SetFixedColumnSelection (FALSE);
    m_ctrlEcMailBoxWrite.EnableSelection (FALSE);
    m_nTimer=SetTimer (ID_ECMAILBOXWRITETIMER, 500, NULL);
	m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitButtons :
*/
void CDlgEcMailBoxWrite::InitButtons ()
{
	m_btnEcMailBoxReset.SetWindowText (LOADSTRING (IDS_EC_MAILBOX_BUT_RESET));
	m_btnEcMailBoxRead.SetWindowText (LOADSTRING (IDS_EC_MAILBOX_BUT_WRITE));
	m_btnEcMailBoxDelete.SetWindowText (LOADSTRING (IDS_EC_MAILBOX_BUT_DELETE));
}

/* grid construction / destruction */
CEcMailBoxWriteGrid::CEcMailBoxWriteGrid ()
{
}

/*
** InitControl :
*/
void CEcMailBoxWriteGrid::InitControl (CPtrArray &slaveArray)
{
	InitStrings (slaveArray);
    InitColumnCaptions ();
    InitGrid ();
}

/*
** InitControl :
*/
void CEcMailBoxWriteGrid::CloseControl ()
{
	m_ImageList.DeleteImageList ();
    CloseStrings ();
}

/*
** InitControl :
*/
void CEcMailBoxWriteGrid::SetColumnSize ()
{
    RECT rc;
    int n;

    GetClientRect (&rc);
    SetColumnWidth (0, 90);
    SetColumnWidth (1, 90);
    SetColumnWidth (2, 90);
    SetColumnWidth (3, 100);
    SetColumnWidth (5, 30);
    n = rc.right>400 ? rc.right-400: 90;
    SetColumnWidth (4, n);
}

/*
** InitColumnCaptions :
*/
void CEcMailBoxWriteGrid::InitColumnCaptions ()
{
    GV_ITEM Item;
    int i;
    CString str;
    CBitmap bm;

    m_ImageList.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_SDOWRITESYMBOLS);
    m_ImageList.Add(&bm, RGB(0, 128, 128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<MAXECMAILBOXWRITECOLS; i++)   {
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
void CEcMailBoxWriteGrid::InitGrid ()
{
    for (int n=0; n<MAXECMAILBOXWRITEROWS-1; n++)
        InitGridLine (n+1);
}

/*
** InitGridLine :
*/
void CEcMailBoxWriteGrid::InitGridLine (int n)
{
    SetCellType(n, 0, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 1, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 2, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 3, RUNTIME_CLASS(CGridCellCombo));
    SetCellType(n, 4, RUNTIME_CLASS(CGridCell));
    SetCellType(n, 5, RUNTIME_CLASS(CGridCell));
    ((CGridCellCombo*)GetCell(n, 0))->SetOptions(m_slRicevitore);
    ((CGridCellCombo*)GetCell(n, 0))->SetStyle (CBS_DROPDOWNLIST);
    ((CGridCellCombo*)GetCell(n, 3))->SetOptions(m_slTipoDato);
    ((CGridCellCombo*)GetCell(n, 3))->SetStyle (CBS_DROPDOWNLIST);
}

/*
** InitStrings :
*/
void CEcMailBoxWriteGrid::InitStrings (CPtrArray &slaveArray)
{
    int n;

    m_slRicevitore.SetSize(slaveArray.GetSize ());
    for (n=0; n<slaveArray.GetSize (); n++)   {
        aszRicevitore[n]=new CString;
        *aszRicevitore[n]=((stAdamoEthSlave *)slaveArray[n])->szName;
        m_slRicevitore[n]=*aszRicevitore[n];
    }
    m_slTipoDato.SetSize (10);
    for (n=0; n<9; n++)   {
        aszTipiDato[n]=new CString;
        *aszTipiDato[n]=LOADSTRING (IDS_ADAMODGSDOVIEW_29+n);
        m_slTipoDato[n]=*aszTipiDato[n];
    }
}

/*
** CloseStrings :
*/
void CEcMailBoxWriteGrid::CloseStrings ()
{
    int n;

    for (n=0; n<m_slRicevitore.GetSize (); n++)
        _delete (aszRicevitore[n]);
    for (n=0; n<9; n++)
        _delete (aszTipiDato[n]);
}       

/*
** OnEditCell :
*/
void CEcMailBoxWriteGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nCol!=MAXECMAILBOXWRITECOLS-1)
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

/*
** OnDestroy :
*/
void CDlgEcMailBoxWrite::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    KillTimer (m_nTimer);
    m_ctrlEcMailBoxWrite.CloseControl ();    	
}

/*
** LoadEthercatXMLFile :
*/
void CDlgEcMailBoxWrite::LoadEthercatXMLFile (CPtrArray &slaveArray)
{
	m_ctrlEcMailBoxWrite.CloseControl ();    	
	m_ctrlEcMailBoxWrite.InitControl (slaveArray);
	m_slaveArray=&slaveArray;
}

void CDlgEcMailBoxWrite::OnBtnEcResetWrite() 
{
	// TODO: Add your control notification handler code here
	int n=0;

    m_bEcMonitoring=false;
    while (n<MAXECMAILBOXWRITEROWS)   {
        m_ctrlEcMailBoxWrite.SetItemText (n, 4, "");
        m_ctrlEcMailBoxWrite.SetItemBkColour (n, 5, RGB(0xFF, 0xFF, 0xE0));
        n++;
    }
    Invalidate ();
}

void CDlgEcMailBoxWrite::OnBtnEcDeleteWrite() 
{
	// TODO: Add your control notification handler code here
    CCellID ci=m_ctrlEcMailBoxWrite.GetFocusCell ();
    for (int i=0; i<MAXSDOREADCOLS; i++)
        m_ctrlEcMailBoxWrite.SetItemText (ci.row, i, "");
    m_ctrlEcMailBoxWrite.SetItemBkColour (ci.row, 7, RGB(0xFF, 0xFF, 0xE0));
    m_ctrlEcMailBoxWrite.Invalidate ();
}

void CDlgEcMailBoxWrite::OnBtnEcReadWrite() 
{
	// TODO: Add your control notification handler code here
	int nSlave, nIndex, nSubIndex, nLen;
	BYTE pData[NMAX_EC_RW_MAILBOX_DATA];
    if (!m_bEcMonitoring)   {
        CAdamoMachine *pMachine=GetMachine ();
        if (pMachine)   {
			for (int i=0; i<MAXECMAILBOXWRITEROWS; i++)   {
				for (int n=0; n<MAXECMAILBOXWRITECOLS; n++)   {
					int nState=m_ctrlEcMailBoxWrite.GetItemState (i, n);
					if (nState & GVNI_FOCUSED)   {
						m_nMonitoredRow=i;
						if (GetEcMalBoxData (i, &nSlave, &nIndex, &nSubIndex, &nLen, pData))   {
							pMachine->StartEcMailBoxCommand (EC_MAILBOX_TYPE_WRITE, nSlave, nIndex, nSubIndex, nLen, pData);
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
bool CDlgEcMailBoxWrite::GetEcMalBoxData (int nRow, int *nSlave, int *nIndex, int *nSubIndex, int *nLen, BYTE *pData)
{
    CString str;
    int l, t;
    bool b=false;
    char *p;

    /* slave */
    str=m_ctrlEcMailBoxWrite.GetItemText (nRow, 0);
    if (str!="")   {
        *nSlave=GetSlaveIndex (str);
        /* indice */
        str=m_ctrlEcMailBoxWrite.GetItemText (nRow, 1);
        str.TrimLeft (); str.TrimRight ();
        if (str!="")   {
            l=strtol (str, &p, 0);
            if (*p=='\0')   {
                *nIndex=l;
                /* sottoindice */
                str=m_ctrlEcMailBoxWrite.GetItemText (nRow, 2);
                str.TrimLeft (); str.TrimRight ();
                if (str!="")   {
                    l=strtol (str, &p, 0);
                    if (*p=='\0')   {
                        *nSubIndex=l;
                        str=m_ctrlEcMailBoxWrite.GetItemText (nRow, 3);
                        str.TrimLeft (); str.TrimRight ();
                        if (str!="")   {
                            /* tipo di dato da interpretare */
                            if (GetWriteData (nRow, str, &l, &t, pData))   {
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

/*
** GetSlaveIndex :
*/
int CDlgEcMailBoxWrite::GetSlaveIndex (CString str)
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
** GetWriteData :
*/
int CDlgEcMailBoxWrite::GetWriteData (int nRow, CString &str, int *l, int *t, BYTE *p)
{
    CString strValue;
    int n=0;
    char *pE;

    strValue=m_ctrlEcMailBoxWrite.GetItemText (nRow, 4);
    strValue.TrimLeft (); strValue.TrimRight ();
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_28))   {
        if (str!="")   {
            BYTE *buf=new BYTE [strValue.GetLength ()];
            ((CMainFrame *)AfxGetMainWnd ())->ScanHexBuffer ((BYTE *)strValue.GetBuffer (0), strValue.GetLength (), buf, l);
            strValue.ReleaseBuffer ();
            if (*l&&*l<=NMAX_EC_RW_MAILBOX_DATA)   {
                memcpy (p, buf, *l);
                n=*l;
                *t=0;
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
            *t=1;
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
            *t=2;
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
            *t=3;
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
            *t=4;
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
            *t=5;
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
            *t=6;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_35))   {
        long lValue;
        *l=sizeof (WORD);
        lValue=strtol (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((WORD *) p)=(WORD)lValue;
            n=*l;
            *t=7;
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
            *t=8;
        }
    }
    else
    if (str==LOADSTRING (IDS_ADAMODGSDOVIEW_37))   {
        long lValue;
        *l=sizeof (DWORD);
        lValue=strtol (strValue, &pE, 0);
        if (*pE=='\0')   {
            *((DWORD *) p)=lValue;
            n=*l;
            *t=9;
        }
    }
    return n;
}

/*
** GetMachine :
*/
CAdamoMachine *CDlgEcMailBoxWrite::GetMachine() 
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
** OnTimer :
*/
void CDlgEcMailBoxWrite::OnTimer(UINT nIDEvent) 
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
				ShowResults (lResult);
            m_bEcMonitoring=false;
        }
    }
	CDialog::OnTimer(nIDEvent);
}

/*
** ShowResults :
*/
void CDlgEcMailBoxWrite::ShowResults(LONG lResult) 
{
    CString str;
    int n=0;

    if (!lResult)
        m_ctrlEcMailBoxWrite.SetItemBkColour (m_nMonitoredRow, 5, RGB (0, 255, 0));
    else
        m_ctrlEcMailBoxWrite.SetItemBkColour (m_nMonitoredRow, 5, RGB (255, 0, 0));
    n++;
    m_ctrlEcMailBoxWrite.Invalidate ();
}

/*
** Evitiamo di uscire come degli stolti
*/
void CDlgEcMailBoxWrite::OnOK ()
{
}

void CDlgEcMailBoxWrite::OnCancel ()
{
}
