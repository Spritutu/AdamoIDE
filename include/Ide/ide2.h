// ide2.h : main header file for the IDE2 application
//

#if !defined(AFX_IDE2_H__A3FA6447_9B87_4B84_A3F1_D73F185A1AAC__INCLUDED_)
#define AFX_IDE2_H__A3FA6447_9B87_4B84_A3F1_D73F185A1AAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/* include files */
#include "resource.h"       // main symbols
#include "KSFileDialog.h"
#include "AdamoOptions.h"
#include "PaneColorSet.h"

/* constants definitions */
#define IMPIANTO_CORRENTE  "PathCurrent"
#define MUTEX_NOTIFY_ITSME "__THIRA__UELAAA__ITSME__MUTEX"

/* windows versions */
#define WINDOWS_XP		0
#define WINDOWS_VISTA	1
#define WINDOWS_7		2

/* enumeration */
enum enTemplateTypes {
    enNoTemplate,
    enConfTemplate,
    enEditDebugTemplate,
    enManDiagTemplate,
    enHTMLTemplate,
    enShowErrorsTemplate,
    enResourcesTemplate,
    enDiagnosticTemplate,
    enParametricTemplate
};

#define GETOPT() ((CIdeApp *)AfxGetApp ())->GetAdamoOptions ()

/////////////////////////////////////////////////////
// CIdeApp:
// See ide2.cpp for the implementation of this class
//

class CLuaView;
class CLuaDoc;
class CProjectFile;
class CWorkspace;
class CAdamoTemplate;

BOOL IsWindow (CWnd *pWnd);

class CIdeApp : public CXTPWinApp
{
public:
	CIdeApp();
    ~CIdeApp();
    CStringArray m_strFindstrings;
    CStringArray m_strReplacestrings;
    CStringArray m_strFindErrorsstrings;
    CStringArray m_strFindDiagnostic;
    CStringArray m_strFindInFilesTypes;
    CStringArray m_strFindInFilesFolders;
    CStringArray m_strFindConf;
	CString strSchema;
	CString strColor;
	AppTheme m_theme;
	CPaneColorSet m_csPane;
	BOOL FirstFileIsNewer(CString strPathName1, CString strPathName2);
	void DeleteAllFilesInCurrentDir();
	BOOL SaveModifiedDocuments();
	CLuaView* OpenProjectFilesView(CProjectFile* pPF, int nLine=-1, bool bShowMarker = true);
	CLuaView* LoadProjectFilesView(CProjectFile* pPF);
	CLuaView* FindProjectFilesView(CProjectFile* pPF);
	XTPPropertyGridPaintTheme GetXTPThemePropertyGrid ();

	CString GetModuleDir();
	void FormatMessage(char* pszAPI);
    CAdamoTemplate* GetTemplate (enTemplateTypes en);
    void CloseCurrentWorkspace ();
    CString     GetCurrentWorkspaceName ();
    CWorkspace* GetCurrentWorkspace ();
    void SetCurrentWorkspace (CWorkspace *pWrk)   { m_pWorkSpace=pWrk; }
	UINT GetWindowsVersion ()					  { return m_win; }
    UINT m_uiCopyClip;
	void ClosePlant() ;
    CAdamoOptions *GetAdamoOptions ()             { return &m_opt; }
    ePswLevel GetPswLevel ()					  { return m_ePswLevel; }
	CXTPModuleHandle GetModule2007 ()			  { return m_hModule2007; }
	CXTPModuleHandle GetModule2010 ()			  { return m_hModule2010; }
	CXTPModuleHandle GetModule2013 ()			  { return m_hModule2013; }
	CXTPModuleHandle GetModuleWindows7 ()		  { return m_hModuleWindows7; }
	CXTPModuleHandle GetModuleVisualStudio2012 () { return m_hModuleVisualStudio2012; }
	CXTPModuleHandle GetModuleVisualStudio2015 () { return m_hModuleVisualStudio2015; }
	AppTheme GetTheme ()						  { return m_theme; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CIdeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnPlants();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlants(CCmdUI* pCmdUI);
	afx_msg void OnClosePlant();
	afx_msg void OnUpdateClosePlant(CCmdUI* pCmdUI);
	//}}AFX_MSG
    virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
                              BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	DECLARE_MESSAGE_MAP()

protected:
	HMODULE m_hScintilla;
private :
	CXTPModuleHandle m_hModule2007;
	CXTPModuleHandle m_hModule2010;
	CXTPModuleHandle m_hModule2013;
	CXTPModuleHandle m_hModuleWindows7;
	CXTPModuleHandle m_hModuleVisualStudio2012;
	CXTPModuleHandle m_hModuleVisualStudio2015;
private :
	UINT m_win;
    CWorkspace* m_pWorkSpace;
    HANDLE m_hMutexOneInstance;
    CAdamoOptions m_opt;
    ePswLevel     m_ePswLevel;
private :
    void                 InitialLoad ();
    static BOOL CALLBACK searcher(HWND hWnd, LPARAM lParam);
    bool                 CheckThiraAlreadyRunning ();
	bool				 LoadDockings ();
	int					 GetPrivateWindowsVersion ();
	int					 ParseCommandLine ();
	void				 LoadXTPModules ();
};

extern CIdeApp theApp;

#define GETWINDOWSVERSION() (((CIdeApp *) AfxGetApp ())->GetWindowsVersion ())

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDE2_H__A3FA6447_9B87_4B84_A3F1_D73F185A1AAC__INCLUDED_)
