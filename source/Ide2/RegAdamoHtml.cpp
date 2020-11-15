// RegAdamoHtml.cpp: implementation of the CRegAdamoHtml class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "RegAdamoHtml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;

#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegAdamoHtml::CRegAdamoHtml()
{

}

CRegAdamoHtml::~CRegAdamoHtml()
{

}

/*
** GetPathAdamoHtml: leggo dal registro il path dell'hardware
*/
CString CRegAdamoHtml::GetPathAdamoHtml()
{
	char    strPath[256];
    DWORD   dwCount=256;
    CRegKey reg;
    CString strPathAdamoHtml;

    LONG lOpen = reg.Open (HKEY_LOCAL_MACHINE, REG_KEY_HW, KEY_READ);
    LONG lQuery = reg.QueryValue (strPath, "PathHtml", &dwCount);
	reg.Close ();
	if (lOpen == ERROR_SUCCESS && lQuery == ERROR_SUCCESS && dwCount != 0) {
		 strPathAdamoHtml = strPath;
	}
	
	return strPathAdamoHtml;
}
