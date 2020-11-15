/*
** AdamoDgtOut.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoDgtOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoDgtOut, CObject, 1)

/*******************
 * CAdamoDgtOut : costruttore
 */
CAdamoDgtOut::CAdamoDgtOut() : m_nIsMonoStable(0), m_nNumOutput (1), m_nTimeOut (100)
{

}

/*******************
 * ~CAdamoDgtOut : distruttore
 */
CAdamoDgtOut::~CAdamoDgtOut()
{

}

/*******************
 * Serialize : serializzazione
 */
void CAdamoDgtOut::Serialize(CArchive &ar)
{
    int i;
    char c=0;

    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())   {
        ar << m_nIsMonoStable << m_nNumOutput ;
        ar << m_nTimeOut;
        for (i=0; i<MAX_AXIS_SPARE_OUT_DIG; ++i)
            ar << c;
    }
    else   {
        ar >> m_nIsMonoStable >> m_nNumOutput ;
        ar >> m_nTimeOut;
        for (i=0; i<MAX_AXIS_SPARE_OUT_DIG; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoDgtOut::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<DigitalOutput>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<Number>%d</Number>\n", m_nNumOutput);
	pFile->WriteString (str);
	str.Format ("<MonoStable>%d</MonoStable>\n", m_nIsMonoStable);
	pFile->WriteString (str);
	str.Format ("<Timeout>%d</Timeout>\n", m_nTimeOut);
	pFile->WriteString (str);
	pFile->WriteString ("</DigitalOutput>\n");
}
