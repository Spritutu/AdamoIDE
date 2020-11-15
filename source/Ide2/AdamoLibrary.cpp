/*
**
*/

#include "stdafx.h"
#include "adamolibrary.h"

int CAdamoLuaLibraryModule::AddObjCode (unsigned char *pObjCode, int nSize)
{
	m_pObjCode = new unsigned char [nSize];
	memcpy (m_pObjCode, pObjCode, nSize);
	m_nObjCodeSize = nSize;
	return 0;
}

int CAdamoLuaLibraryModule::AddFunction (CAdamoFunction* pAdamoFunction)
{
	int nB = -1;
	if (!ExistFunction (pAdamoFunction->GetName ()))   {
		CAdamoFunction* pNewFunction = new CAdamoFunction;
		*pNewFunction = *pAdamoFunction;
		m_listFunctions.push_back (pNewFunction);
		nB = 0;
	}
	return nB;
}

int CAdamoLuaLibraryModule::AddObject   (CAdamoObj* pAdamoObject)
{
	int nB = -1;
	if (!ExistObj (pAdamoObject->GetName ()))   {
		CAdamoObj* pNewObject = new CAdamoObj;
		*pNewObject = *pAdamoObject;
		m_listObjects.push_back (pNewObject);
		nB = 0;
	}
	return nB;
}

void CAdamoLuaLibraryModule::Clear ()
{
	_delete (m_pObjCode);
	ClearFunctions ();
	ClearObjects ();
}

CAdamoFunction* CAdamoLuaLibraryModule::ExistFunction (CString strName)
{
	bool bFound = false;
    list<CAdamoFunction*>::iterator i;
	CAdamoFunction* pFunction, *pFunctionRet = NULL;
    if (m_listFunctions.size())   {
        i = m_listFunctions.begin();
        while (i != m_listFunctions.end())   {
			pFunction = *i;
			if (pFunction->GetName () == strName)   {
				pFunctionRet = pFunction;
				break;
			}
			else
				i++;
		}
	}
	return pFunctionRet;
}

CAdamoObj* CAdamoLuaLibraryModule::ExistObj (CString strName)
{
	bool bFound = false;
    list<CAdamoObj*>::iterator i;
	CAdamoObj* pAdamoObj, *pAdamoObjRet = NULL;
    if (m_listObjects.size())   {
        i = m_listObjects.begin();
        while (i != m_listObjects.end())   {
			pAdamoObj = *i;
			if (pAdamoObj->GetName () == strName)   {
				pAdamoObjRet = pAdamoObj;
				break;
			}
			else
				i++;
		}
	}
	return pAdamoObjRet;
}

void CAdamoLuaLibraryModule::ClearFunctions ()
{
    list<CAdamoFunction*>::iterator i;
    
    i=m_listFunctions.begin();
    while (i!=m_listFunctions.end())   {
        delete *i;
        i=m_listFunctions.erase(i);
    }
}

void CAdamoLuaLibraryModule::ClearObjects ()
{
    list<CAdamoObj*>::iterator i;
    
    i=m_listObjects.begin();
    while (i!=m_listObjects.end())   {
        delete *i;
        i=m_listObjects.erase(i);
    }
}

int CAdamoLuaLibraryModule::ReadObjCode (CString strPath)
{
	CFile file;
	int nB = -1;
	if (!_access (strPath, 00))   {
		if (file.Open (strPath, CFile::modeRead | CFile::typeBinary))   {
			int nLenght = (int) file.GetLength ();
			unsigned char* p = new unsigned char [nLenght];
			file.Read (p, nLenght);
			file.Close ();
			AddObjCode (p, nLenght);
			delete p;
			nB = 0;
		}
	}
	return nB;
}

int CAdamoLuaLibraryModule::SerializeWrite (CFile* file)
{
	WriteName (file);
	WriteCode (file);
	WriteXml (file);
	return 0;
}

int CAdamoLuaLibraryModule::SerializeRead (CFile* file)
{
	ReadName (file);
	ReadCode (file);
	ReadXml (file);
	return 0;
}

int	CAdamoLuaLibraryModule::WriteName (CFile* file)
{
	int nNameLenght = m_strName.GetLength () + 1;
	char szBuffer[256];

	file->Write (&nNameLenght, sizeof (int));
	strcpy (szBuffer, m_strName);
	file->Write (szBuffer, nNameLenght);
	return 0;
}

int	CAdamoLuaLibraryModule::WriteCode (CFile* file)
{
	file->Write (&m_nObjCodeSize, sizeof (int));
	file->Write (m_pObjCode, m_nObjCodeSize);
	return 0;
}

int	CAdamoLuaLibraryModule::WriteXml (CFile* file)
{
	ULONGLONG nFilePointer = file->GetPosition (), nFilePointerGoOn;
	file->Seek (sizeof (int), CFile::current);
	m_nXmlLenght = 0;
	WriteXmlPrologue (file);
	WriteXmlFunctions (file);
	WriteXmlObjects (file);
	WriteXmlEpilogue (file);
	nFilePointerGoOn = file->GetPosition ();
	file->Seek (nFilePointer, CFile::begin);
	file->Write (&m_nXmlLenght, sizeof (int));
	file->Seek (nFilePointerGoOn, CFile::begin);
	return 0;
}

int	CAdamoLuaLibraryModule::WriteXmlPrologue (CFile* file)
{
	WriteString (file, "<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	WriteString (file, "<MODULE>");
	return 0;
}

int	CAdamoLuaLibraryModule::WriteXmlEpilogue (CFile* file)
{
	WriteString (file, "</MODULE>");
	return 0;
}

int	CAdamoLuaLibraryModule::WriteXmlFunctions (CFile* file)
{
    list<CAdamoLibrary*>::iterator l;
    list<CAdamoFunction*>::iterator f;
    list<CAdamoParameter*>::iterator p;
	CAdamoFunction* pFunction;
	CAdamoParameter* pParameter;
	int nLine;

	WriteString (file, "<FUNCTIONS>");
	f = m_listFunctions.begin ();
	while (f!=m_listFunctions.end())   {
		pFunction = *f;
		CString strName = pFunction->GetName ();
		CString strDescription = pFunction->GetDescription ();
		CString strData;
		nLine = pFunction->GetLine ();
		strData.Format ("<FUNCTION NAME=\"%s\" DESCRIPTION=\"%s\" LINE=\"%d\">", strName, strDescription, nLine);
		WriteString (file, strData);
		p = pFunction->GetListParameters ()->begin ();
		while (p!=pFunction->GetListParameters ()->end())   {
			pParameter= *p;
			strName = pParameter->m_strName;
			strDescription = pParameter->m_strDescription;
			strData.Format ("<PARAMETER NAME=\"%s\" DESCRIPTION=\"%s\"></PARAMETER>", strName, strDescription);
			WriteString (file, strData);
			p++;
		}
		WriteString (file, "</FUNCTION>");
		f++;
	}
	WriteString (file, "</FUNCTIONS>");
	return 0;
}

int	CAdamoLuaLibraryModule::WriteXmlObjects (CFile* file)
{
    list<CObjectsLibrary*>::iterator l;
    list<CAdamoObj*>::iterator o;
    list<CAdamoObjectElement*>::iterator e;
    list<CAdamoParameter*>::iterator p;
	CAdamoObj* pObj;
	CAdamoObjectElement* pObjectElement;
	CAdamoParameter* pParameter;
	int nLine;

	WriteString (file, "<OBJECTS>");
	o = m_listObjects.begin ();
	while (o!=m_listObjects.end())   {
		pObj = *o;
		CString strName = pObj->GetName ();
		CString strDescription = pObj->GetDescription ();
		nLine = pObj->GetLine ();
		CString strData; 
		strData.Format ("<OBJECT NAME=\"%s\" DESCRIPTION=\"%s\" LINE=\"%d\">", strName, strDescription, nLine);
		WriteString (file, strData);
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
					WriteString (file, strData);
					break;
				}
				case eMethod :   {
					bool bIsVirtual = ((CAdamoObjectMethod *)pObjectElement)->GetVirtual ();
					strData.Format ("<METHOD NAME=\"%s\" DESCRIPTION=\"%s\" PUBLIC=\"%d\" VIRTUAL=\"%d\">", strName, strDescription, bIsPublic, bIsVirtual, bIsVirtual);
					WriteString (file, strData);
					p = ((CAdamoObjectMethod *)pObjectElement)->GetListParameters ()->begin ();
					while (p!=((CAdamoObjectMethod *)pObjectElement)->GetListParameters ()->end())   {
						pParameter= *p;
						strName = pParameter->m_strName;
						strDescription = pParameter->m_strDescription;
						strData.Format ("<PARAMETER NAME=\"%s\" DESCRIPTION=\"%s\"></PARAMETER>", strName, strDescription);
						WriteString (file, strData);
						p++;
					}
					WriteString (file, "</METHOD>");
					break;
				}
			}
			e++;
		}
		WriteString (file, "</OBJECT>");
		o++;
    }
	WriteString (file, "</OBJECTS>");
	return 0;
}

int CAdamoLuaLibraryModule::WriteString (CFile* file, CString str)
{
	int nLenght = str.GetLength ();
	file->Write (str, nLenght);
	m_nXmlLenght += nLenght;
	return nLenght;
}

int	CAdamoLuaLibraryModule::ReadName (CFile* file)
{
	int nNameLenght;
	char szName[256];

	file->Read (&nNameLenght, sizeof (int));
	file->Read (szName, nNameLenght);
	m_strName = szName;
	return 0;
}

int CAdamoLuaLibraryModule::ReadCode (CFile* file)
{
	file->Read (&m_nObjCodeSize, sizeof (int));
	m_pObjCode = new unsigned char [m_nObjCodeSize];
	file->Read (m_pObjCode, m_nObjCodeSize);
	return 0;
}

int CAdamoLuaLibraryModule::ReadXml (CFile* file)
{
	file->Read (&m_nXmlLenght, sizeof (int));
	unsigned char* pXmlCode = new unsigned char [m_nXmlLenght + 1];
	file->Read (pXmlCode, m_nXmlLenght);
	pXmlCode[m_nXmlLenght] = '\0';
	CString strXml (pXmlCode);
    HRESULT hr = m_pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        m_pDocXml->async = false;
        if (m_pDocXml->loadXML(strXml.AllocSysString ()))   {
            ParseXML ();
            hr=S_OK;
        }
        m_pDocXml.Release ();
    }
	delete pXmlCode;
    return hr == S_OK ? 0 : -1;
}

int CAdamoLuaLibraryModule::ParseXML ()
{
    ElementXmlPtr  pEl=m_pDocXml->documentElement;
	NodeListXmlPtr  pRootChild;
    CString str=(LPCTSTR)pEl->nodeName;
	bool bError = false;
	
	if (str=="MODULE")   {
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
								m_listFunctions.push_back (function);
							}
							n++;
						}
					}
				}
				else
				if (str=="OBJECTS")   {
					pEl=pRootChild->Getitem(n);
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
								m_listObjects.push_back (obj);
							}
							n++;
						}
					}
				}
				n++;
			}
		}
	}
	return 0;
}

int CAdamoLuaLibrary::AddLibraryModule (CString strName, CAdamoLuaLibraryModule* pLuaModule)
{
	int i, nB = 0;
	if (m_nNumModules < NMAXLUALIBMODULES)   {
		for (i = 0; i<m_nNumModules; i++)   {
			CAdamoLuaLibraryModule* pModule = m_arrayLibraryModules[i];
			if (pModule->GetName () == strName)   {
				nB = -1; break;
			}
		}
		if (nB == 0)   {
			m_arrayLibraryModules.at (m_nNumModules) = pLuaModule;
			m_nNumModules++;
		}
	}
	else
		nB = -2;
	return nB;
}

int CAdamoLuaLibrary::RemoveLibraryModule (CString strName)
{
	int i, nB = 0;
	for (i = 0; i<m_nNumModules ; i++)   {
		CAdamoLuaLibraryModule* pModule = m_arrayLibraryModules[i];
		if (pModule->GetName () == strName)   {
			nB = -1; break;
		}
	}
	if (nB == -1)   {
		delete m_arrayLibraryModules[i];
		for (int n = i; n<NMAXLUALIBMODULES-1; n++)
			m_arrayLibraryModules[n] = m_arrayLibraryModules[n + 1];
		m_arrayLibraryModules.at(NMAXLUALIBMODULES - 1) = NULL;
		m_nNumModules--;
	}
	return 0;
}

int CAdamoLuaLibrary::Load (CString strPath)
{
	CFile file;
	int nB = -1;
	if (!_access (strPath, 00))   {
		if (file.Open (strPath, CFile::modeRead | CFile::typeBinary))   {
			if (!ReadHeader (&file))   {
				ReadVersion (&file);
				ReadDescription (&file);
				ReadModules (&file);
				nB = 0;
			}
			file.Close ();
		}
	}
	return nB;
}

int CAdamoLuaLibrary::ReadHeader (CFile* file)
{
	int nB = 0;
	char szBuffer[256];

	file->Read (szBuffer, strlen (HEADER_LUA_FILES) + 1);
	if (strcmp (szBuffer, HEADER_LUA_FILES))
		nB = -1;
	return nB;
}

int CAdamoLuaLibrary::ReadVersion (CFile* file)
{
	file->Read (&m_nVersion, sizeof (int));
	return 0;
}

int CAdamoLuaLibrary::ReadDescription (CFile* file)
{
	int nDescLenght;
	char szDesc[256];

	file->Read (&nDescLenght, sizeof (int));
	file->Read (szDesc, nDescLenght);
	m_strDescription = szDesc;
	return 0;
}

int CAdamoLuaLibrary::ReadModules (CFile* file)
{
	file->Read (&m_nNumModules, sizeof (int));
	for (int i = 0; i<m_nNumModules; i++)   {
		CAdamoLuaLibraryModule* pModule = new CAdamoLuaLibraryModule;
		pModule->SerializeRead (file);
		m_arrayLibraryModules.at (i) = pModule;
	}
	return 0;
}

int CAdamoLuaLibrary::Save (CString strPath)
{
	CFile file;

	if (_access (strPath, 00))   {
		if (file.Open (strPath, CFile::modeCreate | CFile::typeBinary))
			file.Close ();
		else
			return -1;
	}
	if (!_access (strPath, 00))   {
		if (file.Open (strPath, CFile::modeWrite | CFile::typeBinary))   {
			WriteHeader (&file);
			WriteVersion (&file);
			WriteDescription (&file);
			WriteModules (&file);
			file.Close ();
		}
	}
	return 0;
}

int CAdamoLuaLibrary::WriteHeader (CFile* file)
{
	file->Write (HEADER_LUA_FILES, strlen (HEADER_LUA_FILES) + 1);
	return 0;
}

int CAdamoLuaLibrary::WriteVersion (CFile* file)
{
	m_nVersion = VERSION_LUA_FILES;
	file->Write (&m_nVersion, sizeof (int));
	return 0;
}

int CAdamoLuaLibrary::WriteDescription (CFile* file)
{
	int nDescLenght;
	char szDesc[256];

	nDescLenght = m_strDescription.GetLength () + 1;
	file->Write (&nDescLenght, sizeof (int));
	strcpy (szDesc, m_strDescription);
	file->Write (szDesc, nDescLenght);
	return 0;
}

int CAdamoLuaLibrary::WriteModules (CFile* file)
{
	file->Write (&m_nNumModules, sizeof (int));
	for (int i = 0; i<m_nNumModules; i++)   {
		CAdamoLuaLibraryModule* pModule = m_arrayLibraryModules[i];
		pModule->SerializeWrite (file);
	}
	return 0;
}

int CAdamoLuaLibrary::IsLuaLibraryFile (CString strPath, int& nVersion, CString& strDescription)
{
	CFile file;
	int nDescLenght, nB = 0;
	char szBuffer[256];

	if (!_access (strPath, 00))   {
		if (file.Open (strPath, CFile::modeRead | CFile::typeBinary))   {
			file.Read (szBuffer, strlen (HEADER_LUA_FILES) + 1);
			if (!strcmp (szBuffer, HEADER_LUA_FILES))   {
				file.Read (&nVersion, sizeof (int));
				file.Read (&nDescLenght, sizeof (int));
				file.Read (szBuffer, nDescLenght);
				strDescription = szBuffer;
				nB = 1;
			}
			file.Close ();
		}
	}
	return nB;
}

CAdamoLuaLibraryModule* CAdamoLuaLibrary::ExistLibraryModule (CString strModuleName)
{
	CAdamoLuaLibraryModule* pRetModule = NULL;

	for (int i = 0; i<m_nNumModules; i++)   {
		CAdamoLuaLibraryModule* pModule = m_arrayLibraryModules[i];
		if (pModule->GetName () == strModuleName)   {
			pRetModule = pModule; break;
		}
	}
	return pRetModule;
}

int CAdamoLuaLibrary::GetLibraryModules (CStringList& listModules)
{
	for (int i = 0; i<m_nNumModules; i++)   {
		CAdamoLuaLibraryModule* pModule = m_arrayLibraryModules[i];
		listModules.AddTail (pModule->GetName ());
	}
	return m_nNumModules;
}

void CAdamoLuaLibrary::MoveModuleUp (CString strModuleName)
{
	for (int i = 0; i<m_nNumModules; i++)    {
		if (m_arrayLibraryModules[i]->GetName () == strModuleName)   {
			if (i > 0)   {
				CAdamoLuaLibraryModule* p = m_arrayLibraryModules[i - 1];
				m_arrayLibraryModules[i - 1] = m_arrayLibraryModules[i];
				m_arrayLibraryModules[i] = p;
			}
			break;
		}
	}
}

void CAdamoLuaLibrary::MoveModuleDown (CString strModuleName)
{
	for (int i = 0; i<m_nNumModules; i++)    {
		if (m_arrayLibraryModules[i]->GetName () == strModuleName)   {
			if (i < m_nNumModules - 1)   {
				CAdamoLuaLibraryModule* p = m_arrayLibraryModules[i + 1];
				m_arrayLibraryModules[i + 1] = m_arrayLibraryModules[i];
				m_arrayLibraryModules[i] = p;
			}
			break;
		}
	}
}

