/*
** AdamoAnalogOut.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoAnalogOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoAnalogOut, CObject, 1)

/*******************
 * CAdamoAnalogOut : costruttore
 */
 CAdamoAnalogOut::CAdamoAnalogOut() : m_fpLLInf (-10.0), m_fpLLSup (10.0)
{
}

/*******************
 * ~CAdamoAnalogOut : distruttore
 */
CAdamoAnalogOut::~CAdamoAnalogOut()
{

}

/*****************
 * CAdamoAnalogOut : serializzazione
 */
void CAdamoAnalogOut::Serialize(CArchive &ar)
{
    int i;
    char c=0;
    
    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())    {
        ar << m_fpLLInf;
        ar << m_fpLLSup;
        for (i=0; i<MAX_AXIS_SPARE_OUT_ANAG; ++i)
            ar << c;
    }
    else   {
        ar >> m_fpLLInf;
        ar >> m_fpLLSup;
        for (i=0; i<MAX_AXIS_SPARE_OUT_ANAG; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoAnalogOut::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<AnalogOutput>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<LowRange>%.3f</LowRange>\n", m_fpLLInf);
	pFile->WriteString (str);
	str.Format ("<UpperRange>%.3f</UpperRange>\n", m_fpLLSup);
	pFile->WriteString (str);
	pFile->WriteString ("</AnalogOutput>\n");
}
