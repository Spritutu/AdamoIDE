// MainFrame.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRAME_H__CCF97055_F1B3_48F3_B536_F23CF34360E9__INCLUDED_)
#define AFX_MAINFRAME_H__CCF97055_F1B3_48F3_B536_F23CF34360E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ide2.h"
#include "adamotemplates.h"
#include "WorkspaceWnd.h"
#include "OutputWnd.h"
#include "CallStack.h"
#include "Project.h"
#include "MDIClientWnd.h"
#include "VariablesBar.h"
#include "WatchBar.h"
#include "debugger.h"
#include "AdamoGlobal.h"
#include "adamoint.h"
#include "errorsdlg.h"
#include "FindDlg.h"
#include "ReplaceDlg.h"
#include "workspace.h"
#include "DockDevice.h"	// Added by ClassView
#include "DockDevProperty.h"
#include "DockConfigTree.h"
#include "ConfigDoc.h"
#include "AdvancedStatusBar.h"
#include "ColorStaticST.h"
#include "OscView.h"
#include "DockOscilloscope.h"
#include "DockDbgIOView.h"
#include "DockDbgVarView.h"
#include "DockDbgObjectView.h"
#include "DlgStrings.h"
#include "AdamoDbgIOView.h"
#include "AdamoDbgVarView.h"
#include "AdamoDbgObjectView.h"
#include "DockLangView.h"
#include "ConfigFrame.h"
#include "HTMLCtrl.h"
#include "AdamoFindInFilesView.h"
#include "AdamoReferenceView.h"
#include "FileFinder.h"
#include "DockRS.h"
#include "DGSdoView.h"
#include "DockDGSdo.h"
#include "DockEcMailBox.h"
#include "AdamoConfigHWView.h"
#include "DockConfigHW.h"
#include "basedialog.h"
#include "DockHelpOnLine.h"
#include "DockHelpOnLineHtml.h"
#include "DockThiraLuaObjects.h"
#include "DockListErrors.h"

#define ID_MAINTIMER          100
#define MAINTIMER_ELAPSEDTIME 100
#define IMG_DIR               "PathImages"
#define HTML_DIR              "PathHTML"
#define TIMEOUT_CONNECTION   1000
#define NMAX_DIALOGLIST      8

#define LOADSTRING(x) (((CMainFrame *)AfxGetMainWnd ())->LoadString (x))
#define PompaIMessaggi ((CMainFrame *)AfxGetMainWnd ())->PompaFuoriIMessaggi
#define WM_NOTIFY_ITSME		"__THIRA__UELAAA__ITSME__"
#define MSG_CAMBIOLINGUA	"AFX_CAMBIOLINGUA_H__12C3C63D_20FD_4DB8_8A2F_06B9D7D6DB79"
#define MSG_REFRESH_METRICS "WM_REFRESHMETRICS"
#define NUMERO_TENTATIVI	2
#define DIR_LIBRARIES "LIBRARY"

bool WaitConnection (CAdamoMachine *pMachine);
int  filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);

enum eWhatIAmDoing   {
    eDone,
    eCloseApplication,
    eCloseDebuggingSession,
    eClosePlant,
    eOpeningMode,
    eClosingMode
};

struct stThiraErrore : stAdamoErrore   {
	CString m_strModule;
};

struct stThiraStringElement : stStringElement   {
	CString m_strModule;
};

class CAdamoXTPTabClientWnd : public CXTPTabClientWnd   {
	public :
		CAdamoXTPTabClientWnd ()   {
		}
	protected :
		CXTPTabManagerItem* AddItem (CWnd* pChildWnd);
};

struct VERSION {
	DWORD major;
	DWORD minor;
	DWORD hotfix;
	DWORD other;
};

class CMainFrame : public CXTPMDIFrameWnd, CXTPCommandBarsFrameHook
{
    friend class CDebugger;
	DECLARE_DYNAMIC(CMainFrame)
    HINSTANCE       m_hInst;
    HINSTANCE       m_hItalianResources;
public:
	CFindDlg	*m_pFindDlg;
	CReplaceDlg	*m_pReplaceDlg;
	CMainFrame();
// Attributes
public:
	enum {
        modeNoPlantNoProject,
        modeNoProject,
        modeBuild,
        modeExecute,
        modeDebug,
        modeDebugBreak,
        modeConfig,
        modeManDiag,
        modeMachine,
        modeHardware,
        modeLogic,
        modeShowErrors,
        modeResources,
        modeDiagnostic,
	};

protected:
	CAdamoXTPTabClientWnd m_MTIClientWnd;
    CErrorsDlg*         m_pErrorsDlg;
    CDlgStrings*        m_pStringsDlg;
	CWorkspaceWnd	    m_wndWorkspace;
	CErrorWnd		    m_wndOutput;
	CDebugWnd		    m_wndDebug;
    CAdamoFindInFilesView m_wndFFV;
	CAdamoReferenceView m_wndReferences;
	CCallStack		    m_wndCallStack;
	CDebugger           m_debug;
	CVariablesBar	    m_wndLocals;
	CWatchBar		    m_wndWatches;
    CDockDevProperty    m_wndDockDevProperty;
    CDockDevice         m_wndDockDev;
    CDockConfigTree     m_wndDockConfigTree;
    CDockOscilloscope   m_wndDockOscilloscope;
    CDockDbgIOView      m_wndDockDbgIO;
    CDockDbgVarView     m_wndDockDbgVar;
    CDockDbgObjectView  m_wndDockDbgObject;
    CDockLangView       m_wndDockLangView;
    CDockRSCatalogue    m_wndDockRSCatalogue;
    CDockRSProperties   m_wndDockRSProperties;
    CDockSGSDO          m_wndDockDGSdo;
    CDockConfigHW       m_wndDockConfigHW;
	CDockEcMailBox		m_wndDockEcMailBox;
	CDockHelpOnLine     m_wndHelpOnLine;
	CDockHelpOnLineHtml m_wndHelpOnLineHtml;
	CDockThiraLuaObjects m_wndThiraLuaObjects;
	CDockRSActiveXPreview m_wndDockRSActiveXPreview;
	CDockListAllErrors  m_wndDockListAllErrors;
	CDockListInterpreterErrors m_wndDockListInterpreterErrors;
	CDockListPlcErrors  m_wndDockListPlcErrors;
	CDockListISOErrors  m_wndDockListISOErrors;
	CDockListHardwareErrors m_wndDockListHardwareErrors;
	CDockListMessages   m_wndDockListMessages;
	HACCEL m_hAccelBuild, m_hAccelDebug, m_hAccelDebugBreak, m_hAccelNoProject,
                        m_hAccelConfig, m_hAccelResources;
	int m_nAppMode;
	enum {  enCOMDRAG = 1 /*finito drop*/, 
            enCOMDELETE = 2 /*delete drag*/
    };
	BOOL m_bFullScreen;
	CRect m_rcMainFrame;
	UINT m_nIconColor;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	//}}AFX_VIRTUAL
    afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInitCommandsPopup(CXTPPopupBar* pCoomandBar);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
// Implementation
public:
	afx_msg void OnCloseMode();
    POSITION AddEditView (CView* pActiveView, CView* pView);
    void RemoveEditView (POSITION pos);
    CLuaView* GetPrevEditView (CView* pView);
	void SaveFileData();
    void OnShowGlobalVariable (stGlobalDescriptor* pGD);
	void OnButtonDeleteErrorBar();
	void OnButtonString();
	int OnSci(CWnd* pView, SCNotification* pNotify);
 	void SwitchMenu ();
	BOOL ShowWindowEx(int nCmdShow);
	//CProject *NewProject();
	void UpdateActiveProject();
	BOOL GetCalltip(const char* szWord, CString& strCalltip);
	void SetMode(int nMode);
	CView* GotoFileLine(const char* szFile, int nLine, bool bActivateView=false, bool bShowMarker = true);
	LRESULT DebugMessage(UINT nMsg, WPARAM wParam, LPARAM lParam);
	CView* GetActiveView();
	int GetMode() { return m_nAppMode; };
	COutputWnd* GetOutputWnd() { return &m_wndOutput; };
	COutputWnd* GetDebugWnd()  { return &m_wndDebug; };
	CProject* GetProject() { return GetWorkSpace()->GetActiveProject(); };
	CWorkspace* GetWorkSpace() { return GETWS (); };
	CWorkspaceWnd* GetWorkspaceWnd() { return &m_wndWorkspace; };
	CCallStack* GetCallStack() { return &m_wndCallStack; };
	CDockDevProperty* GetDockDevProperty() { return &m_wndDockDevProperty; };
	CDockDevice* GetDockDevice() { return &m_wndDockDev; };
	CDockConfigTree* GetDockConfigTree() { return &m_wndDockConfigTree; };
	CDebugger* GetDebugger() { return & m_debug; };
    CDockLangView*  GetDockLangView ()   { return &m_wndDockLangView; }
    CDockRSCatalogue*   GetDockRSCatalogue ()   { return &m_wndDockRSCatalogue; }
    CDockRSProperties*  GetDockRSProperties ()   { return &m_wndDockRSProperties; }
    CDockRSActiveXPreview* GetDockRSActiveXPreview ()   { return &m_wndDockRSActiveXPreview; }
	CDockConfigHW*  GetDockConfigHW ()   { return &m_wndDockConfigHW; }
	CDockEcMailBox* GetDockEcMailBox ()  { return &m_wndDockEcMailBox; }
	CDockHelpOnLine* GetDockHelpOnLine ()  { return &m_wndHelpOnLine; }
	CDockHelpOnLineHtml* GetDockHelpOnLineHtml ()  { return &m_wndHelpOnLineHtml; }
	CDockThiraLuaObjects* GetDockThiraLuaObjects ()  { return &m_wndThiraLuaObjects; }
	CDockListAllErrors* GetDockListAllErrors ()	{ return &m_wndDockListAllErrors; }
	CDockListInterpreterErrors* GetDockListInterpreterErrors ()	{ return &m_wndDockListInterpreterErrors; }
	CDockListPlcErrors* GetDockListPlcErrors ()	{ return &m_wndDockListPlcErrors; }
	CDockListISOErrors*  GetDockListISOErrors ()	{ return &m_wndDockListISOErrors; }
	CDockListHardwareErrors* GetDockListHardwareErrors ()	{ return &m_wndDockListHardwareErrors; }
	CDockListMessages* GetDockListMessages ()	{ return &m_wndDockListMessages; }
	CAdamoStatusBar* GetStatusBar ()	 { return &m_wndStatusBar; }
    void SetNumFF (int n)   { m_nFFOccurences=n; }
    int  GetNumFF ()        { return m_nFFOccurences; }
    void AddNumFF (int n)   { m_nFFOccurences+=n; }
    void LoadWorkSpace ();
    void CloseWorkSpace ();
	void UpdateFrameTitleForDocument(LPCTSTR lpszDocName);
    CFont* CreateFont (const char* strFont, int nHeight, int nBold);
    CFont* GetProgramFont ()  { return m_pFontTahoma; };
    CFont* GetProgramFontBold () { return m_pFontTahomaGrassetto; }
    void DrawBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart);
    void DrawTransparentBitmap (HDC hdc, HBITMAP hBitmap, short xStart, short yStart, COLORREF cTransparentColor);
	void StretchBitmap (HDC hdc, HBITMAP hBitmap, short x, short y, short w, short h, 
								short xSrc, short ySrc, short wSrc, short hSrc);
    void ParseStrError (const char* strErrore, CString* pstrModule, 
        CString* pstrLine, CString* pstrError);
    void OpenTemplate (int nMode);
    int  GetTemplate ()  { return m_pCT ? m_pCT->KindOf () : enNoTemplate; }
	virtual ~CMainFrame();
    void SetLayout (CXTPDockingPaneLayout* pLayOut);
    CXTPDockingPaneLayout* GetLayoutHW(){ return m_objLayoutHW; }
    CXTPDockingPaneLayout* GetLayoutConfig(){ return m_objLayoutConfig; }
    CXTPDockingPaneLayout* GetLayoutResources(){ return m_objLayoutResources; }
    CXTPDockingPaneManager*  GetPaneManager(){ return &m_paneManager; }
    void NewProject (CProject* pPrj);
    void ClearGotoLines ();
    void SetReadOnly (bool bRead);
    bool CheckParameters ();
    void CreateAxesList (CPtrList& rAxesList);
    void Iterator(CAdamoGroup *pGrp, CPtrList& rAxesList);
    void InsertAx (CAdamoAxis *pDev, CPtrList& rAxesList);
    void OnSelectMonth (int nMonth);
    CString GetDeviceString (int nTypeDevice);
    enTemplateTypes  WhatIAmGoingToOpen ()   { return m_enIAmGoingToOpen; }
    void GetDeviceErrorString (int nDevice, int nTypeDevice, char* szBuffer);
    void GetTypeDeviceErrorString (int nTypeDevice, char* szBuffer);
    CSize GetScreenResolution ();
    bool MatchString (const char* strCmp, const char* str, bool bMatchWord, bool bUpperCase);
    void IAmGoingTo (eWhatIAmDoing eWhatIAmGoingToDo);
    void IHaveDoneIt ();
    eWhatIAmDoing WhatIAmGoingToDo ();
    void PompaFuoriIMessaggi ();
    void SetXTPStyles ();
    CLuaEditor* GetActiveLuaEditor ();
    void CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    void CheckIpAddress (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    int  CheckIPAddressValidity (CString str);
    CString LoadString (UINT nId);
    CString GetCurrentLanguage ();
    void PrintHexBuffer (unsigned char *pData, int nLengthData, unsigned char *pShowBuffer);
    void ScanHexBuffer (unsigned char *pData, int nLengthData, unsigned char *pBuffer, int *nLengthBuffer);
    CString GetPathImages ();
    CxImage* LoadPathImages (CString strPath, DWORD imageType);
    void SetStringToSearch (CString strLastSearch);
    void Connect ();
    void Disconnect (const char *szName);
    int  GetNodeFromHWAddress (const char *szBuffer);
    int  GetChannelFromHWAddress (const char *szBuffer);
    int  GetPinFromHWAddress (const char *szBuffer);
    void LoadSSCNETToolbar ();
	void LoadEthercatToolbar ();
    void LoadCommToolbar ();
    void RemoveCommToolbar ();
    void OnBuildGoFromMD ();
    void OnDebugStopdebuggingFromMD ();
    CString CorreggiPercorsoAssoluto   (CString strPath, CString  str);
    bool ExistDialogList (CString strName);
    void AddDialogList (CAdamoDialog *dv);
    void TimerDialogList ();
    void RemoveDialogList (CString strName="");
    void CloseDialogList (CString strName="");
    int  CountDialogList ()                  { return m_dlgList.GetCount (); }
    void MoveDialogList (CAdamoDialog *dv);
    void DelayAdamoTimer (int nDelayTimer)   { m_nDelayTimer = nDelayTimer; }
    bool CheckIfADACanEdit ();
    ePswLevel GetPswLevel ()                 { return m_ePswLevel; }
    void SwitchModeToolbar ();
	BOOL InitDockingWindows();
	void MoveDlgButton (CButton &btn, int xTop, int yLeft, int xRight, int  yBottom);
	void SendMatrixData (CString strMatrixName, int nRow, CStringArray &keys, CAdamoData* pData);
	int  SetMatrix (CString szVar, int nRow, int nNumColumns, int nBufKeys, char* pBufKeys, CAdamoData* pData);
	int  GetStringaCodiceDatiAsse (int n);
	int  GetCodiceStringaDatiAsse (int n);
        int  ConnectToTcpIp ();
	int	 ConnectToLocal ();
	bool ExistEthercat ();
	void OnForceAllInputsOff ();
	CProgressCtrl *GetCanOpenProgressBar ()		{ return &m_wndProgCtrl; }
	CProgressCtrl *GetEthercatProgressBar ()	{ return &m_wndEcScanProgress; }
	CColorStaticST *GetEthercatSlaveName ()		{ return &m_wndEcScanDevice; }
	void AddErrorBar (const char* sz, int nTypeError);
	CString GetPathHelpOnLine ();
	void LoadDockingProject (CProject* pProject);
	void UpdateDockingProject (CProject* pProject);
	void DeleteHelpOnLineCustomFunction (CString strModule, CString strFunction);
	void OnBuildCleanInfo ();
	int  ParseDottedString (CString str, CStringArray& strArray);
	CString GetLibrariesPathName ();
	void SetSkin (CString& strSkin);
	void LoadSkin ();
	CString ChainMachineError (char *pErr);
	void ResizeBitmap(CBitmap& bmp_src, CBitmap& bmp_dst, int dstW, int dstH);
	int GetFileVersion(CPath pFilePath, VERSION* pRetVersion);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
    CXTPDockingPaneLayout   *m_objLayoutEmpty;
    CXTPDockingPaneLayout   *m_objLayoutEdit;
    CXTPDockingPaneLayout   *m_objLayoutDebug;
    CXTPDockingPaneLayout   *m_objLayoutConfig;
    CXTPDockingPaneLayout   *m_objLayoutManDiag;
    CXTPDockingPaneLayout   *m_objLayoutMachine;
    CXTPDockingPaneLayout   *m_objCurrentLayout;
    CXTPDockingPaneLayout   *m_objLayoutHW;
    CXTPDockingPaneLayout   *m_objLayoutResources;
    CXTPDockingPaneLayout   *m_objLayoutHWDiagnostic;
    CXTPDockingPaneLayout   *m_objLayoutParametric;
	CXTWindowPos            m_wndPosition;
	CAdamoStatusBar         m_wndStatusBar;
    CXTPDockingPaneManager  m_paneManager;
    CColorStaticST          m_wndEcScanDevice;
	CProgressCtrl			m_wndProgCtrl;
	CProgressCtrl			m_wndEcScanProgress;
	CParsedEdit				m_wndEditBrowseSymbol;
    CXTPToolBar             *m_pModulesBar; 
    CXTPToolBar             *m_pModeBar; 
    CXTPToolBar             *m_pErrorBar;
    CXTPToolBar             *m_pEditBar;
    CXTPToolBar             *m_pConfEditBar;
    CXTPToolBar             *m_pConfDevicesBar;
    CXTPToolBar             *m_pEditFindBar;
    CXTPToolBar             *m_pBuildBar;
    CXTPToolBar             *m_pDebugBar;
    CXTPToolBar             *m_pMessageBar;
    CXTPToolBar             *m_pBrowseBar;
    CXTPToolBar             *m_pOscBar;
    CXTPToolBar             *m_pPlcBar;
    CXTPToolBar             *m_pSEBar;
    CXTPToolBar             *m_pRSBar;
    CXTPToolBar             *m_pRSControlsBar;
    CXTPToolBar             *m_pRSFormatBar;
    CXTPToolBar             *m_pRSTools;
    CXTPToolBar             *m_pDGBar;
    CXTPToolBar             *m_pCanOpenBar;
    CXTPToolBar             *m_pNextToolBar;
    CXTPToolBar             *m_pConfHW;
    CXTPToolBar             *m_pECConfHW;
    CXTPToolBar             *m_pDGSSCNET;
    CXTPToolBar             *m_pDGEthercat;
	CXTPControlEdit			*m_pEditBrowse;
// Generated message map functions
protected:
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnAreYouMe(WPARAM wParam,LPARAM lParam);
    void ClearClipBoard();
    void OnShowView(UINT nID);
    afx_msg LRESULT OnThereIsAnError (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeleteErrors (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnOpenDialogBars (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCloseDialogErrors (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCloseDialogStrings (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdamoConnected (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdamoDisconnected (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnThereIsAString (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnProgramStop (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRequestResources (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRequestCCPlanes (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCloseDialogFind (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCloseDialogReplace (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnChangePsw (WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
    afx_msg void OnUpdateCmdForButtonDynamic(CCmdUI* pCmdUI);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnProjectAddFiles();
	afx_msg void OnProjectAddLibrary();
	afx_msg void OnProjectProperties();
	afx_msg void OnClose();
	afx_msg void OnBuildBuild();
	afx_msg void OnBuildCompile();
	afx_msg void OnUpdateBuildBuild(CCmdUI* pCmdUI);
	afx_msg void OnBuildRebuildall();
	afx_msg void OnBuildClean();
	afx_msg void OnBuildGo();
    afx_msg void OnUpdateBuildStop (CCmdUI* pCmdUI) ;
    afx_msg void OnUpdateEditReplace (CCmdUI* pCmdUI) ;
	afx_msg void OnUpdateDebugBreak(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
	afx_msg void OnDebugGo();
	afx_msg void OnDebugStepinto();
	afx_msg void OnDebugStepover();
	afx_msg void OnDebugStepout();
	afx_msg void OnDebugRuntocursor();
	afx_msg void OnDebugBreak();
	afx_msg void OnDebugStopdebugging();
	afx_msg void OnDebugRestart();
	afx_msg void OnUpdateCmdForProject(CCmdUI* pCmdUI);
	afx_msg void OnBuildExecute();
	afx_msg void OnHelpContactauthor();
	afx_msg void OnHelpLuahelppdf();
	afx_msg void OnHelpVisithomepage();
	afx_msg void OnHelpLuahomepage();
	afx_msg void OnDestroy();
	afx_msg void OnBuildStop();
    afx_msg void OnTimer(UINT nIDEvent); 
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnFileAddproject();
	afx_msg void OnFileOpenplant();
	afx_msg void OnFileSaveplant();
	afx_msg void OnUpdateCmdForPlant(CCmdUI* pCmdUI);
	afx_msg void OnFileCloseplant();
	afx_msg void OnDebugViewslots();
	afx_msg void OnBuildViewVar();
	afx_msg void OnDebugViewVar();
	afx_msg void OnDbgBreakViewVar();
	afx_msg void OnBuildViewMbx();
	afx_msg void OnDebugViewMbx();
	afx_msg void OnDbgBreakViewMbx();
	afx_msg void OnViewDevice();
    afx_msg void OnViewProDevice();
	afx_msg void OnUpdateFindReplace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFile(CCmdUI* pCmdUI);
	afx_msg void OnButtonError();
        afx_msg void OnViewRestoredocking();
	afx_msg void OnFileOption();
	afx_msg void OnFileImport();
	afx_msg void OnFileImportXml();
	afx_msg void OnFileExportXml();
    afx_msg void OnUpdateFileOption (CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileImport (CCmdUI* pCmdUI);
    afx_msg void OnUpdateFileImportXml (CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportXml (CCmdUI* pCmdUI);
	afx_msg void OnOpenConfig();
	afx_msg void OnUpdateOpenConfig(CCmdUI* pCmdUI);
	afx_msg void OnOpenEditDebug();
	afx_msg void OnUpdateOpenEditDebug(CCmdUI* pCmdUI);
	afx_msg void OnOpenManDiag();
	afx_msg void OnUpdateOpenManual(CCmdUI* pCmdUI);
	afx_msg void OnDownloadAll();
	afx_msg void OnDownloadParameters();
	afx_msg void OnUpdateCloseMode(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnViewPrevdocking();
	afx_msg void OnViewNextdocking();
	afx_msg void OnOpenMachine();
	afx_msg void OnUpdateOpenMachine(CCmdUI* pCmdUI);
	afx_msg void OnAttachDebugger();
	afx_msg void OnDetachDebugger();
	afx_msg void OnUpdateAttachDebugger(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDetachDebugger(CCmdUI* pCmdUI);
	afx_msg void OnBuildStepInto();
	afx_msg void OnBuildRunToCursor();
	afx_msg void OnUpdateBuildStepInto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildRunToCursor(CCmdUI* pCmdUI);
    afx_msg void OnSysColorChange();
    afx_msg void OnNewMessage();
    afx_msg void OnDeleteMessage();
    afx_msg void OnModifyMessage();
	afx_msg void OnUpdateNewMessage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModifyMessage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteMessage(CCmdUI* pCmdUI);
	afx_msg void OnDownloadStrings();
    afx_msg void OnOscStart();
    afx_msg void OnOscStartPretrigger();
    afx_msg void OnOscStop();
    afx_msg void OnOscClear();
    afx_msg void OnOscZoomToFit();
    afx_msg void OnOscOptions();
    afx_msg void OnOscShowCursor();
    afx_msg void OnOscShowDeltaXCursor();
    afx_msg void OnOscShowDeltaYCursor();
    afx_msg void OnOscLoad();
    afx_msg void OnOscSave();
    afx_msg void OnOscPrint();
    afx_msg void OnUpdateOscStart(CCmdUI* pCmdUI);
    afx_msg void OnUpdateStartPretrigger(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscStop(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscClear(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscOptions(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscZoomToFit(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscShowCursor(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscShowDeltaXCursor(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscShowDeltaYCursor(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscLoad(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscSave(CCmdUI* pCmdUI);
    afx_msg void OnUpdateOscPrint(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewDevice(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewPropDevice(CCmdUI* pCmdUI);
    afx_msg void OnViewHardware();
    afx_msg void OnUpdateViewHardware(CCmdUI* pCmdUI);
	afx_msg void OnShowErrors();
	afx_msg void OnUpdateOpenShowErrors(CCmdUI* pCmdUI);
	afx_msg void OnOpenResource();
	afx_msg void OnUpdateOpenResource(CCmdUI* pCmdUI);
	afx_msg void OnEditFindInFiles();
	afx_msg void OnViewMainDocking();
	afx_msg void OnBuildNextError();
	afx_msg void OnBuildPrevError();
	afx_msg void OnOpenDiagnostic();
	afx_msg void OnUpdateOpenDiagnostic(CCmdUI* pCmdUI);
	afx_msg void OnBuildSearchText();
	afx_msg void OnOpenParametric();
	afx_msg void OnUpdateOpenParametric(CCmdUI* pCmdUI);
	afx_msg void OnEditBreakPoints();
	afx_msg void OnUpdateEditBreakPoints(CCmdUI* pCmdUI);
	afx_msg void OnDebugRunSlots();
	afx_msg void OnUpdateDebugViewslots(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDebugRunslots(CCmdUI* pCmdUI);
	afx_msg void OnDebugClearTrace();
	afx_msg void OnUpdateDebugClearTrace(CCmdUI* pCmdUI);
	afx_msg void OnBuildBreak();
	afx_msg void OnUpdateBuildBreak(CCmdUI* pCmdUI);
   	afx_msg void OnUpdatePswLevel(CCmdUI* pCmdUI);
    afx_msg void OnUpdateProjectAddFiles(CCmdUI* pCmdUI);
    afx_msg void OnUpdateProjectProperties(CCmdUI* pCmdUI);
	//}}AFX_MSG
   	afx_msg void OnCustomize();
    afx_msg void OnChooseErrorLogMonth(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSetAllCanNetwork(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSetAllEthercatNetwork(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateChooseErrorLogMonth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetAllCanNetwork(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSetAllEthercatNetwork(CCmdUI* pCmdUI);
    afx_msg LRESULT OnAdamoGStartDownload (WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnAdamoGDownloaded (WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateSeFilterErrors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSeClearFilter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSeGotoLine(CCmdUI* pCmdUI);
	afx_msg void OnSeFilterErrors();
	afx_msg void OnSeClearFilter();
	afx_msg void OnSeGotoLine();
	afx_msg void OnGotoDefinition();
	afx_msg void OnGotoReference();
    afx_msg void OnSymbolToBrowse(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteBrowseSymbol();
	afx_msg void OnUpdateGotoDefinition(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGotoReference(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSymbolToBrowse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteBrowseSymbol(CCmdUI* pCmdUI);
	afx_msg void OnGotoNextReference ();
	afx_msg void OnGotoPrevReference ();
	afx_msg void OnGotoHelpFunction ();
	afx_msg void OnUpdateGotoHelpFunction(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private :
    CAdamoTemplate* m_pCT;
    CFont*          m_pFontTahoma;
    CFont*          m_pFontTahomaGrassetto;
    CString         m_strCrtMod; 
    int             m_nTabCount;
    bool            m_bCtrlPressed;
    CLuaView*       m_pCtrlView;
    CPtrList        m_lViews;
    stAdamoErrore*  m_pErrorOnBar;
    UINT            m_nTimer;
    CFileFinder	    m_objFinder;
    bool            m_bSearching;
    int             m_nFFOccurences;
    enTemplateTypes m_enIAmGoingToOpen;
    eWhatIAmDoing   m_eIAmDoing;
    CString         m_lastSearch;
    CProjectFile   *m_pBreakedPrjFile;
    int             m_nBreakedPrjLine;
    CString         m_strUser;
    ePswLevel       m_ePswLevel;
    CPtrList        m_dlgList;
    int             m_nDelayTimer;
    int             m_nTimerCounter;
	int             m_nClientConnected;
	int				m_nCx;
	int				m_nCy;
	int				m_nRibbonStyle;
private :
    void CloseDialogErrors ();
    void CloseDialogStrings ();
    void SendDebugData (const char* szSlotName);
    void AttachDebugData (const char* szSlotName);
    void StartDebugging (int nCmd);
    void StartDebugging (int nCmd, int nParameter, int nModule, const char* szModule);
    void StopDebugging ();
    void ShowGlobalData ();
    void ShowMbx ();
    CString GetEditorWord ();
    void ShowGlobalVariable (struct stGlobalDescriptor* pGD);
    void ShowGlobalArray (struct stGlobalDescriptor* pGD);
    void ShowGlobalMatrix (struct stGlobalDescriptor* pGD);
    void ShowGlobalOrLocalVariable (CString strName, int nType, int nGL, int nLevel, int nProg);
    void ShowGlobalOrLocalTable (CString strName, int nGL, int nLevel, int nProg);
	void AddStringBar (const char* sz);
    void DeleteStringBar ();
    void GetErrorString (char* szSource, char** szDest);
    void ResetLayout ();
    void CloseTemplate (CAdamoTemplate* pTmp);
    void CloseDocument (CAdamoTemplate* pTmp);
	LRESULT OpenDialogErrors ();
	LRESULT OpenDialogStrings ();
    void SetInitialEditDebugMode ();
    void UpdateDockingBars ();
    void OpenToolBars ();
    void CloseToolBars ();
    void SwitchToolBars ();
    void DownLoadCC (CProject* pPrj);
    void DownLoadPC (CProject* pPrj);
	void DownLoadModuleParams ();
    void OnSearch (CString strFind, CString strFileType, CString strFolder, bool bMatchCase, bool bMatchWords, bool bMatchRegEx);
    static void FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam);
    void CloseModelessDialogs ();
    void IAmGoingToOpen      (enTemplateTypes en);
    bool CheckIfIDECanWork ();
    void InitLanguages ();
    void SetLanguage (CString strLanguage);
    void FocusOnEditor ();
    void SetToolTips ();
    void SetInitialBreakLine (CCmdGetDebugData *pDD);
    void OpenBreakedView ();
    void GestioneTrace (stThiraErrore *ae);
	void ManageUserPsw (ePswLevel pswVecchia, ePswLevel pswNuova);
	void ManageServicePsw (ePswLevel pswVecchia, ePswLevel pswNuova);
	void OnOpenPswConfig ();
	void OnOpenPswEditDebug ();
	void OnOpenPswManDiag ();
	void OnOpenPswShowErrors ();
	void OnOpenPswResource ();
	void OnOpenPswDiagnostic ();
	void AddSEView (stAdamoErrore *p);
	void MotorizzaMacchine ();
	void DeleteXTRegistry();
	void RecreateLeyout();
	void DeleteXTPanes();
	void BackupImportedFiles ();
	void CreateReferencePane (CLuaView *pView); 
	void ResizeErrorBar ();
	void AddProjectFilesName ();
	void SetCommandBarsTheme(XTPPaintTheme paintTheme, LPCTSTR lpszINI = NULL);
	void SetDockingPaneTheme(XTPDockingPanePaintTheme paintTheme, int nStyle = -1, int nMargin = 0);
	XTPToolTipStyle GetToolTipStyle() const;
	void OnTheme(UINT nID);
	BOOL SetFrameIcon(UINT nID, CSize szIcon);
	void LoadIcons();
	void GestioneErrore (stThiraErrore* p);
	void GestioneCancellazioneErrore (stThiraErrore* p);
	void AddHardwareError (stThiraErrore* p);
	void AddInterpreterError (stThiraErrore* p);
	void AddCycleError (stThiraErrore* p);
	void AddISOError (stThiraErrore* p);
	void AddEvent (stThiraErrore* p);
	void GestioneStringa (stThiraStringElement* p);
	void GestioneCancellazioneStringa (stThiraStringElement* p);
public:
	afx_msg void OnConnectTo();
	afx_msg void OnConnections();
	afx_msg void OnUpdateConnectTo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConnections(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__CCF97055_F1B3_48F3_B536_F23CF34360E9__INCLUDED_)
