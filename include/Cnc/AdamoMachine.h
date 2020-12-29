/*
** AdamoMachine.h
*/

#ifndef __ADAMO_MACHINES__
#define __ADAMO_MACHINES__

/* include files */
#include "adamo.h"
#include "adamobase.h"
#include "adamoaxis.h"
#include "adamoinl.h"
#include "adamoint.h"
#include "adamocomm.h"
#include "adamomodule.h"
#include "AdamoTraduzione.h"

/* import directives */
#import "D:\Sviluppo\Adamo\AdamoBin\AdamoComm.dll" no_namespace named_guids

/* constants definitions */
#define MAX_CFGMACHINE_SPARE 0
#define IPADDRESS_BUFFER     4

/* forward reference */
class CAdamoSink;
class CAdamoOptions;

class CAdamoMachine
{
    private :
        CAdamoSink*         m_pSink;
        DWORD m_dwCookie;
    protected :
        LPUNKNOWN m_objComm;
        IAdaCommObjPtr m_ptrCnc;
		CWnd*	  m_pWnd;
        bool m_bOpened;
        int  m_nModule;
        enum eTypeConnection m_nConnType;
        CString m_strMachinePath;
        BYTE	m_achTcpIpAddress[IPADDRESS_BUFFER];
        int     m_nPort;
		eTypeModule m_eTypeModule;
		int     m_nHWModuleNumber;
    public :
        CAdamoMachine ();        
        virtual ~CAdamoMachine () {};        
        virtual int OpenMachine (const char* szMachinePath, int nMachineType=-1, eTypeConnection eTC = enLocalConnection);
        virtual int CloseMachine ();
        virtual int ReplaceMachine ();
        virtual int ReconnectMachine (eTypeConnection eTC);
        int Run ();
        int DownLoad (CString strProgram);
        int DownLoadParams (CString strParams, CString strHWParams, CString strUserStrings);
        int DownLoadParGen (int nRate, int nFrequency, double fpPrecision, double fpContoureAngle, double fpEpsilonRaggilong);
		int DownLoadModuleParams (CAdamoConfigurazioneGenerale *pModParams);
        int DownLoadUserStrings (CString strStrings);
        int DownLoadCC (CString strCC, CString strName, CString strParams);
        int DownLoadPC (CString strCC, CString strName);
        int DownLoadRS ();
        int UpLoadPC (CString strCC, CString strName);
        int DownLoadTemporary (CString strTemporary, CString strName);
        int DownLoadResources (CString strResources);
        int DownLoadGProgram (CString strGProgram, int nGSlot, int nGType);
		int DownLoadGFrameProgramByNumber (CString strGProgram, int nGNumber, int nGFrameLines);
        int Start ();
        int Stop ();    
        int SendDebugData (const char* strModuleName, CMap<int, int, BOOL, BOOL>& bp);
        int StartDebugging (int nCmd);
        int StartDebugging (int nCmd, int nParameter, int nModule, const char* szModule);
        int StopDebugging ();
        void ClearDebugData ();
        int GetDebugData (CCmdGetDebugData* pDD);
        void InitDebugData (const char* szSlot);
        int IsCncRunning ();
        int IsCncRunningReq ();
        int IsCncOnline ();
        LPUNKNOWN GetCommObj()  { return m_objComm; }
        CAdamoSink* GetSink ()  { return m_pSink; }
        bool IsOpened ()        { return m_bOpened; }
		void SetParentWnd (CWnd *pWnd)	{ m_pWnd = pWnd; }
        void SendDebugCmd (int nCmd, int nParameter, int nModule, const char* szModule);
        int  GetStackData (CPtrList& l);
        int  GetLocalData (CPtrList& l, int nLevel);
        int  GetGlobalData (int nLevel, const char* szExpression, CCmdGetGlobalData* gd);
        int  GetTaskData (int nSlotIni, int nSlotFin, stTaskData* pTaskData);
        int  SlotDelete (stTaskData* pTaskData);
        int  SlotSetStatus (stTaskData* pTaskData);
        int  GetVariable (CString szVar, COleVariant& v);
        int  SetVariable (CString szVar, COleVariant& v);
        int  GetLocalVariable (CString szVar, int nLevel, int nProg, COleVariant& v);
        int  SetLocalVariable (CString szVar, int nLevel, int nProg, COleVariant& v);
        int  GetExpressionVariable (CString szExpression, int nLevel, COleVariant& v);
        int  SetExpressionVariable (CString szExpression, int nLevel, COleVariant& v);
        int  GetMatrixElement (CString szVar, int nIni, CString strCol, CAdamoData* pData);
        int  GetArray (CString szVar, int nIni, int nFin, CAdamoData* pData);
        int  SetArray (CString szVar, int nIni, int nFin, CAdamoData* pData);
        int  GetMatrix (CString szVar, int nRow, int nNumColumns, int nBufKeys, char* pBufKeys, CAdamoData* pData);
        int  SetMatrix (CString szVar, int nRow, int nNumColumns, int nBufKeys, char* pBufKeys, CAdamoData* pData);
        int  SetDevice (int n, CDeviceData* pData);
        int  GetDevice (int n, CDeviceData* pData);
        int  GetAxes   (int n, CDeviceData* pAxes, stDatiAsse* pData);
		int  GetGAxes (int nSlot, int n, CDeviceData* pAxes, stGSlotPosition* pData);
        int  GetAdamoAxes (CDeviceData* pAxes, CAdamoParametri* pAP, CAdamoStatoAsse* aSA);
        int  GotoPosition (int nAsse, double fpQuote, double fpVel, enum enTypeJog TypeJog);
        int  GotoPositionInc (int nAsse, double fpQuote, double fpVel, enum enTypeJog TypeJog);
        int  GotoLinear (int nAsse, double fpQuote, double fpVel, enTypeJog TypeJog);
        int  GotoLinearInc (int nAsse, double fpQuote, double fpVel, enTypeJog TypeJog);
        int  AbortMovement (int nAsse);
        int  OpenPositionLoop (int nAsse);
        int  ClosePositionLoop (int nAsse);
        int  OscStart (long nFrequency, long nBufferSize, long nAndOr, long nPretrigger, bool bStartImmediately);
        int  OscStop ();
        int  OscSendData (CVarData* pVarData, CDeviceData* pData);
        int  OscSendEvents (CDeviceData* pData, long nCmp, int nData, double fpData);
        int  OscGetData (long* nStatus, long* nRecords, char* pBuf, long nBufLength);
        int  OscGetStatus (long* nStatus, long* nError);
        int  GetParCnt (long* pCnt);
        void SetModuleNumber (int nModule)   { if (nModule>=0 && nModule < NMAXMACHINES) m_nModule=nModule; }
        int  GetModuleNumber ()              { return m_nModule; }
        bool ExistSymbol (CString szVar, int nLevel, int* pnGL, int* pnType, int* pnProg);
        int  GetSlotNumber (CString strName);
		int  ClearCCData ();
        int  StartCommTrace ();
        int  StopCommTrace ();
        bool IsCommTraceEnabled ();
        int  GetNumCards (int *nCards);
        int  GetCardInfo (int nCard, CCmdGetCardInfo* pCI);
        int  GetGlobalTableKeys (CString strName, CString strPath, CAdamoValori& varKey, CAdamoValori* pResult, int* pValueType, int& nElements, bool& GheNeAmo);
        int  GetLocalTableKeys  (CString strName, CString strPath, CAdamoValori& varKey, int nLevel, int nProg, CAdamoValori* pResult, int* pValueType, int& nElements, bool& GheNeAmo);
        int  GetGlobalTableValues (CString strName, CString strPath, CAdamoValori& varKey, CAdamoData* pResult);
        int  GetLocalTableValues  (CString strName, CString strPath, CAdamoValori& varKey, int nLevel, int nProg, CAdamoData* pResult);
        int  SetGlobalTableValues (CString strName, CString strPath, CAdamoValori& varKey, CAdamoData& data);
        int  SetLocalTableValues (CString strName, CString strPath, CAdamoValori& varKey, int nLevel, int nProg, CAdamoData& data);
        int  StartCanScan ();
        int  SetCanOperatingMode (long nNode, long nMode);
        int  SetCanInitialization ();
        int  GetScanCanStatus ();
        int  GetCanNodeData (CCmdGetDeviceInfo *pDeviceInfo);
        int  ChangeLanguage ();
        int  StartSDOCommand (int nRequests, stSDORequest *pSDORequests);
        int  GetSDOStatus ();
        int  GetSDOData (int *nRequests, stSDOResults *pSDOResults);
        int  GetDebugInfo (CAdamoDebugInfo *pDI);
        int  SetDebugInfo (int nDebugParameter);
        int  UploadSSCNETParameters (int nCard, int nSSCNETType, int nStation, WORD *pwParameters);
        int  DownloadSSCNETParameters (int nCard, int nStation, int nNumParams, WORD *pwParameters);
        int  StartGProgram (int nGSlot);
        int  StopGProgram (int nGSlot);
        int  AbortGProgram (int nGSlot);
        int  GetGLineInfo (int nGSlot, VARIANT *pInfo);
        int  GetGSlotInfo (int nGSlot, VARIANT *pInfo);
		int  ClearGProgram (int nGSlot);
        int  GetSSCNETStatus (int nCard, int nStation);
		void GetSSCNETDisplay (int nCard, int nStation, DWORD *dwStatus, DWORD *dwDisplay);
        int  GetSSCNETStations (int nCard);
        int  GetSSCNETAlarm (int nCard, int nStation, WORD &wAlarm, CString &strAlarm);
		int  GetEthernetAdapters (CCmdGetAdapters *pAdapters);
        int  SSCNETReadyOn      (int nCard, int nStation, bool bOn);
        int  SSCNETServoOn      (int nCard, int nStation, bool bOn);
        int  SSCNETResetAlarm   (int nCard, int nStation);
        int  ConvertSSCNETAbsPos (int nCard, long nJ3J4);
        int  GetPswLevel ();
        int  WriteAbsEncoder (int nCard, int nStation, WORD wMultiRevCounter, DWORD dwOneRevPos);
        int  ReadAbsEncoder (int nCard, int nStation, WORD *wMultiRevCounter, DWORD *dwOneRevPos);
        int  SetAbsPosition (int nCard, int nStation);
        int  InitSSCNET (int nCard);
        int  RunSlot (CString strSlot);
		int  StartEcMailBoxCommand (int nType, int nSlave, int nIndex, int nSubIndex, int nLen, BYTE *pData);
		int  GetEcMailBoxStatus ();
		int  GetEcMailBoxData (LONG *lResult, int *nLen, BYTE *pData);
		int  RequestState (int nRequestState, int nSlave);
		int  GetInitializationStatus (int *nInitTest, int *nInitStatus);
		int  GetEcInfo (VARIANT *pInfo);
        int  EcReadyOn    (int nSlave, bool bOn);
        int  EcServoOn    (int nSlave, bool bOn);
        int  EcResetAlarm  (int nSlave);
		int  StartGDebugging (int nGSlot);
		int  StopGDebugging (int nGSlot);
        int  SendGDebugData (int nGSlot, int *anb, int nb);
		int  SendGDebugCmd (int nGSlot, int nCmd, int nParameter);
		int  GetGDebugVariables (int nGSlot, int nGVarsIndex, int *nGVars, CCmdGVariables* pGVars);
		int  SetGDebugVariable (int nGSlot, CString strGVar, VARIANT v);
        void SetTcpIpAddress (BYTE *achTcpIpAddress);
		BYTE *GetTcpIpAddress ();
		void SetPort (int nPort);
		int  GetPort ();
		void SetTypeModule (eTypeModule eTM);
		eTypeModule GetTypeModule ();
		void SetHWModuleNumber (int nHWModuleNumber);
		int  GetHWModuleNumber ();
		int  ForceInput (int nInput, bool bForced);
		int  OpenLogFile (int nMonth);
		int  CloseLogFile ();
		int  ReadLogFile (int nRecordDa, int nNumeroDirecord, BYTE *pAdamoErrore);
		eTypeConnection GetConnectionType ()	{ return m_nConnType; }
		int  GetObjectClass (CString szInstanceName, CString szPath, int nLevel, int prog, CString& szClassName);
		int  GetBufferLenght (int nCmd, CString strParam);
		void GetBufferData (BYTE* pBuffer, int nBufferLenght);
        int  EnablePing ();
        int  DisablePing ();
	protected :
        void CreateSink ();
        void CloseSink ();
		virtual void InitCmd ()   {
		}
		virtual void EndCmd ()   {
		}
    private :
        void AssignVarKey (CAdamoValori& varKey, _variant_t& t);
        void AssignVarData (CAdamoData& varKey, _variant_t& t);
};

#pragma pack (push, 1)
struct stCfgExtendedData   {
	bool		 m_bSpikeDetector;
	bool		 m_bMaxUTurnPrecision;
	enSmootherType m_eSmootherType;
	double		 m_fpSingleSpikeSmootherWeight;
	double		 m_fpSingleSpikeFactor;
	double		 m_fpGeometrySpikeFactor;
	bool		 m_bDetectJointUTurn;
	bool		 m_bStopOnAngle;
	double       m_fpMinStopAngle;
	bool		 m_bSimulationMode;
	int		     m_nMaxJitter;
	double       m_fpCountourAngle;
	bool         m_bCentroIncrementale;
	double       m_fpSIP;
	double		 m_afpGeometry_XYZCB[NMAX_CORR_XYZCB];
	double		 m_afpGeometry_TRT[NMAX_CORR_TRT];
	char		 m_spare[0xff97];	
};
#pragma pack (pop)

class CAdamoCfgMachine : public CAdamoMachine
{
    public :
        CAdamoCfgMachine ();
        CAdamoCfgMachine (CObject *pParent);
        virtual ~CAdamoCfgMachine ();
        void Serialize(CArchive &ar);
		void SaveConfigXML (stAdamoParametri *pAdamoParametri, CStdioFile *pStdioFile);
		void SetAdamoModule (CAdamoLogicModule *pModule) { m_pModule = pModule; }
        CAdamoLogicModule* GetAdamoModule(){ return m_pModule; };
        CObject* GetItem(CString strPath){ return m_pModule->GetItem(strPath); };
        CObject* GetItem(enDispositivi eDev, int nID){ return m_pModule->GetItem(eDev, nID); };
        void AddItem(CString strNodePath,CObject *pObj);
        void RemoveItem(CObject *pObj);
        bool IsModified ()   { return m_bModified; }
        void SetModifiedFlag (bool bModified) { m_bModified = bModified; };
        void SetMachineType (eTypeMachine eTypeMachine);
        eTypeMachine   GetMachineType () { return m_eTypeMachine; }
        int  GetFrequency ()    { return m_nFrequency; }
        void SetFrequency (int nFrequency);
        short  GetRate ()            { return m_nRate; }
        void   SetRate (short rate)  { m_nRate=rate; }
        double GetPrecision ()                     { return m_fpPrecision; }
        void   SetPrecision (double fpPrecision)   { m_fpPrecision=fpPrecision; }
        double GetEpsilonRaggi ()                  { return m_fpEpsilonRaggi; }
        void   SetEpsilonRaggi (double fpEpsilonRaggi){ m_fpEpsilonRaggi=fpEpsilonRaggi; }
        double GetContoureAngle ()                 { return m_fpContoureAngle; }
        void   SetContureAngle (double fpContoureAngle)   { m_fpContoureAngle=fpContoureAngle; }
        void   GestisciMappaEId (CString strParamsPathName, bool bLoad=true);
        void   CaricaDescrizioneTradotta (CTraduzioni* pTr);
        int    SalvaDescrizioneTradotta (CTraduzioni* pTr);
        bool   IsLogicModuleEmpty ()               { return m_pModule->IsLogicModuleEmpty (); }
        void   GestisciVersione ()                 { if (m_pModule->GestisciVersione ()) SetModifiedFlag (true); }
        void   DeleteConfigData ();
        void   NewModuleObject (CObject *pParent);
        void   SetTraduzioniObj (CTraduzioni* tr)  { m_tr=tr; }
        void   SetMessageFilteringOnStart (int nMessageFilteringOnStart)    { m_lMessageFilteringOnStart=nMessageFilteringOnStart; }
        int    GetMessageFilteringOnStart ()                                { return m_lMessageFilteringOnStart; }
        void   SetStopAxisOnError (int lStopAxisOnError)                    { m_lStopAxisOnError=lStopAxisOnError; }
        int    GetStopAxisOnError ()                                        { return m_lStopAxisOnError; }
		void   SetPrefetchSplineEnabled (long lPrefetchEnabled)				{ m_lPrefetchEnabled = lPrefetchEnabled; }
		int    GetPrefetchSplineEnabled ()									{ return m_lPrefetchEnabled; }
		void   SetSensoRotazioneCircolari (long lSensoRotazioneCircolari)   { m_lSensoRotazioneCircolari = lSensoRotazioneCircolari; }
		int    GetSensoRotazioneCircolari ()								{ return m_lSensoRotazioneCircolari; }
		void   SetKAC (double fpKAC)										{ m_fpKAC = fpKAC; }
		double GetKAC ()													{ return m_fpKAC; }
		void   SetSmoothing (double fpSmoothing)							{ m_fpSmoothing = fpSmoothing; }
		double GetSmoothing ()												{ return m_fpSmoothing; }
		void   SetBowThickening (double fpBowThickening)					{ m_fpBowThickening = fpBowThickening; }
		double GetBowThickening ()											{ return m_fpBowThickening; }
		void   SetLinearThickening (double fpLinearThickening)				{ m_fpLinearThickening = fpLinearThickening; }
		double GetLinearThickening ()										{ return m_fpLinearThickening; }
		void   SetVelProfile (double fpVelProfile)							{ m_fpVelProfile = fpVelProfile; }
		double GetVelProfile ()												{ return m_fpVelProfile; }
		void   SetAccProfile (long dwAccProfile)							{ m_dwAccProfile = dwAccProfile; }
		long   GetAccProfile ()												{ return m_dwAccProfile; }
		void   SetDecProfile (long dwDecProfile)							{ m_dwDecProfile = dwDecProfile; }
		long   GetDecProfile ()												{ return m_dwDecProfile; }
		void   SetAInterpDefault (int nAInterpDefault)						{ m_nAInterpDefault =  nAInterpDefault; }
		int    GetAInterpDefault ()											{ return m_nAInterpDefault; }
		double GetSingleSpikeFactor ()										{ return m_fpSingleSpikeFactor; }
		void   SetSingleSpikeFactor (double SSF)							{ m_fpSingleSpikeFactor = SSF; }
		double GetGeometrySpikeFactor ()									{ return m_fpGeometrySpikeFactor; }
		void   SetGeometrySpikeFactor (double MSF)							{ m_fpGeometrySpikeFactor = MSF; }
		bool   GetSpikeDetector ()											{ return m_bSpikeDetector; }
		void   SetSpikeDetector (bool SpikeDetector)						{ m_bSpikeDetector = SpikeDetector; }
		bool   GetMaxUTurnPrecision ()										{ return m_bMaxUTurnPrecision; }
		void   SetMaxUTurnPrecision (bool MaxUTurnPrecision)				{ m_bMaxUTurnPrecision = MaxUTurnPrecision; }
		enSmootherType GetSmootherType ()									{ return m_eSmootherType; }
		void   SetSmootherType (enSmootherType SmootherType)				{ m_eSmootherType = SmootherType; }
		double GetSingleSpikeSmootherWeight ()								{ return m_fpSingleSpikeSmootherWeight; }
		void   SetSingleSpikeSmootherWeight (double SingleSpikeSmootherWeight)	{ m_fpSingleSpikeSmootherWeight = SingleSpikeSmootherWeight; }
		bool   GetDetectJointUTurn ()										{ return m_bDetectJointUTurn; }
		void   SetDetectJointUTurn (bool DetectJointUTurn)					{ m_bDetectJointUTurn = DetectJointUTurn; }
		bool   GetStopOnAngle ()											{ return m_bStopOnAngle; }
		void   SetStopOnAngle (bool StopOnAngle)							{ m_bStopOnAngle = StopOnAngle; }
		double GetMinStopAngle ()											{ return m_fpMinStopAngle; }
		void   SetMinStopAngle (double MinStopAngle)						{ m_fpMinStopAngle = MinStopAngle; }
		int    GetRamDisk ()												{ return m_lRamDisk; }
		void   SetRamDisk (bool nEnable)									{ m_lRamDisk = nEnable; }
		int    GetRamDiskBufferSize ()										{ return m_lRamDiskBufferSize; }
		void   SetRamDiskBufferSize (int lRamDiskBufferSize)				{ m_lRamDiskBufferSize = lRamDiskBufferSize; }
		int    GetG0Raggiata ()												{ return m_lG0Raggiata; }
		void   SetG0Raggiata (bool bG0Raggiata)								{ m_lG0Raggiata = bG0Raggiata; }
		double GetG0RaggiataInfittimento ()									{ return m_fpG0RaggiataInfittimento; }
		void   SetG0RaggiataInfittimento (double fpG0RaggiataInfittimento)	{ m_fpG0RaggiataInfittimento = fpG0RaggiataInfittimento; }
		double GetG0RaggiataRaggio ()										{ return m_fpG0RaggiataRaggio; }
		void   SetG0RaggiataRaggio (double fpG0RaggiataRaggio)				{ m_fpG0RaggiataRaggio = fpG0RaggiataRaggio; }
		bool   GetSimulationMode ()											{ return m_bSimulationMode; }
		void   SetSimulationMode (bool bSimulationMode)						{ m_bSimulationMode = bSimulationMode; }
		long   GetMaxJitter ()												{ return m_lMaxJitter; }
		void   SetMaxJitter (int lMaxJitter)								{ m_lMaxJitter = lMaxJitter; }
		bool   GetCentroIncrementale ()										{ return m_bCentroIncrementale; }
		void   SetCentroIncrementale (bool bCentroIncrementale)				{ m_bCentroIncrementale = bCentroIncrementale; }
		int    GetPlcBoost ()												{ return m_nPlcBoost; }
		void   SetPlcBoost (int nPlcBoost)									{ m_nPlcBoost = nPlcBoost; }
		double GetSIP ()													{ return m_fpSIP; }
		void   SetSIP (double SIP)											{ m_fpSIP = SIP; }
		void   GetXYZCBCompensation (double* afpC)							{ memcpy (afpC, m_afpGeometry_XYZCB, sizeof (m_afpGeometry_XYZCB)); }
		void   SetXYZCBCompensation (double* afpC)							{ memcpy (m_afpGeometry_XYZCB, afpC, sizeof (m_afpGeometry_XYZCB)); }
		void   GetTRTCompensation (double* afpC)							{ memcpy (afpC, m_afpGeometry_TRT, sizeof (m_afpGeometry_TRT)); }
		void   SetTRTCompensation (double* afpC)							{ memcpy (m_afpGeometry_TRT, afpC, sizeof (m_afpGeometry_TRT)); }
		void   SetAdamoOptions (CAdamoOptions* pOpt)						{ m_pOpt = pOpt; }
    	bool   GetStrictAbsPosControl ()									{ return m_bStrictAbsPosControl; }
		void   SetStrictAbsPosControl (bool bSAPC)							{ m_bStrictAbsPosControl = bSAPC; }
		int    GetStrictAbsPosEncoderSteps ()								{ return m_nStrictAbsPosEncoderSteps; }
		void   SetStrictAbsPosEncoderSteps (int nSAPES)						{ m_nStrictAbsPosEncoderSteps = nSAPES; }
protected:
        CAdamoLogicModule *m_pModule;
    private :
        void RefreshAdamoDatiFile (CString strParamsPathName);
		void WritePrologue (CStdioFile *pStdioFile);
		void WriteEpilogue (CStdioFile *pStdioFile);
		void SaveExtendedData (CArchive &ar);
		void LoadExtendedData (CArchive &ar);
		CString GetExtCfgPathName (CArchive &ar);
    private :
        bool m_bModified;
        eTypeMachine m_eTypeMachine;
        eTypeMachine m_eNativeTypeMachine;
        int          m_nFrequency;
        int          m_nNativeFrequency;
        double       m_fpPrecision;
        double       m_fpContoureAngle;
        short        m_nRate;
        double       m_fpEpsilonRaggi;
        long         m_lMessageFilteringOnStart;
        long         m_lStopAxisOnError;
        long         m_lPrefetchEnabled;
		long		 m_lSensoRotazioneCircolari;
		double		 m_fpKAC;
		double       m_fpSmoothing;
		double       m_fpBowThickening;
		double       m_fpLinearThickening;
		double		 m_fpVelProfile;
		long		 m_dwAccProfile;
		long		 m_dwDecProfile;
		int 		 m_nAInterpDefault;
		bool		 m_bSpikeDetector;
		bool		 m_bMaxUTurnPrecision;
		enSmootherType m_eSmootherType;
		double		 m_fpSingleSpikeSmootherWeight;
		double		 m_fpSingleSpikeFactor;
		double		 m_fpGeometrySpikeFactor;
		bool         m_bDetectJointUTurn;
		bool		 m_bStopOnAngle;
		double       m_fpMinStopAngle;
		long		 m_lRamDisk;
		long		 m_lRamDiskBufferSize;
		long		 m_lG0Raggiata;
		double		 m_fpG0RaggiataInfittimento;
		double		 m_fpG0RaggiataRaggio;
		bool		 m_bSimulationMode;
		long		 m_lMaxJitter;
		bool         m_bCentroIncrementale;
		int			 m_nPlcBoost;
		bool		 m_bStrictAbsPosControl;
		int		     m_nStrictAbsPosEncoderSteps;
		bool		 m_bLoadVirtualMachine;
		double		 m_fpSIP;
		double		 m_afpGeometry_XYZCB[NMAX_CORR_XYZCB];
		double		 m_afpGeometry_TRT[NMAX_CORR_TRT];
        CTraduzioni* m_tr;
		CAdamoOptions* m_pOpt;
};

class CAdamoVMachine : public CAdamoCfgMachine
{
    public :
        CAdamoVMachine () {};
        CAdamoVMachine (CObject *pParent);
        int OpenMachine (const char* szMachinePath, int nMachineType = -1, eTypeConnection eTC = enLocalConnection);
        int CloseMachine ();
        int ReplaceMachine ();
        int ReconnectMachine (eTypeConnection eTC = enLocalConnection);
		CPtrList &GetAxesList ()   { return m_listAxes; }
	private :
		CPtrList m_listAxes;
	private :
		int  CreateAxesList ();
		void Iterator(CAdamoGroup *pGrp);
		void InsertAx (CAdamoAxis *pDev);
		void DeleteListAxes ();
        int  OpenMachineInternal  (const char* szMachinePath, int nMachineType, eTypeConnection nConnectionType);
        int  CloseMachineInternal ();
	protected :
        void InitCmd ();
        void EndCmd ();
};

#endif
