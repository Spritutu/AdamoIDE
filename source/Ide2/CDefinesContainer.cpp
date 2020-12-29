#include "stdafx.h"
#include "CDefinesContainer.h"

/* construction / destruction */
CDefinesContainer::CDefinesContainer()
{

}

CDefinesContainer::~CDefinesContainer()
{

}

/*
** AddDefine :
*/
int CDefinesContainer::Add (struct stDefinesDescriptor* dd)
{
    int nB = -1;

    if (dd->m_strName.GetLength() < NMAXIDENTIFIER && dd->m_strModule.GetLength () < _MAX_PATH)   {
        struct stDefinesDescriptor* p = new stDefinesDescriptor();
        p->m_strName = dd->m_strName;
        p->m_strModule = dd->m_strModule;
        p->m_nLineDefine = dd->m_nLineDefine;
        p->m_nLineUndefine = dd->m_nLineUndefine;
        p->m_data = dd->m_data;
        if (dd->m_strName.GetLength() < NMAXIDENTIFIER && dd->m_strModule.GetLength() < _MAX_PATH) {
            m_mapDefines.insert(map<string, stDefinesDescriptor*>::value_type(string(p->m_strName), p));
            nB = 0;
        }
    }
    return nB;
}

/*
** Get :
*/
struct stDefinesDescriptor* CDefinesContainer::Get(CString strModule, CString str, int nLine)
{
    CString strUpperModule, strExc;
    char szBuffer[_MAX_PATH];

    strcpy(szBuffer, strModule);
    pair <multimap<string, stDefinesDescriptor*>::iterator, multimap<string, stDefinesDescriptor*>::iterator> keyRange = m_mapDefines.equal_range(string(str));
    struct stDefinesDescriptor* p = NULL;
    for (multimap<string, stDefinesDescriptor*>::iterator it = keyRange.first; it != keyRange.second; ++it) {
        p = it->second;
        strUpperModule = p->m_strModule.MakeUpper ();
        strExc = CString (szBuffer).MakeUpper ();
        if (strUpperModule == strExc)
            break;
    }
    return p;
}

/*
** ClearAll :
*/
void CDefinesContainer::ClearAll()
{
    multimap<string, stDefinesDescriptor*>::iterator itm;
    struct stDefinesDescriptor* p;

    for (itm = m_mapDefines.begin(); itm != m_mapDefines.end(); itm++) {
        p = (*itm).second;
        delete p;
    }
    m_mapDefines.clear();
}

/*
** Load :
*/
int  CDefinesContainer::Load()
{
    stRecDefines rd;
    int nB = -1;
    CFile fDefines;
    if (!_access(m_pathDir, 0)) {
        if (fDefines.Open(m_pathDir, CFile::modeRead | CFile::typeBinary)) {
            while (fDefines.Read(&rd, sizeof(stRecDefines)) == sizeof(stRecDefines)) {
                stDefinesDescriptor* p = new stDefinesDescriptor;
                p->m_strName = rd.szName;
                p->m_strModule = rd.szModule;
                p->m_nLineDefine = rd.nLineDefine;
                p->m_nLineUndefine = rd.nLineUndefine;
                p->m_data = rd.m_data;
                Add(p);
            }
            fDefines.Close();
            nB = 0;
        }
    }
    else {
        fDefines.Open(m_pathDir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
        fDefines.Close();
    }
    return nB;
}

/*
** Save :
*/
int  CDefinesContainer::Save()
{
    int nB = -1;
    CFile fDefines;
    stRecDefines rd;
    stDefinesDescriptor* p;
    multimap<string, stDefinesDescriptor*>::iterator itm;

    if (fDefines.Open(m_pathDir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
        for (itm = m_mapDefines.begin(); itm != m_mapDefines.end(); itm++) {
            p = itm->second;
            strcpy(rd.szName, p->m_strName);
            strcpy(rd.szModule, p->m_strModule);
            rd.nLineDefine = p->m_nLineDefine;
            rd.nLineUndefine = p->m_nLineUndefine;
            rd.m_data = p->m_data;
            fDefines.Write(&rd, sizeof(stRecDefines));
        }
        fDefines.Close();
        nB = 0;
    }
    return nB;
}

/*
** RemoveModule :
*/
void CDefinesContainer::RemoveModule(const char* szModule)
{
    multimap<string, stDefinesDescriptor*>::iterator itm;
    CString strModule (szModule), strUpperModule;
    stDefinesDescriptor* p;

    itm = m_mapDefines.begin();
    while (itm != m_mapDefines.end()) {
        p = itm->second;
        strUpperModule = p->m_strModule;
        if (strUpperModule.MakeUpper () == strModule.MakeUpper ()) {
            delete p;
            itm = m_mapDefines.erase(itm);
        }
        else
            itm++;
    }
}
