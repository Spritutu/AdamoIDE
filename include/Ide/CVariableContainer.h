/*
**
*/

#ifndef __ADAMO_CVARIABLE_CONTAINER__
#define __ADAMO_CVARIABLE_CONTAINER__

#include "idemisc.h"

#define GLOBALS_FILE "Globals.dat"

struct stGlobalDescriptor   {
    CString strName;
    CString strModule;
    int nType;
    int nVarType;
    struct stArr  {
        int nArrType;
        int nArrElements;
    } arr;
    struct stMat  {
        int nRows;
        int nCols;
        int anTypes[MAXCOLUMNS];
        char* strColumnName[MAXCOLUMNS];
    } mat;
};

struct recGlobals   {
    char szName[NMAXIDENTIFIER+1];
    char szModule[_MAX_PATH];
    int nType;
    int nVarType;
    struct   {
        int nArrType;
        int nArrElements;
    } arr;
    struct   {
        int nRows;
        int nCols;
        int anTypes[MAXCOLUMNS];
    } mat;
};

class CVariableContainer
{
    public :
        CVariableContainer ();
        ~CVariableContainer ();
        int Add (struct stGlobalDescriptor* pgd);
        struct stGlobalDescriptor* Get (CString str);
        void ClearAll ();
        int  Load ();
        int  Save ();
        void RemoveModule (const char* szModule);
        void SetProjectDirectory (const char* szDir)   { strcpy (m_szDir, szDir); }
        CMapStringToPtr& GetVarsContainer ()           { return m_glbContainer; }
		bool IsLegacyVariable (CString str);
    private :
        CMapStringToPtr m_glbContainer;
        char m_szDir[_MAX_PATH];
    private :
        void GetRecordFromGlobals (stGlobalDescriptor* p, recGlobals* rc);
        void GetGlobalsFromRecord (recGlobals* rc, stGlobalDescriptor* p);
        int  IsEqual (stGlobalDescriptor* pSor, stGlobalDescriptor* pDest);
};

#endif
