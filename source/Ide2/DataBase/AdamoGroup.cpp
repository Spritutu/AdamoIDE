/*
** AdamoGroup.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoBaseList.h"
#include "AdamoGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoGroup, CAdamoBaseList, 1)

/*******************
 * CAdamoGroup : costruttore
 */
CAdamoGroup::CAdamoGroup()
{
    m_pEx = new CAdamoExList(&m_lAdamoGroup);
    m_pExList = new CAdamoExList(&m_lAdamoList);
}

/*******************
 * ~CAdamoGroup : distruttore
 */
CAdamoGroup::~CAdamoGroup()
{
    delete m_pEx;
    delete m_pExList;

    while (!m_lAdamoGroup.IsEmpty())
        delete m_lAdamoGroup.RemoveHead();
}

/*******************
 * Serialize : serializzazione
 */
void CAdamoGroup::Serialize(CArchive &ar)
{
    CAdamoBaseList::Serialize(ar);
    m_lAdamoGroup.Serialize(ar);
}

/*******************
 * AddTail : aggiunge un dispositivo nella coda della lista
 */
void CAdamoGroup::AddTail(CObject *obj)
{
    if(obj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))
        m_lAdamoGroup.AddTail((CAdamoGroup*)obj);    

    CAdamoBaseList::AddTail(obj);
}   

/*******************
 * More : controlla lista
 */
BOOL CAdamoGroup::More()
{
    return m_pEx->More();
}

/*******************
 * GetNext : incrementa lista
 */
CObject* CAdamoGroup::GetNext()
{
    return m_pEx->Next();
}

/*******************
 * Init : inizializza lista
 */
void CAdamoGroup::Init()
{
    m_pEx->Init();
}

/*******************
 * DevMore : controlla lista device
 */
BOOL CAdamoGroup::DevMore()
{
    if(m_pExList->More())
        return TRUE;

    return FALSE;
}

/*******************
 * DevGetNext : incrementa lista device
 */
CObject* CAdamoGroup::DevGetNext()
{
    if(m_pExList->Current())
        return m_pExList->Next();

    return FALSE;
}

/*******************
 * DevInit : inizializza lista device
 */
void CAdamoGroup::DevInit()
{
    m_pExList->Init();
}

/*******************
 * DeleteObject : cancella oggetto e riferimento nella lista
 */
BOOL CAdamoGroup::DeleteObject(CObject *pObj)
{
    BOOL bExit = FALSE;
    POSITION posGroup = m_lAdamoGroup.GetHeadPosition();
    POSITION posList = m_lAdamoList.GetHeadPosition();
    
    while(posGroup && !bExit){
        if(m_lAdamoGroup.GetAt(posGroup) == pObj){
            m_lAdamoGroup.RemoveAt(posGroup);
            delete pObj;
            bExit = TRUE;
            break;
        }
        bExit = m_lAdamoGroup.GetNext(posGroup)->DeleteObject(pObj);
    }

    while(posList && !bExit){
        if(m_lAdamoList.GetAt(posList) == pObj){
            m_lAdamoList.RemoveAt(posList);
            delete pObj;
            bExit = TRUE;
            break;
        }
        m_lAdamoList.GetNext(posList);
    }

    return bExit;
}
