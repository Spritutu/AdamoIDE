/*
** CObjectsContainer.h
*/
#pragma once

#include "stdafx.h"
#include "mainframe.h"
#include "CObjectsContainer.h"

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/* construction / destruction CAdamoObj */
CAdamoObj::CAdamoObj ()
{
}

CAdamoObj::~CAdamoObj ()
{
    list<CAdamoObjectElement*>::iterator i;
    
    i=m_listObjectElement.begin();
    while (i!=m_listObjectElement.end())   {
        delete *i;
        i=m_listObjectElement.erase(i);
    }
}

/* construction / destruction Library */
CObjectsLibrary::CObjectsLibrary ()
{
}

CObjectsLibrary::~CObjectsLibrary ()   
{
    list<CAdamoObj*>::iterator i;
    
    i=m_listObjects.begin();
    while (i!=m_listObjects.end())   {
        delete *i;
        i=m_listObjects.erase(i);
    }
}

/* construction / destruction object container */
CObjectsContainer::CObjectsContainer ()   
{
}

CObjectsContainer::~CObjectsContainer ()
{
	ClearAll ();
}

/*
** AddElement :
*/
void CAdamoObj::AddElement (CAdamoObjectElement* pElement)
{
	m_listObjectElement.push_back (pElement);
}

/*
** AddObject :
*/
void CObjectsLibrary::AddObject (CAdamoObj* pObject)
{
	m_listObjects.push_back (pObject);
}

/* construction / destruction */

/*
** AddObjectsLibrary :
*/
void CObjectsContainer::AddObjectsLibrary (CObjectsLibrary* pObjectsLibrary)
{
	list<CObjectsLibrary*>::iterator i;
	if (m_listObjectLibraries.size()) {
		i = m_listObjectLibraries.begin();
		while (i != m_listObjectLibraries.end()) {
			if ((*i)->GetName () == pObjectsLibrary->GetName ())
				AfxMessageBox("Attention Plese !! Library already in the collection ...");
			i++;
		}
	}
	m_listObjectLibraries.push_back(pObjectsLibrary);
}

/*
** Clear :
*/
void CObjectsContainer::ClearAll ()
{
    list<CObjectsLibrary*>::iterator i;
    
    i=m_listObjectLibraries.begin();
    while (i!=m_listObjectLibraries.end())   {
        delete *i;
        i=m_listObjectLibraries.erase(i);
    }
}

/*
** Save :
*/
void CObjectsContainer::Save ()
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
void CObjectsContainer::WritePrologue (CStdioFile* pStdioFile)
{
	pStdioFile->WriteString ("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	pStdioFile->WriteString ("<SYNTAX>");
}

/*
** WriteData :
*/
void CObjectsContainer::WriteData (CStdioFile* pStdioFile)
{
    list<CObjectsLibrary*>::iterator l;
    list<CAdamoObj*>::iterator o;
    list<CAdamoObjectElement*>::iterator e;
    list<CAdamoParameter*>::iterator p;
	CObjectsLibrary* pObjectsLibrary;
	CAdamoObj* pObj;
	CAdamoObjectElement* pObjectElement;
	CAdamoParameter* pParameter;
	int nLine;

	pStdioFile->WriteString ("<OBJECTS>");
    l=m_listObjectLibraries.begin();
    while (l!=m_listObjectLibraries.end())   {
        pObjectsLibrary = *l;
		CPath strName = pObjectsLibrary->GetName ();
		CString strDescription = pObjectsLibrary->GetDescription ();
		CString strData;
		strData.Format ("<LIBRARY NAME=\"%s\" DESCRIPTION=\"%s\">", strName.GetName (), strDescription);
		pStdioFile->WriteString (strData);
		o = pObjectsLibrary->GetListObjects ()->begin ();
		while (o!=pObjectsLibrary->GetListObjects ()->end())   {
			pObj = *o;
			strName = pObj->GetName ();
			strDescription = pObj->GetDescription ();
			nLine = pObj->GetLine ();
			strData.Format ("<OBJECT NAME=\"%s\" DESCRIPTION=\"%s\" LINE=\"%d\">", strName, strDescription, nLine);
			pStdioFile->WriteString (strData);
			e = pObj->GetListObjectElements().begin ();
			while (e!=pObj->GetListObjectElements().end())   {
				pObjectElement = *e;
				strName = pObjectElement->GetName ();
				strDescription = pObjectElement->GetDescription ();
				bool bIsPublic = pObjectElement->GetPublic ();
				switch (pObjectElement->KindOf ())   {
					case eData :   {
						int nType = ((CAdamoObjectData *)pObjectElement)->GetData ().nType;
						CString strValue;
						switch (nType)   {
							case LUA_TBOOLEAN :
								strValue.Format ("%d", ((CAdamoObjectData *)pObjectElement)->GetData ().AdamoData.b);
								break;
							case LUA_TNUMBER :
								strValue.Format ("%.3f", ((CAdamoObjectData *)pObjectElement)->GetData ().AdamoData.fp);
								break;
							case LUA_TSTRING :
							case LUA_TTABLE :
								strValue = ((CAdamoObjectData *)pObjectElement)->GetData ().AdamoData.str;
								break;
						}
						strData.Format ("<DATA NAME=\"%s\" DESCRIPTION=\"%s\" PUBLIC=\"%d\" TYPE=\"%d\">%s</DATA>", strName, strDescription, bIsPublic, nType, strValue);
						pStdioFile->WriteString (strData);
						break;
					}
					case eMethod :   {
						bool bIsVirtual = ((CAdamoObjectMethod *)pObjectElement)->GetVirtual ();
						strData.Format ("<METHOD NAME=\"%s\" DESCRIPTION=\"%s\" PUBLIC=\"%d\" VIRTUAL=\"%d\">", strName, strDescription, bIsPublic, bIsVirtual, bIsVirtual);
						pStdioFile->WriteString (strData);
						p = ((CAdamoObjectMethod *)pObjectElement)->GetListParameters ()->begin ();
						while (p!=((CAdamoObjectMethod *)pObjectElement)->GetListParameters ()->end())   {
							pParameter= *p;
							strName = pParameter->m_strName;
							strDescription = pParameter->m_strDescription;
							strData.Format ("<PARAMETER NAME=\"%s\" DESCRIPTION=\"%s\"></PARAMETER>", strName, strDescription);
							pStdioFile->WriteString (strData);
							p++;
						}
						pStdioFile->WriteString ("</METHOD>");
						break;
					}
				}
				e++;
			}
			pStdioFile->WriteString ("</OBJECT>");
			o++;
		}
		l++;
		pStdioFile->WriteString ("</LIBRARY>");
    }
	pStdioFile->WriteString ("</OBJECTS>");
}

/*
** WriteEpilogue :
*/
void CObjectsContainer::WriteEpilogue (CStdioFile* pStdioFile)
{
	pStdioFile->WriteString ("</SYNTAX>");
}

/*
** GetPath :
*/
CString CObjectsContainer::GetPath ()
{
    CProject *pPrj=GETWS ()->GetActiveProject ();
	CString strPath;
	CString strFile;

	strPath = m_strPath;
	strFile = "objects.xml";
	return strPath + "\\" + strFile;
}

/*
** LoadObjects :
*/
void CObjectsContainer::LoadObjects (DocXmlPtr pDocXml)
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
				if (str=="OBJECTS")   {
					pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
						while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (str=="LIBRARY")   {
								CObjectsLibrary* objlibrary = new CObjectsLibrary;
								COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
								if (v.vt==VT_BSTR)                                                      
									objlibrary->SetName (m_strPath + "\\" + CString (v.bstrVal));
								v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
								if (v.vt==VT_BSTR)                                                      
									objlibrary->SetDescription (CString (v.bstrVal));
								ElementXmlPtr pEl=pRootChild->Getitem(n);
								if (pEl->hasChildNodes())	  {
									NodeListXmlPtr  pRootChild;
									pRootChild = pEl->childNodes;
									int nItems=pRootChild->length;
									int n = 0;
									while (n<nItems)   {
										str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
										if (str=="OBJECT")   {
											CAdamoObj* obj = new CAdamoObj;
											COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
											if (v.vt==VT_BSTR)                                                      
												obj->SetName (CString (v.bstrVal));
											v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
											if (v.vt==VT_BSTR)                                                      
												obj->SetDescription (CString (v.bstrVal));
											v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("LINE");
											if (v.vt==VT_BSTR)                                                      
												obj->SetLine (atol (CString (v.bstrVal)));
											ElementXmlPtr pEl=pRootChild->Getitem(n);
											if (pEl->hasChildNodes())	  {
												NodeListXmlPtr  pRootChild;
												pRootChild = pEl->childNodes;
												int nItems=pRootChild->length;
												int n = 0;
												while (n<nItems)   {
													str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
													if (str=="DATA")   {
														CAdamoObjectData* pData= new CAdamoObjectData;
														CAdamoData ad;
														COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
														if (v.vt==VT_BSTR)                                                      
															pData->SetName (CString (v.bstrVal));
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
														if (v.vt==VT_BSTR)                                                      
															pData->SetDescription (CString (v.bstrVal));
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("PUBLIC");
														if (v.vt==VT_BSTR)
															pData->SetPublic (atoi (CString (v.bstrVal)) ? true : false);
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("TYPE");
														if (v.vt==VT_BSTR)   {
															ad.nType = atoi (CString (v.bstrVal));
															str=(LPCTSTR)pRootChild->Getitem(n)->text;
															switch (ad.nType)   {
																case LUA_TBOOLEAN :
																	ad.AdamoData.b = str == "0";
																	break;
																case LUA_TNUMBER :
																	ad.AdamoData.fp = atof (str);
																	break;
																case LUA_TSTRING :
																case LUA_TTABLE :
																	strcpy (ad.AdamoData.str, str);
																	break;
																default :
																	ad.AdamoData.str[0] = '\0';
																	break;
															}
														}
														pData->SetData (ad);
														obj->AddElement (pData);
													}
													else
													if (str=="METHOD")   {
														CAdamoObjectMethod* pMethod= new CAdamoObjectMethod;
														COleVariant v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("NAME");
														if (v.vt==VT_BSTR)                                                      
															((CAdamoObjectElement*)pMethod)->SetName (CString (v.bstrVal));
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("DESCRIPTION");
														if (v.vt==VT_BSTR)                                                      
															((CAdamoObjectElement*)pMethod)->SetDescription (CString (v.bstrVal));
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("PUBLIC");
														if (v.vt==VT_BSTR)
															pMethod->SetPublic (atoi (CString (v.bstrVal)) ? true : false);
														v = ((ElementXmlPtr) pRootChild->Getitem(n))->getAttribute("VIRTUAL");
														if (v.vt==VT_BSTR)
															pMethod->SetVirtual (atoi (CString (v.bstrVal)) ? true : false);
														if (pEl->hasChildNodes())	  {
															NodeListXmlPtr  pRootChild;
															pRootChild = pEl->childNodes;
															int nItems=pRootChild->length;
															int n = 0;
															while (n<nItems)   {
																str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
																if (str=="PARAMETER")   {
																	CAdamoParameter* pParameter = new CAdamoParameter;
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
																			n++;
																		}
																	}
																	pMethod->AddParameter (pParameter);
																}
																n++;
															}
														}
														obj->AddElement (pMethod);
													}
													n++;
												}
											}
											objlibrary->AddObject (obj);
										}
										n++;
									}
								}
								AddObjectsLibrary (objlibrary);
							}
							n++;
						}
					}
				}
				n++;
			}
		}
	}
}

/*
** Load :
*/
HRESULT CObjectsContainer::Load() 
{
	DocXmlPtr pDocXml;
	CString strPath = GetPath ();
    HRESULT hr = pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        pDocXml->async = false;
        if (pDocXml->load (strPath.AllocSysString ()))   {
			LoadObjects (pDocXml);
            hr=S_OK;
        }
        pDocXml.Release ();
    }
    return hr == S_OK ? 0 : -1;
}

/*
** RemoveModule :
*/
bool CObjectsContainer::RemoveModule (CString strModule)
{
	bool bFound = false;
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pObjectLibrary;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pObjectLibrary = *i;
			if (pObjectLibrary->GetName () == strModule)   {
				delete pObjectLibrary;
                i=m_listObjectLibraries.erase(i);
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
** GetLibrariesList :
*/
void CObjectsContainer::GetLibrariesList (CStringList& lOptions, CStringList& lDescription)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
            lOptions.AddTail (pLibrary->GetName ());
			i++;
		}
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
            lDescription.AddTail (pLibrary->GetDescription ());
			i++;
		}
	}
}

/*
** GetObjectsList :
*/
void CObjectsContainer::GetObjectsList (CString strLibrary, CStringList& lObjects, CStringList& lDescription)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetObjectsList (lObjects, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetObjectsElements :
*/
void CObjectsContainer::GetObjectsElements (CString strLibrary, CString strObject, list<CAdamoObjectElement *>& lObjectElement)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetObjectElements (strObject, lObjectElement);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetObjectMethodParameter :
*/
void CObjectsContainer::GetParameterList (CString strLibrary, CString strObject, CString strMethod, CStringList& lParameters, CStringList& lDescription)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				pLibrary->GetParameterList (strObject, strMethod, lParameters, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetFunctionsList :
*/
void CObjectsLibrary::GetObjectsList (CStringList& lObjects, CStringList& lDescription)
{
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pObjects;
    if (m_listObjects.size())   {
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObjects = *i;
            lObjects.AddTail (pObjects->GetName ());
			i++;
		}
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObjects = *i;
            lDescription.AddTail (pObjects->GetDescription ());
			i++;
		}
	}
}

/*
** GetObjectElements :
*/
void CObjectsLibrary::GetObjectElements (CString strObject, list<CAdamoObjectElement *>& lObjectElement)
{
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pObjects;
    if (m_listObjects.size())   {
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObjects = *i;
			if (pObjects->GetName () == strObject)   {
				lObjectElement = pObjects->GetListObjectElements ();
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetParameterList :
*/
void CObjectsLibrary::GetParameterList (CString strObject, CString strMethod, CStringList& lParameters, CStringList& lDescription)
{
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pObjects;
    if (m_listObjects.size())   {
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObjects = *i;
			if (pObjects->GetName () == strObject)   {
				pObjects->GetParameterList (strMethod, lParameters, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetParameterList :
*/
void CAdamoObj::GetParameterList (CString strMethod, CStringList& lParameters, CStringList& lDescription)
{
    list<CAdamoObjectElement*>::iterator i;
	CAdamoObjectElement* pObjectsElement;
    if (m_listObjectElement.size())   {
        i=m_listObjectElement.begin();
        while (i!=m_listObjectElement.end())   {
			pObjectsElement = *i;
			if (pObjectsElement->KindOf () == eMethod && pObjectsElement->GetName () == strMethod)   {
				((CAdamoObjectMethod *)pObjectsElement)->GetParameterList (lParameters, lDescription);
				break;
			}
			else
				i++;
		}
	}
}

/*
** GetObjectElementLine :
*/
bool CAdamoObj::GetObjectElementLine (CString strObjectElement, int& nLine)
{
	bool bFound = false;

	nLine = -1;
    list<CAdamoObjectElement*>::iterator i;
	CAdamoObjectElement* pObjectsElement;
    if (m_listObjectElement.size())   {
        i=m_listObjectElement.begin();
        while (i!=m_listObjectElement.end())   {
			pObjectsElement = *i;
			if (pObjectsElement->GetName () == strObjectElement)   {
				switch (pObjectsElement->KindOf ())   {
					case eData :
						nLine = pObjectsElement->GetDataLine (); break;
					case eMethod :
						nLine = ((CAdamoObjectMethod *)pObjectsElement)->GetLine (); break;
				}
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
** GetObjectElementLine :
*/
CAdamoObj& CAdamoObj::operator =(CAdamoObj& obj)
{
    list<CAdamoObjectElement*>::iterator i;
	CAdamoObjectElement* pObjectsElement;
    if (obj.m_listObjectElement.size())   {
        i=obj.m_listObjectElement.begin();
        while (i!=obj.m_listObjectElement.end())   {
			pObjectsElement = *i;
			switch (pObjectsElement->KindOf ())   {
				case eData :   {
					CAdamoObjectData* pObjData = new CAdamoObjectData;
					*pObjData = *(CAdamoObjectData*)pObjectsElement;
					m_listObjectElement.push_back (pObjData);
					break;
				}
				case eMethod :
					CAdamoObjectMethod* pObjMethod = new CAdamoObjectMethod;
					*pObjMethod = *(CAdamoObjectMethod*) pObjectsElement;
					m_listObjectElement.push_back (pObjMethod);
					break;
			}
			i++;
		}
		m_strName = obj.m_strName;
		m_strDescription = obj.m_strDescription;
		m_strHelp = obj.m_strHelp;
		m_strBasedObj = obj.m_strBasedObj;
		m_nLine = obj.m_nLine;
	}
	return *this;
}

/*
** GetObjectLine :
*/
bool CObjectsContainer::GetObjectLine (CString strLibrary, CString strObject, int& nLine)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
	bool bFound = false;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				if (pLibrary->GetObjectLine (strObject, nLine))   {
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
** GetObjectElementLine :
*/
bool CObjectsContainer::GetObjectElementLine (CString strLibrary, CString strObject, CString strObjectElement, int& nLine)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
	bool bFound = false;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
			if (pLibrary->GetName () == strLibrary)   {
				if (pLibrary->GetObjectElementLine (strObject, strObjectElement, nLine))   {
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
** GetObjectLine :
*/
bool CObjectsLibrary::GetObjectLine (CString strObject, int& nLine)
{
	bool bFound = false;
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pObject;
    if (m_listObjects.size())   {
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObject = *i;
			if (pObject->GetName () == strObject)   {
				nLine = pObject->GetLine ();
				bFound = true;
				break;
			}
			i++;
		}
	}
	return bFound;
}

/*
** GetObjectElementLine :
*/
bool CObjectsLibrary::GetObjectElementLine (CString strObject, CString strObjectElement, int& nLine)
{
	bool bFound = false;
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pObjects;
    if (m_listObjects.size())   {
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObjects = *i;
			if (pObjects->GetName () == strObject)   {
				if (pObjects->GetObjectElementLine (strObjectElement, nLine))   {
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
** GetObject :
*/
CAdamoObj* CObjectsContainer::GetObject (CString strObject)
{
    list<CObjectsLibrary*>::iterator i;
	CObjectsLibrary* pLibrary;
	CAdamoObj* pObj = NULL;
    if (m_listObjectLibraries.size())   {
        i=m_listObjectLibraries.begin();
        while (i!=m_listObjectLibraries.end())   {
			pLibrary = *i;
			if ((pObj = pLibrary->GetObject (strObject)) != NULL)   {
				break;
			}
			else
				i++;
		}
	}
	return pObj;
}

/*
** GetObject :
*/
CAdamoObj* CObjectsLibrary::GetObject (CString strObject)
{
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pObjects, *pObj = NULL;
    if (m_listObjects.size())   {
        i=m_listObjects.begin();
        while (i!=m_listObjects.end())   {
			pObjects = *i;
			if (pObjects->GetName () == strObject)   {
				pObj = pObjects;
				break;
			}
			else
				i++;
		}
	}
	return pObj;
}

/*
** operator = :
*/
CAdamoObjectData& CAdamoObjectData::operator = (CAdamoObjectData& p)
{
	m_ad = p.m_ad;
	SetName (p.GetName ());
	SetDescription (p.GetDescription ());
	SetPublic (p.GetPublic ());
	SetDataLine (p.GetDataLine ());
	return *this;
}

/*
** operator = :
*/
CAdamoObjectMethod& CAdamoObjectMethod::operator = (CAdamoObjectMethod& p)
{
	SetVirtual (p.GetVirtual ());
	return *this;
}

