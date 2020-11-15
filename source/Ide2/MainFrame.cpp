/*
** mainframe.cpp
*/

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "afxctl.h"
#include "MainFrame.h"
#include "ScintillaView.h"
#include "LuaDoc.h"
#include "LuaView.h"
#include "errorsdlg.h"
#include "DialogSlot.h"
#include "DlgSlotView.h"
#include "GlobalMatrix.h"
#include "GlobalVariable.h"
#include "GlobalArr.h"
#include "CVariableContainer.h"
#include "IdeMisc.h"
#include "VarListView.h"
#include "WatchListView.h"
#include "GridView.h"
#include "NewDev.h"
#include "ErrorsComboBox.h"
#include "OptionDlg.h"
#include "AdamoSink.h"
#include "AdamoMDAxesView.h"
#include "HTMLDoc.h"
#include "DlgOscilloscope.h"
#include "adamoerr.h"
#include "adamoselistview.h"
#include "AdamoFindInFiles.h"
#include "DockRS.h"
#include "AdamoTables.h"
#include "OpenSource.h"
#include "Breakpoints.h"
#include "CreateSlot.h"
#include "AdamoVersion.h"
#include "DlgConnectToTcp.h"
#include "AdamoOptions.h"
#include "AdamoDlgShowModules.h"
#include "ListViewErrorsCustomColor.h"
#include "AdamoListErrors.h"
#include "dlgmbx.h"

/* global variables */
UINT m_nChangeLanguage;
UINT m_nAreYouMe;
UINT m_nRefreshMetrics;

/* external references */
extern char* vettMonths[12];
extern const char* achSkinStyles[OPTIONS_SKIN_STYLES];

#ifdef ADAMO_PROTECTED
extern "C" BYTE __stdcall bb3 ( WORD Cmd, 
                                WORD Lpt, 
                                WORD Addr, 
                                BYTE *Dat , 
                                BYTE *Code ) ;

char *bb3err[] = {
    "nessun errore" ,
    "comando non valido" ,
    "errore hardware 1" ,
    "errore hardware 2" ,
    "chiave non presente" ,
    "codice accesso errato" ,
    "indirizzo non valido" ,
    "errore hardware 3" ,
    "errore hardware 4" ,
    "interfaccia parallela non presente" ,
    "LPT num non ammesso" ,
    "codice non inserito" ,
    "driver Win NT non presente",
    "tracciatura attiva",
    "puntatore nullo (Code o Data)",
    "porta LPT occupata (non disponibile)",
    "server di rete non presente",
    "numero di Clients collegati superiore a quello gestito dalla chiave",
    "Login di rete già effettuato",
    "Login di rete non effettuato",
    "invio/ricezione di rete",
    "tipo di chiave non supportato",
} ;

extern char limoncello;
extern int c1;
extern int c2;

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_STRINGS, OnUpdateCmdForButtonDynamic)
	ON_WM_CREATE()
	ON_COMMAND(ID_PROJECT_ADD_FILES, OnProjectAddFiles)
	ON_COMMAND(ID_PROJECT_ADD_LIBRARY, OnProjectAddLibrary)
	ON_COMMAND(ID_PROJECT_PROPERTIES, OnProjectProperties)
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUILD_BUILD, OnBuildBuild)
	ON_COMMAND(ID_BUILD_COMPILE, OnBuildCompile)
	ON_UPDATE_COMMAND_UI(ID_BUILD_COMPILE, OnUpdateBuildBuild)
	ON_COMMAND(ID_BUILD_REBUILDALL, OnBuildRebuildall)
	ON_COMMAND(ID_BUILD_CLEAN, OnBuildClean)
	ON_COMMAND(ID_BUILD_GO, OnBuildGo)
	ON_UPDATE_COMMAND_UI(ID_BUILD_STOP, OnUpdateBuildStop)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_GO, OnUpdateDebugBreak)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_BREAK, OnUpdateDebug)
	ON_COMMAND(ID_DEBUG_GO, OnDebugGo)
	ON_COMMAND(ID_DEBUG_STEPINTO, OnDebugStepinto)
	ON_COMMAND(ID_DEBUG_STEPOVER, OnDebugStepover)
	ON_COMMAND(ID_DEBUG_STEPOUT, OnDebugStepout)
	ON_COMMAND(ID_DEBUG_RUNTOCURSOR, OnDebugRuntocursor)
	ON_COMMAND(ID_DEBUG_BREAK, OnDebugBreak)
	ON_COMMAND(ID_DEBUG_STOPDEBUGGING, OnDebugStopdebugging)
	ON_COMMAND(ID_DEBUG_RESTART, OnDebugRestart)
    ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEPROJECT, OnUpdateCmdForProject)
	ON_COMMAND(ID_BUILD_EXECUTE, OnBuildExecute)
	ON_COMMAND(ID_HELP_CONTACTAUTHOR, OnHelpContactauthor)
	ON_COMMAND(ID_HELP_LUAHELPPDF, OnHelpLuahelppdf)
	ON_COMMAND(ID_HELP_VISITHOMEPAGE, OnHelpVisithomepage)
	ON_COMMAND(ID_HELP_LUAHOMEPAGE, OnHelpLuahomepage)
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUILD_STOP, OnBuildStop)
	ON_WM_TIMER()
    ON_COMMAND(ID_EDIT_FIND, OnEditFind)
    ON_COMMAND(ID_EDIT_REPLACEWORD, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACEWORD, OnUpdateEditReplace)
	ON_COMMAND(ID_FILE_ADDPROJECT, OnFileAddproject)
	ON_COMMAND(ID_FILE_OPENPLANT, OnFileOpenplant)
	ON_COMMAND(ID_FILE_SAVEPLANT, OnFileSaveplant)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEPLANT, OnUpdateCmdForPlant)
	ON_COMMAND(ID_FILE_CLOSEPLANT, OnFileCloseplant)
	ON_COMMAND(ID_DEBUG_VIEWSLOTS, OnDebugViewslots)
	ON_COMMAND(ID_BUILD_VIEWVAR, OnBuildViewVar)
	ON_COMMAND(ID_DEBUG_VIEWVAR, OnDebugViewVar)
	ON_COMMAND(ID_DBGBREAK_VIEWVAR, OnDbgBreakViewVar)
	ON_COMMAND(ID_BUILD_VIEWMBX, OnBuildViewMbx)
	ON_COMMAND(ID_DEBUG_VIEWMBX, OnDebugViewMbx)
	ON_COMMAND(ID_DBGBREAK_VIEWMBX, OnDbgBreakViewMbx)
	ON_COMMAND(ID_VIEW_DEVICE, OnViewDevice)
	ON_COMMAND(ID_VIEW_PROP_DEVICE, OnViewProDevice)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateFindReplace)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFile)
	ON_COMMAND(ID_BUTTON_INTERPRETER, OnButtonError)
    ON_COMMAND(ID_VIEW_RESTOREDOCKING, OnViewRestoredocking)
	ON_COMMAND(ID_FILE_OPTION, OnFileOption)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPTION, OnUpdateFileOption)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_FILE_IMPORT_XML, OnFileImportXml)
	ON_COMMAND(ID_FILE_EXPORT_XML, OnFileExportXml)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT, OnUpdateFileImport)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_XML, OnUpdateFileImportXml)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_XML, OnUpdateFileExportXml)
	ON_COMMAND(IDM_OPEN_CONFIG, OnOpenConfig)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_CONFIG, OnUpdateOpenConfig)
	ON_COMMAND(IDM_OPEN_EDITDEBUG, OnOpenEditDebug)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_EDITDEBUG, OnUpdateOpenEditDebug)
	ON_COMMAND(IDM_OPEN_MANUAL, OnOpenManDiag)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_MANUAL, OnUpdateOpenManual)
	ON_COMMAND(IDM_DOWNLOAD_ALL, OnDownloadAll)
	ON_COMMAND(IDM_DOWNLOAD_PARAMETERS, OnDownloadParameters)
	ON_UPDATE_COMMAND_UI(IDM_CLOSE_MODE, OnUpdateCloseMode)
    ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_PREVDOCKING, OnViewPrevdocking)
	ON_COMMAND(ID_VIEW_NEXTDOCKING, OnViewNextdocking)
	ON_COMMAND(IDM_OPEN_MACHINE, OnOpenMachine)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_MACHINE, OnUpdateOpenMachine)
	ON_COMMAND(IDM_ATTACH_DEBUGGER, OnAttachDebugger)
	ON_COMMAND(IDM_DETACH_DEBUGGER, OnDetachDebugger)
	ON_UPDATE_COMMAND_UI(IDM_ATTACH_DEBUGGER, OnUpdateAttachDebugger)
	ON_UPDATE_COMMAND_UI(IDM_DETACH_DEBUGGER, OnUpdateDetachDebugger)
	ON_COMMAND(ID_BUILD_STEPINTO, OnBuildStepInto)
	ON_COMMAND(ID_BUILD_RUNTOCURSOR, OnBuildRunToCursor)
	ON_UPDATE_COMMAND_UI(ID_BUILD_STEPINTO, OnUpdateBuildStepInto)
	ON_UPDATE_COMMAND_UI(ID_BUILD_RUNTOCURSOR, OnUpdateBuildRunToCursor)
    ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(ID_MESSAGES_NEW, OnNewMessage)
	ON_COMMAND(ID_MESSAGES_DELETE, OnDeleteMessage)
	ON_COMMAND(ID_MESSAGES_MODIFY, OnModifyMessage)
	ON_UPDATE_COMMAND_UI(ID_MESSAGES_NEW, OnUpdateNewMessage)
	ON_UPDATE_COMMAND_UI(ID_MESSAGES_MODIFY, OnUpdateModifyMessage)
	ON_UPDATE_COMMAND_UI(ID_MESSAGES_DELETE, OnUpdateDeleteMessage)
	ON_COMMAND(IDM_DOWNLOAD_STRINGS, OnDownloadStrings)
	ON_COMMAND(ID_OSC_START, OnOscStart)
	ON_COMMAND(ID_OSC_STARTPRETRIG, OnOscStartPretrigger)
	ON_COMMAND(ID_OSC_STOP, OnOscStop)
	ON_COMMAND(ID_OSC_CLEAR, OnOscClear)
	ON_COMMAND(ID_OSC_OPTIONS, OnOscOptions)
	ON_COMMAND(ID_OSC_ZOOMTOFIT, OnOscZoomToFit)
	ON_COMMAND(ID_OSC_CURSOR, OnOscShowCursor)
	ON_COMMAND(ID_OSC_SECOND_CURSOR, OnOscShowDeltaXCursor)
	ON_COMMAND(ID_OSC_THIRD_CURSOR, OnOscShowDeltaYCursor)
	ON_COMMAND(ID_OSC_LOAD, OnOscLoad)
	ON_COMMAND(ID_OSC_SAVE, OnOscSave)
	ON_COMMAND(ID_OSC_PRINT, OnOscPrint)
	ON_UPDATE_COMMAND_UI(ID_OSC_START, OnUpdateOscStart)
	ON_UPDATE_COMMAND_UI(ID_OSC_STARTPRETRIG, OnUpdateStartPretrigger)
	ON_UPDATE_COMMAND_UI(ID_OSC_STOP, OnUpdateOscStop)
	ON_UPDATE_COMMAND_UI(ID_OSC_CLEAR, OnUpdateOscClear)
	ON_UPDATE_COMMAND_UI(ID_OSC_ZOOMTOFIT, OnUpdateOscZoomToFit)
	ON_UPDATE_COMMAND_UI(ID_OSC_OPTIONS, OnUpdateOscOptions)
	ON_UPDATE_COMMAND_UI(ID_OSC_CURSOR, OnUpdateOscShowCursor)
	ON_UPDATE_COMMAND_UI(ID_OSC_SECOND_CURSOR, OnUpdateOscShowDeltaXCursor)
	ON_UPDATE_COMMAND_UI(ID_OSC_THIRD_CURSOR, OnUpdateOscShowDeltaYCursor)
	ON_UPDATE_COMMAND_UI(ID_OSC_LOAD, OnUpdateOscLoad)
	ON_UPDATE_COMMAND_UI(ID_OSC_SAVE, OnUpdateOscSave)
	ON_UPDATE_COMMAND_UI(ID_OSC_PRINT, OnUpdateOscPrint)
    ON_UPDATE_COMMAND_UI(ID_VIEW_DEVICE, OnUpdateViewDevice)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROP_DEVICE, OnUpdateViewPropDevice)
	ON_COMMAND(ID_VIEW_HARDWARE, OnViewHardware)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HARDWARE, OnUpdateViewHardware)
	ON_COMMAND(IDM_OPEN_SHOWERRORS, OnShowErrors)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_SHOWERRORS, OnUpdateOpenShowErrors)
	ON_COMMAND(IDM_OPEN_RESOURCE, OnOpenResource)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_RESOURCE, OnUpdateOpenResource)
	ON_COMMAND(ID_EDIT_FIND_IN_FILES, OnEditFindInFiles)
	ON_COMMAND(ID_VIEW_MAINDOCKING, OnViewMainDocking)
	ON_COMMAND(ID_BUILD_NEXT_ERROR, OnBuildNextError)
	ON_COMMAND(ID_BUILD_PREV_ERROR, OnBuildPrevError)
	ON_COMMAND(IDM_OPEN_DIAGNOSTIC, OnOpenDiagnostic)
	ON_UPDATE_COMMAND_UI(IDM_OPEN_DIAGNOSTIC, OnUpdateOpenDiagnostic)
	ON_COMMAND(ID_BUILD_SEARCH_TEXT, OnBuildSearchText)
	ON_COMMAND(ID_EDIT_BREAKPOINTS, OnEditBreakPoints)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BREAKPOINTS, OnUpdateEditBreakPoints)
	ON_COMMAND(ID_DEBUG_RUNSLOTS, OnDebugRunSlots)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_VIEWSLOTS, OnUpdateDebugViewslots)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_RUNSLOTS, OnUpdateDebugRunslots)
    ON_COMMAND(IDM_CLOSE_MODE, OnCloseMode)
    ON_COMMAND(ID_BUTTON_DELETE_ERRORBAR, OnButtonDeleteErrorBar)
	ON_COMMAND(ID_BUTTON_STRINGS, OnButtonString)
	ON_UPDATE_COMMAND_UI(ID_BUILD_GO, OnUpdateBuildBuild)
   	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEPLANT, OnUpdateCmdForPlant)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFile)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFile)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACEWORD, OnUpdateFindReplace)
	ON_UPDATE_COMMAND_UI(ID_BUILD_REBUILDALL, OnUpdateBuildBuild)
	ON_UPDATE_COMMAND_UI(ID_BUILD_CLEAN, OnUpdateBuildBuild)
	ON_UPDATE_COMMAND_UI(ID_BUILD_BUILD, OnUpdateBuildBuild)
	ON_UPDATE_COMMAND_UI(ID_BUILD_EXECUTE, OnUpdateBuildBuild)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STEPINTO, OnUpdateDebugBreak)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STEPOVER, OnUpdateDebugBreak)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_STEPOUT, OnUpdateDebugBreak)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_RUNTOCURSOR, OnUpdateDebugBreak)
	ON_UPDATE_COMMAND_UI(ID_MODE_CONFIG, OnUpdateCmdForPlant)
    ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
    ON_MESSAGE (WM_THEREISANERROR, OnThereIsAnError)
    ON_MESSAGE (WM_OPENDIALOGBARS, OnOpenDialogBars)
    ON_MESSAGE (WM_CLOSEDIALOGERRORS, OnCloseDialogErrors)
    ON_MESSAGE (WM_CLOSEDIALOGSTRINGS, OnCloseDialogStrings)
    ON_MESSAGE (WM_ADAMO_CONNECTED, OnAdamoConnected)
    ON_MESSAGE (WM_ADAMO_DISCONNECTED, OnAdamoDisconnected)
    ON_MESSAGE (WM_ADAMO_STRING, OnThereIsAString)
    ON_MESSAGE (WM_ADAMO_PROGRAMSTOP, OnProgramStop)
    ON_MESSAGE (WM_ADAMO_RESOURCES, OnRequestResources)
    ON_MESSAGE (WM_ADAMO_CCPLANES,  OnRequestCCPlanes)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETE_ERRORBAR, OnUpdateCmdForButtonDynamic)
	ON_WM_KEYDOWN()
    ON_MESSAGE (WM_CLOSEDIALOGFIND, OnCloseDialogFind)
    ON_MESSAGE (WM_CLOSEDIALOGREPLACE, OnCloseDialogReplace)
    ON_REGISTERED_MESSAGE (m_nChangeLanguage, OnChangeLanguage)
    ON_REGISTERED_MESSAGE (m_nAreYouMe, OnAreYouMe)
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_DEBUG_CLEARTRACE, OnDebugClearTrace)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_CLEARTRACE, OnUpdateDebugClearTrace)
    ON_MESSAGE (WM_ADAMO_CHANGE_PSW, OnChangePsw)
	ON_COMMAND(ID_BUILD_BREAK, OnBuildBreak)
	ON_UPDATE_COMMAND_UI(ID_BUILD_BREAK, OnUpdateBuildBreak)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_PSW_LEVEL, OnUpdatePswLevel) 
	ON_UPDATE_COMMAND_UI(ID_PROJECT_ADD_FILES, OnUpdateProjectAddFiles)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_PROPERTIES, OnUpdateProjectProperties)
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
    ON_XTP_EXECUTE(IDM_SE_CHOOSE_MONTH, OnChooseErrorLogMonth)
    ON_XTP_EXECUTE(IDM_DG_SET_ALLNETWORK, OnSetAllCanNetwork)
    ON_XTP_EXECUTE(IDM_EC_SET_ALLNETWORK, OnSetAllEthercatNetwork)
	ON_XTP_INITCOMMANDSPOPUP()
	ON_UPDATE_COMMAND_UI(IDM_SE_CHOOSE_MONTH, OnUpdateChooseErrorLogMonth)
	ON_UPDATE_COMMAND_UI(IDM_DG_SET_ALLNETWORK, OnUpdateSetAllCanNetwork)
	ON_UPDATE_COMMAND_UI(IDM_EC_SET_ALLNETWORK, OnUpdateSetAllEthercatNetwork)
    ON_MESSAGE (WM_ADAMO_GSTARTDOWNLOAD, OnAdamoGStartDownload)
    ON_MESSAGE (WM_ADAMO_GDOWNLOADED, OnAdamoGDownloaded)
	ON_COMMAND(IDM_CONNECT_TO, &CMainFrame::OnConnectTo)
	ON_COMMAND(IDM_CONNECTIONS, &CMainFrame::OnConnections)
	ON_UPDATE_COMMAND_UI(IDM_CONNECT_TO, &CMainFrame::OnUpdateConnectTo)
	ON_UPDATE_COMMAND_UI(IDM_CONNECTIONS, &CMainFrame::OnUpdateConnections)
	ON_UPDATE_COMMAND_UI(IDM_SE_FILTER_ERRORS, OnUpdateSeFilterErrors)
	ON_UPDATE_COMMAND_UI(IDM_SE_CLEAR_FILTER, OnUpdateSeClearFilter)
	ON_UPDATE_COMMAND_UI(IDM_SE_GOTO_LINE, OnUpdateSeGotoLine)
	ON_COMMAND(IDM_SE_FILTER_ERRORS, OnSeFilterErrors)
	ON_COMMAND(IDM_SE_CLEAR_FILTER, OnSeClearFilter)
	ON_COMMAND(IDM_SE_GOTO_LINE, OnSeGotoLine)
	ON_COMMAND(IDM_GOTODEFINITION, OnGotoDefinition)
	ON_COMMAND(IDM_GOTOREFERENCE, OnGotoReference)
	ON_COMMAND(IDM_DELETE_SYMBOL, OnDeleteBrowseSymbol)
	ON_COMMAND(IDM_GOTONEXTREFERENCE, OnGotoNextReference)
	ON_COMMAND(IDM_GOTOPREVREFERENCE, OnGotoPrevReference)
	ON_UPDATE_COMMAND_UI(IDM_GOTODEFINITION, OnUpdateGotoDefinition)
	ON_UPDATE_COMMAND_UI(IDM_GOTOREFERENCE, OnUpdateGotoReference)
	ON_UPDATE_COMMAND_UI(IDM_DELETE_SYMBOL, OnUpdateDeleteBrowseSymbol)
    ON_XTP_EXECUTE(IDM_BROWSE_SYMBOL, OnSymbolToBrowse)
	ON_UPDATE_COMMAND_UI(IDM_BROWSE_SYMBOL, OnUpdateSymbolToBrowse)
	ON_COMMAND(ID_HELP_SYMBOL, OnGotoHelpFunction)
	ON_UPDATE_COMMAND_UI(ID_HELP_SYMBOL, OnUpdateGotoHelpFunction)
	ON_MESSAGE (WM_DELETEERRORS, OnDeleteErrors)
END_MESSAGE_MAP()

extern int anCodiciTipo[MAXAXESDATA];

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_pStringsDlg (NULL), m_pErrorsDlg (NULL),m_pFindDlg (NULL), m_pReplaceDlg (NULL),
                             m_objCurrentLayout (NULL), m_pCT (NULL), m_nAppMode (modeNoProject),m_pErrorBar (NULL), m_pEditBar (NULL), m_pConfEditBar (NULL),
                             m_pEditFindBar (NULL), m_pBuildBar (NULL), m_pDebugBar (NULL), m_pMessageBar (NULL), m_pOscBar (NULL), m_pSEBar (NULL), m_pDGBar (NULL), m_pCanOpenBar (NULL), m_pConfHW (NULL), m_pDGSSCNET (NULL), m_pDGEthercat (NULL), m_pPlcBar (NULL),
                             m_pBrowseBar (NULL), m_nTabCount (0), m_bCtrlPressed (false), m_pCtrlView (NULL), m_pErrorOnBar (NULL),
                             m_nTimer (-1), m_bSearching (false), m_nFFOccurences (0), m_eIAmDoing (eDone), m_hInst (NULL), m_hItalianResources (NULL), m_pBreakedPrjFile (NULL), m_pFontTahoma (NULL), m_pFontTahomaGrassetto (NULL),
                             m_objLayoutEmpty (NULL), m_objLayoutEdit (NULL), m_objLayoutConfig (NULL),
                             m_objLayoutDebug (NULL), m_objLayoutManDiag (NULL), m_objLayoutMachine (NULL),
                             m_objLayoutHW (NULL), m_objLayoutResources (NULL), m_objLayoutHWDiagnostic (NULL),
                             m_objLayoutParametric (NULL), m_nDelayTimer (0), m_nTimerCounter (0), m_pNextToolBar (NULL), m_nClientConnected (0), m_pModulesBar (NULL), m_pECConfHW (NULL),
							 m_pEditBrowse (NULL)


{
	m_hAccelNoProject = ::LoadAccelerators(theApp.m_hInstance, MAKEINTRESOURCE(IDR_ACCEL_NO_PROJECT));
	m_hAccelBuild = ::LoadAccelerators(theApp.m_hInstance, MAKEINTRESOURCE(IDR_ACCEL_BUILD));
	m_hAccelDebug = ::LoadAccelerators(theApp.m_hInstance, MAKEINTRESOURCE(IDR_ACCEL_DEBUG));
	m_hAccelDebugBreak = ::LoadAccelerators(theApp.m_hInstance, MAKEINTRESOURCE(IDR_ACCEL_DEBUG_BREAK));
	m_hAccelConfig = ::LoadAccelerators(theApp.m_hInstance, MAKEINTRESOURCE(IDR_ACCEL_CONFIG));
	m_hAccelResources = ::LoadAccelerators(theApp.m_hInstance, MAKEINTRESOURCE(IDR_ACCEL_RESOURCES));
	m_rcMainFrame.SetRectEmpty();
}

CMainFrame::~CMainFrame()
{
	::DestroyAcceleratorTable(m_hAccelNoProject);
	::DestroyAcceleratorTable(m_hAccelBuild);
	::DestroyAcceleratorTable(m_hAccelDebug);
	::DestroyAcceleratorTable(m_hAccelDebugBreak);
	::DestroyAcceleratorTable(m_hAccelConfig);
	::DestroyAcceleratorTable(m_hAccelResources);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#ifdef ADAMO_PROTECTED
    if (!CheckKey ())
        return -1;
#endif
	/* registro messaggio notifica */
	m_nChangeLanguage = RegisterWindowMessage(MSG_CAMBIOLINGUA);
	m_nAreYouMe = RegisterWindowMessage(WM_NOTIFY_ITSME);
	m_nRefreshMetrics = ::RegisterWindowMessage(MSG_REFRESH_METRICS);
    /* risorse del file eseguibile */
    m_hItalianResources=AfxGetResourceHandle ();
	/* crea i layout dei template */
    m_objLayoutEmpty = m_paneManager.CreateLayout();
    m_objLayoutEdit = m_paneManager.CreateLayout();
    m_objLayoutConfig = m_paneManager.CreateLayout();
    m_objLayoutManDiag = m_paneManager.CreateLayout();
    m_objLayoutMachine = m_paneManager.CreateLayout();
    m_objLayoutDebug = m_paneManager.CreateLayout();
    m_objLayoutHW = m_paneManager.CreateLayout();
    m_objLayoutResources = m_paneManager.CreateLayout();
    m_objLayoutHWDiagnostic = m_paneManager.CreateLayout();
    m_objLayoutParametric = m_paneManager.CreateLayout();
    /* Create Status bar. 
       Important: All control bars including the Status Bar 
       must be created before CommandBars....*/
	if (!m_wndStatusBar.Create(this))   {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
	/* inizializziamo le status bar */
    if (!m_wndStatusBar.InitializeStatusBar ())
        return -1;

	//InitializeErrorStatusBar ();
	XTPPaintManager()->SetTheme(xtpThemeVisualStudio2012Light);

    /* Initialize the command bars */
    if (!InitCommandBars())
        return -1;
    /* Get a pointer to the command bars object. */
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)   {
        TRACE0("Failed to create command bars object.\n");
        return -1;      // fail to create
    }
    /* Add the menu bar*/
    CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Main Menu"), IDR_MAINFRAME);

    if(pMenuBar == NULL)  {
        TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}
    pMenuBar->SetFlags(xtpFlagAddMDISysPopup|xtpFlagIgnoreSetMenuMessage);
    /* Initialize the docking pane manager and set the
       initial them for the docking panes.  Do this only after all
       control bars objects have been created and docked.*/
    m_paneManager.InstallDockingPanes(this);

    /* creiamo i font del programma */
    m_pFontTahoma = CreateFont ("Segoe UI", 15, FW_NORMAL);
    m_pFontTahomaGrassetto = CreateFont ("Segoe UI", 15, FW_BOLD);
    /* per il find in files */
	m_objFinder.SetCallback(FileFinderProc, this);
    /* setta il timer */
    m_nTimer=SetTimer (ID_MAINTIMER, MAINTIMER_ELAPSEDTIME, NULL);
	SetActiveWindow ();
    SetFocus ();
	/* inizializziamo la dll della lingua corretta */
    InitLanguages ();
	/* attacchiamo il tabbed workspace */
	VERIFY(m_MTIClientWnd.Attach(this));
    /* setta gli stili */
    SetXTPStyles ();
    /* settiamo i tool tips */
    SetToolTips ();
    /* carichiamo la toolbar dei moduli */
    m_pModulesBar = (CXTPToolBar*)pCommandBars->Add(_T("Modules"), xtpBarTop);
	if (!m_pModulesBar || !m_pModulesBar->LoadToolBar(IDR_TOOLBAR_MODULES))  {
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
    /* carichiamo la toolbar dei modi di esecuzione */
    m_pModeBar = (CXTPToolBar*)pCommandBars->Add(_T("Mode"), xtpBarTop);
	if (!m_pModeBar || !m_pModeBar->LoadToolBar(IDR_TOOLBAR_TEMPLATE))  {
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	/* affianchiamole */
	DockRightOf (m_pModeBar, m_pModulesBar);
	return 0;
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;
	    if (lpCreateControl->nID == IDM_SE_CHOOSE_MONTH && pToolBar->GetBarID() == IDR_TOOLBARVIEWERRORS)
	    {
		    CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
            for (int i=0; i<sizeof (vettMonths)/sizeof (char*); i++)
		        pComboState->AddString(vettMonths[i]);
		    pComboState->SetDropDownWidth(150);
		    lpCreateControl->pControl = pComboState;
		    return TRUE;
	    }
	    if (lpCreateControl->nID == IDM_DG_SET_ALLNETWORK && pToolBar->GetBarID() == IDR_TOOLBARCANOPEN)
	    {
		    CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
		    pComboState->AddString(LOADSTRING (IDS_ADAMOMAINFRAME_1));
		    pComboState->AddString(LOADSTRING (IDS_ADAMOMAINFRAME_2));
		    pComboState->SetDropDownWidth(75);
            pComboState->SetCurSel (0);
		    lpCreateControl->pControl = pComboState;
		    return TRUE;
	    }
	    if (lpCreateControl->nID == IDM_DG_SET_PROGRESSBAR && pToolBar->GetBarID() == IDR_TOOLBARCANOPEN)
	    {
   			CXTPControlCustom* pControlStatic = (CXTPControlCustom*)CXTPControlCustom::CreateObject();
			
			if (!m_wndProgCtrl.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,
							CRect(0,0,0,0),  this, IDS_PANE_PROGRESSBAR))   {
				return FALSE;
			}
			m_wndProgCtrl.SetRange (1, 128);
			m_wndProgCtrl.SetPos (0);
			m_wndProgCtrl.SetStep (1);
			pControlStatic->SetControl(&m_wndProgCtrl);
			pControlStatic->SetBorders(3, 3, 3, 3);
			pControlStatic->SetSize (CSize (200, 16));
			lpCreateControl->pControl = pControlStatic;
			return TRUE;
		}
	    if (lpCreateControl->nID == IDM_EC_SET_ALLNETWORK && pToolBar->GetBarID() == IDR_TOOLBARETHERCAT)
	    {
		    CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
		    pComboState->AddString(LOADSTRING (IDS_ADAMOMAINFRAME_1));
		    pComboState->AddString(LOADSTRING (IDS_ADAMOMAINFRAME_2));
		    pComboState->SetDropDownWidth(75);
            pComboState->SetCurSel (0);
		    lpCreateControl->pControl = pComboState;
		    return TRUE;
	    }
	    if (lpCreateControl->nID == IDC_BTN_ALLSTATIONS && pToolBar->GetBarID() == IDR_TOOLBARSSCNET)
	    {
		    CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
		    pComboState->AddString(LOADSTRING (IDS_ADAMODGSSCNET_4));
		    pComboState->AddString(LOADSTRING (IDS_ADAMODGSSCNET_5));
		    pComboState->SetDropDownWidth(100);
            pComboState->SetCurSel (0);
		    lpCreateControl->pControl = pComboState;
		    return TRUE;
	    }
	    if (lpCreateControl->nID == IDM_ECSCAN_DEVICE && pToolBar->GetBarID() == IDR_TOOLBARCONFETHERCAT)
	    {
   			CXTPControlCustom* pControlStatic = (CXTPControlCustom*)CXTPControlCustom::CreateObject();

			if (!m_wndEcScanDevice.Create(this,NULL,CPWS_EX_RAISED_EDGE,WS_CHILD|WS_VISIBLE,
							CRect(0, 0, 100, 16), IDS_PANE_ECSCAN_DEVICE))    {
				return FALSE;
			}
			m_wndEcScanDevice.SetBkColor(RGB(255,255,255));

			pControlStatic->SetControl(&m_wndEcScanDevice);
			pControlStatic->SetBorders(3, 3, 3, 3);
			lpCreateControl->pControl = pControlStatic;
			return TRUE;
	    }
	    if (lpCreateControl->nID == IDM_ECSCAN_PROGRESSBAR && pToolBar->GetBarID() == IDR_TOOLBARCONFETHERCAT)
	    {
   			CXTPControlCustom* pControlStatic = (CXTPControlCustom*)CXTPControlCustom::CreateObject();
			
			if (!m_wndEcScanProgress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,
							CRect(0,0,0,0),  this, IDS_PANE_ECSCAN_PROGRESSBAR))   {
				return FALSE;
			}
			m_wndEcScanProgress.SetRange (1, 100);
			m_wndEcScanProgress.SetPos (0);
			m_wndEcScanProgress.SetStep (1);
			pControlStatic->SetControl(&m_wndEcScanProgress);
			pControlStatic->SetBorders(3, 3, 3, 3);
			pControlStatic->SetSize (CSize (200, 16));
			lpCreateControl->pControl = pControlStatic;
			return TRUE;
		}
	    if (lpCreateControl->nID == IDM_BROWSE_SYMBOL && pToolBar->GetBarID() == IDR_TOOLBAR_BROWSE)
	    {
		    m_pEditBrowse = (CXTPControlEdit*)CXTPControlEdit::CreateObject();
		    lpCreateControl->pControl = m_pEditBrowse ;
			return TRUE;
		}
    }
	return FALSE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::InitDockingWindows()
{
    CXTPDockingPane* pPane;
    CXTPDockingPaneLayout *pDummyLayout = m_paneManager.CreateLayout();
	int nTentativi = 0;
	BOOL bRet = FALSE;

    /* configurazione */
	while (nTentativi < NUMERO_TENTATIVI)   {
		__try   {
			nTentativi++;
			if (m_objLayoutConfig->Load(_T("ConfigLayout")))   {
				m_paneManager.SetLayout(m_objLayoutConfig);
			}else{
				m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
				pPane=m_paneManager.CreatePane(ID_VIEW_PROP_DEVICE, CRect(0, 0,200, 120),xtpPaneDockLeft);
				pPane=m_paneManager.CreatePane(ID_VIEW_DEVICE, CRect(0, 0,200, 120), xtpPaneDockTop,pPane);
				m_paneManager.GetLayout(m_objLayoutConfig);//memorizza il layout configurazione
			}
			/* HW */
			if (m_objLayoutHW->Load(_T("LayoutHW")))   {
				m_paneManager.SetLayout(m_objLayoutHW);
			}else{
			   m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
			   pPane=m_paneManager.CreatePane(ID_VIEW_DEVTREE, CRect(0, 0, 200, 120),xtpPaneDockLeft);
			   pPane=m_paneManager.CreatePane(ID_VIEW_HW, CRect(0, 0,200, 120), xtpPaneDockTop,pPane);
			   m_paneManager.GetLayout(m_objLayoutHW);//memorizza il layout configurazione
			}
			/* manuale diagnostico */
			if (m_objLayoutManDiag->Load(_T("ManDiagLayout")))   {
				m_paneManager.SetLayout(m_objLayoutManDiag);
			}
			else   {
				CXTPDockingPane *pOscPane, *pTabAllErrors, *pTabIntErrors, *pTabPLCErrors, *pTabISOErrors, *pTabHardwareErrors, *pTabMessages;
				m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
				pOscPane=m_paneManager.CreatePane(ID_VIEW_OSCILLOSCOPE, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabAllErrors=m_paneManager.CreatePane(ID_VIEW_ALL_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabIntErrors=m_paneManager.CreatePane(ID_VIEW_INT_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabPLCErrors=m_paneManager.CreatePane(ID_VIEW_PLC_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabISOErrors=m_paneManager.CreatePane(ID_VIEW_ISO_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabHardwareErrors=m_paneManager.CreatePane(ID_VIEW_HARDWARE_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabMessages=m_paneManager.CreatePane(ID_VIEW_MESSAGES, CRect(0, 0,200, 120), xtpPaneDockBottom);
				m_paneManager.DockPane (pTabAllErrors, xtpPaneDockRight, pOscPane);
				m_paneManager.AttachPane(pTabIntErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabPLCErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabISOErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabHardwareErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabMessages, pTabAllErrors);
				pTabAllErrors->Select ();
				m_paneManager.GetLayout(m_objLayoutManDiag);//memorizza il layout configurazione
			}
			/* editor */
			if (m_objLayoutEdit->Load(_T("EditLayout"))){
				m_paneManager.SetLayout(m_objLayoutEdit);
			}else{
				CXTPDockingPane *pWorkSpacePane, *pDevTreePane, *pOutputPane, *pFFPane, *pEditLangPane, *pReferencePane, *pHelpOnLine, *pHelpOnLineHtml, *pThiraLuaObjects, *pTabAllErrors, *pTabIntErrors, *pTabPLCErrors, *pTabISOErrors, *pTabHardwareErrors, *pTabMessages;
				m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
				pWorkSpacePane=pPane=m_paneManager.CreatePane(ID_VIEW_WORKSPACE, CRect(0, 0,200, 120), xtpPaneDockLeft);
				pOutputPane=m_paneManager.CreatePane(ID_VIEW_OUTPUT, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pFFPane=m_paneManager.CreatePane(ID_VIEW_FFV, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pDevTreePane=pPane=m_paneManager.CreatePane(ID_VIEW_DEVTREE, CRect(0, 0,200, 120), xtpPaneDockLeft);
				pEditLangPane=m_paneManager.CreatePane(ID_VIEW_EDITLANGUAGES, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pReferencePane=m_paneManager.CreatePane(ID_VIEW_EDITREFERENCES, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pHelpOnLine=m_paneManager.CreatePane(ID_VIEW_HELP_ON_LINE, CRect(0, 0,200, 120), xtpPaneDockRight);
				pHelpOnLineHtml=m_paneManager.CreatePane(ID_VIEW_HELP_ON_LINE_HTML, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pThiraLuaObjects=m_paneManager.CreatePane(ID_VIEW_THIRA_LUA_OBJECTS, CRect(0, 0,200, 120), xtpPaneDockRight);
				pTabAllErrors=m_paneManager.CreatePane(ID_VIEW_ALL_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabIntErrors=m_paneManager.CreatePane(ID_VIEW_INT_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabPLCErrors=m_paneManager.CreatePane(ID_VIEW_PLC_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabISOErrors=m_paneManager.CreatePane(ID_VIEW_ISO_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabHardwareErrors=m_paneManager.CreatePane(ID_VIEW_HARDWARE_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabMessages=m_paneManager.CreatePane(ID_VIEW_MESSAGES, CRect(0, 0,200, 120), xtpPaneDockBottom);
				m_paneManager.AttachPane(pDevTreePane, pWorkSpacePane);
				m_paneManager.AttachPane(pEditLangPane, pOutputPane);
				m_paneManager.AttachPane(pFFPane, pOutputPane);
				m_paneManager.AttachPane(pReferencePane, pOutputPane);
				m_paneManager.AttachPane(pHelpOnLineHtml, pOutputPane);
				m_paneManager.AttachPane(pTabIntErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabPLCErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabISOErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabHardwareErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabMessages, pTabAllErrors);
				pOutputPane->Select ();
				pTabAllErrors->Select ();
				m_paneManager.GetLayout(m_objLayoutEdit);//memorizza il layout editor
			}
			/* debug */
			if (m_objLayoutDebug->Load(_T("DebugLayout"))){
				m_paneManager.SetLayout(m_objLayoutDebug);
			}else{
				CXTPDockingPane *pDebugPane, *pCallStackPane, *pLocalsPane, *pWatchesPane, *pDebugIOPane, *pDebugVarPane, *pFFPane, *pObjectsPane, *pTabAllErrors, *pTabIntErrors, *pTabPLCErrors, *pTabISOErrors, *pTabHardwareErrors, *pTabMessages;
				m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
				pDebugPane=m_paneManager.CreatePane(ID_VIEW_DEBUG, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pCallStackPane=m_paneManager.CreatePane(ID_VIEW_CALLSTACK, CRect(0, 0,200, 120), xtpPaneDockRight);
				pLocalsPane=m_paneManager.CreatePane(ID_VIEW_LOCALS, CRect(0, 0,200, 120), xtpPaneDockRight/*,m_wndOutput.GetPtrDockingPane()*/);
				pWatchesPane=m_paneManager.CreatePane(ID_VIEW_WATCHES, CRect(0, 0,200, 120), xtpPaneDockRight/*,m_wndLocals.GetPtrDockingPane()*/);
				pDebugIOPane=m_paneManager.CreatePane(ID_VIEW_DBGIO, CRect(0, 0,200, 120), xtpPaneDockRight/*,m_wndDbgIO.GetPtrDockingPane()*/);
				pDebugVarPane=m_paneManager.CreatePane(ID_VIEW_DBGVAR, CRect(0, 0,200, 120), xtpPaneDockRight/*,m_wndDbgVar.GetPtrDockingPane()*/);
				pFFPane=m_paneManager.CreatePane(ID_VIEW_FFV, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pObjectsPane=m_paneManager.CreatePane(ID_VIEW_OBJECTS, CRect(0, 0,200, 120), xtpPaneDockRight);
				pTabAllErrors=m_paneManager.CreatePane(ID_VIEW_ALL_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabIntErrors=m_paneManager.CreatePane(ID_VIEW_INT_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabPLCErrors=m_paneManager.CreatePane(ID_VIEW_PLC_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabISOErrors=m_paneManager.CreatePane(ID_VIEW_ISO_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabHardwareErrors=m_paneManager.CreatePane(ID_VIEW_HARDWARE_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabMessages=m_paneManager.CreatePane(ID_VIEW_MESSAGES, CRect(0, 0,200, 120), xtpPaneDockBottom);
				m_paneManager.AttachPane(pFFPane, pDebugPane);
				m_paneManager.AttachPane(pObjectsPane, pDebugIOPane);
				m_paneManager.AttachPane(pTabIntErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabPLCErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabISOErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabHardwareErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabMessages, pTabAllErrors);
				pDebugPane->Select ();
				pTabAllErrors->Select ();
				pCallStackPane->Hide ();
				pLocalsPane->Hide ();
				pWatchesPane->Hide ();
				pDebugIOPane->Hide ();
				pDebugVarPane->Hide ();
				m_paneManager.GetLayout(m_objLayoutDebug);
			}
			/* risorse */
			if (m_objLayoutResources->Load(_T("ResourcesLayout")))   {
				m_paneManager.SetLayout(m_objLayoutResources);
			}else{
				m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
				pPane=m_paneManager.CreatePane(ID_VIEW_RS_CATALOGUE, CRect(0, 0,200, 120),xtpPaneDockLeft);
				m_paneManager.CreatePane(ID_VIEW_RS_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockRight,pPane);
				m_paneManager.CreatePane(ID_VIEW_RS_ACTIVEX_PREVIEW, CRect(0, 0,200, 120), xtpPaneDockRight);
				m_paneManager.GetLayout(m_objLayoutResources);//memorizza il layout delle risorse
			}
			/* Diagnostica hardware */
			if (m_objLayoutHWDiagnostic->Load(_T("HWDiagnostic")))   {
				m_paneManager.SetLayout(m_objLayoutHWDiagnostic);
			}else{
				CXTPDockingPane *pSDOPane, *pEcMailBoxPane, *pTabAllErrors, *pTabIntErrors, *pTabPLCErrors, *pTabISOErrors, *pTabHardwareErrors, *pTabMessages;;
				m_paneManager.SetLayout(pDummyLayout);//resetta il corrente layout
				pSDOPane=m_paneManager.CreatePane(ID_VIEW_DG_SDO, CRect(0, 0,200, 120),xtpPaneDockBottom);
				pEcMailBoxPane=m_paneManager.CreatePane(ID_VIEW_EC_MAILBOX, CRect(0, 0,200, 120),xtpPaneDockBottom);
				pTabAllErrors=m_paneManager.CreatePane(ID_VIEW_ALL_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabIntErrors=m_paneManager.CreatePane(ID_VIEW_INT_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabPLCErrors=m_paneManager.CreatePane(ID_VIEW_PLC_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabISOErrors=m_paneManager.CreatePane(ID_VIEW_ISO_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabHardwareErrors=m_paneManager.CreatePane(ID_VIEW_HARDWARE_ERRORS, CRect(0, 0,200, 120), xtpPaneDockBottom);
				pTabMessages=m_paneManager.CreatePane(ID_VIEW_MESSAGES, CRect(0, 0,200, 120), xtpPaneDockBottom);
				m_paneManager.AttachPane(pSDOPane, pEcMailBoxPane);
				m_paneManager.DockPane (pTabAllErrors, xtpPaneDockRight, pSDOPane);
				m_paneManager.AttachPane(pTabIntErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabPLCErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabISOErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabHardwareErrors, pTabAllErrors);
				m_paneManager.AttachPane(pTabMessages, pTabAllErrors);
				pTabAllErrors->Select ();
				m_paneManager.GetLayout(m_objLayoutHWDiagnostic);//memorizza il layout delle risorse
			}
			/* empty layout */	
			if (m_objLayoutEmpty->Load(_T("EmptyLayout"))){
				m_paneManager.SetLayout(m_objLayoutEmpty);
			}else{
				m_paneManager.SetLayout(pDummyLayout);    //resetta il corrente layout
				m_paneManager.GetLayout(m_objLayoutEmpty);//memorizza il layout vuoto
			}

			int nIDIcons[] = {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 13 */ID_VIEW_LOCALS, 
													   /* 14 */ID_VIEW_WATCHES, 
													   /* 15 */ID_VIEW_WORKSPACE, 
													   /* 16 */ID_VIEW_PROP_DEVICE, 
													   /* 17 */ID_VIEW_OUTPUT, 
													   /* 18 */ID_VIEW_CALLSTACK, 
													   /* 19 */ID_VIEW_DEVICE, 
													   /* 20 */ID_VIEW_PROP_DEVICE, 
													   /* 21 */ID_VIEW_DEVTREE, 
													   /* 22 */ID_VIEW_DBGIO, 
													   /* 23 */ID_VIEW_DBGVAR, 
													   /* 24 */ID_VIEW_EDITLANGUAGES, 
													   /* 25 */ID_VIEW_OSCILLOSCOPE, 
													   /* 26 */ID_VIEW_PAR_TREE, 
													   /* 27 */ID_VIEW_HW, 
													   /* 28 */ID_VIEW_DEBUG, 
													   /* 29 */ID_VIEW_FFV,
													   /* 30 */ID_VIEW_RS_CATALOGUE, 
													   /* 31 */0, 
													   /* 32 */ID_VIEW_RS_PROPERTIES,
													   /* 33 */ID_VIEW_DG_SDO,
													   /* 34 */ID_VIEW_EC_MAILBOX,
													   /* 35 */ID_VIEW_EDITREFERENCES,
													   /* 36 */ID_VIEW_HELP_ON_LINE,
													   /* 36 */ID_VIEW_HELP_ON_LINE_HTML,
													   /* 37 */ID_VIEW_THIRA_LUA_OBJECTS,
													   /* 38 */ID_VIEW_OBJECTS,
													   /* 39 */ID_VIEW_ALL_ERRORS,
													   /* 40 */ID_VIEW_INT_ERRORS,
													   /* 41 */ID_VIEW_PLC_ERRORS,
													   /* 42 */ID_VIEW_ISO_ERRORS,
													   /* 43 */ID_VIEW_HARDWARE_ERRORS,
													   /* 43 */ID_VIEW_MESSAGES,
													   /* 44 */IDB_CAPCOMMENT
			};
			m_paneManager.SetIcons(IDB_CONF_TREE, nIDIcons,
				sizeof(nIDIcons)/sizeof(nIDIcons[0]), RGB(128, 0, 128));
			bRet = TRUE;
			break;
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))   {
			/* cancelliamo il registry */
			DeleteXTRegistry ();
			/* distruggiamo i pane gia' creati */
			DeleteXTPanes ();
			/* cancelliamo e ricreiamo i layout */
			RecreateLeyout ();
			/* rieseguiamo la creazione delle docking */
		}
	}
	if (bRet)   {
		/* config layout */
		m_paneManager.SetLayout (m_objLayoutConfig);
		m_wndDockDevProperty.Init(this, ID_VIEW_PROP_DEVICE);
		m_wndDockDev.Init(this, ID_VIEW_DEVICE);
		/* HW layout */
		m_paneManager.SetLayout (m_objLayoutHW);
		m_wndDockConfigTree.Init (this, ID_VIEW_DEVTREE);
		m_wndDockConfigHW.Init (this, ID_VIEW_HW);
		/* mandiag layout */
		m_paneManager.SetLayout (m_objLayoutManDiag);
		m_wndDockOscilloscope.Init(this, ID_VIEW_OSCILLOSCOPE);
		/* editor layout */
		m_paneManager.SetLayout (m_objLayoutEdit);
		m_wndWorkspace.Init(this, ID_VIEW_WORKSPACE);
		m_wndOutput.Init(this, ID_VIEW_OUTPUT);
		m_wndFFV.Init (this, ID_VIEW_FFV);
		m_wndDockConfigTree.Init(this, ID_VIEW_DEVTREE);
		m_wndDockLangView.Init(this, ID_VIEW_EDITLANGUAGES);
		m_wndReferences.Init(this, ID_VIEW_EDITREFERENCES);
		m_wndHelpOnLine.Init(this, ID_VIEW_HELP_ON_LINE);
		m_wndHelpOnLineHtml.Init(this, ID_VIEW_HELP_ON_LINE_HTML);
		m_wndThiraLuaObjects.Init(this, ID_VIEW_THIRA_LUA_OBJECTS);
		m_wndDockListAllErrors.Init (this, ID_VIEW_ALL_ERRORS);
		m_wndDockListInterpreterErrors.Init (this, ID_VIEW_INT_ERRORS);
		m_wndDockListPlcErrors.Init (this, ID_VIEW_PLC_ERRORS);
		m_wndDockListISOErrors.Init (this, ID_VIEW_ISO_ERRORS);
		m_wndDockListHardwareErrors.Init (this, ID_VIEW_HARDWARE_ERRORS);
		m_wndDockListMessages.Init (this, ID_VIEW_MESSAGES);
		/* debug layout */
		m_paneManager.SetLayout (m_objLayoutDebug);
		m_wndDebug.Init (this, ID_VIEW_DEBUG);
		m_wndCallStack.Init(this, ID_VIEW_CALLSTACK);
		m_wndLocals.Init(this, ID_VIEW_LOCALS);
		m_wndWatches.Init(this, ID_VIEW_WATCHES);
		m_wndDockDbgIO.Init(this, ID_VIEW_DBGIO);
		m_wndDockDbgVar.Init(this, ID_VIEW_DBGVAR);
		m_wndDockDbgObject.Init(this, ID_VIEW_OBJECTS);
		/* resource layout */
		m_paneManager.SetLayout (m_objLayoutResources);
		m_wndDockRSCatalogue.Init(this, ID_VIEW_RS_CATALOGUE);
		m_wndDockRSProperties.Init(this, ID_VIEW_RS_PROPERTIES);
		m_wndDockRSActiveXPreview.Init(this, ID_VIEW_RS_ACTIVEX_PREVIEW);
		/* diagnostica hw layout */
		m_paneManager.SetLayout (m_objLayoutHWDiagnostic);
		m_wndDockDGSdo.Init(this, ID_VIEW_DG_SDO);
		m_wndDockEcMailBox.Init(this, ID_VIEW_EC_MAILBOX);
		/* il layout all'inizio e' quello vuoto */
		m_paneManager.SetLayout (m_objLayoutEmpty);
		m_objCurrentLayout=m_objLayoutEmpty;
	}
    delete pDummyLayout;
    return bRet;
}

/* 
** filter :
*/
int filter (unsigned int code, struct _EXCEPTION_POINTERS *ep) 
{
	return EXCEPTION_EXECUTE_HANDLER;
}

/*
** DeleteXTRegistry :
*/
void CMainFrame::DeleteXTRegistry() 
{
	CRegKey reg;
    reg.Open (HKEY_CURRENT_USER, "SOFTWARE\\Kinema AST S.r.l. - Thira");
	LONG error = reg.RecurseDeleteKey ("Thira");
	reg.Close ();
}

/*
** DeleteXTPanes :
*/
void CMainFrame::DeleteXTPanes() 
{
	m_paneManager.DestroyAll ();
}

/*
** RecreateLeyout :
*/
void CMainFrame::RecreateLeyout() 
{
    _delete (m_objLayoutEmpty);
    _delete (m_objLayoutEdit);
    _delete (m_objLayoutConfig);
    _delete (m_objLayoutDebug);
    _delete (m_objLayoutManDiag);
    _delete (m_objLayoutMachine);
    _delete (m_objLayoutHW);
    _delete (m_objLayoutResources);
    _delete (m_objLayoutHWDiagnostic);
    _delete (m_objLayoutParametric);

	m_objLayoutEmpty = m_paneManager.CreateLayout();
    m_objLayoutEdit = m_paneManager.CreateLayout();
    m_objLayoutConfig = m_paneManager.CreateLayout();
    m_objLayoutManDiag = m_paneManager.CreateLayout();
    m_objLayoutMachine = m_paneManager.CreateLayout();
    m_objLayoutDebug = m_paneManager.CreateLayout();
    m_objLayoutHW = m_paneManager.CreateLayout();
    m_objLayoutResources = m_paneManager.CreateLayout();
    m_objLayoutHWDiagnostic = m_paneManager.CreateLayout();
    m_objLayoutParametric = m_paneManager.CreateLayout();
}

void CMainFrame::OnProjectAddFiles() 
{
	GetProject()->AddFiles();
}

void CMainFrame::OnProjectAddLibrary() 
{
	GetProject()->AddLibrary();
}

void CMainFrame::OnProjectProperties() 
{
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;
    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;
	GetProject()->Properties();
    if (bFocusOnEditor)
        pView->GetEditor ()->SetFocus ();
}

void CMainFrame::OnShowView(UINT nID)
{
	m_paneManager.ShowPane(nID);
}

void CMainFrame::OnClose() 
{
    CWorkspace *pWork = GetWorkSpace();
    if (pWork)   {
        CProject *pPrj=pWork->GetActiveProject ();
        if (pPrj)   {
            if (pPrj->IsCompiling ())   {
                if (AfxMessageBox (LOADSTRING (IDS_ADAMO_EXITWHILECOMPILING), MB_YESNO)==IDYES)
                    pPrj->AbortCompilation ();
                return;
            }
        }
    }
	if ( m_nAppMode==modeDebug || m_nAppMode==modeDebugBreak)
	{
		if ( AfxMessageBox(LOADSTRING (IDS_ADAMOMAINFRAME_20), MB_OKCANCEL)==IDOK )
			OnDebugStopdebugging();
		else
			return;
	}
    IAmGoingTo (eCloseApplication);
	/* facciamo fuori il timer della main window */
    if (m_nTimer!=-1)   {
        KillTimer (ID_MAINTIMER);
        m_nTimer=-1;
    }
    /* give templates a chance to close */
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo ora tutti i documenti */
    AfxGetApp ()->CloseAllDocuments (TRUE);
    /* ora il workspace */
    if(pWork)   {
        pWork->SavePlantSettings ();
        pWork->Close();
    }
    /*salviamo lo stato delle toolbar*/
    SaveCommandBars(_T("CommandBars"));
    /* chiudiamo le docking */    
    m_paneManager.GetLayout(m_objCurrentLayout);
    m_objLayoutHWDiagnostic->Save (_T("HWDiagnostic"));
    m_objLayoutEdit->Save(_T("EditLayout"));
    m_objLayoutConfig->Save(_T("ConfigLayout"));
    m_objLayoutManDiag->Save(_T("ManDiagLayout"));
    m_objLayoutDebug->Save(_T("DebugLayout"));
    m_objLayoutMachine->Save(_T("MachineLayout"));
    m_objLayoutEmpty->Save(_T("EmptyLayout"));
    m_objLayoutHW->Save(_T("LayoutHW"));
    m_objLayoutResources->Save (_T("ResourcesLayout"));
    m_objLayoutParametric->Save (_T("ParametricLayout"));
    /* Save frame window size and position */
	m_wndPosition.SaveWindowPos(this);
    IHaveDoneIt ();
	/* chiamiamo la base class */
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnUpdateBuildBuild(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
        if (GetProject ())
            if (GetProject()->IsCompiling ())
                pCmdUI->Enable(FALSE);
            else
                pCmdUI->Enable(GetMode ()==modeBuild);
        else
            pCmdUI->Enable(FALSE);
    else
        pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateBuildStop (CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(GetMode ()==modeExecute);
}

void CMainFrame::OnBuildBuild() 
{
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;

	theApp.SaveModifiedDocuments();
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
	    pActivePrj->Build();

    if (bFocusOnEditor)
        pView->GetEditor ()->SetFocus ();
}

void CMainFrame::OnBuildCompile() 
{
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;

    CLuaDoc *pDoc = ((CLuaView*)pView)->GetDocument();

	if ( pDoc->IsModified() )
		pDoc->DoFileSave();

    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)  {
        pActivePrj->StartCompilation ();
        GetCommandBars()->UpdateCommandBars ();
		/* prima passata, riempiamo il file del browser con le function */
		pActivePrj->ClearBrowserInfo (pDoc->GetPathName ());
		pDoc->GetProjectFile()->Browse ();
		/* salviamo le informazioni del browser */
		pActivePrj->SaveBrowserInfo ();
		/* seconda passata */
        if (pActivePrj->CompilaMain ()!=-2)
	        pActivePrj->Compile(pDoc->GetProjectFile());
        pActivePrj->EndCompilation ();
    }                                                
    if (bFocusOnEditor)
        pView->GetEditor ()->SetFocus ();
}

CView* CMainFrame::GetActiveView()
{
	CMDIChildWnd *pChild = (CMDIChildWnd *)GetActiveFrame();
	if ( !pChild )
		return NULL;

	CView *pView = pChild->GetActiveView();
	if(pView && (pView->IsKindOf(RUNTIME_CLASS(CLuaView))
                    || pView->IsKindOf(RUNTIME_CLASS(CConfTreeView))))
		return pView;

	return NULL;
}

void CMainFrame::OnBuildRebuildall()
{
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;
    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;
	OnBuildCleanInfo ();
	OnBuildClean();
	OnBuildBuild();
    if (bFocusOnEditor)
        pView->GetEditor ()->SetFocus ();
}

void CMainFrame::OnBuildClean()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
        pActivePrj->CleanIntermediateAndOutputFiles();
}

void CMainFrame::OnBuildGo()
{
    CDialogSlot dlgSlot;
	CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

	theApp.SaveModifiedDocuments();
    
    if (pActivePrj)  {
        if (CheckParameters ())   {
            /* ricompiliamo il programma */
	        if (!pActivePrj->Build() )
		        return;
            if (dlgSlot.DoModal ()==IDOK)   {
                /*setta layout debug*/
                SetLayout (m_objLayoutDebug);
                /* prepariamo il debugger */
	            m_debug.Prepare();
                /* mandiamo giu' i dati relativi ai breakpoints */
                SendDebugData (dlgSlot.m_szSlotName);
                /* facciamo partire il debugger */
                StartDebugging (CMDDBG_GO);
                /* settiamo l'IDE per il debugger */
                SetMode(modeDebug);
                /* settiamo le toolbar giuste */
                SwitchToolBars ();
            }
        }
    }
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if ( message >= _DMSG_FIRST_MSG && message <= _DMSG_LAST_MSG )
		return DebugMessage(message, wParam, lParam);
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

LRESULT CMainFrame::DebugMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch( nMsg )
	{
	case DMSG_HAS_BREAKPOINT:
		return GetProject()->HasBreakPoint((const char*)wParam, (int)lParam);
	case DMSG_GOTO_FILELINE:
		GotoFileLine((const char*)wParam, (int)lParam);
		break; 
	case DMSG_DEBUG_START:
		SetMode(modeDebug);
		break;
    case DMSG_DEBUG_BREAK:
		SetMode(modeDebugBreak);
		break;
	case DMSG_DEBUG_END:
        m_debug.Stop ();
		SetMode(modeBuild);
        SetLayout (m_objLayoutEdit);
		break;
	case DMSG_CLEAR_STACKTRACE:
		m_wndCallStack.Clear();
		break;
	case DMSG_ADD_STACKTRACE:
		m_wndCallStack.Add(((StackTrace*)wParam)->szDesc, 
			((StackTrace*)wParam)->szFile, 
			((StackTrace*)wParam)->nLine);
		break;
	case DMSG_GOTO_STACKTRACE_LEVEL:
		m_wndCallStack.GotoStackTraceLevel(wParam);
		break;
	case DMSG_GET_STACKTRACE_LEVEL:
		return m_wndCallStack.GetLevel();
	case DMSG_CLEAR_LOCALVARIABLES:
		m_wndLocals.RemoveAll();
		break;
	case DMSG_ADD_LOCALVARIABLE:
		m_wndLocals.AddVariable(((Variable*)wParam)->szName, 
			((Variable*)wParam)->szType, 
			((Variable*)wParam)->szValue);
		break;
	case DMSG_REDRAW_WATCHES:
		m_wndWatches.Redraw();
	}

	return 0;
}

/*
** GotoFileLine : muove il cursore alla linea indicata dal debugger.
*/
CView* CMainFrame::GotoFileLine(const char *szFile, int nLine, bool bActivateView, bool bShowMarker)
{
	CLuaView* pView = NULL;
    /* se il modulo e' cambiato, togliamo la frecciolina del debugger */
    if (m_strCrtMod!=szFile)
        ClearGotoLines ();
    /* memorizziamo l'ultimo file debuggato */
    m_strCrtMod=szFile;
	CProjectFile* pPF = GetProject()->GetProjectFile(szFile);
    /* apriamo il sorgente */
	if ( pPF )   {
        if (GetTemplate ()!=enConfTemplate)   {
            if (bActivateView)   {
                OnOpenEditDebug ();
                pView = theApp.OpenProjectFilesView(pPF, nLine, bShowMarker);
                if (pView)
                    pView->Activate ();
            }
            else   {
	            pView = theApp.OpenProjectFilesView(pPF, nLine, bShowMarker);
                if (pView && GetTemplate ()==enEditDebugTemplate)
                    pView->Activate ();
            }
        }
        else
            if (bActivateView)   {
                OnOpenEditDebug ();
                pView = theApp.OpenProjectFilesView(pPF, nLine, bShowMarker);
                if (pView)
                    pView->Activate ();
            }
    }
	return pView;
}

void CMainFrame::SetMode (int nMode)
{
    CXTPDockingPane* pPane;
	m_nAppMode = nMode;
	switch (nMode)	{
	    case modeConfig:
            m_hAccelTable = m_hAccelConfig;
            SwitchMenu ();
            ((CConfTreeView*)GetActiveView())->SetIsConfig(TRUE);
            ((CConfTreeView*)GetActiveView())->SetIsHW(FALSE);
            ((CConfTreeView*)GetActiveView())->SetPlant(GetWorkSpace()->GetPtrPlant());
            ((CConfTreeView*)GetActiveView())->UpdateTree();
            pPane=m_paneManager.FindPane (ID_VIEW_PROP_DEVICE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_3));
            pPane=m_paneManager.FindPane (ID_VIEW_DEVICE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_4));
		    break;
	    case modeLogic:
            //((CConfTreeView*)m_wndDockConfigTree.GetView())->SetIsConfig (TRUE);
            //((CConfTreeView*)m_wndDockConfigTree.GetView())->SetIsHW (FALSE);
            //((CConfTreeView*)m_wndDockConfigTree.GetView())->SetNoHeader (FALSE);
            //((CConfTreeView*)m_wndDockConfigTree.GetView())->UpdateTree ();
            pPane=m_paneManager.FindPane (ID_VIEW_PROP_DEVICE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_3));
            pPane=m_paneManager.FindPane (ID_VIEW_DEVICE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_4));
            break;
	    case modeMachine:
            SwitchMenu ();
		    break;
        case modeExecute :
            SwitchMenu ();
		    break;
	    case modeManDiag:
            SwitchMenu ();
            pPane=m_paneManager.FindPane (ID_VIEW_OSCILLOSCOPE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_6));
            pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_42));
            pPane=m_paneManager.FindPane (ID_VIEW_INT_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_43));
            pPane=m_paneManager.FindPane (ID_VIEW_PLC_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_44));
            pPane=m_paneManager.FindPane (ID_VIEW_ISO_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_45));
            pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_46));
            pPane=m_paneManager.FindPane (ID_VIEW_MESSAGES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_47));
		    break;
        case modeNoPlantNoProject:
            SwitchMenu ();
		    m_hAccelTable = m_hAccelNoProject;
		    m_wndCallStack.Clear();
		    break;
	    case modeNoProject:
            SwitchMenu ();
		    m_hAccelTable = m_hAccelNoProject;
		    m_wndCallStack.Clear();
		    break;
	    case modeBuild:
		    m_hAccelTable = m_hAccelBuild;
            SwitchMenu ();
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetIsConfig(FALSE);
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetNoHeader(TRUE);
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetIsHW(FALSE);
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->UpdateTree();
		    ClearClipBoard();
            pPane=m_paneManager.FindPane (ID_VIEW_WORKSPACE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_13));
            pPane=m_paneManager.FindPane (ID_VIEW_OUTPUT);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_14));
            pPane=m_paneManager.FindPane (ID_VIEW_FFV);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_15));
            pPane=m_paneManager.FindPane (ID_VIEW_DEVTREE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_16));
            pPane=m_paneManager.FindPane (ID_VIEW_EDITLANGUAGES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_17));
            pPane=m_paneManager.FindPane (ID_VIEW_EDITREFERENCES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_36));
            pPane=m_paneManager.FindPane (ID_VIEW_HELP_ON_LINE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_38));
            pPane=m_paneManager.FindPane (ID_VIEW_HELP_ON_LINE_HTML);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_39));
            pPane=m_paneManager.FindPane (ID_VIEW_THIRA_LUA_OBJECTS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_40));
            pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_42));
            pPane=m_paneManager.FindPane (ID_VIEW_INT_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_43));
            pPane=m_paneManager.FindPane (ID_VIEW_PLC_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_44));
            pPane=m_paneManager.FindPane (ID_VIEW_ISO_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_45));
            pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_46));
            pPane=m_paneManager.FindPane (ID_VIEW_MESSAGES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_47));
            break;
	    case modeDebug:   {
		    m_hAccelTable = m_hAccelDebug;
		    m_wndCallStack.Clear();
            SwitchMenu ();
            pPane=m_paneManager.FindPane (ID_VIEW_DEBUG);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_7));
            pPane=m_paneManager.FindPane (ID_VIEW_CALLSTACK);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_8));
            pPane=m_paneManager.FindPane (ID_VIEW_LOCALS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_9));
            pPane=m_paneManager.FindPane (ID_VIEW_WATCHES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_10));
            pPane=m_paneManager.FindPane (ID_VIEW_DBGIO);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_11));
            pPane=m_paneManager.FindPane (ID_VIEW_DBGVAR);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_12));
            pPane=m_paneManager.FindPane (ID_VIEW_FFV);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_15));
            pPane=m_paneManager.FindPane (ID_VIEW_OBJECTS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_41));
            pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_42));
            pPane=m_paneManager.FindPane (ID_VIEW_INT_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_43));
            pPane=m_paneManager.FindPane (ID_VIEW_PLC_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_44));
            pPane=m_paneManager.FindPane (ID_VIEW_ISO_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_45));
            pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_46));
            pPane=m_paneManager.FindPane (ID_VIEW_MESSAGES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_47));
		    break;
        }
	    case modeDebugBreak:
		    m_hAccelTable = m_hAccelDebugBreak;
		    break;
        case modeHardware:
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetIsConfig(FALSE);
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetIsHW(TRUE);
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetNoHeader(TRUE);
            ((CConfTreeView*)m_wndDockConfigTree.GetView())->UpdateTree();
            pPane=m_paneManager.FindPane (ID_VIEW_DEVTREE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_5));
            pPane=m_paneManager.FindPane (ID_VIEW_HW);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_32));
            break;
	    case modeShowErrors:
            SwitchMenu ();
		    break;
	    case modeResources:
		    m_hAccelTable = m_hAccelResources;
            SwitchMenu ();
            pPane=m_paneManager.FindPane (ID_VIEW_RS_CATALOGUE);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_18));
            pPane=m_paneManager.FindPane (ID_VIEW_RS_PROPERTIES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_19));
            pPane=m_paneManager.FindPane (ID_VIEW_RS_ACTIVEX_PREVIEW);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_35));
		    break;
	    case modeDiagnostic:
            SwitchMenu ();
            pPane=m_paneManager.FindPane (ID_VIEW_DG_SDO);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_31));
            pPane=m_paneManager.FindPane (ID_VIEW_EC_MAILBOX);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_34));
            pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_42));
            pPane=m_paneManager.FindPane (ID_VIEW_INT_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_43));
            pPane=m_paneManager.FindPane (ID_VIEW_PLC_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_44));
            pPane=m_paneManager.FindPane (ID_VIEW_ISO_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_45));
            pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_46));
            pPane=m_paneManager.FindPane (ID_VIEW_MESSAGES);
            if (pPane)
                pPane->SetTitle(LOADSTRING (IDS_ADAMOMAINFRAME_47));
		    break;
	}
	OnUpdateFrameTitle (TRUE);
}

void CMainFrame::OnDebugGo() 
{
    /* facciamo ripartire il debugger */
    SetMode(modeDebug);
    StartDebugging (CMDDBG_GO);
}

void CMainFrame::OnUpdateDebugBreak(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetMode()==modeDebugBreak);	
}

void CMainFrame::OnUpdateDebug(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetMode()==modeDebug);	
}

void CMainFrame::OnDebugStepinto() 
{
    SetMode(modeDebug);
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
        pActivePrj->GetMachine()->SendDebugCmd (CMDDBG_STEP_INTO, 0, 0, NULL);
}

void CMainFrame::OnDebugStepover() 
{
    SetMode(modeDebug);
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
        pActivePrj->GetMachine()->SendDebugCmd (CMDDBG_STEP_OVER, 0, 0, NULL);
}

void CMainFrame::OnDebugStepout() 
{
    SetMode(modeDebug);
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
    	pActivePrj->GetMachine()->SendDebugCmd (CMDDBG_STEP_OUT, 0, 0, NULL);
}

void CMainFrame::OnDebugRuntocursor() 
{
    CString strPathName;
    int nModule;

    CView* pView = GetActiveView();
    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    CLuaDoc* pDoc = ((CLuaView*)pView)->GetDocument();

	if ( !pDoc->IsInProject() )
		return;

	CProjectFile* pPF = pDoc->GetProjectFile();
	strPathName = pPF->GetPathName();
    GetProject()->GetProjectFile (strPathName, &nModule);

	int nLine = ((CLuaView*)pView)->GetEditor()->GetCurrentLine();
	nLine = pPF->GetNearestDebugLine(nLine);

    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
        pActivePrj->GetMachine()->SendDebugCmd (CMDDBG_RUN_TO_CURSOR, nLine, nModule, strPathName);
}

void CMainFrame::OnDebugBreak() 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
    	pActivePrj->GetMachine()->SendDebugCmd (CMDDBG_BREAK, 0, 0, NULL);
}

void CMainFrame::OnDebugStopdebugging()
{
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (m_pCT&&m_pCT->KindOf ()==enEditDebugTemplate)   {
        IAmGoingTo (eCloseDebuggingSession);
        if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))
            bFocusOnEditor=true;
	    m_debug.Stop();
        SetMode(modeBuild);
        SetLayout (m_objLayoutEdit);
        StopDebugging ();
        if (m_wndDockDbgObject.GetView () != NULL)
		m_wndDockDbgObject.GetView ()->ClearData ();
        /* settiamo le toolbar giuste */
        SwitchToolBars ();
        if (bFocusOnEditor)
            pView->GetEditor ()->SetFocus ();
        CloseDialogList ();
        RemoveDialogList ();

        IHaveDoneIt ();
    }
}

void CMainFrame::OnDebugRestart() 
{
    CDialogSlot dlgSlot;
	StopDebugging ();
    if (dlgSlot.DoModal ()==IDOK)   {
        /* mandiamo giu' i dati relativi ai breakpoints */
        SendDebugData (dlgSlot.m_szSlotName);
        /* facciamo partire il debugger */
        StartDebugging (CMDDBG_GO);
    }
}


BOOL CMainFrame::GetCalltip(const char *szWord, CString &strCalltip)
{
	BOOL bFound = m_debug.GetCalltip(szWord, strCalltip.GetBuffer(1000));
	strCalltip.ReleaseBuffer();

	return bFound;
}

void CMainFrame::OnUpdateCmdForProject(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nAppMode != modeNoProject);
}

void CMainFrame::OnBuildExecute() 
{
	theApp.SaveModifiedDocuments();  
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        if (CheckParameters ())   {
	        if ( !pActivePrj->Build() )
		        return;
            CString strProgram=GetProject()->GetDebugPathNameExt();
            pActivePrj->GetMachine()->DownLoad (strProgram);
            pActivePrj->GetMachine()->Start ();
            SetMode (modeExecute);
        }        
    }        
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave it alone!

    CView* pView = GetActiveView();
    UpdateFrameTitleForDocument(NULL);
}

void CMainFrame::UpdateFrameTitleForDocument(LPCTSTR lpszDocName)
{
	// copy first part of title loaded at time of frame creation
	CString strPlantName;
	CString strMachineName;
	TCHAR szText[256+_MAX_PATH];

	if (GetWorkSpace())   {
		strPlantName = GetWorkSpace()->GetName();
		CProject *pPrj = GetWorkSpace()->GetActiveProject ();
		if (pPrj)
			strMachineName = pPrj->GetMachineModule()->GetName();
	}
    szText[0]='\0';
    switch (m_nAppMode)   {
            case modeNoPlantNoProject :
			    lstrcpy(szText, m_strTitle);
                break;
            case modeNoProject :
            case modeBuild :
            case modeExecute :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
                break;
            case modeDebug :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
			    lstrcat(szText, LOADSTRING (IDS_ADAMOMAINFRAME_21));
                break;
            case modeDebugBreak :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
			    lstrcat(szText, LOADSTRING (IDS_ADAMOMAINFRAME_22));
                break;
            case modeLogic :
            case modeConfig :
            case modeHardware :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
                break;
            case modeManDiag :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
			    lstrcat(szText, LOADSTRING (IDS_ADAMOMAINFRAME_23));
                break;
            case modeMachine :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
                break;
            case modeShowErrors :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
                break;
            case modeResources :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
			    lstrcat(szText, LOADSTRING (IDS_ADAMOMAINFRAME_30));
                break;
            case modeDiagnostic :
			    lstrcpy(szText, m_strTitle);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strPlantName);
			    lstrcat(szText, _T(" - "));
			    lstrcat(szText, strMachineName);
                break;
	    }
	// set title if changed, but don't remove completely
	// Note: will be excessive for MDI Frame with maximized child
	AfxSetWindowText(m_hWnd, szText);
}


void CMainFrame::OnHelpContactauthor() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(), "open", "mailto:engineinfo@tiscali.it", NULL, NULL, SW_SHOWNA);	
}

void CMainFrame::OnHelpLuahelppdf() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(), "open", theApp.GetModuleDir() + "\\docs\\refman-4.1-work4.pdf", NULL, NULL, SW_SHOWNA);	
}

void CMainFrame::OnHelpVisithomepage() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(), "open", "http://www.google.com", NULL, NULL, SW_SHOWNA);	
}

void CMainFrame::OnHelpLuahomepage() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(), "open", "http://www.google.com", NULL, NULL, SW_SHOWNA);	
}

void CMainFrame::OnDestroy() 
{
    /* distruggiamo la toolbar principale */
    CXTPCommandBars* pCommandBars = GetCommandBars();
    /* detacciamo il workspace tabbed */
	m_MTIClientWnd.Detach();	
    /* distruggiamo i font del programma */
    _delete (m_pFontTahoma);
    _delete (m_pFontTahomaGrassetto);
    /* distruggiamo i layout */
    _delete (m_objLayoutEmpty);
    _delete (m_objLayoutEdit);
    _delete (m_objLayoutConfig);
    _delete (m_objLayoutDebug);
    _delete (m_objLayoutManDiag);
    _delete (m_objLayoutMachine);
    _delete (m_objLayoutHW);
    _delete (m_objLayoutResources);
    _delete (m_objLayoutHWDiagnostic);
    _delete (m_objLayoutParametric);
    /* chiamiamo la base class */
    CMDIFrameWnd::OnDestroy();
	/* facciamo fuori il timer della main window */
    if (m_nTimer!=-1)   {
        KillTimer (ID_MAINTIMER);
        m_nTimer=-1;
    }
    /* se siamo in find in files chiudiamolo */
    if (m_bSearching)   {
        m_objFinder.StopSearch ();
        while (m_bSearching)
            Sleep (500);
    }
	/* cancelliamo i font */
	delete m_pFontTahoma;
	delete m_pFontTahomaGrassetto;
}

void CMainFrame::OnBuildStop() 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
        pActivePrj->GetMachine()->Stop ();	
    SetMode (modeBuild);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
    CWorkspace* pWorkSpace=GetWorkSpace();
    if (pWorkSpace)   {
        CProject *pActivePrj = GetWorkSpace()->GetActiveProject();
        if (pActivePrj)  {
            CAdamoMachine* pMachine=pActivePrj->GetMachine();
            if (pMachine)
				MotorizzaMacchine ();
            if (m_nDelayTimer>0)   {
                m_nDelayTimer--;
            }
            else
            if (pMachine->IsCncOnline ())   {
                if (WhatIAmGoingToDo ()==eDone)   {
                    /* diamo vita ai templari */
                    if (m_pCT)   {
                        m_pCT->OnTimer ();
                        /* ora diamo vita all'oscilloscopio */
                        if (m_pCT->KindOf ()==enManDiagTemplate)
                            if (CDlgOscilloscope::GetOscilloscope ())
                                CDlgOscilloscope::GetOscilloscope ()->OnAdamoTimer ();
                        /* ora la docking di view dei dispositivi nel debugger */
                        if (m_pCT->KindOf ()==enEditDebugTemplate)   {
                            if (m_wndDockDbgVar.GetView ())
                                m_wndDockDbgVar.GetView ()->OnAdamoTimer ();
                            if (m_wndDockDbgIO.GetView ())
                                m_wndDockDbgIO.GetView ()->OnAdamoTimer ();
                            if (m_wndDockDbgObject.GetView ())
                                m_wndDockDbgObject.GetView ()->OnAdamoTimer ();
                        }
						/* docking dispositivi hw, serve per scan Ethercat */
                        if (m_pCT->KindOf ()==enConfTemplate)   {
                            if (m_wndDockConfigHW.GetView ())
                                m_wndDockConfigHW.GetView ()->OnAdamoTimer ();
						}
                    }
                }
            }
        }
    }
    m_wndStatusBar.OnTimer ();
    m_nTimerCounter++;
    CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::CloseDialogErrors ()
{
    if (m_pErrorsDlg)   {
        m_pErrorsDlg->DestroyWindow ();
        _delete (m_pErrorsDlg);
		m_wndStatusBar.ShowAlarm (false);
    }
}

void CMainFrame::CloseDialogStrings ()
{
    if (m_pStringsDlg)   {
        m_pStringsDlg->DestroyWindow ();
        _delete (m_pStringsDlg);
		m_wndStatusBar.ShowMessage (false);
    }
}

LRESULT CMainFrame::OnThereIsAnError (WPARAM wParam, LPARAM lParam)
{
    stThiraErrore* p=(stThiraErrore*) lParam;

    if (p->nTypeError==TE_TRACE)
        /* gestione dei trace */
        GestioneTrace (p);
    else   {
        if (p->nDeleteError==0)   {
			/* gestione degli errori */
			GestioneErrore (p);
			/* aggiungiamo il messaggio al view degli errori */
			AddSEView (p);
        }
        else
			/* Gestione della cancellazione dell'errore */
			GestioneCancellazioneErrore (p);
    }
    return 0;
}

LRESULT CMainFrame::OpenDialogStrings ()
{
    CWorkspace *pWrs=GETWS();
    if (pWrs)   {
        CProject *pActivePrj = GETWS()->GetActiveProject();
        if (pActivePrj)   {
            CAdamoSink* pSink=pActivePrj->GetMachine()->GetSink ();
            if (pSink)   {
                CPtrList& rList=pSink->GetDBStrings();
                if (!m_pStringsDlg)   {
                    m_pStringsDlg= new CDlgStrings (this);
                    m_pStringsDlg->SetData (&rList);
                    m_pStringsDlg->Create (IDD_LISTSTRINGS, this);
                    m_pStringsDlg->CenterWindow ();
                    m_pStringsDlg->FillList ();
                }
                m_pStringsDlg->SetForegroundWindow();
            }        
        }
    }
	return 0;
}

LRESULT CMainFrame::OpenDialogErrors ()
{
    CWorkspace *pWrs=GETWS();
    if (pWrs)   {
        CProject *pActivePrj = GETWS()->GetActiveProject();
        if (pActivePrj)   {
            CAdamoSink* pSink=pActivePrj->GetMachine()->GetSink ();
            if (pSink)   {
                CPtrList& rList=pSink->GetDBErrors();
                if (!m_pErrorsDlg)   {
                    m_pErrorsDlg= new CErrorsDlg (this);
                    m_pErrorsDlg->SetData (&rList);
                    m_pErrorsDlg->Create (IDD_LISTERRORS, this);
                    m_pErrorsDlg->CenterWindow ();
                }
                m_pErrorsDlg->Refresh ();
                m_pErrorsDlg->SetForegroundWindow();
            }        
        }
    }
	return 0;
}

LRESULT CMainFrame::OnOpenDialogBars (WPARAM wParam, LPARAM lParam)
{
    if (wParam == ID_STATIC_STATIC)
		OpenDialogErrors ();
	else
    if (wParam == ID_STATIC_STRINGS)
		OpenDialogStrings ();
    return 0;
}

LRESULT CMainFrame::OnCloseDialogErrors (WPARAM wParam, LPARAM lParam)
{
    CloseDialogErrors ();
    return 0;
}

LRESULT CMainFrame::OnCloseDialogStrings (WPARAM wParam, LPARAM lParam)
{
    CloseDialogStrings ();
    return 0;
}

void CMainFrame::SendDebugData (const char* szSlotName)
{
    int n=0;
    CString strProgram=GetProject()->GetDebugPathNameExt();
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        pActivePrj->GetMachine()->DownLoad (strProgram);
        pActivePrj->GetMachine()->ClearDebugData ();
        pActivePrj->GetMachine()->InitDebugData (szSlotName);
        CProjectFile* pPrj=GetProject()->GetProjectFile (0);
        while (pPrj)   {
            pActivePrj->GetMachine()->SendDebugData (pPrj->GetPathName(), pPrj->GetBreakPoints ());
            pPrj=GetProject()->GetProjectFile (++n);
        }
    }
}

void CMainFrame::StartDebugging (int nCmd)
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
		ClearGotoLines ();
        pActivePrj->GetMachine()->StartDebugging (nCmd);
        pActivePrj->GetMachine()->Start ();
    }
}

void CMainFrame::StartDebugging (int nCmd, int nParameter, int nModule, const char* szModule)
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
		ClearGotoLines ();
        pActivePrj->GetMachine()->StartDebugging (nCmd, nParameter, nModule, szModule);
        pActivePrj->GetMachine()->Start ();
    }
}

void CMainFrame::StopDebugging ()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        pActivePrj->GetMachine()->StopDebugging ();
        pActivePrj->GetMachine()->Stop ();
    }
}

void CMainFrame::OnEditFind() 
{
    if (!m_pFindDlg)   {
        CView *pView = NULL;
        CMDIChildWnd *pChild = MDIGetActive();
        if (pChild != NULL)  {
            pView = pChild->GetActiveView();
            if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))   {
                m_pFindDlg = new CFindDlg(this);
                if (m_pFindDlg != NULL)   {
                    m_pFindDlg->Create(CFindDlg::IDD, pView);
                    m_pFindDlg->m_pEdit = ((CLuaView*)pView)->GetEditor();
                    m_pFindDlg->ShowWindow(SW_SHOW);
                    m_pFindDlg->InitDialog();
                }
            }
        }
    }
}

void CMainFrame::OnEditReplace() 
{
	if (!m_pReplaceDlg)   {
		CView *pView = NULL;
		CMDIChildWnd *pChild = MDIGetActive();
		if (pChild != NULL)   {
			pView = pChild->GetActiveView();
			if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))   {
				m_pReplaceDlg = new CReplaceDlg(this);
				if (m_pReplaceDlg != NULL)  {
					m_pReplaceDlg->Create(CReplaceDlg::IDD);
					m_pReplaceDlg->m_pEdit = ((CLuaView*)pView)->GetEditor();
					m_pReplaceDlg->ShowWindow(SW_SHOW);
					m_pReplaceDlg->InitDialog();
				}
			}
		}
	}
}

void CMainFrame::OnDebugViewslots() 
{
	// TODO: Add your command handler code here
	CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if (pActivePrj)  {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())   {
            DelayAdamoTimer (1);
            CDlgSlotView vs;
            vs.SetCurrentMachine (GetProject()->GetMachine());
            vs.DoModal ();	
        }
    }
}

void CMainFrame::OnBuildViewVar() 
{
    ShowGlobalData ();
}

void CMainFrame::OnDebugViewVar() 
{
    ShowGlobalData ();
}

void CMainFrame::OnDbgBreakViewVar() 
{
    ShowGlobalData ();
}


void CMainFrame::OnBuildViewMbx() 
{
    ShowMbx ();
}

void CMainFrame::OnDebugViewMbx() 
{
    ShowMbx ();
}

void CMainFrame::OnDbgBreakViewMbx() 
{
    ShowMbx ();
}

void CMainFrame::ShowGlobalData ()
{
    if (GetProject()->GetMachine()->IsCncRunning ())   {
        /* prendiamo la parola dell'editor dove si trova il cursore */
        CString strEditWord=GetEditorWord ();
        /* carichiamo i dati relativi al simbolo cercato */
        if (!strEditWord.IsEmpty ())   {
            struct stGlobalDescriptor* pGD=GetProject ()->GetVariableContainer ().Get (strEditWord);
            /* visualizziamo la dialog */
            if (pGD && pGD->nType != GLOBALOBJECT)   {
                switch (pGD->nType)   {
                    case GLOBALVARIABLE :
                        /* variabile globale semplice */
                        ShowGlobalVariable (pGD);
                        break;
                    case GLOBALARRAY :
                        /* array */
                        ShowGlobalArray (pGD);
                        break;
                    case GLOBALMATRIX :
                        /* matrice */
                        ShowGlobalMatrix (pGD);
                        break;
                }
            }
            else   {
                /* vediamo se la variabile non e' stata dichiarata */
                CAdamoMachine* pMachine=GetProject()->GetMachine();
                if (pMachine)   {
                    int nLevel, nGL, nType, nProg;
                    nLevel = CDebugger::GetDebugger()->GetStackTraceLevel();
                    if (pMachine->ExistSymbol (strEditWord, nLevel, &nGL, &nType, &nProg))
                        /* esiste un simbolo, andiamo avanti */
                        if (nType==LUA_TNUMBER||nType==LUA_TBOOLEAN||nType==LUA_TSTRING||nType==LUA_TNIL)
                            ShowGlobalOrLocalVariable (strEditWord, nType, nGL, nLevel, nProg);
                        else
                            if (nType==LUA_TTABLE)
                                ShowGlobalOrLocalTable (strEditWord, nGL, nLevel, nProg);
                            else;
                    else   {
						/*
                        COleVariant v;
                        nGL=2;
                        if (pMachine->GetExpressionVariable (strEditWord, nLevel, v)==S_OK)    {
                            switch (v.vt)   {
                            case VT_R8 :
                                nType=LUA_TNUMBER;
                                break;
                            case VT_BSTR :
                                nType=LUA_TSTRING;
                                break;
                            case VT_I4 :
                                nType=LUA_TBOOLEAN;
                                break;
                            }
                            ShowGlobalOrLocalVariable (strEditWord, nType, nGL, nLevel, 0);
                        }
						*/
						;
                    }
                }
            }
            FocusOnEditor ();
        }
    }
}

void CMainFrame::ShowMbx ()
{
    CAdamoMachine* pMachine=GetProject()->GetMachine();
    int nLevel, nGL, nType, nProg;
    if (pMachine && pMachine->IsCncRunning ())   {
        /* prendiamo la parola dell'editor dove si trova il cursore */
        CString strEditWord=GetEditorWord ();
        /* carichiamo i dati relativi al simbolo cercato */
        nLevel = CDebugger::GetDebugger()->GetStackTraceLevel();
        if (pMachine->ExistSymbol (strEditWord, nLevel, &nGL, &nType, &nProg))
            /* esiste un simbolo, andiamo avanti */
			if (nType==LUA_TNUMBER)   {
				CDlgMbx dlg;
				dlg.SetMachine (GetProject()->GetMachine());
				dlg.SetVariableName (strEditWord);
				dlg.SetGL (nGL);
				dlg.SetLocalData (nLevel, nProg);
				dlg.DoModal ();
			}
	}
}

CString CMainFrame::GetEditorWord ()
{
    CString str;
    CView *pView = NULL;
    /* prendiamo la finestra MDI attiva */
    CMDIChildWnd *pChild = MDIGetActive();
    if (pChild != NULL)   {
        /* prendiamo il view attivo */
        pView = pChild->GetActiveView();
        if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))   {
            str=((CScintillaView*)pView)->GetEditor ()->GetSelectedText ();
            if (str=="")
                str=((CScintillaView*)pView)->GetEditor ()->GetCaretText ();
        }
    }
    return str;
}

void CMainFrame::ShowGlobalVariable (struct stGlobalDescriptor* pGD)
{
    if (!ExistDialogList (pGD->strName))   {
        if (CountDialogList ()<NMAX_DIALOGLIST)   {
	        CGlobalVariable *dv = new CGlobalVariable;
            dv->SetMachine (GetProject()->GetMachine());
            dv->SetData ((const char*)pGD->strName, (const char*)pGD->strName, (const char*)pGD->strName, pGD->nVarType);
            dv->Create (IDD_GLOBALVARIABLE, this);
            dv->ShowWindow (SW_SHOWNORMAL);
            dv->CenterWindow ();
            MoveDialogList (dv);
            AddDialogList (dv);
        }
    }
}

void CMainFrame::ShowGlobalArray (struct stGlobalDescriptor* pGD)
{
    if (!ExistDialogList (pGD->strName))   {
        if (CountDialogList ()<NMAX_DIALOGLIST)   {
	        CGlobalArr *dv = new CGlobalArr;
            dv->SetMachine (GetProject()->GetMachine());
            dv->SetData ((const char*)pGD->strName, pGD->arr.nArrType, pGD->arr.nArrElements);
            dv->Create (IDD_GLOBALARRAY, this);
            dv->ShowWindow (SW_SHOWNORMAL);
            dv->CenterWindow ();
            MoveDialogList (dv);
            AddDialogList (dv);
        }
    }
}

void CMainFrame::ShowGlobalMatrix (struct stGlobalDescriptor* pGD)
{
    if (!ExistDialogList (pGD->strName))   {
        if (CountDialogList ()<NMAX_DIALOGLIST)   {
            stColumnsData* cd = new stColumnsData [pGD->mat.nCols];

            for (int n=0; n<pGD->mat.nCols; n++)   {
                cd[n].nType=pGD->mat.anTypes[n];
                strcpy (cd[n].szName, pGD->mat.strColumnName[n]);
            }
	        CGlobalMatrix *dv=new CGlobalMatrix;

            dv->SetMachine (GetProject()->GetMachine());
            dv->SetData ((const char*)pGD->strName, pGD->mat.nRows, pGD->mat.nCols, cd);
            dv->Create (IDD_GLOBALMATRIX, this);
            dv->ShowWindow (SW_SHOWNORMAL);
            dv->CenterWindow ();
            MoveDialogList (dv);
            AddDialogList (dv);
            delete cd;
        }
    }
}

void CMainFrame::ShowGlobalOrLocalVariable (CString strName, int nType, int nGL, int nLevel, int nProg)
{
	CGlobalVariable dv;

    dv.SetMachine (GetProject()->GetMachine());
    dv.SetData ((const char*)strName, (const char*)strName, (const char*)strName, nType, true);
    dv.SetGL (nGL);
    dv.SetLocalData (nLevel, nProg);
    dv.DoModal ();
}

void CMainFrame::ShowGlobalOrLocalTable (CString strName, int nGL, int nLevel, int nProg)
{
	CAdamoTables at;

    at.SetData ((const char*)strName, (const char*)strName);
    at.SetGL (nGL);
    at.SetLocalData (nLevel, nProg);
    at.DoModal ();
}

void CMainFrame::OnFileAddproject() 
{	
	if ( GetWorkSpace()->Add() )
	{
		SetMode(modeBuild);
		OnUpdateFrameTitle(TRUE);
	}	
}

void CMainFrame::UpdateActiveProject()
{
	OnUpdateFrameTitle(TRUE);    
}

void CMainFrame::LoadDockingProject (CProject* pProject)
{
	CHelpOnLineView* pHelpOnlineView = m_wndHelpOnLine.GetView ();
	if (pHelpOnlineView)
		pHelpOnlineView->LoadTree (pProject);
	CThiraLuaObjectsView* pThiraLuaObjectsView = m_wndThiraLuaObjects.GetView ();
	if (pThiraLuaObjectsView)
		pThiraLuaObjectsView->LoadTree (pProject);
}

void CMainFrame::UpdateDockingProject (CProject* pProject)
{
	CHelpOnLineView* pHelpOnlineView = m_wndHelpOnLine.GetView ();
	if (pHelpOnlineView)
		pHelpOnlineView->UpdateTree (pProject);
	CThiraLuaObjectsView* pThiraLuaObjectsView = m_wndThiraLuaObjects.GetView ();
	if (pThiraLuaObjectsView)
		pThiraLuaObjectsView->UpdateTree (pProject);
}

void CMainFrame::DeleteHelpOnLineCustomFunction (CString strModule, CString strFunction)
{
	CHelpOnLineView* pHelpOnlineView = m_wndHelpOnLine.GetView ();
	if (pHelpOnlineView)
		pHelpOnlineView->DeleteCustomFunction (strModule, strFunction);
}

void CMainFrame::OnFileOpenplant() 
{
	if ( GetWorkSpace()->Load() )
	{
        ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetPlant(GetWorkSpace()->GetPtrPlant());
		SetMode(modeBuild);
		OnUpdateFrameTitle(TRUE);
	}
}

void CMainFrame::OnFileSaveplant() 
{
	GetWorkSpace()->Save();	
}


void CMainFrame::OnUpdateCmdForPlant(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nAppMode != modeNoPlantNoProject);	
}

void CMainFrame::OnFileCloseplant() 
{
	if ( GetWorkSpace()->Close() )
	{
        GetWorkSpace()->RemoveProject ();
        ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetPlant(NULL);
		SetMode(modeNoPlantNoProject);
        GetWorkSpace()->SetModified(FALSE);
		OnUpdateFrameTitle(TRUE);
    }		
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
    static int n=0;
    if (GetTemplate()==enEditDebugTemplate)   {
        if (pMsg->message==WM_KEYDOWN)   {
            if (pMsg->wParam==VK_CONTROL)   {
                if (m_lViews.GetCount ()>1)
                    m_pCtrlView=(CLuaView*)m_pCT->GetActiveView ();
                m_bCtrlPressed=true;
                m_nTabCount=0;
            }
            else
                if (pMsg->wParam==VK_TAB)   {
                    if (m_bCtrlPressed&&m_pCtrlView)   {
                        CLuaView* v=(CLuaView*)m_pCT->GetActiveView ();
                        v=GetPrevEditView (v);
                        if (v)   {
                            v->Activate ();
                            m_nTabCount++;
                        }
                        return TRUE;
                    }
                    else
                        return FALSE;
                }
                else
                    m_nTabCount=0;
        }
        else   {
            if (pMsg->message==WM_KEYUP)   {
                if (pMsg->wParam==VK_CONTROL)   {
                    if (m_pCtrlView&&m_nTabCount>0)   {
                        CLuaView* v=(CLuaView*)m_pCT->GetActiveView ();
                        if (v!=m_pCtrlView)   {
                            RemoveEditView (v->GetDocument ()->GetViewPosition ());
                            v->GetDocument ()->SetViewPosition (AddEditView (m_pCtrlView, v));
                        }
                    }
                    m_bCtrlPressed=false;
                    m_pCtrlView=NULL;
                }
            }
        }
    }
    if (pMsg->message==WM_KEYDOWN)   {
        int nKeyState = GetKeyState (VK_CONTROL);
		int nShiftState = GetKeyState (VK_SHIFT);
        if (nKeyState&0x8000)   {
            int nB=FALSE;
            switch (pMsg->wParam)   {
                case 49 :
					OnOpenPswConfig ();
                    nB=TRUE;
                    break;
                case 50 :
					OnOpenPswEditDebug ();
                    nB=TRUE;
                    break;
                case 51 :
					OnOpenPswManDiag ();
                    nB=TRUE;
                    break;
                case 52 :
                    OnOpenPswDiagnostic ();
                        nB=TRUE;
                    break;
                case 53 :
                    OnOpenPswResource ();
                        nB=TRUE;
                    break;
                case 54 :
                    OnOpenPswShowErrors ();
                    nB=TRUE;
                    break;
                case 70 :
                case 102 :
					if (GetTemplate () == enEditDebugTemplate)   {
						if (nShiftState&0x8000)
							OnEditFindInFiles ();
						else
							OnEditFind ();
						nB=TRUE;
					}
					break;
                case 72 :
                case 104 :
					if (GetTemplate () == enEditDebugTemplate)   {
						OnEditReplace ();
						nB=TRUE;
					}
					break;
				case 123 :
					if (GetTemplate () == enEditDebugTemplate)   {
						OnGotoReference ();
						nB=TRUE;
					}
					break;
            }
            if (nB)
                return nB;
        }
		else
			if (pMsg->wParam == 123)   {
				if (GetTemplate () == enEditDebugTemplate)   {
					OnGotoDefinition ();
					return TRUE;
				}
			}
    }
	return CXTPMDIFrameWnd::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CXTPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
    if (wParam == XTP_DPN_SHOWWINDOW)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
        
        if (!pPane->IsValid())
        {
            switch (pPane->GetID())
            {
            case ID_VIEW_WORKSPACE:
                pPane->Attach(m_wndWorkspace.GetTreeViewFiles());
                break;
			case ID_VIEW_OUTPUT:   {
					CScintillaView *pView=m_wndOutput.GetSciView();
					if (pView)
						pPane->Attach(pView);
				}			
                break;
            case ID_VIEW_CALLSTACK:
                pPane->Attach(m_wndCallStack.GetSciView());
                break;
            case ID_VIEW_LOCALS:
                pPane->Attach(m_wndLocals.GetVarLocView());                    
                break;
            case ID_VIEW_WATCHES:
                pPane->Attach(m_wndWatches.GetWatchListView());
                break;
            case ID_VIEW_PROP_DEVICE:
                pPane->Attach(m_wndDockDevProperty.GetView());
                break;
            case ID_VIEW_DEVICE:
                pPane->Attach(m_wndDockDev.GetView());
                break;
            case ID_VIEW_DEVTREE:
                pPane->Attach(m_wndDockConfigTree.GetView());
                break;
			case ID_VIEW_OSCILLOSCOPE :   {
					COscView *pView=m_wndDockOscilloscope.GetView();
					if (pView)
						pPane->Attach(pView);
				}
                break;
            case ID_VIEW_DBGIO :
                pPane->Attach(m_wndDockDbgIO.GetView());
                break;
            case ID_VIEW_DBGVAR :
                pPane->Attach(m_wndDockDbgVar.GetView());
                break;
            case ID_VIEW_EDITLANGUAGES :
                pPane->Attach(m_wndDockLangView.GetView());
                break;
            case ID_VIEW_FFV :
                pPane->Attach(m_wndFFV.GetSciView());
                break;
            case ID_VIEW_RS_CATALOGUE :
                pPane->Attach(m_wndDockRSCatalogue.GetView());
                break;
            case ID_VIEW_RS_PROPERTIES :
                pPane->Attach(m_wndDockRSProperties.GetView());
                break;
            case ID_VIEW_DG_SDO :
                pPane->Attach(m_wndDockDGSdo.GetView());
                break;
            case ID_VIEW_EC_MAILBOX :
                pPane->Attach(m_wndDockEcMailBox.GetView());
                break;
            case ID_VIEW_DEBUG:
                pPane->Attach(m_wndDebug.GetSciView());
                break;
            case ID_VIEW_HW:
                pPane->Attach(m_wndDockConfigHW.GetView());
                break;
            case ID_VIEW_RS_ACTIVEX_PREVIEW :
                pPane->Attach(m_wndDockRSActiveXPreview.GetView());
                break;
            case ID_VIEW_EDITREFERENCES :
                pPane->Attach(m_wndReferences.GetSciView());
                break;
            case ID_VIEW_HELP_ON_LINE :
                pPane->Attach(m_wndHelpOnLine.GetView());
				break;
            case ID_VIEW_HELP_ON_LINE_HTML :
                pPane->Attach(m_wndHelpOnLineHtml.GetView());
				break;
            case ID_VIEW_THIRA_LUA_OBJECTS :
                pPane->Attach(m_wndThiraLuaObjects.GetView());
				break;
            case ID_VIEW_OBJECTS :
                pPane->Attach(m_wndDockDbgObject.GetView());
                break;
			case ID_VIEW_ALL_ERRORS :
				pPane->Attach(m_wndDockListAllErrors.GetView());
				break;
			case ID_VIEW_INT_ERRORS :
				pPane->Attach(m_wndDockListInterpreterErrors.GetView());
				break;
			case ID_VIEW_PLC_ERRORS :
				pPane->Attach(m_wndDockListPlcErrors.GetView());
				break;
			case ID_VIEW_ISO_ERRORS :
				pPane->Attach(m_wndDockListISOErrors.GetView());
				break;
			case ID_VIEW_HARDWARE_ERRORS :
				pPane->Attach(m_wndDockListHardwareErrors.GetView());
				break;
			case ID_VIEW_MESSAGES :
				pPane->Attach(m_wndDockListMessages.GetView());
				break;
            }
        }
        return TRUE;
    }

    return FALSE;
}

BOOL CMainFrame::ShowWindowEx(int nCmdShow)
{
	ASSERT_VALID(this);

	// Restore frame window size and position.
	m_wndPosition.LoadWindowPos(this);
	nCmdShow = m_wndPosition.showCmd;

	return ShowWindow(nCmdShow);
}

void CMainFrame::SwitchMenu ()
{
    CXTPCommandBar* pMenuBar = GetCommandBars()->GetMenuBar();
    if (pMenuBar == NULL) return;
    pMenuBar->SetSelected(-1);
    pMenuBar->GetControls()->RemoveAll();
    switch (m_nAppMode)  {
        case modeNoPlantNoProject :
        case modeNoProject :
            pMenuBar->SetTitle("Main Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MAINFRAME);
            break;
        case modeConfig :
            pMenuBar->SetTitle("Config Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MENU_CONFIG);
            break;
        case modeManDiag :
            pMenuBar->SetTitle("ManDiag Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MENU_MANDIAG);
            break;
        case modeMachine:
            pMenuBar->SetTitle("Machine Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MENU_MACHINE);
            break;
        case modeDebug :
        case modeDebugBreak :
            pMenuBar->SetTitle("Editor Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_DEBUG_MENU);
            break;    
        case modeExecute :
        case modeBuild :
            pMenuBar->SetTitle("Build Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_IDE2TYPE);
            break;
        case modeShowErrors:
            pMenuBar->SetTitle("Show Errors Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MENU_SHOWERRORS);
            break;
        case modeResources:
            pMenuBar->SetTitle("Resources Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MENU_RESOURCES);
            break;
        case modeDiagnostic:
            pMenuBar->SetTitle("Diagnostic Menu");
            pMenuBar->SetFlags(xtpFlagHideMDIButtons ,xtpFlagAddMDISysPopup);
            ((CXTPMenuBar*) pMenuBar)->LoadMenuBar(IDR_MENU_DIAGNOSTIC);
            break;
    }
}

void CMainFrame::OnViewDevice() 
{
	m_paneManager.ShowPane(ID_VIEW_DEVICE);	
}

void CMainFrame::OnViewProDevice() 
{
	m_paneManager.ShowPane(ID_VIEW_PROP_DEVICE);	
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());
	
	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);
	
	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME, TRUE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();
	
	cs.DoModal();
}

int CMainFrame::OnSci(CWnd *pView, SCNotification *pNotify)
{
    COutputView* pOutputView = (COutputView*)m_wndOutput.GetSciView();

	if ( pView==m_wndOutput.GetSciView())
		return m_wndOutput.OnSci(pView, pNotify);
	else if ( pView==m_wndCallStack.GetSciView() )
        return m_wndCallStack.OnSci(pView,pNotify);
	else if ( pView==m_wndFFV.GetSciView() )
        return m_wndFFV.OnSci(pView,pNotify);
	else if ( pView==m_wndDebug.GetSciView() )
        return m_wndDebug.OnSci(pView,pNotify);
	else if ( pView==m_wndReferences.GetSciView() )
        return m_wndReferences.OnSci(pView,pNotify);

    return 0;
}

void CMainFrame::OnUpdateFindReplace(CCmdUI* pCmdUI) 
{
    CView* pView = GetActiveView();
	
    if ( pView && (pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
		pCmdUI->Enable(TRUE);
    else
		pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateFile(CCmdUI* pCmdUI) 
{
    if (m_nAppMode == modeNoPlantNoProject || m_nAppMode == modeConfig )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMainFrame::OnButtonError()
{
    OpenDialogErrors ();
}

void CMainFrame::OnButtonString() 
{
    OpenDialogStrings ();
}

void CMainFrame::OnButtonDeleteErrorBar() 
{
}


void CMainFrame::ClearClipBoard()
{
    OpenClipboard();
    EmptyClipboard();
    CloseClipboard();
}

void CMainFrame::AddErrorBar (const char* sz, int nTypeError)
{
}

void CMainFrame::AddStringBar (const char* sz)
{
}

void CMainFrame::DeleteStringBar ()
{
}

void CMainFrame::GetErrorString (char* szSource, char** szDest)
{
    char* p=strchr (szSource, '@'), *q, *r;
    if (p)   {
        q=p+1;
        r=strchr (q, '@');
        if (r)   {
            p=r+1;
            *szDest=p;
        }            
        else
            *szDest=q;    
    }
    else
        *szDest=szSource;
}

void CMainFrame::OnViewRestoredocking() 
{    
    if(!m_pCT)
        return;
    int nMode=GetMode();
    switch(m_pCT->KindOf ()){
        case enConfTemplate :
            if (nMode == modeConfig || nMode==modeLogic) {
                if(m_paneManager.IsPaneClosed(ID_VIEW_PROP_DEVICE))
                    m_paneManager.ShowPane(ID_VIEW_PROP_DEVICE);
    
                if(m_paneManager.IsPaneClosed(ID_VIEW_DEVICE))
                    m_paneManager.ShowPane(ID_VIEW_DEVICE);

                m_paneManager.DockPane(m_paneManager.FindPane(ID_VIEW_PROP_DEVICE),xtpPaneDockLeft);
                m_paneManager.DockPane(m_paneManager.FindPane(ID_VIEW_DEVICE),xtpPaneDockTop,
                                                        m_paneManager.FindPane(ID_VIEW_PROP_DEVICE));
            }
            else 
            if (nMode == modeHardware)    {
                if(m_paneManager.IsPaneClosed(ID_VIEW_DEVTREE))
                    m_paneManager.ShowPane(ID_VIEW_DEVTREE);
    
                if(m_paneManager.IsPaneClosed(ID_VIEW_HW))
                    m_paneManager.ShowPane(ID_VIEW_HW);

                m_paneManager.DockPane(m_paneManager.FindPane(ID_VIEW_HW),xtpPaneDockLeft);
                m_paneManager.DockPane(m_paneManager.FindPane(ID_VIEW_DEVTREE),xtpPaneDockTop,
                                                        m_paneManager.FindPane(ID_VIEW_HW));
            }
            break;
        case enManDiagTemplate :
			if (m_ePswLevel < ePswService)   {
            if(m_paneManager.IsPaneClosed(ID_VIEW_OSCILLOSCOPE))
                m_paneManager.ShowPane(ID_VIEW_OSCILLOSCOPE);
            m_paneManager.DockPane(m_paneManager.FindPane(ID_VIEW_OSCILLOSCOPE),xtpPaneDockBottom);
			}
            break;
        case enEditDebugTemplate :
            if(m_nAppMode == modeNoPlantNoProject
                || m_nAppMode == modeNoProject
                || m_nAppMode == modeBuild
                || m_nAppMode == modeExecute)
                {

                if(m_paneManager.IsPaneClosed(ID_VIEW_WORKSPACE))
                    m_paneManager.ShowPane(ID_VIEW_WORKSPACE);
    
                if(m_paneManager.IsPaneClosed(ID_VIEW_OUTPUT))
                    m_paneManager.ShowPane(ID_VIEW_OUTPUT);
    
                if(m_paneManager.IsPaneClosed(ID_VIEW_DEVTREE))
                    m_paneManager.ShowPane(ID_VIEW_DEVTREE);

                if(m_paneManager.IsPaneClosed(ID_VIEW_FFV))
                    m_paneManager.ShowPane(ID_VIEW_FFV);

                if(m_paneManager.IsPaneClosed(ID_VIEW_EDITLANGUAGES))
                    m_paneManager.ShowPane(ID_VIEW_EDITLANGUAGES);

                if(m_paneManager.IsPaneClosed(ID_VIEW_EDITREFERENCES))
                    m_paneManager.ShowPane(ID_VIEW_EDITREFERENCES);

				if(m_paneManager.IsPaneClosed(ID_VIEW_HELP_ON_LINE))
                    m_paneManager.ShowPane(ID_VIEW_HELP_ON_LINE);

				if(m_paneManager.IsPaneClosed(ID_VIEW_HELP_ON_LINE_HTML))
                    m_paneManager.ShowPane(ID_VIEW_HELP_ON_LINE_HTML);

				if(m_paneManager.IsPaneClosed(ID_VIEW_THIRA_LUA_OBJECTS))
                    m_paneManager.ShowPane(ID_VIEW_THIRA_LUA_OBJECTS);

				if(m_paneManager.IsPaneClosed(ID_VIEW_ALL_ERRORS))
                    m_paneManager.ShowPane(ID_VIEW_ALL_ERRORS);

				if(m_paneManager.IsPaneClosed(ID_VIEW_INT_ERRORS))
                    m_paneManager.ShowPane(ID_VIEW_INT_ERRORS);

				if(m_paneManager.IsPaneClosed(ID_VIEW_PLC_ERRORS))
                    m_paneManager.ShowPane(ID_VIEW_PLC_ERRORS);

				if(m_paneManager.IsPaneClosed(ID_VIEW_ISO_ERRORS))
                    m_paneManager.ShowPane(ID_VIEW_ISO_ERRORS);

				if(m_paneManager.IsPaneClosed(ID_VIEW_HARDWARE_ERRORS))
                    m_paneManager.ShowPane(ID_VIEW_HARDWARE_ERRORS);

				if(m_paneManager.IsPaneClosed(ID_VIEW_MESSAGES))
                    m_paneManager.ShowPane(ID_VIEW_MESSAGES);

				CXTPDockingPane *pWorkSpacePane, *pDevTreePane, *pOutputPane, *pFFPane, *pEditLangPane, *pReferencePane, *pHelpOnLine, *pHelpOnLineHtml, *pThiraLuaObjects, *pAllErrors, *pIntErrors, *pPlcErrors, *pISOErrors, *pHardwareErrors, *pMessages;
                pWorkSpacePane=m_paneManager.FindPane(ID_VIEW_WORKSPACE);
                pDevTreePane=m_paneManager.FindPane(ID_VIEW_DEVTREE);
                pOutputPane=m_paneManager.FindPane(ID_VIEW_OUTPUT);
                pFFPane=m_paneManager.FindPane(ID_VIEW_FFV);
                pEditLangPane=m_paneManager.FindPane(ID_VIEW_EDITLANGUAGES);
                pReferencePane=m_paneManager.FindPane(ID_VIEW_EDITREFERENCES);
				pHelpOnLine=m_paneManager.FindPane(ID_VIEW_HELP_ON_LINE);
				pHelpOnLineHtml=m_paneManager.FindPane(ID_VIEW_HELP_ON_LINE_HTML);
				pThiraLuaObjects=m_paneManager.FindPane(ID_VIEW_THIRA_LUA_OBJECTS);
				pAllErrors =m_paneManager.FindPane(ID_VIEW_ALL_ERRORS);
				pIntErrors =m_paneManager.FindPane(ID_VIEW_INT_ERRORS);
				pPlcErrors =m_paneManager.FindPane(ID_VIEW_PLC_ERRORS);
				pISOErrors =m_paneManager.FindPane(ID_VIEW_ISO_ERRORS);
				pHardwareErrors =m_paneManager.FindPane(ID_VIEW_HARDWARE_ERRORS);
				pMessages =m_paneManager.FindPane(ID_VIEW_MESSAGES);
                m_paneManager.DockPane(pWorkSpacePane,xtpPaneDockLeft);
                m_paneManager.DockPane(pDevTreePane,xtpPaneDockLeft);
                m_paneManager.AttachPane (pDevTreePane, pWorkSpacePane);
                m_paneManager.DockPane(pOutputPane,xtpPaneDockBottom);
                m_paneManager.DockPane(pFFPane,xtpPaneDockBottom);
                m_paneManager.DockPane(pEditLangPane,xtpPaneDockBottom);
				if (pReferencePane)
                m_paneManager.DockPane(pReferencePane,xtpPaneDockBottom);
				if (pHelpOnLineHtml)
					m_paneManager.DockPane(pReferencePane,xtpPaneDockBottom);
				if (pHelpOnLine)
					m_paneManager.DockPane(pHelpOnLine,xtpPaneDockBottom);
				if (pThiraLuaObjects)
					m_paneManager.DockPane(pThiraLuaObjects,xtpPaneDockRight);
				if (pEditLangPane)
                m_paneManager.AttachPane (pEditLangPane, pOutputPane);
				if (pFFPane)
                m_paneManager.AttachPane (pFFPane, pOutputPane);
				if (pReferencePane)
					m_paneManager.AttachPane (pReferencePane, pOutputPane);
				if (pHelpOnLineHtml)
					m_paneManager.AttachPane (pHelpOnLineHtml,pOutputPane);
				if (pAllErrors)
					m_paneManager.DockPane(pAllErrors,xtpPaneDockBottom);
				if (pIntErrors)
					m_paneManager.AttachPane (pIntErrors, pAllErrors);
				if (pPlcErrors)
					m_paneManager.AttachPane (pPlcErrors, pAllErrors);
				if (pISOErrors)
					m_paneManager.AttachPane (pISOErrors, pAllErrors);
				if (pHardwareErrors)
					m_paneManager.AttachPane (pHardwareErrors, pAllErrors);
				if (pHardwareErrors)
					m_paneManager.AttachPane (pMessages, pAllErrors);

            }else if(m_nAppMode == modeDebug
                || m_nAppMode == modeDebugBreak){

                if(m_paneManager.IsPaneClosed(ID_VIEW_DEBUG))
                    m_paneManager.ShowPane(ID_VIEW_DEBUG);
    
                if(m_paneManager.IsPaneClosed(ID_VIEW_LOCALS))
                    m_paneManager.ShowPane(ID_VIEW_LOCALS);
    
                if(m_paneManager.IsPaneClosed(ID_VIEW_CALLSTACK))
                    m_paneManager.ShowPane(ID_VIEW_CALLSTACK);

                if(m_paneManager.IsPaneClosed(ID_VIEW_WATCHES))
                    m_paneManager.ShowPane(ID_VIEW_WATCHES);
                
                if(m_paneManager.IsPaneClosed(ID_VIEW_DBGIO))
                    m_paneManager.ShowPane(ID_VIEW_DBGIO);

                if(m_paneManager.IsPaneClosed(ID_VIEW_DBGVAR))
                    m_paneManager.ShowPane(ID_VIEW_DBGVAR);

                if(m_paneManager.IsPaneClosed(ID_VIEW_FFV))
                    m_paneManager.ShowPane(ID_VIEW_FFV);

                if(m_paneManager.IsPaneClosed(ID_VIEW_OBJECTS))
                    m_paneManager.ShowPane(ID_VIEW_OBJECTS);

				CXTPDockingPane *pDebugPane, *pCallStackPane, *pLocalsPane, *pWatchesPane, *pDebugIOPane, *pDebugVarPane, *pFFPane, *pReferencePane, *pObjectsPane;
                pDebugPane=m_paneManager.FindPane(ID_VIEW_DEBUG);
                pCallStackPane=m_paneManager.FindPane(ID_VIEW_LOCALS);
                pLocalsPane=m_paneManager.FindPane(ID_VIEW_CALLSTACK);
                pWatchesPane=m_paneManager.FindPane(ID_VIEW_WATCHES);
                pDebugIOPane=m_paneManager.FindPane(ID_VIEW_DBGIO);
                pDebugVarPane=m_paneManager.FindPane(ID_VIEW_DBGVAR);
                pFFPane=m_paneManager.FindPane(ID_VIEW_FFV);
				pReferencePane=m_paneManager.FindPane(ID_VIEW_EDITREFERENCES);
				pObjectsPane=m_paneManager.FindPane(ID_VIEW_OBJECTS);
				if (pFFPane)
                m_paneManager.AttachPane (pFFPane, pDebugPane);
				if (pReferencePane)
					m_paneManager.AttachPane (pReferencePane, pDebugPane);
				if (pObjectsPane)
					m_paneManager.AttachPane (pObjectsPane, pDebugVarPane);
                pDebugPane->Select ();

                pCallStackPane->Hide ();
                pLocalsPane->Hide ();
                pWatchesPane->Hide ();
                pDebugIOPane->Hide ();
                pDebugVarPane->Hide ();
                pObjectsPane->Hide ();
            }
            break;
        case enDiagnosticTemplate :  {
			if (m_ePswLevel < ePswService)   {
                if(m_paneManager.IsPaneClosed(ID_VIEW_DG_SDO))
                    m_paneManager.ShowPane(ID_VIEW_DG_SDO);
					if (ExistEthercat ())   {
                if(m_paneManager.IsPaneClosed(ID_VIEW_EC_MAILBOX))
                    m_paneManager.ShowPane(ID_VIEW_EC_MAILBOX);
					}
                CXTPDockingPane *pSDOPane=m_paneManager.FindPane(ID_VIEW_DG_SDO);
                CXTPDockingPane *pEcMailBoxPane=m_paneManager.FindPane(ID_VIEW_EC_MAILBOX);
                m_paneManager.DockPane (pSDOPane, xtpPaneDockBottom);
					if (pEcMailBoxPane)   {
                m_paneManager.DockPane (pEcMailBoxPane, xtpPaneDockBottom);
				m_paneManager.AttachPane (pEcMailBoxPane, pSDOPane);
            }
				}
            }
            break;
        case enResourcesTemplate :   {
                if(m_paneManager.IsPaneClosed(ID_VIEW_RS_CATALOGUE))
                    m_paneManager.ShowPane(ID_VIEW_RS_CATALOGUE);

                if(m_paneManager.IsPaneClosed(ID_VIEW_RS_PROPERTIES))
                    m_paneManager.ShowPane(ID_VIEW_RS_PROPERTIES);

                if(m_paneManager.IsPaneClosed(ID_VIEW_RS_ACTIVEX_PREVIEW))
                    m_paneManager.ShowPane(ID_VIEW_RS_ACTIVEX_PREVIEW);

                CXTPDockingPane *pCataloguePane, *pPropertiesPane, *pActiveXPane;
                pCataloguePane=m_paneManager.FindPane(ID_VIEW_RS_CATALOGUE);
                pPropertiesPane=m_paneManager.FindPane(ID_VIEW_RS_PROPERTIES);
				pActiveXPane=m_paneManager.FindPane(ID_VIEW_RS_ACTIVEX_PREVIEW);

                m_paneManager.DockPane (pCataloguePane, xtpPaneDockLeft);
                m_paneManager.DockPane (pPropertiesPane, xtpPaneDockRight);
                m_paneManager.DockPane (pActiveXPane, xtpPaneDockBottom, pPropertiesPane);
            }
            break;
        case enParametricTemplate :   {
                if (m_paneManager.IsPaneClosed(ID_VIEW_PAR_TREE))
                    m_paneManager.ShowPane(ID_VIEW_PAR_TREE);
                CXTPDockingPane *pViewParTreePane=m_paneManager.FindPane(ID_VIEW_PAR_TREE);
                m_paneManager.DockPane (pViewParTreePane, xtpPaneDockRight);
            }
            break;
    }
}

void CMainFrame::OnFileOption()
{
    COptionDlg dlgOption;
    dlgOption.DoModal();
}

void CMainFrame::OnFileImport()
{
	if (((CMainFrame *)AfxGetMainWnd ())->GetTemplate ()!=enNoTemplate)   {
		if (AfxMessageBox (LOADSTRING (IDS_IMPORT_CLOSEMODE), MB_ICONQUESTION|MB_YESNO)==IDYES)
			/* chiudiamo il modo corrente */
            ((CMainFrame *)AfxGetMainWnd ())->OnCloseMode ();
	}
	/* ora backuppiamo tutti i file */
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		pPrj->ImportFromXml (pPrj->GetProjectDir (), pPrj->GetIntermediateDir (), pPrj->GetName ());
		/* apriamo la pagina di configurazione */
		OnOpenConfig ();
	}
}

void CMainFrame::OnFileImportXml()
{
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		CKSFileDialog fd(TRUE, "xml", pPrj->GetProjectDir (), OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, 
			"Image files (*.xml)|*.xml|All files (*.*)|*.*||", AfxGetMainWnd());
		if (fd.DoModal()==IDOK)   {
			if (((CMainFrame *)AfxGetMainWnd ())->GetTemplate ()!=enNoTemplate)   {
				if (AfxMessageBox (LOADSTRING (IDS_IMPORT_CLOSEMODE), MB_ICONQUESTION|MB_YESNO)==IDYES)   {
					/* chiudiamo il modo corrente */
					((CMainFrame *)AfxGetMainWnd ())->OnCloseMode ();
					/* ora richiediamo il file da cui importare */
					BeginWaitCursor ();
					pPrj->ImportFromFileXml (pPrj->GetProjectDir (), pPrj->GetIntermediateDir (), pPrj->GetName (), fd.GetPathName ());
					/* apriamo la pagina di configurazione */
					OnOpenConfig ();
					EndWaitCursor ();
				}
			}
		}
	}
}

void CMainFrame::OnFileExportXml()
{
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		/* per prima cosa chiediamo il nome e path del file da salvare */
		CKSFileDialog fd(FALSE, "xml", pPrj->GetProjectDir (), OFN_PATHMUSTEXIST, 
			"Image files (*.xml)|*.xml|All files (*.*)|*.*||", AfxGetMainWnd());
		if (fd.DoModal()==IDOK)
			/* ora richiediamo il file da cui importare */
			pPrj->ExportInFileXml (fd.GetPathName ());
	}
}

LRESULT CMainFrame::OnAdamoConnected (WPARAM wParam, LPARAM lParam)
{
	int nModuleNumber = (int) wParam, nNumMachines;
    CProject *pPrj = GETWS ()->GetPtrPlant ()->GetFirst ();
    CProject *pActivePrj = GETWS()->GetPtrPlant ()->GetProject (nModuleNumber);
	
    if (pActivePrj)   {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        /* forziamo l'aggiornamento del campo CNC running in caso di aggancio su programma gia' esistente */
        pMachine->IsCncRunningReq ();
    }
    if (pPrj)
        /* andiamo a leggere le passwords */
        m_ePswLevel=pPrj->GetPswLevel ();
 	nNumMachines = GETWS ()->GetPtrPlant ()->GetCount ();
	if (++m_nClientConnected == nNumMachines) 
		m_wndStatusBar.Connect ();
    return 0;
}

LRESULT CMainFrame::OnAdamoDisconnected (WPARAM wParam, LPARAM lParam)
{
	CString strMachineName, strMsgBox;
	int nModuleNumber = (int) wParam;
	int nCloseMachine = (int) lParam;
    CProject *pPrj = GETWS()->GetPtrPlant ()->GetProject (nModuleNumber);

    m_wndStatusBar.Connect (false);
	m_nClientConnected--;
	if (!nCloseMachine)   {
		strMachineName = pPrj->GetMachineModule()->GetName();
		strMsgBox.Format ("Il modulo %s si e' scollegato", strMachineName);
		AfxMessageBox (strMsgBox, MB_ICONSTOP);
	}
    return 0;
}

/*
** LoadWorkSpace : esegue le inizializzazione del main frame a seguito dell'apertura
**                 di un impianto.
*/
void CMainFrame::LoadWorkSpace ()
{
    char szBuffer[256];

    ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetPlant(GetWorkSpace()->GetPtrPlant());
    sprintf (szBuffer, "%s - %s", (const char *)LOADSTRING (IDS_ADAMOMAINFRAME_24), (const char *)GetWorkSpace()->GetName ());
    SetWindowText (szBuffer);
	SetMode(modeNoProject);
	OnUpdateFrameTitle(TRUE);
}

/*
** CloseWorkSpace : la funzione esegue le operazioni di clean up di un workspace.
**                  Cancella quindi tutti i dati relativi al workspace corrente.
*/
void CMainFrame::CloseWorkSpace ()
{
    if (m_pCT)   {
        IAmGoingTo (eClosePlant);
        /* prendiamo l'oggetto applicazione */
        CIdeApp* pApp=(CIdeApp*)AfxGetApp();
        /* scorriamo i templates */
        for (POSITION pos=pApp->GetFirstDocTemplatePosition (); pos;)   {
            CAdamoTemplate* pTmpl=(CAdamoTemplate*)pApp->GetNextDocTemplate (pos);
            /* chiudiamo tutti i documenti */
            pTmpl->CloseAllDocuments (FALSE);
        }
        /* non c'e', usciamo dalla modalita di edit e chiudiamo tutto */
        SetLayout (m_objLayoutEmpty);
        CloseToolBars ();
        ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetPlant(NULL);
        SetWindowText (LOADSTRING (IDS_ADAMOMAINFRAME_24));
	    SetMode(modeNoPlantNoProject);
	    OnUpdateFrameTitle(TRUE);
        OnButtonDeleteErrorBar ();
        Invalidate (TRUE);
        IHaveDoneIt ();
        m_pCT=NULL;
    }
}

/*
** OnOpenConfig : apre la configurazione.
*/
void CMainFrame::OnOpenConfig ()
{
    /* verifichiamo che la configurazione non sia gia' aperta */
    if (m_pCT&&m_pCT->KindOf()==enConfTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enConfTemplate);
    IAmGoingTo (eOpeningMode);
    /* chiudiamo le dialog eventualmente aperte */
    CloseModelessDialogs ();
    /* chiudiamo il template corrente prima di aprirne un altro */
    OnCloseMode ();
    /* ora apriamo la configurazione */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    pTmpl->OnOpen ();
    SetLayout(m_objLayoutConfig);
    SetMode(modeConfig);
    /* settiamo il template corrente */
    m_pCT=pTmpl;
    /* carichiamo le toolbar */
    OpenToolBars ();
    IHaveDoneIt ();
	LoadSkin ();
}

/*
** OnUpdateOpenConfig : UI per la voce di apertura della configurazione
*/
void CMainFrame::OnUpdateOpenConfig(CCmdUI* pCmdUI) 
{
	if (m_ePswLevel == ePswUser || m_ePswLevel == ePswService)
		pCmdUI->Enable (false);
	else
    if (GETWS ())   {
        if (GetProject ())   {
            if (GetProject()->IsCompiling ())
                pCmdUI->Enable(FALSE);
            else
            if (modeNoPlantNoProject==m_nAppMode)
                pCmdUI->Enable (FALSE);
        }
    }
    else
        pCmdUI->Enable (FALSE);
}

/*
** OnOpenEditDebug : apre la modalita di edit/debug.
*/
void CMainFrame::OnOpenEditDebug() 
{
    /* chiudiamo il template corrente prima di aprirne un altro */
    if (m_pCT&&m_pCT->KindOf()==enEditDebugTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enEditDebugTemplate);
    IAmGoingTo (eOpeningMode);
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo le toolbars */
    CloseToolBars ();
    if (CheckIfADACanEdit ())   {
        /* settiamo la modalita iniziale */
        SetInitialEditDebugMode ();
        /* aggiorniamo le docking */
        UpdateDockingBars ();
        /* settiamo il template corrente */
        m_pCT=((CIdeApp*)AfxGetApp())->GetTemplate (enEditDebugTemplate);
        m_pCT->OnOpen ();
        /* apriamo una view eventualmente brekkata */
        OpenBreakedView ();
        /* carichiamo le toolbar */
        OpenToolBars ();
    }
    IHaveDoneIt ();
}

/*
** OnOpenMachine : apre la modalita di macchina (HTML).
*/
void CMainFrame::OnOpenMachine() 
{
    /* verifichiamo che la configurazione non sia gia' aperta */
    if (m_pCT&&m_pCT->KindOf()==enHTMLTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enHTMLTemplate);
    IAmGoingTo (eOpeningMode);
    /* chiudiamo le dialog eventualmente aperte */
    CloseModelessDialogs ();
    /* chiudiamo il template corrente prima di aprirne un altro */
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo le toolbars */
    CloseToolBars ();
    /* ora apriamo la configurazione */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enHTMLTemplate);
    pTmpl->OnOpen ();
    SetLayout(m_objLayoutMachine);
    SetMode(modeMachine);
    /* settiamo il template corrente */
    m_pCT=pTmpl;
    IHaveDoneIt ();
}

/*
** OnOpenDiagnostic : apre la modalita di diagnostica.
*/
void CMainFrame::OnOpenDiagnostic() 
{
    /* verifichiamo che la configurazione non sia gia' aperta */
    if (m_pCT&&m_pCT->KindOf()==enDiagnosticTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enDiagnosticTemplate);
    IAmGoingTo (eOpeningMode);
    /* chiudiamo le dialog eventualmente aperte */
    CloseModelessDialogs ();
    /* chiudiamo il template corrente prima di aprirne un altro */
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo le toolbars */
    CloseToolBars ();
    /* ora apriamo la configurazione */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enDiagnosticTemplate);
    SetLayout(m_objLayoutHWDiagnostic);
    SetMode(modeDiagnostic);
    /* settiamo il template corrente */
    m_pCT=pTmpl;
    /* carichiamo le toolbar */
    OpenToolBars ();
	/* apriamo il template */
    pTmpl->OnOpen ();
    IHaveDoneIt ();
}

/*
** OnUpdateOpenDiagnostic :
*/
void CMainFrame::OnUpdateOpenDiagnostic(CCmdUI* pCmdUI) 
{
	if (m_ePswLevel == ePswUser)
		pCmdUI->Enable (false);
	else   {
	// TODO: Add your command update UI handler code here
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);	
    if (!CheckIfIDECanWork ())
        pCmdUI->Enable (FALSE);
    if (m_ePswLevel>ePswService)
        pCmdUI->Enable (FALSE);
}
}

/*
** OnUpdateOpenMachine : UI per la voce di apertura macchina
*/
void CMainFrame::OnUpdateOpenMachine(CCmdUI* pCmdUI) 
{
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);	
    if (!CheckIfIDECanWork ())
        pCmdUI->Enable (FALSE);
}

/*
** OnUpdateOpenEditDebug : UI per la voce di apertura del edit/debug
*/
void CMainFrame::OnUpdateOpenEditDebug(CCmdUI* pCmdUI) 
{
	if (m_ePswLevel == ePswUser || m_ePswLevel == ePswService)
		pCmdUI->Enable (false);
	else   {
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);
    if (!CheckIfIDECanWork ())
        pCmdUI->Enable (FALSE);
    if (!CheckIfADACanEdit ())
        pCmdUI->Enable (FALSE);
}
}

/*
** OnOpenManual : apre il manuale diagnostico.
*/
void CMainFrame::OnOpenManDiag () 
{
    /* verifichiamo che la configurazione non sia gia' aperta */
    if (m_pCT&&m_pCT->KindOf()==enManDiagTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enManDiagTemplate);
    IAmGoingTo (eOpeningMode);
    /* chiudiamo le dialog eventualmente aperte */
    CloseModelessDialogs ();
    /* chiudiamo il template corrente prima di aprirne un altro */
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo le toolbars */
    CloseToolBars ();
    /* ora apriamo la configurazione */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enManDiagTemplate);
    SetLayout (m_objLayoutManDiag);
    SetMode (modeManDiag);
    /* settiamo il template corrente */
    m_pCT=pTmpl;
    /* carichiamo le toolbar */
    OpenToolBars ();
	/* apriamo il template */
    pTmpl->OnOpen ();
    IHaveDoneIt ();
	LoadSkin ();
}

/*
** OnUpdateOpenManual : UI per la voce di apertura del manuale diagnostico.
*/
void CMainFrame::OnUpdateOpenManual (CCmdUI* pCmdUI) 
{
	if (m_ePswLevel == ePswUser)
		pCmdUI->Enable (false);
	else   {
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);
    if (!CheckIfIDECanWork ())
        pCmdUI->Enable (FALSE);
}
}

/*
** ResetLayout : resetta il layout settato correntemente.
*/
void CMainFrame::ResetLayout ()
{
    if (m_objCurrentLayout)
        m_paneManager.GetLayout(m_objCurrentLayout);
}

/*
** SetLayout : setta il layout passato per parametro.
*/
void CMainFrame::SetLayout (CXTPDockingPaneLayout* pLayOut)
{
    ResetLayout ();
    m_paneManager.SetLayout (pLayOut);
    m_objCurrentLayout=pLayOut;
    if (pLayOut==m_objLayoutEdit || pLayOut==m_objLayoutDebug)
        m_MTIClientWnd.ShowWorkspace (TRUE);
    else
        m_MTIClientWnd.ShowWorkspace (FALSE);

}

/*
** CloseTemplate : chiude il template corrente.
*/
void CMainFrame::CloseTemplate (CAdamoTemplate* pTmp)
{
    switch (pTmp->KindOf ())   {
        case enNoTemplate :
            break;
        case enConfTemplate :
            CloseDocument (pTmp);
            break;
        case enEditDebugTemplate :
            CloseDocument (pTmp);
            break;
        case enManDiagTemplate :
            CloseDocument (pTmp);
            break;
        case enHTMLTemplate :
            CloseDocument (pTmp);
            break;
        case enShowErrorsTemplate :
            CloseDocument (pTmp);
            break;
        case enResourcesTemplate :
            CloseDocument (pTmp);
            break;
        case enDiagnosticTemplate :
            CloseDocument (pTmp);
            break;
    }
    CloseToolBars ();
}

/*
** CloseDocument : la funzione chiude i documenti associati al template
**                 passato per parametro.
*/
void CMainFrame::CloseDocument (CAdamoTemplate* pTmp)
{
    for (POSITION pos=pTmp->GetFirstDocPosition (); pos; )   {
        CDocument* pDoc=pTmp->GetNextDoc (pos);
        pDoc->OnCloseDocument ();
    }
}

/*
** OnCloseMode : risponde alla voce di menu di chiusura della modalita corrente
*/
void CMainFrame::OnCloseMode ()
{   
    if (m_pCT)   {
        IAmGoingTo (eClosingMode);
        /* prendiamo l'oggetto applicazione */
        CIdeApp* pApp=(CIdeApp*)AfxGetApp();
        /* scorriamo i templates */
        for (POSITION pos=pApp->GetFirstDocTemplatePosition (); pos;)   {
            CAdamoTemplate* pTmpl=(CAdamoTemplate*)pApp->GetNextDocTemplate (pos);
            /* chiudiamo tutti i documenti */
            pTmpl->CloseAllDocuments (FALSE);
        }
        IHaveDoneIt ();
        /* non c'e', usciamo dalla modalita di edit e chiudiamo tutto */
        SetLayout (m_objLayoutEmpty);
        SetMode (modeNoProject);
        /* chiudiamo anche le toolbars */
        CloseToolBars();
        m_pCT=NULL;
    }
}

/*
** OnDownloadAll : esegue il download di tutti i parametri.
*/
void CMainFrame::OnDownloadAll() 
{
    OnDownloadParameters();
    OnDownloadStrings ();
}

/*
** OnDownloadParameters : esegue il download della configurazione.
*/
void CMainFrame::OnDownloadParameters() 
{
    CProject *pPrj = GetWorkSpace()->GetActiveProject();

    if(pPrj)   {
        if (pPrj->GetMachine()->IsCncOnline ())   {
            BeginWaitCursor ();
            CString strParams=pPrj->GetParamsPathName(), strHWParams=pPrj->GetHWConfigPathName(), strUserStrings=pPrj->GetLanguagePathName();
            pPrj->GetMachine()->DownLoadParams (strParams, strHWParams, strUserStrings);
			DownLoadModuleParams ();
            DownLoadCC (pPrj);
            DownLoadPC (pPrj);
            EndWaitCursor ();
        }
    }
}

/*
** OnUpdateCloseMode : chiamata quando deve abilitare o disabilitare voci di menu
*/
void CMainFrame::OnUpdateCloseMode(CCmdUI* pCmdUI) 
{
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);
}

/*
** CreateFont : la funzione crea un generico font.
*/
CFont* CMainFrame::CreateFont (const char* strFont, int nHeight, int nBold)
{
    CFont* pFont = new CFont;
    pFont->CreateFont(
         nHeight,
         0,
         0,
         0,
         nBold,
	     FALSE,
         FALSE,
         0,
         ANSI_CHARSET,
	     OUT_DEFAULT_PRECIS,
	     CLIP_DEFAULT_PRECIS,
	     DEFAULT_QUALITY,
	     DEFAULT_PITCH, _T(strFont));
    return pFont;
}

/*
** OnSize : 
*/
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
    /*gestione toolbar sopra una certa dimensione della mainform*/
    if (cx > 50)  {
        if(m_pErrorBar) {
			m_nCx = cx;
			m_nCy = cy;
			ResizeErrorBar ();
        }
    }
}

/*
** ResizeErrorBar :
*/
void CMainFrame::ResizeErrorBar () 
{
    int nY = 2;
    int nHeight = 24;
	m_nCx-=1;
    CRect objRectStatic(33,nY,(int)(m_nCx*0.5-13),nHeight);            
    CXTPControl *pCtrlStatic = m_pErrorBar->GetControls()->FindControl(ID_STATIC_STATIC);
    pCtrlStatic->SetRect(objRectStatic);
    
    CRect objRectStrings((int)(m_nCx*0.5+10),nY,m_nCx-36,nHeight);            
    CXTPControl *pCtrlString = m_pErrorBar->GetControls()->FindControl(ID_STATIC_STRINGS);
    pCtrlString->SetRect(objRectStrings);

    CRect objRectButStrings((int)(m_nCx*0.5-13),nY,(int)(m_nCx*0.5+10),nHeight);
    CXTPControl *pCtrlButString = m_pErrorBar->GetControls()->FindControl(ID_BUTTON_STRINGS);
    pCtrlButString->SetRect(objRectButStrings);

    CRect objRectButDel(m_nCx-36,nY,m_nCx-35+23,nHeight);
    CXTPControl *pCtrlButDel = m_pErrorBar->GetControls()->FindControl(ID_BUTTON_DELETE_ERRORBAR);
    pCtrlButDel->SetRect(objRectButDel);
}

/*
** OnViewPrevdocking : passa alla docking precendente
*/
void CMainFrame::OnViewPrevdocking () 
{
    BOOL isFocus=TRUE;
   	POSITION pos = m_paneManager.GetPaneList ().GetHeadPosition ();
	while (pos)   {
		CXTPDockingPane* pPane = m_paneManager.GetPaneList ().GetAt (pos);
        if (pPane->IsFocus ())   {
            m_paneManager.GetPaneList ().GetPrev (pos);
            if (!pos)
                pos = m_paneManager.GetPaneList ().GetTailPosition ();                        
            pPane = m_paneManager.GetPaneList ().GetAt (pos);
            /*seleziono il pane successivo*/
            m_paneManager.ShowPane (pPane);
            isFocus = FALSE;
            break;
        }
        m_paneManager.GetPaneList ().GetNext (pos);
	}
    /*se non è selezionata nessuna docking seleziono la prima*/
    if (isFocus)   {
        POSITION pos = m_paneManager.GetPaneList ().GetHeadPosition ();
        CXTPDockingPane* pPane = m_paneManager.GetPaneList ().GetAt (pos);
        m_paneManager.ShowPane (pPane);
    }
}

/*
** OnViewNextdocking : passa alla docking successiva
*/
void CMainFrame::OnViewNextdocking () 
{
    BOOL isFocus=TRUE;
    POSITION pos = m_paneManager.GetPaneList ().GetHeadPosition ();
	while (pos)   {
		CXTPDockingPane* pPane = m_paneManager.GetPaneList ().GetAt (pos);
        if (pPane->IsFocus ())   {
			m_paneManager.GetPaneList ().GetNext (pos);
            if (!pos)
                pos = m_paneManager.GetPaneList ().GetHeadPosition ();
            pPane = m_paneManager.GetPaneList ().GetAt (pos);
            /*seleziono il pane seguente*/
            m_paneManager.ShowPane (pPane);
            if (pPane->GetID ()==ID_VIEW_DEVTREE)   {
                m_wndDockConfigTree.GetView()->Activate ();
                m_wndDockConfigTree.GetView()->SetFocus ();
            }
            pPane->Select ();
            pPane->SetFocus ();
            isFocus = FALSE;
            break;
        }
        m_paneManager.GetPaneList ().GetNext (pos);
	}
    /*se non è selezionata nessuna docking seleziono la prima*/
    if (isFocus)   {
        POSITION pos = m_paneManager.GetPaneList ().GetHeadPosition ();
        CXTPDockingPane* pPane = m_paneManager.GetPaneList ().GetAt (pos);
        m_paneManager.ShowPane (pPane);
    }
}       

void CMainFrame::OnUpdateCmdForButtonDynamic(CCmdUI* pCmdUI)
{
    pCmdUI->Enable();
}

LRESULT CMainFrame::OnThereIsAString (WPARAM wParam, LPARAM lParam)
{
	CString strError;
    stThiraStringElement *pSE=(stThiraStringElement *)lParam;
    if (pSE->nPriority==-1)
        GestioneCancellazioneStringa (pSE);
	else   {
		GestioneStringa (pSE);
	}
    return 0;
}

void CMainFrame::OnAttachDebugger() 
{
    CDialogSlot dlgSlot;

    if (dlgSlot.DoModal ()==IDOK)   {
        /*setta layout debug*/
        SetLayout (m_objLayoutDebug);
        /* prepariamo il debugger */
	    m_debug.Prepare();
        /* mandiamo giu' i dati relativi ai breakpoints */
        AttachDebugData (dlgSlot.m_szSlotName);
        /* settiamo l'IDE per il debugger */
        SetMode(modeDebug);
        /* settiamo le toolbar giuste */
        SwitchToolBars ();

    }
}

void CMainFrame::OnDetachDebugger() 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)   {
	    m_debug.Stop();
        SetLayout (m_objLayoutEdit);
        pActivePrj->GetMachine()->StopDebugging();
        CloseDialogList ();
        RemoveDialogList ();
        SetMode(modeExecute);
        SwitchToolBars ();
    }
}

void CMainFrame::AttachDebugData (const char* szSlotName)
{
    int n=0;
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        pActivePrj->GetMachine()->ClearDebugData ();
        pActivePrj->GetMachine()->InitDebugData (szSlotName);
        CProjectFile* pPrj=GetProject()->GetProjectFile (0);
        while (pPrj)   {
            pActivePrj->GetMachine()->SendDebugData (pPrj->GetPathName(), pPrj->GetBreakPoints ());
            pPrj=GetProject()->GetProjectFile (++n);
        }
        pActivePrj->GetMachine()->StartDebugging (CMDDBG_GO);
    }
}

void CMainFrame::OnUpdateAttachDebugger(CCmdUI* pCmdUI) 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())
            pCmdUI->Enable ();
        else        
            pCmdUI->Enable (FALSE);
    }
}

void CMainFrame::OnUpdateDetachDebugger(CCmdUI* pCmdUI) 
{
    if (GetMode()==modeDebug)
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}

LRESULT CMainFrame::OnProgramStop (WPARAM wParam, LPARAM lParam)
{
    if (m_pCT&&m_pCT->KindOf()==enEditDebugTemplate)   {
        if (GetMode()==modeDebug)
            DebugMessage (DMSG_DEBUG_END, 0, 0);
        else
            SetMode(modeBuild);
        if (WhatIAmGoingToDo ()!=eCloseDebuggingSession)   {
            SwitchToolBars ();
            IHaveDoneIt ();
        }
    }
    return 0;
}

void CMainFrame::OnBuildStepInto() 
{
    CDialogSlot dlgSlot;
	CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

	theApp.SaveModifiedDocuments();
    
    if(pActivePrj){
        if (CheckParameters ())   {
            /* ricompiliamo il programma */
	        if (!pActivePrj->Build() )
		        return;
            /*setta layout debug*/
            SetLayout (m_objLayoutDebug);
            /* prepariamo il debugger */
	        m_debug.Prepare();
            /* mandiamo giu' i dati relativi ai breakpoints */
            SendDebugData ("main");
            /* facciamo partire il debugger */
            StartDebugging (CMDDBG_RUN_TO_MAIN);
            /* settiamo l'IDE per il debugger */
            SetMode(modeDebug);
            /* settiamo le toolbar giuste */
            SwitchToolBars ();
        }
    }
}

void CMainFrame::OnBuildRunToCursor() 
{
    CString strPathName;
    int nModule;

    CView* pView = GetActiveView();
    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    CLuaDoc* pDoc = ((CLuaView*)pView)->GetDocument();

	if ( !pDoc->IsInProject() )
		return;

	CProjectFile* pPF = pDoc->GetProjectFile();
	strPathName = pPF->GetPathName();
    GetProject()->GetProjectFile (strPathName, &nModule);

	int nLine = ((CLuaView*)pView)->GetEditor()->GetCurrentLine();
	nLine = pPF->GetNearestDebugLine(nLine);

    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if (pActivePrj)   {
        if (CheckParameters ())   {        
            /* ricompiliamo il programma */  
	        if (!pActivePrj->Build() )
		        return;
            /*setta layout debug*/
            SetLayout (m_objLayoutDebug);
            /* prepariamo il debugger */
	        m_debug.Prepare();
            /* mandiamo giu' i dati relativi ai breakpoints */
            SendDebugData ("main");
            /* facciamo partire il debugger */
            StartDebugging (CMDDBG_RUN_TO_CURSOR_MAIN , nLine, nModule, strPathName);
            /* settiamo l'IDE per il debugger */
            SetMode(modeDebug);
            /* settiamo le toolbar giuste */
            SwitchToolBars ();
        }
    }
}

void CMainFrame::OnUpdateBuildStepInto(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
        if (GetProject ())
            if (GetProject()->IsCompiling ())
                pCmdUI->Enable(FALSE);
            else
                pCmdUI->Enable(GetMode()==modeBuild);
        else
            pCmdUI->Enable(FALSE);
    else
        pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateBuildRunToCursor(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
        if (GetProject ())
            if (GetProject()->IsCompiling ())
                pCmdUI->Enable(FALSE);
            else
	            pCmdUI->Enable(GetMode()==modeBuild);
        else
            pCmdUI->Enable(FALSE);
    else
        pCmdUI->Enable(FALSE);
}

void CMainFrame::SetInitialEditDebugMode ()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if (pActivePrj)  {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
	    /* vediamo innanzitutto se sta girando qualche cosa */
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())   {
            /* se siamo qui, vuol dire che possiamo essere o in running o in debug */
            CCmdGetDebugData dd;
            if (pMachine->GetDebugData (&dd) == S_OK)   {
                if (dd.m_nStatus==DBG_STOPPED)   {
                    /* apriamo il frame di dedit/debug */
                    SetLayout (m_objLayoutEdit);
                    /* prima i settaggi del build */
                    SetMode(modeBuild);
                    /* siamo in executing */
                    SetMode (modeExecute);
                }
                else   {
                    /* apriamo il frame di dedit/debug */
                    SetLayout (m_objLayoutDebug);
                    /* siamo in debug */
                    SetMode (modeDebug);
                    if (dd.m_nStatus==DBG_BREAKED)   {
                        /* il programma e' attualimente breccato */
                        SetMode (modeDebugBreak);
                        /* andiamo alla linea se necessario */
                        SetInitialBreakLine (&dd);
                    }
                }    
            }
        }    
        else   {
            /* apriamo il frame di dedit/debug */
            SetLayout (m_objLayoutEdit);
            /* se non sta girando niente, vuol dire che mettiamo tutto a modeBuild */
            SetMode(modeBuild);
        }
    }
}

bool CMainFrame::CheckParameters ()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();
    long par;
    bool b=true;

    if (pActivePrj)   {
        if (pActivePrj->GetMachine()->GetParCnt (&par) == S_OK)   {
            if (par==0)
                if (AfxMessageBox (LOADSTRING (IDS_ADAMOMAINFRAME_26), MB_YESNO | MB_ICONQUESTION)==IDYES)
                    OnDownloadAll ();
                else
                    b=false;
        }
        else
            b = false;
    }
    return b;
}

void CMainFrame::UpdateDockingBars ()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();
    if (pActivePrj)  {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        /* inizializziamo il view dell'IO nel debugger */
        CAdamoDbgIOView* pDbgIOView=m_wndDockDbgIO.GetView ();
        if (pDbgIOView)   {
            pDbgIOView->UpdateData ();
            pDbgIOView->SetMachine (pMachine);
        }
        /* inizializziamo il view delle variabili nel debugger */
        CAdamoDbgVarView* pDbgVarView=m_wndDockDbgVar.GetView ();
        if (pDbgVarView)   {
            pDbgVarView->UpdateData ();
            pDbgVarView->SetMachine (pMachine);
        }
        /* inizializziamo il view degli oggetti nel debugger */
        CAdamoDbgObjectView* pDbgObjectView=m_wndDockDbgObject.GetView ();
        if (pDbgObjectView)
            pDbgObjectView->SetMachine (pMachine);
    }
}

void CMainFrame::OnShowGlobalVariable (stGlobalDescriptor* pGD)
{
    if (pGD)   {
        switch (pGD->nType)   {
            case GLOBALVARIABLE :
                break;
            case GLOBALARRAY :
                ShowGlobalArray (pGD);
                break;
            case GLOBALMATRIX :
                ShowGlobalMatrix (pGD);
                break;
        }
    }
}

void CMainFrame::OnSysColorChange() 
{
	CMDIFrameWnd::OnSysColorChange();
	/* update toolkit colors.*/
	XTAuxData().UpdateSysColors();	
}

/*
**
*/
void CMainFrame::OpenToolBars ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();

    switch (m_nAppMode)   {
        case modeConfig :
            m_pConfEditBar = (CXTPToolBar*)pCommandBars->Add(_T("ConfEditing"), xtpBarTop);
            m_pConfEditBar->LoadToolBar(IDR_TOOLBARCONF);
            DockRightOf (m_pConfEditBar, m_pModeBar);
            m_pConfDevicesBar = (CXTPToolBar*)pCommandBars->Add(_T("ConfDevices"), xtpBarTop);
            m_pConfDevicesBar->LoadToolBar(IDR_TOOLBARCONFDEVICES);
            DockRightOf (m_pConfDevicesBar, m_pConfEditBar);
            m_pConfHW = (CXTPToolBar*)pCommandBars->Add(_T("ConfHW"), xtpBarTop);
            m_pConfHW->LoadToolBar(IDR_TOOLBARCONFHW);
            DockRightOf (m_pConfHW, m_pConfDevicesBar);
            m_pECConfHW = (CXTPToolBar*)pCommandBars->Add(_T("ECConfHW"), xtpBarTop);
            m_pECConfHW->LoadToolBar(IDR_TOOLBARCONFETHERCAT);
            DockRightOf (m_pECConfHW, m_pConfHW);
            break;
        case modeDebug :
        case modeDebugBreak :
            m_pEditBar = (CXTPToolBar*)pCommandBars->Add(_T("Editing"), xtpBarTop);
            m_pEditBar->LoadToolBar(IDR_MAINFRAME);
            DockRightOf (m_pEditBar, m_pModeBar);
            m_pDebugBar = (CXTPToolBar*)pCommandBars->Add(_T("Debug"), xtpBarTop);
            m_pDebugBar->LoadToolBar(IDR_TOOLBARDEBUG);
            DockRightOf (m_pDebugBar, m_pEditBar);
            m_pEditFindBar= (CXTPToolBar*)pCommandBars->Add(_T("FindEditing"), xtpBarTop);
            m_pEditFindBar->LoadToolBar(IDR_TOOLBAREDIT);
            DockRightOf (m_pEditFindBar, m_pDebugBar);
            m_pBrowseBar= (CXTPToolBar*)pCommandBars->Add(_T("Browse"), xtpBarRight);
            m_pBrowseBar->LoadToolBar(IDR_TOOLBAR_BROWSE);
            DockRightOf (m_pBrowseBar, m_pEditFindBar);
            break;
        case modeExecute :
        case modeBuild :
            m_pEditBar = (CXTPToolBar*)pCommandBars->Add(_T("Editing"), xtpBarTop);
            m_pEditBar->LoadToolBar(IDR_MAINFRAME);
            DockRightOf (m_pEditBar, m_pModeBar);
            m_pBuildBar= (CXTPToolBar*)pCommandBars->Add(_T("Build"), xtpBarTop);
            m_pBuildBar->LoadToolBar(IDR_TOOLBARBUILD);
            DockRightOf (m_pBuildBar, m_pEditBar);
            m_pEditFindBar= (CXTPToolBar*)pCommandBars->Add(_T("FindEditing"), xtpBarTop);
            m_pEditFindBar->LoadToolBar(IDR_TOOLBAREDIT);
            DockRightOf (m_pEditFindBar, m_pBuildBar);
            m_pMessageBar= (CXTPToolBar*)pCommandBars->Add(_T("Messages"), xtpBarTop);
            m_pMessageBar->LoadToolBar(IDR_TOOLBARMESSAGES);
            DockRightOf (m_pMessageBar, m_pEditFindBar);
            m_pBrowseBar= (CXTPToolBar*)pCommandBars->Add(_T("Browse"), xtpBarRight);
            m_pBrowseBar->LoadToolBar(IDR_TOOLBAR_BROWSE);
            DockRightOf (m_pBrowseBar, m_pMessageBar);
            break;
        case modeManDiag :
            m_pOscBar = (CXTPToolBar*)pCommandBars->Add(_T("Oscilloscope"), xtpBarTop);
            m_pOscBar->LoadToolBar(IDR_TOOLBAROSC);
            DockRightOf (m_pOscBar, m_pModeBar);
            m_pPlcBar = (CXTPToolBar*)pCommandBars->Add(_T("PlcRunStop"), xtpBarTop);
            m_pPlcBar->LoadToolBar(IDR_TOOLBARPLC);
            DockRightOf (m_pPlcBar, m_pOscBar);
            break;
        case modeShowErrors :
            m_pSEBar = (CXTPToolBar*)pCommandBars->Add(_T("ViewErrors"), xtpBarTop);
            m_pSEBar->LoadToolBar(IDR_TOOLBARVIEWERRORS);
            DockRightOf (m_pSEBar, m_pModeBar);
            OnSelectMonth (((CAdamoSEDoc*)m_pCT->GetDocument (""))->GetMonth ());
            break;
        case modeMachine :
            m_pSEBar = (CXTPToolBar*)pCommandBars->Add(_T("ViewErrors"), xtpBarTop);
            m_pSEBar->LoadToolBar(IDR_TOOLBARVIEWERRORS);
            DockRightOf (m_pSEBar, m_pModeBar);
            break;
        case modeResources :
            m_pRSBar = (CXTPToolBar*)pCommandBars->Add(_T("Risorse"), xtpBarTop);
            m_pRSBar->LoadToolBar(IDR_TOOLBARRESOURCES);
            DockRightOf (m_pRSBar, m_pModeBar);
            m_pRSControlsBar= (CXTPToolBar*)pCommandBars->Add(_T("Controlli"), xtpBarTop);
            m_pRSControlsBar->LoadToolBar(IDR_TOOLBARRESOURCESCONTROLS);
            DockRightOf (m_pRSControlsBar, m_pRSBar);
            m_pRSFormatBar= (CXTPToolBar*)pCommandBars->Add(_T("Formato"), xtpBarTop);
            m_pRSFormatBar->LoadToolBar(IDR_TOOLBARRESOURCESFORMAT);
            DockRightOf (m_pRSFormatBar, m_pRSControlsBar);
            m_pRSTools= (CXTPToolBar*)pCommandBars->Add(_T("Tools"), xtpBarTop);
            m_pRSTools->LoadToolBar(IDR_TOOLBARRESOURCESTOOLS);
            DockRightOf (m_pRSTools, m_pRSFormatBar);
            break;
        case modeDiagnostic :
            m_pCanOpenBar = (CXTPToolBar*)pCommandBars->Add(_T("CanOpen"), xtpBarTop);
            m_pCanOpenBar->LoadToolBar(IDR_TOOLBARCANOPEN);
            DockRightOf (m_pCanOpenBar, m_pModeBar);
            m_pNextToolBar = m_pCanOpenBar;
            break;
    }
}

/*
** CloseToolBars :
*/
void CMainFrame::CloseToolBars ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if (m_pConfEditBar)   {
        pCommandBars->Remove (m_pConfEditBar);
        m_pConfEditBar=NULL;
    }
    if (m_pConfDevicesBar)   {
        pCommandBars->Remove (m_pConfDevicesBar);
        m_pConfDevicesBar=NULL;
    }
    if (m_pDebugBar)   {
        pCommandBars->Remove (m_pDebugBar);
        m_pDebugBar=NULL;
    }
    if (m_pEditBar)   {
        pCommandBars->Remove (m_pEditBar);
        m_pEditBar=NULL;
    }
    if (m_pEditFindBar)   {
        pCommandBars->Remove (m_pEditFindBar);
        m_pEditBar=NULL;
    }
    if (m_pBuildBar)   {
        pCommandBars->Remove (m_pBuildBar);
        m_pBuildBar=NULL;
    }
    if (m_pMessageBar)   {
        pCommandBars->Remove (m_pMessageBar);
        m_pMessageBar=NULL;
    }
    if (m_pBrowseBar)   {
        pCommandBars->Remove (m_pBrowseBar);
        m_pBrowseBar=NULL;
    }
    if (m_pOscBar)   {
        pCommandBars->Remove (m_pOscBar);
        m_pOscBar=NULL;
    }
    if (m_pPlcBar)   {
        pCommandBars->Remove (m_pPlcBar);
        m_pPlcBar=NULL;
    }
    if (m_pSEBar)   {
        pCommandBars->Remove (m_pSEBar);
        m_pSEBar=NULL;
    }
    if (m_pRSBar)   {
        pCommandBars->Remove (m_pRSBar);
        m_pRSBar=NULL;
    }
    if (m_pRSControlsBar)   {
        pCommandBars->Remove (m_pRSControlsBar);
        m_pRSControlsBar=NULL;
    }
    if (m_pRSFormatBar)   {
        pCommandBars->Remove (m_pRSFormatBar);
        m_pRSFormatBar=NULL;
    }
    if (m_pRSTools)   {
        pCommandBars->Remove (m_pRSTools);
        m_pRSTools=NULL;
    }
    if (m_pDGBar)   {
        pCommandBars->Remove (m_pDGBar);
        m_pDGBar=NULL;
    }
    if (m_pConfHW)   {
        pCommandBars->Remove (m_pConfHW);
        m_pConfHW=NULL;
    }
    if (m_pECConfHW)   {
        pCommandBars->Remove (m_pECConfHW);
        m_pECConfHW=NULL;
    }
    if (m_pDGSSCNET)   {
        pCommandBars->Remove (m_pDGSSCNET);
        m_pDGSSCNET=NULL;
    }
    if (m_pDGEthercat)   {
        pCommandBars->Remove (m_pDGEthercat);
        m_pDGEthercat=NULL;
    }
    if (m_pCanOpenBar)   {
        pCommandBars->Remove (m_pCanOpenBar);
        m_pCanOpenBar=NULL;
    }
}

/*
** SwitchToolBars
*/
void CMainFrame::SwitchToolBars ()
{
    CloseToolBars ();
    OpenToolBars ();
}

void CMainFrame::OnNewMessage()
{
    m_wndDockLangView.GetView ()->AddMessage();
}

void CMainFrame::OnDeleteMessage()
{
    m_wndDockLangView.GetView ()->DeleteMessage();
}

void CMainFrame::OnModifyMessage()
{
    m_wndDockLangView.GetView ()->ModifyMessage ();
}

void CMainFrame::OnUpdateNewMessage(CCmdUI* pCmdUI)
{
    if (m_ePswLevel<=ePswService)   {
        CView* pView = GetActiveView();
	    
        if ( pView && (pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
		    pCmdUI->Enable(TRUE);
        else
		    pCmdUI->Enable(FALSE);	
    }
    else
		pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateModifyMessage(CCmdUI* pCmdUI)
{
    if (m_ePswLevel<=ePswService)   {
        CView* pView = GetActiveView();
	    
        if ( pView && (pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
		    pCmdUI->Enable(TRUE);
        else
		    pCmdUI->Enable(FALSE);	
    }
    else
		pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateDeleteMessage(CCmdUI* pCmdUI)
{
    if (m_ePswLevel<=ePswService)   {
        CView* pView = GetActiveView();
	    
        if ( pView && (pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
		    pCmdUI->Enable(TRUE);
        else
		    pCmdUI->Enable(FALSE);	
    }
    else
		pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnDownloadStrings() 
{
    CProject *pPrj = GetWorkSpace()->GetActiveProject();

    if(pPrj)   {
        if (pPrj->GetMachine()->IsCncOnline ())   {
            BeginWaitCursor ();
            CString strParams=pPrj->GetLanguagePathName();
            pPrj->GetMachine()->DownLoadUserStrings (strParams);
            EndWaitCursor ();
        }    
    }
}

void CMainFrame::OnOscStart()
{
    COscView* pOsc=m_wndDockOscilloscope.GetView ();
    if (pOsc)
        if (pOsc->GetCurSel()==2)   {
            CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
            if (pDlgOsc)
                pDlgOsc->OnOscStart ();
        }
}

void CMainFrame::OnOscStartPretrigger()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscStartPretrigger ();
}

void CMainFrame::OnOscStop()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscStop ();
}

void CMainFrame::OnOscClear()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscClear ();
}

void CMainFrame::OnOscOptions()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscOptions ();
}

void CMainFrame::OnOscShowCursor()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscCursor ();
}

void CMainFrame::OnOscLoad()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscLoad ();	
}

void CMainFrame::OnOscSave()
{
    CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscSave ();	
}

void CMainFrame::OnOscPrint()
{
}

void CMainFrame::OnUpdateOscStart(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateStartPretrigger(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscStop(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)
        pCmdUI->Enable ();
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscClear(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)
        pCmdUI->Enable ();
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscZoomToFit(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscOptions(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscShowCursor(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscShowDeltaXCursor(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscShowDeltaYCursor(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscLoad(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscSave(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

void CMainFrame::OnUpdateOscPrint(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)   {
        COscView* pOsc=m_wndDockOscilloscope.GetView ();
        if (pOsc)
            if (pOsc->GetCurSel()==2)   {
                CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
                long l=pDlgOsc->GetWorkStatus ();
                pCmdUI->Enable (!l);
            }
            else
                pCmdUI->Enable (FALSE);
    }
    else
        pCmdUI->Enable (FALSE);
}

/*
** OpenTemplate : apre il modo passato per parametro.
*/
void CMainFrame::OpenTemplate (int nMode)
{
    switch (nMode)   {
        case enConfTemplate :
			OnOpenPswConfig ();
            break;
        case enEditDebugTemplate :
			OnOpenPswEditDebug ();
            break;
        case enManDiagTemplate :
			OnOpenPswManDiag ();
            break;
        case enShowErrorsTemplate :
			OnOpenPswShowErrors ();
            break;
        case enResourcesTemplate :
			OnOpenPswResource ();
            break;
        case enDiagnosticTemplate :
			OnOpenPswDiagnostic ();
            break;
    }
}

/*
** OnUpdateViewPropDevice
*/
void CMainFrame::OnUpdateViewDevice(CCmdUI* pCmdUI) 
{
	if (GetMode() == modeHardware)
        pCmdUI->Enable(FALSE);	
}

/*
** OnUpdateViewPropDevice
*/
void CMainFrame::OnUpdateViewPropDevice(CCmdUI* pCmdUI) 
{
	if (GetMode() == modeHardware)
        pCmdUI->Enable(FALSE);	
}

/*
** OnViewHardware 
*/
void CMainFrame::OnViewHardware() 
{
	m_paneManager.ShowPane(ID_VIEW_DEVTREE);
		
}

/*
** OnUpdateViewHardware
*/
void CMainFrame::OnUpdateViewHardware(CCmdUI* pCmdUI) 
{
	if (m_ePswLevel == ePswUser)
		pCmdUI->Enable (false);
	else   {
	if (GetMode() == modeConfig || GetMode() == modeLogic)
        pCmdUI->Enable(FALSE);
}
}

/*
** SaveFileData
*/
void CMainFrame::SaveFileData()
{
    GetWorkSpace()->SaveDatConfig();
}

/*
** NewProject : messaggio ricevuto quando si istanzia un nuovo progetto.
*/
void CMainFrame::NewProject (CProject* pPrj)
{
    if(GetMode() == CMainFrame::modeNoProject)
		SetMode(CMainFrame::modeBuild);
    ((CMainFrame *)AfxGetMainWnd ())->UpdateActiveProject();
}

/*
** ClearGotoLines : la funzione ripulisce le frecce che indicano la posizione corrente del debugger
*/
void CMainFrame::ClearGotoLines ()
{
    CAdamoTemplate*pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enEditDebugTemplate);
    for (POSITION pos=pTmpl->GetFirstDocPosition (); pos;)   {
        CDocument* pDoc=pTmpl->GetNextDoc (pos);
        if (pDoc)   {
            for (POSITION pos2=pDoc->GetFirstViewPosition (); pos2;)   {
                CView* pView=pDoc->GetNextView (pos2);
                if (pView&&pView->IsKindOf (RUNTIME_CLASS (CLuaView)))
                    ((CLuaView*)pView)->GetEditor ()->ClearAllLines ();
            }
        }
    }
}

/*
** SetReadOnly : la funzione setta tutte le finestre che contengono un editor a read-only
*/
void CMainFrame::SetReadOnly (bool bRead)
{
    CAdamoTemplate*pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enEditDebugTemplate);
    for (POSITION pos=pTmpl->GetFirstDocPosition (); pos;)   {
        CDocument* pDoc=pTmpl->GetNextDoc (pos);
        if (pDoc)   {
            for (POSITION pos2=pDoc->GetFirstViewPosition (); pos2;)   {
                CView* pView=pDoc->GetNextView (pos2);
                if (pView&&pView->IsKindOf (RUNTIME_CLASS (CLuaView)))
                    ((CLuaView*)pView)->GetEditor ()->SetReadOnly (bRead);
            }
        }
    }
}

void CMainFrame::OnShowErrors() 
{
    /* verifichiamo che la configurazione non sia gia' aperta */
    if (m_pCT&&m_pCT->KindOf()==enShowErrorsTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enShowErrorsTemplate);
    IAmGoingTo (eOpeningMode);
    /* chiudiamo le dialog eventualmente aperte */
    CloseModelessDialogs ();
    /* chiudiamo il template corrente prima di aprirne un altro */
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo le toolbars */
    CloseToolBars ();
    /* ora apriamo la configurazione */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enShowErrorsTemplate);
    pTmpl->OnOpen ();
    SetLayout(m_objLayoutEmpty);
    SetMode(modeShowErrors);
    /* settiamo il template corrente */
    m_pCT=pTmpl;
    /* carichiamo le toolbar */
    OpenToolBars ();
    IHaveDoneIt ();
	/* carichiamo lo skin giusto */
	LoadSkin ();
}

void CMainFrame::OnUpdateOpenShowErrors(CCmdUI* pCmdUI) 
{
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);
    if (!CheckIfIDECanWork ())
        pCmdUI->Enable (FALSE);
}

POSITION CMainFrame::AddEditView (CView* pActiveView, CView* pView)
{
    POSITION retPos=NULL;
    int e = GetTemplate ();
    if (e==enEditDebugTemplate)   {
        if (!pActiveView)
            pActiveView=m_pCT->GetActiveView ();
        if (pActiveView)
            for (POSITION pos=m_lViews.GetHeadPosition (); pos; )   {
                CLuaView* pCurView=(CLuaView*)m_lViews.GetNext (pos);
                if (pCurView==pActiveView)   {
                    retPos=m_lViews.InsertAfter (((CLuaView*)pActiveView)->GetDocument ()->GetViewPosition (), pView);
                    break;
                }
            }
        if (!retPos)
            retPos=m_lViews.AddTail (pView);
    }
    return retPos;
}

void CMainFrame::RemoveEditView (POSITION pos)
{
    m_lViews.RemoveAt (pos);
}

CLuaView* CMainFrame::GetPrevEditView (CView* pView)
{
    for (POSITION pos=m_lViews.GetTailPosition (); pos; )   {
        CLuaView* pCurView=(CLuaView*)m_lViews.GetPrev (pos);
        if (pCurView==pView)
            if (pos)
                return (CLuaView*) m_lViews.GetPrev (pos);
            else   {
                pos = m_lViews.GetTailPosition ();
                return (CLuaView*) m_lViews.GetPrev (pos);
            }
    }
    return NULL;
}

/*
** Connect : gestisco i messaggi del OCX 
*/
void CMainFrame::Connect ()
{
    ((CConfTreeView*)m_wndDockConfigTree.GetView())->SetColorItemDrag();
}

/*
** Disconnect : gestisco i messaggi del OCX 
*/
void CMainFrame::Disconnect (const char *szName)
{
    ((CConfTreeView*)m_wndDockConfigTree.GetView())->DelColorItemDrag(szName);
}

/*
** DownLoadModuleParams :
*/
void CMainFrame::DownLoadModuleParams ()
{
	CAdamoConfigurazioneGenerale cg;
    CProject *pPrj = GetWorkSpace()->GetActiveProject();
	double afpXYZCBCompensation[NMAX_CORR_XYZCB];
	double afpTRTCompensation[NMAX_CORR_TRT];

	cg.SetRate (pPrj->GetRate ());
	cg.SetFrequenzaGenerale (pPrj->GetFrequency());
	cg.SetEpsilon (pPrj->GetPrecision());
	cg.SetMaxAngleRad (pPrj->GetContoureAngle());
	cg.SetEpsilonRaggi (pPrj->GetEpsilonRaggi ());
    cg.SetMessageFilteringOnStart (pPrj->GetMessageFilteringOnStart ());
	cg.SetStopAxisOnError (pPrj->GetStopAxisOnError ());
	cg.SetPrefetchSplineEnabled (pPrj->GetPrefetchSplineEnabled ());
	cg.SetKAC (pPrj->GetKAC ());
	cg.SetSensoRotazioneCircolari (pPrj->GetSensoRotazioneCircolari ());
	cg.SetSmoothing (pPrj->GetSmoothing ());
	cg.SetBowThickening (pPrj->GetBowThickening ());
	cg.SetLinearThickening (pPrj->GetLinearThickening ());
	cg.SetVelProfile (pPrj->GetVelProfile ());
	cg.SetAccProfile (pPrj->GetAccProfile ());
	cg.SetDecProfile (pPrj->GetDecProfile ());
	cg.SetAInterpDefault (pPrj->GetAInterpDefault ());
	cg.SetSpikeDetector (pPrj->GetSpikeDetector ());
	cg.SetMaxUTurnPrecision (pPrj->GetMaxUTurnPrecision ());
	cg.SetSmootherType (pPrj->GetSmootherType ());
	cg.SetSingleSpikeSmootherWeight (pPrj->GetSingleSpikeSmootherWeight ());
	cg.SetSingleSpikeFactor (pPrj->GetSingleSpikeFactor ());
	cg.SetGeometrySpikeFactor (pPrj->GetGeometrySpikeFactor ());
	cg.SetDetectJointUTurn (pPrj->GetDetectJointUTurn ());
	cg.SetStopOnAngle (pPrj->GetStopOnAngle ());
	cg.SetMinStopAngle (pPrj->GetMinStopAngle ());
	cg.SetRamDisk (pPrj->GetRamDisk ());
	cg.SetRamDiskBufferSize (pPrj->GetRamDiskBufferSize ());
	cg.SetSimulationMode (pPrj->GetSimulationMode ());
	cg.SetMaxJitter (pPrj->GetMaxJitter ());
	cg.SetCentroIncrementale (pPrj->GetCentroIncrementale ());
	cg.SetPlcBoost (pPrj->GetPlcBoost ());
	cg.SetSIP (pPrj->GetSIP ());
	pPrj->GetXYZCBCompensation (afpXYZCBCompensation);
	pPrj->GetTRTCompensation (afpTRTCompensation);
	cg.SetXYZCBCompensation (afpXYZCBCompensation);
	cg.SetTRTCompensation (afpTRTCompensation);
	cg.SetStrictAbsPosControl (pPrj->GetStrictAbsPosControl ());
	cg.SetStrictAbsPosEncoderSteps (pPrj->GetStrictAbsPosEncoderSteps ());
	pPrj->GetMachine()->DownLoadModuleParams (&cg);
}

/*
** DownLoadCC : download dei correttori incrociati.
*/
void CMainFrame::DownLoadCC (CProject* pPrj)
{
    CPtrList AxesList;
    CreateAxesList (AxesList);
    for (POSITION pos=AxesList.GetHeadPosition(); pos; )   {
        CAdamoAxis* pAxes=(CAdamoAxis*)AxesList.GetNext (pos);
        if (pAxes->GetCCName ()!="")   {
			/* per sicurezza verificare se i limiti sono settati se no non si esegue il download */
			if (pAxes->GetEnableLimAxesPos ()&&pAxes->GetEnableLimAxesNeg ())
				/* eseguiamo il download */
				pPrj->GetMachine()->DownLoadCC (pPrj->GetParamsPath()+"\\"+pAxes->GetCCName (), pAxes->GetName (), pPrj->GetParamsPathName());        
			else   {
				CString str;
				str.Format (LOADSTRING (IDS_ADAMOMAINFRAME_27), pAxes->GetName ());
				AfxMessageBox (str);
			}
		}
    }
	pPrj->GetMachine()->ClearCCData ();
}

/*
** DownLoadPC : download dei correttori incrociati.
*/
void CMainFrame::DownLoadPC (CProject* pPrj)
{
    CPtrList AxesList;
    CreateAxesList (AxesList);
    for (POSITION pos=AxesList.GetHeadPosition(); pos; )   {
        CAdamoAxis* pAxes=(CAdamoAxis*)AxesList.GetNext (pos);
        CString strName = pAxes->GetName ();
        CString strPCFileName = strName + "_PLANE";
        CString strPathPC = pPrj->GetParamsPath()+"\\" + strPCFileName;
        if (!_access (strPathPC, 00))
		    pPrj->GetMachine()->DownLoadPC (strPathPC, strPCFileName);
    }
	pPrj->GetMachine()->ClearCCData ();
}

/*
** CreateAxesList :
*/
void CMainFrame::CreateAxesList (CPtrList& rAxesList)
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)
        Iterator ((CAdamoGroup*)pPrj->GetMachineModule(), rAxesList);
}

void CMainFrame::Iterator(CAdamoGroup *pGrp, CPtrList& rAxesList)
{
    pGrp->Init();
    pGrp->DevInit();
    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase *)pGrp->DevGetNext();
        if (pDev->GetType ()== enAsse)
            InsertAx ((CAdamoAxis *)pDev, rAxesList);
    }
    while(pGrp->More())   {
        CObject *obj = (CObject*)pGrp->GetNext();
        Iterator((CAdamoGroup*)obj, rAxesList);
    }
}

void CMainFrame::InsertAx (CAdamoAxis *pDev, CPtrList& rAxesList)
{
    for (POSITION pos2, pos=rAxesList.GetHeadPosition(); pos2=pos;)   {
        CAdamoAxis* pListDev=(CAdamoAxis*) rAxesList.GetNext (pos);
        if (pDev->GetName().Compare (pListDev->GetName ())<0)   {
            rAxesList.InsertBefore (pos2, pDev);
            return;
        }
    }
    rAxesList.AddTail (pDev);
}

/*
**
*/
void CMainFrame::ParseStrError (const char* strErrore, CString* pstrModule, 
    CString* pstrLine, CString* pstrError)
{
    char szBuffer[256]; 
    char *p, *q, *r;

    strcpy (szBuffer, strErrore);
    p=strchr (szBuffer, '@');
    if (p)   {
        q=p+1;
        *p='\0';
        r=strrchr (szBuffer, '\\');
		*pstrModule = '\0';
		if (r)
			*pstrModule=r+1;
        r=strchr (q, '@');
        if (r)   {
            p=r+1;
            *r='\0';
            *pstrLine=q;
            *pstrError=p;
        }            
        else
            *pstrError=q;    
    }
    else
        *pstrError=strErrore;    
}

void CMainFrame::DrawBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart)
{
    BITMAP     bm;
    HBITMAP    bmBackOld;
    HDC        hdcTemp;

    // bitmap size
    ::GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
    // Create an in-memory DC compatible with the 
    // display DC we're using to paint
    hdcTemp = ::CreateCompatibleDC(hdc);
    // Select the bitmap into the in-memory DC
    bmBackOld   = (HBITMAP)::SelectObject(hdcTemp, hBitmap);
    // copy the bitmap
    ::BitBlt(hdc, xStart, yStart, bm.bmWidth, bm.bmHeight, hdcTemp, 0, 0, SRCCOPY);
    SelectObject(hdcTemp, bmBackOld);
}

/*
** DrawTransparentBitmap : disegna un bitmap con lo sfondo trasparente.
*/
void CMainFrame::DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
                           short yStart, COLORREF cTransparentColor)
{
   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = ::CreateCompatibleDC(hdc);
   ::SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   ::GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   ptSize.x = bm.bmWidth;            // Get width of bitmap
   ptSize.y = bm.bmHeight;           // Get height of bitmap
   ::DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device
                                     // to logical points

   // Create some DCs to hold temporary data.
   hdcBack   = ::CreateCompatibleDC(hdc);
   hdcObject = ::CreateCompatibleDC(hdc);
   hdcMem    = ::CreateCompatibleDC(hdc);
   hdcSave   = ::CreateCompatibleDC(hdc);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = ::CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = ::CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = ::CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = ::CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = (HBITMAP)::SelectObject(hdcBack, bmAndBack);
   bmObjectOld = (HBITMAP)::SelectObject(hdcObject, bmAndObject);
   bmMemOld    = (HBITMAP)::SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = (HBITMAP)::SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   ::SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   ::BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = ::SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   ::BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   ::SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   ::BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   ::BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   ::BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   ::BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   ::BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   ::BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   ::BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   ::DeleteObject(SelectObject(hdcBack, bmBackOld));
   ::DeleteObject(SelectObject(hdcObject, bmObjectOld));
   ::DeleteObject(SelectObject(hdcMem, bmMemOld));
   ::DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // Delete the memory DCs.
   ::DeleteDC(hdcMem);
   ::DeleteDC(hdcBack);
   ::DeleteDC(hdcObject);
   ::DeleteDC(hdcSave);
   ::DeleteDC(hdcTemp);
}

/*
** StretchBitmap : disegna un bitmap con lo sfondo trasparente.
*/
void CMainFrame::StretchBitmap (HDC hdc, HBITMAP hBitmap, short x, short y, short w, short h, 
								short xSrc, short ySrc, short wSrc, short hSrc)
{
    HDC hMemDC = CreateCompatibleDC (hdc);
	SelectObject (hMemDC, hBitmap) ;
    StretchBlt (hdc, x, y, w, h, hMemDC, xSrc, ySrc, wSrc, hSrc, SRCCOPY);
    DeleteDC (hMemDC) ;
}

void CMainFrame::OnChooseErrorLogMonth(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)	{
		int nMonth = pControl->GetCurSel();
        if (m_pCT&&m_pCT->KindOf ()==enShowErrorsTemplate)
            ((CAdamoSEView*)m_pCT->GetActiveView ())->Reload (nMonth+1);
    }
}

void CMainFrame::OnSetAllCanNetwork(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)	{
		int nScelta= pControl->GetCurSel();
        if (m_pCT&&m_pCT->KindOf ()==enDiagnosticTemplate)
            ((CAdamoDGView*)m_pCT->GetActiveView ())->SetDGMode (nScelta);
    }
}

void CMainFrame::OnSetAllEthercatNetwork(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)	{
		int nScelta= pControl->GetCurSel();
        if (m_pCT&&m_pCT->KindOf ()==enDiagnosticTemplate)
            ((CAdamoDGView*)m_pCT->GetActiveView ())->SetEcMode (nScelta);
    }
}

void CMainFrame::OnUpdateChooseErrorLogMonth(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateSetAllCanNetwork(CCmdUI* pCmdUI)
{
    if (m_pCT&&m_pCT->KindOf ()==enDiagnosticTemplate)   {
        CAdamoDGView *pView=(CAdamoDGView*)m_pCT->GetActiveView ();
        if (pView)   {
			if (m_ePswLevel < ePswService)   {
            bool b=((CAdamoDGView*)m_pCT->GetActiveView ())->IsDGDeviceOpened ();
            pCmdUI->Enable(b);
        }
    }
}
}

void CMainFrame::OnUpdateSetAllEthercatNetwork(CCmdUI* pCmdUI)
{
    if (m_pCT&&m_pCT->KindOf ()==enDiagnosticTemplate)   {
        CAdamoDGView *pView=(CAdamoDGView*)m_pCT->GetActiveView ();
        if (pView)   {
			if (m_ePswLevel < ePswService)   {
            bool b=((CAdamoDGView*)m_pCT->GetActiveView ())->IsEcDeviceOpened ();
            pCmdUI->Enable(b);
        }
    }
}
}

void CMainFrame::OnSelectMonth (int nMonth)
{
	if (m_pSEBar)   {
        CXTPControlComboBox* pStateCombo = (CXTPControlComboBox*)m_pSEBar->GetControls()->FindControl(IDM_SE_CHOOSE_MONTH);
        if (pStateCombo)
            pStateCombo->SetCurSel(nMonth-1);
    }
}

void CMainFrame::OnEditFindInFiles() 
{
    CView *pView = NULL;
    CMDIChildWnd *pChild = MDIGetActive();
    if (pChild != NULL)   {
        pView = pChild->GetActiveView();
        if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))   {
            if (m_bSearching)   {
                m_objFinder.StopSearch ();
                m_wndFFV.WriteCanceled ();
            }
            else   {
                // TODO: Add your command handler code here
                CAdamoFindInFiles aff;
                aff.m_pEdit=((CLuaView*)pView)->GetEditor();
                m_pCT->SaveAllModified ();
                DelayAdamoTimer (1);
                if (aff.DoModal ()==IDOK)
                    OnSearch (aff.m_strFind, aff.m_strFileType, aff.m_strFolder, aff.m_bMatchcase!=0, aff.m_bMatchwords!=0, aff.m_bMatchregex!=0);
                aff.m_pEdit->SetFocus ();
            }
        }
    }
}

void CMainFrame::OnSearch (CString strFind, CString strFileType, CString strFolder, bool bMatchCase, bool bMatchWords, bool bMatchRegEx) 
{
	CFileFinder::CFindOpts	opts;
	/* Set CFindOpts object */
	opts.sBaseFolder = strFolder;
	opts.sFileMask.Format("*%s*", strFileType);
	opts.bSubfolders = TRUE;
    opts.FindText(strFind, bMatchCase, bMatchWords, bMatchRegEx);
	/* Find files */
	m_bSearching = true;
	m_objFinder.RemoveAll();
    m_wndFFV.Clear ();
    SetNumFF (0);
    m_paneManager.ShowPane (ID_VIEW_FFV);
	AddProjectFilesName ();
	m_objFinder.Find(opts);
	m_bSearching = false;
    m_wndFFV.WriteOccurences (GetNumFF());
}

void CMainFrame::AddProjectFilesName ()
{
	CStringList m_listProject;

	GETWS()->GetActiveProject ()->GetProjectFiles (m_listProject);
	for (POSITION pos = m_listProject.GetHeadPosition (); pos;)
	{
		CString str = m_listProject.GetNext (pos);
		m_objFinder.AddProjectFile (str);
	}
}

void CMainFrame::FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam)
{
	CString			sText, sNewFile;
	MSG				msg;
	CMainFrame	    *pMainFrame = (CMainFrame *)pCustomParam;

	if (dwCode==FF_FOUND)   {
		sNewFile = pFinder->GetFilePath(pFinder->GetFileCount() - 1);
        CStringList& pStrList=pFinder->GetStrFound ();
        for (POSITION pos = pStrList.GetHeadPosition (); pos; )   {
            CString str=pStrList.GetNext (pos);
		    pMainFrame->m_wndFFV.Add (sNewFile, str); 
        }
        pMainFrame->AddNumFF (pStrList.GetCount ());
        pStrList.RemoveAll ();
    }
	// Process all process messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CMainFrame::OnInitCommandsPopup(CXTPPopupBar* pCommandBar)
{
    // get the list of commands for the popup.
    CXTPControls* pCommandList = pCommandBar->GetControls();
    CXTPControl* pCommandFF = pCommandList->FindControl(
        xtpControlButton, ID_EDIT_FIND_IN_FILES, TRUE, FALSE);
    if (pCommandFF)    {
        if (m_bSearching)
            pCommandFF->SetCaption("Stop F&ind in Files");
        else
            pCommandFF->SetCaption("F&ind in Files ...");
    }
}

LRESULT CMainFrame::OnCloseDialogFind (WPARAM wParam, LPARAM lParam)
{
    if (m_pFindDlg)   {
        m_pFindDlg->m_pEdit->SetFocus ();
        m_pFindDlg->DestroyWindow ();
        _delete (m_pFindDlg);
    }
    return 0;
}

LRESULT CMainFrame::OnCloseDialogReplace (WPARAM wParam, LPARAM lParam)
{
    if (m_pReplaceDlg)   {
        m_pReplaceDlg->m_pEdit->SetFocus ();
        m_pReplaceDlg->DestroyWindow ();
        _delete (m_pReplaceDlg);
    }
    return 0;
}

void CMainFrame::OnOpenResource() 
{
    /* verifichiamo che la configurazione non sia gia' aperta */
    if (m_pCT&&m_pCT->KindOf()==enResourcesTemplate)
        return;
    /* marchiamo il template che stiamo andando ad aprire */
    IAmGoingToOpen (enResourcesTemplate);
    IAmGoingTo (eOpeningMode);
    /* chiudiamo le dialog eventualmente aperte */
    CloseModelessDialogs ();
    /* chiudiamo il template corrente prima di aprirne un altro */
    if (m_pCT)
        m_pCT->OnClose ();
    /* chiudiamo le toolbars */
    CloseToolBars ();
    /* ora apriamo la configurazione */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    pTmpl->OnOpen ();
    SetLayout(m_objLayoutResources);
    SetMode(modeResources);
    /* settiamo il template corrente */
    m_pCT=pTmpl;
    /* carichiamo le toolbar */
    OpenToolBars ();
    IHaveDoneIt ();
}

void CMainFrame::OnUpdateOpenResource(CCmdUI* pCmdUI) 
{
	if (m_ePswLevel == ePswUser)
		pCmdUI->Enable (false);
	else   {
    if (modeNoPlantNoProject==m_nAppMode)
        pCmdUI->Enable (FALSE);
    if (!CheckIfIDECanWork ())
        pCmdUI->Enable (FALSE);
    if (m_ePswLevel>ePswBuilder)
        pCmdUI->Enable (FALSE);
}
}

void CMainFrame::OnViewMainDocking() 
{
    CLuaView* v;

    if (m_pCT)
        switch (m_pCT->KindOf())   {
            case enConfTemplate :
                break;
            case enEditDebugTemplate :
                v=(CLuaView*)m_pCT->GetActiveView ();
				if (v)   {
                    v->Activate ();
					v->GetEditor ()->SetFocus ();
				}
                break;
            case enResourcesTemplate :
                break;
        }
}

LRESULT CMainFrame::OnRequestResources (WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CMainFrame::OnRequestCCPlanes (WPARAM wParam, LPARAM lParam)
{
    CProject *pPrj = GetWorkSpace()->GetActiveProject();
    CPtrList AxesList;
    CString strArea = *(CString *)lParam, str;
    int nAsse = (int) wParam;

    if (pPrj)   {
        if (pPrj->GetMachine()->IsCncOnline ())   {
            BeginWaitCursor ();
            CreateAxesList (AxesList);
            for (POSITION pos=AxesList.GetHeadPosition(); pos; )   {
                CAdamoAxis* pAxes=(CAdamoAxis*)AxesList.GetNext (pos);
                if (pAxes->GetDeviceID () == nAsse)   {
                    CString strPCFileName = pAxes->GetName () + "_PLANE";
                    CString strPathPC = pPrj->GetParamsPath()+ "\\" + strPCFileName;
		            pPrj->GetMachine()->UpLoadPC (strPathPC, strArea);
                }
            }
            EndWaitCursor ();
        }
    }
    return 0;
}

void CMainFrame::CloseModelessDialogs ()
{
    if (m_pFindDlg)
        OnCloseDialogFind (0, 0L);
    if (m_pReplaceDlg)
        OnCloseDialogReplace (0, 0L);                    
}

CString CMainFrame::GetDeviceString (int nTypeDevice)
{
    CString str;
    switch (nTypeDevice)   {
        case enInputDigitale :
            str=LOADSTRING (IDS_ADAMODEVICE_1);
            break;
        case enOutputDigitale :
            str=LOADSTRING (IDS_ADAMODEVICE_2);
            break;
        case enInputAnalogico :
            str=LOADSTRING (IDS_ADAMODEVICE_3);
            break;
        case enOutputAnalogico :
            str=LOADSTRING (IDS_ADAMODEVICE_4);
            break;
        case enAsse :
            str=LOADSTRING (IDS_ADAMODEVICE_5);
            break;
        case enTimer :
            str=LOADSTRING (IDS_ADAMODEVICE_6);
            break;
        case enCounter :
            str=LOADSTRING (IDS_ADAMODEVICE_7);
            break;
        case enGenericCan :
            str=LOADSTRING (IDS_ADAMODEVICE_8);
            break;
    }
    return str; 
}                        

void CMainFrame::IAmGoingToOpen (enTemplateTypes en)
{
    m_enIAmGoingToOpen=en;
}

void CMainFrame::IAmGoingTo (eWhatIAmDoing eWhatIAmGoingToDo)
{
    m_eIAmDoing=eWhatIAmGoingToDo;
}

eWhatIAmDoing CMainFrame::WhatIAmGoingToDo ()
{
    return m_eIAmDoing;
}

void CMainFrame::IHaveDoneIt ()
{
    m_eIAmDoing=eDone;
}

void CMainFrame::GetDeviceErrorString (int nDevice, int nTypeDevice, char* szBuffer)
{
    switch (nTypeDevice)   {
        case HARDWARE_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_14));
            break;
        case RECEIVER_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_15), nDevice);
            break;
        case OMIKRON_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_18));
            break;
        case SSCNET_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_20), nDevice);
            break;
		case SYSTEM_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_24));
			break;
        case ETHERCAT_ERROR :
            sprintf (szBuffer, LOADSTRING (IDS_ADAMOSELISTVIEW_25));
            break;
    }
}

void CMainFrame::GetTypeDeviceErrorString (int nTypeDevice, char* szBuffer)
{
    CString str;
    if (nTypeDevice<HARDWARE_ERROR)
        str=((CMainFrame*)AfxGetMainWnd ())->GetDeviceString (nTypeDevice);
    else
        switch (nTypeDevice)   {
            case ETHERCAT_ERROR :
            case HARDWARE_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_16);
                break;
            case RECEIVER_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_17);
                break;
            case OMIKRON_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_19);
                break;
            case SSCNET_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_21);
                break;
            case SYSTEM_ERROR :
                str=LOADSTRING (IDS_ADAMOSELISTVIEW_23);
                break;
        }
    strcpy (szBuffer, str);
}

/*
** CheckIfIDECanWork :
*/
bool CMainFrame::CheckIfIDECanWork ()
{
    bool b=false;
    CWorkspace* pWrkspc=GETWS();       
    if (pWrkspc)   {
        CProject *pPrj=pWrkspc->GetActiveProject ();
        if (pPrj)
            b=true;
    }
    return b;
}
 
/*
** CheckIfADACanEdit :
*/
bool CMainFrame::CheckIfADACanEdit ()
{
    bool b=false;
    CWorkspace* pWrkspc=GETWS();       
    if (pWrkspc)   {
        CProject *pPrj=pWrkspc->GetActiveProject ();
        if (pPrj)   {
            if (pPrj->NofFiles ())
                b=true;
        }
    }
    return b;
}

/*
** GetScreenResolution :
*/
CSize CMainFrame::GetScreenResolution ()
{
    CClientDC dc (this);
    CSize size;

    size.cx = dc.GetDeviceCaps (HORZRES);
    size.cy = dc.GetDeviceCaps (VERTRES);
    return size;
}

void CMainFrame::OnBuildNextError() 
{
	// TODO: Add your command handler code here
    if (m_paneManager.IsPaneSelected (ID_VIEW_OUTPUT))
        m_wndOutput.GotoNextError ();
    if (m_paneManager.IsPaneSelected (ID_VIEW_DEBUG))
        m_wndDebug.GotoNextError ();
    if (m_paneManager.IsPaneSelected (ID_VIEW_FFV))
        m_wndFFV.GotoNextFFLine ();
}

void CMainFrame::OnBuildPrevError() 
{
	// TODO: Add your command handler code here
    if (m_paneManager.IsPaneSelected (ID_VIEW_OUTPUT) || m_paneManager.IsPaneSelected (ID_VIEW_DEBUG))
        m_wndOutput.GotoPrevError ();
    if (m_paneManager.IsPaneSelected (ID_VIEW_DEBUG))
        m_wndDebug.GotoPrevError ();
    if (m_paneManager.IsPaneSelected (ID_VIEW_FFV))
        m_wndFFV.GotoPrevFFLine ();
}

/*
** MatchString
*/
bool CMainFrame::MatchString (const char* strCmp, const char* str, bool bMatchWord, bool bUpperCase)
{
	char szStr[256], szCmp[256];
	char *p, *q;
	bool b=false;

	strcpy (szStr, str);
	strcpy (szCmp, strCmp);
	if (!bUpperCase)   {
		_strupr (szStr);
		_strupr (szCmp);
	}
	if (p=strstr (szStr, szCmp))   {
		if (bMatchWord)   {
            q=p;
            if (p-1>szStr)
                p--;
			q+=strlen (szCmp);
			if (!(*p=='_'||(*p>='a'&&*p<='z')||(*p>='A'&&*p<='Z')||(*p>='0'&&*p<='9')))
                if (!(*q=='_'||(*q>='a'&&*q<='z')||(*q>='A'&&*q<='Z')||(*q>='0'&&*q<='9')))
				    b=true;
		}
		else
			b=true;
	}
	return b;
}

/*
** PompaFuoriIMessaggi :
*/
void CMainFrame::PompaFuoriIMessaggi ()
{
    MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/*
** Set2003Style :
*/
void CMainFrame::SetXTPStyles ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();

	pImageManager->InternalAddRef();
	m_paneManager.SetImageManager(pImageManager);

	m_paneManager.SetDefaultPaneOptions(xtpPaneHasMenuButton);

	m_MTIClientWnd.EnableToolTips();

	m_MTIClientWnd.GetPaintManager()->m_bShowIcons = TRUE;
	m_MTIClientWnd.SetNewTabPosition(xtpWorkspaceNewTabRightMost);
	//LoadSkin ();
	CXTPToolTipContext* pToolTipContext = GetCommandBars()->GetToolTipContext();
	pToolTipContext->SetStyle(GetToolTipStyle());
	pToolTipContext->SetDelayTime(TTDT_INITIAL, 900);

}

/*
** GetActiveLuaView :
*/
CLuaEditor* CMainFrame::GetActiveLuaEditor ()
{
    CMDIChildWnd *pChild = MDIGetActive();
    if (pChild != NULL)  {
        CView* pView = pChild->GetActiveView();
        if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))   {
            return ((CLuaView*)pView)->GetEditor ();
        }
    }
    return NULL;
}

/*
** CheckChars :
*/
void CMainFrame::CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
   	CString str;
    int nRange = 0;
    bool bError = FALSE;

	pEdit->CEdit::GetWindowText(str);		

    str.TrimLeft (); str.TrimRight ();
    if (str=="")
        return;
    int n = objCheck.CheckString(str, 0);
    if (n > 0)   {
        nRange = n-1;
        bError = TRUE;
    }
    else
    if (str.GetLength() > nMaxLength)   {
        nRange = nMaxLength;
        bError = TRUE;
    }
    if (bError)   {
		MessageBeep((UINT)-1);
        str.Delete (nRange);
        pEdit->SetWindowText (str);
        pEdit->SetSel (0, -1);
        pEdit->ReplaceSel (str);
	}
}

/*
** CheckIpAddress :
*/
void CMainFrame::CheckIpAddress (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
   	CString str;
    int nRange = 0;
    bool bError = FALSE;

	pEdit->CEdit::GetWindowText(str);		
    CString strValue = pEdit->GetItem ()->GetValue ();
    str.TrimLeft (); str.TrimRight ();
    if (str=="")
        return;
    int n = objCheck.CheckString(str, 0);
    if (n > 0)   {
        nRange = n-1;
        bError = TRUE;
    }
    else
    if (CheckIPAddressValidity (str))   {
        nRange = nMaxLength;
        bError = TRUE;
    }
    if (bError)   {
		MessageBeep((UINT)-1);
        str.Delete (nRange);
        pEdit->SetWindowText (strValue);
        pEdit->SetSel (0, -1);
        pEdit->ReplaceSel (strValue);
	}
}

/*
** CheckIPAddressValidity :
*/
int CMainFrame::CheckIPAddressValidity (CString strIPAddress)
{
	CString str[4];
	int nPunticini, PunticinoPrima = 0, nPunticinoDopo;
    int a, nB = 0;
    bool bFinished;

    nPunticini = 0; bFinished = false;
	for (int i = 0; i<4; i++)   {
        if (PunticinoPrima == strIPAddress.GetLength ())
            break;
		nPunticinoDopo = strIPAddress.Find ('.', PunticinoPrima);
        if (nPunticinoDopo == -1)  {
			nPunticinoDopo = strIPAddress.GetLength ();
            bFinished = true;
        }
        else
            nPunticini++;
		str[i] = strIPAddress.Mid (PunticinoPrima, nPunticinoDopo - PunticinoPrima);
        if (str[i] == "")   {
            nB = 1; break;
        }
        else   {
	        a = strtol (str[i], NULL, 10);
            if (a > 255)   {
                nB = 1; break;
            }
        }
		PunticinoPrima = nPunticinoDopo + 1;
        if (bFinished)
            break;
	}
    if (nPunticini > 3)
        nB = 1;
    if (nB == 0 && strIPAddress.Find ('.', PunticinoPrima) != -1)
        nB = 1;
    return nB;
}

/*
** LoadString : carica una stringa dal file delle risorse.
*/
CString CMainFrame::LoadString (UINT nId)
{
    CString str;

    str.LoadString (nId);
    return str;
}

/*
** InitLanguages : carica la dll corretta delle lingue in funzione della lingua Kinema correntemente settata
*/
void CMainFrame::InitLanguages ()
{
	CString strLanguage = GETOPT()->GetLanguageThira ();
    strLanguage.MakeUpper ();
    SetLanguage (strLanguage);
}
    
/*
** SetLanguage : carica fisicamente la dll corretta.
*/
void CMainFrame::SetLanguage (CString strLanguage)
{
    if (strLanguage!="")   {
        if (m_hInst)   {
            if (m_hInst!=m_hItalianResources)   {
                FreeLibrary (m_hInst);
                m_hInst=NULL;
            }
        }
		CString strPathBin = GETOPT()->GetPathBin ();
		if (strPathBin != "")   {
            CString str = strPathBin + "\\" + strLanguage;
            m_hInst = LoadLibrary(str);
		}
    }
    else   {
        if (m_hInst)   {
            if (m_hInst!=m_hItalianResources)   {
                FreeLibrary (m_hInst);
                m_hInst=NULL;
            }
        }
        m_hInst=m_hItalianResources;
    }
    if (m_hInst)
        AfxSetResourceHandle (m_hInst);
}

/*
** OnChangeLanguage : gestisce il messaggio di cambio lingua che arriva dall' oggetto delle traduzioni
*/
LRESULT CMainFrame::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
    CString strThiraResources;

	CString strThiraLanguage = GETOPT()->GetLanguageThira ();
    /* settiamo la nuova dll di risorse del programma */
    SetLanguage (strThiraResources);
    /* cambiamo al volo i menu */
    SwitchMenu ();
    /* segnaliamo al workspace che e' cambiata la lingua */
    GETWS ()->ChangeLanguage ();
    /* diamo una bella rinfrescata alle finestre */
    return 0;
}

/*
** GetCurrentLanguage : ritorna la lingua corrente.
*/
CString CMainFrame::GetCurrentLanguage ()
{
	return GETOPT()->GetCurLanguage ();
}

/*
** PrintHexBuffer : stampa in caratteri leggibili esadecimali.
*/
void CMainFrame::PrintHexBuffer (unsigned char *pData, int nLengthData, unsigned char *pShowBuffer)
{
    int n=0, m=0;
    char c;

    while (n<nLengthData)   {
        c=pData[n]&0xf0;
        switch (c)   {
            case 0 :
            case 1 :
            case 2 :
            case 3 :
            case 4 :
            case 5 :
            case 6 :
            case 7 :
            case 8 :
            case 9 :
                pShowBuffer[m]=c+'0';
                break;
            case 10 :
            case 11 :
            case 12 :
            case 13 :
            case 14 :
            case 15 :
                pShowBuffer[m]='A'+c-10;
        }
        m++;
        c=pData[n]&0x0f;
        switch (c)   {
            case 0 :
            case 1 :
            case 2 :
            case 3 :
            case 4 :
            case 5 :
            case 6 :
            case 7 :
            case 8 :
            case 9 :
                pShowBuffer[m]=c+'0';
                break;
            case 10 :
            case 11 :
            case 12 :
            case 13 :
            case 14 :
            case 15 :
                pShowBuffer[m]='A'+c-10;
        }
        m++; n++;
        if (n!=nLengthData)
            pShowBuffer[m++]='.';
    }
}

/*
** ScanHexBuffer : legge e converte da caratteri leggibili esadecimali in un buffer di caratteri.
*/
void CMainFrame::ScanHexBuffer (unsigned char *pData, int nLengthData, unsigned char *pBuffer, int *nLengthBuffer)
{
    int n=0, m=0, q;
    bool b=false;
    char ch;

    while (n<nLengthData)   {
        ch=toupper (pData[n]);
        if (ch>='0'&&ch<='9'||ch>='A'&&ch<='F')   {
            if (ch>='0'&&ch<='9')
                q=ch-'0';
            else
                q=ch-'A'+0x0a;
            if (!b)   {
                pBuffer[m]=0;
                pBuffer[m]|=q<<4;
            }
            else   {
                pBuffer[m]|=q;
                m++;
            }
            b=!b;
        }
        n++;
    }
    *nLengthBuffer=m;
}

void CMainFrame::FocusOnEditor ()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	CLuaView *pView = (CLuaView*)GetActiveView();

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;
	pView->Activate ();
    pView->GetEditor ()->SetFocus ();
}

/*
** GetPathImages :
*/
CString CMainFrame::GetPathImages ()
{
	return GETOPT()->GetPathImages ();
}

/*
** LoadPathImages :
*/
CxImage* CMainFrame::LoadPathImages (CString strPath, DWORD imageType)
{
    CFile f;
    CxImage *pImg=NULL;

    if (f.Open (strPath, CFile::modeRead))   {
        DWORD dw=(DWORD)f.GetLength ();
        char *lpBuffer=new char [dw];
        f.Read (lpBuffer, dw);
        pImg=new CxImage ((BYTE*)lpBuffer, dw, imageType);
        delete lpBuffer;
        f.Close ();
    }
    return pImg;
}

void CMainFrame::OnBuildSearchText() 
{
    if (m_lastSearch!="")   {
        CView *pView = NULL;
        CMDIChildWnd *pChild = MDIGetActive();
        if (pChild != NULL)  {
            pView = pChild->GetActiveView();
            if (pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView)))   {
                CLuaEditor *pEdit = ((CLuaView*)pView)->GetEditor();
                pEdit->SearchForward (m_lastSearch.LockBuffer());
            }
        }
    }
}

void CMainFrame::SetStringToSearch (CString strLastSearch) 
{
    m_lastSearch=strLastSearch;
}

/*
** OnUpdateEditBreakPoints : UI per la voce di apertura della configurazione
*/
void CMainFrame::OnUpdateEditBreakPoints (CCmdUI* pCmdUI) 
{
    bool b=false;
    if (m_ePswLevel<=ePswService)
        b=true;
    pCmdUI->Enable(b);
}

/*
** OnKillFocus : chiamata da windows quando il frame principale perde il focus
*/
void CMainFrame::OnKillFocus(CWnd* pNewWnd) 
{
	CMDIFrameWnd::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
    m_bCtrlPressed=false;
    m_pCtrlView=NULL;
}

/*
** GetStationFromHWAddress :
*/
int CMainFrame::GetNodeFromHWAddress (const char *szBuffer)
{
    int n=0;
    char sz[256], *p, *q;

    strcpy (sz, szBuffer);
    p=strchr (sz, 'R');
    if (p++)   {
        q=p;
        while (*p>='0'&&*p<='9') p++;
        *p='\0';
        n=atoi (q);
    }
    return n-1;
}

/*
** GetChannelFromHWAddress :
*/
int CMainFrame::GetChannelFromHWAddress (const char *szBuffer)
{
    int n=0;
    char sz[256], *p, *q;

    strcpy (sz, szBuffer);
    p=strchr (sz, 'C');
    if (p++)   {
        q=p;
        while (*p>='0'&&*p<='9') p++;
        *p='\0';
        n=atoi (q);
    }
    return n-1;
}

/*
** GetPinFromHWAddress :
*/
int CMainFrame::GetPinFromHWAddress (const char *szBuffer)
{
    int n=0;
    char sz[256], *p, *q;

    strcpy (sz, szBuffer);
    p=strstr (sz, "Pin");
    if (p)   {
        p+=strlen ("Pin");
        q=p;
        while (*p>='0'&&*p<='9') p++;
        *p='\0';
        n=atoi (q);
    }
    return n-1;
}

/*
** LoadSSCNETToolbar :
*/
void CMainFrame::LoadSSCNETToolbar ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if (!m_pDGSSCNET)   {
        m_pDGSSCNET = (CXTPToolBar*)pCommandBars->Add(_T("SSCNET"), xtpBarTop);
        m_pDGSSCNET->LoadToolBar(IDR_TOOLBARSSCNET);
        DockRightOf (m_pDGSSCNET, m_pNextToolBar);
        m_pNextToolBar = m_pDGSSCNET;
    }
}

/*
** LoadEthercatToolbar :
*/
void CMainFrame::LoadEthercatToolbar ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if (!m_pDGEthercat)   {
        m_pDGEthercat = (CXTPToolBar*)pCommandBars->Add(_T("Ethercat"), xtpBarTop);
        m_pDGEthercat->LoadToolBar(IDR_TOOLBARETHERCAT);
        DockRightOf (m_pDGEthercat, m_pNextToolBar);
        m_pNextToolBar = m_pDGEthercat;
    }
}

/*
** LoadCommToolbar :
*/
void CMainFrame::LoadCommToolbar ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if (!m_pDGBar)   {
        m_pDGBar = (CXTPToolBar*)pCommandBars->Add(_T("Diagnostic"), xtpBarTop);
        m_pDGBar->LoadToolBar(IDR_TOOLBARDIAGNOSTIC);
        DockRightOf (m_pDGBar, m_pNextToolBar);
        m_pNextToolBar = m_pDGBar;
    }
}

/*
** RemoveCommToolbar :
*/
void CMainFrame::RemoveCommToolbar ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if (m_pDGBar)   {
        pCommandBars->Remove (m_pDGBar);
        m_pDGBar=NULL;
    }
}

/*
** WaitConnection :
*/
bool WaitConnection (CAdamoMachine *pMachine)
{
    bool b=true;
    DWORD dwIni=GetTickCount (), dwFin;
    while (!pMachine->IsCncOnline ())   {
        pMachine->Run ();
        dwFin=GetTickCount ();
        if (dwFin-dwIni>TIMEOUT_CONNECTION)   {
            b=false; break;
        }
    }
    return b;
}

void CMainFrame::OnEditBreakPoints() 
{
	// TODO: Add your command handler code here
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;
	CBreakpoints dlg;
    DelayAdamoTimer (1);
    dlg.DoModal ();
    if (bFocusOnEditor)
        pView->GetEditor ()->SetFocus ();
}

/*
** OnAreYouMe : gestisce il messaggio di cambio lingua che arriva dall' oggetto delle traduzioni
*/
LRESULT CMainFrame::OnAreYouMe (WPARAM wParam, LPARAM lParam)
{
    return m_nAreYouMe;
}

/*
** SetToolTips :
*/
void CMainFrame::SetToolTips ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetCommandBarsOptions()->bShowPopupBarToolTips = TRUE;
	if (pCommandBars->GetToolTipContext()->IsBalloonStyleSupported())	{
		pCommandBars->GetToolTipContext()->SetStyle(xtpToolTipBalloon);
		pCommandBars->GetToolTipContext()->ShowTitleAndDescription(TRUE);
	}
}

/*
** SetInitialBreakLine :
*/
void CMainFrame::SetInitialBreakLine (CCmdGetDebugData *pDD)
{
    if (GetTemplate ()==enConfTemplate)   {
        CProject *pProject = GETWS()->GetActiveProject();
        if (pProject)   {
            m_pBreakedPrjFile=pProject->GetProjectFile (pDD->m_nModule);
            m_nBreakedPrjLine=pDD->m_nLine;
        }
    }
}

/*
** OpenBreakedView :
*/
void CMainFrame::OpenBreakedView ()
{
    if (m_pBreakedPrjFile)   {
        CLuaView* pView = theApp.OpenProjectFilesView(m_pBreakedPrjFile, m_nBreakedPrjLine);
        if (pView)
            pView->Activate ();
        m_pBreakedPrjFile=NULL;
    }
}

/*
** OnBuildGoFromMD :
*/
void CMainFrame::OnBuildGoFromMD() 
{
    CDialogSlot dlgSlot;
	CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if (pActivePrj)  {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && !pMachine->IsCncRunning ())   {
	        theApp.SaveModifiedDocuments();
            if (CheckParameters ())   {
                if (dlgSlot.DoModal ()==IDOK)   {
                    /* prepariamo il debugger */
	                m_debug.Prepare();
                    /* mandiamo giu' i dati relativi ai breakpoints */
                    SendDebugData (dlgSlot.m_szSlotName);
                    /* facciamo partire il debugger */
                    StartDebugging (CMDDBG_GO);
                }
            }
        }
    }
}

/*
** OnBuildGoFromMD :
*/
void CMainFrame::OnDebugStopdebuggingFromMD()
{
	CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if (pActivePrj)  {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())   {
            CCmdGetDebugData dd;
            if (pMachine->GetDebugData (&dd) == S_OK)   {
                if (dd.m_nStatus!=DBG_STOPPED)   {
	                m_debug.Stop();
                    StopDebugging ();
                }
            }
        }
    }
}

void CMainFrame::OnDebugRunSlots() 
{
	// TODO: Add your command handler code here
	CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if (pActivePrj)  {
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())   {
            DelayAdamoTimer (1);            
            CCreateSlot cs;
            cs.SetBrowserContainer (&pActivePrj->GetBrowserContainer ());
            cs.DoModal ();
        }
    }
}

void CMainFrame::OnUpdateDebugViewslots(CCmdUI* pCmdUI) 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())
            pCmdUI->Enable ();
        else        
            pCmdUI->Enable (FALSE);
    }
}

void CMainFrame::OnUpdateDebugRunslots(CCmdUI* pCmdUI) 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj){
        CAdamoMachine* pMachine=pActivePrj->GetMachine();
        pMachine->Run ();
        if (pMachine->IsCncOnline () && pMachine->IsCncRunning ())
            pCmdUI->Enable ();
        else        
            pCmdUI->Enable (FALSE);
    }
}

/*
** GestioneTrace :
*/
void CMainFrame::GestioneTrace (stThiraErrore *ae)
{
    if (ae->nDeleteError==1)
        OnDebugClearTrace ();
	else   {
		CString strMachineTrace = ((stThiraErrore *) ae)->m_strModule;
		if (strMachineTrace == GETWS ()->GetActiveProject ()->GetName ())
        m_wndDebug.Write (CString (ae->strErrore)+"\n");
}
}



void CMainFrame::OnDebugClearTrace() 
{
	// TODO: Add your command handler code here
	m_wndDebug.Clear ();
}

void CMainFrame::OnUpdateDebugClearTrace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

CString CMainFrame::CorreggiPercorsoAssoluto (CString strPath, CString str)
{
    CString strNewPath;
    int n=0;
    char *p=str.GetBuffer (MAX_PATH), *q=p+strlen(p);
    
    while (q!=p)   {
        if (*q=='\\')
            n++;
        if (n==3)   {
            strNewPath=strPath+q;
            break;
        }
        q--;
    }
    str.ReleaseBuffer ();
    return strNewPath;
}

LRESULT CMainFrame::OnChangePsw (WPARAM wParam, LPARAM lParam)
{
    stPswRecord *pr=(stPswRecord *) lParam;
    if (pr->pswLevel!=m_ePswLevel)   {
        ePswLevel oldPsw=m_ePswLevel;
        m_strUser=pr->szUser;
        m_ePswLevel=pr->pswLevel;
		switch (pr->pswLevel)   {
			case  ePswKinema :
				if (m_pCT)
					m_pCT->OnChangePsw (oldPsw, m_ePswLevel);
				break;
			case  ePswBuilder :
				if (m_pCT)
					m_pCT->OnChangePsw (oldPsw, m_ePswLevel);
				break;
			case  ePswService :
				ManageServicePsw (oldPsw, m_ePswLevel);
				break;
			case  ePswUser :
				ManageUserPsw (oldPsw, m_ePswLevel);
				break;
		}
    }
    return 0;
}

void CMainFrame::OnBuildBreak() 
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
	    pActivePrj->AbortCompilation ();
}

void CMainFrame::OnUpdateBuildBreak(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
        if (GetProject ())
            if (GetProject()->IsCompiling ())
                pCmdUI->Enable();
            else
                pCmdUI->Enable(FALSE);
        else
            pCmdUI->Enable(FALSE);
    else
	    pCmdUI->Enable(FALSE);
}

bool CMainFrame::ExistDialogList (CString strName)
{
    CAdamoDialog *pDlg;
    bool b=false;

    for (POSITION pos=m_dlgList.GetHeadPosition (); pos; )   {
        pDlg=(CAdamoDialog *) m_dlgList.GetNext (pos);
        if (pDlg->GetKey ()==strName)   {
            b=true; break;
        }
    }
    return b;
}

void CMainFrame::AddDialogList (CAdamoDialog *dv)
{
    m_dlgList.AddTail (dv);
}

void CMainFrame::MoveDialogList (CAdamoDialog *dv)
{
    RECT rc, rc2;
    bool bMoved;
    int i=0;
    while (true)   {
        dv->GetWindowRect (&rc);
        bMoved=false;
        for (POSITION pos=m_dlgList.GetHeadPosition (); pos;)   {
            CAdamoDialog *pDlg=(CAdamoDialog *)m_dlgList.GetNext (pos);
            pDlg->GetWindowRect (&rc2);
            if (rc2.left==rc.left && rc2.top==rc.top)   {
                dv->MoveWindow (rc.left+32, rc.top+32, rc.right-rc.left, rc.bottom-rc.top);
                bMoved=true;
            }
        }
        if (!bMoved)
            break;
    }
}

void CMainFrame::TimerDialogList ()
{
    if (m_dlgList.GetCount ())   {
        CAdamoDialog *pDlg;

        for (POSITION pos=m_dlgList.GetHeadPosition (); pos; )   {
            pDlg=(CAdamoDialog *) m_dlgList.GetNext (pos);
            pDlg->OnTimer ();
        }
    }
}

void CMainFrame::RemoveDialogList (CString strName)
{
    if (m_dlgList.GetCount ())   {
        CAdamoDialog *pDlg;
        POSITION pos, pos2;

        for (pos=m_dlgList.GetHeadPosition (); pos2=pos; )   {
            pDlg=(CAdamoDialog *) m_dlgList.GetNext (pos);
            if (pDlg->GetKey ()==strName || strName=="")   {
                m_dlgList.RemoveAt (pos2);
                delete pDlg;
            }
        }
    }
}

void CMainFrame::CloseDialogList (CString strName)
{
    if (m_dlgList.GetCount ())   {
        CAdamoDialog *pDlg;

        for (POSITION pos=m_dlgList.GetHeadPosition (); pos; )   {
            pDlg=(CAdamoDialog *) m_dlgList.GetNext (pos);
            if (pDlg->GetKey ()==strName || strName=="")
                pDlg->DestroyWindow ();
        }
    }
}

void CMainFrame::OnUpdatePswLevel(CCmdUI* pCmdUI) //@parm handle of textpane for linenumbers - see mainfrm.cpp at top
{
    CString str;

    str=LOADSTRING (ID_INDICATOR_PSW_LEVEL);
    str.TrimRight ();
    if (m_ePswLevel==ePswUser)   {
        str+=" ";
        str+=LOADSTRING (IDS_PSWLEVEL_UTENTE);
    }
    if (m_ePswLevel==ePswService)   {
        str+=" ";
        str+=LOADSTRING (IDS_PSWLEVEL_MANUTENTORE);
    }
    if (m_ePswLevel==ePswBuilder)   {
        str+=" ";
        str+=LOADSTRING (IDS_PSWLEVEL_COSTRUTTORE);
    }
    if (m_ePswLevel==ePswKinema)   {
        str+=" ";
        str+=LOADSTRING (IDS_PSWLEVEL_KINEMA);
    }
    pCmdUI->SetText (str);
} 

/*
** OnUpdateFileOption : UI per la voce di apertura della configurazione
*/
void CMainFrame::OnUpdateFileOption (CCmdUI* pCmdUI) 
{
    bool b=false;
    if (m_ePswLevel<=ePswService)
        b=true;
    pCmdUI->Enable(b);
}

/*
** OnUpdateFileImport : UI per la voce di apertura del menu di importazione file XML.
*/
void CMainFrame::OnUpdateFileImport (CCmdUI* pCmdUI) 
{
    bool b=false;
    if (m_ePswLevel<=ePswService)
        b=true;
    pCmdUI->Enable(b);
}

/*
** OnUpdateFileImportXml : UI per la voce di apertura del menu di importazione file XML.
*/
void CMainFrame::OnUpdateFileImportXml (CCmdUI* pCmdUI) 
{
    bool b=false;
    if (m_ePswLevel<=ePswService)
        b=true;
    pCmdUI->Enable(b);
}

/*
** OnUpdateFileExportXml : UI per la voce di apertura del menu di importazione file XML.
*/
void CMainFrame::OnUpdateFileExportXml (CCmdUI* pCmdUI) 
{
    bool b=false;
    if (m_ePswLevel<=ePswService)
        b=true;
    pCmdUI->Enable(b);
}

void CMainFrame::OnUpdateEditReplace (CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
    	pCmdUI->Enable(m_nAppMode != modeNoPlantNoProject);
    else
        pCmdUI->Enable(false);
}

/*
** OnUpdateProjectAddFiles :
*/
void CMainFrame::OnUpdateProjectAddFiles(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswBuilder)
        OnUpdateBuildBuild (pCmdUI);
    else
	    pCmdUI->Enable(false);
}

/*
** OnUpdateProjectProperties :
*/
void CMainFrame::OnUpdateProjectProperties(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswBuilder)
        OnUpdateBuildBuild (pCmdUI);
    else
	    pCmdUI->Enable(false);
}

/*
** SwitchModeToolbar :
*/
void CMainFrame::SwitchModeToolbar ()
{
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if (m_pModeBar)   {
        pCommandBars->Remove (m_pModeBar);
        m_pModeBar=NULL;
    }
    m_pModeBar = (CXTPToolBar*)pCommandBars->Add(_T("Mode"), xtpBarTop);
    m_pModeBar->LoadToolBar(IDR_TOOLBAR_TEMPLATE);
    pCommandBars->InvalidateCommandBars ();
}

/*
** MoveDlgButton :
*/
void CMainFrame::MoveDlgButton (CButton &btn, int yTop, int xLeft, int yBottom, int  xRight)
{
	CRect rc;

	rc.top = yTop;
	rc.left = xLeft;
	rc.bottom = yBottom;
	rc.right = xRight;
	btn.MoveWindow (&rc);
}

LRESULT CMainFrame::OnAdamoGStartDownload (WPARAM wParam, LPARAM lParam)
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();
	CString strPath ((BSTR)lParam);
	if (pActivePrj)   {
		if (!_access (strPath, 00))
			pActivePrj->GetMachine()->DownLoadGProgram (strPath, (int) wParam, 0);
	}
	return 0L;
}

LRESULT CMainFrame::OnAdamoGDownloaded (WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

/*
** SendMatrixData : spedisce fisicamente i dati al controllo.
*/
void CMainFrame::SendMatrixData (CString strMatrixName, int nRow, CStringArray &keys, CAdamoData* pData)
{
	CAdamoMachine *pMachine = GETWS ()->GetActiveProject ()->GetMachine ();
    if (pMachine)   {
		/* prepariamo ora il buffer delle chiavi */
		int nDim=0, i, nColumnCount=keys.GetSize ();
		char *pBuffer, *p;
		for (i=0; i<nColumnCount; i++)
			nDim+=strlen (keys[i])+1;
		nDim+=2*sizeof(int);
		pBuffer=new char [4096];
		p=pBuffer;
		*((int*)p)=nDim-2*sizeof(int);
		p+=sizeof (int);
		*((int*)p)=nColumnCount;
		p+=sizeof (int);
		for (i=0; i<nColumnCount; i++)   {
			strcpy (p, keys[i]);
			p+=strlen (p)+1;
		}
		/* spediamo al controllo */
		pMachine->SetMatrix (strMatrixName, nRow, -nColumnCount, 0, pBuffer, pData);
		/* liberiamo la memoria */
		delete pBuffer;
	}
}

#ifdef ADAMO_PROTECTED
BOOL CMainFrame::CheckKey ()
{
    int retErr;
    char szBuffer[256];
    unsigned char code[8];

    code[2]=0x9c;
    code[3]=0x4f;
	retErr = bb3 ( 0, 30, 0 , 0 , 0 );
    if (retErr)   {
        sprintf (szBuffer, "%s", bb3err[retErr]);
        AfxMessageBox (szBuffer);
    }
    else   {
        szBuffer[0]=0;
        code[0]=(unsigned char)pow (c1, 2);
        code[1]=(unsigned char)sqrt (c2);
        code[4]=0xc8;
        code[5]=0xa0;
        code[6]=0xde;
        code[7]=0x2a;
	    retErr = bb3 ( 1, 30, 0 , (unsigned char *)szBuffer , code );
        if (!retErr)   {
            if (szBuffer[0]==limoncello)
                return TRUE;
        }
        else   {
            sprintf (szBuffer, "%s", bb3err[retErr]);
            AfxMessageBox (szBuffer);
        }
   }
   return FALSE;
}
#endif

int CMainFrame::GetStringaCodiceDatiAsse (int n)
{
	return anCodiciTipo[n];
}

int CMainFrame::GetCodiceStringaDatiAsse (int n)
{
	for (int i = 0; i<MAXAXESDATA; i++)
		if (n == anCodiciTipo[i])
			return i;
	return 0;
}

void CMainFrame::ManageUserPsw (ePswLevel pswVecchia, ePswLevel pswNuova)
{
	/* se il template aperto non e' quello degli errori, chiudiamolo */
	enTemplateTypes eTT = (enTemplateTypes) ((CMainFrame *)AfxGetMainWnd ())->GetTemplate ();
    if (eTT!=enNoTemplate && eTT != enShowErrorsTemplate)
		OnShowErrors ();
	if (m_pCT)
		m_pCT->OnChangePsw (pswVecchia, pswNuova);
}

void CMainFrame::ManageServicePsw (ePswLevel pswVecchia, ePswLevel pswNuova)
{
	/* se il template aperto non e' quello degli errori, chiudiamolo */
	enTemplateTypes eTT = (enTemplateTypes) ((CMainFrame *)AfxGetMainWnd ())->GetTemplate ();
    if (eTT!=enNoTemplate && eTT != enManDiagTemplate && eTT != enDiagnosticTemplate && eTT != enShowErrorsTemplate)
		OnOpenPswManDiag ();
	if (m_pCT)
		m_pCT->OnChangePsw (pswVecchia, pswNuova);
}

void CMainFrame::OnOpenPswConfig ()
{
	if (m_ePswLevel < ePswService)
		OnOpenConfig ();
	else
		if (m_ePswLevel == ePswService)
			OnOpenPswManDiag ();
		else
			OnShowErrors ();
}

void CMainFrame::OnOpenPswEditDebug ()
{
	if (m_ePswLevel < ePswService)
		OnOpenEditDebug ();
	else
		if (m_ePswLevel == ePswService)
			OnOpenPswManDiag ();
		else
			OnShowErrors ();
}

void CMainFrame::OnOpenPswManDiag ()
{
	if (m_ePswLevel < ePswUser)
		OnOpenManDiag ();
	else
		OnShowErrors ();
}

void CMainFrame::OnOpenPswShowErrors ()
{
	OnShowErrors ();
}

void CMainFrame::OnOpenPswResource ()
{
	if (m_ePswLevel < ePswService)
		OnOpenResource ();
	else
		if (m_ePswLevel == ePswService)
			OnOpenPswManDiag ();
		else
			OnShowErrors ();
}

void CMainFrame::OnOpenPswDiagnostic ()
{
	if (m_ePswLevel < ePswUser)
		OnOpenDiagnostic ();
	else
		OnShowErrors ();
}

/*
** ExistEthercat :
*/
bool CMainFrame::ExistEthercat ()
{
	bool b=false;
	return b;
}

void CMainFrame::OnConnectTo()
{
	// TODO: aggiungere qui il codice per la gestione dei comandi.
	CDlgConnectToTcp dlg;

	if (dlg.DoModal ())   {
	}
}

void CMainFrame::OnConnections()
{
	// TODO: aggiungere qui il codice per la gestione dei comandi.
	CAdamoDlgShowModules dlg;

	if (dlg.DoModal ())   {
	}
}

void CMainFrame::OnUpdateConnectTo(CCmdUI *pCmdUI)
{
	// TODO: aggiungere qui il codice per la gestione dell'interfaccia di aggiornamento dei comandi.
}

void CMainFrame::OnUpdateConnections(CCmdUI *pCmdUI)
{
	// TODO: aggiungere qui il codice per la gestione dell'interfaccia di aggiornamento dei comandi.
	bool bEnable = false;
	if (GETWS ())
		bEnable = TRUE;
	pCmdUI->Enable (bEnable);
}

int CMainFrame::ConnectToTcpIp ()
{
    CWorkspace *pWrs=GETWS();
    if (pWrs)   {
        CProject *pActivePrj = GETWS()->GetActiveProject();
        if (pActivePrj)   {
			BYTE achIPAddress[IPADDRESS_BUFFER];
			CAdamoOptions *pOpt = ((CIdeApp*) AfxGetApp ())->GetAdamoOptions ();
			pOpt->GetTcpIpAddress (achIPAddress[0], achIPAddress[1], achIPAddress[2], achIPAddress[3]);
			pActivePrj->GetMachine ()->SetTcpIpAddress (achIPAddress);
			pActivePrj->GetMachine ()->SetPort (pOpt->GetTcpIpPort ());
            pActivePrj->ReconnectMachine (enTCPConnection);
        }
    }
    return 0;
}

int CMainFrame::ConnectToLocal ()
{
    CWorkspace *pWrs=GETWS();
    if (pWrs)   {
        CProject *pActivePrj = GETWS()->GetActiveProject();
        if (pActivePrj)   {
			BYTE achIPAddress[IPADDRESS_BUFFER];
			memset (achIPAddress, 0, sizeof (achIPAddress));
			pActivePrj->GetMachine ()->SetTcpIpAddress (achIPAddress);
			pActivePrj->GetMachine ()->SetPort (-1);
            pActivePrj->ReconnectMachine (enLocalConnection);
        }
    }
    return 0;
}

void CMainFrame::OnForceAllInputsOff ()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
        pActivePrj->GetMachine()->ForceInput (-1, false);
}

void CMainFrame::OnUpdateSeFilterErrors(CCmdUI* pCmdUI) 
{
	bool bEnable = false;
	// TODO: Add your command update UI handler code here
	if (m_pCT && m_pCT->KindOf () == enShowErrorsTemplate)   {
		CAdamoSEView *pSEView = (CAdamoSEView *) m_pCT->GetActiveView ();
		if (pSEView)   {
			if (pSEView->GetActiveErrorType () != TE_ALL)
				bEnable = true;
		}
	}
	pCmdUI->Enable (bEnable);
}

void CMainFrame::OnUpdateSeClearFilter(CCmdUI* pCmdUI) 
{
	bool bEnable = false;
	// TODO: Add your command update UI handler code here
	if (m_pCT && m_pCT->KindOf () == enShowErrorsTemplate)   {
		CAdamoSEView *pSEView = (CAdamoSEView *) m_pCT->GetActiveView ();
		if (pSEView)   {
			if (pSEView->FilterEnabled ())
				bEnable = true;
		}
	}
	pCmdUI->Enable (bEnable);
}

void CMainFrame::OnUpdateSeGotoLine(CCmdUI* pCmdUI) 
{
	bool bEnable = false;
	// TODO: Add your command update UI handler code here
	if (m_pCT && m_pCT->KindOf () == enShowErrorsTemplate)   {
		if (m_ePswLevel < ePswService)   {
			CAdamoSEView *pSEView = (CAdamoSEView *) m_pCT->GetActiveView ();
			if (pSEView)   {
				if (pSEView->GetActiveErrorType () == TE_INTERPRETER || pSEView->GetActiveErrorType () == TE_ALL)
					bEnable = true;
			}
		}
	}
	pCmdUI->Enable (bEnable);
}

void CMainFrame::OnSeFilterErrors() 
{
	if (m_pCT && m_pCT->KindOf () == enShowErrorsTemplate)   {
		CAdamoSEView *pSEView = (CAdamoSEView *) m_pCT->GetActiveView ();
		if (pSEView)
			pSEView->OnSeFilterErrors ();
	}
}

void CMainFrame::OnSeClearFilter() 
{
	if (m_pCT && m_pCT->KindOf () == enShowErrorsTemplate)   {
		CAdamoSEView *pSEView = (CAdamoSEView *) m_pCT->GetActiveView ();
		if (pSEView)
			pSEView->OnSeClearFilter ();
	}
}

void CMainFrame::OnSeGotoLine() 
{
	if (m_pCT && m_pCT->KindOf () == enShowErrorsTemplate)   {
		CAdamoSEView *pSEView = (CAdamoSEView *) m_pCT->GetActiveView ();
		if (pSEView)
			pSEView->OnSeGotoLine ();
	}
}

void CMainFrame::AddSEView (stAdamoErrore *p)
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enShowErrorsTemplate);
	if (pTmpl)   {
		CAdamoSEView *pSEView = (CAdamoSEView *) pTmpl->GetActiveView ();
		if (pSEView)
			pSEView->AddError (p, 0);
	}
}
void CMainFrame::MotorizzaMacchine ()
{
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		CAdamoMachine *pMachine = pPrj->GetMachine ();
		if (pMachine)
			pMachine->Run ();
		pPrj = (CProject *) pPlant->GetNext ();
	}
}

CString CMainFrame::ChainMachineError (char *pErr)
{
	CString strMachineName;
	if (GETWS ()->IsMultiMachine ())   {
		strMachineName = GETWS ()->GetActiveProject ()->GetName ();
		strMachineName = strMachineName + " - " + pErr;
	}
	else
		strMachineName = pErr;
	return strMachineName;
}

void CMainFrame::OnOscZoomToFit ()
{
	CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscZoomToFit ();
}

void CMainFrame::OnOscShowDeltaXCursor ()
{
	CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscDeltaXCursor ();
}

void CMainFrame::OnOscShowDeltaYCursor ()
{
	CDlgOscilloscope* pDlgOsc=CDlgOscilloscope::GetOscilloscope ();
    pDlgOsc->OnOscDeltaYCursor ();
}

void CMainFrame::OnGotoDefinition() 
{
	// TODO: Add your command handler code here
	CString str = m_pEditBrowse->GetEditText ();
	CLuaView *pView = (CLuaView*)GetActiveView();

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;	

	CLuaView* pLuaView = pView->GotoDefinition (str);
	FocusOnEditor ();
}

void CMainFrame::OnGotoReference() 
{
	CString str = m_pEditBrowse->GetEditText ();
	CLuaView *pView = (CLuaView*)GetActiveView();

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

	pView->GotoReference (str);
	CreateReferencePane (pView);
	FocusOnEditor ();
}

void CMainFrame::OnDeleteBrowseSymbol() 
{
	if (m_pEditBrowse)
		m_pEditBrowse->SetEditText ("");
}

void CMainFrame::OnGotoNextReference() 
{
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;
	CLuaView* pLuaView = pView->GotoNextReference ();
	if (pLuaView && bFocusOnEditor)
		pLuaView->Activate ();
}

void CMainFrame::OnGotoPrevReference() 
{
    CWnd* wnd=GetFocus ();
	CLuaView *pView = (CLuaView*)GetActiveView();
    bool bFocusOnEditor=false;

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

    if (wnd&&wnd->GetSafeHwnd ()==pView->GetEditor ()->GetSafeHwnd ())
        bFocusOnEditor=true;
	CLuaView* pLuaView = pView->GotoPrevReference ();
	if (pLuaView && bFocusOnEditor)
		pLuaView->Activate ();
}

void CMainFrame::OnUpdateGotoDefinition(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
	pCmdUI->Enable ();
    else
        pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateGotoReference(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
	pCmdUI->Enable ();
    else
        pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateDeleteBrowseSymbol(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
	pCmdUI->Enable ();
    else
        pCmdUI->Enable(false);
}

void CMainFrame::OnSymbolToBrowse(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlEdit* pControl = (CXTPControlEdit*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlEdit)	{
    }
}

void CMainFrame::OnUpdateSymbolToBrowse(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
	pCmdUI->Enable ();
    else
        pCmdUI->Enable(false);
}

void CMainFrame::CreateReferencePane (CLuaView *pView) 
{
	stBrowserDescriptor* pReferenceData;
	CXTPDockingPane *pReferencePane;

	CXTPDockingPane* pPane=GetPaneManager()->FindPane (ID_VIEW_EDITREFERENCES);
	if (pPane == NULL)   {
		pReferencePane=m_paneManager.CreatePane(ID_VIEW_EDITREFERENCES, CRect(0, 0,200, 120), xtpPaneDockBottom);
		m_wndReferences.Init(this, ID_VIEW_EDITREFERENCES);
		CXTPDockingPane* pOutputPane=GetPaneManager()->FindPane (ID_VIEW_OUTPUT);
		if (pOutputPane != NULL)
			m_paneManager.AttachPane(pReferencePane, pOutputPane);
	}
    m_paneManager.ShowPane (ID_VIEW_EDITREFERENCES);
	int nRefCount = pView->GetReferenceData (&pReferenceData);
	m_wndReferences.Clear ();
	if (nRefCount)   {
		m_wndReferences.WritePrologue (pReferenceData->szSymbol, nRefCount);
		for (int i = 0; i<nRefCount; i++)   {
			m_wndReferences.Add (pReferenceData[i].szModule, pReferenceData[i].nLine, pReferenceData[i].nItem);
		}
	}
	else
		m_wndReferences.WritePrologue ();
}

CString CMainFrame::GetPathHelpOnLine () 
{
	CString strPathHTML = GETOPT()->GetPathHTML ();
	if (strPathHTML != "")
		strPathHTML = strPathHTML + "\\" + "Syntax";
    return strPathHTML;
}

void CMainFrame::OnBuildCleanInfo ()
{
    CProject *pActivePrj = GetWorkSpace()->GetActiveProject();

    if(pActivePrj)
	    pActivePrj->ClearProjectInfo ();
}

int CMainFrame::ParseDottedString (CString str, CStringArray& strArray)
{
	CString strSeparator (".");
	int nPosition = 0;

	CString strToken = str.Tokenize (strSeparator, nPosition);
	while(!strToken.IsEmpty())
	{
		strArray.Add (strToken);
		strToken = str.Tokenize(strSeparator, nPosition);
	}
	return strArray.GetSize ();
}

CString CMainFrame::GetLibrariesPathName ()
{
	CString strBasePathName = GETWS ()->GetBasePathName ();
	return strBasePathName + "\\" + DIR_LIBRARIES;
}

void CMainFrame::OnGotoHelpFunction() 
{
	CLuaView *pView = (CLuaView*)GetActiveView();

    if (!(pView && pView->IsKindOf(RUNTIME_CLASS(CLuaView))))
        return;

	pView->GotoHelpSymbol ();
	FocusOnEditor ();
}

void CMainFrame::OnUpdateGotoHelpFunction(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswService)
		pCmdUI->Enable ();
    else
        pCmdUI->Enable(false);
}

void CMainFrame::SetCommandBarsTheme(XTPPaintTheme paintTheme, LPCTSTR lpszINI /*=NULL*/)
{
	if (lpszINI != NULL)
		XTPThemeDLL()->SetHandle(lpszINI);
	else
		XTPThemeDLL()->Reset();
	((CIdeApp*)AfxGetApp ())->m_csPane.RefreshMetrics();

	LoadIcons();

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return;

	CXTPPaintManager::SetTheme(paintTheme);

	if (((CIdeApp*)AfxGetApp ())->m_theme <= themeVS2010)
	{
		m_bAllowDwm = TRUE;
		EnableFrameTheme(NULL);
		pCommandBars->SetAllCaps(FALSE);

		for (int i = 0; i < pCommandBars->GetCount(); i++)
		{
			CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
			pToolBar->SetFlags(xtpFlagAlignTop | xtpFlagAlignBottom | xtpFlagAlignLeft
								   | xtpFlagAlignRight | xtpFlagFloating,
							   0);
			pToolBar->SetShowGripper(TRUE);
		}

		SetFrameIcon(0, CSize(16, 16));
	}
	else
	{
		m_bAllowDwm = FALSE;
		EnableFrameTheme(pCommandBars);
		pCommandBars->SetAllCaps(theApp.m_theme == themeVS2012Light
								 || theApp.m_theme == themeVS2012Dark);

		for (int i = 0; i < pCommandBars->GetCount(); i++)
		{
			CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
			pToolBar->SetFlags(xtpFlagAlignTop, xtpFlagAlignBottom | xtpFlagAlignLeft
													| xtpFlagAlignRight | xtpFlagFloating);

			if (xtpBarTypeMenuBar == pToolBar->GetType())
			{
				pToolBar->SetShowGripper(FALSE);
			}
			else
			{
				pToolBar->SetShowGripper(TRUE);
			}
		}

	}

	// Update tooltip styles.
	CXTPToolTipContext* pToolTipContext = m_wndStatusBar.GetToolTipContext();
	if (NULL != pToolTipContext)
	{
		pToolTipContext->SetStyle(GetToolTipStyle());
	}

	pToolTipContext = GetCommandBars()->GetToolTipContext();
	if (NULL != pToolTipContext)
	{
		pToolTipContext->SetStyle(GetToolTipStyle());
	}

	pCommandBars->GetPaintManager()->m_bAutoResizeIcons = TRUE;
	pCommandBars->GetCommandBarsOptions()->SetDPIScallingOptions(TRUE);
	pCommandBars->RedrawCommandBars();

	UpdateMDIChildrenTheme();

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

XTPToolTipStyle CMainFrame::GetToolTipStyle() const
{
	XTPToolTipStyle style;

	switch (theApp.m_theme)
	{
		case themeVS6:
		case themeVS2003:
		case themeVS2005: style = xtpToolTipResource; break;

		default: style = xtpToolTipStandard; break;
	}

	return style;
}

void CMainFrame::OnTheme(UINT nID)
{
	theApp.strSchema.Empty();
	theApp.strColor.Empty();

	switch (nID)
	{
		case ID_THEME_VC6:
			theApp.m_theme = themeVS6;
			SetCommandBarsTheme(xtpThemeVisualStudio6);
			SetDockingPaneTheme(xtpPaneThemeGrippered);
			break;

		case ID_THEME_VC7:
			theApp.m_theme = themeVS2003;
			SetCommandBarsTheme(xtpThemeOfficeXP);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2003);
			break;

		case ID_THEME_VC8:
			theApp.m_theme = themeVS2005;
			SetCommandBarsTheme(xtpThemeVisualStudio2005);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2005, xtpPaneStickerStyleVisualStudio2005);
			m_paneManager.GetPaintManager()->GetPanelPaintManager()->m_bShowIcons = TRUE;
			m_paneManager.GetPaintManager()->GetTabPaintManager()->m_bShowIcons   = FALSE;
			break;

		case ID_THEME_VC9:
			theApp.m_theme = themeVS2008;
			SetCommandBarsTheme(xtpThemeVisualStudio2008);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2008, xtpPaneStickerStyleVisualStudio2008);
			break;

		case ID_THEME_VC10:
			theApp.m_theme = themeVS2010;
			SetCommandBarsTheme(xtpThemeVisualStudio2010);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2010, xtpPaneStickerStyleVisualStudio2010);
			break;

		case ID_THEME_VC11_LIGHT:
			theApp.m_theme = themeVS2012Light;
			SetCommandBarsTheme(xtpThemeVisualStudio2012Light, xtpIniVisualStudio2012Light);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2012,
								xtpPaneStickerStyleVisualStudio2012Light, XTP_DPI_X(5));
			break;

		case ID_THEME_VC11_DARK:
			theApp.m_theme = themeVS2012Dark;
			SetCommandBarsTheme(xtpThemeVisualStudio2012Dark, xtpIniVisualStudio2012Dark);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2012,
								xtpPaneStickerStyleVisualStudio2012Dark, XTP_DPI_X(5));
			break;

		case ID_THEME_VC14_LIGHT:
			theApp.m_theme = themeVS2015Light;
			SetCommandBarsTheme(xtpThemeVisualStudio2015, xtpIniVisualStudio2015Light);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2015,
								xtpPaneStickerStyleVisualStudio2015Light, XTP_DPI_X(5));
			break;

		case ID_THEME_VC14_DARK:
			theApp.m_theme = themeVS2015Dark;
			SetCommandBarsTheme(xtpThemeVisualStudio2015, xtpIniVisualStudio2015Dark);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2015,
								xtpPaneStickerStyleVisualStudio2015Dark, XTP_DPI_X(5));
			break;

		case ID_THEME_VC14_BLUE:
			theApp.m_theme = themeVS2015Blue;
			SetCommandBarsTheme(xtpThemeVisualStudio2015, xtpIniVisualStudio2015Blue);
			SetDockingPaneTheme(xtpPaneThemeVisualStudio2015,
								xtpPaneStickerStyleVisualStudio2015Blue, XTP_DPI_X(5));
			break;
	}
}

BOOL CMainFrame::SetFrameIcon(UINT nID, CSize szIcon)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return FALSE;

	CXTPPaintManager* pPaintManager = pCommandBars->GetPaintManager();
	if (!pPaintManager)
		return FALSE;

	CXTPFramePaintManager* pFramePaintManager = pPaintManager->GetFramePaintManager();
	if (!pFramePaintManager)
		return FALSE;

	HICON hIcon = NULL;

	if (nID > 0)
	{
		hIcon = (HICON)::LoadImage(theApp.m_hInstance, MAKEINTRESOURCE(nID), IMAGE_ICON, szIcon.cx,
								   szIcon.cy, LR_SHARED);
	}

	pFramePaintManager->SetIcon(hIcon, szIcon);
	return TRUE;
}

void CMainFrame::LoadIcons()
{
}

void CMainFrame::SetDockingPaneTheme(XTPDockingPanePaintTheme paintTheme, int nStyle /*=-1*/,
									 int nMargin /*=0*/)
{
	BOOL bShowStickers = (nStyle != -1);

	m_paneManager.SetTheme(paintTheme);
	m_paneManager.SetClientMargin(nMargin);
	m_paneManager.SetThemedFloatingFrames(bShowStickers);
	m_paneManager.SetAlphaDockingContext(bShowStickers);
	m_paneManager.SetShowDockingContextStickers(bShowStickers);
	m_paneManager.SetShowContentsWhileDragging(bShowStickers);

	if (bShowStickers)
	{
		m_paneManager.SetDockingContextStickerStyle((XTPDockingContextStickerStyle)nStyle);
	}

	SendMessageToDescendants(m_nRefreshMetrics, theApp.m_theme);

	m_paneManager.UpdatePanes();
	m_paneManager.RedrawPanes();

	if (theApp.m_theme > themeVS2008)
	{
		m_MTIClientWnd.SetFlags(xtpWorkspaceHideClose | xtpWorkspaceHideArrowsAlways
								| xtpWorkspaceShowActiveFiles | xtpWorkspaceShowCloseTab);
	}
	else
	{
		m_MTIClientWnd.SetFlags(0);
	}

	m_paneManager.ShowFloatingCaptionMaximizeButton(themeVS2010 <= theApp.m_theme);
}

void CMainFrame::SetSkin (CString& strSkin)
{
	bool bFound = false;
	for (int i = 0; i<sizeof (achSkinStyles) / sizeof (char*); i++)
		if (strSkin == achSkinStyles[i])   {
			switch (i)   {
				case 0 :
					OnTheme (ID_THEME_VC10);
					break;
				case 1 :
					OnTheme (ID_THEME_VC11_LIGHT);
					break;
				case 2 :
					OnTheme (ID_THEME_VC11_DARK);
					break;
				case 3 :
					OnTheme (ID_THEME_VC14_BLUE);
					break;
				case 4 :
					OnTheme (ID_THEME_VC14_LIGHT);
					break;
				case 5 :
					OnTheme (ID_THEME_VC14_DARK);
					break;
			}
			bFound = true;
			break;
		}
	if (!bFound)
		OnTheme (ID_THEME_VC14_LIGHT);
}

void CMainFrame::LoadSkin ()
{
	CString strSkin = GETOPT()->GetSkin ();
	SetSkin (strSkin);
}

void CMainFrame::GestioneErrore (stThiraErrore* p)
{
	switch (p->nTypeError)   {
        case TE_HARDWARE :
            AddHardwareError (p);
            break;
        case TE_INTERPRETER :
            AddInterpreterError (p);
            break;
        case TE_CYCLE :
            AddCycleError (p);
            break;
        case TE_ISO :
            AddISOError (p);
            break;
        case TE_EVENT :
            AddEvent (p);
            break;
	}
	m_wndStatusBar.ShowAlarm ();
}

void CMainFrame::AddHardwareError (stThiraErrore* p)
{
	CXTPDockingPane *pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
	if (pPane)   {
		CAdamoListHardwareErrors *pView = (CAdamoListHardwareErrors *) m_wndDockListHardwareErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
	pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
	if (pPane)   {
		CAdamoListAllErrors *pView = (CAdamoListAllErrors *) m_wndDockListAllErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
}

void CMainFrame::AddInterpreterError (stThiraErrore* p)
{
	CXTPDockingPane *pPane=m_paneManager.FindPane (ID_VIEW_INT_ERRORS);
	if (pPane)   {
		CAdamoListInterpreterErrors *pView = (CAdamoListInterpreterErrors *) m_wndDockListInterpreterErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
	pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
	if (pPane)   {
		CAdamoListAllErrors *pView = (CAdamoListAllErrors *) m_wndDockListAllErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
}

void CMainFrame::AddCycleError (stThiraErrore* p)
{
	CXTPDockingPane *pPane=m_paneManager.FindPane (ID_VIEW_PLC_ERRORS);
	if (pPane)   {
		CAdamoListPLCErrors *pView = (CAdamoListPLCErrors *) m_wndDockListPlcErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
	pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
	if (pPane)   {
		CAdamoListAllErrors *pView = (CAdamoListAllErrors *) m_wndDockListAllErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
}

void CMainFrame::AddISOError (stThiraErrore* p)
{
	CXTPDockingPane *pPane=m_paneManager.FindPane (ID_VIEW_ISO_ERRORS);
	if (pPane)   {
		CAdamoListISOErrors *pView = (CAdamoListISOErrors *) m_wndDockListISOErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
	pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
	if (pPane)   {
		CAdamoListAllErrors *pView = (CAdamoListAllErrors *) m_wndDockListAllErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
}

void CMainFrame::AddEvent (stThiraErrore* p)
{
	CXTPDockingPane *pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
	if (pPane)   {
		CAdamoListAllErrors *pView = (CAdamoListAllErrors *) m_wndDockListAllErrors.GetView ();
		if (pView)   {
			pView->AddError (p);
			pPane->Select ();
		}
	}
}

void CMainFrame::GestioneCancellazioneErrore (stThiraErrore* p)
{
}

void CMainFrame::GestioneStringa (stThiraStringElement* p)
{
	CXTPDockingPane *pPane=m_paneManager.FindPane (ID_VIEW_MESSAGES);
	if (pPane)   {
		CAdamoListMessages *pView = (CAdamoListMessages *) m_wndDockListMessages.GetView ();
		if (pView)   {
			pView->AddMessage (p->str, p->m_strModule, p->nPriority, p->dt);
			pPane->Select ();
		}
	}
	pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
	if (pPane)   {
		CAdamoListAllErrors *pView = (CAdamoListAllErrors *) m_wndDockListAllErrors.GetView ();
		if (pView)   {
			pView->AddMessage (p->str, p->m_strModule, p->nPriority, p->dt);
			pPane->Select ();
		}
	}
	m_wndStatusBar.ShowMessage ();
}

void CMainFrame::GestioneCancellazioneStringa (stThiraStringElement* p)
{
}

LRESULT CMainFrame::OnDeleteErrors (WPARAM wParam, LPARAM lParam)
{
    CXTPDockingPane* pPane;
	/* spegniamo i bottoni della status bar */
	m_wndStatusBar.ShowAlarm (false);
	m_wndStatusBar.ShowMessage (false);
	/* ora azzeriamo i pane, prima quello di tutti gli errori */
    pPane=m_paneManager.FindPane (ID_VIEW_ALL_ERRORS);
    if (pPane)
		m_wndDockListAllErrors.GetView ()->DeleteErrors ();
	/* interprete */
    pPane=m_paneManager.FindPane (ID_VIEW_INT_ERRORS);
    if (pPane)
		m_wndDockListInterpreterErrors.GetView ()->DeleteErrors ();
	/* Plc */
    pPane=m_paneManager.FindPane (ID_VIEW_PLC_ERRORS);
    if (pPane)
		m_wndDockListPlcErrors.GetView ()->DeleteErrors ();
	/* Iso */
    pPane=m_paneManager.FindPane (ID_VIEW_ISO_ERRORS);
    if (pPane)
		m_wndDockListISOErrors.GetView ()->DeleteErrors ();
	/* Hardware */
    pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
    if (pPane)
		m_wndDockListHardwareErrors.GetView ()->DeleteErrors ();
	/* Messages */
    pPane=m_paneManager.FindPane (ID_VIEW_HARDWARE_ERRORS);
    if (pPane)
		m_wndDockListMessages.GetView ()->DeleteMessages ();
	return S_OK;
}

CXTPTabManagerItem* CAdamoXTPTabClientWnd::AddItem (CWnd* pChildWnd)
{
	CXTPTabManagerItem* pTabItem = NULL;
	if (pChildWnd->IsKindOf(RUNTIME_CLASS(CLuaFrame)))
		pTabItem = __super::AddItem (pChildWnd);
	return pTabItem;
}
