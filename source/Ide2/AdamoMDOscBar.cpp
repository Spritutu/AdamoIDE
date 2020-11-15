// AdamoMDOscBar.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDOscBar dialog


CAdamoMDOscBar::CAdamoMDOscBar(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoMDOscBar::IDD, pParent), m_pOsc (NULL)
{
	//{{AFX_DATA_INIT(CAdamoMDOscBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoMDOscBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoMDOscBar)
	DDX_Control(pDX, IDC_STATUSOSCILLOSCOPE, m_ctrlOscStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoMDOscBar, CDialog)
	//{{AFX_MSG_MAP(CAdamoMDOscBar)
	ON_BN_CLICKED(IDC_OSCSTART, OnOscStart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OSCSTARTPRETRIGGER, OnOscStartPretrigger)
	ON_BN_CLICKED(IDC_OSCSTOP, OnOscStop)
	ON_BN_CLICKED(IDC_OSCCLEAR, OnOscClear)
	ON_BN_CLICKED(IDC_OSCOPTIONS, OnOscOptions)
	ON_BN_CLICKED(IDC_OSCCURSOR, OnOscCursor)
	ON_BN_CLICKED(IDC_OSCLOAD, OnOscLoad)
	ON_BN_CLICKED(IDC_OSCSAVE, OnOscSave)
	ON_BN_CLICKED(IDC_OSCPRINT, OnOscPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDOscBar message handlers

BOOL CAdamoMDOscBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_pOsc=CDlgOscilloscope::GetOscilloscope ();
    m_pOsc->SettaCmdOscBar (this);

	m_ctrlOscStart.AutoLoad (IDC_OSCSTART, this);
	m_ctrlOscStartPrTrigger.AutoLoad (IDC_OSCSTARTPRETRIGGER, this);
	m_ctrlOscStop.AutoLoad (IDC_OSCSTOP, this);
	m_ctrlOscClear.AutoLoad (IDC_OSCCLEAR, this);
	m_ctrlOscOptions.AutoLoad (IDC_OSCOPTIONS, this);
	m_ctrlOscCursor.AutoLoad (IDC_OSCCURSOR, this);
	m_ctrlOscLoad.AutoLoad (IDC_OSCLOAD, this);
	m_ctrlOscSave.AutoLoad (IDC_OSCSAVE, this);
	m_ctrlOscPrint.AutoLoad (IDC_OSCPRINT, this);

    m_pOscButtons[0]=&m_ctrlOscStart;
    m_pOscButtons[1]=&m_ctrlOscStartPrTrigger;
    m_pOscButtons[2]=&m_ctrlOscStop;
    m_pOscButtons[3]=&m_ctrlOscClear;
    m_pOscButtons[4]=&m_ctrlOscOptions;
    m_pOscButtons[5]=&m_ctrlOscCursor;
    m_pOscButtons[6]=&m_ctrlOscLoad;
    m_pOscButtons[7]=&m_ctrlOscSave;
    m_pOscButtons[8]=&m_ctrlOscPrint;
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoMDOscBar::OnOscStart() 
{
	// TODO: Add your control notification handler code here
	m_pOsc->OnOscStart ();
}

void CAdamoMDOscBar::OnDestroy() 
{
	CDialog::OnDestroy();
	
}

void CAdamoMDOscBar::OnOscStartPretrigger() 
{
	// TODO: Add your control notification handler code here
	m_pOsc->OnOscStartPretrigger ();
}

void CAdamoMDOscBar::OnOscStop() 
{
	// TODO: Add your control notification handler code here
	m_pOsc->OnOscStop ();
}

void CAdamoMDOscBar::OnOscClear() 
{
	// TODO: Add your control notification handler code here
	m_pOsc->OnOscClear ();
}

void CAdamoMDOscBar::OnOscOptions() 
{
	// TODO: Add your control notification handler code here
    m_pOsc->OnOscOptions ();
}

void CAdamoMDOscBar::OnOscCursor() 
{
	// TODO: Add your control notification handler code here
	m_pOsc->OnOscCursor ();
}

void CAdamoMDOscBar::OnOscLoad() 
{
	// TODO: Add your control notification handler code here
	
}

void CAdamoMDOscBar::OnOscSave() 
{
	// TODO: Add your control notification handler code here
    m_pOsc->OnOscSave ();	
}

void CAdamoMDOscBar::OnOscPrint() 
{
	// TODO: Add your control notification handler code here
	
}

void CAdamoMDOscBar::EnableButtons (DWORD dwEnable)
{
	// TODO: Add your control notification handler code here
	for (int i=0; i<NMAXOSCBARBUTTONS; i++)   {
        if (dwEnable&(1<<i))
            m_pOscButtons[i]->EnableWindow ();
        else
            m_pOscButtons[i]->EnableWindow (FALSE);
    }
}

void CAdamoMDOscBar::OnOscStarted ()
{
    EnableButtons (0x000c);
}

void CAdamoMDOscBar::OnOscStopped()
{
    EnableButtons (0xfffb);
}

void CAdamoMDOscBar::OnOscStatus (const char* s)
{
    static char strOsc[32] = { '\0' };

    if (strcmp (s, strOsc))   {
        m_ctrlOscStatus.SetWindowText (s);
        strcpy (strOsc, s);
    }
}
