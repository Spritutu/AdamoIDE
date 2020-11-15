/*
** AdamoCounter.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoDgtIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoDgtIn, CObject, 1)

/*******************
 * CAdamoDgtIn : costruttore
 */
CAdamoDgtIn::CAdamoDgtIn() : m_nNumInput (1)
{

}

/*******************
 * ~CAdamoDgtIn : distruttore
 */
CAdamoDgtIn::~CAdamoDgtIn()
{

}

/*******************
 * Serialize : serializzazione
 */
void CAdamoDgtIn::Serialize(CArchive &ar)
{
    int i;
    char c=0;

    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())   {
        ar << m_nNumInput;
        for (i=0; i<MAX_AXIS_SPARE_INP_DIG; ++i)
            ar << c;
    }
    else   {
        ar >> m_nNumInput;
        for (i=0; i<MAX_AXIS_SPARE_INP_DIG; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoDgtIn::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<DigitalInput>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<Number>%d</Number>\n", m_nNumInput);
	pFile->WriteString (str);
	pFile->WriteString ("</DigitalInput>\n");
}

