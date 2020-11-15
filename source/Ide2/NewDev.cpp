// NewDev.cpp: implementation of the CNewDev class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "NewDev.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewDev::CNewDev():m_pAdamoBase(NULL)
{

}

CNewDev::CNewDev(CRuntimeClass* pClass,CObject *pNode):m_pAdamoBase(NULL)
{
	SetObj(pClass,pNode);
}

CNewDev::~CNewDev()
{

}

CAdamoBase* CNewDev::GetObject()
{
    return m_pAdamoBase;
}

void CNewDev::NameManage(CObject *pNode)
{
    if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoGroup))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_1),pNode);
    }if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoAxis))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_2),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoAnalogIn))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_3),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoAnalogOut))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_4),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoDgtIn))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_5),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoDgtOut))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_6),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoCounter))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_7),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoTimer))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_8),pNode);
    }else if(m_pAdamoBase->IsKindOf(RUNTIME_CLASS(CAdamoGenericCan))){
        NameManageControl(LOADSTRING (IDS_ADAMONEWDEV_9),pNode);
    }
}

void CNewDev::NameManageControl(CString strName,CObject *pNode)
{
    CString strNew(strName);
    int nCount = 0;

    if(pNode->IsKindOf(RUNTIME_CLASS(CProject)))
        pNode = ((CProject*)pNode)->GetMachineModule();

	((CAdamoGroup*)pNode)->Init();
    ((CAdamoGroup*)pNode)->DevInit();

    while(((CAdamoGroup*)pNode)->DevMore()){
        CAdamoBase *pDev = (CAdamoBase*)((CAdamoGroup*)pNode)->DevGetNext();
        if(pDev->GetName() == strNew)
            strNew.Format(strName+"(%d)",++nCount);
    }

    while(((CAdamoGroup*)pNode)->More()){
        CAdamoBase *pGroup = (CAdamoBase*)((CAdamoGroup*)pNode)->GetNext();
        if(pGroup->GetName() == strNew)
            strNew.Format(strName+"(%d)",++nCount);
    }

    m_pAdamoBase->SetName(strNew);
}

void CNewDev::SetObj(CRuntimeClass* pClass,CObject *pNode)
{
    if(!pNode)
        return;

    m_pAdamoBase = (CAdamoBase*)pClass->CreateObject();

    if(m_pAdamoBase)
        NameManage(pNode);
}