// DlgSlotView.cpp : implementation file
//

#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DlgSlotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSlotView dialog


CDlgSlotView::CDlgSlotView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSlotView::IDD, pParent), m_bFreeze (false)
{
	//{{AFX_DATA_INIT(CDlgSlotView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgSlotView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSlotView)
	DDX_Control(pDX, IDC_SLOTBREAK, m_ctrlButBreak);
	DDX_Control(pDX, IDC_SLOTSTATESUSPENDED, m_ctrlButSuspend);
	DDX_Control(pDX, IDC_SLOTSTATERUN, m_ctrlButRun);
	DDX_Control(pDX, IDC_SLOTDELETE, m_ctrlButDelete);
	DDX_Control(pDX, IDC_SLOTFREEZE, m_ctrlButFreeze);
	DDX_Control(pDX, IDC_LISTSLOTS, m_slotView);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSlotView, CDialog)
	//{{AFX_MSG_MAP(CDlgSlotView)
	ON_BN_CLICKED(IDC_SLOTDELETE, OnSlotDelete)
	ON_BN_CLICKED(IDC_SLOTSTATERUN, OnSlotStateRun)
	ON_BN_CLICKED(IDC_SLOTSTATESUSPENDED, OnSlotStateSuspended)
	ON_BN_CLICKED(IDC_SLOTFREEZE, OnSlotFreeze)
	ON_BN_CLICKED(IDC_SLOTBREAK, OnSlotBreak)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSlotView message handlers

BOOL CDlgSlotView::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_slotView.InitSlotView();
    CenterWindow ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSlotView::OnSlotDelete() 
{
    stTaskData td;

    if (m_slotView.GetSelectedSlot (&td)!=-1)
        m_pMachine->SlotDelete (&td);
    m_slotView.DaUnaRinfrescata ();
    m_slotView.SetFocus ();
}


void CDlgSlotView::OnSlotStateRun() 
{
    stTaskData td;

    if (m_slotView.GetSelectedSlot (&td)!=-1)   {
        td.nStatus=TS_RUN;
        m_pMachine->SlotSetStatus (&td);
    }
    m_slotView.DaUnaRinfrescata ();
    m_slotView.SetFocus ();
}

void CDlgSlotView::OnSlotStateSuspended() 
{
    stTaskData td;

    if (m_slotView.GetSelectedSlot (&td)!=-1)   {
        td.nStatus=TS_SUSPENDED;
        m_pMachine->SlotSetStatus (&td);
    }
    m_slotView.DaUnaRinfrescata ();
    m_slotView.SetFocus ();
}

void CDlgSlotView::OnSlotFreeze() 
{
    if (m_bFreeze)   {
        m_bFreeze=false;
        m_ctrlButFreeze.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_1));
        m_slotView.Refresh ();
    }
    else   {
        m_bFreeze=true;
        m_ctrlButFreeze.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_2));
        m_slotView.Freeze ();
    }
    m_slotView.SetFocus ();
}

void CDlgSlotView::OnSlotBreak() 
{
    CCmdGetDebugData dd;
    stTaskData td;
    /* leggiamo la task selezionata */
    if (m_slotView.GetSelectedSlot (&td)!=-1)   {
        m_pMachine->SendDebugCmd (CMDDBG_BREAK, 0, 0, td.szTaskName);
        if (m_pMachine->GetDebugData (&dd) == S_OK)   {
            CProject* pPrj=GETWS()->GetActiveProject();
            CProjectFile* pPrjFile=pPrj->GetProjectFile (dd.m_nModule);
            ((CMainFrame* )AfxGetMainWnd ())->GotoFileLine (pPrjFile->GetPathName(), dd.m_nLine);
            EndDialog (0);
        }
    }
}
