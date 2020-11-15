/*
**
*/

#ifndef __ADAMO__FILEDATI__
#define __ADAMO__FILEDATI__

/* include files */
#include "AdamoDati.h"
#include "path.h"

/* namespace */
using namespace nsPath;

/* constants definitions */
#define DEVICE_FILE             "Device.dat"
#define HWCONFIG_FILE           "Hardware.dat"
#define MACHINE_SETTINGS_FILE   "Settings.dat"
#define PLANT_SETTINGS_FILE     "Plant.dat"
#define RESOURCES_FILE          "Resources.mdb"
#define CONFIG_FILE				"Config.xml"

#define OFFSET_MD       4

#define SSCNETIII_444   0
#define SSCNETIII_888   1

/* forward references */
class CAdamoLogicModule;
class CAdamoBase;

struct stIdRegistry   {
    int m_nInpDig;
    int m_nOutDig;
    int m_nInpAnag;
    int m_nOutAnag;
    int m_nCounter;
    int m_nTimer;
    int m_nAxes;
    int m_nCan;
};

class CAdamoDatiFile : public CFile
{
    protected :
        stAdamoParametri AdamoParametri;
        stIdRegistry     IdRegistry;
    public :
        CAdamoDatiFile ();
        virtual ~CAdamoDatiFile();
    public :
        BOOL Open (CPath pPath, bool bMode);
        void WriteData ();
        void ReadData ();
        void Close();
        void CloseFile();
        int  AddAdamoDevice    (CAdamoBase* pDev);
        int  RemoveAdamoDevice (CAdamoBase* pDev);
        int  ModifyAdamoDevice (CAdamoBase* pDev);
		int  ForceAdamoDevice (CAdamoBase* pDev, CString strHWAddress);
        void Refresh (CAdamoLogicModule *pModule);
        stAdamoParametri* GetAdamoParametri () { return &AdamoParametri; }
        void Reset ();
        int  GetDeviceName (int nDevice, int nTypeDevice, char* szBuffer);
        int  GetDevicePathName (int nDevice, int nTypeDevice, char* szBuffer);
        int  GetDeviceHW (int nDevice, int nTypeDevice, char* szBuffer);
        char* GetName (char* szBuffer);
		void ResetHWAddress ();
		void ResetEthHWAddress ();
    private :
        int ManageInputDig (CAdamoBase* pDev);
        int ManageOutputDig (CAdamoBase* pDev);
        int ManageInputAnag (CAdamoBase* pDev);
        int ManageOutputAnag (CAdamoBase* pDev);
        int ManageCounter (CAdamoBase* pDev);
        int ManageTimer (CAdamoBase* pDev);
        int ManageAxes (CAdamoBase* pDev);
        int ManageCanDevice (CAdamoBase* pDev);
        void SetInputDigData (CAdamoBase* pDev, int nIndex);
        void SetOutputDigData (CAdamoBase* pDev, int nIndex);
        void SetInputAnagData (CAdamoBase* pDev, int nIndex);
        void SetOutputAnagData (CAdamoBase* pDev, int nIndex);
        void SetCounterData (CAdamoBase* pDev, int nIndex);
        void SetTimerData (CAdamoBase* pDev, int nIndex);
        void SetAxesData (CAdamoBase* pDev, int nIndex);
        void SetCanDeviceData (CAdamoBase* pDev, int nIndex);
		int  ExistDevice (CAdamoBase* pDev);
};

struct stMDSettings   {
    /* ultimo modo di visualizzazione impostato */
    struct   {
        /* AxCmdBar */
        int nCmdBar;
        int nAxSettings;
        int nTypeMove;
        int nStepComboIndex;
        double fpVMan;
        double fpFeedRate;
        double fpQuotaAbs;
        int nCurAxIndex;
        /* AxStatus */
        int nGenComboIndex;
        /* AxSettings */
        int nModeSingleInterp;
        double fpQuotaPos;
        double fpQuotaNeg;
        double fpDelay;
    } stAxSettings [NMAXAXIS];
    /* AxStatus */
    int nAxStatus;
    int nCurPageAx;
    int nCurIndexAx;
    int nCurIndexView;
    int nCurAx;
    /* AxSettings */
    int nFolderIDAxSettings;
    /* windows */
    RECT rcIO;
    RECT rcAxes;
    char achSpare[0xfd14];
};

struct stMachineSettings   {
    struct stMDSettings MDSettings;
    char achSpare[0xF0000];
};

class CAdamoMachineSettings : public CFile
{
    private :
        stMachineSettings m_as;    
    public :
        CAdamoMachineSettings ();
        BOOL Open ();
        void WriteMDSettings (stMDSettings* MDS);
        void ReadMDSettings (stMDSettings* MDS);
        void Close();
};

struct stPlantSettings   {
    int nMode;
    int nActiveModule;
    int nDataVersion;
};

class CAdamoPlantSettings : public CFile
{
    private :
        stPlantSettings m_ps;    
    public :
        CAdamoPlantSettings ();
        BOOL Open ();
        void WritePlantSettings ();
        void ReadPlantVersion ();
        void ReadPlantSettings ();
        int  GetMode ()         { return m_ps.nMode; }
        int  GetActiveModule () { return m_ps.nActiveModule; }
        void SetMode (int nMode)            { m_ps.nMode=nMode; }
        void SetActiveModule (int nModule)  { m_ps.nActiveModule=nModule; }
        int  GetDataVersion ()  { return m_ps.nDataVersion; }
        void SetDataVersion (int nVersion)  { m_ps.nDataVersion=nVersion; }
        void Close();
};

class CCavStrzFile : public CStdioFile   {
    private :
        CString m_strNomeFile;
        CString m_strNomeAsse;
        CStringArray m_objAxList;
        int     m_bEnabled[NMAXAXESCORRETTED];
        double m_fpLimNeg;
        double m_fpLimPos;
        double m_fpIntervallo;
        double m_fpStep;
        double m_data[NMAXROWS][NMAXAXESCORRETTED];
        int    m_nMaxRow;
    public :
        CCavStrzFile () : m_nMaxRow (0) { for (int i=0; i<NMAXROWS; i++) for (int n=0; n<NMAXAXESCORRETTED; n++) m_data[i][n]=NOVALUE; memset (m_bEnabled, 0, sizeof (m_bEnabled)); m_objAxList.SetSize (NMAXAXESCORRETTED); }
        int  Open (const char* szNomeFile, bool bRead=false);
        void Close ();
        void SetNomeFile (CString strNomeFile)  { m_strNomeFile=strNomeFile; }
        void SetNomeAsse (CString strNomeAsse)  { m_strNomeAsse=strNomeAsse; }
        void SetLimits (double fpLimNeg, double fpLimPos)   { m_fpLimNeg=fpLimNeg; m_fpLimPos=fpLimPos; }
        void SetInterval (double fpIntervallo)  { m_fpIntervallo=fpIntervallo; }
        void SetEnabled (int nAx1, int nAx2, int nAx3, int nAx4, int nAx5)   {
            m_bEnabled[0]=nAx1; m_bEnabled[1]=nAx2; m_bEnabled[2]=nAx3; m_bEnabled[3]=nAx4; m_bEnabled[4]=nAx5;
        }
        void SetStep (double fpStep)            { m_fpStep = fpStep; }
        void SetData (int nRow, int nCol, double fpValue) { if (nRow+1>m_nMaxRow) m_nMaxRow = nRow+1; m_data[nRow][nCol]=fpValue; }
        void SetMaxRow (int nRows)              { m_nMaxRow=nRows; }
        int  GetMaxRow ()                       { return m_nMaxRow; }
        CString& GetNomeFile ()                 { return m_strNomeFile; }
        CString& GetNomeAsse ()                 { return m_strNomeAsse; }
        double GetLimPos ()                     { return m_fpLimPos; }
        double GetLimNeg ()                     { return m_fpLimNeg; }
        double GetInterval ()                   { return m_fpIntervallo; }
        double GetStep ()                       { return m_fpStep; }
        void GetEnabled (int* pnAx1, int* pnAx2, int* pnAx3, int* pnAx4, int* pnAx5)   {
            *pnAx1=m_bEnabled[0]; *pnAx2=m_bEnabled[1]; *pnAx3=m_bEnabled[2]; *pnAx4=m_bEnabled[3]; *pnAx5=m_bEnabled[4];
        }
        double GetData (int nRow, int nCol)     { return m_data[nRow][nCol]; }
        int WriteData ();
        int ReadData ();
        CStringArray& GetAxList ()              { return m_objAxList; }
        void AddAx (CString str, int nIndex)    { m_objAxList[nIndex]=str; }
        void ClearAx ()                         { m_objAxList.RemoveAll (); m_objAxList.SetSize(NMAXAXESCORRETTED); }
        void EmptyColumn (int nColumn)          { int i; for (i=0; i<NMAXROWS; i++)  m_data[i][nColumn]=NOVALUE; }
        void GetAxAddress (CString m_strNomeAsse, int* nAddress, struct stAdamoParametri* pAP);
        int  FormatBuffer (int& s, unsigned char* pBuf, int nDim, struct stAdamoParametri* pAP);
};

class CAdamoHWConfigFile : public CFile
{
    private :
        stHWAdamoConf m_HWConf;
    public :
        CAdamoHWConfigFile ();
        BOOL Open (CPath pPath, bool bMode);
        void WriteData ();
        void ReadData  ();
        void CloseFile ();
		void SetVariableData (int nID, int nVar, int nOffset, int nLenght, int nType);
		void GetVariableData (int nID, int nVar, bool *pbConfigured, int *pnOffset, int *pnLenght, int *pnType);
		void CleanEthercatInfo ();
		void RemoveVariableData (int nID);
        WORD GetSSCNETPar (int nCard, int nStation, int nType, int nPar)               { return m_HWConf.m_HWSSCNETParameters[nCard].m_awParameters[nStation][nType][nPar]; }
        void SetSSCNETPar (int nCard, int nStation, int nType, int nPar, WORD wValue)  { m_HWConf.m_HWSSCNETParameters[nCard].m_awParameters[nStation][nType][nPar]=wValue; }
        int  GetCanBaudrate (int nCanale)                                { return m_HWConf.m_HWCanOpenNet[nCanale].m_nBaudRate; }
        void SetCanBaudrate (int nCanale, int nBaudRate)                 { m_HWConf.m_HWCanOpenNet[nCanale].m_nBaudRate=nBaudRate; }
        int  GetSyncFrequency (int nCanale)                              { return m_HWConf.m_HWCanOpenNet[nCanale].m_nSyncFrequency; }
        void SetSyncFrequency (int nCanale, int nSyncFrequency)          { m_HWConf.m_HWCanOpenNet[nCanale].m_nSyncFrequency=nSyncFrequency; }
        int  GetSDOTimeOut (int nCanale)                                 { return m_HWConf.m_HWCanOpenNet[nCanale].m_nSDOTimeout; }
        void SetSDOTimeOut (int nCanale, int nSDOTimeout)                { m_HWConf.m_HWCanOpenNet[nCanale].m_nSDOTimeout=nSDOTimeout; }
        int  GetGuardTime (int nCanale)                                  { return m_HWConf.m_HWCanOpenNet[nCanale].m_nGuardTime; }
        void SetGuardTime (int nCanale, int nGuardTime)                  { m_HWConf.m_HWCanOpenNet[nCanale].m_nGuardTime=nGuardTime; }   
        int  GetLifeTime (int nCanale)                                   { return m_HWConf.m_HWCanOpenNet[nCanale].m_nLifeTime; }
        void SetLifeTime (int nCanale, int nLifeTime)                    { m_HWConf.m_HWCanOpenNet[nCanale].m_nLifeTime=nLifeTime; }
        int  GetEnableWarnings (int nCanale)                             { return m_HWConf.m_HWCanOpenNet[nCanale].m_nEnableWarning; }
        void SetEnableWarnings (int nCanale, int nEnableWarning)         { m_HWConf.m_HWCanOpenNet[nCanale].m_nEnableWarning=nEnableWarning; }
		int  GetSSCNETFrequency (int nCanale)							 { return m_HWConf.m_HWSSCNETNet[nCanale].m_nFrequency; }
		void SetSSCNETFrequency (int nCanale, int nFrequency)			 { m_HWConf.m_HWSSCNETNet[nCanale].m_nFrequency = nFrequency; }
        int  GetSSCNETWarnings (int nCanale)                             { return m_HWConf.m_HWSSCNETNet[nCanale].m_bEnableWarnings; }
        void EnableSSCNETWarnings (int nCanale, bool bEnable=true)       { m_HWConf.m_HWSSCNETNet[nCanale].m_bEnableWarnings=bEnable; }
        int  IsSSCNETNetworkEnabled (int nCanale)                        { return m_HWConf.m_HWSSCNETNet[nCanale].m_bNetworkEnabled; }
        void EnableSSCNETNetwork (int nCanale, bool bEnable=true)        { m_HWConf.m_HWSSCNETNet[nCanale].m_bNetworkEnabled=(int)bEnable; }
        void SetSSCNETNetworkType (int nCard, bool bNetworkType=false) { m_HWConf.m_HWSSCNETNet[nCard].m_bNetworkType=bNetworkType; }
        bool GetSSCNETNetworkType (int nCard)                          { return m_HWConf.m_HWSSCNETNet[nCard].m_bNetworkType != 0; }
        void SetAbsPosition (int nCard, int nStation, WORD wMultiRevCounter, DWORD dwOneRevPos)    { 
			if (nCard ==0)   {
				m_HWConf.m_HWSSCNETAbsPos[nStation-1].wOPMultiRevCounter=wMultiRevCounter; 
				m_HWConf.m_HWSSCNETAbsPos[nStation-1].dwOPOneRevPos=dwOneRevPos; 
			}
			else   {
				m_HWConf.m_HWSSCNETAbsPosMS[nCard][nStation-1].wOPMultiRevCounter=wMultiRevCounter; 
				m_HWConf.m_HWSSCNETAbsPosMS[nCard][nStation-1].dwOPOneRevPos=dwOneRevPos; 
			}
		}
        void GetAbsPosition (int nCard, int nStation, WORD &wMultiRevCounter, DWORD &dwOneRevPos)  { 
			if (nCard ==0)   {
				wMultiRevCounter=m_HWConf.m_HWSSCNETAbsPos[nStation-1].wOPMultiRevCounter; 
				dwOneRevPos=m_HWConf.m_HWSSCNETAbsPos[nStation-1].dwOPOneRevPos; 
			}
			else   {
				wMultiRevCounter = m_HWConf.m_HWSSCNETAbsPosMS[nCard][nStation-1].wOPMultiRevCounter; 
				dwOneRevPos = m_HWConf.m_HWSSCNETAbsPosMS[nCard][nStation-1].dwOPOneRevPos; 
			}
		}
        stHWCanOpenNode   &GetCanNode (int nCanale, int nNode)           { return m_HWConf.m_HWCanOpenNet[nCanale].m_HWCanOpenNode[nNode]; }
        stHWSSCNETStation &GetSSCNETStation (int nCanale, int nStation)  { return m_HWConf.m_HWSSCNETNet[nCanale].m_hWSSCNETStation[nStation]; }
		CString GetEthercatMasterXML ()									 { return CString (m_HWConf.m_HWEthercatNet.strMasterXML); }
		void    SetEthercatMasterXML (CString strMasterXML)				 { strcpy (m_HWConf.m_HWEthercatNet.strMasterXML, (const char *)strMasterXML); }
		int     GetEthercatSlave ()										 { return m_HWConf.m_HWEthercatNet.nSlave; }
		void    SetEthercatSlave (int nSlave)							 { m_HWConf.m_HWEthercatNet.nSlave = nSlave; }
		int  GetEthercatDeviceType (int nSlave, int nIndex)				 { return m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].nDeviceType[nIndex]; }
		void SetEthercatDeviceType (int nSlave, int nIndex, int nType)	 { m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].nDeviceType[nIndex]=nType; }
		void EnableEthercatVariables (int nSlave, int nIndex, stHWEthVarMask iEnableMask)	{ m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EnableVariables[nIndex]=iEnableMask; }
		void GetEnabledEthercatVariables (int nSlave, int nIndex, stHWEthVarMask *iEnableMask)	{ *iEnableMask=m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EnableVariables[nIndex]; }
		void SetRequestedState (int nRequestedState)					 { m_HWConf.m_HWEthercatNet.m_nRequestedState=nRequestedState; }
		int  GetRequestedState ()										 { return m_HWConf.m_HWEthercatNet.m_nRequestedState; }
		void SetEthercatDCStartTime (__int64 nDCStartTime)			     { m_HWConf.m_HWEthercatNet.m_nStartDCTiming=nDCStartTime; }
		__int64 GetEthercatDCStartTime ()							     { return m_HWConf.m_HWEthercatNet.m_nStartDCTiming; }
		void SetEthercatDCOffsetTime (int nDCOffsetTime)				 { m_HWConf.m_HWEthercatNet2.m_nOffsetDCTiming=nDCOffsetTime; }
		int  GetEthercatDCOffsetTime ()									 { return m_HWConf.m_HWEthercatNet2.m_nOffsetDCTiming; }
		void SetEthercatFrequency (int nFrequency)					     { m_HWConf.m_HWEthercatNet.m_nFrequency=nFrequency; }
		int  GetEthercatFrequency ()					                 { return m_HWConf.m_HWEthercatNet.m_nFrequency; }
		void SetEthercatContinousMeasuring (int nContinousMeasuring)	 { m_HWConf.m_HWEthercatNet.m_bContinuosMeasuring=nContinousMeasuring; }
		int  GetEthercatContinousMeasuring ()					         { return m_HWConf.m_HWEthercatNet.m_bContinuosMeasuring; }
		int  GetEthercatInpAnalogHW (int nSlave, int nHWAddress, double *fpLimInf, double *fpLimSup);
		int  GetEthercatOutAnalogHW (int nSlave, int nHWAddress, double *fpLimInf, double *fpLimSup);
		int  SetEthercatInpAnalogHW (int nSlave, int nHWAddress, int nSelect, double fpLimInf, double fpLimSup);
		int  SetEthercatOutAnalogHW (int nSlave, int nHWAddress, int nSelect, double fpLimInf, double fpLimSup);
		stHWEthAxInfo& GetEthercatAxInfo (int nAxis)					 { return m_HWConf.m_HWEthercatNet.HWEthAxis[nAxis]; }
		stHWEthSlave&    GetEthercatSlaveInfo (int nSlave)				 { return m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave]; }
		void RemoveEthercatInpAnalogHW (int nSlave, int nHWAddress);
		void RemoveEthercatOutAnalogHW (int nSlave, int nHWAddress);
        BYTE GetStationConf (int nCard, int nStation);
        void SetStationConf (int nCard, int nStation, BYTE conf);
		void WriteHWConfigXML (CStdioFile *pXMLFile);
		CString GetHpeDriver ()											{ return CString (m_HWConf.m_HWEthercatNet2.strHpeDriver); }
		void SetHpeDriver (CString strHpeDriver)						{ strcpy (m_HWConf.m_HWEthercatNet2.strHpeDriver, strHpeDriver); }
		int  SetEthercatInpAnalogType (int nSlave, int nHWAddress, int nType);
		int  SetEthercatOutAnalogType (int nSlave, int nHWAddress, int nType);
	private :
		void WritePrologue (CStdioFile *pXMLFile);
		void WriteEpilogue (CStdioFile *pXMLFile);
		void WriteData (CStdioFile *pXMLFile);
		void WriteChannelData (int nChannel, CStdioFile *pXMLFile);
		void WriteCanOpenData (int nChannel, CStdioFile *pXMLFile);
		void WriteCanOpenNode (int nChannel, int nNode, CStdioFile *pXMLFile);
		void WriteSSCNETData (int nChannel, CStdioFile *pXMLFile);
		void WriteEthercatData (CStdioFile *pXMLFile);
};

#endif
