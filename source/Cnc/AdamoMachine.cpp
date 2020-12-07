/*
** AdamoMachine.cpp
*/

/* include files */
#include "stdafx.h"
#include <atlbase.h>
#include <afxctl.h>
#include "MainFrame.h"
#include "adamo.h"
#include "adamoint.h"
#include "AdamoGlobal.h"
#include "AdamoMachine.h"
#include "adamosink.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" 
{
	#include "lua.h" 
	#include "lauxlib.h"
	#include "lualib.h"
	#include "lfunc.h"
	#include "lmem.h"
	#include "lobject.h"
	#include "lopcodes.h"
	#include "lstring.h"
	#include "lundump.h" 
	#include "lualib.h"
}

/* implementation */
CAdamoMachine::CAdamoMachine () : m_bOpened (false), m_objComm(NULL), m_ptrCnc (NULL), m_pSink (NULL), m_nModule (0), m_pWnd (NULL), 
	m_nPort (-1), m_eTypeModule (enNoModule), m_nHWModuleNumber (0), m_nConnType (enLocalConnection)
{
}

int CAdamoMachine::OpenMachine (const char* szMachinePath, int nMachineType, eTypeConnection eTC)
{
    return 0;
}

int CAdamoMachine::CloseMachine ()
{
    return 0;
    }

int CAdamoMachine::ReplaceMachine ()
{
    return 0;
}

int CAdamoMachine::ReconnectMachine (eTypeConnection eTC)
{
    return 0;
}

void CAdamoMachine::CreateSink ()
{
    if (!m_pSink)   {
        m_pSink=new CAdamoSink;
        m_pSink->SetImpianto (this);
		m_pSink->SetParentWnd (m_pWnd ? m_pWnd : AfxGetMainWnd ());
        LPUNKNOWN pUnkSink = m_pSink->GetIDispatch(FALSE);
        if (!AfxConnectionAdvise (m_objComm, DIID__IAdaCommObjEvents, pUnkSink, FALSE, &m_dwCookie))
            TRACE ("");
    }
}

void CAdamoMachine::CloseSink ()
{
    if (m_pSink)   {
        m_pSink->ForceDisconnection ();
        LPUNKNOWN pUnkSink = m_pSink->GetIDispatch(FALSE);
        AfxConnectionUnadvise (m_objComm, DIID__IAdaCommObjEvents, pUnkSink, FALSE, m_dwCookie);
        delete m_pSink;
		m_pSink = NULL;
    }
}

int CAdamoMachine::Run ()
{
    if (m_ptrCnc)
        m_ptrCnc->Run ();
    return 0;
}

int CAdamoMachine::DownLoad (CString strProgram)
{
    _bstr_t bstrProgram = strProgram;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoad (bstrProgram);        
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadParams (CString strParams, CString strHWParams, CString strUserStrings)
{
    _bstr_t bstrParams = strParams, bstrUserStrings = strUserStrings, bstrHWParams = strHWParams;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadParams (bstrParams, bstrHWParams, bstrUserStrings);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadParGen (int nRate, int nFrequency, double fpPrecision, double fpContureAngle, double fpEpsilonRaggi)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadParGen (nRate, nFrequency, fpPrecision, fpContureAngle, fpEpsilonRaggi);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadModuleParams (CAdamoConfigurazioneGenerale *pModParams)
{
    if (m_ptrCnc)   {
        InitCmd ();
		m_ptrCnc->DownLoadModuleParams ((char *)pModParams, sizeof (CAdamoConfigurazioneGenerale));
        EndCmd ();
	}
	return 0;
}

int CAdamoMachine::DownLoadUserStrings (CString strParams)
{
    _bstr_t bstrParams = strParams;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadUserStrings (bstrParams);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadCC (CString strCC, CString strName, CString strParams)
{
    _bstr_t bstrCC = strCC;
    _bstr_t bstrName = strName;
    _bstr_t bstrParams = strParams;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadCC (bstrCC, bstrName, bstrParams);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadPC (CString strCC, CString strName)
{
    _bstr_t bstrCC = strCC;
    _bstr_t bstrName = strName;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadPC (bstrCC, bstrName);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::UpLoadPC (CString strCC, CString strArea)
{
    _bstr_t bstrCC = strCC;
    _bstr_t bstrArea = strArea;
    int nB=S_FALSE;

    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->UpLoadPC (bstrCC, bstrArea);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::DownLoadTemporary (CString strTemporary, CString strName)
{
    _bstr_t bstrTemporary = strTemporary;
    _bstr_t bstrName = strName;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadTemporary (bstrTemporary, bstrName);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadResources (CString strResources)
{
    _bstr_t bstrResources = strResources;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadResources (bstrResources);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::Start ()
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->Start ();
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::Stop ()
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->Stop ();
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::SendDebugData (const char* strModuleName, CMap<int, int, BOOL, BOOL>& bp)
{
    IAdaDebugPtr pDbgData;
    BSTR bstr;
    HRESULT hr;
    if (m_ptrCnc)   {
        hr=pDbgData.CreateInstance (__uuidof(AdaDebug));
        if (SUCCEEDED (hr))  {
            LPUNKNOWN pUnk=((IUnknown*)pDbgData);
            pDbgData->SetModuleName (bstr=CString (strModuleName).AllocSysString());
            POSITION pos = bp.GetStartPosition();
            while (pos != NULL)   {
                int nLine, nTemp;
	            bp.GetNextAssoc (pos, nLine, nTemp);
                pDbgData->SetBreakLine (nLine);
            }
            InitCmd ();
            m_ptrCnc->SendDebugData (&pUnk);    
            EndCmd ();
            ::SysFreeString (bstr);
        }
    }
    return 0;        
}

int CAdamoMachine::StartDebugging (int nCmd)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SendDebugCmd (nCmd, 0, 0, "");
        m_ptrCnc->StartDebug ();
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::StartDebugging (int nCmd, int nParameter, int nModule, const char* szModule)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SendDebugCmd (nCmd, nParameter, nModule, szModule);
        m_ptrCnc->StartDebug ();
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::StopDebugging ()
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->StopDebug ();
        EndCmd ();
    }
    return 0;
}

void CAdamoMachine::ClearDebugData ()
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->ClearDebugData ();
        EndCmd ();
    }
}

int CAdamoMachine::IsCncRunning ()
{
    long nCncRunning = 0;
    if (m_ptrCnc)
        nCncRunning=m_ptrCnc->GetIsCncRunning ();
    return nCncRunning;
}

int CAdamoMachine::IsCncRunningReq ()
{
    long nCncRunning = 0;
    if (m_ptrCnc)
        if (m_ptrCnc->GetAdamoStatus (&nCncRunning) == S_FALSE)
            nCncRunning = 0;
    return nCncRunning;
}

int CAdamoMachine::IsCncOnline ()
{
    short snCncOnline = 0;
    if (m_ptrCnc)
        snCncOnline=(short)m_ptrCnc->GetIsCncOnline ();
    return snCncOnline;
}

int CAdamoMachine::GetDebugData (CCmdGetDebugData* pDD)
{
    IAdaDebugPtr pobjDD;
    _bstr_t b;
    int nB=S_FALSE;
    HRESULT hr;
    if (m_ptrCnc)   {
        hr=pobjDD.CreateInstance (__uuidof(AdaDebug));
        if (SUCCEEDED (hr))  {
            LPUNKNOWN pUnk=((IUnknown*)pobjDD);
            InitCmd ();
            if (m_ptrCnc->GetDebugData (&pUnk)==S_OK)   {
                pDD->m_nStatus=pobjDD->GetLevel ();
                pDD->m_nModule=pobjDD->GetModuleIndex ();
                pDD->m_nLine=pobjDD->GetLineNumber ();
                b=pobjDD->GetTaskName ();
                strcpy (pDD->m_szTaskName, b);
                nB=S_OK;
            }
            EndCmd ();
        }            
    }            
    return nB;
}

void CAdamoMachine::SendDebugCmd (int nCmd, int nParameter, int nModule, const char* szModule)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SendDebugCmd (nCmd, nParameter, nModule, szModule);
        EndCmd ();
    }
}

void CAdamoMachine::InitDebugData (const char* szSlot)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->InitDebugData (szSlot);
        EndCmd ();
    }
}

int CAdamoMachine::GetStackData (CPtrList& l)
{
    IAdaDebugPtr pobjSD;
    _bstr_t b;
    int nB=S_OK, n=0;
    HRESULT hr;
    if (m_ptrCnc)   {
        hr=pobjSD.CreateInstance (__uuidof(AdaDebug), NULL, CLSCTX_INPROC_SERVER);
        if (SUCCEEDED (hr))  {
            LPUNKNOWN pUnk=((IUnknown*)pobjSD);
            pobjSD->PutLevel (-1);
            while (n<1024)   {
                pobjSD->PutLevel (n++);
                InitCmd ();
                if (m_ptrCnc->GetStackData (&pUnk)==S_OK)   {
                    if (pobjSD->GetLevel ()>0)   {
                        CCmdGetStackData* pSD=new CCmdGetStackData;    
                        strcpy (pSD->szModule, pobjSD->GetModuleName ());
                        strcpy (pSD->szFunctionName, pobjSD->GetTaskName ()); 
                        pSD->nLine=pobjSD->GetLineNumber ();
                        l.AddTail (pSD);
                    }
                    else 
                        break;
                }
                else    {
                    nB=S_FALSE;
                    break;
                }
                EndCmd ();
            }             
        }            
        else
            nB=S_FALSE;
    }            
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::GetLocalData (CPtrList& l, int nLevel)
{
    IAdaDebugPtr pobjSD;
    _bstr_t b;
    int nB=S_OK, n=0, nProg=1;
    HRESULT hr;
    if (m_ptrCnc)   {
        hr=pobjSD.CreateInstance (__uuidof(AdaDebug), NULL, CLSCTX_INPROC_SERVER);
        if (SUCCEEDED (hr))  {
            LPUNKNOWN pUnk=((IUnknown*)pobjSD);
            while (n<1024)   {
                pobjSD->PutLevel (nLevel);
                pobjSD->PutLineNumber (nProg++);
                InitCmd ();
                if (m_ptrCnc->GetLocalData (&pUnk)==S_OK)   {
                    if (pobjSD->GetLevel ()>0)   {
                        CCmdGetLocalData* pLD=new CCmdGetLocalData;
                        pLD->nType=pobjSD->GetModuleIndex ();
                        pLD->fpValue=pobjSD->GetNumberValue();
                        strcpy (pLD->szLocalName, pobjSD->GetVarName ());
                        strcpy (pLD->szTypeName, pobjSD->GetTypeName ());
                        strcpy (pLD->szValue, pobjSD->GetValue ());
                        l.AddTail (pLD);
                    }
                    else 
                        break;
                }
                else    {
                    nB=S_FALSE;
                    break;
                }
                EndCmd ();
            }             
        }            
        else
            nB=S_FALSE;
    }            
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::GetGlobalData (int nLevel, const char* szExpression, CCmdGetGlobalData* gd)
{
    IAdaDebugPtr pobjSD;
    int nB=S_OK, n=0, nProg=1;
    HRESULT hr;
    if (m_ptrCnc)   {
        hr=pobjSD.CreateInstance (__uuidof(AdaDebug), NULL, CLSCTX_INPROC_SERVER);
        if (SUCCEEDED (hr))  {
            LPUNKNOWN pUnk=((IUnknown*)pobjSD);
            pobjSD->PutLevel (nLevel);
            pobjSD->PutExpression (szExpression);
            InitCmd ();
            if (m_ptrCnc->GetGlobalData (&pUnk)==S_OK)   {
                gd->nType=pobjSD->GetModuleIndex ();
                gd->fpValue=pobjSD->GetNumberValue();
                strcpy (gd->szTypeName, pobjSD->GetTypeName ());
                strcpy (gd->szValue, pobjSD->GetValue ());
            }
            else
                nB=S_FALSE;
            EndCmd ();
        }            
        else
            nB=S_FALSE;
    }            
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::GetTaskData (int nSlotIni, int nSlotFin, stTaskData* pTaskData)
{
    int nB=S_OK, n=(nSlotFin-nSlotIni+1)*sizeof (stTaskData);

    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetTaskData (nSlotIni, nSlotFin, (char*) pTaskData, n);
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::SlotDelete (stTaskData* pTaskData)
{
    int nB=S_OK;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SlotDelete (pTaskData->nTask, pTaskData->szTaskName);
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::SlotSetStatus (stTaskData* pTaskData)
{
    int nB=S_OK;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SlotSetStatus (pTaskData->nTask, pTaskData->szTaskName, pTaskData->nStatus);
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::GetVariable (CString szVar, COleVariant& v)
{
    _bstr_t bstrVar = szVar;
    int nB=S_OK;

    if (m_ptrCnc)   {
        InitCmd ();
        v=m_ptrCnc->GetVariable (bstrVar);
        if (v.vt == VT_EMPTY)
            nB = S_FALSE;
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::SetVariable (CString szVar, COleVariant& v)
{
    _bstr_t bstrVar = szVar;
    int nB=S_OK;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetVariable (bstrVar, v);
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::GetLocalVariable (CString szVar, int nLevel, int nProg, COleVariant& v)
{
    _bstr_t bstrVar = szVar;
    int nB=S_OK;

    if (m_ptrCnc)   {
        InitCmd ();
        v=m_ptrCnc->GetLocalVariable (bstrVar, nLevel, nProg);
        if (v.vt == VT_EMPTY)
            nB = S_FALSE;
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::SetLocalVariable (CString szVar, int nLevel, int nProg, COleVariant& v)
{
    _bstr_t bstrVar = szVar;
    int nB=S_OK;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetLocalVariable (bstrVar, nLevel, nProg, v);
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::GetArray (CString szVar, int nIni, int nFin, CAdamoData* pData)
{
    _bstr_t bstrVar = szVar;
    int i, nB=S_OK;
    char pBuf[COMMBUFDATA-sizeof (CCmdReadWriteGlobalData)+1], *p;

    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->GetArray (bstrVar, nIni, nFin, pBuf, sizeof (pBuf))==S_OK)   {
            p=pBuf;
            for (i=0; i<=nFin-nIni; i++)    {
                pData[i].nType=*((int*)p);
                p+=sizeof (int);
                switch (pData[i].nType)   {
                    case LUA_TBOOLEAN :
                        pData[i].AdamoData.b=*((int*)p);
                        p+=sizeof (int);
                        break;
                    case LUA_TNUMBER :
                        pData[i].AdamoData.fp=*((double*)p);
                        p+=sizeof (double);
                        break;
                    case LUA_TSTRING :   {
                            int nLen=strlen (p)+1;
                            if (nLen<=NMAXSTRING)
                                strcpy (pData[i].AdamoData.str, p);
                            else   {
                                memcpy (pData[i].AdamoData.str, p, NMAXSTRING-1);
                                pData[i].AdamoData.str[NMAXSTRING-1]='\0';
                            }
                            p+=nLen;
                        }    
                        break;
                    default :
                        break;
                }
            }
        }
        else
            nB=S_FALSE;
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::SetArray (CString szVar, int nIni, int nFin, CAdamoData* pData)
{
    _bstr_t bstrVar = szVar;
    char pBuf[COMMBUFDATA-sizeof (CCmdReadWriteGlobalData)+1], *p;
    int i, n=0, nB=S_OK;

    if (m_ptrCnc)   {
        p=pBuf;
        for (i=0; i<=nFin-nIni; i++)   {
            n+=4;
            switch (pData[i].nType)   {
                case LUA_TBOOLEAN :
                    n+=sizeof (int);
                    break;
                case LUA_TNUMBER :
                    n+=sizeof (double);
                    break;
                case LUA_TSTRING :
                    n+=strlen (p)+1;
                    break;
            }
        }
        p=pBuf;    
        if (n<=COMMBUFDATA)
            for (i=0; i<=nFin-nIni; i++)   {
                *((int*)p)=pData[i].nType;
                p+=sizeof (int);
                switch (pData[i].nType)   {
                    case LUA_TBOOLEAN :
                        *((int*)p)=pData[i].AdamoData.b;
                        p+=sizeof (int);
                        break;
                    case LUA_TNUMBER :
                        *((double*)p)=pData[i].AdamoData.fp;
                        p+=sizeof (double);
                        break;
                    case LUA_TSTRING :
                        strcpy (p, pData[i].AdamoData.str);
                        p+=strlen (p)+1;
                        break;
                }
            }
        else
            nB=S_FALSE;
        InitCmd ();
        m_ptrCnc->SetArray (bstrVar, nIni, nFin, pBuf, sizeof (pBuf));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetMatrix (CString szVar, int nRow, int nNumColumns, int nBufKeys, char* pBufKeys, CAdamoData* pData)
{
    _bstr_t bstrVar = szVar;
    int i, nB=S_OK;
    char pBuf[COMMBUFDATA-sizeof (CCmdReadWriteGlobalData)+1], *p;

    if (m_ptrCnc)   {
        memcpy (pBuf, pBufKeys, nBufKeys);
        InitCmd ();
        if (m_ptrCnc->GetMatrix (bstrVar, nRow, pBuf, sizeof (pBuf))==S_OK)   {
            p=pBuf;
            for (i=0; i<nNumColumns; i++)    {
                pData[i].nType=*((int*)p);
                p+=sizeof (int);
                switch (pData[i].nType)   {
                    case LUA_TBOOLEAN :
                        pData[i].AdamoData.b=*((int*)p);
                        p+=sizeof (int);
                        break;
                    case LUA_TNUMBER :
                        pData[i].AdamoData.fp=*((double*)p);
                        p+=sizeof (double);
                        break;
                    case LUA_TSTRING :   {
                            int nLen=strlen (p)+1;
                            if (nLen<=NMAXSTRING)
                                strcpy (pData[i].AdamoData.str, p);
                            else   {
                                memcpy (pData[i].AdamoData.str, p, NMAXSTRING-1);
                                pData[i].AdamoData.str[NMAXSTRING-1]='\0';
                            }
                            p+=nLen;
                        }
                        break;
                    default :
                        break;
                }
            }
        }
        else
            nB=S_FALSE;
        EndCmd ();
    }
    else
        nB=S_FALSE;
    return nB;
}

int CAdamoMachine::SetMatrix (CString szVar, int nRow, int nNumColumns, int nBufKeys, char* pBufKeys, CAdamoData* pData)
{
    _bstr_t bstrVar = szVar;
    char pBuf[COMMBUFDATA-sizeof (CCmdReadWriteGlobalData)+1], *p, *pKeys;
    int i, n=0, nB=S_OK, nLen, s;

    if (m_ptrCnc)   {
        p=pBuf;    
        if (nNumColumns<0)   {
            nNumColumns=abs(nNumColumns);
            pKeys=pBufKeys+2*sizeof(int);
            for (i=0; i<nNumColumns; i++)   {
                s=strlen (pKeys)+1;
                n+=s;
                pKeys+=s;
                n+=4;
                switch (pData[i].nType)   {
                    case LUA_TBOOLEAN :
                        n+=sizeof (int);
                        break;
                    case LUA_TNUMBER :
                        n+=sizeof (double);
                        break;
                    case LUA_TSTRING :
                        n+=strlen (pData[i].AdamoData.str)+1;
                        break;
                }
            }
            if (n<=COMMBUFDATA)   {
                pKeys=pBufKeys+2*sizeof(int);
                *((int*)p)=*((int*)pBufKeys+1);
                p+=sizeof (int);
                for (i=0; i<nNumColumns; i++)   {
                    nLen=strlen (pKeys)+1;
                    strcpy (p, pKeys);
                    p+=nLen, pKeys+=nLen;
                    *((int*)p)=pData[i].nType;
                    p+=sizeof (int);
                    switch (pData[i].nType)   {
                        case LUA_TBOOLEAN :
                            *((int*)p)=pData[i].AdamoData.b;
                            p+=sizeof (int);
                            break;
                        case LUA_TNUMBER :
                            *((double*)p)=pData[i].AdamoData.fp;
                            p+=sizeof (double);
                            break;
                        case LUA_TSTRING :
                            strcpy (p, pData[i].AdamoData.str);
                            p+=strlen (p)+1;
                            break;
                    }
                }
                nNumColumns=-1;
            }
            else
                nB=S_FALSE;
        }
        else   {
            strcpy (p, pBufKeys+2*sizeof(int));
            p+=strlen (p)+1;
            *((int*)p)=pData[0].nType;
            p+=sizeof (int);
            switch (pData[0].nType)   {
                case LUA_TBOOLEAN :
                    *((int*)p)=pData[0].AdamoData.b;
                    break;
                case LUA_TNUMBER :
                    *((double*)p)=pData[0].AdamoData.fp;
                    break;
                case LUA_TSTRING :
                    strcpy (p, pData[0].AdamoData.str);
                    break;
            }
        }
        InitCmd ();
        m_ptrCnc->SetMatrix (bstrVar, nRow, nNumColumns, pBuf, sizeof (pBuf));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::SetDevice (int n, CDeviceData* pData)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetDevice (n, (unsigned char*)pData, n*sizeof (CDeviceData));
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::GetDevice (int n, CDeviceData* pData)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetDevice (n, (unsigned char*)pData, n*sizeof (CDeviceData));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetAxes (int n, CDeviceData* pAxes, stDatiAsse* pData)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        memcpy (pData, pAxes, n*sizeof (CDeviceData));
        nB = m_ptrCnc->GetAxes (n, (unsigned char*)pData, n*sizeof (stDatiAsse));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetGAxes (int nSlot, int n, CDeviceData* pAxes, stGSlotPosition* pData)
{
    int nB=S_FALSE, i=0;
    if (m_ptrCnc)   {
		BYTE *p = new BYTE [n*sizeof (stGSlotPosition)];
        memcpy (p, pAxes, n*sizeof (CDeviceData));
        InitCmd ();
        m_ptrCnc->GetGAxes (nSlot, n, p, n*sizeof (stGSlotPosition));
        EndCmd ();
		while (i<n)   {
			pData[i].m_fpMachinePos=*((double *)p);
			pData[i].m_fpMachineTarget=*((double *)p+1);
			pData[i].m_fpRefPos=*((double *)p+2);
			pData[i].m_fpRefTarget=*((double *)p+3);
			pData[i].m_fpSlotPos=*((double *)p+4);
			pData[i].m_fpSlotTarget=*((double *)p+5);
			i++;
		}
		delete p;
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::GetAdamoAxes (CDeviceData* pAxes, CAdamoParametri* pAP, CAdamoStatoAsse* aSA)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        char* pBuf = new char [sizeof (CAdamoParametri)+sizeof (CAdamoStatoAsse)];
        memcpy (pBuf, pAxes, sizeof (CDeviceData));
        InitCmd ();
        nB = m_ptrCnc->GetAdamoAxes ((unsigned char*)pBuf, sizeof (CAdamoParametri)+sizeof (CAdamoStatoAsse));
        EndCmd ();
        *pAP=*((CAdamoParametri*)pBuf);
        *aSA=*((CAdamoStatoAsse*)(pBuf+sizeof (CAdamoParametri)));
        delete pBuf;
    }
    return nB;
}

int CAdamoMachine::GotoPosition (int nAsse, double fpQuote, double fpVel, enTypeJog TypeJog)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad[2];
        ad[0].m_eDev=enAsse;
        ad[0].m_nAddress=nAsse;
		ad[0].m_nType=TypeJog;
        ad[0].m_fpDati=fpQuote;
        ad[1].m_fpDati=fpVel;
        InitCmd ();
        m_ptrCnc->GotoPosition ((unsigned char*)ad, 2*sizeof (CDeviceData));    
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::GotoPositionInc (int nAsse, double fpQuote, double fpVel, enTypeJog TypeJog)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad[2];
        ad[0].m_eDev=enAsse;
        ad[0].m_nAddress=nAsse;
		ad[0].m_nType=TypeJog;
        ad[0].m_fpDati=fpQuote;
        ad[1].m_fpDati=fpVel;
        InitCmd ();
        m_ptrCnc->GotoPositionInc ((unsigned char*)ad, 2*sizeof (CDeviceData));    
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::GotoLinear (int nAsse, double fpQuote, double fpVel, enTypeJog TypeJog)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad[2];
        ad[0].m_eDev=enAsse;
        ad[0].m_nAddress=nAsse;
		ad[0].m_nType=TypeJog;
        ad[0].m_fpDati=fpQuote;
        ad[1].m_fpDati=fpVel;
        InitCmd ();
        m_ptrCnc->GotoLinear ((unsigned char*)ad, 2*sizeof (CDeviceData));    
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::GotoLinearInc (int nAsse, double fpQuote, double fpVel, enTypeJog TypeJog)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad[2];
        ad[0].m_eDev=enAsse;
        ad[0].m_nAddress=nAsse;
		ad[0].m_nType=TypeJog;
        ad[0].m_fpDati=fpQuote;
        ad[1].m_fpDati=fpVel;
        InitCmd ();
        m_ptrCnc->GotoLinearInc ((unsigned char*)ad, 2*sizeof (CDeviceData));
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::AbortMovement (int nAsse)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad;
        ad.m_eDev=enAsse;
        ad.m_nAddress=nAsse;
        InitCmd ();
        m_ptrCnc->AbortMovement ((unsigned char*)&ad, sizeof (CDeviceData));
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::OpenPositionLoop (int nAsse)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad;
        ad.m_eDev=enAsse;
        ad.m_nAddress=nAsse;
        InitCmd ();
        m_ptrCnc->OpenPositionLoop ((unsigned char*)&ad, sizeof (CDeviceData));
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::ClosePositionLoop (int nAsse)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        CDeviceData ad;
        ad.m_eDev=enAsse;
        ad.m_nAddress=nAsse;
        InitCmd ();
        m_ptrCnc->ClosePositionLoop ((unsigned char*)&ad, sizeof (CDeviceData));
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::OscStart (long nFrequency, long nBufferSize, long nAndOr, long nPretrigger, bool bStartImmediately)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->OscStart (nFrequency, nBufferSize, nAndOr, nPretrigger, bStartImmediately);
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::OscStop ()
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->OscStop ();
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::OscSendData (CVarData* pVarData, CDeviceData* pDevData)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SendOscData (CString (pVarData->m_szVarName).AllocSysString (), 
							   pVarData->m_nRow,
							   CString (pVarData->m_szColumnName).AllocSysString (),
							   pVarData->m_nVarType,
							   pDevData->m_nAddress, 
							   pDevData->m_eDev, 
							   pDevData->m_nType);
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::OscSendEvents (CDeviceData* pData, long nCmp, int nData, double fpData)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SendOscEvent (pData->m_nAddress, pData->m_eDev, pData->m_nType, nCmp, nData, fpData);
        EndCmd ();
        nB=S_OK;
    }
    return nB;
}

int CAdamoMachine::OscGetData (long* nStatus, long* nRecords, char* pBuf, long nBufLength)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetOscData (nStatus, nRecords, (unsigned char*)pBuf, nBufLength);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::OscGetStatus (long* nStatus, long* nError)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetOscStatus (nStatus, nError);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetParCnt (long* pCnt)
{
    int nB=S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetParCnt (pCnt);
        EndCmd ();
    }
    return nB;
}

bool CAdamoMachine::ExistSymbol (CString szVar, int nLevel, int* pnGL, int* pnType, int* pnProg)
{
    _bstr_t bstrVar = szVar;
    int nB=0;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->ExistSymbol (bstrVar, (long)nLevel, (long*)pnGL, (long*)pnType, (long*)pnProg);
        EndCmd ();
    }
    return nB!=0;
}

int CAdamoMachine::GetSlotNumber (CString strName)
{
    _bstr_t bstrName = strName;
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->GetSlotNumber (bstrName);
        EndCmd ();
    }
    return nB;
}

int  CAdamoMachine::ClearCCData ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->ClearCCData ();
        EndCmd ();
    }
	return nB;
}

int  CAdamoMachine::StartCommTrace ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->StartCommTrace ();
        EndCmd ();
    }
	return nB;
}

int  CAdamoMachine::StopCommTrace ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->StopCommTrace ();
        EndCmd ();
    }
	return nB;
}

bool CAdamoMachine::IsCommTraceEnabled ()
{
    long l;
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->get_IsCommTraceEnabled (&l);
        EndCmd ();
    }
	return l!=0;
}

int  CAdamoMachine::GetNumCards (int *nCards)
{
    int nB=S_FALSE;

    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetCardsData (-1, (char*)nCards, sizeof (int));
        EndCmd ();
    }
    return nB;
}

int  CAdamoMachine::GetCardInfo (int nCard, CCmdGetCardInfo* pCI)
{
    int nB=S_FALSE;

    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetCardsData (nCard, (char*)pCI, sizeof (CCmdGetCardInfo));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetGlobalTableKeys (CString strName, CString strPath, CAdamoValori& varKey, CAdamoValori* pResult, int* pValueType, int& nElements, bool& GheNeAmo)
{
    BSTR bstrName, bstrPath;
    _variant_t t;
    long nDimension;
    int nB=S_FALSE, i=0, nType;
    LONG  bAmo;
    bool bErr=false;
    char* p, *q;

    AssignVarKey (varKey, t);
    q=p=new char [COMMBUF]; nDimension=COMMBUF;
    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->GetTableKey (bstrName=strName.AllocSysString (), bstrPath=strPath.AllocSysString (), t, -1, -1, p, nDimension, &bAmo)==S_OK)  {
            if (*((int*)q)!=-1)   {
                while (*q!='\0')   {
                    nType=*((int *)q);
                    pResult[i].nType=nType;
                    q+=sizeof (int);
                    switch (nType)   {
                        case LUA_TNUMBER :
                            pResult[i].AdamoData.fp=*((double*)q);
                            q+=sizeof (double);
                            break;
                        case LUA_TSTRING :
                            strcpy (pResult[i].AdamoData.str, q);
                            q+=strlen (q)+1;
                            break;
                        default :
                            bErr=true;
                    }
                    if (bErr)
                        break;
                    pValueType[i]=*((int *)q);
                    q+=sizeof (int);
                    i++;
                }
                nElements=i;
                GheNeAmo=(bAmo!=0);
                nB=S_OK;
            }
        }
        ::SysFreeString (bstrName);
        ::SysFreeString (bstrPath);
        EndCmd ();
    }
    _delete (p);
    return nB;
}

int CAdamoMachine::GetLocalTableKeys  (CString strName, CString strPath, CAdamoValori& varKey, int nLevel, int nProg, CAdamoValori* pResult, int* pValueType, int& nElements, bool& GheNeAmo)
{
    BSTR bstrName, bstrPath;
    _variant_t t;
    long nDimension;
    int nB=S_FALSE, i=0, nType;
    bool bErr=false;
    LONG bAmo;
    char* p, *q;

    AssignVarKey (varKey, t);
    q=p=new char [COMMBUF]; nDimension=COMMBUF;
    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->GetTableKey (bstrName=strName.AllocSysString (), bstrPath=strPath.AllocSysString (), t, nLevel, nProg, p, nDimension, &bAmo)==S_OK)   {
            if (*((int*)q)!=-1)   {
                while (*q!='\0')   {
                    nType=*((int *)q);
                    pResult[i].nType=nType;
                    q+=sizeof (int);
                    switch (nType)   {
                        case LUA_TNUMBER :
                            pResult[i].AdamoData.fp=*((double*)q);
                            q+=sizeof (double);
                            break;
                        case LUA_TSTRING :
                            strcpy (pResult[i].AdamoData.str, q);
                            q+=strlen (q)+1;
                            break;
                        default :
                            bErr=true;
                    }
                    if (bErr)
                        break;
                    pValueType[i]=*((int *)q);
                    q+=sizeof (int);
                    i++;
                }
                GheNeAmo=(bAmo!=0);
                nElements=i;
                nB=S_OK;
            }
        }
        ::SysFreeString (bstrName);
        ::SysFreeString (bstrPath);
        EndCmd ();
    }
    _delete (p);
    return nB;
}

int CAdamoMachine::GetGlobalTableValues (CString strName, CString strPath, CAdamoValori& varKey, CAdamoData* pResult)
{
    BSTR bstrName, bstrPath;
    _variant_t t;
    long nDimension;
    int nB=S_FALSE, nType;
    char* p, *q;

    AssignVarKey (varKey, t);
    q=p=new char [COMMBUF]; nDimension=COMMBUF;
    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->GetTableData (bstrName=strName.AllocSysString (), bstrPath=strPath.AllocSysString (), t, -1, -1, p, nDimension)==S_OK)   {
            if (*((int*)q)!=-1)   {
                nType=*((int *)q);
                pResult->nType=nType;
                q+=sizeof (int);
                switch (nType)   {
                    case LUA_TBOOLEAN :
                        pResult->AdamoData.b=*((int*)q)!=0;
                        q+=sizeof (int);
                        break;
                    case LUA_TNUMBER :
                        pResult->AdamoData.fp=*((double*)q);
                        q+=sizeof (double);
                        break;
                    case LUA_TSTRING :
                        strcpy (pResult->AdamoData.str, q);
                        q+=strlen (q)+1;
                        break;
                }
                nB=S_OK;
            }
        }
        EndCmd ();
        ::SysFreeString (bstrName);
        ::SysFreeString (bstrPath);
    }
    _delete (p);
    return nB;
}

int CAdamoMachine::GetLocalTableValues  (CString strName, CString strPath, CAdamoValori& varKey, int nLevel, int nProg, CAdamoData* pResult)
{
    BSTR bstrName, bstrPath;
    _variant_t t;
    long nDimension;
    int nB=S_FALSE, nType;
    char* p, *q;

    AssignVarKey (varKey, t);
    q=p=new char [COMMBUF]; nDimension=COMMBUF;
    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->GetTableData (bstrName=strName.AllocSysString (), bstrPath=strPath.AllocSysString (), t, nLevel, nProg, p, nDimension)==S_OK)   {
            if (*((int*)q)!=-1)   {
                nType=*((int *)q);
                pResult->nType=nType;
                q+=sizeof (int);
                switch (nType)   {
                    case LUA_TBOOLEAN :
                        pResult->AdamoData.b=*((int*)q)!=0;
                        q+=sizeof (int);
                        break;
                    case LUA_TNUMBER :
                        pResult->AdamoData.fp=*((double*)q);
                        q+=sizeof (double);
                        break;
                    case LUA_TSTRING :
                        strcpy (pResult->AdamoData.str, q);
                        q+=strlen (q)+1;
                        break;
                }
            }
			else   {
				pResult->nType = LUA_TNIL;
				pResult->AdamoData.str[0] = '\0';
			}
            nB=S_OK;
        }
        EndCmd ();
        ::SysFreeString (bstrName);
        ::SysFreeString (bstrPath);
    }
    _delete (p);
    return nB;
}

void CAdamoMachine::AssignVarKey (CAdamoValori& varKey, _variant_t& t)
{
    switch (varKey.nType)   {
        case LUA_TNUMBER :
            t=varKey.AdamoData.fp;
            break;
        case LUA_TSTRING :
            t=varKey.AdamoData.str;
            break;
    }
}

void CAdamoMachine::AssignVarData (CAdamoData& data, _variant_t& t)
{
    switch (data.nType)   {
        case LUA_TNUMBER :
            t=data.AdamoData.fp;
            break;
        case LUA_TSTRING :
            t=data.AdamoData.str;
            break;
        case LUA_TBOOLEAN :
            t=(bool)(data.AdamoData.b!=0);
            break;
    }
}

int CAdamoMachine::SetGlobalTableValues (CString strName, CString strPath, CAdamoValori& varKey, CAdamoData& data)
{
    BSTR bstrName, bstrPath;
    _variant_t t, td;
    int nB=S_FALSE;

    AssignVarKey (varKey, t);
    AssignVarData (data, td);
    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->SetTableData (bstrName=strName.AllocSysString (), bstrPath=strPath.AllocSysString (), t, -1, -1, td);
        EndCmd ();
        ::SysFreeString (bstrName);
        ::SysFreeString (bstrPath);
    }
    return nB;
}

int CAdamoMachine::SetLocalTableValues (CString strName, CString strPath, CAdamoValori& varKey, int nLevel, int nProg, CAdamoData& data)
{
    BSTR bstrName, bstrPath;
    _variant_t t, td;
    int nB=S_FALSE;

    AssignVarKey (varKey, t);
    AssignVarData (data, td);
    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->SetTableData (bstrName=strName.AllocSysString (), bstrPath=strPath.AllocSysString (), t, nLevel, nProg, td);
        EndCmd ();
        ::SysFreeString (bstrName);
        ::SysFreeString (bstrPath);
    }
    return nB;
}

int CAdamoMachine::StartCanScan ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->StartCanScan ();
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::SetCanOperatingMode (long nNode, long nMode)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->SetCanOperatingMode (nNode, nMode);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetScanCanStatus ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->GetScanCanStatus ();
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetCanNodeData (CCmdGetDeviceInfo *pDeviceInfo)
{
    int nB=0;

    if (m_ptrCnc)   {
        InitCmd ();
        for (int i=0; i<NMAX_CANNODES; i++)   {
            if (m_ptrCnc->GetCanNodeData (i, (char *)(&pDeviceInfo[i]), sizeof (CCmdGetDeviceInfo))!=S_OK)   {
                nB=-1; break;
            }
        }
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::ChangeLanguage ()
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->ChangeLanguage ();
        EndCmd ();
    }
    return 0;
}

int  CAdamoMachine::StartSDOCommand (int nRequests, stSDORequest *pSDORequests)
{
    int nB=-1;

    if (m_ptrCnc)   {
        stTxSDORequest *pTxSDO=new stTxSDORequest[nRequests];
        for (int n=0; n<nRequests; n++)   {
            pTxSDO[n].m_nCanale=pSDORequests[n].m_nCanale;
            pTxSDO[n].m_nSDOType=pSDORequests[n].m_nSDOType;
            pTxSDO[n].m_nNode=pSDORequests[n].m_nNode;
            pTxSDO[n].m_nIndex=pSDORequests[n].m_nIndex;
            pTxSDO[n].m_nSubIndex=pSDORequests[n].m_nSubIndex;
            pTxSDO[n].m_nLengthData=pSDORequests[n].m_nLengthData;
            memcpy (pTxSDO[n].m_data, pSDORequests[n].m_data, NMAX_READ_WRITE_SDO_DATA);
        }
        InitCmd ();
        if (m_ptrCnc->StartSDOCommand (nRequests, (char *)pTxSDO, nRequests*sizeof (stTxSDORequest))==S_OK)
            nB=0;
        EndCmd ();
        _delete (pTxSDO);
    }
    return nB;
}

int  CAdamoMachine::GetSDOStatus ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->GetSDOStatus ();
        EndCmd ();
    }
    return nB;
}

int  CAdamoMachine::GetSDOData (int *nRequests, stSDOResults *pSDOResults)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        stTxSDOResults *pTxSDO=new stTxSDOResults[NMAX_READ_WRITE_SDO*sizeof (stTxSDOResults)];
        InitCmd ();
        if (m_ptrCnc->GetSDOData (nRequests, (char *)pTxSDO, NMAX_READ_WRITE_SDO*sizeof (stTxSDOResults))==S_OK)   {
            for (int n=0; n<*nRequests; n++)   {
                pSDOResults[n].m_pe=pTxSDO[n].m_pe;
                pSDOResults[n].m_nLengthData=pTxSDO[n].m_nLengthData;
                memcpy (pSDOResults[n].m_data, pTxSDO[n].m_data, pSDOResults[n].m_nLengthData);
            }
            nB=0;
        }
        EndCmd ();
        _delete (pTxSDO);
    }
    return nB;
}

int CAdamoMachine::GetDebugInfo (CAdamoDebugInfo *pDI)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetDebugInfo ((char *)pDI, sizeof (CAdamoDebugInfo));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::SetDebugInfo (int nDebugParameter)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetDebugInfo (nDebugParameter);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::UploadSSCNETParameters (int nCard, int nSSCNETType, int nStation, WORD *pwParameters)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        if (nSSCNETType == 0)
            nB = m_ptrCnc->UploadSSCNETParameters (nCard, nStation, (char *)pwParameters, NMAXMGA023PARAMETERS*NMAXMGA023PARAMETERSTYPES*sizeof (WORD));
        else
            nB = m_ptrCnc->UploadSSCNETParameters (nCard, nStation, (char *)pwParameters, NMAXMGA030PARAMETERS*NMAXMGA030PARAMETERSTYPES*sizeof (WORD));
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::DownloadSSCNETParameters (int nCard, int nStation, int nNumParams, WORD *pwParameters)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownloadSSCNETParameters (nCard, nStation, nNumParams, (char *)pwParameters, nNumParams*sizeof (WORD)*2);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::StartGProgram (int nGSlot)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->StartGProgram (nGSlot);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::StopGProgram (int nGSlot)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->StopGProgram (nGSlot);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::AbortGProgram (int nGSlot)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->AbortGProgram (nGSlot);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::GetGLineInfo (int nGSlot, VARIANT *pInfo)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->GetGLineInfo (nGSlot, pInfo);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::GetGSlotInfo (int nGSlot, VARIANT *pInfo)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->GetGSlotInfo (nGSlot, pInfo);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::GetSSCNETStatus (int nCard, int nStation)
{
    DWORD dwStatus=0;
    
    if (m_ptrCnc)   {
        InitCmd ();
        dwStatus=m_ptrCnc->GetSSCNETServoStatus (nCard, nStation);
        EndCmd ();
    }
    return dwStatus;
}

void CAdamoMachine::GetSSCNETDisplay (int nCard, int nStation, DWORD *dwStatus, DWORD *dwDisplay)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->GetSSCNETServoDisplay (nCard, nStation, dwStatus, dwDisplay);
        EndCmd ();
    }
}

int CAdamoMachine::GetSSCNETAlarm (int nCard, int nStation, WORD &wAlarm, CString &strAlarm)
{
    BSTR bstrAlarm;
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->GetSSCNETAlarm (nCard, nStation, &wAlarm, &bstrAlarm) == S_OK)
            strAlarm=bstrAlarm;
		else   {
			wAlarm = 0;
            strAlarm = "";
		}
        EndCmd ();
        nB=0;
    }
    return nB;
}

int  CAdamoMachine::GetSSCNETStations (int nCard)
{
    DWORD dwStations=0;
    
    if (m_ptrCnc)   {
        InitCmd ();
        dwStations=m_ptrCnc->GetSSCNETStations (nCard);
        EndCmd ();
    }
    return dwStations;
}

int CAdamoMachine::SSCNETReadyOn (int nCard, int nStation, bool bOn)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SSCNETReadyOn (nCard, nStation, bOn);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::SSCNETServoOn (int nCard, int nStation, bool bOn)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SSCNETServoOn (nCard, nStation, bOn);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::SSCNETResetAlarm (int nCard, int nStation)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SSCNETResetAlarm (nCard, nStation);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::SetCanInitialization ()
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetCanInitialization ();
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::GetPswLevel ()
{
    int nPswLevel;

    if (m_ptrCnc)   {
        InitCmd ();
        nPswLevel=m_ptrCnc->GetPswLevel ();
        EndCmd ();
    }
    return nPswLevel;
}

int CAdamoMachine::WriteAbsEncoder (int nCard, int nStation, WORD wMultiRevCounter, DWORD dwOneRevPos)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->WriteAbsEncoder (nCard, nStation, wMultiRevCounter, dwOneRevPos);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::ReadAbsEncoder (int nCard, int nStation, WORD *wMultiRevCounter, DWORD *dwOneRevPos)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        if (m_ptrCnc->ReadAbsEncoder (nCard, nStation, wMultiRevCounter, dwOneRevPos) == S_OK)
			nB = 0;
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::SetAbsPosition (int nCard, int nStation)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetAbsPosition (nCard, nStation);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::InitSSCNET (int nCard)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->InitSSCNET (nCard);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::RunSlot (CString strSlot)
{
    BSTR bstr;
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->RunSlot (bstr=strSlot.AllocSysString ());
        EndCmd ();
        ::SysFreeString (bstr);
        nB=0;
    }
    return nB;
}

int CAdamoMachine::GetExpressionVariable (CString szExpression, int nLevel, COleVariant& v)
{
    CCmdGetGlobalData gd;
    int nB=S_FALSE;

    szExpression="return "+szExpression;
    if (GetGlobalData (nLevel, (const char*)szExpression, &gd)==S_OK)   {
        if (gd.nType!=-1)   {
            switch (gd.nType)   {
            case LUA_TNUMBER :
                v=gd.fpValue;        
                break;
            case LUA_TBOOLEAN :
                v=(long)gd.fpValue;        
                break;
            case LUA_TSTRING :
                v=((const char*)gd.szValue);
                break;
            case LUA_TNIL :
                break;
            }
            nB=S_OK;
        }
    }
    else
        v = (double) 0.0;
    return nB;
}

int CAdamoMachine::SetExpressionVariable (CString szExpression, int nLevel, COleVariant& v)
{
    CCmdGetGlobalData gd;
    CString str;
    int nB=S_FALSE;

    switch (v.vt)   {
    case VT_R8 :
        str.Format ("%8.3f", v.dblVal);
        break;
    case VT_BSTR :
        str="\""+CString (v.bstrVal)+"\"";
        break;
    case VT_BOOL :
        str=v.lVal ? "true" : "false";
        break;
    }
    szExpression=szExpression+"="+str;
    if (GetGlobalData (nLevel, (const char*)szExpression, &gd))
        nB=S_OK;
    return nB;
}

int  CAdamoMachine::StartEcMailBoxCommand (int nType, int nSlave, int nIndex, int nSubIndex, int nLen, BYTE *pData)
{
    int nB=-1;

    if (m_ptrCnc)   {
		if (nLen<NMAX_EC_RW_MAILBOX_DATA)   {
			stTxEcMailBoxRequest *pTxEcMailBox=new stTxEcMailBoxRequest;
			pTxEcMailBox->m_nType=nType;
			pTxEcMailBox->m_nSlave=nSlave;
			pTxEcMailBox->m_nIndex=nIndex;
			pTxEcMailBox->m_nSubIndex=nSubIndex;
			pTxEcMailBox->m_nLengthData=nLen;
			if (nType==2)
				memcpy (pTxEcMailBox->m_data, pData, nLen);
            InitCmd ();
			if (m_ptrCnc->StartEcMailBoxCommand ((char *)pTxEcMailBox, sizeof (stTxEcMailBoxRequest))==S_OK)
				nB=0;
            EndCmd ();
			_delete (pTxEcMailBox);
		}
    }
    return nB;
}

int  CAdamoMachine::GetEcMailBoxStatus ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->GetEcMailBoxStatus ();
        EndCmd ();
    }
    return nB;
}

int  CAdamoMachine::GetEcMailBoxData (LONG *lResult, int *nLen, BYTE *pData)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        stTxEcMailBoxResults *pTxEcMailBoxResult=new stTxEcMailBoxResults;
        InitCmd ();
        if (m_ptrCnc->GetEcMailBoxData ((char *)pTxEcMailBoxResult, sizeof (stTxEcMailBoxResults))==S_OK)   {
			*lResult=pTxEcMailBoxResult->m_lResult;
			*nLen=pTxEcMailBoxResult->m_nLengthData;
			memcpy (pData, pTxEcMailBoxResult->m_data, pTxEcMailBoxResult->m_nLengthData);
            nB=0;
        }
        EndCmd ();
        _delete (pTxEcMailBoxResult);
    }
    return nB;
}

int  CAdamoMachine::RequestState (int nRequestState, int nSlave)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->EcRequestState (nRequestState, nSlave);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::DownLoadGProgram (CString strGProgram, int nGSlot, int nGType)
{
    _bstr_t bstrGProgram = strGProgram;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadGProgram (bstrGProgram, nGSlot, nGType);
        EndCmd ();
    }
    return 0;
}

int CAdamoMachine::DownLoadGFrameProgramByNumber (CString strGProgram, int nGNumber, int nGFrameLines)
{
    _bstr_t bstrGProgram = strGProgram;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->DownLoadGFrameProgramByNumber (bstrGProgram, nGNumber, nGFrameLines);
        EndCmd ();
    }
	return 0;
}

int CAdamoMachine::ClearGProgram (int nGSlot)
{
    if (m_ptrCnc)   {
        InitCmd ();
		m_ptrCnc->ClearGProgram (nGSlot);
        EndCmd ();
    }
	return 0;
}

int  CAdamoMachine::GetInitializationStatus (int *nInitTest, int *nInitStatus)
{
    int nB = S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetAdamoInitializationStatus (nInitTest, nInitStatus);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::GetEcInfo (VARIANT *pInfo)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetEcInfo (pInfo);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::EcReadyOn (int nSlave, bool bOn)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->EcReadyOn (nSlave, bOn);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::EcServoOn (int nSlave, bool bOn)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->EcServoOn (nSlave, bOn);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::EcResetAlarm (int nSlave)
{
    int nB=-1;
    
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->EcResetAlarm (nSlave);
        EndCmd ();
        nB=0;
    }
    return nB;
}

int CAdamoMachine::StartGDebugging (int nGSlot)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->StartGDebug (nGSlot);
        EndCmd ();
    }
	return 0;
}

int CAdamoMachine::StopGDebugging (int nGSlot)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->StopGDebug (nGSlot);
        EndCmd ();
    }
	return 0;
}

int CAdamoMachine::SendGDebugData (int nGSlot, int *anb, int nb)
{
    IAdaDebugPtr pDbgData;
    HRESULT hr;
    if (m_ptrCnc)   {
        hr=pDbgData.CreateInstance (__uuidof(AdaDebug));
        if (SUCCEEDED (hr))  {
            LPUNKNOWN pUnk=((IUnknown*)pDbgData);
			for (int i = 0; i<nb; i++)
                pDbgData->SetBreakLine (anb[i]);
            InitCmd ();
            m_ptrCnc->SendGDebugData (nGSlot, &pUnk);
            EndCmd ();
        }
    }
	return 0;
}

int CAdamoMachine::SendGDebugCmd (int nGSlot, int nCmd, int nParameter)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SendGDebugCmd (nGSlot, nCmd, nParameter);
        EndCmd ();
    }
	return 0;
}

int CAdamoMachine::GetGDebugVariables (int nGSlot, int nGVarsIndex, int *nGVars, CCmdGVariables* pGVars)
{
	int nAgain = 0;
    if (m_ptrCnc)   {
        InitCmd ();
        nAgain = m_ptrCnc->GetGDebugVariables (nGSlot, nGVarsIndex, nGVars, (char *)pGVars, sizeof (CCmdGVariables) * NMAX_COMMGVARS);
        EndCmd ();
    }
	return nAgain;
}

int CAdamoMachine::SetGDebugVariable (int nGSlot, CString strGVar, VARIANT v)
{
    _bstr_t bstrVar = strGVar;
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->SetGDebugVariable (nGSlot, bstrVar, v);
        EndCmd ();
    }
	return 0;
}

int CAdamoMachine::GetMatrixElement (CString szVar, int nIni, CString strCol, CAdamoData* pData)
{
	COleVariant v;
	int nB = S_OK;

	if (m_ptrCnc)   {
        InitCmd ();
        v = m_ptrCnc->GetMatrixElement (szVar.AllocSysString (), nIni, strCol.AllocSysString ());
		if (v.vt!=VT_EMPTY)   {
			switch (v.vt)   {
				case VT_I4 :
					pData->nType = LUA_TBOOLEAN;
					pData->AdamoData.b = v.lVal ? true : false;
					break;
				case VT_R8 :
					pData->nType = LUA_TNUMBER;
					pData->AdamoData.fp = v.dblVal;
					break;
				case VT_BSTR :
					pData->nType = LUA_TSTRING;
					strcpy (pData->AdamoData.str, CString (v.bstrVal));
					break;
			}
		}
		else
			nB=S_FALSE;
        EndCmd ();
	}
    else
        nB=S_FALSE;
    return nB;
}

void CAdamoMachine::SetTcpIpAddress (BYTE *achTcpIpAddress)
{
    m_achTcpIpAddress[0] = achTcpIpAddress[0];
    m_achTcpIpAddress[1] = achTcpIpAddress[1];
    m_achTcpIpAddress[2] = achTcpIpAddress[2];
    m_achTcpIpAddress[3] = achTcpIpAddress[3];
}

BYTE *CAdamoMachine::GetTcpIpAddress ()
{
	return m_achTcpIpAddress;
}

int CAdamoMachine::GetPort ()
{
	return m_nPort;
}

void CAdamoMachine::SetPort (int nPort)
{
	m_nPort = nPort;
}

void CAdamoMachine::SetTypeModule (eTypeModule eTM)
{
	m_eTypeModule = eTM;
}

eTypeModule CAdamoMachine::GetTypeModule ()
{
	return m_eTypeModule;
}

void CAdamoMachine::SetHWModuleNumber (int nHWModuleNumber)
{
	m_nHWModuleNumber = nHWModuleNumber;
}

int CAdamoMachine::GetHWModuleNumber ()
{
	return m_nHWModuleNumber;
}

int CAdamoMachine::ForceInput (int nInput, bool bForced)
{
    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->ForceInputDig (nInput, bForced);
        EndCmd ();
    }
	return 0;
}

int CAdamoMachine::ConvertSSCNETAbsPos (int nCard, long nJ3J4)
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->ConvertSSCNETAbsPos (nCard, nJ3J4);
        EndCmd ();
    }
    return nB;
}

int CAdamoMachine::OpenLogFile (int nMonth)
{
	int nB = -1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->OpenLogFile (nMonth);
        EndCmd ();
    }
	return nB;
}

int CAdamoMachine::CloseLogFile ()
{
	int nB = -1;

    if (m_ptrCnc)   {
        InitCmd ();
        m_ptrCnc->CloseLogFile ();
        EndCmd ();
		nB = 0;
    }
	return nB;
}

int CAdamoMachine::ReadLogFile (int nRecordDa, int
								nNumeroDirecord, BYTE *pAdamoErrore)
{
	int nRecordLetti = -1;

    if (m_ptrCnc)   {
        InitCmd ();
		nRecordLetti =m_ptrCnc->ReadLogFile (nRecordDa, nNumeroDirecord, pAdamoErrore, COMMBUF);
        EndCmd ();

    }
	return nRecordLetti;
}

int  CAdamoMachine::EnablePing ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->EnablePing ();
        EndCmd ();
    }
	return nB;
}

int  CAdamoMachine::DisablePing ()
{
    int nB=-1;

    if (m_ptrCnc)   {
        InitCmd ();
        nB=m_ptrCnc->DisablePing ();
        EndCmd ();
    }
	return nB;
}

/*
** GetEthernetAdapters :
*/
int  CAdamoMachine::GetEthernetAdapters (CCmdGetAdapters *pAdapters)
{
	int nB = S_FALSE;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetEthernetAdapters ((unsigned char *)pAdapters, sizeof (CCmdGetAdapters));
        EndCmd ();
    }
	return nB;
}

/*
** GetObjectClass :
*/
int CAdamoMachine::GetObjectClass (CString szInstanceName, CString szPath, int nLevel, int prog, CString& szClassName)
{
	COleVariant v;
	int nB = 0;
	int nRet = false;
    if (m_ptrCnc)   {
        InitCmd ();
        nB = m_ptrCnc->GetObjectClass (szInstanceName.AllocSysString (), szPath.AllocSysString (), nLevel, prog, &v);
		if (nB == 1)
			szClassName = CString (v);
        EndCmd ();
	}
	return nB;
}

/* 
** GetBufferLenght :
*/
int CAdamoMachine::GetBufferLenght (int nCmd, CString strParam)
{
	int nBufferLenght = 0;
    if (m_ptrCnc)   {
        InitCmd ();
		nBufferLenght = m_ptrCnc->GetBufferLenght (nCmd, strParam.AllocSysString ());
        EndCmd ();
	}
	return nBufferLenght;
}

/*
** GetBufferData :
*/
void CAdamoMachine::GetBufferData (BYTE* pBuffer, int nBufferLenght)
{
    if (m_ptrCnc)   {
        InitCmd ();
		m_ptrCnc->GetBufferData (pBuffer, nBufferLenght);
        EndCmd ();
	}
}
