/*
** CBrowserContainer.h
*/

#ifndef __ADAMO_CBROWSER_CONTAINER__
#define __ADAMO_CBROWSER_CONTAINER__

#include <map>

#define BROWSER_FILE "Browser.dat"
#define BROWSER_TYPE_NOTHING     -1
#define BROWSER_TYPE_FUNCTION    10
#define BROWSER_TYPE_GLOBAL      11
#define BROWSER_TYPE_LOCAL       12
#define BROWSER_TYPE_DEFINE      13
#define BROWSER_TYPE_SYMBOL      14
#define BROWSER_TYPE_OBJECT      15
#define BROWSER_TYPE_METHOD      16
#define BROWSER_TYPE_OBJECT_FIELD 17
#define BROWSER_TYPE_TABLE_FIELD 18

#define BROWSER_DEFINITION       0
#define BROWSER_REFERENCE        1

#define BROWSER_DATA_SIZE		 1024			 

struct stBrowserDescriptor
{
    stBrowserDescriptor () : nBrowserType (-1), nBrowserReference (-1), nLine (-1), nItem (-1), nModule (-1)   { 
        szModule[0] = szSymbol[0] = szFunction[0] = '\0';
    }
    int nBrowserType;
    int nBrowserReference;
    char szSymbol[NMAXIDENTIFIER+1];
    char szModule[_MAX_PATH+1];
    int nLine;
    char szFunction[NMAXIDENTIFIER + 1];
    char szObject[NMAXIDENTIFIER + 1];
    int nModule;
	int nItem;
};

class CBrowserContainer
{
    public :
        CBrowserContainer ();
        ~CBrowserContainer ();
    public :
        int  Add (int nType, int nReference, CString strSymbol, CString strFunction, CString strObject, CString strModule, int nLine, int nItem = -1);
        void RemoveModule (const char* szModule);
        void ClearAll ();
        int  Load ();
        int  Save ();
        void SetProjectDirectory (const char* szDir)   { strcpy (m_szDir, szDir); }
		void GetLuaFunctions (CPtrList& listLua);
		void UpdateLineNumbers (CString strModule, int nStartLine, int nUpdateLines, int nDeleteLine);
		bool GetDefinitionSymbol (CString strSymbol, CString strFunction, CString strVariable, int& nSymbolType, CString& strModule, int& nLine);
		void BeginGetReferenceSymbol (CString strSymbol, int nSymbolType, CString strFunction, CString strVariable);
		void EndGetReferenceSymbol ();
		int  GetNextReferenceSymbol (CString& strModule, int& nLine, CString& strSymbol, int& nItem);
		int  GetPrevReferenceSymbol (CString& strModule, int& nLine, CString& strSymbol, int& nItem);
		int  GetReferenceData (stBrowserDescriptor** pReferenceData)		{ *pReferenceData = m_pBrowseData; return m_nReferenceCount; }
	private :
		int GetModuleNumber (CString strModule);
        stBrowserDescriptor* GetMultipleDefinitions (stBrowserDescriptor* m_pBrowseData, int nItemSearch);
private :
        multimap<string, stBrowserDescriptor*> m_mapDefinitions;
        multimap<string, stBrowserDescriptor*> m_mapReferences;
		stBrowserDescriptor* m_pBrowseData;
        char m_szDir[_MAX_PATH];
		int m_nReferenceCount;
		int m_nReferenceIndex;
};

#endif
