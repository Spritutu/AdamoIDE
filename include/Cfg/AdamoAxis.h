/*
** AdamoAxis.h
*/ 

#if !defined(AFX_ADAMOAXIS_H__10693EF9_E931_4BD4_B90F_D7D52F382EEA__INCLUDED_)
#define AFX_ADAMOAXIS_H__10693EF9_E931_4BD4_B90F_D7D52F382EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_AXIS_CORR           6

class CAdamoAxis : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoAxis)
public:
	CAdamoAxis();
    CAdamoAxis(CAdamoAxis& aa);
	virtual ~CAdamoAxis();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
    virtual enDispositivi GetType()    { return enAsse; }
    void SetResolution(double num)   { m_nResolution = num; }
    void SetAxisType(int num)   { m_nAxisType = num; }
    void SetAxesCategory(int num)   { m_nAxesCategory = num; }
    void SetUnit(int num)   { m_nUnit = num; }
    void SetInvPhase(int num)   { m_nInvPhase = num; }
    void SetInvRif(int num)   { m_nInvRif = num; }
    void SetEnableNick(int num)   { m_nEnableNick = num; }
    void SetEnableJerk (int num)   { m_nEnableJerk=num; }
    void SetSpeed(double num)   { m_nSpeed = num; }
    void SetSpeedMan(double num)   { m_nSpeedMan = num; }
    void SetAcc(double num)   { m_nAcc = num; }
    void SetDec(double num)   { m_nDec = num; }
    void SetProp(double num)   { m_nProp = num; }
    void SetInt(double num)   { m_nInt = num; }
    void SetDer(double num)   { m_nDer = num; }
    void SetFeedForward(double num)   { m_nFeedForward = num; }
    void SetFeedForwardAcc(double num)   { m_nFeedForwardAcc = num; }
	void SetIntKT (double num)   { m_fpIntKT = num; }
	void SetEnableSwitchedInt (int num)   { m_nEnableSwitchedInt = num; }
	void SetSwitchedIntSpeed (double num)	{ m_fpSwitchedIntSpeed = num; }
	void SetDerN (double num)	{ m_fpDerN = num; }
    void SetRif(double num)   { m_fpRifVoltage = num; }
    void SetRampType(int num)   { m_nRampType = num; }
    void SetInterSpeed(double num)   { m_nInterSpeed = num; }
    void SetInterAcc(double num)   { m_nInterAcc = num; }
    void SetInterDec(double num)   { m_nInterDec = num; }
    void SetInterProp(double num)   { m_nInterProp = num; }
    void SetInterInt(double num)   { m_nInterInt = num; }
    void SetInterDer(double num)   { m_nInterDer = num; }
    void SetInterFeedForward(double num)   { m_nInterFeedForward = num; }
    void SetInterFeedForwardAcc(double num)   { m_nInterFeedForwardAcc = num; }
	void SetInterIntKT (double num)   { m_fpInterIntKT = num; }
	void SetInterEnableSwitchedInt (int num)   { m_nInterEnableSwitchedInt = num; }
	void SetInterSwitchedIntSpeed (double num)	{ m_fpInterSwitchedIntSpeed = num; }
	void SetInterDerN (double num)	{ m_fpInterDerN = num; }
    void SetInterRampType(int num)   { m_nInterRampType = num; }
    void SetWaitStopAxis(int num)   { m_nWaitStopAxis = num; }
    void SetLimEAConc(double num)   { m_fpLimEAConc = num; }
    void SetLimEADisc(double num)   { m_fpLimEADisc = num; }
    void SetLimEAMin(double num)   { m_fpLimEAMin = num; }
    void SetEnableLimAxesPos(int num)   { m_nEnableLimAxesPos = num; }
    void SetEnableLimAxesNeg(int num)   { m_nEnableLimAxesNeg = num; }
    void SetLimAxisPos(double num)   { m_nLimAxisPos = num; }
    void SetLimAxisNeg(double num)   { m_nLimAxisNeg = num; }
    void SetServoLimitType (int nServoLimitType)   { m_nServoLimitType = nServoLimitType; }
    void SetWinPos (double num)   { m_fpWinPos = num; }
    void SetWinNeg(double num)   { m_fpWinNeg = num; }
    void SetAlias(CString str)   { m_strAlias = str; }
    void SetGName(CString str)   { m_szGName = str; }
    void SetGSlot (int nGSlot)   { m_nGSlot = nGSlot; }
    void SetGantryAxes (int nAxes)   { m_nAsseGantry=nAxes; }
    void SetPropGantry (double num)  { m_nPropGantry=num; }
    void SetDerGantry (double num)   { m_nDerGantry=num; }
    void SetIntGantry (double num)   { m_nIntGantry=num; }
    void SetDisplay (int nDisplay)   { m_nDisplay=nDisplay; }
    void SetPercTorque (int nTorque)   { m_nPercTorque=nTorque; }
    void SetTIC (int nTIC)   { m_nTIC=nTIC; }
    void SetMovementCategory (int nMT)   { m_nMT=nMT; }
	void SetStopOnAccDecChange (int nACAD)   { m_nACAD=nACAD; }
	void SetEnableRecuperoGioco (int nEnableRecuperoGioco)   { m_nEnableRecuperoGioco = nEnableRecuperoGioco; }
	void SetRecuperoGioco (double fpRecuperoGioco)  { m_fpRecuperoGioco=fpRecuperoGioco; }
	void SetTempoDiRecupero (double fpTempoDiRecupero)   { m_fpTempoDiRecupero=fpTempoDiRecupero; }
	void SetRangeQuotaNeg (double fpRangeQuotaNeg)  { m_fpRangeQuotaNeg=fpRangeQuotaNeg; }
    void SetRangeQuotaPos (double fpRangeQuotaPos)  { m_fpRangeQuotaPos=fpRangeQuotaPos; }
	void SetFiltroRotanti (double fpFiltroRotanti)  { m_fpFiltraRotanti = fpFiltroRotanti; }
	void SetTempoArrivoInQuota (int nTempoArrivoInQuota)  { m_nTempoArrivoInQuota = nTempoArrivoInQuota; }
	void SetTempoAsseFermo (int nTempoAsseFermo)    { m_nTempoAsseFermo = nTempoAsseFermo; }
	void SetCostanteDiTempoAzionamento (double fpCostanteDiTempoAzionamento)   { m_fpCostanteDiTempoAzionamento = fpCostanteDiTempoAzionamento; }
	void SetVelocitaDiCorrezione (double fpVelocitaDiCorrezione)	{ m_fpVelocitaDiCorrezione = fpVelocitaDiCorrezione; }
	void SetCorrezioneDinamica (int nCorrezioneDinamica)			{ m_nCorrezioneDinamica = nCorrezioneDinamica; }
	void SetEnableCompensazioneEA (int nEnableCompensazioneEA)		{ m_nEnableCompensazioneEA = nEnableCompensazioneEA; }
	void SetCompensazioneEA (double fpCompensazioneEA)				{ m_fpCompensazioneEA = fpCompensazioneEA; }
	void SetCompensazioneEAA (double fpCompensazioneEAA)			{ m_fpCompensazioneEAA = fpCompensazioneEAA; }
	void SetFiltroCollegamentoEncoder (WORD wFiltroCollegamentoEncoder)		{ m_wFiltroCollegamentoEncoder = wFiltroCollegamentoEncoder; }
	void SetFiltroServoError (WORD wFiltroServoError)				{ m_wFiltroServoError = wFiltroServoError; }
    double GetResolution()   { return m_nResolution; }
    int GetAxisType()   { return m_nAxisType; }
    int GetAxesCategory()   { return m_nAxesCategory; }
    int GetUnit()   { return m_nUnit; }
    int GetInvPhase()   { return m_nInvPhase; }
    int GetInvRif()   { return m_nInvRif; }
    int GetEnableNick()   { return m_nEnableNick; }
    int GetEnableJerk ()   { return m_nEnableJerk; }
    double GetSpeed()   { return m_nSpeed; }
    double GetSpeedMan()   { return m_nSpeedMan; }
    double GetAcc()   { return m_nAcc; }
    double GetDec()   { return m_nDec; }
    double GetProp()   { return m_nProp; }
    double GetInt()   { return m_nInt; }
    double GetDer()   { return m_nDer; }
    double GetFeedForward()   { return m_nFeedForward; }
    double GetFeedForwardAcc()   { return m_nFeedForwardAcc; }
	double GetIntKT ()   { return m_fpIntKT; }
	int	   GetEnableSwitchedInt ()   { return m_nEnableSwitchedInt; }
	double GetSwitchedIntSpeed ()	{ return m_fpSwitchedIntSpeed; }
	double GetDerN ()	{ return m_fpDerN; }
    double GetRif()   { return m_fpRifVoltage; }
    int GetRampType()   { return m_nRampType; }
    double GetInterSpeed()   { return m_nInterSpeed; }
    double GetInterAcc()   { return m_nInterAcc; }
    double GetInterDec()   { return m_nInterDec; }
    double GetInterProp()   { return m_nInterProp; }
    double GetInterInt()   { return m_nInterInt; }
    double GetInterDer()   { return m_nInterDer; }
    double GetInterFeedForward()   { return m_nInterFeedForward; }
    double GetInterFeedForwardAcc()   { return m_nInterFeedForwardAcc; }
	double GetInterIntKT ()   { return m_fpInterIntKT; }
	int	   GetInterEnableSwitchedInt ()   { return m_nInterEnableSwitchedInt; }
	double GetInterSwitchedIntSpeed ()	{ return m_fpInterSwitchedIntSpeed; }
	double GetInterDerN ()	{ return m_fpInterDerN; }
    int GetInterRampType()   { return m_nInterRampType; }
    int GetWaitStopAxis()   { return m_nWaitStopAxis; }
    double GetLimEAConc()   { return m_fpLimEAConc; }
    double GetLimEADisc()   { return m_fpLimEADisc; }
    double GetLimEAMin()   { return m_fpLimEAMin; }
    int    GetServoLimitType () { return m_nServoLimitType; }
    int    GetEnableLimAxesPos ()   { return m_nEnableLimAxesPos; }
    int    GetEnableLimAxesNeg ()   { return m_nEnableLimAxesNeg; }
    double GetLimAxisPos()   { return m_nLimAxisPos; }
    double GetLimAxisNeg()   { return m_nLimAxisNeg; }
    double GetWinPos()   { return m_fpWinPos; }
    double GetWinNeg()   { return m_fpWinNeg; }
    CString GetAlias()   { return m_strAlias; }
    CString GetCCName()  { return m_szName; }
    void    SetCCName (CString strName)   { strcpy (m_szName, strName); }
    struct stCustomData* GetCD ()         { return m_stCD; }
    void SetCDEl (stCustomData* pCD, int n)  {
        if (n>=0&&n<MAX_AXIS_CUSTOM)
            m_stCD[n]=*pCD;
    }
    int     GetDimBufferIntegrale()  { return m_nDimBufferIntegrale; }
    void    SetDimBufferIntegrale (int nDimBufferIntegrale)   { m_nDimBufferIntegrale=nDimBufferIntegrale; }
    double  GetPercRT ()  { return m_fpPercentualeRampaTrapezoidale; }
    void    SetPercRT (double fpPercRT)   { m_fpPercentualeRampaTrapezoidale=fpPercRT; }
    double  GetInterPercRT ()  { return m_fpInterPercentualeRampaTrapezoidale; }
    void    SetInterPercRT (double fpPercRT)   { m_fpInterPercentualeRampaTrapezoidale=fpPercRT; }
    CString GetGName()           { return m_szGName; }
    int     GetGSlot ()          { return m_nGSlot; }
    int     GetGantryAxes ()   { return m_nAsseGantry; }
    double  GetPropGantry ()   { return m_nPropGantry; }
    double  GetDerGantry  ()   { return m_nDerGantry; }
    double  GetIntGantry  ()   { return m_nIntGantry; }
    int     GetDisplay ()      { return m_nDisplay; }
    int     GetTIC ()  { return m_nTIC; }
    int     GetMovementCategory () { return m_nMT; }
	int		GetStopOnAccDecChange ()   { return m_nACAD; }
    int     GetEnableRecuperoGioco ()   { return m_nEnableRecuperoGioco; }
	double  GetRecuperoGioco ()   { return m_fpRecuperoGioco; }
	double  GetTempoDiRecupero ()   { return m_fpTempoDiRecupero; }
	double  GetRangeQuotaNeg ()   { return m_fpRangeQuotaNeg; }
	double  GetRangeQuotaPos ()   { return m_fpRangeQuotaPos; }
    int     GetPercTorque ()      { return m_nPercTorque; }
	double  GetFiltroRotanti ()   { return m_fpFiltraRotanti; }
	int     GetTempoArrivoInQuota (){ return m_nTempoArrivoInQuota; }
	int     GetTempoAsseFermo ()    { return m_nTempoAsseFermo; }
	double  GetCostanteDiTempoAzionamento ()   { return m_fpCostanteDiTempoAzionamento; }
	double  GetVelocitaDiCorrezione ()	{ return m_fpVelocitaDiCorrezione; }
	int     GetEnableCompensazioneEA ()			{ return m_nEnableCompensazioneEA; }
	double  GetCompensazioneEA ()				{ return m_fpCompensazioneEA; }
	double  GetCompensazioneEAA ()				{ return m_fpCompensazioneEAA; }
	int     GetCorrezioneDinamica ()			{ return m_nCorrezioneDinamica; }
	WORD    GetFiltroCollegamentoEncoder ()		{ return m_wFiltroCollegamentoEncoder; }
	WORD    GetFiltroServoError ()				{ return m_wFiltroServoError; }
    CAdamoAxis& operator= (const CAdamoAxis& aa);
protected:
    CString m_strAlias;
    CString m_szGName;
    int     m_nGSlot;
    double m_nResolution;
    int m_nAxisType;
    int m_nAxesCategory;
    int m_nUnit;
    int m_nInvPhase;
    int m_nInvRif;
    int m_nWaitStopAxis;
    int m_nEnableNick;
    int m_nEnableJerk;
    double m_fpRifVoltage;
    double m_nSpeed;
    double m_nSpeedMan;
    double m_nAcc;
    double m_nDec;
    double m_nProp;
    double m_nInt;
    double m_nDer;
    double m_nFeedForward;
    double m_nFeedForwardAcc;
	double m_fpIntKT;
	int    m_nEnableSwitchedInt;
	double m_fpSwitchedIntSpeed;
	double m_fpDerN;
    int    m_nRampType;
    double m_nInterSpeed;
    double m_nInterAcc;
    double m_nInterDec;
    double m_nInterProp;
    double m_nInterInt;
    double m_nInterDer;
    double m_nInterFeedForward;
    double m_nInterFeedForwardAcc;
	double m_fpInterIntKT;
	int    m_nInterEnableSwitchedInt;
	double m_fpInterSwitchedIntSpeed;
	double m_fpInterDerN;
    int    m_nInterRampType;
    double m_fpLimEAConc;
    double m_fpLimEADisc;
    double m_fpLimEAMin;
    int    m_nEnableLimAxesPos;
    int    m_nEnableLimAxesNeg;
    double m_nLimAxisPos;
    double m_nLimAxisNeg;
    double m_fpWinPos;
    double m_fpWinNeg;
    int    m_nServoLimitType;
	int    m_nDimBufferIntegrale;
	double m_fpPercentualeRampaTrapezoidale;
	double m_fpInterPercentualeRampaTrapezoidale;
    char   m_szName[_MAX_FNAME];
    struct stCustomData m_stCD[MAX_AXIS_CUSTOM];
    int    m_nAsseGantry;
    double m_nPropGantry;
    double m_nIntGantry;
    double m_nDerGantry;
    int    m_nDisplay;
    int    m_nTIC;
    int    m_nMT;
	int    m_nACAD;
    int    m_nEnableRecuperoGioco;
	double m_fpRecuperoGioco;
	double m_fpTempoDiRecupero;
	double m_fpRangeQuotaNeg;
	double m_fpRangeQuotaPos;
	int    m_nPercTorque;
	double m_fpFiltraRotanti;
    int    m_nTempoArrivoInQuota;
    int    m_nTempoAsseFermo;
	double m_fpCostanteDiTempoAzionamento;
	double m_fpVelocitaDiCorrezione;
	int    m_nCorrezioneDinamica;
	int    m_nEnableCompensazioneEA;
	double m_fpCompensazioneEA;
	double m_fpCompensazioneEAA;
	WORD   m_wFiltroCollegamentoEncoder;
	WORD   m_wFiltroServoError;
    char   m_cSpare[MAX_AXIS_SPARE_AXES];
private :
	bool   m_bIsConnected;
};

#endif
