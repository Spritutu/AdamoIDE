// ide2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ide2.h"

#include "MainFrame.h"
#include "LuaFrame.h"
#include "LuaDoc.h"
#include "LuaView.h"
#include "ConfigDoc.h"
#include "ConfigFrame.h"
#include "TypeSource.h"
#include "OpenSource.h"
#include "TransparentCheckBox.h"
#include "AdamoSplash.h"
#include "AdamoAboutBox.h"
#include "CiLinearGaugeX.h"
#include "CiSevenSegmentAnalogX.h"
#include "CiLabelX.h"
#include "CiLabelX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* external references */
extern const char* achSkinStyles[OPTIONS_SKIN_STYLES];
extern UINT m_nAreYouMe;

#ifdef ADAMO_PROTECTED
/* for protection */
int c1 = 0x02;
int c2 = 0x4731;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIdeApp

BEGIN_MESSAGE_MAP(CIdeApp, CWinApp)
	//{{AFX_MSG_MAP(CIdeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(IDM_PLANTS, OnPlants)
	ON_UPDATE_COMMAND_UI(IDM_PLANTS, OnUpdatePlants)
	ON_COMMAND(IDM_CLOSEPLANT, OnClosePlant)
	ON_UPDATE_COMMAND_UI(IDM_CLOSEPLANT, OnUpdateClosePlant)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIdeApp construction

CIdeApp::CIdeApp() : m_pWorkSpace (NULL), m_hScintilla (NULL), m_hMutexOneInstance (NULL), m_win (WINDOWS_XP), m_ePswLevel ((ePswLevel)-1), m_theme (themeVS2015Light)
{
	strSchema.Empty();
	strColor.Empty();
}

CIdeApp::~CIdeApp()
{
    if (m_hMutexOneInstance)   {
        ::CloseHandle (m_hMutexOneInstance);
        m_hMutexOneInstance=NULL;
    }
    _delete (m_pWorkSpace);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIdeApp object

CIdeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIdeApp initialization

BOOL CIdeApp::InitInstance()
{
    BOOL b=FALSE;

	if (!CheckThiraAlreadyRunning ())   {          // verifichiamo se stiamo gia girando
		::CoInitialize(NULL);
	    m_hScintilla = LoadLibrary("SciLexer.DLL");
	    if ( !m_hScintilla )
	    {
		    AfxMessageBox(LOADSTRING (IDS_ADAMOIDE2_1), MB_OK|MB_ICONERROR);
		    return FALSE;
	    }
		
	    AfxEnableControlContainer();
		
	    // Standard initialization
	    // If you are not using these features and wish to reduce the size
	    //  of your final executable, you should remove from the following
	    //  the specific initialization routines you do not need.

	    // Change the registry key under which our settings are stored.
	    // TODO: You should modify this string to be something appropriate
	    // such as the name of your company or organization.
	    SetRegistryKey(_T("Kinema AST S.r.l. - Thira"));
    
	    LoadStdProfileSettings();  // Load standard INI file options (including MRU)
		if (GetPrivateWindowsVersion ())   {
		    AfxMessageBox(LOADSTRING (IDS_ADAMOIDE2_5), MB_OK|MB_ICONERROR);
			return FALSE;
		}
		/* carichiamo i moduli delle librerie XTP */
		LoadXTPModules ();
        m_opt.LoadOptions (GetModuleDir ());
		ParseCommandLine ();
        CAdamoSplash* pSplash=new CAdamoSplash;
        pSplash->Create (IDD_DLG_SPLASH);
	    // Register the application's document templates.  Document templates
	    //  serve as the connection between documents, frame windows and views.
	    AddDocTemplate(new CEditDebugTemplate);
        AddDocTemplate(new CConfTemplate);
        AddDocTemplate(new CManDiagTemplate);
        AddDocTemplate(new CHTMLTemplate);
        AddDocTemplate(new CShowErrorsTemplate);
        AddDocTemplate(new CResourcesTemplate);
        AddDocTemplate(new CDiagnosticTemplate);
		// create main MDI Frame window
	    CMainFrame* pMainFrame = new CMainFrame;
	    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		    return FALSE;
	    m_pMainWnd = pMainFrame;
		// carichiamo le docking bars */
		if (LoadDockings ())   {
			// Register our Clipboard format names.
			m_uiCopyClip = ::RegisterClipboardFormat("Copy_Object");
			pMainFrame->SetMode(CMainFrame::modeNoPlantNoProject);
			/* carchiamo il path dove trovare gli impianti */
			if (CWorkspace::LoadPlantPath ())
				/* carichiamo l'impianto corrente */
				InitialLoad ();
			pMainFrame->ShowWindowEx(m_nCmdShow);
			pMainFrame->UpdateWindow();
			pSplash->DestroyWindow ();
			b=TRUE;
		}
		_delete (pSplash);
    }
	return b;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CIdeApp::OnAppAbout()
{
	CAdamoAboutBox about;
	about.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIdeApp message handlers


int CIdeApp::ExitInstance() 
{
    if (m_hMutexOneInstance)   {
        ::CloseHandle (m_hMutexOneInstance);
        m_hMutexOneInstance=NULL;
    }

    if ( m_hScintilla )
		FreeLibrary(m_hScintilla);		

	return CWinApp::ExitInstance();
}

CLuaView* CIdeApp::FindProjectFilesView(CProjectFile *pPF)
{
    CAdamoTemplate* pTemplate=GetTemplate (enEditDebugTemplate);
	POSITION pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL)	  {
		CLuaDoc* pDoc = (CLuaDoc*)pTemplate->GetNextDoc(pos);
		if ( pDoc->GetProjectFile() == pPF )
			return pDoc->GetView();
	}
	return NULL;
}

CLuaView* CIdeApp::LoadProjectFilesView(CProjectFile *pPF)
{
	CLuaDoc* pDoc = (CLuaDoc*)GetTemplate (enEditDebugTemplate)->OpenDocumentFile(pPF->GetPathName(),TRUE);
	if ( pDoc )
		return pDoc->GetView();
	else
		return NULL;
}

CLuaView* CIdeApp::OpenProjectFilesView(CProjectFile *pPF, int nLine, bool bShowMarker)
{
	CLuaView* pView = FindProjectFilesView(pPF);
	if ( !pView )
		pView = LoadProjectFilesView(pPF);

	if ( pView )
        if (((CMainFrame*)AfxGetMainWnd ())->GetTemplate ()==enEditDebugTemplate)
    		pView->Activate();

	if ( pView && nLine>=0 )   {
		pView->GetEditor()->EnsureVisible (nLine);
		pView->GetEditor()->GotoLine(nLine, bShowMarker);
	}

	return pView;
}

BOOL CIdeApp::SaveModifiedDocuments()
{
    CAdamoTemplate* pTemplate=GetTemplate (enEditDebugTemplate);
	BOOL bModified = FALSE;

	POSITION pos = pTemplate->GetFirstDocPosition();
	while (pos != NULL)
	{
		CLuaDoc* pDoc = (CLuaDoc*)pTemplate->GetNextDoc(pos);
		if ( !pDoc->IsInProject() )
			continue;

		if ( pDoc->IsModified() )
		{
			pDoc->DoFileSave();
			bModified = TRUE;

			// writing new lines changes breakpoint positions
			pDoc->GetView()->GetEditor()->SetBreakPointsIn(pDoc->GetProjectFile());
		}
	}

	return bModified;
}

void CIdeApp::FormatMessage(char *pszAPI)
{
	LPVOID lpMsgBuf;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	CString str;
	str.Format((LPCTSTR)LOADSTRING (IDS_ADAMOIDE2_2), pszAPI, GetLastError(), lpMsgBuf);
	MessageBox( NULL, (LPCTSTR)str, "Error", MB_OK | MB_ICONINFORMATION );
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

CString CIdeApp::GetModuleDir()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
   	char lpFilename[MAX_PATH];

	GetModuleFileName(GetModuleHandle(NULL), lpFilename, MAX_PATH);
	_splitpath( lpFilename, drive, dir, fname, ext );
	dir[strlen(dir)-1] = '\0';

	return CString(drive) + CString(dir);
}


void CIdeApp::DeleteAllFilesInCurrentDir()
{
	BOOL bFound;
	CFileFind ff;
	if ( ff.FindFile() )
		do
		{
			bFound = ff.FindNextFile();
			CString strPathName = ff.GetFilePath();
			if ( strPathName!=".." && strPathName!="." )
				DeleteFile(strPathName);
		}
		while ( bFound ) ;
}

BOOL CIdeApp::FirstFileIsNewer(CString strPathName1, CString strPathName2)
{
	WIN32_FILE_ATTRIBUTE_DATA attrFile1, attrFile2;

	if (! ::GetFileAttributesEx(strPathName1, GetFileExInfoStandard, &attrFile1) )
		return TRUE;

	if (! ::GetFileAttributesEx(strPathName2, GetFileExInfoStandard, &attrFile2) )
		return TRUE;

	ULARGE_INTEGER time1, time2;
	time1.LowPart = attrFile1.ftLastWriteTime.dwLowDateTime;
	time1.HighPart = attrFile1.ftLastWriteTime.dwHighDateTime;
	time2.LowPart = attrFile2.ftLastWriteTime.dwLowDateTime;
	time2.HighPart = attrFile2.ftLastWriteTime.dwHighDateTime;

	return ( time1.QuadPart > time2.QuadPart );
}

BOOL CIdeApp::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
			      BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CKSFileDialog dlgFile(bOpenFileDialog);
	dlgFile.SetAppPointer(this);
	dlgFile.SetStringFilter(LOADSTRING (IDS_ADAMOIDE2_3));
	dlgFile.SetMyParent(AfxGetMainWnd());
	return dlgFile.DoPromptFileName(fileName,nIDSTitle,lFlags,bOpenFileDialog, pTemplate);
}

/*
** apre un nuovo file .ADA o .CLA
*/
void CIdeApp::OnFileNew() 
{
    CTypeSource ts;
    CProjectFile* pPF=NULL;
    /* chiediamo che nome vogliamo dargli */
    
    CLuaEditor* pLE=((CMainFrame *)AfxGetMainWnd ())->GetActiveLuaEditor ();
    if (ts.DoModal()==IDOK)   {
        CAdamoTemplate* pTemplate=GetTemplate (enEditDebugTemplate);
        CStdioFile f;
        CString strFileName=ts.GetResult();
        /* creaiamo il file vuoto */
        if (f.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))   {
            f.Close();
            /* aggiungiamolo alla lista dei file */
            pPF = m_pWorkSpace->GetActiveProject()->AddFile(strFileName);
        }
        /* apriamo il documento */
        pTemplate->OpenDocumentFile(strFileName);
	    if ( pPF )   {
	        CLuaView* pView = FindProjectFilesView (pPF);
            pView->Activate ();
        }
    }
    else
        if (pLE)
            pLE->SetFocus ();
}

BOOL IsWindow (CWnd *pWnd)
{
	if (pWnd == 0)
		return (0);
	if (::IsWindow (pWnd->m_hWnd) == 0)
		return (0);
	return (1);
}

/*
** OnPlants : gestione dell'apertura e manutenzione degli impianti
*/
void CIdeApp::OnPlants() 
{
    CWorkspace* ws=new CWorkspace;
	if (ws->Load ())  {
        ((CMainFrame*)m_pMainWnd)->LoadWorkSpace ();
        m_pWorkSpace->Activate ();
    }
}

/*
** OnUpdatePlants : abilitiamo sempre e comunque la voce di menu.
*/
void CIdeApp::OnUpdatePlants(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable ();	
}

/*
** OnUpdateFileNew : abilitiamo sempre e comunque la voce di menu.
*/
void CIdeApp::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    CMainFrame *pFrame;
    bool b=false;

    pFrame = (CMainFrame *) AfxGetMainWnd ();
    if (pFrame)   {
        if (pFrame->GetPswLevel ()<=ePswBuilder)
            b=true;
    }
    pCmdUI->Enable (b);	
}

/*
** GetTemplate : la funzione ritorna un puntatore al template il cui enum
**               e' passato per parametro.
*/
CAdamoTemplate* CIdeApp::GetTemplate (enTemplateTypes en)
{
    for(POSITION pos = GetFirstDocTemplatePosition(); pos; ) {
        CAdamoTemplate *pTmpl = (CAdamoTemplate*) GetNextDocTemplate (pos);
        if (pTmpl->KindOf ()==en)
            return pTmpl;
    }
    return NULL;
}

/*
** OnClosePlant : chiude l'impianto corrente rispondendo a un comando da menu
*/
void CIdeApp::OnClosePlant() 
{
    if (m_pWorkSpace)   {
        if (AfxMessageBox (LOADSTRING (IDS_ADAMOIDE2_4), MB_YESNO|MB_ICONQUESTION)==IDYES)   {
            m_pWorkSpace->SavePlantSettings ();
            ((CMainFrame*)m_pMainWnd)->CloseWorkSpace ();
            m_pWorkSpace->Close ();
            m_pWorkSpace->RemoveProject ();
            _delete (m_pWorkSpace);
        }
    }
}

/*
** ClosePlant : chiude l'impianto corrente
*/
void CIdeApp::ClosePlant() 
{
    if (m_pWorkSpace)   {
        m_pWorkSpace->SavePlantSettings ();
        ((CMainFrame*)m_pMainWnd)->CloseWorkSpace ();
        m_pWorkSpace->Close ();
        m_pWorkSpace->RemoveProject ();
        _delete (m_pWorkSpace);
    }
}

void CIdeApp::OnUpdateClosePlant(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable (m_pWorkSpace!=NULL);
}

/*
** InitialLoad ;
*/
void CIdeApp::InitialLoad () 
{
    m_pWorkSpace=new CWorkspace;
	if (m_pWorkSpace->InitialLoad ())  {
        ((CMainFrame*)m_pMainWnd)->LoadWorkSpace ();
        m_pWorkSpace->Activate ();
    }
	else
        _delete (m_pWorkSpace);
}

/*
** CloseCurrentWorkspace :
*/
void CIdeApp::CloseCurrentWorkspace () 
{
    if (m_pWorkSpace)   {
        m_pWorkSpace->SavePlantSettings ();
        ((CMainFrame*)m_pMainWnd)->CloseWorkSpace ();
        m_pWorkSpace->Close ();
        m_pWorkSpace->RemoveProject ();
        _delete (m_pWorkSpace);
    }
}

/*
** GetCurrentWorkspaceName :
*/
CString CIdeApp::GetCurrentWorkspaceName () 
{
    return m_pWorkSpace->GetWorkspacePathName ();
}

/*
** GetCurrentWorkspace :
*/
CWorkspace* CIdeApp::GetCurrentWorkspace () 
{
    return m_pWorkSpace;
}

/*
** OnFileOpen :
*/
void CIdeApp::OnFileOpen()
{
    COpenSource os;

    CLuaEditor* pLE=((CMainFrame *)AfxGetMainWnd ())->GetActiveLuaEditor ();
    /* chiediamo che file vogliamo aprire */
    if (os.DoModal()==IDOK)   {
        /* prendiamo il prescelto */
        CString strFileName=os.GetResult();
        /* apriamolo */
        CProjectFile* pPF = GETWS()->GetActiveProject()->GetProjectFile(strFileName);
        /* apriamo il sorgente */
	    if ( pPF )   {
		    CLuaView* pView = theApp.OpenProjectFilesView(pPF);
            pView->Activate ();
        }
    }
    else
        if (pLE)
            pLE->SetFocus ();
}

BOOL CALLBACK CIdeApp::searcher(HWND hWnd, LPARAM lParam)
{
    DWORD result;
    LRESULT ok = ::SendMessageTimeout(hWnd,
                                      m_nAreYouMe,
                                      0, 0, 
                                      SMTO_BLOCK |
                                      SMTO_ABORTIFHUNG,
                                      200,
                                      &result);
    if(ok == 0)
       return TRUE;
    if(result == m_nAreYouMe)    {
        HWND * target = (HWND *)lParam;
        *target = hWnd;
        return FALSE;
    }
    return TRUE;
} 

bool CIdeApp::CheckThiraAlreadyRunning ()
{
    bool bAlareadyRunning;
    m_hMutexOneInstance = ::CreateMutex( NULL, FALSE, MUTEX_NOTIFY_ITSME);
    bAlareadyRunning = ( ::GetLastError() == ERROR_ALREADY_EXISTS || 
                       ::GetLastError() == ERROR_ACCESS_DENIED);
    // The call fails with ERROR_ACCESS_DENIED if the Mutex was 
    // created in a different users session because of passing
    // NULL for the SECURITY_ATTRIBUTES on Mutex creation);

    if (bAlareadyRunning)   {
	    HWND hOther = NULL;
        EnumWindows(searcher, (LPARAM)&hOther);
        if ( hOther != NULL )        {
            ::SetForegroundWindow( hOther );
            if ( IsIconic( hOther ) )   {
                ::ShowWindow( hOther, SW_RESTORE );
            }
            else
                ::ShowWindow( hOther, SW_NORMAL );
        }
    }
    return bAlareadyRunning;
}

bool CIdeApp::LoadDockings ()
{
	bool bRet = true;
	/* inizializzazione docking */
	if (!((CMainFrame *)AfxGetMainWnd ())->InitDockingWindows())   {
		AfxMessageBox (LOADSTRING (IDS_ERROR_RESTORING_DOCKINGS));
		bRet = false;
	}
	else
		((CMainFrame *)AfxGetMainWnd ())->LoadSkin ();
	return bRet;
}

int CIdeApp::GetPrivateWindowsVersion ()
{
	OSVERSIONINFO ver;
	int nB=0;

	ver.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	::GetVersionEx (&ver);
	if (ver.dwMajorVersion == 5 && ver.dwMinorVersion == 2)
		m_win = WINDOWS_XP;
	else
	if (ver.dwMajorVersion == 5 && ver.dwMinorVersion == 1)
		m_win = WINDOWS_XP;
	else
	if (ver.dwMajorVersion == 6 && ver.dwMinorVersion == 0)
		m_win = WINDOWS_VISTA;
	else
	if (ver.dwMajorVersion == 6 && ver.dwMinorVersion == 1)
		m_win = WINDOWS_7;
	else
	if (ver.dwMajorVersion == 6 && ver.dwMinorVersion == 2)
		m_win = WINDOWS_7;
	else
		nB=-1;
	return nB;
}

int CIdeApp::ParseCommandLine ()
{
	int n, nRet = -1;

	if (m_lpCmdLine[0] != '\0')   {
		CString str (m_lpCmdLine);
		if ((n = str.Find ("/PSW")) != -1)   {
			n = str.Find (' ', n);	
			while (str[n] == ' ' && str[n] != '\0')
				n++;
			if (str[n] != '\0')	 {
				m_ePswLevel = (ePswLevel) (str[n] - '0');
				if (m_ePswLevel < ePswBuilder)
					m_ePswLevel = ePswBuilder;
				else
				if (m_ePswLevel > ePswUser)
					m_ePswLevel = ePswUser;
				nRet = 0;
			}
		}
	}
	return nRet;
}

void CIdeApp::LoadXTPModules ()
{
	TCHAR szAppPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(m_hInstance, szAppPath, _MAX_PATH));

	CString strAppPath(szAppPath);
	int nIndex = strAppPath.ReverseFind(_T('\\'));
	if (nIndex > 0)
	{
		strAppPath = strAppPath.Left(nIndex);
	}
	else
	{
		strAppPath.Empty();
	}

	CString strStylesPath;
	strStylesPath += strAppPath + _T("\\Styles\\");
	m_hModule2007.LoadLibrary(strStylesPath + _T("Office2007.dll"));
	m_hModule2010.LoadLibrary(strStylesPath + _T("Office2010.dll"));
	m_hModule2013.LoadLibrary(strStylesPath + _T("Office2013.dll"));
	m_hModuleWindows7.LoadLibrary(strStylesPath + _T("Windows7.dll"));
	m_hModuleVisualStudio2012.LoadLibrary(strStylesPath + _T("VisualStudio2012.dll"));
	m_hModuleVisualStudio2015.LoadLibrary(strStylesPath + _T("VisualStudio2015.dll"));
}

XTPPropertyGridPaintTheme CIdeApp::GetXTPThemePropertyGrid ()
{
	XTPPropertyGridPaintTheme pt;
	CString strSkin = m_opt.GetSkin ();
	for (int i = 0; i<sizeof (achSkinStyles) / sizeof (char*); i++)
		if (strSkin == achSkinStyles[i])   {
			switch (i)   {
				case 0 :
					pt = xtpGridThemeVisualStudio2010;
					break;
				case 1 :
					pt = xtpGridThemeVisualStudio2012Light;
					break;
				case 2 :
					pt = xtpGridThemeVisualStudio2012Dark;
					break;
				case 3 :
				case 4 :
				case 5 :
					pt = xtpGridThemeVisualStudio2015;
					break;
			}
		}
	return pt;
}
