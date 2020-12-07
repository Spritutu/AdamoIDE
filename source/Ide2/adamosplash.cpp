// AdamoSplash.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include <resource.h>
#include "AdamoSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoSplash dialog


CAdamoSplash::CAdamoSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoSplash::IDD, pParent), m_bInitialized (false)
{
	//{{AFX_DATA_INIT(CAdamoSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_SPLASH_VERSION, m_ctlVersion);
	//{{AFX_DATA_MAP(CAdamoSplash)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoSplash, CDialog)
	//{{AFX_MSG_MAP(CAdamoSplash)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSplash message handlers

BOOL CAdamoSplash::OnInitDialog() 
{
    BITMAP b;
	CDialog::OnInitDialog();
	CDC* pDC = GetDC();
	CBitmap bmp, bmpResized;
	bmp.LoadBitmapA (IDB_THYRASPLASH);
	bmp.GetBitmap(&b);
	double fpRatio = (double) b.bmWidth / (double) b.bmHeight;
	//m_fpSizeX = DIMENSIONE_SPLASH * fpRatio;
	//m_fpSizeY = DIMENSIONE_SPLASH;
	m_fpSizeX = b.bmWidth;
	m_fpSizeY = b.bmHeight;
	//((CMainFrame*)AfxGetMainWnd())->ResizeBitmap(bmp, bmpResized, (int) m_fpSizeX, (int) m_fpSizeY);
	/* carichiamo il bitmap */
	//m_bkg.CreatePatternBrush (&bmpResized);
	m_bkg.CreatePatternBrush(&bmp);
	/* ridimensioniamo la dialog */
	m_bInitialized = true;
	SetWindowPos (NULL, 0, 0, (int) m_fpSizeX, (int) m_fpSizeY, SWP_NOMOVE);
	/* centriamo la finestra */
	CenterWindow ();
	ReleaseDC(pDC);
	InitVersionLabel();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAdamoSplash::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_DLG)
        return m_bkg;

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

/*
** OnSize :
*/
void CAdamoSplash::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_bInitialized)
		m_ctlVersion.MoveWindow (26, (cy - 150), 300, 25);
}

/*
** InitVersionLabel :
*/
void CAdamoSplash::InitVersionLabel ()
{
	CString str;
	VERSION v;

	m_ctlVersion.SetTransparent(true);
	//m_ctlVersion.SetTextColor (RGB(0, 96, 160));
	m_ctlVersion.SetTextColor (RGB(255, 255, 255));
	m_ctlVersion.SetFontName("Segoe UI");
	m_ctlVersion.SetFontSize(10);
	m_ctlVersion.SetFontBold(true);
	m_ctlVersion.SetFontItalic(false);
	CPath strPath = GETOPT()->GetPathBin();
	strPath = (CString) strPath + "\\" + "Exact.exe";
	if (((CMainFrame*)AfxGetMainWnd())->GetFileVersion(strPath, &v) == 0)   {
		str.Format("Version %d.%d.%d.%d", v.major, v.minor, v.hotfix, v.other);
		m_ctlVersion.SetText (str);
	}
}
