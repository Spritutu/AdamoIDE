/*
** AdamoCounter.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoCounter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoCounter, CObject, 1)

/*******************
 * CAdamoCounter : costruttore
 */
 CAdamoCounter::CAdamoCounter() : m_nType (0)
{

}

/*******************
 * ~CAdamoCounter : distruttore
 */
CAdamoCounter::~CAdamoCounter()
{

}

/*******************
 * Serialize : serializzazione
 */
void CAdamoCounter::Serialize(CArchive &ar)
{
    int i;
    char c=0;

    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())   {
       ar << m_nType;
        for (i=0; i<MAX_AXIS_SPARE_COUNTER; ++i)
            ar << c;
    }
    else   {
        ar >> m_nType;
        for (i=0; i<MAX_AXIS_SPARE_COUNTER; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoCounter::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<Counter>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<Type>%d</Type>\n", m_nType);
	pFile->WriteString (str);
	pFile->WriteString ("</Counter>\n");
}
