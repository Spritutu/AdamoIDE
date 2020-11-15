/*
** AdamoTimer.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoTimer, CObject, 1)

/*******************
 * CAdamoTimer : costruttore
 */
CAdamoTimer::CAdamoTimer() : m_nPeriod(1000), m_bEnabled (0), m_nUpDown (0), m_nStored (0)
{

}

/*******************
 * ~CAdamoTimer : distruttore
 */
CAdamoTimer::~CAdamoTimer()
{

}

/*******************
 * Serialize : serializzazione
 */
void CAdamoTimer::Serialize(CArchive &ar)
{
    int i;
    char c=0;

    CAdamoBase::Serialize(ar);
    if (ar.IsStoring())	{
        ar << m_nPeriod;
        ar << m_nUpDown;
	    ar << m_bEnabled;
	    ar << m_nStored;
        for (i=0; i<MAX_AXIS_SPARE_TIMER; ++i)
            ar << c;
    }
    else	  {
        ar >> m_nPeriod;
        ar >> m_nUpDown;
	    ar >> m_bEnabled;
	    ar >> m_nStored;
        for (i=0; i<MAX_AXIS_SPARE_TIMER; ++i)
            ar >> c;
    }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoTimer::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<Timer>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<Period>%d</Period>\n", m_nPeriod);
	pFile->WriteString (str);
	str.Format ("<Enabled>%d</Enabled>\n", m_bEnabled);
	pFile->WriteString (str);
	str.Format ("<UpDown>%d</UpDown>\n", m_nUpDown);
	pFile->WriteString (str);
	str.Format ("<Stored>%d</Stored>\n", m_nStored);
	pFile->WriteString (str);
	pFile->WriteString ("</Timer>\n");
}
