#pragma once	

/* include files */
#include "resource.h"
#include "MainFrame.h"
#include "AdamoRSObjects.h"

class CAdamoSynCtrl
{
	public:
		CAdamoSynCtrl(void);
        virtual ~CAdamoSynCtrl ();
        virtual eRSCtrl KindOf () = 0; 
        void SetName (CString strName)          { m_strName=strName; }
        CString GetName ()                      { return m_strName; }
        void SetTabIndex (int nTabIndex)        { m_nTabIndex=nTabIndex; }
        int  GetTabIndex ()                     { return m_nTabIndex; }
        void SetID (int nID)                    { m_nID=nID; }
        int  GetID()                            { return m_nID; }
        void SetZOrder (int nZOrder)            { m_nZOrder=nZOrder; }
        int  GetZOrder ()                       { return m_nZOrder; }
        void SetValue (CString strValue)        { m_strValue=strValue; }
        CString GetValue ()                     { return m_strValue; }
        CAdamoRSStyle& GetObjStyle ()           { return m_objStyle; }
		CAdamoRSVarControl &GetObjVar ()		{ return m_objVar; }
		virtual int Create () = 0;
		virtual int Destroy () = 0;
		virtual int OnSave () = 0;
		virtual int OnLoad () = 0;
		virtual int OnDownload () = 0;
		virtual int OnTimer () = 0;
    protected :
        CAdamoRSStyle       m_objStyle;
		CAdamoRSVarControl  m_objVar;
    private :
        CString m_strName;
		CString m_strValue;
        int     m_nZOrder;
        int     m_nTabIndex;
        int     m_nID;
};

class CAdamoRTSynCtrl : public CAdamoSynCtrl
{
	public :
		CAdamoRTSynCtrl () : m_pMachine (NULL), m_pDev (NULL), m_nLogicAddress (-1), m_eDispositivi (enBase)   { }
		void SetMachine (CAdamoMachine *pMachine)   { m_pMachine = pMachine; }
		void SetPath (CString strPath)				{ 
			m_strPath = strPath; 
			m_pDev = (CAdamoBase *)((CAdamoCfgMachine *)GETSYNCONT()->GetMachine ())->GetItem (GetPath ());
		}
		void SetLogicAddress (int nLogicAddress)	{ m_nLogicAddress = nLogicAddress; }
		void SetDeviceType (enDispositivi e)		{ m_eDispositivi = e; }
		void SetData (int nData)					{ m_nData = nData; }
		CString GetPath ()							{ return m_strPath; }
		CAdamoMachine *GetMachine ()				{ return m_pMachine; }
		enDispositivi GetDeviceType ()				{ return m_eDispositivi; }
		int GetLogicAddress ()						{ return m_nLogicAddress; }
		CAdamoBase* GetDevice ()					{ return m_pDev; }
		int GetData ()								{ return m_nData; }
	private :
		CAdamoMachine *m_pMachine;
		CAdamoBase    *m_pDev;
		CString		   m_strPath;
		enDispositivi  m_eDispositivi;
		int		m_nLogicAddress;
		int     m_nData;
};
