/*
** AdamoAnalogIn.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoAnalogIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoAnalogIn, CObject, 1)

/*******************
 * CAdamoAnalogIn : costruttore
 */
CAdamoAnalogIn::CAdamoAnalogIn() : m_nType(0), m_fpLLInf (-10.0), m_fpLLSup (10.0)
{
    
}

/*******************
 * ~CAdamoAnalogIn : distruttore
 */
CAdamoAnalogIn::~CAdamoAnalogIn()
{

}

/*******************
 * CAdamoAnalogIn : serializzazione
 */
void CAdamoAnalogIn::Serialize(CArchive &ar)
{
    int i;
    char c=0;
    
    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())    {
        ar << m_nType;
        ar << m_fpLLInf;
        ar << m_fpLLSup;
        for (i=0; i<MAX_AXIS_SPARE_INP_ANAG; ++i)
            ar << c;
    }
    else   {
        ar >> m_nType;
        ar >> m_fpLLInf;
        ar >> m_fpLLSup;
        for (i=0; i<MAX_AXIS_SPARE_INP_ANAG; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoAnalogIn::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<AnalogInput>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<Type>%d</Type>\n", m_nType);
	pFile->WriteString (str);
	str.Format ("<LowRange>%.3f</LowRange>\n", m_fpLLInf);
	pFile->WriteString (str);
	str.Format ("<UpperRange>%.3f</UpperRange>\n", m_fpLLSup);
	pFile->WriteString (str);
	pFile->WriteString ("</AnalogInput>\n");
}
