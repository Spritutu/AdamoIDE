// DlgImportImage.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "ParEdit.h"
#include "dlgimportimage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NMAX_ID_LENGTH       32

/////////////////////////////////////////////////////////////////////////////
// CDlgImportImage dialog


CDlgImportImage::CDlgImportImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImportImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgImportImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImportImage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImportImage, CDialog)
	//{{AFX_MSG_MAP(CDlgImportImage)
	ON_BN_CLICKED(IDC_BTN_CHOOSEJPG, OnBtnChooseJpg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImportImage message handlers

void CDlgImportImage::OnBtnChooseJpg() 
{
	// TODO: Add your control notification handler code here
    int nB=0;
	CKSFileDialog fd(TRUE, "jpg", m_strPath, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, 
		"Image files (*.jpg)|*.jpg|All files (*.*)|*.*||", AfxGetMainWnd());
    if (fd.DoModal()==IDOK)   {
        m_strPath=fd.GetPathName ();
        GetDlgItem (IDC_ST_PATH)->SetWindowText (m_strPath);
    }
}

BOOL CDlgImportImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editID.SubclassEdit(IDC_EDIT_IDJPG, this, PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE);
    CenterWindow ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImportImage::OnOK ()
{
    CString str;
    m_editID.GetWindowText (str);
    str.TrimLeft (); str.TrimRight ();
    if (str.GetLength ()<=NMAX_ID_LENGTH)   {
        if (str.GetLength()>0)   {
            m_strID=str;
            CDialog::OnOK ();
        }
        else   {
            AfxMessageBox (LOADSTRING (IDS_ADAMODLGIMPORTIMAGE_1));
            m_editID.SetFocus ();
        }
    }
    else   {
        AfxMessageBox (LOADSTRING (IDS_ADAMODLGIMPORTIMAGE_2));
        m_editID.SetFocus ();
    }
}

