// AdamoAboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "TransparentStatic.h"
#include "AdamoAboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox dialog


CAdamoAboutBox::CAdamoAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoAboutBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoAboutBox)
	//}}AFX_DATA_INIT
}


void CAdamoAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoAboutBox)
	DDX_Control(pDX, IDC_ST_PRODUCTID, m_stProductID);
	DDX_Control(pDX, IDC_ST_VERSION, m_stProductVersion);
	DDX_Control(pDX, IDC_ST_COMPANY, m_stProductCompany);
	DDX_Control(pDX, IDC_ST_RIGHTS_RESERVED, m_stRightsReserved);
	DDX_Control(pDX, IDC_ST_KINEMASUITE, m_stKinemaSuite);
	DDX_Control(pDX, IDC_ST_COMMUNICATIONINTERFACE, m_stCommInterface);
	DDX_Control(pDX, IDC_ST_COMMUNICATIOMODULE, m_stCommModule);
	DDX_Control(pDX, IDC_ST_TRANSLATIONMODULE, m_stTranslationModule);
	DDX_Control(pDX, IDC_ST_FIRMWARE, m_stFirmware);
	DDX_Control(pDX, IDC_ST_CIVER, m_stCommInterfaceVer);
	DDX_Control(pDX, IDC_ST_CMVER, m_stCommModuleVer);
	DDX_Control(pDX, IDC_ST_TMVER, m_stTranslationModuleVer);
	DDX_Control(pDX, IDC_ST_FWVER, m_stFirmwareVer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoAboutBox, CDialog)
	//{{AFX_MSG_MAP(CAdamoAboutBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox message handlers

BOOL CAdamoAboutBox::OnInitDialog() 
{
    BITMAP b;
	CDialog::OnInitDialog();
    /* carichiamo il bitmap */
    CBitmap bmp;
    bmp.LoadBitmap (IDB_THYRAABOUT);
    bmp.GetBitmap (&b);
    m_bkg.CreatePatternBrush (&bmp);
    /* ridimensioniamo la dialog */
    SetWindowPos (NULL, 0, 0, b.bmWidth, b.bmHeight, SWP_NOMOVE);
	/* centriamo la finestra */
	CenterWindow ();
    /* creiamo il font con cui visualizziamo le informazioni */
    m_fnt=((CMainFrame*) AfxGetMainWnd ())->CreateFont ("Segoe UI", 18, FW_BOLD);
	m_fntBig = ((CMainFrame*)AfxGetMainWnd())->CreateFont("Segoe UI", 24, FW_BOLD);
	m_stProductID.SetFont (m_fnt);
    m_stProductVersion.SetFont (m_fnt);
    m_stProductCompany.SetFont (m_fntBig);
    m_stRightsReserved.SetFont(m_fnt);
	m_stKinemaSuite.SetFont(m_fntBig);
	m_stCommInterface.SetFont(m_fnt);
	m_stCommModule.SetFont(m_fnt);
	m_stTranslationModule.SetFont(m_fnt);
	m_stFirmware.SetFont(m_fnt);
	m_stCommInterfaceVer.SetFont(m_fnt);
	m_stCommModuleVer.SetFont(m_fnt);
	m_stTranslationModuleVer.SetFont(m_fnt);
	m_stFirmwareVer.SetFont(m_fnt);
	/* attributi dei controlli statici */
    m_stProductID.SetForeColor (RGB (64, 64, 64));
    m_stProductID.SetBackColor (RGB (255, 255, 255));
    m_stProductID.SetXOffset (1);
    m_stProductID.SetYOffset (1);
    m_stProductVersion.SetForeColor (RGB (64, 64, 64));
    m_stProductVersion.SetBackColor (RGB (255, 255, 255));
    m_stProductVersion.SetXOffset (-1);
    m_stProductVersion.SetYOffset (-1);
    m_stProductCompany.SetForeColor (RGB (64, 64, 64));
    m_stProductCompany.SetBackColor (RGB (255, 255, 255));
    m_stProductCompany.SetXOffset (1);
    m_stProductCompany.SetYOffset (1);
    m_stRightsReserved.SetForeColor(RGB(64, 64, 64));
    m_stRightsReserved.SetBackColor(RGB(255, 255, 255));
    m_stRightsReserved.SetXOffset(1);
    m_stRightsReserved.SetYOffset(1);
	m_stKinemaSuite.SetForeColor(RGB(64, 64, 64));
	m_stKinemaSuite.SetBackColor(RGB(255, 255, 255));
	m_stKinemaSuite.SetXOffset(1);
	m_stKinemaSuite.SetYOffset(1);
	m_stCommInterface.SetForeColor(RGB(64, 64, 64));
	m_stCommInterface.SetBackColor(RGB(255, 255, 255));
	m_stCommInterface.SetXOffset(1);
	m_stCommInterface.SetYOffset(1);
	m_stCommModule.SetForeColor(RGB(64, 64, 64));
	m_stCommModule.SetBackColor(RGB(255, 255, 255));
	m_stCommModule.SetXOffset(1);
	m_stCommModule.SetYOffset(1);
	m_stTranslationModule.SetForeColor(RGB(64, 64, 64));
	m_stTranslationModule.SetBackColor(RGB(255, 255, 255));
	m_stTranslationModule.SetXOffset(1);
	m_stTranslationModule.SetYOffset(1);
	m_stFirmware.SetForeColor(RGB(64, 64, 64));
	m_stFirmware.SetBackColor(RGB(255, 255, 255));
	m_stFirmware.SetXOffset(1);
	m_stFirmware.SetYOffset(1);
	m_stCommInterfaceVer.SetForeColor(RGB(64, 64, 64));
	m_stCommInterfaceVer.SetBackColor(RGB(255, 255, 255));
	m_stCommInterfaceVer.SetXOffset(1);
	m_stCommInterfaceVer.SetYOffset(1);
	m_stCommModuleVer.SetForeColor(RGB(64, 64, 64));
	m_stCommModuleVer.SetBackColor(RGB(255, 255, 255));
	m_stCommModuleVer.SetXOffset(1);
	m_stCommModuleVer.SetYOffset(1);
	m_stTranslationModuleVer.SetForeColor(RGB(64, 64, 64));
	m_stTranslationModuleVer.SetBackColor(RGB(255, 255, 255));
	m_stTranslationModuleVer.SetXOffset(1);
	m_stTranslationModuleVer.SetYOffset(1);
	m_stFirmwareVer.SetForeColor(RGB(64, 64, 64));
	m_stFirmwareVer.SetBackColor(RGB(255, 255, 255));
	m_stFirmwareVer.SetXOffset(1);
	m_stFirmwareVer.SetYOffset(1);
	InitVersionLabel();
	InitCommInterfaceLabel();
	InitCommModuleLabel();
	InitTranslationModuleLabel();
	InitFirmwareLabel();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAdamoAboutBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_DLG)
        return m_bkg;

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAdamoAboutBox::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_fnt->DeleteObject ();
    _delete (m_fnt);
	m_fntBig->DeleteObject();
	_delete(m_fntBig);
}

/*
** InitVersionLabel :
*/
void CAdamoAboutBox::InitVersionLabel()
{
	CString str;
	VERSION v;

	CPath strPath = GETOPT()->GetPathBin();
	strPath = (CString)strPath + "\\" + "Exact.exe";
	if (((CMainFrame*)AfxGetMainWnd())->GetFileVersion(strPath, &v) == 0) {
		str.Format("Version %d.%d.%d.%d", v.major, v.minor, v.hotfix, v.other);
		m_stProductVersion.SetWindowTextA (str);
	}
}

/*
** InitCommInterfaceLabel :
*/
void CAdamoAboutBox::InitCommInterfaceLabel()
{
	CString str;
	VERSION v;

	CPath strPath = GETOPT()->GetPathBin();
	strPath = (CString)strPath + "\\" + "AdamoComm.dll";
	if (((CMainFrame*)AfxGetMainWnd())->GetFileVersion(strPath, &v) == 0) {
		str.Format("Version %d.%d.%d.%d", v.major, v.minor, v.hotfix, v.other);
		m_stCommInterfaceVer.SetWindowTextA(str);
	}
}

/*
** InitCommModuleLabel :
*/
void CAdamoAboutBox::InitCommModuleLabel()
{
	CString str;
	VERSION v;

	CPath strPath = GETOPT()->GetPathBin();
	strPath = (CString)strPath + "\\" + "ThiraLink.exe";
	if (((CMainFrame*)AfxGetMainWnd())->GetFileVersion(strPath, &v) == 0) {
		str.Format("Version %d.%d.%d.%d", v.major, v.minor, v.hotfix, v.other);
		m_stCommModuleVer.SetWindowTextA(str);
	}
}

/*
** InitTranslationModuleLabel :
*/
void CAdamoAboutBox::InitTranslationModuleLabel()
{
	CString str;
	VERSION v;

	CPath strPath = GETOPT()->GetPathBin();
	strPath = (CString)strPath + "\\" + "Traduzioni.dll";
	if (((CMainFrame*)AfxGetMainWnd())->GetFileVersion(strPath, &v) == 0) {
		str.Format("Version %d.%d.%d.%d", v.major, v.minor, v.hotfix, v.other);
		m_stTranslationModuleVer.SetWindowTextA(str);
	}
}

/*
** InitFirmwareLabel :
*/
void CAdamoAboutBox::InitFirmwareLabel()
{
	CString str;
	VERSION v;

	CPath strPath = GETOPT()->GetPathBin();
	strPath = (CString)strPath + "\\" + "AdamoRTA.RTA";
	if (((CMainFrame*)AfxGetMainWnd())->GetFileVersion(strPath, &v) == 0) {
		str.Format("Version %d.%d.%d.%d", v.major, v.minor, v.hotfix, v.other);
		m_stFirmwareVer.SetWindowTextA(str);
	}
}
