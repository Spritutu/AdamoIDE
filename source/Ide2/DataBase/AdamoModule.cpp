/*
** AdamoModule.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoModule.h"
#ifdef ADAMO_IDE
#include "workspace.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoLogicModule, CObject, 1)

/*******************
 * CAdamoLogicModule : costruttore
 */
CAdamoLogicModule::CAdamoLogicModule(CObject *pParent)
{
    m_pParent = pParent;
}

/*******************
 * ~CAdamoLogicModule : distruttore
 */
CAdamoLogicModule::~CAdamoLogicModule()
{
}

/*******************
 * Serialize : serializzazione
 */
void CAdamoLogicModule::Serialize(CArchive &ar)
{
    CAdamoGroup::Serialize(ar);
}

/*******************
 * CreateMap : crea mappa
 */
void CAdamoLogicModule::CreateMap()
{
    m_mDevMap.RemoveAll();
    m_mDevMap[GetName()] = this;
    Iterator((CAdamoGroup*)this,GetName());
}

/*******************
 * GestisciVersione : crea mappa
 */
int CAdamoLogicModule::GestisciVersione()
{
    int nB=0;
#ifdef ADAMO_IDE
    int nOldVersion=GETWS()->GetDataVersion ();
    int nVersion=GETWS()->GetProgramVersion ();
    if (nOldVersion!=nVersion)
        if (nOldVersion==0x00010000)   {
GETWS ()->SetDataVersion (0x00010100);
            GETWS ()->SavePlantSettings ();
            nB=1;
        }
#endif
    return nB;
}

/*
 * Iterator : itera i nodi dell'albero
 */
void CAdamoLogicModule::Iterator(CAdamoGroup *pGrp,CString strName)
{
    pGrp->Init();
    pGrp->DevInit();

    while(pGrp->DevMore()){
        CObject *dev = (CObject*)pGrp->DevGetNext();
        ((CAdamoBase*)dev)->SetParentObj((CObject*)pGrp);
        CString strPath(strName + "." + ((CAdamoBase*)dev)->GetName());
        m_mDevMap[strPath] = dev;
    }

    while(pGrp->More()){
        CObject *obj = (CObject*)pGrp->GetNext();
        ((CAdamoBase*)obj)->SetParentObj((CObject*)pGrp);
        CString strPath(strName + "." + ((CAdamoBase*)obj)->GetName());
        m_mDevMap[strPath] = obj;
        Iterator((CAdamoGroup*)obj,strPath);
    }
}

/*
 * IteratorUsers : itera i nodi dell'albero
 */
void CAdamoLogicModule::IteratorUsers(CAdamoGroup *pGrp)
{
    pGrp->Init();
    pGrp->DevInit();

    while(pGrp->DevMore()){
        CObject *dev = (CObject*)pGrp->DevGetNext();
        ((CAdamoBase*)dev)->SetWAcces(ePswUser);
        ((CAdamoBase*)dev)->SetRAcces(ePswUser);
    }
    while(pGrp->More()){
        CObject *obj = (CObject*)pGrp->GetNext();
        IteratorUsers((CAdamoGroup*)obj);
    }
}

/*
** UpdateDeviceFile :
*/
void CAdamoLogicModule::ResetID ()
{
    m_nID=-1;
    /* inizializz	iamo a -1 tutti gli ID */
    IteratorID (this);
}

/*
 * Iterator : itera i nodi dell'albero
 */
void CAdamoLogicModule::IteratorID (CAdamoGroup *pGrp)
{
    pGrp->Init();
    pGrp->DevInit();

    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase*)pGrp->DevGetNext();
        pDev->SetDeviceID (-1);
    }

    while(pGrp->More()){
        CAdamoGroup *pNextGrp = (CAdamoGroup *)pGrp->GetNext();
        pNextGrp->SetDeviceID (-1);
        IteratorID (pNextGrp);
    }
}

/*
** WriteFreeDevices : scrive nel file AdamoDatFile i dispositivi il cui indirizzo
**                    logico non e' stato ancora assegnato.
*/
void CAdamoLogicModule::WriteFreeDevices (CAdamoDatiFile* padf)
{

    /* inizializziamo a -1 tutti gli ID */
    IteratorDevice (this, padf);
}

/*
** IteratorDevice : itera i nodi dell'albero per scoprire i dispositivi il cui
**                  ID non e' ancora stato assegnato e quando ne trova uno
**                  lo scrive nel file DEVICE.DAT.
*/
void CAdamoLogicModule::IteratorDevice (CAdamoGroup *pGrp, CAdamoDatiFile* padf)
{
#if !defined ADAMO_LINK && !defined ADAMO_THIRACC
    pGrp->Init();
    pGrp->DevInit();

    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase*)pGrp->DevGetNext();
        if (pDev->GetDeviceID ()==-1)
            padf->AddAdamoDevice (pDev);
    }

    while(pGrp->More()){
        CAdamoGroup *pNextGrp = (CAdamoGroup *)pGrp->GetNext();
        IteratorDevice (pNextGrp, padf);
    }
#endif
}

/*
** CaricaDescrizioneTradotta :
*/
void CAdamoLogicModule::CaricaDescrizioneTradotta (CTraduzioni* pTr)
{
    CObject* dev;
    CString strKey, strMessaggio, strDescrizione;
    LONG dwNumero;

    for (POSITION pos=m_mDevMap.GetStartPosition (); pos;)   {
        m_mDevMap.GetNextAssoc (pos, strKey, dev);
        if (pTr->MessaggioStringa (1, strKey, &dwNumero, &strMessaggio, &strDescrizione))
            ((CAdamoBase*)dev)->SetDesc (strMessaggio);
    }
}

/*
** SalvaDescrizioneTradotta :
*/
int CAdamoLogicModule::SalvaDescrizioneTradotta (CTraduzioni* pTr)
{
    CObject* dev;
    CString strKey, strMessaggio, strDescrizione;
    LONG dwNumero;
    int n=0;

    for (POSITION pos=m_mDevMap.GetStartPosition (); pos;)   {
        m_mDevMap.GetNextAssoc (pos, strKey, dev);
        if (dev)   {
            if (pTr->MessaggioStringa (1, strKey, &dwNumero, &strMessaggio, &strDescrizione))   {
                if (((CAdamoBase*)dev)->GetDesc ()!=strMessaggio)   {
                    pTr->CancellaMessaggioConStringa (1, strKey);
                    pTr->ScriviMessaggio (1, -1, strKey, ((CAdamoBase*)dev)->GetDesc (), "");
                }
            }
            else   {
                if (((CAdamoBase*)dev)->GetDesc ()!="")
                    pTr->ScriviMessaggio (1, -1, strKey, ((CAdamoBase*)dev)->GetDesc (), "");
            }
        }
        else
            n=-1;
    }
    return n;
}

/*
 * IteratorXML : itera i nodi dell'albero
 */
void CAdamoLogicModule::IteratorXML (CAdamoGroup *pGrp, stAdamoParametri *pAdamoParametri, CStdioFile *pStdioFile)
{
    pGrp->Init();
    pGrp->DevInit();

	BeginXML (pGrp, pStdioFile);
    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase*)pGrp->DevGetNext();
        pDev->Serialize (pAdamoParametri, pStdioFile);
    }

    while(pGrp->More()){
        CAdamoGroup *pNextGrp = (CAdamoGroup *)pGrp->GetNext();
        pNextGrp->SetDeviceID (-1);
        IteratorXML (pNextGrp, pAdamoParametri, pStdioFile);
    }
	EndXML (pStdioFile);
}

/*
** BeginXML :
*/
void CAdamoLogicModule::BeginXML (CAdamoGroup *pGrp, CStdioFile *pStdioFile)
{
	CString str;

	pStdioFile->WriteString ("<Group>\n");
	str.Format ("<Name>%s</Name>\n", pGrp->GetName ());
	pStdioFile->WriteString (str);
	str.Format ("<Description>%s</Description>\n", pGrp->GetDesc ());
	pStdioFile->WriteString (str);
}

/*
** EndXML :
*/
void CAdamoLogicModule::EndXML (CStdioFile *pStdioFile)
{
	pStdioFile->WriteString ("</Group>");
}

/*
** GetItem :
*/
CObject* CAdamoLogicModule::GetItem(enDispositivi eDev, int nID)
{
	return (CObject *) IteratorDeviceID((CAdamoGroup*)this, eDev, nID);
}

/*
 * IteratorUsers : itera i nodi dell'albero
 */
CAdamoBase* CAdamoLogicModule::IteratorDeviceID(CAdamoGroup *pGrp, enDispositivi eDev, int nID)
{
	CAdamoBase* pDevRet = NULL;

    pGrp->Init();
    pGrp->DevInit();
    while(pGrp->DevMore()){
        CAdamoBase *dev = (CAdamoBase *)pGrp->DevGetNext();
		if (dev->GetType () == eDev && dev->GetDeviceID () == nID)
			return dev;
    }
    while(pGrp->More()){
        CObject *obj = (CObject*)pGrp->GetNext();
        if ((pDevRet = IteratorDeviceID((CAdamoGroup*)obj, eDev, nID)) != NULL)
			break;
    }
	return pDevRet;
}
