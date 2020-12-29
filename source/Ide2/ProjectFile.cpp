// ProjectFile.cpp: implementation of the CProjectFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "ProjectFile.h"

#include "LuaEditor.h"
#include "MainFrame.h"
#include "Executor.h"
#include "ScintillaView.h"
#include "LuaHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectFile::CProjectFile() : m_nBroseError (0), m_bInComment (false), m_pObject (NULL)
{
	RemoveAllDebugLines();
	RemoveAllBreakPoints(TRUE);

	GetSystemTime(&m_timeCompiled);
}

CProjectFile::~CProjectFile()
{

}

void CProjectFile::RemoveAllDebugLines()
{
	m_nMinDebugLine = 2147483647;
	m_nMaxDebugLine = 0;

	m_debugLines.RemoveAll();
}

void CProjectFile::AddDebugLine(int nLine)
{
	m_debugLines[nLine] = 1;
	if ( nLine<m_nMinDebugLine )
		m_nMinDebugLine = nLine;
	if ( nLine>m_nMaxDebugLine )
		m_nMaxDebugLine = nLine;
}

void CProjectFile::RemoveAllBreakPoints(BOOL isFromLoad)
{
	m_nMinBreakPoint = 2147483647;
	m_nMaxBreakPoint = 0;

	m_breakPoints.RemoveAll();

    if(!isFromLoad){
        CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
        if(pProject)
            pProject->SetIsModified (true);
    }
}

void CProjectFile::AddBreakPoint(int nLine)
{
	m_breakPoints[nLine] = 1;
	if ( nLine<m_nMinBreakPoint)
		m_nMinBreakPoint = nLine;
	if ( nLine>m_nMaxBreakPoint )
		m_nMaxBreakPoint = nLine;

	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
    pProject->SetIsModified (true);
}

void CProjectFile::RemoveBreakPoint(int nLine)
{
	m_breakPoints.RemoveKey(nLine);

	m_nMinBreakPoint = 2147483647;
	m_nMaxBreakPoint = 0;

	POSITION pos = m_breakPoints.GetStartPosition();
	int nTemp;
	while (pos != NULL)
	{
		m_breakPoints.GetNextAssoc( pos, nLine, nTemp );
		if ( nLine<m_nMinBreakPoint)
			m_nMinBreakPoint = nLine;
		if ( nLine>m_nMaxBreakPoint )
			m_nMaxBreakPoint = nLine;
	}

	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
    pProject->SetIsModified (true);
}

int CProjectFile::GetNextDebugLine(int nLine)
{
	int nTemp;
	++nLine;

	while ( nLine<=m_nMaxDebugLine )
		if ( m_debugLines.Lookup(nLine, nTemp) )
			return nLine;
		else
			++nLine;

	return 0;
}

int CProjectFile::GetPreviousDebugLine(int nLine)
{
	int nTemp;
	--nLine;

	while ( nLine>=m_nMinDebugLine )
		if ( m_debugLines.Lookup(nLine, nTemp) )
			return nLine;
		else
			--nLine;

	return 0;
}

int CProjectFile::GetNearestDebugLine(int nLine)
{
	int nTemp, nNearest;
	if ( m_debugLines.Lookup(nLine, nTemp) )
		return nLine;

	if ( (nNearest=GetNextDebugLine(nLine)) > 0 )
		return nNearest;

	if ( (nNearest=GetPreviousDebugLine(nLine)) > 0 )
		return nNearest;

	return 0;
}

BOOL CProjectFile::PositionBreakPoints()
{
	if ( !CLuaHelper::LoadDebugLines(this) )
		return FALSE;

	BOOL bModified = FALSE;
	POSITION pos = m_breakPoints.GetStartPosition();
	int nLine, nTemp, nNearest;
	while (pos != NULL)
	{
		m_breakPoints.GetNextAssoc( pos, nLine, nTemp );
		nNearest = GetNearestDebugLine(nLine);
		if ( nNearest == 0 )
		{
			m_breakPoints.RemoveKey(nLine);
			bModified = TRUE;
		}
		else if ( nLine != nNearest )
		{
			m_breakPoints.RemoveKey(nLine);
			m_breakPoints.SetAt(nNearest, 1);
			bModified = TRUE;
		}
	}

	return bModified;
}

CString CProjectFile::GetName()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	return CString(fname);
}

CString CProjectFile::GetNameExt()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	return CString(fname)+ext;
}

BOOL CProjectFile::HasBreakPoint(int nLine)
{
	int nTemp;
	return m_breakPoints.Lookup(nLine, nTemp);
}

void CProjectFile::SetBreakPointsIn(CLuaEditor *pEditor)
{
	pEditor->ClearAllBreakpoints();

	POSITION pos = m_breakPoints.GetStartPosition();
	int nLine, nTemp;
	while (pos != NULL)
	{
		m_breakPoints.GetNextAssoc( pos, nLine, nTemp );
		pEditor->SetBreakpoint(nLine);
	}
}

BOOL CProjectFile::HasFile(CString strPathName)
{
	return !m_strPathName.CompareNoCase(strPathName);
}


BOOL CProjectFile::Load(CArchive &ar,CProject *pPrj)
{
    BOOL b=TRUE;

	RemoveAllDebugLines();
    RemoveAllBreakPoints(TRUE);

	ar >> m_strRelPathName;

    m_strPathName = pPrj->GetProjectDir();
	PathAppend(m_strPathName.GetBuffer(MAX_PATH), m_strRelPathName);
	ar >> m_nMinBreakPoint;
	ar >> m_nMaxBreakPoint;

	int nBreakPoints;
	ar >> nBreakPoints;

	for ( int i=0; i<nBreakPoints; ++i )
	{
		int nLine;
		ar >> nLine;

		m_breakPoints[nLine] = 1;
	}
    if (_access (m_strPathName, 00))
        b=FALSE;
	return b;
}

BOOL CProjectFile::Save(CArchive &ar)
{
	ar << m_strRelPathName;
	ar << m_nMinBreakPoint;
	ar << m_nMaxBreakPoint;

	int nBreakPoints = m_breakPoints.GetCount();
	ar << nBreakPoints;

	POSITION pos = m_breakPoints.GetStartPosition();
	int nLine, nTemp;
	while (pos != NULL)
	{
		m_breakPoints.GetNextAssoc( pos, nLine, nTemp );
		ar << nLine;
	}

	return TRUE;
}

BOOL CProjectFile::IsModified()
{
	WIN32_FILE_ATTRIBUTE_DATA sourceFile, compiledFile;

	if (! ::GetFileAttributesEx(m_strPathName, GetFileExInfoStandard, &sourceFile) )
		return TRUE;

	if (! ::GetFileAttributesEx(GetOutputPathNameExt(), GetFileExInfoStandard, &compiledFile) )
		return TRUE;

	ULARGE_INTEGER sourceTime, compiledTime;
	sourceTime.LowPart = sourceFile.ftLastWriteTime.dwLowDateTime;
	sourceTime.HighPart = sourceFile.ftLastWriteTime.dwHighDateTime;
	compiledTime.LowPart = compiledFile.ftLastWriteTime.dwLowDateTime;
	compiledTime.HighPart = compiledFile.ftLastWriteTime.dwHighDateTime;

	return ( sourceTime.QuadPart > compiledTime.QuadPart );
}

int CProjectFile::Compile(bool bOnlyPreprocess)
{
    int nB=0, bIsMain=false;
	CExecutor m_exe;
    CString strOutput;

	COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();

    if (!bOnlyPreprocess)
        pOutput->Write(GetNameExt() + "\n");

    CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	if (!_access (GetPathName(), 00))   {
		CFile::Rename (GetPathName(), GetPreprocessPathName());
		if (pProject->GetName ()==GetName ())
			bIsMain=true;    
		nB=pProject->GetPreprocessor().StartPreProcessing (GetPreprocessPathName(), GetPathName(), bIsMain!=0);
		pProject->SaveDefinesInfo ();
		pProject->SaveGlobalVariables ();
		if (!bOnlyPreprocess)   {
			strOutput=pProject->GetPreprocessor().GetOutputString ();
			if ( !strOutput.IsEmpty() )
				pOutput->Write(strOutput);
			else   {
				CString strCmdLine;
				if (pProject->SyntaxCheckingEnabled ())
					strCmdLine.Format("\"%s\" -o \"%s\" -f \"%s\" \"%s\"", 
						theApp.GetModuleDir() + "\\" + "luac.exe", GetOutputPathNameExt(), GetProjectDir (), GetPathName());
				else
					strCmdLine.Format("\"%s\" -o \"%s\" \"%s\"", 
						theApp.GetModuleDir() + "\\" + "luac.exe", GetOutputPathNameExt(), GetPathName());
				m_exe.Execute(strCmdLine);
				strOutput = m_exe.GetOutputString();
    			if ( !strOutput.IsEmpty() )   {
					strOutput=FiltraLua (strOutput);
					pOutput->Write(strOutput);
					nB=-3;
				}
			}
		}
		CFile::Remove (GetPathName());
		CFile::Rename (GetPreprocessPathName(), GetPathName());
	}
	else   {
		strOutput.Format ("%s: %s %s\n", LOADSTRING (IDS_ADAMO_COMPILER), GetPathName(),  LOADSTRING (IDS_ADAMO_COMPILER_1));
		pOutput->Write(strOutput);
		nB = -4;
	}
	return nB;
}

void CProjectFile::DeleteIntermediateFiles()
{
	DeleteFile(GetOutputPathNameExt());
}

CString CProjectFile::GetOutputPathNameExt()
{
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();	
	return pProject->GetIntermediateDir() + "\\" + GetOutputNameExt();
}

CString CProjectFile::GetProjectDir()
{
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();	
	return pProject->GetProjectDir ();
}

void CProjectFile::UpdateRelPathName(CProject *pPrj)
{
	PathRelativePathTo(m_strRelPathName.GetBuffer(MAX_PATH), 
	pPrj->GetProjectDir(), FILE_ATTRIBUTE_DIRECTORY,m_strPathName, 0);
	m_strRelPathName.ReleaseBuffer();
}

CString CProjectFile::GetPreprocessPathName ()
{
    char szBuffer[_MAX_PATH];
    strcpy (szBuffer, GetPathName ());
    strcpy (strrchr (szBuffer, '.'), ".ppr");
    return CString (szBuffer);
}

void CProjectFile::CheckFiles ()
{
    if (!_access (GetPreprocessPathName (), 00))   {
        if (!_access (GetPathName (), 00))    
            CFile::Remove (GetPathName ());
        CFile::Rename (GetPreprocessPathName (), GetPathName ());
    }
}

#define LUA_ID "luac.exe"

CString CProjectFile::FiltraLua (CString str)
{
    CString strRet (str);
    char* p=str.GetBuffer (str.GetLength ()+1), *q, *s;

    q=strchr (p, ':');
    while (q)   {
        s=q;
        if (q-p>=(int)strlen (LUA_ID))   {
            q-=strlen (LUA_ID);
            if (!strncmp (q, LUA_ID, strlen (LUA_ID)))   {
                strRet=LOADSTRING (IDS_ADAMO_COMPILER)+s;
                break;
            }
        }
        q=strchr (q+1, ':');
    }    
    str.ReleaseBuffer ();
    return strRet;
}

/*
** Browse :
*/
int CProjectFile::Browse ()
{
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	CStdioFile f;
	CString strFile, str;
	int nProgLine = 0;

	if (f.Open (GetPathName(), CFile::modeRead))   {
		CAdamoLibrary* pLibrary = new CAdamoLibrary;
		CObjectsLibrary* pObjectLibrary = new CObjectsLibrary;
		pLibrary->SetName (GetPathName());
		pObjectLibrary->SetName (GetPathName());
		while (f.ReadString (strFile))   {
			nProgLine++;
			if (m_strGlobalName == "")   {
				if (CleanComment (strFile, str))   {
					if (BrowseObject (pObjectLibrary, str, nProgLine) > 0)
						if (BrowseFunction (pLibrary, str, nProgLine) > 0)
							if (BrowseGlobalVariable (str, nProgLine) > 0)
								if (BrowseLocalVariable (str, nProgLine) > 0)
									if (BrowseDefines (str, nProgLine) > 0)   {
									}
					if (m_nBroseError)
						break;
				}
			}
			else
				SearchTableFields (strFile, nProgLine);
		}
		pProject->AddFunctionsLibrary (pLibrary);
		pProject->AddObjectsLibrary (pObjectLibrary);
		f.Close ();
	}
	return 0;
}

/*
** BrowseFunction :
*/
int CProjectFile::BrowseFunction (CAdamoLibrary *pLibrary, CString& str, int nProgLine)
{
	int nState, nName, nParam, n, nB = 1;
	bool bOk;

	/* se non e' un oggetto */
	if (m_pObject == NULL)   {
		char *pName = new char [2048];
		char *pParam = new char [2048];
		/* andiamo a cercare la definizione di una function */
		if ((n = str.Find ("function")) != -1)   {
			/* allochiamo l'oggetto function */
			CAdamoFunction* pFunction = new CAdamoFunction;
			/* trovata */
			nB = 0;
			n += CString ("function").GetLength ();
			nName = nState = 0;
			bOk = false;
			while (n < str.GetLength ())   {
				switch (nState)   {
					case 0 :
						/* andiamo a cercare l'inizio del nome della function */
						if (str[n] == ' ' || str[n] == '\t')
							break;
						else
							nState = 1;
					case 1 :
						/* andiamo a cercare la fine del nome */
						if (str[n] == ' ' || str[n] == '\t' || str[n] == '(')
							nState = 2;
						else   {
							pName[nName++] = str[n];
							break;
						}
					case 2 :
						/* andiamo a cercare l'inizio dei parametri */
						if (str[n] == '(')   {
							nParam = 0;
							nState = 3;
						}
						break;
					case 3 :
						/* andiamo a cercare la fine dei parametri */
						if (str[n] == ')')   {
							if (nParam)   {
								pParam[nParam] = '\0';
								CAdamoParameter* pParameter = new CAdamoParameter;
								pParameter->m_strName = pParam;
								pFunction->AddParameter (pParameter);
								nParam = 0;
							}
							pName[nName] = '\0';
							nState = 4; 
							bOk = true;
						}
						else
							if (str[n] == ' ' || str[n] == '\t')
								;
							else
								if (str[n] == ',')   {
									pParam[nParam] = '\0';
									CAdamoParameter* pParameter = new CAdamoParameter;
									pParameter->m_strName = pParam;
									pFunction->AddParameter (pParameter);
									nParam = 0;
								}
								else
									pParam[nParam++] = str[n];
						break;
					case 4:
						/* se non ci sono altri caratteri, function ok */
						if (str[n] != ' ' && str[n] != '\t' && str[n] != '\n')
							bOk = false;
						break;
				}
				n++;
			}
			if (bOk)   {
				/* abbiamo trovato una function, memorizziamola nel browser */
				if (strlen (pName) <= NMAXIDENTIFIER)   {
					CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
					CBrowserContainer &bc = pProject->GetBrowserContainer ();
					bc.Add (BROWSER_TYPE_FUNCTION, BROWSER_DEFINITION, pName, "", "", GetPathName (), nProgLine);
					m_strCurrentFunction = pName;
					pFunction->SetName (pName);
					pFunction->SetLine (nProgLine);
					pLibrary->AddFunction (pFunction);
				}
				else   {
					delete pFunction;
					BrowseError (-1, nProgLine);
					nB = -1;
				}
			}
		}
		delete pName;
		delete pParam;
	}
	return nB;
}

/*
** BrowseGlobalVariable :
*/
int CProjectFile::BrowseGlobalVariable (CString& str, int nProgLine)
{
	CStringList listTableFields;
	int nState, nName, n, nB = 1;
	char *pName = new char [2048];
	bool bOk;
	/* andiamo a cercare la definizione di una function */
	if ((n = str.Find ("global")) != -1)   {
		/* trovata */
		nB = 0;
		n += CString ("global").GetLength ();
		nName = nState = 0;
		bOk = false;
		while (n < str.GetLength () && !bOk)   {
			switch (nState)   {
				case 0 :
					/* andiamo a cercare l'inizio del nome della function */
					if (str[n] == ' ' || str[n] == '\t')
						break;
					else
						nState = 1;
				case 1 :
					/* andiamo a cercare la fine del nome */
					if (str[n] == ' ' || str[n] == '\t' || str[n] == '[')   {
						char szBuffer[1024];
						pName[nName] = '\0';
						strcpy (szBuffer, str);
						if (SearchTableFields (szBuffer, n, listTableFields))
							m_strGlobalName = pName;
						bOk = true;
					}
					else
						pName[nName++] = str[n];
					break;
			}
			n++;
		}
		if (bOk)   {
			/* abbiamo trovato una variabile globale, memorizziamola nel browser */
			if (strlen (pName) <= NMAXIDENTIFIER)   {
				CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
				CBrowserContainer &bc = pProject->GetBrowserContainer ();
				bc.Add (BROWSER_TYPE_GLOBAL, BROWSER_DEFINITION, pName, "", "", GetPathName (), nProgLine);
				if (listTableFields.GetCount () > 0)   {
					for (POSITION pos = listTableFields.GetHeadPosition (); pos;)   {
						CString str = listTableFields.GetNext (pos);
						bc.Add (BROWSER_TYPE_TABLE_FIELD, BROWSER_DEFINITION, str, pName, "", GetPathName (), nProgLine);
					}
				}
			}
			else   {
				BrowseError (-1, nProgLine);
				nB = -1;
			}
		}
	}
	delete pName;
	return nB;
}

/*
** BrowseLocalVariable :
*/
int CProjectFile::BrowseLocalVariable (CString& str, int nProgLine)
{
	int nState, nName, n, nB = 1;
	char *pName = new char [2048];
	bool bOk;
	/* andiamo a cercare la definizione di una function */
	if ((n = str.Find ("local")) != -1)   {
		/* trovata */
		nB = 0;
		n += CString ("local").GetLength ();
		nName = nState = 0;
		bOk = false;
		while (n < str.GetLength () + 1 && !bOk)   {
			switch (nState)   {
				case 0 :
					/* andiamo a cercare l'inizio del nome della variabile locale */
					if (str[n] == ' ' || str[n] == '\t')
						break;
					else
						nState = 1;
				case 1 :
					/* andiamo a cercare la fine del nome */
					if (str[n] == ' ' || str[n] == '\t' || str[n] == '\0' || str[n] == ',' || str[n] == '=')   {
						bOk = true;
						nState = 2;
					}
					else   {
						pName[nName++] = str[n];
						break;
					}
				case 2 :
					if (str[n] == '\0' || str[n] == '=')
						break;
					else   {
						if (str[n] == ',')
							nState = 0;
					}
			}
			if (bOk)   {
				pName[nName] = '\0';
				/* abbiamo trovato una function, memorizziamola nel browser */
				if (strlen (pName) <= NMAXIDENTIFIER)   {
					CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
					CBrowserContainer &bc = pProject->GetBrowserContainer ();
					bc.Add (BROWSER_TYPE_LOCAL, BROWSER_DEFINITION, pName, m_strCurrentFunction, "", GetPathName (), nProgLine);
					nName = 0;
					if (nState == 0)
					bOk = false;
				}
				else   {
					BrowseError (-1, nProgLine);
					nB = -1;
					break;
				}
			}
			n++;
		}
	}
	delete pName;
	return nB;
}

/*
** BrowseDefines :
*/
int CProjectFile::BrowseDefines (CString& str, int nProgLine)
{
	int nState, nName, n, nB = 1;
	char *pName = new char [2048];
	bool bOk;
	/* andiamo a cercare la definizione di una function */
	if ((n = str.Find ("define")) != -1)   {
		/* trovata */
		nB = 0;
		n += CString ("define").GetLength ();
		nName = nState = 0;
		bOk = false;
		while (n < str.GetLength () && !bOk)   {
			switch (nState)   {
				case 0 :
					/* andiamo a cercare l'inizio del nome della function */
					if (str[n] == ' ' || str[n] == '\t')
						break;
					else
						nState = 1;
				case 1 :
					/* andiamo a cercare la fine del nome */
					if (str[n] == ' ' || str[n] == '\t')   {
						pName[nName] = '\0';
						bOk = true;
					}
					else
						pName[nName++] = str[n];
					break;
			}
			n++;
		}
		if (bOk)   {
			/* abbiamo trovato una function, memorizziamola nel browser */
			if (strlen (pName) <= NMAXIDENTIFIER)   {
				CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
				CBrowserContainer &bc = pProject->GetBrowserContainer ();
				bc.Add (BROWSER_TYPE_DEFINE, BROWSER_DEFINITION, pName, "", "", GetPathName (), nProgLine);
			}
			else   {
				BrowseError (-1, nProgLine);
				nB = -1;
			}
		}
	}
	delete pName;
	return nB;
}

/*
** BrowseError :
*/
void CProjectFile::BrowseError (int nError, int nProgLine)
{
	COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();
	CString str;

	_delete (m_pObject);
	switch (nError)   {
		case -1 :
			str.Format ("%s: %s:%d:%s\n", LOADSTRING (IDS_ADAMO_PREPROCESSOR), GetPathName (), nProgLine, (const char*)LOADSTRING (IDS_ADAMOPREPROCESSOR_20));
			break;
		default :
			break;
	}
	pOutput->Write(str);
	m_nBroseError = nError;
}

/*
** CleanComment :
*/
bool CProjectFile::CleanComment (CString str, CString& strRet)
{
	CString strNew;
	int nIndex;
	bool bRet = false;

	if ((nIndex = str.Find ("--")) != -1 && str.GetLength () >= nIndex + 4 && str[nIndex+2] != '[' && str[nIndex+3] != '[')
		strRet = str.Left (nIndex);
	else   {
		int nIndexFirst = str.Find ("--[["), nIndexLast;
		if (!m_bInComment)   {
			if (nIndexFirst != -1)   {
				strRet = str.Left (nIndexFirst);
				m_bInComment = true;
			}
			else
				strRet = str;
		}
		else
			strRet = "";
		if (m_bInComment)   {
			while ((nIndexLast = str.Find ("]]--")) != -1)   {
				nIndexLast+=4;
				m_bInComment = false;
				nIndexFirst = str.Find ("--[[", nIndexLast);
				if (nIndexFirst != -1)   {
					strRet += str.Mid (nIndexLast, nIndexFirst - nIndexLast);
					m_bInComment = true;
				}
				else   {
					strRet += str.Mid (nIndexLast, str.GetLength() - nIndexLast);
					break;
				}
			}
		}
	}
	return strRet.GetLength () > 0;
}

/*
** BrowseObject :
*/
int CProjectFile::BrowseObject (CObjectsLibrary* pLibrary, CString& str, int nProgLine)
{
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	CBrowserContainer& bc = pProject->GetBrowserContainer();
	CStringArray strTokens;
	CString strWork (str);
	CString strBaseObj;
	int n, nPosition = 0;
	bool bObjectTreated = false;

	if (m_pObject != NULL)   {
		int nFunction = IsALuaKey (str, "function", " \t(");
		int nAs = IsALuaKey (str, "as", " \t");
		if (nFunction != -1)   {
			bool bIsVirtual = false;
			bool bIsPublic = false;
			Tokenize (strTokens, strWork, " \t(,)");
			if (strTokens[0] == "public" || strTokens[0] == "private")   {
				if (strTokens[0] == "public")
					bIsPublic = true;
				strTokens.RemoveAt (0);
			}
			if (strTokens[0] == "virtual")   {
				bIsVirtual = true;
				strTokens.RemoveAt (0);
			}
			if (strTokens[0] == "function")   {
				if (strTokens.GetSize () > 1)   {
					CAdamoObjectMethod* pObjMethod = new CAdamoObjectMethod;
					((CAdamoObjectElement*)pObjMethod)->SetName (strTokens[1]);
					((CAdamoObjectElement*)pObjMethod)->SetDescription ("");
					pObjMethod->SetLine (nProgLine);
					pObjMethod->SetVirtual (bIsVirtual);
					pObjMethod->SetPublic (bIsPublic);
					int nParameter = 2;
					while (nParameter < strTokens.GetSize ())   {
						CAdamoParameter* pParameter = new CAdamoParameter;
						pParameter->m_strName = strTokens[nParameter];
						pParameter->m_strDescription = "";
						pParameter->m_strEnum = "";
						pObjMethod->AddParameter (pParameter);
						nParameter++;
					}
					bc.Add(BROWSER_TYPE_METHOD, BROWSER_DEFINITION, ((CAdamoObjectElement*)pObjMethod)->GetName (), "", m_pObject->GetName(), GetPathName(), nProgLine);
					m_pObject->AddElement (pObjMethod);
				}
				else
					BrowseError (-1, nProgLine);
			}
			else
				BrowseError (-1, nProgLine);
		}
		else
		if (nAs != -1)   {
			bool bIsPublic = false;
			Tokenize (strTokens, strWork, " \t=");
			if (strTokens[0] == "public" || strTokens[0] == "private")   {
				if (strTokens[0] == "public")
					bIsPublic = true;
				strTokens.RemoveAt (0);
			}
			if (strTokens.GetSize () == 3 || strTokens.GetSize () == 4)   {
				CAdamoData ad;
				if (strTokens[2] == "boolean")   {
					ad.nType = LUA_TBOOLEAN;
					ad.AdamoData.b = 0;
				}
				else
					if (strTokens[2] == "number")   {
					ad.nType = LUA_TNUMBER;
					ad.AdamoData.fp = 0.0;
					}
				else
					if (strTokens[2] == "string")   {
					ad.nType = LUA_TSTRING;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "table")   {
					ad.nType = LUA_TTABLE;
					ad.AdamoData.str[0] = '\0';
					bObjectTreated = true;
				}
				else
				if (strTokens[2] == "object")   {
					ad.nType = LUA_TOBJECT;
					ad.AdamoData.str[0] = '\0';
					bObjectTreated = true;
				}
				else
				if (strTokens[2] == "DigitalInput")   {
					ad.nType = OBJ_DIGITAL_INPUT;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "DigitalOutput")   {
					ad.nType = OBJ_DIGITAL_OUPUT;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "AnalogInput")   {
					ad.nType = OBJ_ANALOG_INPUT;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "AnalogOutput")   {
					ad.nType = OBJ_ANALOG_OUTPUT;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "Axis")   {
					ad.nType = OBJ_AXIS;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "Encoder")   {
					ad.nType = OBJ_ENCODER;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "Timer")   {
					ad.nType = OBJ_TIMER;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "Memory")   {
					ad.nType = OBJ_MEMORY;
					ad.AdamoData.str[0] = '\0';
				}
				else
				if (strTokens[2] == "DeviceCan")   {
					ad.nType = OBJ_GENERIC_CAN;
					ad.AdamoData.str[0] = '\0';
				}
				else
					ad.nType = LUA_TNIL;
				if (ad.nType != LUA_TNIL)   {
					if (strTokens.GetSize () == 4)   {
						switch (ad.nType)   {
							case LUA_TBOOLEAN :
								ad.AdamoData.b = atoi (strTokens[3]);
								break;
							case LUA_TNUMBER :
								ad.AdamoData.fp = atof (strTokens[3]);
								break;
							case LUA_TSTRING :
								strcpy (ad.AdamoData.str, strTokens[3]);
								break;
							default :
								break;
						}
					}
					CAdamoObjectData* pObjData = new CAdamoObjectData;
					bc.Add(BROWSER_TYPE_OBJECT_FIELD, BROWSER_DEFINITION, strTokens[0], "", m_pObject->GetName(), GetPathName(), nProgLine);
					pObjData->SetName (strTokens[0]);
					pObjData->SetPublic (bIsPublic);
					pObjData->SetData (ad);
					pObjData->SetDataLine (nProgLine);
					m_pObject->AddElement (pObjData);
				}
			}
			else
				if (strTokens[2] == "object")   {
					if (strTokens.GetSize () >= 5)   {
						CAdamoObjectData* pObjData = new CAdamoObjectData;
						CAdamoData ad;
						ad.nType = LUA_TOBJECT;
						strcpy (ad.AdamoData.str, strTokens[4]);
						pObjData->SetName (strTokens[0]);
						pObjData->SetPublic (bIsPublic);
						pObjData->SetData (ad);
						m_pObject->AddElement (pObjData);
						bObjectTreated = true;
					}
				}
			}
	}
	if ((n = IsALuaKey (strWork, "object", " \t")) != -1 && !bObjectTreated)   {
		int nReplace = strWork.Replace (':', ' ');
		if (nReplace <= 1)   {
			Tokenize (strTokens, strWork, " \t");
			if (strTokens.GetSize () == 2 ||  strTokens.GetSize () == 3)   {
				if (strTokens[1] == "end")   {
					bc.Add(BROWSER_TYPE_OBJECT, BROWSER_DEFINITION, m_pObject->GetName (), "", "", GetPathName(), m_pObject->GetLine ());
					pLibrary->AddObject (m_pObject);
					m_pObject = NULL;
				}
				else   {
					if (strTokens.GetSize () == 3)
						strBaseObj = strTokens[2];
					m_pObject = new CAdamoObj;
					m_pObject->SetName (strTokens[1]);
					m_pObject->SetDescription ("");
					m_pObject->SetLine (nProgLine);
					m_pObject->SetBasedObj (strBaseObj);
				}
			}
			else
				BrowseError (-1, nProgLine);
		}
		else
			BrowseError (-1, nProgLine);
	}
	return 1;
}

/*
** Tokenize :
*/
int CProjectFile::Tokenize (CStringArray& strList, CString str, CString strSeparator)
{
	int nPosition = 0;

	CString strToken = str.Tokenize (strSeparator, nPosition);
	while(!strToken.IsEmpty())
	{
		strList.Add (strToken);
		strToken = str.Tokenize(strSeparator, nPosition);
	}
	return strList.GetSize ();
}

/*
** IsALuaKey :
*/
int CProjectFile::IsALuaKey (CString strBase, CString str, CString strSeparator)
{
	bool bIniOK = false, bFinOK = false;
	int nIndexIni = strBase.Find (str), nRet = -1;
	if (nIndexIni != -1)   {
		int nIndexFin = nIndexIni + strlen (str);
		if (nIndexIni > 0)   {
			for (int i = 0; i<strSeparator.GetLength (); i++)   {
				if (strBase[nIndexIni-1] == strSeparator[i])   {
					bIniOK = true; break;
				}
			}
		}
		else
			bIniOK = true;
		if (nIndexFin<nIndexIni + strBase.GetLength ())   {
			for (int i = 0; i<strSeparator.GetLength (); i++)   {
				if (strBase[nIndexFin] == strSeparator[i])   {
					bFinOK = true;   break;
				}
			}
		}
		else
			bFinOK = true;
	}
	if (bIniOK && bFinOK)
		nRet = nIndexIni;
	return nRet;
}

/*
** SearchTableFields :
*/
bool CProjectFile::SearchTableFields (char* str, int& n, CStringList& sl)
{
	char* p, *q, *s, pName[1024];
	bool bRet = false;
	while (str[n] != '[' && str[n] != ' ' && str[n] != '\t' && str[n] != '\0')
		n++;
	p = &str[n];
	if (*p == '[')   {
		s = strchr (p, '_');
		if (s)   {
			*s = '\0';
			bRet = true;
		}
		while (true)   {
			p = strchr (p, ':');
			if (p)   {
				p++;
				while (*p == ' ' || *p == '\t')
					p++;
				if (*p == '\0')
					break;
				else   {
					q = p;
					while (*p != ' ' && *p != '\t' && *p != ',' && *p != '\0')
						p++;
					if (p - q < 1024)   {
						memcpy (pName, q, p - q);
						pName[p - q] = '\0';
						sl.AddTail (CString (pName));
					}
				}
			}
			else
				break;
		}
	}
	return bRet;
}

/*
** SearchTableFields :
*/
void CProjectFile::SearchTableFields (CString strFile, int nProgLine)
{
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	CBrowserContainer &bc = pProject->GetBrowserContainer ();
	char szBuffer[1024], pName[1024], *s, *p, *q;
	bool bUnderScore = false;

	strcpy (szBuffer, strFile);
	s = strchr (szBuffer, '_');
	if (s)   {
		*s = '\0';
		bUnderScore = true;
	}
	p = szBuffer;
	while (true)   {
		p = strchr (p, ':');
		if (p)   {
			p++;
			while (*p == ' ' || *p == '\t')
				p++;
			if (*p == '\0')
				break;
			else   {
				q = p;
				while (*p != ' ' && *p != '\t' && *p != ',' && *p != '\0')
					p++;
				if (p - q < 1024)   {
					memcpy (pName, q, p - q);
					pName[p - q] = '\0';
					bc.Add (BROWSER_TYPE_TABLE_FIELD, BROWSER_DEFINITION, pName, m_strGlobalName, "", GetPathName (), nProgLine);
				}
			}
		}
		else
			break;
	}
	if (!bUnderScore)
		m_strGlobalName = "";
}
