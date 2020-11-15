/*
** AdamoModule.h
*/ 

#if !defined(AFX_ADAMOMODULE_H__407277B0_F869_4D1F_9683_918AA4B458DE__INCLUDED_)
#define AFX_ADAMOMODULE_H__407277B0_F869_4D1F_9683_918AA4B458DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdamoGroup.h"
#include "AdamoFileDati.h"
#include "AdamoTraduzione.h"

typedef CTypedPtrMap<CMapStringToOb, CString, CObject*> CAdamoMap;

class CAdamoLogicModule : public CAdamoGroup
{
    DECLARE_SERIAL(CAdamoLogicModule)
    public:
	    CAdamoLogicModule():m_pParent(NULL) {};
	    CAdamoLogicModule(CObject *pParent);
	    virtual ~CAdamoLogicModule();
        virtual void Serialize(CArchive &ar);
	    void CreateMap();
        CObject* GetItem(CString strPath) { CObject* pObj; BOOL b=m_mDevMap.Lookup (strPath, pObj); return b ? pObj : NULL; };
		CObject* GetItem(enDispositivi eDev, int nID);
        void SetItem(CString strPath,CObject *pObj) { m_mDevMap[strPath] = pObj; };
        void WriteFreeDevices (CAdamoDatiFile* padf);
        void ResetID ();
        CObject *GetProject() { return m_pParent; };
        void CaricaDescrizioneTradotta (CTraduzioni* pTr);
        int  SalvaDescrizioneTradotta (CTraduzioni* pTr);
        bool IsLogicModuleEmpty ()   { return !(GetCount () || GetDevCount ()); }
        int  GestisciVersione();
		void IteratorXML (CAdamoGroup *pGrp, stAdamoParametri *pAdamoParametri, CStdioFile *pStdioFile);
    protected:
	    void Iterator(CAdamoGroup *pGrp,CString strName);
        void IteratorID (CAdamoGroup *pGrp);
        void IteratorDevice (CAdamoGroup *pGrp, CAdamoDatiFile* padf);
        void IteratorUsers(CAdamoGroup *pGrp);
		CAdamoBase* IteratorDeviceID(CAdamoGroup *pGrp, enDispositivi eDev, int nID);
        CAdamoMap m_mDevMap;
        CObject *m_pParent;
	private :
		void BeginXML (CAdamoGroup *pGrp, CStdioFile *pStdioFile);
		void EndXML (CStdioFile *pStdioFile);
};

#endif // !defined(AFX_ADAMOMODULE_H__407277B0_F869_4D1F_9683_918AA4B458DE__INCLUDED_)
