// AdamoBase.cpp: implementation of the CAdamoBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxadv.h>
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoGroup.h"
#include "AdamoModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoBase, CObject, 1)

/*******************
 * CAdamoBaseDisp : costruttore
 */
CAdamoBase::CAdamoBase():m_sName(""),m_sDesc(""),m_nRAcces(3),m_nWAcces(3),m_isPublic(0),m_pObjParent(NULL), m_nID(0)
{
    memset (m_cSpare, 0, MAX_DEV_SPARE);
}

/*******************
 * CAdamoBaseDisp : copy constructor
 */
CAdamoBase::CAdamoBase(CAdamoBase& ab):m_sName(ab.m_sName),
                                       m_sDesc(ab.m_sDesc),
                                       m_nRAcces(ab.m_nRAcces),
                                       m_nWAcces(ab.m_nWAcces),
                                       m_isPublic(ab.m_isPublic),
                                       m_pObjParent(ab.m_pObjParent), 
                                       m_nID(ab.m_nID)
{
    memset (m_cSpare, 0, MAX_DEV_SPARE);
}

/*******************
 * ~CAdamoBaseDisp : distruttore
 */
CAdamoBase::~CAdamoBase()
{

}

/*******************
 * Serialize : serializzazione
 */
void CAdamoBase::Serialize(CArchive &ar)
{
    CFile* pFile=ar.GetFile ();
    bool bIsInClipboard=pFile->IsKindOf (RUNTIME_CLASS(CSharedFile))!=0;

    CObject::Serialize(ar);
    if (ar.IsStoring())   {
       ar << m_sName;
       if (bIsInClipboard)
            ar << m_sDesc;
       ar << m_nRAcces << m_nWAcces << m_isPublic;
       for (int i=0;i<MAX_DEV_SPARE;++i)
          ar << m_cSpare[i];
    }
    else   {
        ar >> m_sName;
       if (bIsInClipboard)
            ar >> m_sDesc;
        ar >> m_nRAcces >> m_nWAcces >> m_isPublic;
        for (int i=0;i<MAX_DEV_SPARE;++i)
           ar >> m_cSpare[i];   
   }
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoBase::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str, strHWAddress;

	str.Format ("<Name>%s</Name>\n", m_sName);
	pFile->WriteString (str);
	str.Format ("<Description>%s</Description>\n", m_sDesc);
	pFile->WriteString (str);
	str.Format ("<ReadAccess>%d</ReadAccess>\n", m_nRAcces);
	pFile->WriteString (str);
	str.Format ("<WriteAccess>%d</WriteAccess>\n", m_nWAcces);
	pFile->WriteString (str);
	str.Format ("<Public>%d</Public>\n", m_isPublic);
	pFile->WriteString (str);
	str.Format ("<ID>%d</ID>\n", m_nID);
	pFile->WriteString (str);
	strHWAddress = GetHWAddress (pAdamoParametri);
	if (strHWAddress != "")   {
		str.Format ("<HWAddress>%s</HWAddress>\n", strHWAddress);
		pFile->WriteString (str);
	}
}

/*******************
 * SetParentObj : setta oggetto parente
 */
void CAdamoBase::SetParentObj(CObject *pObj)
{
    m_pObjParent = pObj;
}

/*******************
 * GetParentObj : ritorna oggetto parente
 */
CObject *CAdamoBase::GetParentObj()
{
    return m_pObjParent;
}

/*******************
 * GetPathName : ritorna path
 */
CString CAdamoBase::GetPathName(bool bNoMachine)
{
    CString sPath;
    CObject *pParent = GetParentObj();
    
    if (!pParent)
        sPath = GetName();
    while (pParent)   {
        /*
        if(pParent->IsKindOf(RUNTIME_CLASS(CProject)))   {
            if(!bNoMachine)
            sPath = ((CAdamoGroup*)((CProject*)pParent)->GetMachineModule())->GetName()+sPath;
		else 
                sPath = sPath.Right(sPath.GetLength()-1);
            pParent = ((CAdamoGroup*)((CProject*)pParent)->GetMachineModule())->GetParentObj();
        }else 
        */
		if (pParent->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))   {
			sPath = "."+((CAdamoBase*)pParent)->GetName()+sPath;
			pParent = ((CAdamoBase*)pParent)->GetParentObj();
		}
        else 
			if (pParent->IsKindOf(RUNTIME_CLASS(CAdamoLogicModule)))   {
                if (!bNoMachine)
				    sPath = ((CAdamoBase*)pParent)->GetName()+sPath;
                else
                    sPath = sPath.Right(sPath.GetLength()-1);
				pParent = NULL;
			}
    }
    return sPath;
}

/*
** operator= operatore di ugualianza
*/
CAdamoBase& CAdamoBase::operator= (const CAdamoBase& ab)
{
    m_sName=ab.m_sName;
    m_sDesc=ab.m_sDesc;
    m_nRAcces=ab.m_nRAcces;
    m_nWAcces=ab.m_nWAcces;
    m_isPublic=ab.m_isPublic;
    m_pObjParent=ab.m_pObjParent;
    m_nID=ab.m_nID;
    return *this;
}

/*
** GetHWAddress :
*/
CString CAdamoBase::GetHWAddress (stAdamoParametri *pAdamoParametri)
{
	CString str;

	if (m_nID != -1)
		switch (GetType ())   {
			case enInputDigitale :
				str = pAdamoParametri->AdamoInputDig[m_nID].szHWAddress;
				break;
			case enOutputDigitale :
				str = pAdamoParametri->AdamoOutputDig[m_nID].szHWAddress;
				break;
			case enInputAnalogico :
				str = pAdamoParametri->AdamoInputAnag[m_nID].szHWAddress;
				break;
			case enOutputAnalogico :
				str = pAdamoParametri->AdamoOutputAnag[m_nID].szHWAddress;
				break;
			case enAsse :
				str = pAdamoParametri->AdamoAsse[m_nID].szHWAddress;
				break;
			case enEncoder :
				str = pAdamoParametri->AdamoEncoder[m_nID].szHWAddress;
				break;
			case enGenericCan :
				str = pAdamoParametri->AdamoCanDevice[m_nID].szHWAddress;
				break;
		}
	return str;
}
