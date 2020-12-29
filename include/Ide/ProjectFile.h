// ProjectFile.h: interface for the CProjectFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTFILE_H__737B3FC6_28E7_47DD_B9F6_D41D550C8E23__INCLUDED_)
#define AFX_PROJECTFILE_H__737B3FC6_28E7_47DD_B9F6_D41D550C8E23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSyntaxContainer.h"
#include "CObjectsContainer.h"

class CLuaEditor;
class CProject;
class CAdamoLibrary;

class CProjectFile
{
public:
	void UpdateRelPathName(CProject *pPrj);
	void DeleteIntermediateFiles();
	int  Compile(bool bOnlyPreprocess=false);
	BOOL IsModified();
	BOOL Save(CArchive& ar);
	BOOL Load(CArchive& ar,CProject *pPrj);
	void RemoveBreakPoint(int nLine);
	BOOL HasFile(CString strPathName);
	void SetBreakPointsIn(CLuaEditor* pEditor);
	BOOL HasBreakPoint(int nLine);
	CProjectFile();
	virtual ~CProjectFile();

	void AddDebugLine(int nLine);
	void RemoveAllDebugLines();
	void AddBreakPoint(int nLine);
	void RemoveAllBreakPoints(BOOL isFromLoad = FALSE);

	BOOL PositionBreakPoints();
	int GetNearestDebugLine(int nLine);
	int GetPreviousDebugLine(int nLine);
	int GetNextDebugLine(int nLine);

	void SetPathName(CString strPathName,CProject *pPrj) { m_strPathName=strPathName; UpdateRelPathName(pPrj); };
	CString GetPathName() { return m_strPathName; };
	CString GetName();
	CString GetNameExt();
	CString GetOutputNameExt() { return GetName()+".out"; }
	CString GetOutputPathNameExt();
    CString GetPreprocessPathName ();
	CString GetProjectDir();
	CString GetRelPathName ()  { return m_strRelPathName; }

    CMap<int, int, BOOL, BOOL>& GetBreakPoints ()   {
        return m_breakPoints;
    }
    void CheckFiles ();
	int  Browse ();
	void BrowseError (int nError, int nProgLine);
	int  InBrowseError ()		{ return m_nBroseError; }
	void ResetBrowseError ()	{ m_nBroseError = 0; m_strCurrentFunction = ""; }
	CString CleanEditWord(CString str);
protected:
	CString m_strPathName, m_strRelPathName, m_strCurrentFunction;
	CMap<int, int, BOOL, BOOL> m_breakPoints;
	int m_nMinBreakPoint, m_nMaxBreakPoint;
	CMap<int, int, BOOL, BOOL> m_debugLines;
	int m_nMinDebugLine, m_nMaxDebugLine;
	SYSTEMTIME	m_timeCompiled;
	int  m_nBroseError;
	bool m_bInComment;
	CAdamoObj* m_pObject;
	CString m_strGlobalName;
private :
    CString FiltraLua (CString str);
	int BrowseFunction (CAdamoLibrary *pLibrary, CString& str, int nProgLine);
	int BrowseGlobalVariable (CString& str, int nProgLine);
	int BrowseLocalVariable (CString& str, int nProgLine);
	int BrowseDefines (CString& str, int nProgLine);
	int BrowseObject (CObjectsLibrary* pLibrary, CString& str, int nProgLine);
	bool CleanComment (CString str, CString& strRet);
	int Tokenize (CStringArray& strList, CString str, CString strSeparator);
	int IsALuaKey (CString strBase, CString str, CString strSeparator);
	bool SearchTableFields (char* p, int& n, CStringList& sl);
	void SearchTableFields (CString strFile, int nProgLine);
};

#endif // !defined(AFX_PROJECTFILE_H__737B3FC6_28E7_47DD_B9F6_D41D550C8E23__INCLUDED_)
