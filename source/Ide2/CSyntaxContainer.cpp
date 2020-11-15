/*
** adamofunctiondictionary.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "CSyntaxContainer.h"

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
CAdamoFunction::CAdamoFunction () : m_nLine (-1)
{
}

CAdamoFunction::~CAdamoFunction ()
{
    list<CAdamoParameter*>::iterator i;
    
    i=m_listParameters.begin();
    while (i!=m_listParameters.end())   {
        delete *i;
        i=m_listParameters.erase(i);
    }
}

/*
** AddParameter :
*/
void CAdamoFunction::AddParameter (CAdamoParameter* pAdamoParameter)
{
    m_listParameters.push_back(pAdamoParameter);
}

/*
** GetParameterList :
*/
void CAdamoFunction::GetParameterList (CStringList& lParameters, CStringList& lDescription)
{
    list<CAdamoParameter*>::iterator i;
	CAdamoParameter* pParameter;
    if (m_listParameters.size())   {
        i=m_listParameters.begin();
        while (i!=m_listParameters.end())   {
			pParameter = *i;
            lParameters.AddTail (pParameter->m_strName);
			i++;
		}
        i=m_listParameters.begin();
        while (i!=m_listParameters.end())   {
			pParameter = *i;
            lDescription.AddTail (pParameter->m_strDescription);
			i++;
		}
	}
}

/*
** GetParameterValues :
*/
void CAdamoFunction::GetParameterValues (CString strParameter, CStringList& lValues)
{
    list<CAdamoParameter*>::iterator i;
	CAdamoParameter* pParameter;
    if (m_listParameters.size())   {
        i=m_listParameters.begin();
        while (i!=m_listParameters.end())   {
			pParameter = *i;
			if (pParameter->m_strName == strParameter)   {
				for (POSITION pos = pParameter->m_strValues.GetHeadPosition (); pos;)   {
					CString str = pParameter->m_strValues.GetNext (pos);
					lValues.AddTail (str);
				}
				break;
			}
			else
				i++;
		}
 	}
}

/*
** GetParameterValues :
*/
void CAdamoFunction::GetParameterEnum (CString strParameter, CString& strEnum)
{
    list<CAdamoParameter*>::iterator i;
	CAdamoParameter* pParameter;
    if (m_listParameters.size())   {
        i=m_listParameters.begin();
        while (i!=m_listParameters.end())   {
			pParameter = *i;
			if (pParameter->m_strName == strParameter)   {
				strEnum = pParameter->m_strEnum;
				break;
			}
			else
				i++;
		}
 	}
}

/*
** operator = :
*/
CAdamoFunction& CAdamoFunction::operator = (CAdamoFunction& function)
{
    list<CAdamoParameter*>::iterator i;
	m_strName = function.m_strName;
	m_strDescription = function.m_strDescription;
	m_strHelp = function.m_strHelp;
	m_nLine = function.m_nLine;
    if (function.m_listParameters.size())   {
        i=function.m_listParameters.begin();
        while (i!=function.m_listParameters.end())   {
			CAdamoParameter* pParameter = new CAdamoParameter;
			pParameter = *i;
			m_listParameters.push_back (pParameter);
			i++;
		}
 	}
	return *this;
}

/* construction / destruction */
CAdamoLibrary::CAdamoLibrary ()
{
}

CAdamoLibrary::~CAdamoLibrary ()
{
    list<CAdamoFunction*>::iterator i;
    
    i=m_listFunctions.begin();
    while (i!=m_listFunctions.end())   {
        delete *i;
        i=m_listFunctions.erase(i);
    }
}

/*
** AddFunction :
*/
void CAdamoLibrary::AddFunction (CAdamoFunction* pAdamoFunction)
{
    m_listFunctions.push_back(pAdamoFunction);
}

/*
** GetFunctionsList :
*/
void CAdamoLibrary::GetFunctionsList (CStringList& lFunctions, CStringList& lDescription)
{
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
            lFunctions.AddTail (pFunction->GetName ());
			i++;
		}
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
            lDescription.AddTail (pFunction->GetDescription ());
			i++;
		}
	}
}

/*
** GetParametersList :
*/
void CAdamoLibrary::GetParametersList (CString strFunction, CStringList& lParameters, CStringList& lDescription)
{
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				pFunction->GetParameterList (lParameters, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetParametersValues :
*/
void CAdamoLibrary::GetParametersValues (CString strFunction, CString strParameter, CStringList& lValues)
{
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				pFunction->GetParameterValues (strParameter, lValues);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetParametersValues :
*/
void CAdamoLibrary::GetParametersEnum (CString strFunction, CString strParameter, CString& strEnum)
{
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				pFunction->GetParameterEnum (strParameter, strEnum);
				break;
			}
			else
				i++;
		}
	}
}

/*
** ExistFunction :
*/
bool CAdamoLibrary::ExistFunction (CString strFunction)
{
	bool bFound = false;
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				bFound = true;
				break;
			}
			i++;
		}
	}
	return bFound;
}

/*
** DeleteFunction :
*/
bool CAdamoLibrary::DeleteFunction (CString strFunction)
{
	bool bFound = false;
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				delete pFunction;
                i=m_listFunctions.erase(i);
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
** GetFunctionHelp :
*/
bool CAdamoLibrary::GetFunctionHelp (CString strFunction, CString& strHelp)
{
	bool bFound = false;
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				strHelp = pFunction->GetHelp ();
				bFound = true;
				break;
			}
			i++;
		}
	}
	return bFound;
}

/*
** UpdateLineNumbers :
*/
void CAdamoLibrary::UpdateLineNumbers (int nStartLine, int nUpdateLines)
{
	CAdamoFunction* pFunction;
    list<CAdamoFunction*>::iterator i;
	i=m_listFunctions.begin();
	while (i!=m_listFunctions.end())   {
		pFunction=*i;
		if (pFunction->GetLine () > nStartLine)
			pFunction->SetLine (pFunction->GetLine () + nUpdateLines);
		i++;
	}
}

/*
** GetFunctionLine :
*/
bool CAdamoLibrary::GetFunctionLine (CString strFunction, int& nLine)
{
	bool bFound = false;
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction;
    if (m_listFunctions.size())   {
        i=m_listFunctions.begin();
        while (i!=m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strFunction)   {
				nLine = pFunction->GetLine ();
				bFound = true;
				break;
			}
			i++;
		}
	}
	return bFound;
}

/* construction / destruction */
CAdamoEnum::CAdamoEnum ()
{
}

CAdamoEnum::~CAdamoEnum ()
{
    list<CAdamoEnumValues*>::iterator i;

    i=m_listEnumValues.begin();
    while (i!=m_listEnumValues.end())   {
        delete *i;
        i=m_listEnumValues.erase(i);
    }
}

/*
** AddEnumValue :
*/
void CAdamoEnum::AddEnumValue (CAdamoEnumValues* pEnumValues)   {
	m_listEnumValues.push_back (pEnumValues);
}

/*
** GetEnumList :
*/
void CAdamoEnum::GetEnumList (CStringList& lEnum, CStringList& lEnumValue)		{
    list<CAdamoEnumValues*>::iterator i;
	CAdamoEnumValues* pEnumValue;
    i=m_listEnumValues.begin();
    while (i!=m_listEnumValues.end())   {
		pEnumValue = *i;
		lEnum.AddTail (pEnumValue->m_strName);
        lEnumValue.AddTail (pEnumValue->m_strValue);
		i++;
	}
}

/* construction / destruction */
CSyntaxContainer::CSyntaxContainer ()
{
}

CSyntaxContainer::~CSyntaxContainer ()
{
    list<CAdamoLibrary*>::iterator i;
    list<CAdamoEnum*>::iterator e;
    
    i=m_listLibraries.begin();
    while (i!=m_listLibraries.end())   {
        delete *i;
        i=m_listLibraries.erase(i);
    }
    e=m_listEnums.begin();
    while (e!=m_listEnums.end())   {
        delete *e;
        e=m_listEnums.erase(e);
    }
    m_mapEnumValues.clear ();
}

/*
** AddLibrary :
*/
void CSyntaxContainer::AddLibrary (CAdamoLibrary* pAdamoLibrary)
{
    m_listLibraries.push_back(pAdamoLibrary);
}

/*
** AddEnum :
*/
void CSyntaxContainer::AddEnum (CAdamoEnum* pAdamoEnum)
{
    m_listEnums.push_back(pAdamoEnum);
}

/*
** LoadXML :
*/
HRESULT CSyntaxContainer::Load() 
{
	DocXmlPtr pDocXml;
	CString strPath = GetPath ();
    HRESULT hr = pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        pDocXml->async = false;
        if (pDocXml->load (strPath.AllocSysString ()))   {
			LoadSyntax (pDocXml);
            hr=S_OK;
        }
        pDocXml.Release ();
    }
    return hr == S_OK ? 0 : -1;
}

/*
** GetLanguageExtension :
*/
CString CSyntaxContainer::GetLanguageExtension () 
{
	return ((CMainFrame *) AfxGetMainWnd ())->GetCurrentLanguage ();
}

/*
** LoadSyntax :
*/
void CSyntaxContainer::LoadSyntax (DocXmlPtr pDocXml)
{
    ElementXmlPtr  pEl=pDocXml->documentElement;
	NodeListXmlPtr  pRootChild;
    CString str=(LPCTSTR)pEl->nodeName;
	bool bError = false;
	
	if (str=="SYNTAX")   {
		if (pEl->hasChildNodes())	  {
			NodeListXmlPtr  pRootChild;
			pRootChild = pEl->childNodes;
			int nItems=pRootChild->length;
			int n = 0;
			while (n<nItems)   {
				str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
				if (str=="FUNCTIONS")   {
					pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
						while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (str=="LIBRARY")   {
								CAdamoLibrary* library = new CAdamoLibrary;
								COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
								if (v.vt==VT_BSTR)                                                      
									library->SetName (CString (v.bstrVal));
								v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
								if (v.vt==VT_BSTR)                                                      
									library->SetDescription (CString (v.bstrVal));
								ElementXmlPtr pEl=pRootChild->Getitem(n);
								if (pEl->hasChildNodes())	  {
									NodeListXmlPtr  pRootChild;
									pRootChild = pEl->childNodes;
									int nItems=pRootChild->length;
									int n = 0;
									while (n<nItems)   {
										str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
										if (str=="FUNCTION")   {
											CAdamoFunction* function = new CAdamoFunction;
											COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
											if (v.vt==VT_BSTR)                                                      
												function->SetName (CString (v.bstrVal));
											v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
											if (v.vt==VT_BSTR)                                                      
												function->SetDescription (CString (v.bstrVal));
											v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("LINE");
											if (v.vt==VT_BSTR)                                                      
												function->SetLine (atol (CString (v.bstrVal)));
											ElementXmlPtr pEl=pRootChild->Getitem(n);
											if (pEl->hasChildNodes())	  {
												NodeListXmlPtr  pRootChild;
												pRootChild = pEl->childNodes;
												int nItems=pRootChild->length;
												int n = 0;
												while (n<nItems)   {
													str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
													if (str=="PARAMETER" || str == "OPT_PARAMETER")   {
														CAdamoParameter* pParameter = new CAdamoParameter;
														if (str == "OPT_PARAMETER")
															pParameter->m_bOptional = true;
														COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
														if (v.vt==VT_BSTR)                                                      
															pParameter->m_strName = CString (v.bstrVal);
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
														if (v.vt==VT_BSTR)                                                      
															pParameter->m_strDescription = CString (v.bstrVal);
														ElementXmlPtr pEl=pRootChild->Getitem(n);
														if (pEl->hasChildNodes())	  {
															NodeListXmlPtr  pRootChild;
															pRootChild = pEl->childNodes;
															int nItems=pRootChild->length;
															int n = 0;
															while (n<nItems)   {
																str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
																if (str=="VALUE")   {
																	str=(LPCTSTR)pRootChild->Getitem(n)->text;
																	pParameter->m_strValues.AddTail (str);
																}
																else
																if (str=="ENUM")   {
																	str=(LPCTSTR)pRootChild->Getitem(n)->text;
																	pParameter->m_strEnum = str;
																}
																n++;
															}
														}
														function->AddParameter (pParameter);
													}
													else
													if (str == "HTML")   {
														COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
														if (v.vt==VT_BSTR)
															function->SetHelp (CString (v.bstrVal));
													}
													n++;
												}
											}
											library->AddFunction (function);
										}
										n++;
									}
								}
								AddLibrary (library);
							}
							n++;
						}
					}
				}
				else
				if (str=="ENUMS")   {
					pEl=pRootChild->Getitem(n);
					NodeListXmlPtr  pRootChild;
					pRootChild = pEl->childNodes;
					int nItems=pRootChild->length;
					int n = 0;
					while (n<nItems)   {
						CAdamoEnum* enumeration = new CAdamoEnum;
						COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
						if (v.vt==VT_BSTR)                                                      
							enumeration->SetName (CString (v.bstrVal));
						v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
						if (v.vt==VT_BSTR)                                                      
							enumeration->SetDescription (CString (v.bstrVal));
						ElementXmlPtr pEl=pRootChild->Getitem(n);
						if (pEl->hasChildNodes())	  {
							NodeListXmlPtr  pRootChild;
							pRootChild = pEl->childNodes;
							int nItems=pRootChild->length;
							int n = 0;
							while (n<nItems)   {
								CAdamoEnumValues* enumvalue = new CAdamoEnumValues;
								v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
								if (v.vt==VT_BSTR)                                                      
									enumvalue->m_strName = CString (v.bstrVal);
								enumvalue->m_strValue = (LPCTSTR)pRootChild->Getitem(n)->text;
								enumeration->AddEnumValue (enumvalue);
								n++;
							}
						}
						AddEnum (enumeration);
						n++;
					}
					BuildEnumMap ();
				}
				n++;
			}
		}
	}
}

/*
** GetLibrariesList :
*/
void CSyntaxContainer::GetLibrariesList (CStringList& lOptions, CStringList& lDescription)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
            lOptions.AddTail (pLibrary->GetName ());
			i++;
		}
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
            lDescription.AddTail (pLibrary->GetDescription ());
			i++;
		}
	}
}

/*
** GetFunctionsList :
*/
void CSyntaxContainer::GetFunctionsList (CString strLibrary, CStringList& lFunctions, CStringList& lDescription)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetFunctionsList (lFunctions, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetParametersList :
*/
void CSyntaxContainer::GetParametersList (CString strLibrary, CString strFunction, CStringList& lParameters, CStringList& lDescription)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetParametersList (strFunction, lParameters, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetParametersValues :
*/
void CSyntaxContainer::GetParametersValues (CString strLibrary, CString strFunction, CString strParameter, CStringList& lValues)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetParametersValues (strFunction, strParameter, lValues);
				break;
			}
			else
				i++;
		}
	}
}

/*
** BuildEnumMap :
*/
void CSyntaxContainer::BuildEnumMap ()
{
	CStringList sel, sevl;
	CString strEnum;
    list<CAdamoEnum*>::iterator i;
	CAdamoEnum* pEnum;
    i=m_listEnums.begin();
    while (i!=m_listEnums.end())   {
		pEnum = *i;
		strEnum = pEnum->GetName ();
		sel.RemoveAll (); sevl.RemoveAll ();
		pEnum->GetEnumList (sel, sevl);
		for (POSITION pos = sel.GetHeadPosition (), pos1 = sevl.GetHeadPosition (); pos; )   {
			CString strEnumName = sel.GetNext (pos);
			CString strEnumValue = sevl.GetNext (pos1);
			CString strKey = strEnum + "." + strEnumName;
			m_mapEnumValues.insert (map<CString, CString>::value_type(strKey, strEnumValue));
		}
		i++;
	}
}

/*
** GetEnumValue :
*/
bool CSyntaxContainer::GetEnumValue (CString strEnumKey, CString& strEnumValue)
{
    map<CString, CString>::iterator it;
	CString str;
	bool b = false;

    it=m_mapEnumValues.find (strEnumKey);
	if (it!=m_mapEnumValues.end())   {
        strEnumValue = (*it).second;
		b = true;
	}
	return b;
}

/*
** GetEnumList :
*/
void CSyntaxContainer::GetEnumList (CStringList& lEnum)
{
    list<CAdamoEnum*>::iterator i;
	CAdamoEnum* pEnum;
    if (m_listEnums.size())   {
        i=m_listEnums.begin();
        while (i!=m_listEnums.end())   {
			pEnum = *i;
			lEnum.AddTail (pEnum->GetName ());
			i++;
		}
	}
}

/*
** GetEnumValue :
*/
void CSyntaxContainer::GetEnumValues (CString strEnum, CStringList& lEnum, CStringList& lEnumValue)
{
    list<CAdamoEnum*>::iterator i;
	CAdamoEnum* pEnum;
    if (m_listEnums.size())   {
        i=m_listEnums.begin();
        while (i!=m_listEnums.end())   {
			pEnum = *i;
			if (pEnum->GetName () == strEnum)   {
				pEnum->GetEnumList (lEnum, lEnumValue);
				break;
			}
			else
				i++;
		}
	}
}

/*
** ExistFunction :
*/
bool CSyntaxContainer::ExistFunction (CString strLibrary, CString strFunction)
{
	bool bFound = false;
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				bFound = pLibrary->ExistFunction (strFunction);
				break;
			}
			else
				i++;
		}	
	}
	return bFound;
}

/*
** GetParametersValues :
*/
void CSyntaxContainer::GetParametersEnum (CString strLibrary, CString strFunction, CString strParameter, CString& strEnum)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetParametersEnum (strFunction, strParameter, strEnum);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetFunctionHelp :
*/
bool CSyntaxContainer::GetFunctionHelp (CString strLibrary, CString strFunction, CString& strHelp)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
	bool bFound = false;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetFunctionHelp (strFunction, strHelp);
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
** GetFunctionHelp :
*/
bool CSyntaxContainer::GetFunctionLine (CString strLibrary, CString strFunction, int& nLine)
{
    list<CAdamoLibrary*>::iterator i;
	CAdamoLibrary* pLibrary;
	bool bFound = false;
    if (m_listLibraries.size())   {
        i=m_listLibraries.begin();
        while (i!=m_listLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				if (pLibrary->GetFunctionLine (strFunction, nLine))   {
					bFound = true;
					break;
				}
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
CString CSyntaxContainer::GetPath ()
{
	CString strPath = ((CMainFrame *)AfxGetMainWnd ())->GetPathHelpOnLine ();
	CString strLang = GetLanguageExtension ();
	CString strFile = "syntax.xml";
	return strPath + "\\" + strLang + "\\" + strFile;
}

/*
** GetWorkPath :
*/
CString CSyntaxContainer::GetWorkPath ()
{
	CString strPath = ((CMainFrame *)AfxGetMainWnd ())->GetPathHelpOnLine ();
	CString strLang = GetLanguageExtension ();
	return strPath + "\\" + strLang;
}
