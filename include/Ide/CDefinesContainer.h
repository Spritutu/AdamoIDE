/*
** CDefinesContainer
*/

#pragma once

/* include files */
#include <map>
#include "idemisc.h"

/* constants definitions */
#define DEFINES_FILE "Defines.dat"

/* data structures */
struct stDefinesDescriptor {
    CString     m_strName;
    CString     m_strModule;
    int         m_nLineDefine;
    int         m_nLineUndefine;
    CAdamoData  m_data;
};

struct stRecDefines   {
    char szName[NMAXIDENTIFIER + 1];
    char szModule[_MAX_PATH];
    int  nLineDefine;
    int  nLineUndefine;
    CAdamoData  m_data;
};

class CDefinesContainer
{
    public :
        CDefinesContainer();
        ~CDefinesContainer();
        int Add (struct stDefinesDescriptor* dd);
        struct stDefinesDescriptor* Get (CString strModule, CString str, int nLine);
        void ClearAll();
        int  Load();
        int  Save();
        void SetProjectDirectory(const char* szDir) { m_pathDir = szDir; m_pathDir.AddBackslash().Append(DEFINES_FILE); }
        void RemoveModule(const char* szModule);
    private :
        multimap<string, stDefinesDescriptor*> m_mapDefines;
        CPath m_pathDir;
};

