// Debugger.h: interface for the CDebugger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGGER_H__344DB359_4B39_4DAF_BF7D_B2BFE030E7DB__INCLUDED_)
#define AFX_DEBUGGER_H__344DB359_4B39_4DAF_BF7D_B2BFE030E7DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LuaHelper.h"
class CLuaEditor;
class CProject;

#include "DebuggerMessages.h"
#include "AdamoMachine.h"

#define DMOD_NONE					0
#define DMOD_STEP_INTO				1
#define DMOD_STEP_OVER				2
#define DMOD_STEP_OUT				3
#define DMOD_RUN_TO_CURSOR			4

#define DMOD_BREAK					10
#define DMOD_STOP					11

typedef struct
{
	const char* szDesc;
	const char* szFile;
	int nLine;
} StackTrace;

typedef struct
{
	const char* szName;
	const char* szType;
	const char* szValue;
} Variable;

class CDebugger  
{
public:
	void Execute();
	CString Eval(CString strCode);
	static void EndThread();
	BOOL GetCalltip(const char* szWord, char* szCalltip);
	void AddLocalVariable(const char* name, const char* type, const char* value);
	void ClearLocalVariables();
	void AddGlobalVariable(const char* name, const char* type, const char* value);
	void ClearGlobalVariables();
	void StackLevelChanged();
	void Break();
	void Stop();
	void DebugBreak(const char* szFile, int nLine);
	void LineHook(const char* szFile, int nLine);
	void FunctionHook(const char* szFile, int nLine, BOOL bCall);
	void Write(const char* szMsg);
	BOOL Start();
	BOOL Prepare();
	CDebugger();
	virtual ~CDebugger();

	void Go();
	void StepInto();
	void StepOver();
	void StepOut();
	void RunToCursor();

	void ClearStackTrace();
	void AddStackTrace(const char* strDesc, const char* strFile, int nLine);
	int GetStackTraceLevel();

    void SetImpianto (CAdamoMachine* pMachine);
    void DrawStackTrace ();
    void DrawLocalVariables ();

	static CDebugger* GetDebugger() { return m_pDebugger; };
	lua_State* GetLuaState() { return m_lua.GetState(); };
	HWND GetMainWnd() { return m_hWndMainFrame; };
protected:
	static UINT StartDebugger( LPVOID pParam );	

	CLuaHelper m_lua;
	HWND m_hWndMainFrame;
	CEvent m_event;

	int m_nMode;
	CString m_strPathName;
	int m_nLine, m_nLevel;
	CWinThread* m_pThread;

	static CDebugger* m_pDebugger;

    CAdamoMachine* m_pImpianto;
private :
    /* helper functions */
    void FormatCallStack (CCmdGetStackData* pSD, char* szBuffer);
};

#endif // !defined(AFX_DEBUGGER_H__344DB359_4B39_4DAF_BF7D_B2BFE030E7DB__INCLUDED_)
