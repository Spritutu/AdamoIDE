// DlgOscilloscope.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "mainframe.h"
#include "Workspace.h"
#include "DlgOscilloscope.h"
#include "IdeMisc.h"
#include "OscProp.h"
#include "AdamoGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NORMAL_TRIGGER  0
#define PRE_TRIGGER     1
#define OSCBUF_LENGTH   8192
#define NMAXXSCALE      7
#define NMAXYSCALE      25

struct stScale   {
    char*  pStr;
    double nDivision;
} ScaleX[] = { 
    "0.1",  10,
    "0.25", 4,
    "0.5",  2,
    "1",    1,
    "2",    0.5,
    "4",    0.25,
    "10",   0.1
};

struct stScale ScaleY[] = { 
    "50000",   100000,
    "20000",    50000,
    "10000",    20000,
    "5000",     10000,
    "2500",      5000,
    "1000",      2000,
    "500",       1000,
    "250",        500,
    "100",        200,
    "50",         100,
    "25",          50,
    "10",          20,
    "5",           10,
    "2.5",          5,
    "1",            2,
    "0.5",          1,
    "0.25",       0.5,
    "0.1",        0.2,
    "0.05",       0.1,
    "0.025",     0.05,
    "0.001",     0.02,
    "0.005",     0.01,
    "0.0025",   0.005,
    "0.001",    0.002,
    "0.0005",   0.001
};

static CString* stStatusOscilloscope[4];

struct    {
    char* pVettStr;
    int   nValue;
} stFreq[NMAXFREQUENCY] = {
    "50",   FREQ50,
    "100",  FREQ100,
    "500",  FREQ500,
    "1000", FREQ1000,
    "1126", FREQ1136,
    "2000", FREQ2000
};

struct    {
    char* pVettStr;
    int   nValue;
} stOscBuf[NMAXOSCBUFFER] = {
    "65k",   OSCBUF64,
    "128k",  OSCBUF128,
    "256k",  OSCBUF256,
    "512k",  OSCBUF512,
    "1024k", OSCBUF1024
};

static CString *aszTipo[MAXAXESDATA];

int anCodiciTipo[MAXAXESDATA] = {
    AX_ST,
    AX_DIR,
    AX_QT,
    AX_QR,
    AX_QENC,
    AX_EA,
    AX_VC,
    AX_VCR,
    AX_ACCINST,
    AX_ACCINSTR,
    AX_JERKINST,
    AX_JERKINSTR,
    AX_FF,
    AX_FFA,
    AX_P,
    AX_I,
    AX_D,
    AX_PG,
    AX_IG,
    AX_DG,
    AX_TORQUE,
};

int anCodiciTipoStringa[MAXAXESSTRINGDATA] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
	0,
	0,
    17,
    18,
    19,
    20
};

extern CString *aszTipoSamples[MAXAXESDATA];

char seps[]   = " ,\t\n";

CDlgOscilloscope* CDlgOscilloscope::m_pThis=NULL;

/////////////////////////////////////////////////////////////////////////////
// CDlgOscilloscope dialog
CDlgOscilloscope::CDlgOscilloscope(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOscilloscope::IDD, pParent), m_hWndOsc (NULL), m_iUnknown (NULL), m_bInitialized (false),
      m_pDlgSamples (NULL), m_pDlgTriggers (NULL), m_pMachine (NULL), m_nAndOr (0),
      m_nFrequency (0), m_nBufferSize (2), m_bOscStarted (false), m_pBuf (NULL),
      m_nContinue (0), m_nOscIndex (0), m_nIndexXScale (3), m_fpBaseSpan (stFreq[m_nFrequency].nValue*10),
      m_nTimerCounter (0), m_bCleared (true), m_nCurSample (-1), m_nOscCount (0), m_nCursorIndex (0), m_nCursorDeltaXIndex (-1),
	  m_nCursorDeltaYIndex (-1), m_nStatus (OSC_READY), m_nWorkStatus (0), m_pFont (NULL)
{
	//{{AFX_DATA_INIT(CDlgOscilloscope)
	//}}AFX_DATA_INIT
	memset (stStatusOscilloscope, 0, sizeof (stStatusOscilloscope));
	memset (aszTipo, 0, sizeof (aszTipo));
    m_pThis=this;
}

BEGIN_MESSAGE_MAP(CDlgOscilloscope, CDialog)
	//{{AFX_MSG_MAP(CDlgOscilloscope)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_XPLUS, OnXPlus)
	ON_BN_CLICKED(IDC_BTN_XMINUS, OnXMinus)
	ON_BN_CLICKED(IDC_BTN_YPLUS, OnYPlus)
	ON_BN_CLICKED(IDC_BTN_YMINUS, OnYMinus)
	//}}AFX_MSG_MAP
    ON_MESSAGE (IDMSG_SELOSCCHAANNEL, OnSelectOscChannel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOscilloscope message handlers

void CDlgOscilloscope::OnSize(UINT nType, int cx, int cy) 
{
	int i;
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_hWndOsc)   {
	    ::MoveWindow (m_hWndOsc, 130, 14, cx-180-130, cy-14, TRUE);
        if (m_bInitialized)   {
            m_ctrlOscStatus.MoveWindow (cx-172, 2, 178, 36);
            for (i=0; i<m_nOscCount; i++)
                m_pOscChannelData[i]->MoveWindow (cx-178, 40+(i*75), 178, 75);
            if (cx)
                m_fpScaleY=((double)cy) / ((double)cx/2.0);
            else    
                m_fpScaleY=1.0;
            AdjustAllYAxes ();
        }
    }
}

BOOL CDlgOscilloscope::OnInitDialog() 
{
    int i;
    CWnd* pWnd;
	
    CDialog::OnInitDialog();
    InitStrings ();
    pWnd = GetDlgItem(IDC_IPLOTX1);
    m_iUnknown = pWnd->GetControlUnknown();
    m_iUnknown->QueryInterface(__uuidof(iPlotLibrary::IiPlotX),(LPVOID*)&m_iPlotX1iDispatch);
    m_hWndOsc=pWnd->GetSafeHwnd();
    AdjustXAxes ();
    m_bInitialized = true;
    for (i=0; i<MAXSAMPLESNUMBER; i++)
        InitOscChannel (i);
    InitVettOscChannelData ();
	InitKnobs ();
	InitButtons ();
	InitLabels ();
	m_iPlotX1iDispatch->DataView[0]->GridLineColor = RGB (128, 128, 128); // hide grid 
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOscilloscope::OnOscStart() 
{
    bool bStartImmediately=false;
	/* per prima cosa recuperiamo i trigger */
    if (m_pDlgTriggers&&m_pDlgSamples)   {
        CleanLists ();
        m_pDlgTriggers->GetTriggers (m_pTriggers);
        if (!m_pTriggers.GetCount ())
            bStartImmediately=true;
        /* ci sono dei trigger, ora i samples */
        m_pDlgSamples->GetSamples (m_pSamples);
        if (m_pSamples.GetCount ())   {
            /* ci sono anche dei samples */
            m_pMachine=GETWS()->GetActiveProject ()->GetMachine ();
            if (m_pMachine)
                m_pMachine->OscStop ();
                if ((bStartImmediately||(!bStartImmediately&&SendTriggers ()))&&SendSamples ())   {
                    /* ok, sono andati giu, via alle danze */
                    //m_pDlgOscBar->OnOscStarted ();
                    m_nWorkStatus=1;
                    /* allochiamo il buffer di transito */
                    m_pBuf=new char [OSCBUF_LENGTH];
                    m_bCleared=false;
                    StartOscilloscope (NORMAL_TRIGGER);
                }
        }        
    }	
}

void CDlgOscilloscope::OnOscStartPretrigger() 
{
	/* per prima cosa recuperiamo i trigger */
    if (m_pDlgTriggers&&m_pDlgSamples)   {
        CleanLists ();
        m_pDlgTriggers->GetTriggers (m_pTriggers);
        if (m_pTriggers.GetCount ())   {
            /* ci sono dei trigger, ora i samples */
            m_pDlgSamples->GetSamples (m_pSamples);
            if (m_pSamples.GetCount ())   {
                /* ci sono anche dei samples */
                m_pMachine=GETWS()->GetActiveProject ()->GetMachine ();
                if (m_pMachine)   {
                    m_pMachine->OscStop ();
                    if (SendTriggers ()&&SendSamples ())   {
                        /* ok, sono andati giu, via alle danze */
                        //m_pDlgOscBar->OnOscStarted ();
                        m_nWorkStatus=1;
                        /* allochiamo il buffer di transito */
                        m_pBuf=new char [OSCBUF_LENGTH];
                        m_bCleared=false;
                        StartOscilloscope (PRE_TRIGGER);
                    }
                }        
            }        
        }        
    }	
}

void CDlgOscilloscope::OnOscStop() 
{
    /* riabilitiemo i bottoni */
    //m_pDlgOscBar->OnOscStopped ();
    m_nWorkStatus=0;
    /* stoppiamo il debugger */
    if (m_bOscStarted)
        StopOscilloscope ();
    m_bCleared=true;
}

void CDlgOscilloscope::OnOscClear() 
{
    int n;
    for (n=0; n<m_pSamples.GetCount (); n++)
        m_iPlotX1iDispatch->GetChannel (n)->Clear ();
	CleanLists ();
    OnOscStop ();
    m_nOscIndex=0, m_bCleared=true;
}

void CDlgOscilloscope::OnOscZoomToFit() 
{
	ZoomToFit ();
}

void CDlgOscilloscope::OnOscDeltaXCursor() 
{
	ShowDeltaXCursor ();
}

void CDlgOscilloscope::OnOscDeltaYCursor() 
{
	ShowDeltaYCursor ();
}

int CDlgOscilloscope::SendTriggers (void)
{
    CDeviceData dd;
    POSITION pos;
    int nB=FALSE;
    bool bFirstTime=true;

    for (pos=m_pTriggers.GetHeadPosition (); pos; )   {
        CAdamoOscEvento* pOsc=(CAdamoOscEvento*)m_pTriggers.GetNext (pos);
        dd.m_eDev=(enDispositivi)pOsc->m_nTypeDevice;
        dd.m_nAddress=pOsc->m_nDevice;
        dd.m_nType=pOsc->m_nDatoAsse;
        if (m_pMachine)
            if (m_pMachine->OscSendEvents (&dd, pOsc->m_nCmp, pOsc->m_nDato, pOsc->m_fpDato)==S_OK)   {
                if (bFirstTime)
                    bFirstTime=false;
                nB=TRUE;
            }
    }
    return nB;
}

int CDlgOscilloscope::SendSamples ()
{
    CVarData vd;
    CDeviceData dd;
    POSITION pos;
    int nB=FALSE, n;

    for (pos=m_pSamples.GetHeadPosition (), n=0; pos; n++)   {
        CAdamoOscDato* pOsc=(CAdamoOscDato*)m_pSamples.GetNext (pos);
		strcpy (vd.m_szVarName, pOsc->szVarName);
		vd.m_nRow = pOsc->m_nRow;
		strcpy (vd.m_szColumnName, pOsc->szColumnName);
		vd.m_nVarType = pOsc->m_nVarType;
        dd.m_eDev=(enDispositivi)pOsc->m_nTypeDevice;
        dd.m_nAddress=pOsc->m_nDevice;
        dd.m_nType=pOsc->m_nDatoAsse;
        if (m_pMachine)
            if (m_pMachine->OscSendData (&vd, &dd)==S_OK)
                nB=TRUE;
    }
    return nB;
}

int CDlgOscilloscope::StartOscilloscope (int nMode)
{
    int nB=FALSE;

    if (m_pMachine)   {
        m_pMachine->OscStart (stFreq[m_nFrequency].nValue, stOscBuf[m_nBufferSize].nValue, m_nAndOr, nMode, !m_pTriggers.GetCount());
        m_bOscStarted=true;
        nB=TRUE;
    }
    return nB;
}

int CDlgOscilloscope::StopOscilloscope ()
{
    bool b=m_pMachine->OscStop () == S_OK;
    m_bOscStarted=false;
    return b;
}

void CDlgOscilloscope::CleanLists ()
{
    POSITION pos;
    int n;

    for (pos=m_pSamples.GetHeadPosition (), n=0; pos; n++)   {
        CAdamoOscDato* pOsc=(CAdamoOscDato*)m_pSamples.GetNext (pos);
        delete pOsc;
    }
    m_pSamples.RemoveAll ();
    for (pos=m_pTriggers.GetHeadPosition (); pos; )   {
        CAdamoOscEvento* pOsc=(CAdamoOscEvento*)m_pTriggers.GetNext (pos);
        delete pOsc;
    }
    m_pTriggers.RemoveAll ();
    _delete (m_pBuf);
}

void CDlgOscilloscope::OnDestroy() 
{
	CDialog::OnDestroy();
    CleanLists ();
    DestroyStrings ();
}

void CDlgOscilloscope::OnAdamoTimer () 
{
    long nRecords, nError;
    char* p=m_pBuf;

    if (m_bOscStarted)    {
        if (m_pMachine&&m_pBuf)    {
            if (m_nContinue)   {
                if (m_pMachine->OscGetStatus (&m_nStatus, &nError) == S_OK)   {
                    /* modalita non in continua, vediamo se fermarci */
                    if (m_nStatus==OSC_JOBDONE)   {
                        do   {
                            if (m_pMachine->OscGetData (&m_nStatus, &nRecords, m_pBuf, OSCBUF_LENGTH) == S_OK)   {
                                p=m_pBuf;
                                if (nRecords)   {
                                    while (nRecords--)
                                        ParseData (&p);
                                }
                            }
                            else
                                break;
                        } while (nRecords);
                        OnOscStop ();
                    }
                }
            }
            else   {
                if (m_pMachine->OscGetData (&m_nStatus, &nRecords, m_pBuf, OSCBUF_LENGTH) == S_OK)   {
                    p=m_pBuf;
                    if (nRecords)
                        while (nRecords--)
                            ParseData (&p);
                }
            }
        }
        m_nTimerCounter=10;
        OnOscStatus (*stStatusOscilloscope[m_nStatus]);
    }
    else
        if (!m_nTimerCounter)
            OnOscStatus (*stStatusOscilloscope[OSC_READY]);
        else   {
            OnOscStatus (*stStatusOscilloscope[OSC_JOBDONE]);
            m_nTimerCounter--;
        }
}

int CDlgOscilloscope::ParseData (char** p)
{
    POSITION pos;
    int i;

    for (pos=m_pSamples.GetHeadPosition (), i=0; pos; i++)   {
        CAdamoOscDato* pOsc=(CAdamoOscDato*)m_pSamples.GetNext (pos);
		if (pOsc->szVarName[0] != '\0')
			*p+=ParseVariables (*p, pOsc, i);
		else
			if (pOsc->m_nTypeDevice==enAsse)
				*p+=ParseAxes (*p, pOsc, i);
			else
				*p+=ParseDevices (*p, pOsc, i);
    }
    m_nOscIndex++;
    return 0;
}

int CDlgOscilloscope::ParseVariables (char* p, CAdamoOscDato* pOsc, int nChannel)
{
    int nLengthData = 0;
    switch (pOsc->m_nVarType)   {
        case LUA_TBOOLEAN :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((int*)p));
            nLengthData=4;
            break;
        case LUA_TNUMBER :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
	}
    return nLengthData;
}

int CDlgOscilloscope::ParseAxes (char* p, CAdamoOscDato* pOsc, int nChannel)
{
    int nLengthData;
    switch (pOsc->m_nDatoAsse)   {
        case AX_ST :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((char*)p));
            nLengthData=1;
            break;
        case AX_DIR :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((char*)p));
            nLengthData=1;
            break;
        case AX_QENC :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_QR :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_QT :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_EA :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_VC :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_VCR :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_ACCINST :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_ACCINSTR :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_JERKINST :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_JERKINSTR :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_FF :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_FFA :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_P :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_I :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_D :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_PG:
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_IG:
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_DG:
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case AX_ADJUST :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((int*)p));
            nLengthData=4;
            break;
        case AX_DAC :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((int*)p));
            nLengthData=4;
            break;
        case AX_TORQUE :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
    }
    return nLengthData;
}

int CDlgOscilloscope::ParseDevices (char* p, CAdamoOscDato* pOsc, int nChannel)
{
    int nLengthData;
    switch (pOsc->m_nTypeDevice)   {
        case enInputDigitale :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((int*)p));
            nLengthData=4;
            break;
        case enOutputDigitale :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, (double)*((int*)p));
            nLengthData=4;
            break;
        case enInputAnalogico :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case enOutputAnalogico :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case enTimer :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
        case enCounter :
            m_iPlotX1iDispatch->GetChannel (nChannel)->AddXY (m_nOscIndex, *((double*)p));
            nLengthData=8;
            break;
    }
    return nLengthData;
}

void CDlgOscilloscope::OnOscOptions() 
{
    COscProp dlg;

    dlg.m_nMode=m_nContinue;
    dlg.m_nTriggerAnd=m_nAndOr;
    dlg.m_nDimBuffer=m_nBufferSize;
    dlg.m_nFrequency=m_nFrequency;
    if (dlg.DoModal ()==IDOK)   {
        m_nContinue = dlg.m_nMode;
        m_nAndOr = dlg.m_nTriggerAnd;
        m_nBufferSize=dlg.m_nDimBuffer;
        m_nFrequency=dlg.m_nFrequency;
        m_fpBaseSpan=stFreq[m_nFrequency].nValue*10;
        m_iPlotX1iDispatch->GetXAxis (0)->put_Span (m_fpBaseSpan);
		AdjustXAxes ();
    }    
}

void CDlgOscilloscope::OnOscLoad () 
{
    CPath strOscPath;
    bool bGoOn=true;
    if (m_pDlgSamples->GetGridCount ()||m_pDlgTriggers->GetGridCount ())   {
        if (AfxMessageBox (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_25), MB_YESNO)==IDNO)
            bGoOn=false;
    }
    if (bGoOn)   {
        CPath strPath=CreateOscDir ();
        strPath&="*.osc";
	    CKSFileDialog fd(TRUE, "osc", strPath, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, 
		    LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_26), AfxGetMainWnd());
	    if ( fd.DoModal()==IDOK ) 	{
            /* ok, andiamo a caricare i dati della tracciata */
            BeginWaitCursor ();
            OnOscClear ();
            if (GetOscFile (fd.GetPathName(), strOscPath))   {
                m_iPlotX1iDispatch->PutDataFileColumnSeparator (iPlotLibrary::ipfcsTab);
                m_iPlotX1iDispatch->LoadDataFromFile (CString (strOscPath).AllocSysString());
            }
            CFile::Remove (strOscPath);
            EndWaitCursor ();
        }
    }
}

void CDlgOscilloscope::OnOscSave () 
{
    CPath strPath=CreateOscDir ();
    strPath&="*.osc";
	CKSFileDialog fd(FALSE, "osc", strPath, OFN_PATHMUSTEXIST, 
		LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_26), AfxGetMainWnd());
	if ( fd.DoModal()==IDOK ) 	{
        /* ok, andiamo a salvare i dati della tracciata */
        BeginWaitCursor ();
        strPath=fd.GetPathName();
        m_iPlotX1iDispatch->PutDataFileColumnSeparator (iPlotLibrary::ipfcsTab);
        m_iPlotX1iDispatch->SaveDataToFile (CString(strPath).AllocSysString());
        SaveEpilogue (strPath);
        EndWaitCursor ();
    }
}

void CDlgOscilloscope::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOscilloscope)
	DDX_Control(pDX, IDC_OSCSTATUS, m_ctrlOscStatus);
	DDX_Control(pDX, IDC_OSCCHANNELDATA8, m_ctrlOscChannelData8);
	DDX_Control(pDX, IDC_OSCCHANNELDATA7, m_ctrlOscChannelData7);
	DDX_Control(pDX, IDC_OSCCHANNELDATA6, m_ctrlOscChannelData6);
	DDX_Control(pDX, IDC_OSCCHANNELDATA5, m_ctrlOscChannelData5);
	DDX_Control(pDX, IDC_OSCCHANNELDATA4, m_ctrlOscChannelData4);
	DDX_Control(pDX, IDC_OSCCHANNELDATA3, m_ctrlOscChannelData3);
	DDX_Control(pDX, IDC_OSCCHANNELDATA2, m_ctrlOscChannelData2);
	DDX_Control(pDX, IDC_OSCCHANNELDATA1, m_ctrlOscChannelData1);
	DDX_Control(pDX, IDC_IKNOBX_XAXIS, m_knobXAxis);
	DDX_Control(pDX, IDC_IKNOBX_YAXIS, m_knobYAxis);
	DDX_Control(pDX, IDC_BTN_XPLUS, m_btnXPlus);
	DDX_Control(pDX, IDC_BTN_YPLUS, m_btnYPlus);
	DDX_Control(pDX, IDC_BTN_XMINUS, m_btnXMinus);
	DDX_Control(pDX, IDC_BTN_YMINUS, m_btnYMinus);
	DDX_Control(pDX, IDC_XVALUE, m_stXValue);
	DDX_Control(pDX, IDC_YVALUE, m_stYValue);
	DDX_Control(pDX, IDC_XUM, m_stXUMM);
	DDX_Control(pDX, IDC_YUM, m_stYUMM);

	//}}AFX_DATA_MAP
}

void CDlgOscilloscope::AddChannels ()
{
    CPtrList pLD;
    POSITION pos;
    int i;
    char szBuffer[256];

    m_iPlotX1iDispatch->RemoveAllChannels ();
    CSamplesGrid& sg=m_pDlgSamples->GetSamplesGrid ();
    sg.GetSamplesDevices (pLD);
	for (i=0; i<pLD.GetCount (); i++)
        m_iPlotX1iDispatch->AddChannel();
	for (pos=pLD.GetHeadPosition (), i=0; pos; i++)   {
        CAdamoBase* pDev=(CAdamoBase*) pLD.GetNext (pos);
        m_iPlotX1iDispatch->GetChannel (i)->PutTitleText (pDev->GetName ().AllocSysString ());
        sprintf (szBuffer, (const char *) LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_28), i+1);
        m_iPlotX1iDispatch->GetChannel (i)->put_XAxisName (_bstr_t (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_29)));
        m_iPlotX1iDispatch->GetChannel (i)->put_YAxisName (_bstr_t (szBuffer));
        m_iPlotX1iDispatch->GetChannel (i)->put_Visible (TRUE);
        m_iPlotX1iDispatch->GetChannel (i)->put_VisibleInLegend (FALSE);
        m_iPlotX1iDispatch->GetChannel (i)->put_Color ((OLE_COLOR) COscChannelData::m_colors[i]);
        sprintf (szBuffer, (const char *) LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_27), i);
        m_iPlotX1iDispatch->GetChannel (i)->put_Name (_bstr_t (szBuffer));
    }
    pLD.RemoveAll ();
}

void CDlgOscilloscope::AddYAxes ()
{
    CPtrList pLD;
    POSITION pos;
    int i;
    char szBuffer[256];

    m_iPlotX1iDispatch->RemoveAllYAxes ();
    CSamplesGrid& sg=m_pDlgSamples->GetSamplesGrid ();
    sg.GetSamplesDevices (pLD);
	for (i=0; i<pLD.GetCount (); i++)
        m_iPlotX1iDispatch->AddYAxis ();
	for (pos=pLD.GetHeadPosition (), i=0; pos; i++)   {
        CAdamoBase* pDev=(CAdamoBase*) pLD.GetNext (pos);
        sprintf (szBuffer, (const char *) LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_28), i+1);
        m_iPlotX1iDispatch->GetYAxis (i)->put_Name(_bstr_t (szBuffer));
        if (i>0)
            m_iPlotX1iDispatch->GetYAxis (i)->put_AlignRefAxisName (_bstr_t (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_30)));
        m_iPlotX1iDispatch->GetYAxis (i)->put_Visible(FALSE);
        m_iPlotX1iDispatch->GetYAxis (i)->put_DesiredIncrement (10.0);
        m_iPlotX1iDispatch->GetYAxis (i)->put_TrackingStyle ((iPlotLibrary::TxiPlotTrackingStyle)3);
        InitOscChannel (i);
    }
    pLD.RemoveAll ();
}

void CDlgOscilloscope::InitIPlotX ()
{
    AddChannels ();
    AddYAxes ();
    AdjustAllYAxes ();
}

void CDlgOscilloscope::OnXMinus() 
{
    if (m_nIndexXScale)   {
        m_nIndexXScale--;    
        AdjustXAxes ();
		m_knobXAxis.SetPosition ((double)m_nIndexXScale*16.66);
		SetXLabel ();
	}
}

void CDlgOscilloscope::OnXPlus()
{
    if (m_nIndexXScale<NMAXXSCALE-1)   {
        m_nIndexXScale++;
        AdjustXAxes ();
		m_knobXAxis.SetPosition ((double)m_nIndexXScale*16.66);
		SetXLabel ();	
	}
}

void CDlgOscilloscope::AdjustXAxes ()
{
    double m_fpCurSpan;
    m_fpCurSpan=m_fpBaseSpan/ScaleX[m_nIndexXScale].nDivision;
    m_iPlotX1iDispatch->GetXAxis (0)->put_Span (m_fpCurSpan);
    m_iPlotX1iDispatch->GetXAxis (0)->put_DesiredIncrement (m_fpCurSpan / 10);
}

void CDlgOscilloscope::AdjustAllYAxes ()
{
    int i;

	for (i=0; i<m_iPlotX1iDispatch->GetChannelCount (); i++)
        AdjustYAxes (i);
}

void CDlgOscilloscope::ShowOscChannelData ()
{
    CPtrList pL;
    RECT rc;
    int i=0, n;

    GetWindowRect (&rc);
    CSamplesGrid& sg=m_pDlgSamples->GetSamplesGrid ();
    sg.GetSamplesDevices (pL);
    m_nOscCount=pL.GetCount ();
	for (POSITION pos=pL.GetHeadPosition(); pos; i++)   {
        CAdamoBase* pAD=(CAdamoBase*) pL.GetNext (pos);
        m_pOscChannelData[i]->MoveWindow (rc.right-180, 40+(i*75), 178, 75);
        m_pOscChannelData[i]->ShowWindow (SW_SHOW);
        m_pOscChannelData[i]->SetChannelNumber (i);
        m_pOscChannelData[i]->SetAdamoDevice (pAD);
        m_pOscChannelData[i]->SetName (pAD->GetName ());
        if (pAD->GetType ()==enAsse)
            m_pOscChannelData[i]->SetType (sg.GetItemText (i+1, 2));
		else
			if (pAD->GetType ()==enBase)   {
				CString str;
				if (pAD->GetDeviceID () != -1)
					if (pAD->GetDesc () != "")
						str.Format ("%s %d - %s %s", LOADSTRING (IDS_ADAMODLGSAMPLES_5), pAD->GetDeviceID (), LOADSTRING (IDS_ADAMODLGSAMPLES_6), pAD->GetDesc ());
					else
						str.Format ("%s %d", LOADSTRING (IDS_ADAMODLGSAMPLES_7), pAD->GetDeviceID ());
				m_pOscChannelData[i]->SetType (str);
			}
    }
    if (i)
        OnSelectOscChannel (0, 0);
    for (n=i; n<MAXSAMPLESNUMBER; n++)
        m_pOscChannelData[n]->ShowWindow (SW_HIDE);
    pL.RemoveAll ();
}

void CDlgOscilloscope::OnYMinus() 
{
    if (m_nCurSample != -1 && m_OscCanali[m_nCurSample].nScale>0)   {
        m_OscCanali[m_nCurSample].nScale--;
        AdjustYAxes (m_nCurSample);
		m_knobYAxis.SetPosition ((double)m_OscCanali[m_nCurSample].nScale*4.16);
		SetYLabel ();
	}
}

void CDlgOscilloscope::OnYPlus() 
{
    if (m_nCurSample != -1 && m_OscCanali[m_nCurSample].nScale<NMAXYSCALE-1)   {
        m_OscCanali[m_nCurSample].nScale++;
        AdjustYAxes (m_nCurSample);
		m_knobYAxis.SetPosition ((double)m_OscCanali[m_nCurSample].nScale*4.16);
		SetYLabel ();
    }
}

void CDlgOscilloscope::InitOscChannel (int n)
{
    m_OscCanali[n].pScale=ScaleY;
    m_OscCanali[n].nScale=9;
}

void CDlgOscilloscope::AdjustYAxes (int i)
{
    int n=m_OscCanali[i].nScale;
    double fpScaleMax=m_OscCanali[i].pScale[n].nDivision*5.0;
    double fpOldMin, fpOldSpan, fpNewSpan, fpNewMin;

    m_iPlotX1iDispatch->GetYAxis (i)->get_Min (&fpOldMin);
    m_iPlotX1iDispatch->GetYAxis (i)->get_Span (&fpOldSpan);
    fpNewSpan=fpScaleMax*m_fpScaleY;
    m_iPlotX1iDispatch->GetYAxis (i)->put_Span (fpNewSpan);
    if (m_bCleared)
        m_iPlotX1iDispatch->GetYAxis (i)->put_Min (-(((fpScaleMax*m_fpScaleY)/MAXSAMPLESNUMBER)*((MAXSAMPLESNUMBER-1)-i)));
    else    {
        fpNewMin=fpNewSpan*fpOldMin/fpOldSpan;
        m_iPlotX1iDispatch->GetYAxis (i)->put_Min (fpNewMin);
    }
}

void CDlgOscilloscope::InitVettOscChannelData ()
{
	m_pOscChannelData[0]=&m_ctrlOscChannelData1;
	m_pOscChannelData[1]=&m_ctrlOscChannelData2;
	m_pOscChannelData[2]=&m_ctrlOscChannelData3;
	m_pOscChannelData[3]=&m_ctrlOscChannelData4;
	m_pOscChannelData[4]=&m_ctrlOscChannelData5;
	m_pOscChannelData[5]=&m_ctrlOscChannelData6;
	m_pOscChannelData[6]=&m_ctrlOscChannelData7;
	m_pOscChannelData[7]=&m_ctrlOscChannelData8;
}

LRESULT CDlgOscilloscope::OnSelectOscChannel (WPARAM wParam, LPARAM lParam)
{
    int i;

    for (i=0; i<m_nOscCount; i++)   { 
        m_iPlotX1iDispatch->GetYAxis (i)->put_Visible(FALSE);
        m_pOscChannelData[i]->Select (false);
    }    
    m_iPlotX1iDispatch->GetYAxis (wParam)->put_Visible(TRUE);
    m_pOscChannelData[wParam]->Select ();
    for (i=0; i<m_nOscCount; i++)
        m_pOscChannelData[i]->Invalidate ();
    m_nCurSample=wParam;
	m_knobYAxis.SetPosition ((double)m_OscCanali[m_nCurSample].nScale*4.16);
	SetYLabel ();
	if (m_pOscChannelData[m_nCurSample]->GetAdamoDevice ()->GetType () != enBase)
		SetUnitLabel (m_pOscChannelData[m_nCurSample]->GetAdamoDevice (), m_pOscChannelData[m_nCurSample]->GetType ());
    return 0;
}

void CDlgOscilloscope::InitOscilloscope ()
{
    if (!m_nOscCount||(m_nOscCount&&(m_pDlgSamples->IsModified () || m_pDlgTriggers->IsModified ())))   {
        OnOscClear();
        InitIPlotX ();
        ShowOscChannelData ();
    }
    m_pDlgSamples->ResetModified (); m_pDlgTriggers->ResetModified ();
}

void CDlgOscilloscope::OnOscCursor ()
{
    if (m_nCursorIndex)   {
        m_iPlotX1iDispatch->DeleteDataCursor (m_nCursorIndex);
        ResetCursorValues ();
        m_nCursorIndex=0;
    }
    else   {
        m_nCursorIndex=m_iPlotX1iDispatch->AddDataCursor ();
        m_iPlotX1iDispatch->GetDataCursor (m_nCursorIndex)->put_Visible (TRUE);
        SetCursorName (m_nCurSample);
    }
}

void CDlgOscilloscope::SetCursorName (int nIndexChannel)
{
    char szBuffer[256];

    if (m_nCursorIndex)   {
        sprintf (szBuffer, (const char *) LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_27), nIndexChannel);
        m_iPlotX1iDispatch->GetDataCursor (m_nCursorIndex)->put_ChannelName (_bstr_t ("<All>"));
    }
}

BEGIN_EVENTSINK_MAP(CDlgOscilloscope, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgOscilloscope)
	ON_EVENT(CDlgOscilloscope, IDC_IPLOTX1, 6 /* OnDataCursorChange */, OnDataCursorChange, VTS_I4)
	ON_EVENT(CDlgOscilloscope, IDC_IKNOBX_XAXIS, 4 /* OnPositionChange */, OnPositionChangeIknobX, VTS_NONE)
	ON_EVENT(CDlgOscilloscope, IDC_IKNOBX_XAXIS, 11 /* OnMouseUp */, OnMouseUpIknobX, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgOscilloscope, IDC_IKNOBX_XAXIS, 13, OnPositionChangeFinishedIknobX, VTS_NONE)
	ON_EVENT(CDlgOscilloscope, IDC_IKNOBX_YAXIS, 4 /* OnPositionChange */, OnPositionChangeIknobY, VTS_NONE)
	ON_EVENT(CDlgOscilloscope, IDC_IKNOBX_YAXIS, 11 /* OnMouseUp */, OnMouseUpIknobY, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgOscilloscope, IDC_IKNOBX_YAXIS, 13, OnPositionChangeFinishedIknobY, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDlgOscilloscope::OnDataCursorChange(long Index) 
{
    CString str;
    double fp;
    int i;

    if (m_nCursorIndex)   {
        i=0;
        for (POSITION pos=m_pSamples.GetHeadPosition (); pos; i++)   {
            CAdamoOscDato* pOscDato=(CAdamoOscDato*)m_pSamples.GetNext (pos);
            m_iPlotX1iDispatch->GetChannel (i)->get_DataCursorYValue (&fp);
			if (pOscDato->szVarName[0] != '\0')   {
				switch (pOscDato->m_nVarType)   {
					case LUA_TNUMBER :
						str.Format ("%8.3f", fp);
						break;
					case LUA_TBOOLEAN :
						str.Format ("%d", (int) fp);
						break;
				}
			}
			else   {
				if (pOscDato->m_nTypeDevice==enAsse)   {
					switch (pOscDato->m_nDatoAsse)   {
						case AX_ST :
							switch ((int)fp)   {
								case 1 :
									str=LOADSTRING (IDS_ADAMOSTATOACCELERAZIONE);
									break;
								case 2 :
									str=LOADSTRING (IDS_ADAMOSTATOREGIME);
									break;
								case 3 :
									str=LOADSTRING (IDS_ADAMOSTATODECELERAZIONE);
									break;
								case 4 :
									str=LOADSTRING (IDS_ADAMOSTATOQUOTA);
									break;
								case 5 :
									str=LOADSTRING (IDS_ADAMOSTATOFINESTRA);
									break;
								case 6 :
									str=LOADSTRING (IDS_ADAMOSTATOATTESAASSEFERMO);
									break;
								default :
									str="Guru";
									break;
							}
							break;
						case AX_DIR :
							switch ((int)fp)   {
								case 1 :
									str=LOADSTRING (IDS_ADAMOPOSITIVA);
									break;
								case -1 :
									str=LOADSTRING (IDS_ADAMONEGATIVA);
									break;
								default :
									str="Guru";
									break;
							}
							break;
						case AX_QENC :
						case AX_QR :
						case AX_QT :
						case AX_TORQUE :
							str.Format ("%8.2f", fp);
							break;
						case AX_EA :
						case AX_VC :
						case AX_VCR :
							str.Format ("%8.3f", fp);
							break;
						case AX_ACCINST :
						case AX_ACCINSTR :
						case AX_JERKINST :
						case AX_JERKINSTR :
						case AX_FF :
						case AX_FFA :
						case AX_P :
						case AX_I :
						case AX_D :
						case AX_PG:
						case AX_IG:
						case AX_DG:
							str.Format ("%8.6f", fp);
							break;
						case AX_ADJUST :
						case AX_DAC :
							str.Format ("%d", (int)fp);
							break;
					}
				}
				else   {
					switch (pOscDato->m_nTypeDevice)   {
						case enInputDigitale :
						case enOutputDigitale :
							str=fp==0.0 ? LOADSTRING (IDS_ADAMODISATTIVO) : LOADSTRING (IDS_ADAMOATTIVO);
							break;
						case enInputAnalogico :
						case enOutputAnalogico :
						case enTimer :
						case enCounter :
						case enGenericCan :
							str.Format ("%8.2f", fp);
							break;
					}
				}
			}
            m_pOscChannelData[i]->SetCurrentValue (str);
         }
    }
}

void CDlgOscilloscope::ResetCursorValues ()
{
    for (int i=0; i<m_nOscCount; i++)
        m_pOscChannelData[i]->SetCurrentValue ("");
}

void CDlgOscilloscope::OnOscStatus (const char* s)
{
    static char strOsc[32] = { '\0' };

    if (strcmp (s, strOsc))   {
        m_ctrlOscStatus.SetWindowText (s);
        strcpy (strOsc, s);
    }
}

/*
** CreateOscDir
*/
CPath CDlgOscilloscope::CreateOscDir ()
{
    CPath strPath;

    strPath=GETWS()->GetActiveProject ()->GetProjectDir ();
    strPath&=DIROSC;
    if (_access (strPath, 00))
        CreateDirectory(strPath, NULL);
    return strPath;
}

/*
** SaveEpilogue :
*/
void CDlgOscilloscope::SaveEpilogue (CPath strFile)
{
    POSITION pos;
    FILE* pFile;
    CString str, strDevice;
    double fpMin, fpSpan;
    int n;

    /* andiamo a fine file */
    if (pFile=fopen (strFile, "a+t"))   {
        /* scritta Epilogue */
        fputs ("Epilogue\n", pFile);
        /* piantiamo dentro un bel newline */
        fputs ("\n", pFile);
        /* scriviamo i dati globali della tracciata */
        str.Format ("%d, %d, %d, %d\n", m_nFrequency, m_nBufferSize, m_nContinue, m_nAndOr);
        fputs (str, pFile);
        /* un altro bel newline */
        fputs ("\n", pFile);
        /* scriviamo trigger */
        fputs ("Triggers\n", pFile);
        /* ora la lista dei trigger, 
            - path del dispositivo
            - tipo del dispositivo
            - tipo
            - azione
            - valore */
        for (pos=m_pTriggers.GetHeadPosition (); pos; )   {
            CAdamoOscEvento* pOsc=(CAdamoOscEvento*)m_pTriggers.GetNext (pos);
            if (!GetDeviceName (pOsc->m_nDevice, pOsc->m_nTypeDevice, strDevice))
                str.Format ("%s, %d, %d, %d, %d, %8.3f\n", strDevice, pOsc->m_nTypeDevice, pOsc->m_nDatoAsse, pOsc->m_nCmp, pOsc->m_nDato, pOsc->m_fpDato);
            fputs (str, pFile);
        }
        /* piantiamo dentro un bel newline */
        fputs ("\n", pFile);
        /* scriviamo samples */
        fputs ("Samples\n", pFile);
        /* e la lista dei samples 
            - path del dispositivo
            - tipo del dispositivo
            - stato
            - scale index
            - span
            - min
        */
        for (n=0, pos=m_pSamples.GetHeadPosition (); pos; n++)   {
            CAdamoOscDato* pOsc=(CAdamoOscDato*)m_pSamples.GetNext (pos);
            if (!GetDeviceName (pOsc->m_nDevice, pOsc->m_nTypeDevice, strDevice))   {
                m_iPlotX1iDispatch->GetYAxis (n)->get_Min (&fpMin);
                m_iPlotX1iDispatch->GetYAxis (n)->get_Span (&fpSpan);
                str.Format ("%s, %d, %d %d %8.3f %8.3f\n",
                    strDevice,
                    pOsc->m_nTypeDevice,
                    pOsc->m_nDatoAsse, 
                    m_OscCanali[n].nScale,
                    fpSpan,
                    fpMin);
                fputs (str, pFile);
            }
        }
        fclose (pFile);
    }
}

/*
** GetDeviceName :
*/
int CDlgOscilloscope::GetDeviceName (int nDevice, int nTypeDevice, CString& strDevice)
{
    int n=-1;

    CAdamoDatiFile* pADF=new CAdamoDatiFile;
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)   {
        if (pADF->Open (pPrj->GetParamsPathName(), TRUE))   {
            pADF->ReadData ();
            if (!pADF->GetDevicePathName (nDevice, nTypeDevice, strDevice.GetBuffer (_MAX_PATH)))
                n=0;
            strDevice.ReleaseBuffer ();
            pADF->Close ();
        }
    }
    _delete (pADF);
    return n;
}

/*
** GetOscFile :
*/
bool CDlgOscilloscope::GetOscFile (CString strPathName, CPath& strOscPath)
{
    CString strPath, strFile, strRecord;
    CStdioFile tmpFile, sourceFile;
    bool bWrongData=false;
    /* proviamo a prendere il path dei file temporanei */
    if (GetTempPath (_MAX_PATH, strPath.GetBuffer (_MAX_PATH)))
        strPath.ReleaseBuffer ();
    else
        strPath=CreateOscDir ();
    /* creaimo il file temporaneo */
    GetTempFileName (strPath, "tmp", 1, strFile.GetBuffer (_MAX_PATH));
    strFile.ReleaseBuffer ();
    /* apriamo i file */
    if (tmpFile.Open (strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))   {
        if (sourceFile.Open (strPathName, CFile::modeRead | CFile::typeText))   {
            /* cominciamo a tradurre */
            while (sourceFile.ReadString (strRecord))
                if (strRecord=="Epilogue")   {
                    /* new line */
                    sourceFile.ReadString (strRecord);
                    /* record dei dati di configurazione */
                    sourceFile.ReadString (strRecord);
                    GetTokensConf (strRecord.GetBuffer (strRecord.GetLength ()+1));
                    strRecord.ReleaseBuffer ();
                    /* settiamo la scala delle X in base alla frequenza registrata */
                    m_fpBaseSpan=stFreq[m_nFrequency].nValue*10;
                    m_iPlotX1iDispatch->GetXAxis (0)->put_Span (m_fpBaseSpan);
                    /* new line */
                    sourceFile.ReadString (strRecord);
                    /* Triggers */
                    sourceFile.ReadString (strRecord);
                    /* tiriamo su ora i triggers */
                    sourceFile.ReadString (strRecord);
                    CString strTriggerName;
                    double  fpValue;
                    int     nTipoDev, nDatoAsse, nCmp, nValue;
                    /* cancelliamo tutti i dati nel grid */
                    m_pDlgTriggers->DeleteAllRows ();
                    while (strRecord!="")   {
                        GetTokensTrigger (strRecord.GetBuffer (strRecord.GetLength()+1), strTriggerName.GetBuffer (_MAX_PATH), &nTipoDev, &nDatoAsse, &nCmp, &nValue, &fpValue);
                        strRecord.ReleaseBuffer ();
                        strTriggerName.ReleaseBuffer ();
                        if (!m_pDlgTriggers->AddFromFile (strTriggerName, nTipoDev, nDatoAsse, nCmp, nValue, fpValue))
                            bWrongData=true;
                        sourceFile.ReadString (strRecord);
                    }
                    /* riempiamo la lista dei trigger */
                    m_pDlgTriggers->GetTriggers (m_pTriggers);
                    /* il new line lo abbiamo gia' letto */
                    /* Samples */
                    sourceFile.ReadString (strRecord);
                    /* tiriamo su ora i samples */
                    CString strSampleName;
                    int nIndiceDiScala[MAXSAMPLESNUMBER], i=0;
                    double fpControlMin[MAXSAMPLESNUMBER], fpControlSpan[MAXSAMPLESNUMBER];
                    bool b=true;
                    /* cancelliamo tutti i dati nel grid */
                    m_pDlgSamples->DeleteAllRows ();
                    if (sourceFile.ReadString (strRecord))   {
                        while (b)   {
                            GetTokensSample (strRecord.GetBuffer (strRecord.GetLength()+1), strSampleName.GetBuffer (_MAX_PATH), &nTipoDev, &nDatoAsse, &nIndiceDiScala[i], &fpControlMin[i], &fpControlSpan[i]);
                            strSampleName.ReleaseBuffer ();
                            if (!m_pDlgSamples->AddFromFile (strSampleName, nTipoDev, nDatoAsse))
                                bWrongData=true;
                            b=sourceFile.ReadString (strRecord)!=0;
                            i++;
                        }
                        if (!bWrongData)
                            m_pDlgSamples->GetSamples (m_pSamples);
                    }
                    if (!bWrongData)   {
                        InitIPlotX ();
                        ShowOscChannelData ();
                        for (int n=0; n<MAXSAMPLESNUMBER; n++)
                            if (n<i)
                                InitializeTrace (n, nIndiceDiScala[n], fpControlMin[n], fpControlSpan[n]);
                            else   {
                                m_OscCanali[i].pScale=NULL;
                                m_OscCanali[i].nScale=0;
                            }
                    }
                }
                else
                    tmpFile.WriteString (strRecord+'\n');
            sourceFile.Close ();
        }
        tmpFile.Close ();
    }
    strOscPath=strFile;
    if (bWrongData)   {
        m_pDlgTriggers->DeleteAllRows ();
        m_pDlgSamples->DeleteAllRows ();
    }            
    return !bWrongData;
}

void CDlgOscilloscope::InitializeTrace (int i, int nIndiceDiScala, double fpControlMin, double fpControlSpan)
{
    m_OscCanali[i].pScale=ScaleY;
    m_OscCanali[i].nScale=nIndiceDiScala;
    m_iPlotX1iDispatch->GetYAxis (i)->put_Min (fpControlSpan);
    m_iPlotX1iDispatch->GetYAxis (i)->put_Span (fpControlMin);
}

void CDlgOscilloscope::GetTokensConf (char* p)
{
    int i=0;
    char* tok=strtok (p, seps);
    while (tok!=NULL)   {
        strdblk (tok);
        switch (i)   {
            case 0 :
                m_nFrequency=atoi (tok);
                break;
            case 1 :
                m_nBufferSize=atoi (tok);
                break;
            case 2 :
                m_nContinue=atoi (tok);
                break;
            case 3 :
                m_nAndOr=atoi (tok);
                break;
        }
        tok=strtok (NULL, seps);
        i++;
    }
}

void CDlgOscilloscope::GetTokensTrigger (char *p, char* szTriggerName, int* nTipoDev, int* nDatoAsse, int* nCmp, int* nValue, double* fpValue)
{
    int i=0;
    char* tok=strtok (p, seps);
    while (tok!=NULL)   {
        strdblk (tok);
        switch (i)   {
            case 0 :
                strcpy (szTriggerName, strdblk (tok));
                break;
            case 1 :
                *nTipoDev=atoi (tok);
                break;
            case 2 :
                *nDatoAsse=atoi (tok);
                break;
            case 3 :
                *nCmp=atoi (tok);
                break;
            case 4 :
                *nValue=atoi (tok);
                break;
            case 5 :
                *fpValue=atof (tok);
                break;
        }
        tok=strtok (NULL, seps);
        i++;
    }
}

void CDlgOscilloscope::GetTokensSample (char *p, char* szSampleName, int* nTipoDev, int* nDatoAsse, int* nIndiceDiScala, double* fpControlMin, double* fpControlSpan)
{
    int i=0;
    char* tok=strtok (p, seps);
    while (tok!=NULL)   {
        strdblk (tok);
        switch (i)   {
            case 0 :
                strcpy (szSampleName, strdblk (tok));
                break;
            case 1 :
                *nTipoDev=atoi (tok);
                break;
            case 2 :
                *nDatoAsse=atoi (tok);
                break;
            case 3 :
                *nIndiceDiScala=atoi (tok);
                break;
            case 4 :
                *fpControlMin=atof (tok);
                break;
            case 5 :
                *fpControlSpan=atof (tok);
                break;
        }
        tok=strtok (NULL, seps);
        i++;
    }
}

void CDlgOscilloscope::TraceFromAxSettings (CString strDevice)
{
    bool bGoOn=true;
    /* se ci sono gia delle tracce, chiadiamoci cosa vogliamo fare */
    if (m_pDlgSamples->GetGridCount ()||m_pDlgTriggers->GetGridCount ())   {
        if (AfxMessageBox (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_25), MB_YESNO)==IDNO)
            bGoOn=false;
    }
    if (bGoOn)   {
        OnOscClear ();
        /* cancelliamo tutti i dati nel grid relativi ai trigger*/
        m_pDlgTriggers->DeleteAllRows ();
        /* cancelliamo tutti i dati nel grid relativi ai samples*/
        m_pDlgSamples->DeleteAllRows ();
        /* carichiamo i dati che vogliamo inserire */
        m_pDlgSamples->AddFromFile (strDevice, enAsse, 0);
        m_pDlgSamples->AddFromFile (strDevice, enAsse, 5);
        m_pDlgSamples->AddFromFile (strDevice, enAsse, 6);
        /* inizializziamo l'osciloscopio */
        InitOscilloscope ();
    }
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CDlgOscilloscope::InitStrings ()
{
    int i;

    for (i=0; i<4; i++)   {
        stStatusOscilloscope[i]=new CString;
        *stStatusOscilloscope[i]=LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_1+i);
    }
    for (i=0; i<MAXAXESDATA; i++)   {
        aszTipo[i]=new CString;
        *aszTipo[i]=LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_5+i);
    }
}

/*
** DestroyStrings : distrugge le stringhe della dialog
*/
void CDlgOscilloscope::DestroyStrings ()
{
    int i;

    for (i=0; i<4; i++)
        _delete (stStatusOscilloscope[i]);
    for (i=0; i<MAXAXESDATA; i++)
        _delete (aszTipo[i]);
}

/*
** Evitiamo di uscire come degli stolti
*/
void CDlgOscilloscope::OnOK ()
{
}

void CDlgOscilloscope::OnCancel ()
{
}

HBRUSH CDlgOscilloscope::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	if (nCtlColor==CTLCOLOR_DLG)
		hbr = (HBRUSH) GetStockObject (GRAY_BRUSH);
	else 
		if (nCtlColor==CTLCOLOR_STATIC)   {
			hbr = (HBRUSH) GetStockObject (GRAY_BRUSH);
			pDC->SetBkColor (RGB (128, 128, 128));
			pDC->SetTextColor (RGB (255, 255, 255));
		}
	return hbr;
}

/*
** InitKnobs :
*/
void CDlgOscilloscope::InitKnobs ()
{
	m_knobXAxis.SetBackGroundColor (RGB (128, 128, 128));
	m_knobYAxis.SetBackGroundColor (RGB (128, 128, 128));
	m_knobXAxis.SetShowTickLabels (FALSE);
	m_knobYAxis.SetShowTickLabels (FALSE);
	m_knobXAxis.SetTickMajorCount (7);
	m_knobYAxis.SetTickMajorCount (25);
	m_knobXAxis.SetTickMajorColor (RGB (192, 192, 192));
	m_knobYAxis.SetTickMajorColor (RGB (192, 192, 192));
	m_knobXAxis.SetShowTicksMajor (true);
	m_knobXAxis.SetShowTicksMinor (false);
	m_knobYAxis.SetShowTicksMajor (true);
	m_knobYAxis.SetShowTicksMinor (false);
	m_knobXAxis.SetTickMargin (1);
	m_knobXAxis.SetTickMajorLength (2);
	m_knobYAxis.SetTickMargin (1);
	m_knobYAxis.SetTickMajorLength (2);
	m_knobXAxis.SetPosition (m_nIndexXScale*16.66);
	m_knobYAxis.SetPosition (0);
	m_knobYAxis.MoveWindow (24, 24, 72, 72);
	m_knobXAxis.MoveWindow (24, 24+98+40, 72, 72);
}

/*
** InitLabels :
*/
void CDlgOscilloscope::InitLabels ()
{
	m_pFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Arial", 16, FW_BOLD);
	GetDlgItem (IDC_YCAP)->SetWindowText (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_36));
	GetDlgItem (IDC_XCAP)->SetWindowText (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_35));
	GetDlgItem (IDC_YCAP)->MoveWindow (24, 6, 100, 18);
	GetDlgItem (IDC_XCAP)->MoveWindow (24, 6+98+40, 100, 18);
	GetDlgItem (IDC_YVALUE)->MoveWindow (20, 24+72, 40, 18);
	GetDlgItem (IDC_YUM)->MoveWindow (20+40, 24+72, 70, 18);
	GetDlgItem (IDC_XVALUE)->MoveWindow (20, 8+98+40+88, 40, 18);
	GetDlgItem (IDC_XUM)->MoveWindow (20+40, 8+98+40+88, 70, 18);
	GetDlgItem (IDC_XCAP)->SetFont (m_pFont);
	GetDlgItem (IDC_YCAP)->SetFont (m_pFont);
	GetDlgItem (IDC_XVALUE)->SetFont (m_pFont);
	GetDlgItem (IDC_XUM)->SetFont (m_pFont);
	GetDlgItem (IDC_YVALUE)->SetFont (m_pFont);
	GetDlgItem (IDC_YUM)->SetFont (m_pFont);
	GetDlgItem (IDC_OSCSTATUS)->SetFont (m_pFont);
	GetDlgItem (IDC_YVALUE)->SetWindowText ("");
	GetDlgItem (IDC_YUM)->SetWindowText ("");
	GetDlgItem (IDC_XVALUE)->SetWindowText ("");
	GetDlgItem (IDC_XUM)->SetWindowText ("");
	SetXLabel ();
}

/*
** OnPositionChangeIknobX :
*/
void CDlgOscilloscope::OnPositionChangeIknobX() 
{
	double fpPosition = m_knobXAxis.GetPosition ();
}

/*
** OnMouseUpIknobX :
*/
void CDlgOscilloscope::OnMouseUpIknobX(short Button, short Shift, long X, long Y) 
{
	double fpPosition = m_knobXAxis.GetPosition ();
}

/*
** OnPositionChangeFinishedIknobX :
*/
void CDlgOscilloscope::OnPositionChangeFinishedIknobX()
{
	// TODO: Add your message handler code here
	double fpPosition = m_knobXAxis.GetPosition ();
	double fpTarget = floor (fpPosition / 16.66);
	double fpResto = fpPosition - (fpTarget * 16.66);
	if (fpResto > (16.66 / 2.0))
		fpTarget+=1.0;
	m_knobXAxis.SetPosition (fpTarget*16.66);
    m_nIndexXScale = (int) fpTarget;
	SetXLabel ();
	AdjustXAxes ();
}

/*
** OnPositionChangeIknobY :
*/
void CDlgOscilloscope::OnPositionChangeIknobY() 
{
	double fpPosition = m_knobYAxis.GetPosition ();
}

/*
** OnMouseUpIknobY :
*/
void CDlgOscilloscope::OnMouseUpIknobY(short Button, short Shift, long X, long Y) 
{
	double fpPosition = m_knobYAxis.GetPosition ();
}

/*
** OnPositionChangeFinishedIknobY :
*/
void CDlgOscilloscope::OnPositionChangeFinishedIknobY()
{
	// TODO: Add your message handler code here
	if (m_nCurSample != -1)   {
		double fpPosition = m_knobYAxis.GetPosition ();
		double fpTarget = floor (fpPosition / 4.16);
		double fpResto = fpPosition - (fpTarget * 4.16);
		if (fpResto > (4.16 / 2.0))
			fpTarget+=1.0;
		m_knobYAxis.SetPosition (fpTarget*4.16);
		m_OscCanali[m_nCurSample].nScale = (int) fpTarget;
		AdjustYAxes (m_nCurSample);
		SetYLabel ();
	}
}

/*
** InitButtons :
*/
void CDlgOscilloscope::InitButtons ()
{
	m_btnYPlus.SetSkin(IDB_OSC_PU,IDB_OSC_PD,IDB_OSC_PU,IDB_OSC_PU,0,IDB_OSC_MASK,0,0,0);
	m_btnYMinus.SetSkin(IDB_OSC_MU,IDB_OSC_MD,IDB_OSC_MU,IDB_OSC_MU,0,IDB_OSC_MASK,0,0,0);
	m_btnYPlus.MoveWindow (108, 32, 22, 22);
	m_btnYMinus.MoveWindow (108, 64, 22, 22);
	m_btnXPlus.SetSkin(IDB_OSC_PU,IDB_OSC_PD,IDB_OSC_PU,IDB_OSC_PU,0,IDB_OSC_MASK,0,0,0);
	m_btnXMinus.SetSkin(IDB_OSC_MU,IDB_OSC_MD,IDB_OSC_MU,IDB_OSC_MU,0,IDB_OSC_MASK,0,0,0);
	m_btnXPlus.MoveWindow (108, 170, 22, 22);
	m_btnXMinus.MoveWindow (108, 202, 22, 22);
}

/*
** SetXLabel :
*/
void CDlgOscilloscope::SetXLabel ()
{
	GetDlgItem (IDC_XVALUE)->SetWindowText (ScaleX[m_nIndexXScale].pStr);
	GetDlgItem (IDC_XUM)->SetWindowText (LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_37));
}

/*
** SetXLabel :
*/
void CDlgOscilloscope::SetYLabel ()
{
	if (m_nCurSample != -1)   {
		int nYIndex = m_OscCanali[m_nCurSample].nScale;
		GetDlgItem (IDC_YVALUE)->SetWindowText (ScaleY[nYIndex].pStr);
	}
	else
		GetDlgItem (IDC_YVALUE)->SetWindowText ("");
}

/*
** SetXLabel :
*/
CString CDlgOscilloscope::GetUnitString (enDispositivi nDevice, int nType, enUnitaMisura nUnit)
{
	CString strUnit;

    if (nDevice == enAsse)   {
        switch (nType)   {
            case AX_QENC :
            case AX_QR :
            case AX_QT :
            case AX_EA :
				strUnit = GetUnitSpace (nUnit);
				break;
            case AX_TORQUE :
				strUnit = GetUnitTorque (nUnit);
                break;
            case AX_VC :
            case AX_VCR :
				strUnit = GetUnitSpeed (nUnit);
                break;
            case AX_ACCINST :
            case AX_ACCINSTR :
				strUnit = GetUnitAcc (nUnit);
                break;
            case AX_JERKINST :
            case AX_JERKINSTR :
            case AX_FF :
            case AX_FFA :
            case AX_P :
            case AX_I :
            case AX_D :
            case AX_PG:
            case AX_IG:
            case AX_DG:
            case AX_ADJUST :
			case AX_DAC :
                break;
        }
    }
    else   {
        switch (nDevice)   {
            case enInputDigitale :
            case enOutputDigitale :
                break;
            case enInputAnalogico :
            case enOutputAnalogico :
				strUnit = GetUnitVolt (nUnit);
				break;
            case enTimer :
				strUnit = GetUnitTimer (nUnit);
				break;
            case enCounter :
            case enGenericCan :
                break;
		}
	}
	return strUnit;
}

/*
** GetUnitSpace :
*/
CString CDlgOscilloscope::GetUnitSpace (enUnitaMisura nUnit)
{
	CString strUnit;

	switch (nUnit)   {
		case enUMMmin :
		case enUMMMmin :
			strUnit = LOADSTRING (IDS_ADAMOGRIDVIEW_77);
			break;
		case enUMinch :
			strUnit = LOADSTRING (IDS_ADAMOGRIDVIEW_78);
			break;
		case enUMgiri :
			strUnit = LOADSTRING (IDS_ADAMOGRIDVIEW_79);
			break;
		case enUMgradisec :
		case enUMgradimin :
			strUnit = LOADSTRING (IDS_ADAMOGRIDVIEW_80);
			break;
	}
	return strUnit;
}

/*
** GetUnitSpace :
*/
CString CDlgOscilloscope::GetUnitTorque (enUnitaMisura nUnit)
{
	return LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_38);
}

/*
** GetUnitSpeed :
*/
CString CDlgOscilloscope::GetUnitSpeed (enUnitaMisura nUnit)
{
	CString strUnit;

	switch (nUnit)   {
		case enUMMmin :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_39);
			break;
		case enUMinch :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_40);
			break;
		case enUMgiri :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_41);
			break;
		case enUMgradisec :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_43);
			break;
		case enUMMMmin :
			strUnit = LOADSTRING (IDS_ADAMOGRIDVIEW_178);
			break;
		case enUMgradimin :
			strUnit = LOADSTRING (IDS_ADAMOGRIDVIEW_179);
			break;
	}
	return strUnit;
}

/*
** GetUnitAcc :
*/
CString CDlgOscilloscope::GetUnitAcc (enUnitaMisura nUnit)
{
	CString strUnit;

	switch (nUnit)   {
		case enUMMmin :
		case enUMMMmin :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_44);
			break;
		case enUMinch :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_45);
			break;
		case enUMgiri :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_46);
			break;
		case enUMgradisec :
		case enUMgradimin :
			strUnit = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_47);
			break;
	}
	return strUnit;
}

/*
** GetUnitVolt :
*/
CString CDlgOscilloscope::GetUnitVolt (enUnitaMisura nUnit)
{
	return LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_48);
}

/*
** GetUnitTimer :
*/
CString CDlgOscilloscope::GetUnitTimer (enUnitaMisura nUnit)
{
	return LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_49);
}

/*
** SetUnitLabel :
*/
void CDlgOscilloscope::SetUnitLabel (CAdamoBase* ab, CString strType)
{
	CString strUnit;
	int nUnit = 0;
	int nDatoAsse = -1, n;

	/* per prima cosa se e' un asse cerchiamo il sottotipo */
	if (ab->GetType () == enAsse)   {
		nUnit = ((CAdamoAxis *)ab)->GetUnit ();
        for (n=0; n<sizeof (aszTipoSamples)/sizeof (CString*); n++)   {
            if (strType == *aszTipoSamples[n])   {
                nDatoAsse=anCodiciTipo[n];
                break;
            }
        }
	}
	/* leggiamo la stringa di unita di misura */
	strUnit = GetUnitString (ab->GetType (), nDatoAsse, (enUnitaMisura)nUnit);
	/* settiamola */
	GetDlgItem (IDC_YUM)->SetWindowText (strUnit);
}

/*
** ZoomToFit :
*/
void CDlgOscilloscope::ZoomToFit () 
{
    if (m_nCurSample != -1)   {
		m_iPlotX1iDispatch->GetYAxis (m_nCurSample)->ZoomToFit ();
    }
}

/*
** ShowDeltaXCursor :
*/
void CDlgOscilloscope::ShowDeltaXCursor () 
{
	char szBuffer[256];
	if (m_nCursorDeltaXIndex == -1)   {
		m_nCursorDeltaXIndex=m_iPlotX1iDispatch->AddDataCursor ();
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->put_Visible (TRUE);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->PutStyle (iPlotLibrary::ipcsDeltaX);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->PutHintShow (TRUE);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->PutColor ((OLE_COLOR) RGB (0, 128, 192));
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->PutPointerPenWidth (2);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->PutUseChannelColor (FALSE);
		sprintf (szBuffer, (const char *) LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_27), m_nCurSample);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaXIndex)->put_ChannelName (_bstr_t (szBuffer));
	}
	else   {
        m_iPlotX1iDispatch->DeleteDataCursor (m_nCursorDeltaXIndex);
		m_nCursorDeltaXIndex = -1;
	}
}

/*
** ShowDeltaYCursor :
*/
void CDlgOscilloscope::ShowDeltaYCursor () 
{
	char szBuffer[256];
	if (m_nCursorDeltaYIndex == -1)   {
		m_nCursorDeltaYIndex=m_iPlotX1iDispatch->AddDataCursor ();
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->put_Visible (TRUE);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->PutStyle (iPlotLibrary::ipcsDeltaY);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->PutHintShow (TRUE);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->PutColor ((OLE_COLOR) RGB (0, 255, 255));
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->PutPointerPenWidth (2);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->PutUseChannelColor (FALSE);
		sprintf (szBuffer, (const char *) LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_27), m_nCurSample);
		m_iPlotX1iDispatch->GetDataCursor (m_nCursorDeltaYIndex)->put_ChannelName (_bstr_t (szBuffer));
	}
	else   {
        m_iPlotX1iDispatch->DeleteDataCursor (m_nCursorDeltaYIndex);
		m_nCursorDeltaYIndex = -1;
	}
}
