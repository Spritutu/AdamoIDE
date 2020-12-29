/*
**
*/

/* include files */
#include <stdafx.h>
#include <workspace.h>
#include <CBrowserContainer.h>
#include <DlgSelSymbol.h>

/* functions prototypes */
static int SortBrowseData (const void* p1, const void* p2);

/** construction / destruction **/
CBrowserContainer::CBrowserContainer () : m_pBrowseData (NULL), m_nReferenceCount (0), m_nReferenceIndex (0)
{
}

CBrowserContainer::~CBrowserContainer ()
{
    ClearAll ();
}

int CBrowserContainer::Add (int nType, int nReference, CString strSymbol, CString strFunction, CString strObject, CString strModule, int nLine, int nItem)
{
	CString strKey;
    struct stBrowserDescriptor *p;
    int nB=-1;
    p=new stBrowserDescriptor;
    p->nBrowserType=nType;
    p->nBrowserReference=nReference;
    if (strSymbol.GetLength ()<=NMAXIDENTIFIER)   {
        strcpy (p->szSymbol, strSymbol);
        if (strModule.GetLength ()<=_MAX_PATH)   {
            strcpy (p->szModule, strModule);
            strcpy (p->szFunction, strFunction);
			strcpy (p->szObject, strObject);
			p->nModule = GetModuleNumber (strModule);
            p->nLine = nLine;
			p->nItem = nItem;
			if (strFunction != "")
				strKey.Format ("%s_%d_%s", strSymbol, nType, strFunction);
			else
				strKey.Format ("%s_%d", strSymbol, nType);
			if (nReference == BROWSER_DEFINITION)
				m_mapDefinitions.insert (map<string, stBrowserDescriptor*>::value_type(string (strKey), p));
			else
				m_mapReferences.insert (map<string, stBrowserDescriptor*>::value_type(string (strKey), p));
            nB=0;
        }
    }
    return nB;
}

void CBrowserContainer::ClearAll ()
{
    map<string, stBrowserDescriptor*>::iterator it;
    multimap<string, stBrowserDescriptor*>::iterator itm;
    struct stBrowserDescriptor* p;

    for (it=m_mapDefinitions.begin(); it!=m_mapDefinitions.end(); it++)   {
        p=(*it).second;
        delete p;
    }
    m_mapDefinitions.clear();
    for (itm=m_mapReferences.begin(); itm!=m_mapReferences.end(); itm++)   {
        p=(*itm).second;
        delete p;
    }
    m_mapReferences.clear();
}

int CBrowserContainer::Load ()
{
	CString strKey;
    int nB=-1;
    CFile fBrowser;
    stBrowserDescriptor bd;
    CString fn=CString (m_szDir) + "\\" + BROWSER_FILE;
    if (!_access (fn, 0))   {
        if (fBrowser.Open(fn, CFile::modeRead|CFile::typeBinary))   {
            while (fBrowser.Read (&bd, sizeof (stBrowserDescriptor))==sizeof (stBrowserDescriptor))   {
                stBrowserDescriptor *p = new stBrowserDescriptor;
                *p=bd;
				if (p->nBrowserReference == BROWSER_DEFINITION)   {
					if (CString (p->szFunction) != "")
						strKey.Format ("%s_%d_%s", p->szSymbol, p->nBrowserType, p->szFunction);
					else
						strKey.Format ("%s_%d", p->szSymbol, p->nBrowserType);
					m_mapDefinitions.insert (map<string, stBrowserDescriptor*>::value_type(string (strKey), p));
				}
				else
					if (p->nBrowserReference == BROWSER_REFERENCE)   {
						if (CString (p->szFunction) != "")
							strKey.Format ("%s_%d_%s", p->szSymbol, p->nBrowserType, p->szFunction);
						else
							strKey.Format ("%s_%d", p->szSymbol, p->nBrowserType);
						m_mapReferences.insert (map<string, stBrowserDescriptor*>::value_type(string (strKey), p));
					}
            }
            fBrowser.Close ();
            nB=0;
        }
    }
    else   {
        fBrowser.Open(fn, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
        fBrowser.Close ();
    }
    return nB;
}

int CBrowserContainer::Save ()
{
    CFile fBrowser;
	map<string, stBrowserDescriptor*>::iterator it;
    multimap<string, stBrowserDescriptor*>::iterator itm;
    struct stBrowserDescriptor* p;
    int nB=-1;

    CString fn=CString (m_szDir) + "\\" + BROWSER_FILE;
    if (fBrowser.Open(fn, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))   {
		for (it=m_mapDefinitions.begin(); it!=m_mapDefinitions.end(); it++)   {
			p=(*it).second;
            fBrowser.Write (p, sizeof (stBrowserDescriptor));
        }
		for (itm=m_mapReferences.begin(); itm!=m_mapReferences.end(); itm++)   {
			p=(*itm).second;
            fBrowser.Write (p, sizeof (stBrowserDescriptor));
        }
        fBrowser.Close ();
        nB=0;
    }
    return nB;
}

void CBrowserContainer::RemoveModule (const char* szModule)
{
	map<string, stBrowserDescriptor*>::iterator it;
	multimap<string, stBrowserDescriptor*>::iterator itm;
    struct stBrowserDescriptor* p;

	it=m_mapDefinitions.begin();
    while (it!=m_mapDefinitions.end())   {
		p=(*it).second;
        if (!strcmp (p->szModule, szModule))   {
            delete p;
            it = m_mapDefinitions.erase (it);
		}
		else
			it++;
	}
	itm=m_mapReferences.begin();
    while (itm!=m_mapReferences.end())   {
		p=(*itm).second;
        if (!strcmp (p->szModule, szModule))   {
            delete p;
            itm = m_mapReferences.erase (itm);
		}
		else
			itm++;
	}
}

void CBrowserContainer::GetLuaFunctions (CPtrList& listLua)
{
	map<string, stBrowserDescriptor*>::iterator it;
    struct stBrowserDescriptor* p;

	for (it=m_mapDefinitions.begin(); it!=m_mapDefinitions.end(); it++)   {
		p=(*it).second;
		if (p->nBrowserType == BROWSER_TYPE_FUNCTION)
			if (strcmp (p->szSymbol, "main"))
				listLua.AddTail (p);
	}
}

void CBrowserContainer::UpdateLineNumbers (CString strModule, int nStartLine, int nUpdateLines, int nDeleteLine)
{
	map<string, stBrowserDescriptor*>::iterator it;
	multimap<string, stBrowserDescriptor*>::iterator itm;
    struct stBrowserDescriptor* p;

	/* cominciamo ad aggiornare le definizioni */
	it=m_mapDefinitions.begin();
	if (nUpdateLines < 0)  {
		while (it!=m_mapDefinitions.end())   {
			p=(*it).second;
			if (!strcmp (p->szModule, strModule))   {
				if (p->nLine >= nStartLine && p->nLine < nStartLine + abs (nUpdateLines))   {
					delete p;
					it = m_mapDefinitions.erase (it);
				}
				else
					it++;
			}
			else
				it++;
		}
	}
	it=m_mapDefinitions.begin();
	while (it!=m_mapDefinitions.end())   {
		p=(*it).second;
		if (!strcmp (p->szModule, strModule))   {
			if (p->nLine > nStartLine)
				p->nLine += nUpdateLines;
		}
		it++;
	}
	/* ora i reference */
	if (nUpdateLines < 0)  {
		itm=m_mapReferences.begin();
		while (itm!=m_mapReferences.end())   {
			p=(*itm).second;
			if (!strcmp (p->szModule, strModule))   {
				if (p->nLine >= nStartLine && p->nLine < nStartLine + abs (nUpdateLines))   {
					delete p;
					itm = m_mapReferences.erase (itm);
				}
				else
					itm++;
			}
			else
				itm++;
		}
	}
	itm=m_mapReferences.begin();
	while (itm!=m_mapReferences.end())   {
		p=(*itm).second;
		if (!strcmp (p->szModule, strModule))   {
			if (p->nLine > nStartLine)
				p->nLine += nUpdateLines;
		}
		itm++;
	}
	/* ora i reference correnti */
	for (int i = 0; i<m_nReferenceCount; i++)   {
		p = &m_pBrowseData[i];
        if (!strcmp (p->szModule, strModule))   {
			if (p->nLine >= nStartLine && p->nLine < nStartLine + abs (nUpdateLines))   {
				if (i < m_nReferenceCount)   {
					memmove (p, p+1, sizeof (stBrowserDescriptor) * (m_nReferenceCount - (i+1)));
					i = -1;
					m_nReferenceCount--;
				}
			}
		}
	}
	for (int i = 0; i<m_nReferenceCount; i++)   {
		p = &m_pBrowseData[i];
        if (!strcmp (p->szModule, strModule))   {
			if (p->nLine > nStartLine)
				p->nLine += nUpdateLines;
		}
	}
}

bool CBrowserContainer::GetDefinitionSymbol(CString strSymbol, CString strFunction, CString strVariable, int& nSymbolType, CString& strModule, int& nLine)
{
	map<string, stBrowserDescriptor*>::iterator it;
	CString str;
	int nB = -1, nItemSearch;
	bool bFound = false;

	switch (nSymbolType) {
	case  BROWSER_TYPE_OBJECT :
			/* object */
			str.Format("%s_%d", strSymbol, BROWSER_TYPE_OBJECT);
			it = m_mapDefinitions.find(string(str));
			if (it == m_mapDefinitions.end())
				nSymbolType = BROWSER_TYPE_NOTHING;
			else   {
				/* trovato, copiamo modulo e linea */
				stBrowserDescriptor* p = (*it).second;
				strModule = p->szModule;
				nLine = p->nLine;
				bFound = true;
			}
			break;
		case BROWSER_TYPE_METHOD: 
			{
				stBrowserDescriptor* p;
				m_pBrowseData = new stBrowserDescriptor[1024];
				nItemSearch = 0;
				str.Format("%s_%d", strSymbol, BROWSER_TYPE_METHOD);
				pair <multimap<string, stBrowserDescriptor*>::iterator, multimap<string, stBrowserDescriptor*>::iterator> keyRange = m_mapDefinitions.equal_range(string(str));
				for (multimap<string, stBrowserDescriptor*>::iterator it = keyRange.first; it != keyRange.second && nItemSearch < 1024; ++it) {
					m_pBrowseData[nItemSearch] = *it->second;
					nItemSearch++;
				}
				if (nItemSearch > 0) {
					p = GetMultipleDefinitions(m_pBrowseData, nItemSearch);
					if (p != NULL) {
						strModule = p->szModule;
						nLine = p->nLine;
						bFound = true;
					}
				}
				_delete(m_pBrowseData);
			}
			break;
		default:
			/* proviamo prima a prendere le define */
			str.Format("%s_%d", strSymbol, BROWSER_TYPE_DEFINE);
			it = m_mapDefinitions.find(string(str));
			if (it == m_mapDefinitions.end()) {
				/* poi variabile locale */
				str.Format("%s_%d_%s", strSymbol, BROWSER_TYPE_LOCAL, strFunction);
				it = m_mapDefinitions.find(string(str));
				if (it == m_mapDefinitions.end()) {
					/* campo di table */
					str.Format("%s_%d_%s", strSymbol, BROWSER_TYPE_TABLE_FIELD, strVariable);
					it = m_mapDefinitions.find(string(str));
					if (it == m_mapDefinitions.end()) {
						/* variabile globale */
						str.Format("%s_%d", strSymbol, BROWSER_TYPE_GLOBAL);
						it = m_mapDefinitions.find(string(str));
						if (it == m_mapDefinitions.end()) {
							/* function */
							str.Format("%s_%d", strSymbol, BROWSER_TYPE_FUNCTION);
							it = m_mapDefinitions.find(string(str));
							if (it == m_mapDefinitions.end())
								nSymbolType = BROWSER_TYPE_NOTHING;
							else
								nSymbolType = BROWSER_TYPE_FUNCTION;
						}
						else
							nSymbolType = BROWSER_TYPE_GLOBAL;
					}
					else
						nSymbolType = BROWSER_TYPE_TABLE_FIELD;
				}
				else
					nSymbolType = BROWSER_TYPE_LOCAL;
			}
			else
				nSymbolType = BROWSER_TYPE_DEFINE;
			if (it!=m_mapDefinitions.end ())   {
				/* trovato, copiamo modulo e linea */
				stBrowserDescriptor* p = (*it).second;
				strModule = p->szModule;
				nLine = p->nLine;
				bFound = true;
			}
	}
	return bFound;
}

/*
** BeginGetReferenceSymbol :
*/
void CBrowserContainer::BeginGetReferenceSymbol (CString strSymbol, int nSymbolType, CString strFunction, CString strVariable)
{
	multimap<string, stBrowserDescriptor*>::iterator it;
	CString strSearch;
	int n = 0;

	/* terminiamo la sessione precedente */
	EndGetReferenceSymbol ();
	/* creiamo il buffer */
	m_pBrowseData = new stBrowserDescriptor[1024];
	/* creiamo il simbolo da cercare */
	switch (nSymbolType)   {
		case BROWSER_TYPE_LOCAL :
			strSearch.Format ("%s_%d_%s", strSymbol, BROWSER_TYPE_SYMBOL, strFunction);
			break;
		case BROWSER_TYPE_TABLE_FIELD :
			strSearch.Format ("%s_%d_%s", strSymbol, BROWSER_TYPE_SYMBOL, strVariable);
			break;
		default :
			strSearch.Format ("%s_%d", strSymbol, BROWSER_TYPE_SYMBOL);
	}
	/* cerchiamo tutti i simboli e mettiamolo nel buffer */
    pair <multimap<string, stBrowserDescriptor*>::iterator, multimap<string, stBrowserDescriptor*>::iterator> keyRange = m_mapReferences.equal_range (string (strSearch));
    for (multimap<string, stBrowserDescriptor*>::iterator it = keyRange.first; it != keyRange.second; ++it)   {
		if (nSymbolType == BROWSER_TYPE_LOCAL)
			if (CString (it->second->szFunction) == strFunction)
				m_pBrowseData[n] = *it->second;
			else
				;
		else
			m_pBrowseData[n] = *it->second;
		n++;
	}
	/* azzeriamo gli indici */
	m_nReferenceCount = n;
	m_nReferenceIndex = -1;
	/* ordiniamolo */
	qsort (m_pBrowseData, n, sizeof (stBrowserDescriptor), SortBrowseData);
}

/*
** EndGetReferenceSymbol :
*/
void CBrowserContainer::EndGetReferenceSymbol ()
{
	/* cancelliamo l'array dei dati browser */
	_delete (m_pBrowseData);
}

/*
** GetModuleNumber :
*/
int CBrowserContainer::GetModuleNumber (CString strModule)
{
	int nModule = -1;
    CProject* pPrj = GETWS()->GetActiveProject();
	pPrj->GetProjectFile (strModule, &nModule);
	return nModule;
}

/*
** SortBrowseData :
*/
int SortBrowseData (const void* p1, const void* p2)
{
	stBrowserDescriptor* pBD1 = (stBrowserDescriptor*) p1;
	stBrowserDescriptor* pBD2 = (stBrowserDescriptor*) p2;
	int nB = 0;

	if (pBD1->nModule < pBD2->nModule)
		nB = -1;
	else
	if (pBD1->nModule > pBD2->nModule)
		nB = 1;
	else
		if (pBD1->nLine < pBD2->nLine)
			nB = -1;
		else
		if (pBD1->nLine > pBD2->nLine)
			nB = 1;
		else
			if (pBD1->nItem < pBD2->nItem)
				nB = -1;
			else
			if (pBD1->nItem > pBD2->nItem)
				nB = 1;
	return nB;
}
	
/*
** GetNextReferenceSymbol :
*/
int CBrowserContainer::GetNextReferenceSymbol (CString& strModule, int& nLine, CString& strSymbol, int& nItem)
{
	int nB = -1;

	if (m_nReferenceCount > 0)   {
		if (m_nReferenceIndex == -1)
			m_nReferenceIndex = m_nReferenceCount - 1;
		m_nReferenceIndex = m_nReferenceIndex == m_nReferenceCount - 1 ? 0 : m_nReferenceIndex + 1;
		strModule = CString (m_pBrowseData[m_nReferenceIndex].szModule);
		nLine = m_pBrowseData[m_nReferenceIndex].nLine;
		nItem = m_pBrowseData[m_nReferenceIndex].nItem;
		strSymbol = CString (m_pBrowseData[m_nReferenceIndex].szSymbol);
		nB = 0;
	}
	return nB;
}

/*
** GetPrevReferenceSymbol :
*/
int CBrowserContainer::GetPrevReferenceSymbol (CString& strModule, int& nLine, CString& strSymbol, int& nItem)
{
	int nB = -1;

	if (m_nReferenceCount > 0)   {
		if (m_nReferenceIndex == -1)
			m_nReferenceIndex = 0;
		m_nReferenceIndex = m_nReferenceIndex == 0 ? m_nReferenceCount - 1 : m_nReferenceIndex - 1;
		strModule = CString (m_pBrowseData[m_nReferenceIndex].szModule);
		nLine = m_pBrowseData[m_nReferenceIndex].nLine;
		nItem = m_pBrowseData[m_nReferenceIndex].nItem;
		strSymbol = CString (m_pBrowseData[m_nReferenceIndex].szSymbol);
		nB = 0;
	}
	return nB;
}

/*
** GetMultipleDefinitions :
*/
stBrowserDescriptor* CBrowserContainer::GetMultipleDefinitions (stBrowserDescriptor* pBrowseData, int nItemSearch)
{
	CDlgSelSymbol SelSymbol;
	stBrowserDescriptor* p = NULL;

	SelSymbol.SetSymbols(pBrowseData, nItemSearch);
	if (SelSymbol.DoModal() == IDOK)
		p = SelSymbol.GetSelectedBrowserDescriptor();
	return p;
}
