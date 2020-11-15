// DlgHWCanOpenNodes.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoTemplates.h"
#include "dlghwcanopennodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNodes dialog
IMPLEMENT_DYNAMIC (CDlgHWCanOpenNodes, CDialog)

CDlgHWCanOpenNodes::CDlgHWCanOpenNodes(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWCanOpenNodes::IDD, pParent), m_pConfigDoc (NULL), m_bInitialized (false), m_pDigitalIO (NULL), m_pAnalogIO (NULL), m_pCanData (NULL), m_pCanAxes (NULL), m_pFont (NULL), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgHWCanOpenNodes)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWCanOpenNodes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHWCanOpenNodes)
	DDX_Control(pDX, IDC_ST_CANTITLE, m_stCanTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHWCanOpenNodes, CDialog)
	//{{AFX_MSG_MAP(CDlgHWCanOpenNodes)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNodes message handlers
void CDlgHWCanOpenNodes::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

/*
** OnInitDialog :
*/
BOOL CDlgHWCanOpenNodes::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CreateFont ();
    InitializeTitles ();
	SetConfigDoc ();
    CreateTab ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** Find :
*/
void CDlgHWCanOpenNodes::Find (const char *szBuffer)
{
    if (strstr (szBuffer, "ID") || strstr (szBuffer, "OD"))   {
        m_pFlatTabCtrl->SetCurSel (0);
        m_pDigitalIO->Find (szBuffer);
    }
    else
    if (strstr (szBuffer, "IA") || strstr (szBuffer, "OA"))   {
        m_pFlatTabCtrl->SetCurSel (1);
        m_pAnalogIO->Find (szBuffer);
    }
}

/*
** InitializeTitles :
*/
void CDlgHWCanOpenNodes::InitializeTitles ()
{
    CString strFormat, strFormat_1, str;
    strFormat=LOADSTRING (IDS_CONFIG_HW_CANOPEN_NODE);
	strFormat_1=LOADSTRING (IDS_CONFIG_HW_CANOPEN_MODULE_NODE);
	if (m_nModule == -1)
		str.Format (strFormat, m_nNode);
	else
		str.Format (strFormat_1, m_nModule, m_nNode);
    m_stCanTitle.SetWindowText (str);
}

/*
** OnSize :
*/
void CDlgHWCanOpenNodes::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_stCanTitle.MoveWindow (10, 10, 300, 40);
        m_pFlatTabCtrl->MoveWindow (10, 35, cx-20, cy-45);
        m_pFlatTabCtrl->Invalidate ();
    }
}

/*
** CreateTab :
*/
void CDlgHWCanOpenNodes::CreateTab ()
{
    RECT rc;

    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTFlatTabCtrl();
	if (m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		&rc, this, IDC_FLAT_TABCTRL))  {

        m_pDigitalIO=new CCanOpenDigitalIO ();
        m_pDigitalIO->SetConfigDoc (m_pConfigDoc);
        m_pDigitalIO->SetProject (m_pPrj);
        m_pDigitalIO->SetKinemaAddress (m_nKinemaAddress);
        m_pDigitalIO->SetCanOpenNode (m_nNode);
        m_pDigitalIO->Create (IDD_DLG_HW_CANOPEN_DIGITALIO, this);
    	m_pFlatTabCtrl->InsertItem(0, LOADSTRING (IDS_CONFIG_HW_CANOPEN_DIGITAL), m_pDigitalIO);

        m_pAnalogIO=new CCanOpenAnalogIO ();
        m_pAnalogIO->SetConfigDoc (m_pConfigDoc);
        m_pAnalogIO->SetProject (m_pPrj);
        m_pAnalogIO->SetKinemaAddress (m_nKinemaAddress);
        m_pAnalogIO->SetCanOpenNode (m_nNode);
        m_pAnalogIO->Create (IDD_DLG_HW_CANOPEN_ANALOGIO, this);
    	m_pFlatTabCtrl->InsertItem(1, LOADSTRING (IDS_CONFIG_HW_CANOPEN_ANALOG), m_pAnalogIO);

        m_pCanAxes=new CCanOpenAxes ();
        m_pCanAxes->SetConfigDoc (m_pConfigDoc);
        m_pCanAxes->SetProject (m_pPrj);
        m_pCanAxes->SetKinemaAddress (m_nKinemaAddress);
        m_pCanAxes->SetCanOpenNode (m_nNode);
        m_pCanAxes->Create (IDD_DLG_HW_CANOPEN_AXES, this);
    	m_pFlatTabCtrl->InsertItem(2, LOADSTRING (IDS_CONFIG_HW_CANOPEN_AXES), m_pCanAxes);

        m_pCanData=new CCanOpenData ();
        m_pCanData->SetConfigDoc (m_pConfigDoc);
        m_pCanData->SetProject (m_pPrj);
        m_pCanData->SetKinemaAddress (m_nKinemaAddress);
        m_pCanData->SetCanOpenNode (m_nNode);
        m_pCanData->Create (IDD_DLG_HW_CANOPEN_DATA, this);
    	m_pFlatTabCtrl->InsertItem(3, LOADSTRING (IDS_CONFIG_HW_CANOPEN_DATIGENERALI), m_pCanData);

		m_pFlatTabCtrl->SetCurSel(0);
    }
}

/*
** RemoveCurrentConnection :
*/
void CDlgHWCanOpenNodes::RemoveCurrentConnection ()
{
    m_pDigitalIO->RemoveCurrentConnection ();
    m_pAnalogIO->RemoveCurrentConnection ();
}

/*
** CreateFont :
*/
void CDlgHWCanOpenNodes::CreateFont ()
{
    m_pFont=((CMainFrame *) AfxGetMainWnd ())->CreateFont ("Tahoma", 18, FW_NORMAL);
    m_stCanTitle.SetFont (m_pFont);
}

/*
** OnDestroy :
*/
void CDlgHWCanOpenNodes::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    if (m_pFont)   {
	    DeleteObject (m_pFont);
        _delete (m_pFont);
    }
    m_pFlatTabCtrl->DeleteAllItems ();
    m_pFlatTabCtrl->DestroyWindow ();
    _delete (m_pFlatTabCtrl);
    m_pDigitalIO->DestroyWindow ();
    m_pAnalogIO->DestroyWindow ();
    m_pCanAxes->DestroyWindow ();
    m_pCanData->DestroyWindow ();
    _delete (m_pDigitalIO);
    _delete (m_pAnalogIO);
	_delete (m_pCanAxes);
    _delete (m_pCanData);
}

/*
** Save :
*/
void CDlgHWCanOpenNodes::Save() 
{
    if (m_pCanAxes)
        m_pCanAxes->Save ();
    if (m_pCanData)
        m_pCanData->Save ();
}

/*
** OnChangePsw :
*/
void CDlgHWCanOpenNodes::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_pCanAxes->OnChangePsw (oldPsw, newPsw);
    m_pAnalogIO->OnChangePsw (oldPsw, newPsw);
    m_pCanAxes->OnChangePsw (oldPsw, newPsw);
    m_pCanData->OnChangePsw (oldPsw, newPsw);
}
