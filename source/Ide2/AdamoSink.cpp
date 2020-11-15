// AdamoSink.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "project.h"
#include "mainframe.h"
#include "AdamoSink.h"
#include "AdamoGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoSink

IMPLEMENT_DYNCREATE(CAdamoSink, CCmdTarget)

CAdamoSink::CAdamoSink() : m_pImpianto (NULL), m_pParentWnd (NULL)
{
	EnableAutomation();
}

void CAdamoSink::SetImpianto (CAdamoMachine* pMachine)
{
	m_pImpianto=pMachine;
}

void CAdamoSink::SetParentWnd (CWnd* pParentWnd)
{
	m_pParentWnd=pParentWnd;
}

CAdamoSink::~CAdamoSink()
{
    ClearDBErrors ();
    ClearDBStrings ();
}


void CAdamoSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAdamoSink, CCmdTarget)
	//{{AFX_MSG_MAP(CAdamoSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAdamoSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CAdamoSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoBreaked",1,OnAdamoBreaked,VT_I4,VTS_NONE)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoError",2,OnAdamoError,VT_I4,VTS_UNKNOWN)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoMessage", 3, OnAdamoMessage, VT_I4, VTS_I4 VTS_WBSTR VTS_WBSTR VTS_WBSTR VTS_PVARIANT)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoStop",5,OnAdamoStop,VT_I4,VTS_NONE)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoConnected",6,OnAdamoConnected,VT_I4,VTS_NONE)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoDisconnected",7,OnAdamoDisconnected, VT_I4, VTS_NONE)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoString", 8, OnAdamoString, VT_I4, VTS_I4 VTS_DATE VTS_WBSTR)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoResources",  9, OnAdamoResources, VT_I4, VTS_NONE)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoChangePsw", 16, OnAdamoChangePsw, VT_I4, VTS_WBSTR VTS_I4)
    DISP_FUNCTION_ID(CAdamoSink,"OnAdamoUploadCCPlane", 22, OnAdamoUploadCCPlane, VT_I4, VTS_I4 VTS_WBSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAdamoSink to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D17784B6-1CAD-485E-80CE-CB950F40E9D2}
//static  const GUID DIID__IAdamoConnectionEvents =
//    {0x49ceab41,0x7437,0x416a,{0x8d,0x67,0xf4,0xd8,0x0f,0x66,0x38,0x63}};

//static const IID IID_IAdamoSink =
//{ 0xd17784b6, 0x1cad, 0x485e, { 0x80, 0xce, 0xcb, 0x95, 0xf, 0x40, 0xe9, 0xd2 } };

BEGIN_INTERFACE_MAP(CAdamoSink, CCmdTarget)
	INTERFACE_PART(CAdamoSink, DIID__IAdaCommObjEvents, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSink message handlers
int CAdamoSink::OnAdamoError (LPUNKNOWN p)
{
    stThiraErrore* pAdamoErrors;
    _bstr_t b;

    /* recuperiamo l'errore */
    IAdaError* ae=(IAdaError*)p;
    /* ora memorizziamolo */
    pAdamoErrors=new stThiraErrore;
    pAdamoErrors->dt=ae->GetErrorTime ();
    pAdamoErrors->nTypeError=ae->GetErrorType();
    pAdamoErrors->nErrore=ae->GetErrorNumber();
    pAdamoErrors->nTask=ae->GetTaskNumber();
    pAdamoErrors->nDevice=ae->GetDeviceNumber();
    pAdamoErrors->nTypeDevice=ae->GetDeviceType();
    pAdamoErrors->nDeleteError=ae->GetDelete();
	pAdamoErrors->m_strModule=((CAdamoCfgMachine *)m_pImpianto)->GetAdamoModule ()->GetName ();
    b=ae->GetErrorString ();
    strcpy (pAdamoErrors->strErrore, b);
    b=ae->GetTaskName();
    strcpy (pAdamoErrors->szTaskName, b);
    /* se ce ne sono bufferizzati troppi, togliamo l'ultimo */
    if (pAdamoErrors->nDeleteError==0)   {
	    if (m_lErrorsDB.GetCount()>NMAX_ERRORSDB)   {
		    POSITION pos=m_lErrorsDB.GetHeadPosition();
		    stAdamoErrore* pOb=(stAdamoErrore*)m_lErrorsDB.GetAt (pos);
		    m_lErrorsDB.RemoveAt (pos);
		    delete pOb;
	    }
        /* memorizziamo il nuovo */
	    m_lErrorsDB.AddTail (pAdamoErrors);
	}
    /* comunichiamo alla finestra main l'avvenuto successo */
	m_pParentWnd->SendMessage (WM_THEREISANERROR, NULL, (LPARAM)pAdamoErrors);
    return 0;
}

int CAdamoSink::OnAdamoBreaked ()
{
    CCmdGetDebugData DD;
    if (m_pImpianto->GetDebugData (&DD)==S_OK)   {
        CProject *pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
        CProjectFile* pPrjFile=pProject->GetProjectFile (DD.m_nModule);
        m_pParentWnd->SendMessage (DMSG_GOTO_FILELINE, (WPARAM)(const char*)pPrjFile->GetPathName(), (LPARAM)DD.m_nLine);
        CDebugger::GetDebugger()->DrawStackTrace ();
	    m_pParentWnd->SendMessage (DMSG_GOTO_STACKTRACE_LEVEL, 0, 0);
        m_pParentWnd->SendMessage (DMSG_DEBUG_BREAK, 0, 0);
    }
    return 0;
}

int CAdamoSink::OnAdamoConnected ()
{
    m_pParentWnd->SendMessage (WM_ADAMO_CONNECTED, m_pImpianto->GetModuleNumber (), 0);
    return 0;
}

int CAdamoSink::OnAdamoDisconnected ()
{
    m_pParentWnd->SendMessage (WM_ADAMO_DISCONNECTED, m_pImpianto->GetModuleNumber (), 0);
    return 0;
}

void CAdamoSink::ClearDBErrors ()
{
    POSITION pos, pos2;
    for (pos=m_lErrorsDB.GetHeadPosition(); pos2=pos;)    {
        stAdamoErrore* pOb=(stAdamoErrore*)m_lErrorsDB.GetNext(pos);
        m_lErrorsDB.RemoveAt (pos2);
        delete pOb;
    }
}

void CAdamoSink::ClearDBStrings ()
{
    POSITION pos, pos2;
    for (pos=m_lStringDB.GetHeadPosition(); pos2=pos;)    {
        stStringElement* pOb=(stStringElement*)m_lStringDB.GetNext(pos);
        m_lStringDB.RemoveAt (pos2);
        delete pOb;
    }
}

CPtrList& CAdamoSink::GetDBErrors ()
{
    return m_lErrorsDB;

}

CPtrList& CAdamoSink::GetDBStrings ()
{
    return m_lStringDB;

}

void CAdamoSink::ForceDisconnection ()
{
    m_pParentWnd->SendMessage (WM_ADAMO_DISCONNECTED, m_pImpianto->GetModuleNumber (), 1);
}

int CAdamoSink::OnAdamoString (long nPriority, DATE dt, BSTR str)
{
    stThiraStringElement* pSE = new stThiraStringElement;
    COleDateTime oledt (dt);
    strcpy (pSE->str, CString (str));
	pSE->m_strModule=((CAdamoCfgMachine *)m_pImpianto)->GetAdamoModule ()->GetName ();
    pSE->nPriority=nPriority;
    pSE->dt=CTime(oledt.GetYear(), oledt.GetMonth(), oledt.GetDay(), oledt.GetHour(), oledt.GetMinute(), oledt.GetSecond ()).GetTime ();
    if (pSE->nPriority>=0)   {
	    if (m_lStringDB.GetCount()>NMAX_STRINGSSDB)   {
		    POSITION pos=m_lStringDB.GetHeadPosition();
		    stStringElement* pOb=(stStringElement*)m_lStringDB.GetAt (pos);
		    m_lStringDB.RemoveAt (pos);
		    delete pOb;
	    }
	    m_lStringDB.AddTail (pSE);
    }
    m_pParentWnd->SendMessage (WM_ADAMO_STRING, 0, (LPARAM)pSE);
    return 0;
}

int CAdamoSink::OnAdamoStop ()
{
    m_pParentWnd->SendMessage (WM_ADAMO_PROGRAMSTOP, 0, 0);    
    return 0;
}

int CAdamoSink::OnAdamoMessage (long nID, BSTR szMsgQueue, BSTR szMsgSender, BSTR szMsgSlot, VARIANT *v)
{
    return 0;
}

int CAdamoSink::OnAdamoResources ()
{
    /* richiesta di download delle risorse */
    m_pParentWnd->SendMessage (WM_ADAMO_RESOURCES, 0, 0);
    return 0;
}

int CAdamoSink::OnAdamoChangePsw (BSTR szUser, LONG nPswLevel)
{
    CString strUser (szUser);

    stPswRecord *pr=new stPswRecord;
    strcpy (pr->szUser, CString (szUser));
    pr->pswLevel=(ePswLevel)nPswLevel;
    m_pParentWnd->SendMessage (WM_ADAMO_CHANGE_PSW, 0, (LPARAM)pr);
    _delete (pr);
    return 0;
}

int CAdamoSink::OnAdamoUploadCCPlane (LONG nAsse, BSTR bstrArea)
{
    CString strArea (bstrArea), str;
    m_pParentWnd->SendMessage (WM_ADAMO_CCPLANES, nAsse, (LPARAM) &strArea);
    return 0;
}
