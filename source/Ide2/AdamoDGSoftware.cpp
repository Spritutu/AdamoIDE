// AdamoDGSoftware.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "AdamoDGSoftware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSoftware dialog


CAdamoDGSoftware::CAdamoDGSoftware(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDGSoftware::IDD, pParent), m_pMachine (NULL)
{
	//{{AFX_DATA_INIT(CAdamoDGSoftware)
	//}}AFX_DATA_INIT
}


void CAdamoDGSoftware::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoDGSoftware)
	DDX_Control(pDX, IDC_ST_EC_DC_CORRECTIONS, m_stDCCorrections);
	DDX_Control(pDX, IDC_ST_EC_SENT, m_stDynPacketSend);
	DDX_Control(pDX, IDC_ST_EC_RECEIVED, m_stDynPacketsReceived);
	DDX_Control(pDX, IDC_ST_EC_DIFFERENCE, m_stDynPacketsDifference);
	DDX_Control(pDX, IDC_ST_TIMERS_MAX_INTERVAL, m_stMaxTimersPeriod);
	DDX_Control(pDX, IDC_ST_TIMERS_INTERVAL, m_stTimersPeriod);
	DDX_Control(pDX, IDC_ST_TIMERS_AVGBUSY, m_stAvgTimerBusy);
	DDX_Control(pDX, IDC_ST_TIMERS_MAXBUSY, m_stMaxTimerBusy);
	DDX_Control(pDX, IDC_ST_TIMERS_BUSY, m_stTimersBusy);
	DDX_Control(pDX, IDC_ST_TIMERS_AVGCAN, m_stAvgTimerCan);
	DDX_Control(pDX, IDC_ST_TIMERS_MAXCAN, m_stMaxTimerCan);
	DDX_Control(pDX, IDC_ST_TIMERS_CAN, m_stTimersCan);
	DDX_Control(pDX, IDC_ST_OBJ_SDO, m_stObjSDO);
	DDX_Control(pDX, IDC_ST_OBJ_MS, m_stObjMS);
	DDX_Control(pDX, IDC_ST_OBJ_LOOP, m_stObjLoop);
	DDX_Control(pDX, IDC_ST_OBJ_EVENTI, m_stObjEventi);
	DDX_Control(pDX, IDC_ST_OBJ_ASSI, m_stObjAssi);
	DDX_Control(pDX, IDC_ST_STATIC_CHUNKS_USATI, m_stStaticChunksUsati);
	DDX_Control(pDX, IDC_ST_STATIC_CHUNKS_LIBERI, m_stStaticChunksLiberi);
	DDX_Control(pDX, IDC_ST_STATIC_BYTES_USATI, m_stStaticBytesUsati);
	DDX_Control(pDX, IDC_ST_DYN_BYTES_USATI, m_stDynBytesUsati);
	DDX_Control(pDX, IDC_ST_STATIC_BYTES_LIBERI, m_stStaticBytesLiberi);
	DDX_Control(pDX, IDC_ST_STATIC_BYTE_TOTALI, m_stStaticByteTotali);
	DDX_Control(pDX, IDC_ST_DYN_CHUNKS_USATI, m_stDynCunksUsati);
	DDX_Control(pDX, IDC_ST_DYN_CHUNKS_LIBERI, m_stDynChunksLiberi);
	DDX_Control(pDX, IDC_ST_DYN_BYTES_LIBERI, m_stDynBytesLiberi);
	DDX_Control(pDX, IDC_ST_DYN_BYTE_TOTALI, m_stDynByteTotali);
	DDX_Control(pDX, IDC_ST_STATIC_HEAP, m_stStaticHeapSize);
	DDX_Control(pDX, IDC_ST_DYNAMIC_HEAP, m_stDynamicHeapSize);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_8, m_btnDebugPar8);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_7, m_btnDebugPar7);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_6, m_btnDebugPar6);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_4, m_btnDebugPar4);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_5, m_btnDebugPar5);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_3, m_btnDebugPar3);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_2, m_btnDebugPar2);
	DDX_Control(pDX, IDC_BTN_DEBUG_WRITE_1, m_btnDebugPar1);
	DDX_Control(pDX, IDC_ILEDRECTANGLE_CANOPEN, m_ledCanOpen);
	DDX_Control(pDX, IDC_ILEDRECTANGLE_MATCHABSPOS, m_ledMatchAbsPos);
	DDX_Control(pDX, IDC_ILEDRECTANGLE_RESTOREABSPOS, m_ledRestoreAbsPos);
    DDX_Control(pDX, IDC_ILEDRECTANGLE_CONFIGURATION, m_ledConfigurazione);
	DDX_Control(pDX, IDC_ILEDRECTANGLE_ETHERCAT, m_ledEthercat);
	DDX_Control(pDX, IDC_ILEDRECTANGLE_DCETHERCAT, m_ledDCEthercat);
	DDX_Control(pDX, IDC_ILEDRECTANGLE_PROTECTION_PLC, m_ledProtectionPlc);
	DDX_Control(pDX, IDC_ST_CANOPEN, m_stCanOpen);
	DDX_Control(pDX, IDC_ST_MATCHABS, m_stMatchAbsPos);
	DDX_Control(pDX, IDC_ST_RESTOREABS, m_stRestoreAbsPos);
	DDX_Control(pDX, IDC_ST_CONFIGURAZIONE, m_stConfigurazione);
	DDX_Control(pDX, IDC_ST_ETHERCAT, m_stEthercat);
	DDX_Control(pDX, IDC_ST_DCETHERCAT, m_stDCEthercat);
	DDX_Control(pDX, IDC_ST_PROTECTION_PLC, m_stProtectionPlc);
	DDX_Control(pDX, IDC_BTN_RESET_TIMER, m_btnResetTimer);
	DDX_Control(pDX, IDC_BTN_RESET_BUSY, m_btnResetBusy);
	DDX_Control(pDX, IDC_BTN_RESET_CAN, m_btnResetCan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoDGSoftware, CDialog)
	//{{AFX_MSG_MAP(CAdamoDGSoftware)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_1, OnBtnDebugWrite1)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_2, OnBtnDebugWrite2)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_3, OnBtnDebugWrite3)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_4, OnBtnDebugWrite4)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_5, OnBtnDebugWrite5)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_6, OnBtnDebugWrite6)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_7, OnBtnDebugWrite7)
	ON_BN_CLICKED(IDC_BTN_DEBUG_WRITE_8, OnBtnDebugWrite8)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_RESET_TIMER, OnBtnResetTimer)
	ON_BN_CLICKED(IDC_BTN_RESET_BUSY, OnBtnResetBusy)
	ON_BN_CLICKED(IDC_BTN_RESET_CAN, OnBtnResetCan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSoftware message handlers
/*
** SetMachine :
*/
void CAdamoDGSoftware::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

void CAdamoDGSoftware::RinfrescaInformazioni ()
{
    CAdamoDebugInfo di;
	COleVariant v, vEl;
    CString str;
	long lDim, lCorrectionsFrequency;
	int nInitTest, nInitStatus;

    if (m_pMachine)   {
        if (m_pMachine->GetDebugInfo (&di) == S_FALSE)
            memset (&di, 0, sizeof (CAdamoDebugInfo));
        str.Format ("%d", di.memory.m_nGlobalHeap);
        m_stDynamicHeapSize.SetWindowText (str);
        str.Format ("%d", di.memory.m_nStaticHeap);
        m_stStaticHeapSize.SetWindowText (str);
        str.Format ("%d", di.memory.m_nTotalBytes);
        m_stDynByteTotali.SetWindowText (str);
        str.Format ("%d", di.memory.m_nUsedChunks);
	    m_stDynCunksUsati.SetWindowText (str);
        str.Format ("%d", di.memory.m_nFreeChunks);
	    m_stDynChunksLiberi.SetWindowText (str);
        str.Format ("%d", di.memory.m_nUsedBytes);
	    m_stDynBytesUsati.SetWindowText (str);
        str.Format ("%d", di.memory.m_nFreeBytes);
	    m_stDynBytesLiberi.SetWindowText (str);

        str.Format ("%d", di.memory.m_nStaticUsedChunks);
        m_stStaticChunksUsati.SetWindowText (str);
        str.Format ("%d", di.memory.m_nStaticFreeChunks);
	    m_stStaticChunksLiberi.SetWindowText (str);
        str.Format ("%d", di.memory.m_nStaticUsedBytes);
	    m_stStaticBytesUsati.SetWindowText (str);
        str.Format ("%d", di.memory.m_nStaticFreeBytes);
	    m_stStaticBytesLiberi.SetWindowText (str);
        str.Format ("%d", di.memory.m_nStaticTotalBytes);
	    m_stStaticByteTotali.SetWindowText (str);

        str.Format ("%d", di.m_nSDO);
	    m_stObjSDO.SetWindowText (str);
        str.Format ("%d", di.m_nMachineState);
	    m_stObjMS.SetWindowText (str);
        str.Format ("%d", di.m_AxisLoop);
	    m_stObjLoop.SetWindowText (str);
        str.Format ("%d", di.m_nEvents);
	    m_stObjEventi.SetWindowText (str);
        str.Format ("%d", di.m_AxisList);
	    m_stObjAssi.SetWindowText (str);

        str.Format ("%d", di.timers.m_dwPeriodTime);
	    m_stTimersPeriod.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwMaxPeriodTime);
	    m_stMaxTimersPeriod.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwBusyTime);
	    m_stTimersBusy.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwMaxBusyTime);
	    m_stMaxTimerBusy.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwAvgBusyTime);
	    m_stAvgTimerBusy.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwCanTime);
	    m_stTimersCan.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwMaxCanTime);
	    m_stMaxTimerCan.SetWindowText (str);
        str.Format ("%d", di.timers.m_dwAvgCanTime);
	    m_stAvgTimerCan.SetWindowText (str);

        str.Format ("%d", di.packets.m_nPacketDelivered);
	    m_stDynPacketSend.SetWindowText (str);
        str.Format ("%d", di.packets.m_nPacketReceived);
	    m_stDynPacketsReceived.SetWindowText (str);
        str.Format ("%d", di.packets.m_nPacketDelivered-di.packets.m_nPacketReceived);
	    m_stDynPacketsDifference.SetWindowText (str);

		if (m_pMachine->GetInitializationStatus (&nInitTest, &nInitStatus) == S_OK)
		    ManageInitialization (nInitTest, nInitStatus);

        lCorrectionsFrequency = 0;    
        if (m_pMachine->GetEcInfo (&v) == S_OK)   {
		    if (v.vt!=VT_NULL)   {
			    COleSafeArray sa (*v);
			    lDim=1;
			    sa.GetElement (&lDim, &vEl);
			    lCorrectionsFrequency = vEl.lVal;
		    }
        }
		str.Format ("%d", lCorrectionsFrequency);
		m_stDCCorrections.SetWindowText (str);
	}
}

void CAdamoDGSoftware::OnBtnDebugWrite1() 
{
	time_t dt;
	// TODO: Add your control notification handler code here
	stThiraErrore* te = new stThiraErrore;
	te->nTypeError = TE_INTERPRETER;
	strcpy (te->strErrore, "c:\\Sviluppo.ada@100@Hello World !!!");
	strcpy (te->szTaskName, "Task Motta");
	te->m_strModule = "TestSviluppo";
	te->nDeleteError = 0;
	te->nErrore=1000;
	te->nTask = 10;
	te->nDevice=2;
	te->nTypeDevice = 2;
	localtime (&dt);
	te->dt = dt;
	((CMainFrame* )AfxGetMainWnd ())->SendMessage (WM_THEREISANERROR, 0, (LPARAM)te);
    if (m_pMachine)
        m_pMachine->SetDebugInfo (0);
}

void CAdamoDGSoftware::OnBtnDebugWrite2() 
{
	// TODO: Add your control notification handler code here
	time_t dt;
	stThiraErrore* te = new stThiraErrore;
	te->nTypeError = TE_HARDWARE;
	strcpy (te->strErrore, "c:\\Sviluppo.ada@100@Hello World !!!");
	strcpy (te->szTaskName, "Task Motta");
	te->m_strModule = "TestSviluppo";
	te->nDeleteError = 0;
	te->nErrore=1000;
	te->nTask = 10;
	te->nDevice=2;
	te->nTypeDevice = 2;
	localtime (&dt);
	te->dt = dt;
	((CMainFrame* )AfxGetMainWnd ())->SendMessage (WM_THEREISANERROR, 0, (LPARAM)te);
    if (m_pMachine)
        m_pMachine->SetDebugInfo (1);
}

void CAdamoDGSoftware::OnBtnDebugWrite3() 
{
	stThiraStringElement* se = new stThiraStringElement;
	time_t dt;
	localtime (&dt);
	se->m_strModule = "TestSviluppo";
	strcpy (se->str, "azz.. la funsiuna !!!");
	se->nPriority = 1;
	se->dt = dt;
	((CMainFrame* )AfxGetMainWnd ())->SendMessage (WM_ADAMO_STRING, 0, (LPARAM)se);
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (2);
}

void CAdamoDGSoftware::OnBtnDebugWrite4() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (3);
}

void CAdamoDGSoftware::OnBtnDebugWrite5() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (4);
}

void CAdamoDGSoftware::OnBtnDebugWrite6() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (5);
}

void CAdamoDGSoftware::OnBtnDebugWrite7() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (6);
}

void CAdamoDGSoftware::OnBtnDebugWrite8() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (7);
}

int CAdamoDGSoftware::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetMachine ();
	return 0;
}

HBRUSH CAdamoDGSoftware::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
    if (nCtlColor==CTLCOLOR_DLG)
        hbr=m_brWhite;
    if (nCtlColor==CTLCOLOR_STATIC)    {
        pDC->SetBkColor (GetSysColor (COLOR_WINDOW));
        hbr=m_brWhite;
    }
	return hbr;
}

BOOL CAdamoDGSoftware::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brWhite.CreateSolidBrush (GetSysColor (COLOR_WINDOW));
	/* gestiamo le password */
	GestionePsw ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoDGSoftware::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    m_brWhite.DeleteObject ();	
}

void CAdamoDGSoftware::OnBtnResetTimer() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (128);
}

void CAdamoDGSoftware::OnBtnResetBusy() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (129);
}

void CAdamoDGSoftware::OnBtnResetCan() 
{
	// TODO: Add your control notification handler code here
    if (m_pMachine)
        m_pMachine->SetDebugInfo (130);
}

void CAdamoDGSoftware::ManageInitialization (int nInitTest, int nInitStatus)
{
	if (nInitTest & INITSTATUS_CANOPEN)   {
		m_ledCanOpen.EnableWindow (TRUE);
		m_stCanOpen.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_CANOPEN)
			m_ledCanOpen.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledCanOpen.EnableWindow (FALSE);
		m_stCanOpen.EnableWindow (FALSE);
	}
	if (nInitTest & INITSTATUS_MATCH_ABS)   {
		m_ledMatchAbsPos.EnableWindow (TRUE);
		m_stMatchAbsPos.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_MATCH_ABS)
			m_ledMatchAbsPos.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledMatchAbsPos.EnableWindow (FALSE);
		m_stMatchAbsPos.EnableWindow (FALSE);
	}
	if (nInitTest & INITSTATUS_RESTORE_ABS)   {
		m_ledRestoreAbsPos.EnableWindow (TRUE);
		m_stRestoreAbsPos.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_RESTORE_ABS)
			m_ledRestoreAbsPos.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledRestoreAbsPos.EnableWindow (FALSE);
		m_stRestoreAbsPos.EnableWindow (FALSE);
	}
	if (nInitTest & INITSTATUS_CONFIGURATION)   {
		m_ledConfigurazione.EnableWindow (TRUE);
		m_stConfigurazione.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_CONFIGURATION)
			m_ledConfigurazione.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledConfigurazione.EnableWindow (FALSE);
		m_stConfigurazione.EnableWindow (FALSE);
	}
	if (nInitTest & INITSTATUS_ETHERCAT)   {
		m_ledEthercat.EnableWindow (TRUE);
		m_stEthercat.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_ETHERCAT)
			m_ledEthercat.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledEthercat.EnableWindow (FALSE);
		m_stEthercat.EnableWindow (FALSE);
	}
	if (nInitTest & INITSTATUS_DCETHERCAT)   {
		m_ledDCEthercat.EnableWindow (TRUE);
		m_stDCEthercat.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_DCETHERCAT)
			m_ledDCEthercat.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledDCEthercat.EnableWindow (FALSE);
		m_stDCEthercat.EnableWindow (FALSE);
	}
	if (nInitTest & INITSTATUS_PROTECTION)   {
		m_ledProtectionPlc.EnableWindow (TRUE);
		m_stProtectionPlc.EnableWindow (TRUE);
		if (nInitStatus & INITSTATUS_PROTECTION)
			m_ledProtectionPlc.put_Active (TRUE);		
		else
			;
	}
	else   {
		m_ledProtectionPlc.EnableWindow (FALSE);
		m_stProtectionPlc.EnableWindow (FALSE);
	}
}

void CAdamoDGSoftware::GestionePsw ()
{
	bool bEnable = false;

    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	if (m_ePswLevel < ePswService)
		bEnable = true;
	m_btnDebugPar1.EnableWindow (bEnable);
	m_btnDebugPar2.EnableWindow (bEnable);
	m_btnDebugPar3.EnableWindow (bEnable);
	m_btnDebugPar4.EnableWindow (bEnable);
	m_btnDebugPar5.EnableWindow (bEnable);
	m_btnDebugPar6.EnableWindow (bEnable);
	m_btnDebugPar7.EnableWindow (bEnable);
	m_btnDebugPar8.EnableWindow (bEnable);
	m_btnResetTimer.EnableWindow (bEnable);
	m_btnResetBusy.EnableWindow (bEnable);
}

/*
** OnChangePsw :
*/
void CAdamoDGSoftware::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	GestionePsw ();
}
