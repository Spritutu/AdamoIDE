/*
** adamofunctiondictionary.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "CFunctionsContainer.h"

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/* construction / destruction */
CFunctionsContainer::CFunctionsContainer ()
{
}

CFunctionsContainer::~CFunctionsContainer ()
{
	ClearAll ();
}

/*
** Clear :
*/
void CFunctionsContainer::ClearAll ()
{
    list<CAdamoLibrary*>::iterator i;
    
    i=m_listLibraries.begin();
    while (i!=m_listLibraries.end())   {
        delete *i;
        i=m_listLibraries.erase(i);
    }
    m_mapEnumValues.clear ();
}

/*
** Save :
*/
void CFunctionsContainer::Save ()
{
	CStdioFile file;
	CString strPath = GetPath ();
	if (file.Open (strPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))   {
		WritePrologue (&file);
		WriteData (&file);
		WriteEpilogue (&file);
		file.Close ();
	}
}

/*
** WritePrologue :
*/
void CFunctionsContainer::WritePrologue (CStdioFile* pStdioFile)
{
	pStdioFile->WriteString ("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	pStdioFile->WriteString ("<SYNTAX>");
}

/*
** WriteData :
*/
void CFunctionsContainer::WriteData (CStdioFile* pStdioFile)
{
    list<CAdamoLibrary*>::iterator l;
    list<CAdamoFunction*>::iterator f;
    list<CAdamoParameter*>::iterator p;
	CAdamoLibrary* pLibrary;
	CAdamoFunction* pFunction;
	CAdamoParameter* pParameter;
	int nLine;
	pStdioFile->WriteString ("<FUNCTIONS>");
    
    l=m_listLibraries.begin();
    while (l!=m_listLibraries.end())   {
        pLibrary = *l;
		CString strName = pLibrary->GetName ();
		CString strDescription = pLibrary->GetDescription ();
		CString strData;
		strData.Format ("<LIBRARY NAME=\"%s\" DESCRIPTION=\"%s\">", strName, strDescription);
		pStdioFile->WriteString (strData);
		f = pLibrary->GetListFunctions ()->begin ();
		while (f!=pLibrary->GetListFunctions ()->end())   {
			pFunction = *f;
			strName = pFunction->GetName ();
			strDescription = pFunction->GetDescription ();
			nLine = pFunction->GetLine ();
			strData.Format ("<FUNCTION NAME=\"%s\" DESCRIPTION=\"%s\" LINE=\"%d\">", strName, strDescription, nLine);
			pStdioFile->WriteString (strData);
			p = pFunction->GetListParameters ()->begin ();
			while (p!=pFunction->GetListParameters ()->end())   {
				pParameter= *p;
				strName = pParameter->m_strName;
				strDescription = pParameter->m_strDescription;
				strData.Format ("<PARAMETER NAME=\"%s\" DESCRIPTION=\"%s\"></PARAMETER>", strName, strDescription);
				pStdioFile->WriteString (strData);
				p++;
			}
			pStdioFile->WriteString ("</FUNCTION>");
			f++;
		}
		l++;
		pStdioFile->WriteString ("</LIBRARY>");
    }
	pStdioFile->WriteString ("</FUNCTIONS>");
}

/*
** WriteEpilogue :
*/
void CFunctionsContainer::WriteEpilogue (CStdioFile* pStdioFile)
{
	pStdioFile->WriteString ("</SYNTAX>");
}

/*
** UpdateLineNumbers :
*/
void CFunctionsContainer::UpdateLineNumbers (CString strModule, int nStartLine, int nUpdateLines, int nDeleteLine)
{
    list<CAdamoLibrary*>::iterator l;
	CAdamoLibrary* pLibrary;

	l=m_listLibraries.begin();
    while (l!=m_listLibraries.end())   {
        pLibrary = *l;
		if (pLibrary->GetName () == strModule)   {
			pLibrary->UpdateLineNumbers (nStartLine, nUpdateLines);
			break;
		}
		l++;
	}
}

/*
** ExistFunction :
*/
bool CFunctionsContainer::ExistFunction (CString strFunction, CString& strModule)
{
	bool bFound = false;
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->ExistFunction (strFunction))   {
				strModule = pLibrary->GetName ();
				bFound = true;
				break;
			}
			else
				i++;
		}	
	}
	return bFound;
}

/*
** DeleteFunction :
*/
bool CFunctionsContainer::DeleteFunction (CString strFunction)
{
	bool bFound = false;
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->DeleteFunction (strFunction))   {
				bFound = true;
				break;
			}
			else
				i++;
		}	
	}
	return bFound;
}

/*
** DeleteFunction :
*/
bool CFunctionsContainer::RemoveModule (CString strModule)
{
	bool bFound = false;
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strModule)   {
				delete pLibrary;
                i=m_listLibraries.erase(i);
				bFound = true;
				break;
			}
			else
				i++;
		}	
	}
	return bFound;
}

/*
** GetPath :
*/
CString CFunctionsContainer::GetPath ()
{
    CProject *pPrj=GETWS ()->GetActiveProject ();
	CString strPath;
	CString strFile;

	strPath = m_strPath;
	strFile = "functions.xml";
	return strPath + "\\" + strFile;
}
