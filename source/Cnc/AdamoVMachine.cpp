/*
** AdamoSimulatorMachine.cpp
*/

/* include files */
#include "stdafx.h"
#include <atlbase.h>
#include "adamo.h"
#include "adamoint.h"
#include "AdamoGlobal.h"
#include "AdamoMachine.h"
#include "mainframe.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdamoVMachine::CAdamoVMachine (CObject *pParent) : CAdamoCfgMachine(pParent)
{
}

int CAdamoVMachine::OpenMachine (const char* szMachinePath, int nMachineType, eTypeConnection nConnectionType)
{
    int nB=1;
    /* istanziamo la connessione */
    HRESULT hr = m_ptrCnc.CreateInstance (__uuidof(AdaCommObj), NULL, CLSCTX_INPROC_SERVER);
    if (FAILED(hr))   {
        nB=0;
    }
    else   {
		if (!m_ptrCnc->OpenPlant (m_nModule, szMachinePath))   {
			m_strMachinePath = szMachinePath;
			m_bOpened=true;
			hr=m_ptrCnc->QueryInterface (IID_IUnknown, (void**)&m_objComm);
			if (FAILED(hr))
				nB=0;
			else   {
				CreateSink ();
				CreateAxesList ();
			}
		}
		else
			nB = 0;
    }
    return nB;
}

int CAdamoVMachine::CloseMachine ()
{
    if (m_bOpened)   {
        CloseSink();
        m_ptrCnc->ClosePlant ();
        m_ptrCnc.Release();
        m_bOpened=false;
    }
    return 0;
}

int CAdamoVMachine::ReconnectMachine (eTypeConnection eTC)
{
	int nB = 1;

    if (m_bOpened)
		CloseMachine ();
	nB = OpenMachine (m_strMachinePath, -1, eTC);
    return nB;
}

int CAdamoVMachine::ReplaceMachine ()
{
    return m_ptrCnc->ReplaceMachine (GetMachineType ());
}

int CAdamoVMachine::CreateAxesList ()
{
	Iterator ((CAdamoGroup*)GetAdamoModule());
	return 0;
}

void CAdamoVMachine::Iterator(CAdamoGroup *pGrp)
{
    pGrp->Init();
    pGrp->DevInit();
    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase *)pGrp->DevGetNext();
		if (pDev->GetType ()== enAsse)   {
            InsertAx ((CAdamoAxis *)pDev);
    }
    }
    while(pGrp->More())   {
        CObject *obj = (CObject*)pGrp->GetNext();
        Iterator((CAdamoGroup*)obj);
    }
}

void CAdamoVMachine::InsertAx (CAdamoAxis *pDev)
{
    for (POSITION pos2, pos=m_listAxes.GetHeadPosition(); pos2=pos;)   {
        CAdamoAxis* pListDev=(CAdamoAxis*) m_listAxes.GetNext (pos);
        if (pDev->GetName().Compare (pListDev->GetName ())<0)   {
            m_listAxes.InsertBefore (pos2, pDev);
            return;
        }
    }
    m_listAxes.AddTail (pDev);
}

void CAdamoVMachine::DeleteListAxes ()
{
    m_listAxes.RemoveAll ();
}

void CAdamoVMachine::InitCmd ()
{
    CAdamoStatusBar *pStatusBar = ((CMainFrame *) AfxGetMainWnd ())->GetStatusBar ();
    if (pStatusBar)
        pStatusBar->InitCmd ();
}

void CAdamoVMachine::EndCmd ()
{
    CAdamoStatusBar *pStatusBar = ((CMainFrame *) AfxGetMainWnd ())->GetStatusBar ();
    if (pStatusBar)
        pStatusBar->EndCmd ();
}
