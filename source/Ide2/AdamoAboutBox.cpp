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
    /* creiamo la regione */
    m_rgn.CreateRoundRectRgn (0, 0, b.bmWidth, b.bmHeight, 32, 32);
    SetWindowRgn (m_rgn, false);
	/* centriamo la finestra */
	CenterWindow ();
    /* creiamo il font con cui visualizziamo le informazioni */
    m_fnt=((CMainFrame*) AfxGetMainWnd ())->CreateFont ("Ariel", 18, FW_BOLD);
    m_stProductID.SetFont (m_fnt);
    m_stProductVersion.SetFont (m_fnt);
    m_stProductCompany.SetFont (m_fnt);
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
}
