/*
** AdamoBaseList.cpp
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

IMPLEMENT_SERIAL(CAdamoBaseList, CObject, 1)

/*******************
 * CAdamoBaseList : costruttore
 */
CAdamoBaseList::CAdamoBaseList()
{

}

/*******************
 * ~CAdamoBaseList : costruttore
 */
CAdamoBaseList::~CAdamoBaseList()
{
    while (!m_lAdamoList.IsEmpty())
        delete m_lAdamoList.RemoveHead();
}

/*******************
 * Serialize : serializzazione
 */
void CAdamoBaseList::Serialize(CArchive &ar)
{
    CAdamoBase::Serialize(ar);
    
    m_lAdamoList.Serialize(ar);
}

/*******************
 * AddTail : aggiunge oggetto alla fine delle liste
 */
void CAdamoBaseList::AddTail(CObject *obj)
{
    if(!obj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))
        m_lAdamoList.AddTail(obj);
}

/*******************
 * Next : incrementa
 */
CObject* CAdamoExList::Next()
{
    if(m_curPos)
        return (CObject*)m_curList->GetNext(m_curPos);
    
    return NULL;
}

/*******************
 * Current : corrente
 */
CObject* CAdamoExList::Current()
{
    if(m_curPos)
        return (CObject*)m_curList->GetAt(m_curPos);

    return NULL;
}

/*******************
 * More : controllo
 */
BOOL CAdamoExList::More()
{
    if(m_curPos)
        return TRUE;

    return FALSE;
}

/*******************
 * Init : inizializza
 */
void CAdamoExList::Init()
{
    m_curPos = m_curList->GetHeadPosition();
}
