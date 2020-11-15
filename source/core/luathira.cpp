/*
** $Id: luathira.c,v 1.44 2003/04/07 20:34:20 lhf Exp $
** Lua / Thira Interface 
*/

/* include files */
#include <windows.h>
#include <winreg.h>
#include "luathira.h"
#include "lstate.h"
#include <map>
#include <string>

/* namespace */
using namespace std;

/* global variables */
static map <string, stGlobalDescriptor*> m_mapGV;
static map <string, stFunctionDescriptor*> m_mapFN;
static map <string, stBrowserDescriptor*> m_mapLF;
static bool bInStrictSyntax;

/*
** lua_thira_load_global_vars : carica il file delle variabili globali.
*/
int lua_thira_load_global_vars (const char* szPath)
{
	FILE *f;
    recGlobals rc;
	stGlobalDescriptor* p;
	int i, nB = -1;
	char szFile[_MAX_PATH], szBuffer[NMAXIDENTIFIER+1];
	
	if (szPath)   {
		/* creaimo il path dove si trova il file delle variabili globali */
		strcpy (szFile, szPath);
		strcat (szFile, "\\");
		strcat (szFile, GLOBALS_DAT);
		if (f = fopen (szFile, "rb"))   {
			while (fread (&rc, 1, sizeof (recGlobals), f) == sizeof (recGlobals))   {
				p = (stGlobalDescriptor *) malloc (sizeof (stGlobalDescriptor));
				GetGlobalsFromRecord (&rc, p);
				for (i=0; i<rc.mat.nCols; i++)   {
					fread (szBuffer, sizeof (szBuffer), 1, f); 
					p->mat.strColumnName[i] = (char *) malloc (strlen (szBuffer)+1);
					strcpy (p->mat.strColumnName[i], szBuffer);
				}
				AddGlobalVariable (p);
			}
			fclose (f);
			nB = 0;
		}
		bInStrictSyntax = true;
	}
	else
		bInStrictSyntax = false;
	return nB;
}

/*
** lua_thira_load_functions : carica il file delle funzioni.
*/
int lua_thira_load_functions ()
{
	FILE *f;
	int nArgs, nOptional, nFirstChar;
	char szBuffer[LUAL_BUFFERSIZE], szFile[LUAL_BUFFERSIZE], *p, *q, *r;

	/* leggiamo dal registry dove andare a prendere i file di configurazione .api */
	if (GetFunctionsPath (szBuffer))   {
		/* apriamo il file */
		sprintf (szFile, "%s\\%s", szBuffer, FUNCTION_API);
		if (f = fopen (szFile, "rt"))   {
			while (fgets (szBuffer, LUAL_BUFFERSIZE, f))   {
				stFunctionDescriptor *pFN = (stFunctionDescriptor *) malloc (sizeof (stFunctionDescriptor));
				p = strchr (szBuffer, '(');
				if (p)   {
					*p = '\0'; q = szBuffer; r = p - 1;
					/* togliamo blank e tabulazioni */
					while (*q == ' ' || *q == '\t')
						q++;
					while (*r == ' ' || *r == '\t')   {
						*r = '\0';
						r--;
					}
					/* nome funzione */
					strcpy (pFN->strName, q);
					p++;
					/* argomenti */
					if (strstr (p, "..."))   {
						pFN->dwArgs = 0xffffffff;
						pFN->bZeroArgs = 0;
					}
					else   {
						nArgs = nOptional = nFirstChar = 0;
						while (*p != ')')   {
							if (*p == ',')
								nArgs++;
							else
							if (*p == '[')
								nOptional++;
							else
							if (!nFirstChar)   {
								if (*p != ' ' && *p != '\t')   {
									nArgs++;
									nFirstChar = 1;
								}
							}
							p++;
						}
						pFN->bZeroArgs = nArgs == nOptional ? 1 : 0;
						pFN->dwArgs = 0;
						if (nArgs)  {
							pFN->dwArgs = 1<<(nArgs - 1);
							while (nOptional)   {
								int nBit = nArgs - 1 - nOptional;
								if (nBit >= 0)
									pFN->dwArgs |= 1<<(nArgs - 1 - nOptional);
								nOptional--;
							}
						}
					}
					AddFunction (pFN);
				}
			}
			fclose (f);
		}
	}
	return 0;
}

/*
** lua_thira_load_lua_functions : carica il file delle function LUA.
*/
int lua_thira_load_lua_functions (const char* szPath)
{
	FILE *f;
    stBrowserDescriptor bd, *p;
	int nB = -1;
	char szFile[_MAX_PATH];
	
	if (szPath)   {
		/* creaimo il path dove si trova il file delle variabili globali */
		strcpy (szFile, szPath);
		strcat (szFile, "\\");
		strcat (szFile, BROWSER_DAT);
		if (f = fopen (szFile, "rb"))   {
			while (fread (&bd, 1, sizeof (stBrowserDescriptor), f) == sizeof (stBrowserDescriptor))   {
				if (bd.nBrowserType == BROWSER_TYPE_FUNCTION)   {
					p = (stBrowserDescriptor *) malloc (sizeof (stBrowserDescriptor));
					*p=bd;
					AddLuaFunction (p);
				}
			}
			fclose (f);
			nB = 0;
		}
	}
	return nB;
}

/*
** lua_thira_unload_global_vars : libera la memoria riservata.
*/
int lua_thira_unload_global_vars ()
{
	ClearMapGlobalVariables ();
	return 0;
}

/*
** lua_thira_unload_functions : libera la memoria riservata.
*/
int lua_thira_unload_functions ()
{
	ClearMapFunctions ();
	return 0;
}

/*
** lua_thira_unload_lua_functions : libera la memoria riservata.
*/
int lua_thira_unload_lua_functions ()
{
	ClearMapLuaFunctions ();
	return 0;
}

/*
** lua_thira_search_global_var : cerca la variabile globale indicata.
*/
stGlobalDescriptor* lua_thira_search_global_var (const char* strGV)
{
    stGlobalDescriptor* pGV=NULL;
    map<string, stGlobalDescriptor*>::iterator it;

	if (bInStrictSyntax)   {
		it=m_mapGV.find (strGV);
		if (it!=m_mapGV.end())   {
			pGV=(*it).second;
		}
	}
	else
		pGV = (stGlobalDescriptor*)1;
    return pGV;
}

/*
** lua_thira_check_global_var :
*/
int lua_thira_check_global_var (LexState *ls, const char *szvar, int nbrackets, expdesc *row, expdesc *col, int nfield, expdesc *field, int bIsLocal)
{
	stGlobalDescriptor* pD;
	int n, nB = 0;
	bool bFound;

	if (bInStrictSyntax)   {
		/* eseguiamo il controllo di sintassi solo se siamo dentro una function */
		if (ls->fs->prev)   {
			/* verifichiamo se c'e' la variabile globale, se non c'e' non facciamo niente, potrebbe essere una table locale */
			if (pD = lua_thira_search_global_var (szvar))   {
				if (pD->nVarType != LUA_TTABLE && pD->nVarType != LUA_TFUNCTION)   {
					/* procediamo a testare se non e' una variabile globale di tipo table o function */
					if (nbrackets == 1 && nfield == 0)   {
						/* deve essere un array */
						if (pD->nType != GLOBALARRAY)
							nB = -1;
						else   {
							/* se e' una costante, l'elemento dell'array deve essere entro i limiti dichiarati */
							if (row->k == VK)   {
								int nRow = (int) ls->fs->f->k[row->info].value.n;
								if (nRow < 0 || nRow >= pD->arr.nArrElements)
									nB = -2;
							}
						}
					}
					else
					if (nbrackets == 1 && nfield == 1)   {
						/* e' una matrice, verifichiamo il campo riga e colonna */
						if (pD->nType != GLOBALMATRIX)
							nB = -1;
						else   {
							if (row->k == VK)   {
								int nRow = (int) ls->fs->f->k[row->info].value.n;
								if (nRow < 0 || nRow >= pD->mat.nRows)
									nB = -2;
							}
							if (!nB)   {
								/* se non c'e' stato nessun errore */
								if (field->k == VK)   {
									char strfield[LUA_MINBUFFER];
									TString *t = (TString *) ls->fs->f->k[field->info].value.gc;
									strcpy (strfield, getstr (t));
									/* andiamo a cercare se il campo di struttura indicato e' stato dichiarato */
									n = 0; bFound = false;
									while (n < pD->mat.nCols)   {
										if (!strcmp (pD->mat.strColumnName[n], strfield))   {
											bFound = true;
											break;
										}
										n++;
									}
									if (!bFound)
										nB = -3;
								}
								else
									nB = -5;
							}
						}
					}
					else
					if (nbrackets == 2 && nfield == 0)   {
						/* e' una matrice, verifichiamo il campo riga e colonna */
						if (pD->nType != GLOBALMATRIX)
							nB = -1;
						else   {
							if (row->k == VK)   {
								int nRow = (int) ls->fs->f->k[row->info].value.n;
								if (nRow < 0 || nRow >= pD->mat.nRows)
									nB = -2;
							}
							if (!nB)   {
								/* se non c'e' stato nessun errore */
								if (col->k == VK)   {
									char strfield[LUA_MINBUFFER];
									if (ls->fs->f->k[col->info].value.gc->gch.tt == LUA_TSTRING)   {
										TString *t = (TString *) ls->fs->f->k[col->info].value.gc;
										strcpy (strfield, getstr (t));
										/* andiamo a cercare se il campo di struttura indicato e' stato dichiarato */
										n = 0; bFound = false;
										while (n < pD->mat.nCols)   {
											if (!strcmp (pD->mat.strColumnName[n], strfield))   {
												bFound = true;
												break;
											}
											n++;
										}
										if (!bFound)
											nB = -3;
									}
									else
										nB = -4;
								}
							}
						}
					}
				}
			}
			else
				if (!bIsLocal)
					nB = -6;
		}
	}
	return nB;
}

/*
** lua_thira_search_C_function :
*/
stFunctionDescriptor* lua_thira_search_C_function (const char *strFN)
{
    stFunctionDescriptor* pFN=NULL;
    map<string, stFunctionDescriptor*>::iterator it;

	if (bInStrictSyntax)   {
		it=m_mapFN.find (strFN);
		if (it!=m_mapFN.end())   {
			pFN=(*it).second;
		}
	}
	else
		pFN = (stFunctionDescriptor*)1;
    return pFN;
}

/*
** lua_thira_check_C_functions :
*/
int lua_thira_check_C_functions (LexState *ls, const char *szvar, int nfield, expdesc *field, int nArgs, int bIsLocal, char *szres)
{
	char szfield[LUA_MINBUFFER], szkey[LUAL_BUFFERSIZE];
    stFunctionDescriptor* pFN=NULL;
	stGlobalDescriptor* pD = NULL;
	int nB = 0;
	bool bFound = false;

	if (bInStrictSyntax)   {
		/* sono in controllo di sintassi */
		if (!bIsLocal)   {
			/* non e' una variabile locale */
			if (field->k == VK)   {
				/* verifichiamo che non sia una variabile globale di tipo table, consentiamolo */
				if (pD = lua_thira_search_global_var (szvar))   {
					if (pD->nVarType == LUA_TTABLE || pD->nVarType == LUA_TFUNCTION)   {
						bFound = true;
					}
				}
				/* se non e' una variabile globale, allora deve essere una function nostra, verifichiamolo */
				if (!bFound)   {
					/* il campo deve essere una costante, recuperiamola */
					TString *t = (TString *) ls->fs->f->k[field->info].value.gc;
					strcpy (szfield, getstr (t));
					sprintf (szkey, "%s.%s", szvar, szfield);
					if (pFN = lua_thira_search_C_function (szkey))   {
						/* function trovata */
						if (nArgs)   {
							unsigned bArgs = 1<<(nArgs-1);
							if (!(pFN->dwArgs & bArgs))
								nB = -8;
						}
						else
							if (!pFN->bZeroArgs)
								nB = -8;
					}
					else
						nB = -7;
				}
			}
		}
		if (nB<0)   {
			strncpy (szres, szkey, LUA_MINBUFFER);
			szres[LUA_MINBUFFER-1] = '\0';
		}
	}
	return nB;
}

/*
** lua_thira_search_C_function :
*/
stBrowserDescriptor* lua_thira_search_lua_function (const char *strLF)
{
    stBrowserDescriptor* pLF=NULL;
    map<string, stBrowserDescriptor*>::iterator it;

	if (bInStrictSyntax)   {
		it=m_mapLF.find (strLF);
		if (it!=m_mapLF.end())   {
			pLF=(*it).second;
		}
	}
	else
		pLF = (stBrowserDescriptor*)1;
    return pLF;
}

/*
** lua_thira_error :
*/
char *lua_thira_error (char *sz, const char *str, int n)
{
	switch (n)   {
		case -1 :
			sprintf (sz, "%s, %s", str, "Type is different from declaration");
			break;
		case -2 :
			sprintf (sz, "%s, %s", str, "Row index out of range");
			break;
		case -3 :
			sprintf (sz, "%s, %s", str, "Field not found");
			break;
		case -4 :
			sprintf (sz, "%s, %s", str, "Type is different from declaration");
			break;
		case -5 :
			sprintf (sz, "%s, %s", str, "Type is different from declaration");
			break;
		case -6 :
			sprintf (sz, "%s, %s", str, "Variable not declared");
			break;
		case -7 :
			sprintf (sz, "%s, %s", str, "Function not found");
			break;
		case -8 :
			sprintf (sz, "%s, %s", str, "Wrong number of parameters");
			break;
		default :
			sz[0] = '\0';
			break;
	}
	return sz;
}

/*
** GetRecordFromGlobals :
*/
static void GetGlobalsFromRecord (recGlobals* rc, stGlobalDescriptor* p)
{
    strcpy (p->strName, rc->szName);
    strcpy (p->strModule, rc->szModule);
    p->nType=rc->nType;
    p->nVarType=rc->nVarType;
    memcpy (&p->arr, &rc->arr, sizeof (p->arr));
    p->mat.nRows=rc->mat.nRows;
    p->mat.nCols=rc->mat.nCols;
    memcpy (p->mat.anTypes, rc->mat.anTypes, sizeof (rc->mat.anTypes));
}

/*
** AddGlobalVariable :
*/
static void AddGlobalVariable (stGlobalDescriptor* p)
{
	m_mapGV.insert (map <string, stGlobalDescriptor*>::value_type(p->strName, p));
}

/*
** AddFunction :
*/
static void AddFunction (stFunctionDescriptor* p)
{
	m_mapFN.insert (map <string, stFunctionDescriptor*>::value_type(p->strName, p));
}

/*
** AddLuaFunction :
*/
static void AddLuaFunction (stBrowserDescriptor* p)
{
	m_mapLF.insert (map <string, stBrowserDescriptor*>::value_type(p->szSymbol, p));
}

/*
** ClearMapGlobalVariables :
*/
static void ClearMapGlobalVariables ()
{
    stGlobalDescriptor* pGV;
    map<string, stGlobalDescriptor*>::iterator it;

    for (it=m_mapGV.begin(); it!=m_mapGV.end(); it++)   {
        pGV=(*it).second;
        free (pGV);
    }
    m_mapGV.clear ();
}

/*
** ClearMapFunctions :
*/
static void ClearMapFunctions ()
{
    stFunctionDescriptor* pFN;
    map<string, stFunctionDescriptor*>::iterator it;

    for (it=m_mapFN.begin(); it!=m_mapFN.end(); it++)   {
        pFN=(*it).second;
        free (pFN);
    }
    m_mapFN.clear ();
}

/*
** ClearMapLuaFunctions :
*/
static void ClearMapLuaFunctions ()
{
    stBrowserDescriptor* pLF;
    map<string, stBrowserDescriptor*>::iterator it;

    for (it=m_mapLF.begin(); it!=m_mapLF.end(); it++)   {
        pLF=(*it).second;
        free (pLF);
    }
    m_mapLF.clear ();
}

/*
** GetFunctionsPath :
*/
static int GetFunctionsPath (char *szBuffer)
{
	HKEY hKey = 0;
	DWORD dwType, dwBufSize;
	int nB = 0;
	char *subkey = "SOFTWARE\\Kinema";

	if (RegOpenKey (HKEY_LOCAL_MACHINE, subkey, &hKey) == ERROR_SUCCESS)    {
        dwType = REG_SZ; dwBufSize = LUAL_BUFFERSIZE;
        if (RegQueryValueEx (hKey, "PathLibraryHelp", 0, &dwType, (BYTE*)szBuffer, &dwBufSize) == ERROR_SUCCESS)
			nB = 1;
        RegCloseKey(hKey);
	}
	return nB;
}
