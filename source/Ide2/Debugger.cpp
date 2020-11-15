// Debugger.cpp: implementation of the CDebugger class.
//
//////////////////////////////////////////////////////////////////////

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "Debugger.h"
#include "MainFrame.h"
#include "ScintillaView.h"
#include "LuaDoc.h"
#include "LuaView.h"
#include "lua.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugger* CDebugger::m_pDebugger = NULL;

CDebugger::CDebugger()
{
	m_pDebugger = this;
}

CDebugger::~CDebugger()
{

}

BOOL CDebugger::Prepare()
{
    CAdamoDbgVarView* pDbgVarView;
	m_hWndMainFrame = AfxGetMainWnd()->GetSafeHwnd();

	COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();
    COutputWnd* pDebug = ((CMainFrame*)AfxGetMainWnd())->GetDebugWnd();

	pOutput->Clear ();
    pDebug->Clear ();

	CProject *pProject = GETWS()->GetActiveProject();
	if ( pProject->PositionBreakPoints() )
		AfxMessageBox(LOADSTRING (IDS_ADAMODEBUGGER_1), MB_OK);
    m_pImpianto=pProject->GetMachine();
    CDockDbgVarView* wndDockDbgVar=&((CMainFrame*)AfxGetMainWnd ())->m_wndDockDbgVar;
    pDbgVarView=wndDockDbgVar->GetView ();
    if (pDbgVarView)
        pDbgVarView->UpdateData ();
    ((CMainFrame*)AfxGetMainWnd ())->m_strCrtMod="";
	return TRUE;
}

BOOL CDebugger::Start()
{
	m_pThread = AfxBeginThread(StartDebugger, 0);
	return m_pThread!=NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// thread functions
//////////////////////////////////////////////////////////////////////////////////////////////

UINT CDebugger::StartDebugger( LPVOID pParam )
{
	::SendMessage(m_pDebugger->m_hWndMainFrame, DMSG_DEBUG_START, 0, 0);
	int nRet = m_pDebugger->m_lua.StartDebugger();
	EndThread();
	return nRet;
}

void CDebugger::Write(const char* szMsg)
{
	::SendMessage(m_hWndMainFrame, DMSG_WRITE_DEBUG, (WPARAM)szMsg, 0);
}

void CDebugger::LineHook(const char *szFile, int nLine)
{
	if ( m_nMode == DMOD_STOP )
		EndThread();

	if ( ::SendMessage(m_hWndMainFrame, DMSG_HAS_BREAKPOINT, (WPARAM)szFile, (LPARAM)nLine) ||
		m_nMode==DMOD_STEP_INTO || 
		m_nMode==DMOD_BREAK ||
		(m_nMode==DMOD_STEP_OVER && m_nLevel<=0) || 
		(m_nMode==DMOD_STEP_OUT && m_nLevel<0) ||
		(m_nMode==DMOD_RUN_TO_CURSOR && m_strPathName.CompareNoCase(szFile)==0 && m_nLine==nLine) )
	{
		DebugBreak(szFile, nLine);		
	}
}

void CDebugger::FunctionHook(const char *szFile, int nLine, BOOL bCall)
{
	if ( m_nMode == DMOD_STOP )
		EndThread();

	m_nLevel += (bCall?1:-1);
}

void CDebugger::DebugBreak(const char *szFile, int nLine)
{
	m_nMode = DMOD_NONE;

	::SendMessage(m_hWndMainFrame, DMSG_GOTO_FILELINE, (WPARAM)szFile, (LPARAM)nLine);

	m_lua.DrawStackTrace();
	m_lua.DrawGlobalVariables();
	::SendMessage(m_hWndMainFrame, DMSG_REDRAW_WATCHES, 0, 0);
	::SendMessage(m_hWndMainFrame, DMSG_GOTO_STACKTRACE_LEVEL, 0, 0);

	m_event.ResetEvent();
	
	::SendMessage(m_hWndMainFrame, DMSG_DEBUG_BREAK, 0, 0);
	CSingleLock lock(&m_event, TRUE);

	if ( m_nMode == DMOD_STOP )
		EndThread();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void CDebugger::Go()
{
	m_event.SetEvent();
	::SendMessage(m_hWndMainFrame, DMSG_DEBUG_START, 0, 0);
}

void CDebugger::StepInto()
{
	m_nMode = DMOD_STEP_INTO;
	Go();
}

void CDebugger::StepOver()
{
	m_nMode = DMOD_STEP_OVER;
	m_strPathName = m_lua.GetSource();
	m_nLevel = 0;
	Go();
}

void CDebugger::StepOut()
{
	m_nMode = DMOD_STEP_OUT;
	m_strPathName = m_lua.GetSource();
	m_nLevel = 0;
	Go();
}

void CDebugger::RunToCursor()
{
    CMainFrame *pMainFrame = ((CMainFrame*)AfxGetMainWnd());
	CLuaView* pView = (CLuaView*)pMainFrame->GetActiveView();
	CLuaDoc* pDoc = pView->GetDocument();

	if ( !pDoc->IsInProject() )
		return;

	m_nMode = DMOD_RUN_TO_CURSOR;
	
	CProjectFile* pPF = pDoc->GetProjectFile();
	m_strPathName = pPF->GetPathName();

	int nLine = pView->GetEditor()->GetCurrentLine();
	m_nLine = pPF->GetNearestDebugLine(nLine);

	Go();
}

void CDebugger::Stop()
{
    ((CMainFrame*)AfxGetMainWnd ())->m_wndCallStack.Clear();
    ((CMainFrame*)AfxGetMainWnd ())->ClearGotoLines ();
}

void CDebugger::Break()
{
	m_nMode = DMOD_BREAK;
}

void CDebugger::ClearStackTrace()
{
	::SendMessage(m_hWndMainFrame, DMSG_CLEAR_STACKTRACE, 0, 0);
}

void CDebugger::AddStackTrace(const char* szDesc, const char* szFile, int nLine)
{
	StackTrace st;
	st.szDesc = szDesc;
	st.szFile = szFile;
	st.nLine = nLine;

	::SendMessage(m_hWndMainFrame, DMSG_ADD_STACKTRACE, (WPARAM)&st, 0);
}

int CDebugger::GetStackTraceLevel()
{
	return ::SendMessage(m_hWndMainFrame, DMSG_GET_STACKTRACE_LEVEL, 0, 0);
}

void CDebugger::StackLevelChanged()
{
	DrawLocalVariables();
}

void CDebugger::ClearLocalVariables()
{
	::SendMessage(m_hWndMainFrame, DMSG_CLEAR_LOCALVARIABLES, 0, 0);
}

void CDebugger::AddLocalVariable(const char *name, const char *type, const char *value)
{
	Variable var;
	var.szName = name;
	var.szType = type;
	var.szValue = value;

	::SendMessage(m_hWndMainFrame, DMSG_ADD_LOCALVARIABLE, (WPARAM)&var, 0);
}

void CDebugger::ClearGlobalVariables()
{
	::SendMessage(m_hWndMainFrame, DMSG_CLEAR_GLOBALVARIABLES, 0, 0);
}

void CDebugger::AddGlobalVariable(const char *name, const char *type, const char *value)
{
	Variable var;
	var.szName = name;
	var.szType = type;
	var.szValue = value;

	::SendMessage(m_hWndMainFrame, DMSG_ADD_GLOBALVARIABLE, (WPARAM)&var, 0);
}

BOOL CDebugger::GetCalltip(const char *szWord, char *szCalltip)
{
	//return m_lua.GetCalltip(szWord, szCalltip);
    return FALSE;
}

void CDebugger::EndThread()
{
	FreeConsole();
	::SendMessage(m_pDebugger->m_hWndMainFrame, DMSG_DEBUG_END, 0, 0);
	m_pDebugger->Write(LOADSTRING (IDS_ADAMODEBUGGER_2));
	m_pDebugger->m_lua.StopDebugger();
	AfxEndThread(0);
}

CString CDebugger::Eval(CString strCode)
{
    CCmdGetGlobalData gd;
	CString strRet;
    char szBuffer[256];
    int nLevel = GetStackTraceLevel();

	strCode = "return " + strCode;
    if (m_pImpianto->GetGlobalData (nLevel, (const char*)strCode, &gd) == S_OK)   {
        if (gd.nType==LUA_TNUMBER)
            sprintf (gd.szValue, "%8.6f", gd.fpValue);
        sprintf(szBuffer, "%s : %.64s", gd.szTypeName, gd.szValue);
    }
    else
        gd.szValue[0] = '\0';
	return CString (gd.szValue);
}

void CDebugger::Execute()
{
	CString strInterpreter = "F:\\a ja tak\\lubie cie bardzo\\lua.exe"; //theApp.GetModuleDir() + "\\" + "lua.exe";
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();

	_spawnl( _P_NOWAIT, strInterpreter, strInterpreter, "\"" + pProject->GetDebugPathNameExt() + "\"", NULL );
}

void CDebugger::SetImpianto (CAdamoMachine* pMachine)
{
    m_pImpianto=pMachine;        
}

void CDebugger::DrawStackTrace ()
{
    CPtrList pl;
    char szBuffer[256];
    ClearStackTrace();
    if (m_pImpianto->GetStackData (pl)==S_OK)   {
        for (POSITION pos=pl.GetHeadPosition(); pos;)   {
            CCmdGetStackData* pSD=(CCmdGetStackData*)pl.GetNext (pos);
            if (strcmp (pSD->szModule, "(luac)"))   {
                FormatCallStack (pSD, szBuffer);
                CDebugger::GetDebugger()->AddStackTrace(szBuffer, pSD->szModule, pSD->nLine);
            }
            delete pSD;
        }                         
    }
}

void CDebugger::DrawLocalVariables ()
{
    CPtrList pl;
	ClearLocalVariables();
	int nLevel = CDebugger::GetDebugger()->GetStackTraceLevel();
    char szValue[32];
    if (m_pImpianto->GetLocalData (pl, nLevel)==S_OK)   {
        for (POSITION pos=pl.GetHeadPosition(); pos;)   {
            CCmdGetLocalData* pLD=(CCmdGetLocalData*)pl.GetNext (pos);
			if (!strncmp (pLD->szLocalName, "__internal_switch__", strlen ("__internal_switch__")))
				continue;
            switch (pLD->nType)   {
                case LUA_TNUMBER :
                    sprintf (szValue, "%8.6f", pLD->fpValue);
                    CDebugger::GetDebugger()->AddLocalVariable(pLD->szLocalName, pLD->szTypeName, szValue);
                    break;
                case LUA_TSTRING :
                    CDebugger::GetDebugger()->AddLocalVariable(pLD->szLocalName, pLD->szTypeName, pLD->szValue);
                    break;
                case LUA_TBOOLEAN :
                    strcpy (szValue, pLD->fpValue ? LOADSTRING (IDS_ADAMOVERO) : LOADSTRING (IDS_ADAMOFALSO));
                    CDebugger::GetDebugger()->AddLocalVariable(pLD->szLocalName, pLD->szTypeName, szValue);
                    break;
                default :
                    CDebugger::GetDebugger()->AddLocalVariable(pLD->szLocalName, pLD->szTypeName, pLD->szValue);
                    break;
            }
            delete pLD;
        }                         
    }
}

void CDebugger::FormatCallStack (CCmdGetStackData* pSD, char* szBuffer)
{                                     
    char* p=strrchr (pSD->szModule, '\\');
    if (p)
        wsprintf (szBuffer, "%s %s %d", p+1, pSD->szFunctionName, pSD->nLine);
    else
        wsprintf (szBuffer, "%s %s %d", pSD->szModule, pSD->szFunctionName, pSD->nLine);
}