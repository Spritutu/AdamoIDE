/*
**
*/

/* include files */
#include <stdafx.h>
#include "CVariableContainer.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVariableContainer::CVariableContainer ()
{
}

CVariableContainer::~CVariableContainer ()
{
    ClearAll ();
}

int CVariableContainer::Add (struct stGlobalDescriptor* pgd)
{
    struct stGlobalDescriptor* p;
    int nB=0;

    if (m_glbContainer.Lookup (pgd->strName, (void*&)p))   {
        if (!IsEqual (p, pgd))
            nB=-1;
		else
			nB = -2;
    }    
    else
        m_glbContainer.SetAt (pgd->strName, pgd);
    return nB;
}

struct stGlobalDescriptor* CVariableContainer::Get (CString str)
{
    struct stGlobalDescriptor* p;
    if (m_glbContainer.Lookup (str, (void*&)p))
        return p;
    return NULL;
}

void CVariableContainer::ClearAll ()
{
    struct stGlobalDescriptor* p;
    CString str;
    for (POSITION pos=m_glbContainer.GetStartPosition(); pos;)   {
        m_glbContainer.GetNextAssoc (pos, str, (void*&)p);
        for (int i=0; i<((struct stGlobalDescriptor*)p)->mat.nCols; i++)
            delete ((struct stGlobalDescriptor*)p)->mat.strColumnName[i];
        delete (struct stGlobalDescriptor*)p;
    }
    m_glbContainer.RemoveAll();
}

int CVariableContainer::Load ()
{
    int nB=-1, i;
    char szBuffer[NMAXIDENTIFIER+1];
	recGlobals rc;
    CFile fGlobals;
    CString fn=CString (m_szDir) + "\\" + GLOBALS_FILE;
    if (!_access (fn, 0))   {
        if (fGlobals.Open(fn, CFile::modeRead|CFile::typeBinary))   {
            while (fGlobals.Read (&rc, sizeof (recGlobals))==sizeof (recGlobals))   {
				stGlobalDescriptor* p = new stGlobalDescriptor;
				GetGlobalsFromRecord (&rc, p);
				for (i=0; i<rc.mat.nCols; i++)   {
					fGlobals.Read (szBuffer, sizeof (szBuffer)); 
					p->mat.strColumnName[i]=new char [strlen (szBuffer)+1];
					strcpy (p->mat.strColumnName[i], szBuffer);
				}
				Add (p);
			}
            fGlobals.Close ();
            nB=0;
        }
    }
    else   {
        fGlobals.Open(fn, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
        fGlobals.Close ();
    }
    return nB;
}

int CVariableContainer::Save ()
{
    int nB=-1, i;
    recGlobals rc;
    CFile fGlobals;
    CString str;
    stGlobalDescriptor* p;    
    CString fn=CString (m_szDir) + "\\" + GLOBALS_FILE;
    char szBufferName[NMAXIDENTIFIER+1];

    if (fGlobals.Open(fn, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))   {
        for (POSITION pos=m_glbContainer.GetStartPosition (); pos;)   {
            m_glbContainer.GetNextAssoc (pos, str, (void*&)p);
            GetRecordFromGlobals (p, &rc);
            fGlobals.Write (&rc, sizeof (recGlobals));
            for (i=0; i<rc.mat.nCols; i++)   {
                strcpy (szBufferName, p->mat.strColumnName[i]);
                fGlobals.Write (szBufferName, NMAXIDENTIFIER+1);
            }
        }
        fGlobals.Close ();
        nB=0;
    }
    return nB;
}

void CVariableContainer::GetRecordFromGlobals (stGlobalDescriptor* p, recGlobals* rc)
{
    strcpy (rc->szName, p->strName);
    strcpy (rc->szModule, p->strModule);
    rc->nType=p->nType;
    rc->nVarType=p->nVarType;
    memcpy (&rc->arr, &p->arr, sizeof (rc->arr));
    rc->mat.nRows=p->mat.nRows;
    rc->mat.nCols=p->mat.nCols;
    memcpy (rc->mat.anTypes, p->mat.anTypes, sizeof (p->mat.anTypes));
}

void CVariableContainer::GetGlobalsFromRecord (recGlobals* rc, stGlobalDescriptor* p)
{
    p->strName=rc->szName;
    p->strModule=rc->szModule;
    p->nType=rc->nType;
    p->nVarType=rc->nVarType;
    memcpy (&p->arr, &rc->arr, sizeof (p->arr));
    p->mat.nRows=rc->mat.nRows;
    p->mat.nCols=rc->mat.nCols;
    memcpy (p->mat.anTypes, rc->mat.anTypes, sizeof (rc->mat.anTypes));
}

int CVariableContainer::IsEqual (stGlobalDescriptor* pSor, stGlobalDescriptor* pDest)
{
    int nB=0;
    if (pSor->strName==pDest->strName&&pSor->nType==pDest->nType)
        switch (pSor->nType)   {
            case GLOBALVARIABLE :
                if (pSor->nVarType==pDest->nVarType)
                    nB=1;
                break;
            case GLOBALARRAY :
                if (pSor->arr.nArrType==pDest->arr.nArrType&&pSor->arr.nArrElements==pDest->arr.nArrElements)
                    nB=1;
                break;
            case GLOBALMATRIX :
                if (pSor->mat.nRows==pDest->mat.nRows&&pSor->mat.nCols==pDest->mat.nCols)   {
                    nB=1;
                    for (int i=0; i<pSor->mat.nCols; i++)
                        if (pSor->mat.anTypes[i]!=pDest->mat.anTypes[i]||
                            strcmp (pSor->mat.strColumnName[i], pDest->mat.strColumnName[i]))   {
                            nB=0;
                            break;
                        }
                }
                break;
    }
    return nB;
}

void CVariableContainer::RemoveModule (const char* szModule)
{
    CString str, strModule;
    stGlobalDescriptor* p;
    for (POSITION pos=m_glbContainer.GetStartPosition (), pos2; pos2=pos;)   {
        m_glbContainer.GetNextAssoc (pos, str, (void*&)p);
        p->strModule.MakeUpper();
        strModule=szModule;
        strModule.MakeUpper ();
        if (p->strModule==strModule)   {
            m_glbContainer.RemoveKey (str);
            for (int i=0; i<((struct stGlobalDescriptor*)p)->mat.nCols; i++)
                delete ((struct stGlobalDescriptor*)p)->mat.strColumnName[i];
            delete p;
        }
    }
}

bool CVariableContainer::IsLegacyVariable (CString str)
{
	return str != "_GIORGETTI_CLARY"; 
}
