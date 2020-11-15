/*
**
*/

#ifndef __ADAMO__DATI__
#define __ADAMO__DATI__

#include "devicedefine.h"
#include "AdamoIdeEthercat.h"

/* generico dispositivo CAN di configurazione */
struct stAdamoDevice
{
    char szNome[MAXPATH];
	char szDescription[MAXDESCLENGTH];
};

struct stAdamoHWDevice : stAdamoDevice
{
	char szHWAddress[MAXHWLENGTH];
};

/* input digitale */
struct stAdamoInputDig : stAdamoHWDevice
{
    int  nNumInputDig;
    char szSpare[32];
};

/* output digitale */
struct stAdamoOutputDig : stAdamoHWDevice
{
    int  nType;
    int  nNumOutputDig;
    int  nTimeOut;
    char szSpare[32];
};

/* input analogico */
struct stAdamoInputAnag : stAdamoHWDevice
{
    double fpLLInf;
    double fpLLSup;
    char szSpare[32];
};

/* output analogico */
struct stAdamoOutputAnag : stAdamoHWDevice
{
    double fpLLInf;
    double fpLLSup;
    char szSpare[32];
};

/* contatore */
struct stAdamoCounter : stAdamoDevice
{
    int  nType;
    char szSpare[32];
};

/* timer */
struct stAdamoTimer	: stAdamoDevice
{
	double nPeriod;
	int    bEnabled;
    int    nUpDown;
    int    nStored;
    char   szSpare[32];
};

/* asse analogico */
struct stAdamoAsse : stAdamoHWDevice
{
    char    szAlias[16];
    char    szGName[2];
    double  fpResolution;
    int     nAxisType;
    int     nAxisCategory;
    int     nUnit;
    int     nInvPhase;
    int     nInvRif;
    int     nEnableNick;
    int     nEnableJerk;
    double  fpSpeed;
    double  fpSpeedMan;
    double  fpAcc;
    double  fpDec;
    double  fpProp;
    double  fpInt;
    double  fpDer;
    double  fpFeedForward;
    double  fpFeedForwardAcc;
    double  fpRif;
    int     nRampType;
    double  fpInterSpeed;
    double  fpInterAcc;
    double  fpInterDec;
    double  fpInterProp;
    double  fpInterInt;
    double  fpInterDer;
    double  fpInterFeedForward;
    double  fpInterFeedForwardAcc;
    int     nInterRampType;
    double  fpManualVel;
    int     nWaitStopAxis;
    double  fpLimEAConc;
    double  fpLimEADisc;
    double  fpLimEAMin;
    int     nEnableLimAxesPos;
    int     nEnableLimAxesNeg;
    double  fpLimAxisPos;
    double  fpLimAxisNeg;
    double  fpWinPos;
    double  fpWinNeg;
    int     nServoLimitType;
    double  fpDelta;
    int     nAbilitazioneCorrettori;
    int     nNumAssiCorretti;
    int     anAssiCorretti[6];
    struct  stCustomData stCD[MAX_AXIS_CUSTOM];
	double  fpPercentualeRampaTrapezoidale;
	double  fpInterPercentualeRampaTrapezoidale;
	int     nDimBufferIntegrale;
    int     nAsseGantry;
    double  nPropGantry;
    double  nIntGantry;
    double  nDerGantry;
    int     nGSlot;
    int     nControlType;
    int     nMovementCategory;
	int		nArrestoSuCambioAccDec;
	int		nAbilitazioneCompensazioneGioco;
	double  fpCompensazioneGioco;
	double  fpTempoDiCompensazione;
	double  fpRangeQuotaPos;
	double  fpRangeQuotaNeg;
	double  fpFiltroRotanti;
    int     nTempoArrivoInQuota;
    int     nTempoAsseFermo;
	double  fpCostanteDiTempoAzionamento;
	double  fpVelocitaDiCorrezione;
	bool    bCorrezioneDinamica;
	int     nAbilitaCompensazioneEA;
	double  fpCompensazioneEA;
	double  fpCompensazioneEAA;
	double  fpIntegrativaKT;
	int     nEnableSwitchedIntegral;
	double  fpSwitchedIngetegralSpeed;
	double  fpDerivativeN;
	double  fpInterIntegrativaKT;
	int     nInterEnableSwitchedIntegral;
	double  fpInterSwitchedIngetegralSpeed;
	double  fpInterDerivativeN;
	WORD    wFiltroCollegamentoEncoder;
	WORD    wFiltroServoError;
	char    szSpare[852];
};

/* Encoder */
struct stAdamoEncoder : stAdamoHWDevice
{
    double  fpResolution;
    int     nEncoderCategory;
    int     nUnit;
    int     nInvPhase;
    int     nEnableLimAxesPos;
    int     nEnableLimAxesNeg;
    double  fpLimAxisPos;
    double  fpLimAxisNeg;
    struct  stCustomData stCD[MAX_AXIS_CUSTOM];
    char    szSpare[1024];
};

/* generico dispositivo CAN */
struct stAdamoCanDevice : stAdamoHWDevice
{
    char szSpare[32];
};

/* struttura di memorizzazione parametri */
struct stAdamoParametri
{
    stAdamoInputDig     AdamoInputDig[NMAXINPDIG];
    stAdamoOutputDig    AdamoOutputDig[NMAXOUTDIG];
    stAdamoInputAnag    AdamoInputAnag[NMAXINPANA];
    stAdamoOutputAnag   AdamoOutputAnag[NMAXOUTANA];
    stAdamoCounter      AdamoCounter[NMAXCOUNTER];
    stAdamoTimer        AdamoTimer[NMAXTIMER];
    stAdamoAsse         AdamoAsse[NMAXAXIS];
    stAdamoEncoder      AdamoEncoder[NMAXENCODERS];
    stAdamoCanDevice    AdamoCanDevice[NMAXGENERICCAN];
};

#pragma pack (push, 1)

struct stHWCanOpenNode
{
    double afpLLIAInf[12];
    double afpLLIASup[12];
    double afpLLOAInf[12];
    double afpLLOASup[12];
    WORD   wTPDOType[4];
    DWORD  dwNodeGuarding;
    DWORD  dwHeartBeatTime;
    bool   bIsVirtual;
    bool   bIsDisabled;
    bool   bIsPolling;
	bool   bIsDSP402Node;
	int    nStatusWord[2];
	int    nControlWord[2];
	int    nPositionActualValue[2];
	int    nTargetPosition[2];
	int    nSpeedActualValue[2];
	int    nTargetSpeed[2];
	int    nTorqueActualValue[2];
	int    nTargetTorque[2];
	int    nImageTypeInput;
	int    nImageTypeOutput;
    char achSpare[947];
};

struct stHWCanOpenNet
{
    int m_nBaudRate;
    int m_nSyncFrequency;
    int m_nSDOTimeout;
    int m_nGuardTime;
    int m_nLifeTime;
	int m_nEnableWarning;
    char chCanSpare[1020];
    stHWCanOpenNode m_HWCanOpenNode[NMAX_CANNODES];
};

struct stHWSSCNETStation   {
    bool m_bAbsoluteEncoder;
    double m_fpHomePosition;
};

struct stHWSSCNETNet
{
    int  m_nFrequency;
    bool m_bEnableWarnings;
    stHWSSCNETStation m_hWSSCNETStation[NMAXMGASTATION];
    BYTE m_chFree[10240];
	int  m_bNetworkEnabled;
    int  m_bNetworkType;
    char chSSCNETSpare[1016];
};

struct stHWSSCNETAbsPos
{
    DWORD dwOPOneRevPos;
    WORD  wOPMultiRevCounter;
};

struct stHWEthAnalogLimit
{
	short int    nHWAddress;
	short int    nType;
    double afpLLIAInf;
    double afpLLIASup;
};

struct stHWEthAnalog
{
	stHWEthAnalogLimit AnalogInput;
	stHWEthAnalogLimit AnalogOutput;
};

struct stHWEthSlave
{
	int nDeviceType[MAX_ETHERCAT_SLAVE_TYPE];
	stHWEthVarMask EnableVariables[MAX_ETHERCAT_SLAVE_TYPE];
	stHWEthAnalog EthAnalogDevice[MAX_ETHERCAT_ANALOG_DEVICE];
};

struct stHWEthAxData  {
	bool m_bConfigured;
	int m_nType;
	int m_nOffset;
	int m_nLength;
};

struct stHWEthAxInfo 
{
	stHWEthAxData m_inpStatus;
	stHWEthAxData m_inpControl;
	stHWEthAxData m_outStatus;
	stHWEthAxData m_outControl;
};

struct stHWEthercatNet
{
	char strMasterXML[_MAX_FNAME];
	int  nSlave;
	stHWEthAxInfo	HWEthAxis[NMAXAXIS];
	stHWEthSlave	HWEthSlave[MAX_ETHERCAT_SLAVE];
	int             m_nRequestedState;
	int				m_nFrequency;
	int				m_bContinuosMeasuring;
	__int64			m_nStartDCTiming;
};

struct stHWEthercatNet2
{
	char strHpeDriver[_MAX_FNAME];
	int				m_nOffsetDCTiming;
};

struct stSSCNETConf
{
    BYTE m_achSSCNETConf[NMAXMGASTATION / 2];
};

struct stSSCNETConfPar
{
    WORD m_awParameters[NMAXMGASTATION][NMAXMGA030PARAMETERSTYPES][NMAXMGA030PARAMETERS];
};

struct stHWAdamoConf   {
    stHWCanOpenNet		m_HWCanOpenNet[NMAX_SCHEDE];
    stHWSSCNETNet		m_HWSSCNETNet[NMAX_SCHEDE];
    stHWSSCNETAbsPos    m_HWSSCNETAbsPos[NMAXMGASTATION];
	stHWEthercatNet		m_HWEthercatNet;
    stSSCNETConf        m_HWSSCNETConf[NMAX_SCHEDE];
    stSSCNETConfPar     m_HWSSCNETParameters[NMAX_SCHEDE];
    stHWSSCNETAbsPos    m_HWSSCNETAbsPosMS[NMAX_SCHEDE][NMAXMGASTATION];
	stHWEthercatNet2    m_HWEthercatNet2;
};

#pragma pack (pop)

/* generico dispositivo CAN di configurazione */
struct stTxAdamoDevice
{
    bool bIsConfigured;
    char szNome[MAXPATH];
	char szDescrizione[MAXDESCLENGTH];
};

struct stTxAdamoHWDevice : stTxAdamoDevice
{
	char szHWAddress[MAXHWLENGTH];
};


/* input digitale */
struct stTxAdamoInputDig : stTxAdamoHWDevice
{
    int  nNumInputDig;
    char szSpare[32];
};

/* output digitale */
struct stTxAdamoOutputDig : stTxAdamoHWDevice
{
    int  nType;
    int  nNumOutputDig;
    int  nTimeOut;
    char szSpare[32];
};

/* input analogico */
struct stTxAdamoInputAnag : stTxAdamoHWDevice
{
    double fpLLInf;
    double fpLLSup;
    char szSpare[32];
};

/* output analogico */
struct stTxAdamoOutputAnag : stTxAdamoHWDevice
{
    double fpLLInf;
    double fpLLSup;
    char szSpare[32];
};

/* generico dispositivo CAN */
struct stTxAdamoCanDevice : stTxAdamoHWDevice
{
    char szSpare[32];
};

/* contatore */
struct stTxAdamoCounter	: stTxAdamoDevice
{
    int  nType;
    char szSpare[32];
};

/* timer */
struct stTxAdamoTimer : stTxAdamoDevice
{
	int  nPeriod;
	int  bEnabled;
	int  nUpDown;
	int  nStored;
    char szSpare[32];
};

/* asse analogico */
struct stTxAdamoAsse : stTxAdamoHWDevice
{
    char    szAlias[16];
    char    szGName[2];
    double  fpResolution;
    int     nAxisType;
    int     nAxisCategory;
    int     nUnit;
    int     nInvPhase;
    int     nInvRif;
    int     nEnableNick;
    int     nEnableJerk;
    double  fpSpeed;
    double  fpSpeedMan;
    double  fpAcc;
    double  fpDec;
    double  fpProp;
    double  fpInt;
    double  fpDer;
    double  fpFeedForward;
    double  fpFeedForwardAcc;
    double  fpRif;
    int     nRampType;
    double  fpInterSpeed;
    double  fpInterAcc;
    double  fpInterDec;
    double  fpInterProp;
    double  fpInterInt;
    double  fpInterDer;
    double  fpInterFeedForward;
    double  fpInterFeedForwardAcc;
    int     nInterRampType;
    double  fpManualVel;
    int     nWaitStopAxis;
    double  fpLimEAConc;
    double  fpLimEADisc;
    double  fpLimEAMin;
    int     nEnableLimAxesPos;
    int     nEnableLimAxesNeg;
    double  fpLimAxisPos;
    double  fpLimAxisNeg;
    double  fpWinPos;
    double  fpWinNeg;
    int     nServoLimitType;
    double  fpDelta;
    int     nAbilitazioneCorrettori;
    int     nNumAssiCorretti;
    int     anAssiCorretti[6];
    struct  stCustomData stCD[MAX_AXIS_CUSTOM];
	double  fpPercentualeRampaTrapezoidale;
	double  fpInterPercentualeRampaTrapezoidale;
	int     nDimBufferIntegrale;
    int     nAsseGantry;
    double  nPropGantry;
    double  nIntGantry;
    double  nDerGantry;
    int     nGSlot;
    int     nControlType;
	int     nArrestoSuCambioAccDec;
	int     nAbilitazioneCompensazioneGioco;
	double  fpCompensazioneGioco;
	double  fpTempoDiCompensazione;
	double  fpRangeQuotaPos;
	double  fpRangeQuotaNeg;
	double  fpFiltroRotanti;
    int     nTempoArrivoInQuota;
    int     nTempoAsseFermo;
    double  fpCostanteDiTempoAzionamento;
    double  fpVelocitaDiCorrezione;
	bool    bCorrezioneDinamica;
	int     nAbilitaCompensazioneEA;
	double  fpCompensazioneEA;
	double  fpCompensazioneEAA;
	double  fpIntegrativaKT;
	int     nEnableSwitchedIntegral;
	double  fpSwitchedIngetegralSpeed;
	double  fpDerivativeN;
	double  fpInterIntegrativaKT;
	int     nInterEnableSwitchedIntegral;
	double  fpInterSwitchedIngetegralSpeed;
	double  fpInterDerivativeN;
	WORD    wFiltroCollegamentoEncoder;
	WORD    wFiltroServoError;
    char    szSpare[756];
};

/* encoder */
struct stTxAdamoEncoder : stTxAdamoHWDevice   {
    double  fpResolution;
    int     nEncoderCategory;
    int     nUnit;
    int     nInvPhase;
    int     nEnableLimAxesPos;
    int     nEnableLimAxesNeg;
    double  fpLimAxisPos;
    double  fpLimAxisNeg;
    struct  stCustomData stCD[MAX_AXIS_CUSTOM];
    char    szSpare[1024];
};

/* struttura di memorizzazione parametri */
struct stTxAdamoParametri
{
    stTxAdamoInputDig     AdamoInputDig[NMAXINPDIG];
    stTxAdamoOutputDig    AdamoOutputDig[NMAXOUTDIG];
    stTxAdamoInputAnag    AdamoInputAnag[NMAXINPANA];
    stTxAdamoOutputAnag   AdamoOutputAnag[NMAXOUTANA];
    stTxAdamoCounter      AdamoCounter[NMAXCOUNTER];
    stTxAdamoTimer        AdamoTimer[NMAXTIMER];
    stTxAdamoAsse         AdamoAsse[NMAXAXIS];
    stTxAdamoEncoder      AdamoEncoder[NMAXENCODERS];
    stTxAdamoCanDevice    AdamoCanDevice[NMAXGENERICCAN];
};

struct stTxHWCanOpenNode
{
    double afpLLIAInf[12];
    double afpLLIASup[12];
    double afpLLOAInf[12];
    double afpLLOASup[12];
    WORD   wTPDOType[32];
    DWORD  dwNodeGuarding;
    DWORD  dwHeartBeatTime;
    bool   bIsVirtual;
    bool   bIsDisabled;
    bool   bIsPolling;
	bool   bIsDSP402Node;
	int    nStatusWord[2];
	int    nControlWord[2];
	int    nPositionActualValue[2];
	int    nTargetPosition[2];
	int    nSpeedActualValue[2];
	int    nTargetSpeed[2];
	int    nTorqueActualValue[2];
	int    nTargetTorque[2];
	int    nImageTypeInput;
	int    nImageTypeOutput;
};

struct stTxHWCanOpenNet
{
    int m_nBaudRate;
    int m_nSyncFrequency;
    int m_nSDOTimeout;
    int m_nGuardTime;
    int m_nLifeTime;
	int m_nEnableWarning;
    stTxHWCanOpenNode m_HWCanOpenNode[NMAX_CANNODES];
};

struct stTxHWSSCNETStation   {
    bool   m_bAbsoluteEncoder;
    double m_fpHomePosition;
    DWORD  m_dwOPOneRevPos;
    WORD   m_wOPMultiRevCounter;
};

struct stTxHWSSCNETNet
{
    int  m_nFrequency;
    bool m_bEnableWarnings;
	bool m_bNetworkEnabled;
    bool m_bNetworkType;
    stTxHWSSCNETStation m_hWSSCNETStation[NMAXMGASTATION];
    BYTE m_achNetworkConf[NMAXMGASTATION / 2];
};

struct stTxHWEthSlave
{
	stHWEthAnalog EthAnalogDevice[MAX_ETHERCAT_ANALOG_DEVICE];
};

struct stTxHWEthAxInfo 
{
	stHWEthAxData m_inpStatus;
	stHWEthAxData m_inpControl;
	stHWEthAxData m_outStatus;
	stHWEthAxData m_outControl;
};

struct stTxEthercatNet
{
	stTxHWEthAxInfo	HWEthAxis[NMAXAXIS];
	stTxHWEthSlave	HWEthSlave[MAX_ETHERCAT_SLAVE];
	int             m_nRequestedState;
	int				m_nFrequency;
	int				m_bContinuosMeasuring;
	__int64			m_nStartDCTiming;
	int  			m_nOffsetDCTiming;
};

struct stTxHWAdamoConf
{
    stTxHWCanOpenNet m_HWCanOpenNet[NMAX_SCHEDE];
    stTxHWSSCNETNet  m_HWSSCNETNet[NMAX_SCHEDE];
	stTxEthercatNet  m_HWEthercatNet;
	bool			 m_bSimulationMode;
};

#endif
