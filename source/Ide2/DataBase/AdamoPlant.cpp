/*
** AdamoPlant.cpp
*/ 

#include "stdafx.h"
#include "AdamoPlant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CAdamoPlant, CObject, 1)

/*******************
 * CAdamoPlant : costruttore
 */
CAdamoPlant::CAdamoPlant()
{
}

/*******************
 * ~CAdamoPlant : distruttore
 */
CAdamoPlant::~CAdamoPlant()
{
}

/*******************
 * Serialize : serializzazione
 */
void CAdamoPlant::Serialize(CArchive &ar)
{
    m_lAdamoModule.Serialize(ar);
}

/*******************
 * AddTailGroup : aggiunge un modulo alla fine della lista
 */
void CAdamoPlant::AddTail(CObject *mdl)
{
    if(mdl->IsKindOf(RUNTIME_CLASS(CProject)))
        m_lAdamoModule.AddTail((CProject*)mdl);   
}

/*******************
 * Init : inizializza
 */
void CAdamoPlant::Init()
{
    m_curPos = m_lAdamoModule.GetHeadPosition();
}

/*******************
 * More : controlla
 */
BOOL CAdamoPlant::More()
{
    if(m_curPos)
        return TRUE;

    return FALSE;

}

/*******************
 * GetNext : incrementa
 */
CObject* CAdamoPlant::GetNext()
{
    if(m_curPos)
        return (CObject*)m_lAdamoModule.GetNext(m_curPos);
    
    return NULL;
}

/*******************
 * GetNext : incrementa
 */
CObject* CAdamoPlant::GetNext(POSITION& pos)
{
	CObject *obj = NULL;

	if (pos)
		obj = (CObject*)m_lAdamoModule.GetNext(pos);
	return obj;
}

/*******************
 * GetCount : restituisce numero moduli
 */
int CAdamoPlant::GetCount()
{
    return m_lAdamoModule.GetCount();
}

/*******
 * Close : cancella oggetti
 */
void CAdamoPlant::Close()
{
    POSITION pos = m_lAdamoModule.GetHeadPosition();

    while(pos)   {
        (m_lAdamoModule.GetAt(pos))->Close();
        m_lAdamoModule.GetNext(pos);
    }
	
    pos = m_lAdamoModule.GetHeadPosition();
    while(pos)   {
        delete m_lAdamoModule.GetNext(pos);
    }

    m_lAdamoModule.RemoveAll();
}

/*******************
 * DeleteObject : cancella un oggetto
 */
void CAdamoPlant::DeleteObject(CObject *pObj)
{
    POSITION pos = m_lAdamoModule.GetHeadPosition();

    while(pos){
        CProject *pPrj = m_lAdamoModule.GetAt(pos);
        if(pObj == pPrj){
            pPrj->Close();
            m_lAdamoModule.RemoveAt(pos);
            delete pPrj;
            break;
        }
        m_lAdamoModule.GetNext(pos);
    }
}

/*******************
 * GetFirst : restituisce primo progetto
 */
CProject* CAdamoPlant::GetFirst()
{
    m_curPos = m_lAdamoModule.GetHeadPosition();

    if(m_curPos)
        return m_lAdamoModule.GetNext(m_curPos);
        
    return NULL;
}
/*******************
 * GetFirst : restituisce primo progetto
 */
CProject* CAdamoPlant::GetFirst(POSITION& pos)
{
	CProject *pPrj = NULL;

    pos = m_lAdamoModule.GetHeadPosition();
	if (pos)
		pPrj = m_lAdamoModule.GetNext(pos);
	return pPrj;
}

/*
**
*/
CProject* CAdamoPlant::GetProject (int nModuleNumber)
{
    POSITION pos = m_lAdamoModule.GetHeadPosition();
	CProject *pPrj, *pValidPrj = NULL;

	for (POSITION pos = m_lAdamoModule.GetHeadPosition(); pos;)   {
        pPrj = (CProject *) m_lAdamoModule.GetNext(pos);
		if (pPrj->GetMachine ())
			if (pPrj->GetMachine ()->GetModuleNumber () == nModuleNumber)   {
				pValidPrj = pPrj; break;
			}
	}
	return pValidPrj;
}
