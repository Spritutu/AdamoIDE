// AdamoSplash.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoSplash dialog


CAdamoSplash::CAdamoSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoSplash)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoSplash)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoSplash, CDialog)
	//{{AFX_MSG_MAP(CAdamoSplash)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSplash message handlers

BOOL CAdamoSplash::OnInitDialog() 
{
    BITMAP b;
	CDialog::OnInitDialog();
    /* carichiamo il bitmap */
    CBitmap bmp;
    bmp.LoadBitmap (IDB_THYRASPLASH);
    bmp.GetBitmap (&b);
    m_bkg.CreatePatternBrush (&bmp);
    /* ridimensioniamo la dialog */
    SetWindowPos (NULL, 0, 0, b.bmWidth, b.bmHeight, SWP_NOMOVE);
    /* creiamo la regione */
    m_rgn.CreateRoundRectRgn (0, 0, b.bmWidth, b.bmHeight, 32, 32);
    SetWindowRgn (m_rgn, false);
	/* centriamo la finestra */
	CenterWindow ();
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
