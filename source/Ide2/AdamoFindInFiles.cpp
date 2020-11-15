// AdamoFindInFiles.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "workspace.h"
#include "LuaEditor.h"
#include "AdamoFindInFiles.h"
#include "DlgGetPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoFindInFiles dialog


CAdamoFindInFiles::CAdamoFindInFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoFindInFiles::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoFindInFiles)
	m_bMatchcase = FALSE;
	m_bMatchregex = FALSE;
	m_bMatchwords = FALSE;
	//}}AFX_DATA_INIT
}


void CAdamoFindInFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoFindInFiles)
	DDX_Control(pDX, IDC_INFOLDER, m_ctrlInFolder);
	DDX_Control(pDX, IDC_INFILESTYPES, m_ctrlFileType);
	DDX_Control(pDX, IDC_SEARCHTEXT, m_ctrlSearchtext);
	DDX_Check(pDX, IDC_MATCHCASE, m_bMatchcase);
	DDX_Check(pDX, IDC_MATCHREGEX, m_bMatchregex);
	DDX_Check(pDX, IDC_MATCHWORDS, m_bMatchwords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoFindInFiles, CDialog)
	//{{AFX_MSG_MAP(CAdamoFindInFiles)
	ON_BN_CLICKED(IDC_BTNFIND, OnBtnFind)
	ON_BN_CLICKED(IDC_BTNSEARCHFOLDER, OnBtnSearchFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoFindInFiles message handlers

BOOL CAdamoFindInFiles::OnInitDialog() 
{
	CDialog::OnInitDialog();
    // get app with old searchstrings
    CIdeApp *app = (CIdeApp*)AfxGetApp();

    CenterWindow(GetParent());

    // set selected text to dropdown
    if (m_pEdit != NULL && app != NULL)   {
        int i;

        m_ctrlSearchtext.ResetContent();
        //    get old find strings and add to combobox
        for (i= app->m_strFindstrings.GetSize()-1;i>=0;i--)   {
            m_ctrlSearchtext.AddString(app->m_strFindstrings.GetAt(i));
        }
        m_ctrlFileType.ResetContent();
        //    get old replace strings and add to combobox
        for (i= app->m_strFindInFilesTypes.GetSize()-1;i>=0;i--)   {
            m_ctrlFileType.AddString(app->m_strFindInFilesTypes.GetAt(i));
        }
        m_ctrlInFolder.ResetContent();
        //    get old replace strings and add to combobox
        for (i= app->m_strFindInFilesFolders.GetSize()-1;i>=0;i--)   {
            m_ctrlInFolder.AddString(app->m_strFindInFilesFolders.GetAt(i));
        }
        //    set new searchtext and check if already present in combobox
        CString strFind = m_pEdit->GetCaretText();
        if (strFind.FindOneOf(_T("\r\n")) < 0)
            m_ctrlSearchtext.SetWindowText(strFind);
        CProject* pPrj=GETWS()->GetActiveProject ();
        if (pPrj)
            m_ctrlInFolder.SetWindowText (pPrj->GetProjectDir());
        m_ctrlFileType.SetWindowText ("*.ada");
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoFindInFiles::StoreStrings (CIdeApp *app, LPCSTR szSearch, LPCSTR szFileType, LPCSTR szFolder)
{
    int i;

    if (app != NULL)   {
        /* stringa da cercare */
        for (i=0;i<app->m_strFindstrings.GetSize();i++)   {
         if (strcmp(app->m_strFindstrings.GetAt(i), szSearch) == 0)
            return;
        }
        app->m_strFindstrings.Add(szSearch);
        m_ctrlSearchtext.InsertString(0, szSearch);
        /* file type */
        for (i=0;i<app->m_strFindInFilesTypes.GetSize();i++)   {
         if (strcmp(app->m_strFindInFilesTypes.GetAt(i), szFileType) == 0)
            return;
        }
        app->m_strFindInFilesTypes.Add(szFileType);
        m_ctrlFileType.InsertString(0, szFileType);
        /* folder */
        for (i=0;i<app->m_strFindInFilesFolders.GetSize();i++)   {
         if (strcmp(app->m_strFindInFilesFolders.GetAt(i), szFolder) == 0)
            return;
        }
        app->m_strFindInFilesFolders.Add(szFolder);
        m_ctrlInFolder.InsertString(0, szFolder);
    }
}


void CAdamoFindInFiles::OnBtnFind() 
{
	// TODO: Add your control notification handler code here
	/* verifichiamo innanzitutto che ci sia una stringa valida da cercare */
    m_ctrlSearchtext.GetWindowText(m_strFind);
    m_strFind.TrimLeft (); m_strFind.TrimRight ();
    if (m_strFind!="")   {
        /* verifichiamo poi che esista un tipo di file valido */
        m_ctrlFileType.GetWindowText(m_strFileType);
        m_strFileType.TrimLeft (); m_strFileType.TrimRight ();
        if (m_strFileType!="")   {
            /* verifichiamo poi che esista veramente il percorso indicato */
            m_ctrlInFolder.GetWindowText(m_strFolder);
            if (!_access (m_strFolder, 00))   {
                /* ok,ora possiamo lanciare la ricerca */
                StoreStrings ((CIdeApp*)AfxGetApp (), m_strFind, m_strFileType, m_strFolder);
                OnOK ();
            }
            else   {
                AfxMessageBox ("Directory non esistente", MB_ICONWARNING);
            }
        }
    }
}

void CAdamoFindInFiles::OnBtnSearchFolder() 
{
	// TODO: Add your control notification handler code here
	CDlgGetPath dlg( this );
    CString strFolder;

    m_ctrlInFolder.GetWindowText(strFolder);
	dlg.SetPath (strFolder);
	if (dlg.DoModal()==IDOK)	{
		strFolder=dlg.GetPath ();
        m_ctrlInFolder.SetWindowText (strFolder);
	}
}
