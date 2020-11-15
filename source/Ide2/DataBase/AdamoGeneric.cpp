/*
** AdamoCanDevice.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoGeneric.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoGenericCan, CObject, 1)

/*******************
 * CAdamoCanDevice : costruttore
 */
CAdamoGenericCan::CAdamoGenericCan()
{

}

/*******************
 * ~CAdamoCanDevice : distruttore
 */
CAdamoGenericCan::~CAdamoGenericCan()
{

}

/******************
 * CAdamoGenericCan : serializzazione
 */
void CAdamoGenericCan::Serialize(CArchive &ar)
{
    int i;
    char c=0;

    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())    {
        for (i=0; i<MAX_AXIS_SPARE_CAN; ++i)
            ar << c;
    }
    else   {
        for (i=0; i<MAX_AXIS_SPARE_CAN; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoGenericCan::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<GenericCan>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	pFile->WriteString ("</GenericCan>\n");
}
