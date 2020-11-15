/*
** AdamoAxis.cpp
*/ 

#include "stdafx.h"
#include "adamo.h"
#include "AdamoBase.h"
#include "AdamoAxis.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CAdamoAxis, CObject, 1)

/*
** CAdamoAxis : costruttore
 */
CAdamoAxis::CAdamoAxis()
{
    m_strAlias = "";
    m_nResolution = 100.0;
    m_nAxisType = enAsseContinua;
    m_nAxesCategory = enAsseLineare;
    m_nUnit = enUMMmin;
    m_nInvPhase = 0;
    m_nInvRif = 0;
    m_nWaitStopAxis = 0;
    m_nEnableNick = 0;
    m_nEnableJerk = 0;
    m_fpRifVoltage = 0;

    m_nSpeed = 0.0;
    m_nSpeedMan = 0.0;
    m_nAcc = 0.0;
    m_nDec = 0.0;
    
    m_nProp = 0.0;
    m_nInt = 0.0;
    m_nDer = 0.0;
    m_nFeedForward = 0.0;
    m_nFeedForwardAcc = 0.0;

    m_nRampType = enLineare;
    
    m_nInterSpeed = 0.0;
    m_nInterAcc = 0.0;
    m_nInterDec = 0.0;
    
    m_nInterProp = 0.0;
    m_nInterInt = 0.0;
    m_nInterDer = 0.0;
    m_nInterFeedForward = 0.0;
    m_nInterFeedForwardAcc = 0.0;
    
    m_nInterRampType = enLineare;

    m_fpLimEAConc = 0.0;
    m_fpLimEADisc = 0.0;
    m_fpLimEAMin = 0.0;
    m_nEnableLimAxesPos = 0;
    m_nEnableLimAxesNeg = 0;
    m_nLimAxisPos = 0.0;
    m_nLimAxisNeg = 0.0;

    m_fpWinPos = 0.0;
    m_fpWinNeg = 0.0;

    m_nServoLimitType = 0;
    memset (m_szName, 0, sizeof (m_szName));

	m_nDimBufferIntegrale=256;
	m_fpPercentualeRampaTrapezoidale=10.0;
	m_fpInterPercentualeRampaTrapezoidale=10.0;
    m_nGSlot=_NO_GSLOT;

    memset (m_stCD, 0, sizeof (m_stCD));
    for (int i=0; i<MAX_AXIS_CUSTOM; i++)
        m_stCD[i].nType=-1;

    m_nAsseGantry=_NO_GANTRY;
    m_nPropGantry=0.0;
    m_nIntGantry =0.0;
    m_nDerGantry =0.0;
    m_nDisplay=1;
    m_nTIC=enTICPosizione;
	m_nEnableRecuperoGioco=0;
	m_fpRecuperoGioco=0.0;
	m_fpTempoDiRecupero=0.0;
	m_fpRangeQuotaNeg=0.05;
	m_fpRangeQuotaPos=0.05;
	m_nPercTorque = MAX_TORQUE;
	m_fpFiltraRotanti = 0.0;
    m_nTempoArrivoInQuota = 0;
    m_nTempoAsseFermo = 0;
	m_fpCostanteDiTempoAzionamento = 0.0;
	m_fpVelocitaDiCorrezione = 0.0;
	m_nCorrezioneDinamica = false;
	m_nEnableCompensazioneEA = 0;
	m_fpCompensazioneEA = 0.0;
	m_fpCompensazioneEAA = 0.0;
	m_wFiltroCollegamentoEncoder = 0;
	m_wFiltroServoError = 0;
    memset (m_cSpare, 0, MAX_AXIS_SPARE_AXES);
	m_bIsConnected = false;
	m_fpIntKT = 0.0;
	m_nEnableSwitchedInt = 0;
	m_fpSwitchedIntSpeed = 0.0;
	m_fpDerN = 0.0;
	m_fpInterIntKT = 0.0;
	m_nInterEnableSwitchedInt = 0;
	m_fpInterSwitchedIntSpeed = 0.0;
	m_fpInterDerN = 0.0;
}

/*
** CAdamoAxis : copy constructor
 */
CAdamoAxis::CAdamoAxis(CAdamoAxis& aa)
{
    //m_strAlias=aa.m_strAlias;
    m_nResolution=aa.m_nResolution;
    m_nAxisType=aa.m_nAxisType;
    m_nAxesCategory=aa.m_nAxesCategory;
    m_nUnit=aa.m_nUnit;
    m_nInvPhase=aa.m_nInvPhase;
    m_nInvRif=aa.m_nInvRif;
    m_nWaitStopAxis=aa.m_nWaitStopAxis;
    m_nEnableNick=aa.m_nEnableNick;
    m_nEnableJerk=aa.m_nEnableJerk;
    m_fpRifVoltage=aa.m_fpRifVoltage;

    m_nSpeed=aa.m_nSpeed;
    m_nSpeedMan=aa.m_nSpeedMan;
    m_nAcc=aa.m_nAcc;
    m_nDec=aa.m_nDec;
    
    m_nProp=aa.m_nProp;
    m_nInt=aa.m_nInt;
    m_nDer=aa.m_nDer;
    m_nFeedForward=aa.m_nFeedForward;
    m_nFeedForwardAcc=aa.m_nFeedForwardAcc;

    m_nRampType=aa.m_nRampType;
    
    m_nInterSpeed=aa.m_nInterSpeed;
    m_nInterAcc=aa.m_nInterAcc;
    m_nInterDec=aa.m_nInterDec;
    
    m_nInterProp=aa.m_nInterProp;
    m_nInterInt=aa.m_nInterInt;
    m_nInterDer=aa.m_nInterDer;
    m_nInterFeedForward=aa.m_nInterFeedForward;
    m_nInterFeedForwardAcc=aa.m_nInterFeedForwardAcc;
    
    m_nInterRampType=aa.m_nInterRampType;

    m_fpLimEAConc=aa.m_fpLimEAConc;
    m_fpLimEADisc=aa.m_fpLimEADisc;
    m_fpLimEAMin=aa.m_fpLimEAMin;
    m_nEnableLimAxesPos=aa.m_nEnableLimAxesPos;
    m_nEnableLimAxesNeg=aa.m_nEnableLimAxesNeg;
    m_nLimAxisPos=aa.m_nLimAxisPos;
    m_nLimAxisNeg=aa.m_nLimAxisNeg;

    m_fpWinPos=aa.m_fpWinPos;
    m_fpWinNeg=aa.m_fpWinNeg;

    m_nServoLimitType=aa.m_nServoLimitType;
    strcpy (m_szName, aa.m_szName);

	m_nDimBufferIntegrale=aa.m_nDimBufferIntegrale;
	m_fpPercentualeRampaTrapezoidale=aa.m_fpPercentualeRampaTrapezoidale;
	m_fpInterPercentualeRampaTrapezoidale=aa.m_fpInterPercentualeRampaTrapezoidale;

    for (int i=0; i<MAX_AXIS_CUSTOM; i++)
        m_stCD[i]=aa.m_stCD[i];

    m_nAsseGantry=aa.m_nAsseGantry;
    m_nPropGantry=aa.m_nPropGantry;
    m_nIntGantry =aa.m_nIntGantry;
    m_nDerGantry =aa.m_nDerGantry;
    m_nDisplay=aa.m_nDisplay;
    m_nTIC=aa.m_nTIC;
	m_nMT = aa.m_nMT;
	m_nACAD = aa.m_nACAD;

    //m_szGName=aa.m_szGName;
    m_nGSlot=aa.m_nGSlot;
	m_nEnableRecuperoGioco=aa.m_nEnableRecuperoGioco;
	m_fpRecuperoGioco = aa.m_fpRecuperoGioco;
	m_fpTempoDiRecupero = aa.m_fpTempoDiRecupero;
	m_fpRangeQuotaNeg = aa.m_fpRangeQuotaNeg;
	m_fpRangeQuotaPos = aa.m_fpRangeQuotaPos;
	m_nPercTorque = aa.m_nPercTorque;
	m_fpFiltraRotanti = aa.m_fpFiltraRotanti;
	m_nTempoArrivoInQuota = aa.m_nTempoArrivoInQuota;
	m_nTempoAsseFermo = aa.m_nTempoAsseFermo;
	m_fpCostanteDiTempoAzionamento = aa.m_fpCostanteDiTempoAzionamento;
	m_fpVelocitaDiCorrezione = aa.m_fpVelocitaDiCorrezione;
	m_nCorrezioneDinamica = aa.m_nCorrezioneDinamica;
	m_nEnableCompensazioneEA = aa.m_nEnableCompensazioneEA;
	m_fpCompensazioneEA = aa.m_fpCompensazioneEA;
	m_fpCompensazioneEAA = aa.m_fpCompensazioneEAA;
	m_wFiltroCollegamentoEncoder = aa.m_wFiltroCollegamentoEncoder;
	m_wFiltroServoError = aa.m_wFiltroServoError;
    memset (m_cSpare, 0, MAX_AXIS_SPARE_AXES);
	m_bIsConnected = aa.m_bIsConnected;
	m_fpIntKT = aa.m_fpIntKT;
	m_nEnableSwitchedInt = aa.m_nEnableSwitchedInt;
	m_fpSwitchedIntSpeed = aa.m_fpSwitchedIntSpeed;
	m_fpDerN = aa.m_fpDerN;
	m_fpInterIntKT = aa.m_fpInterIntKT;
	m_nInterEnableSwitchedInt = aa.m_nInterEnableSwitchedInt;
	m_fpInterSwitchedIntSpeed = aa.m_fpInterSwitchedIntSpeed;
	m_fpInterDerN = aa.m_fpInterDerN;
}

/*
** ~CAdamoAxis : distruttore
*/
CAdamoAxis::~CAdamoAxis()
{

}

/*
** Serialize : serializzazione
*/
void CAdamoAxis::Serialize(CArchive &ar)
{
	int i;

    CAdamoBase::Serialize(ar);

    if (ar.IsStoring()){
        ar << m_strAlias;
        ar << m_nResolution;
        ar << m_nAxisType;
        ar << m_nAxesCategory;
        ar << m_nUnit;
        ar << m_nInvPhase;
        ar << m_nInvRif;
        ar << m_nWaitStopAxis;
        ar << m_nEnableNick;
        ar << m_nEnableJerk;
        ar << m_fpRifVoltage;

        ar << m_nSpeed;
        ar << m_nSpeedMan;
        ar << m_nAcc;
        ar << m_nDec;
    
        ar << m_nProp;
        ar << m_nInt;
        ar << m_nDer;
        ar << m_nFeedForward;
        ar << m_nFeedForwardAcc;

        ar << m_nRampType;

        ar << m_nInterSpeed;
        ar << m_nInterAcc;
        ar << m_nInterDec;
    
        ar << m_nInterProp;
        ar << m_nInterInt;
        ar << m_nInterDer;
        ar << m_nInterFeedForward;
        ar << m_nInterFeedForwardAcc;
    
        ar << m_nInterRampType;

        ar << m_fpLimEAConc;
        ar << m_fpLimEADisc;
        ar << m_nEnableLimAxesPos;
        ar << m_nEnableLimAxesNeg;
        ar << m_nLimAxisPos;
        ar << m_nLimAxisNeg;

        ar << m_fpWinPos;
        ar << m_fpWinNeg;

        ar << m_nServoLimitType;

        ar << CString (m_szName);
        
        for (i=0; i<MAX_AXIS_CUSTOM; i++)   {
            ar << m_stCD[i].nType;
            if (m_stCD[i].nType!=-1)   {
				int n;

                ar << CString (m_stCD[i].szName);
                switch (m_stCD[i].nType)   {
                    case LUA_TBOOLEAN :
                        n=(int)m_stCD[i].uData.b; ar << n; m_stCD[i].uData.b; break;
                    case LUA_TNUMBER :
                        ar << m_stCD[i].uData.fp; break;
                    case LUA_TSTRING :
                        ar << CString (m_stCD[i].uData.str); break;
                }
            }
        }

        ar << m_nDimBufferIntegrale;
        ar << m_fpPercentualeRampaTrapezoidale;
        ar << m_fpInterPercentualeRampaTrapezoidale;
        ar << m_szGName;
        ar << m_nGSlot;
        ar << m_nAsseGantry;
        ar << m_nPropGantry;
        ar << m_nIntGantry;
        ar << m_nDerGantry;
        ar << m_nDisplay;
        ar << m_nTIC;
        ar << m_nMT;
		ar << m_nACAD;
		ar << m_nEnableRecuperoGioco;
		ar << m_fpRecuperoGioco;
		ar << m_fpTempoDiRecupero;
		ar << m_fpRangeQuotaNeg;
		ar << m_fpRangeQuotaPos;
        ar << m_nPercTorque;
		ar << m_fpFiltraRotanti;
		ar << m_nTempoArrivoInQuota;
		ar << m_nTempoAsseFermo;
		ar << m_fpCostanteDiTempoAzionamento;
		ar << m_fpVelocitaDiCorrezione;
		ar << m_fpLimEAMin;
		ar << m_nEnableCompensazioneEA;
		ar << m_fpCompensazioneEA;
		ar << m_nCorrezioneDinamica;
		ar << m_fpIntKT;
		ar << m_nEnableSwitchedInt;
		ar << m_fpSwitchedIntSpeed;
		ar << m_fpDerN;
		ar << m_fpInterIntKT;
		ar << m_nInterEnableSwitchedInt;
		ar << m_fpInterSwitchedIntSpeed;
		ar << m_fpInterDerN;
		ar << m_wFiltroCollegamentoEncoder;
		ar << m_wFiltroServoError;
		ar << m_fpCompensazioneEAA;
        for(i=0; i<MAX_AXIS_SPARE_AXES; i++)
            ar << m_cSpare[i];
    }
    else   {
        ar >> m_strAlias;
        ar >> m_nResolution;
        ar >> m_nAxisType;
        ar >> m_nAxesCategory;
        ar >> m_nUnit;
        ar >> m_nInvPhase;
        ar >> m_nInvRif;
        ar >> m_nWaitStopAxis;
        ar >> m_nEnableNick;
        ar >> m_nEnableJerk;
        ar >> m_fpRifVoltage;

        ar >> m_nSpeed;
        ar >> m_nSpeedMan;
        ar >> m_nAcc;
        ar >> m_nDec;
    
        ar >> m_nProp;
        ar >> m_nInt;
        ar >> m_nDer;
        ar >> m_nFeedForward;
        ar >> m_nFeedForwardAcc;

        ar >> m_nRampType;

        ar >> m_nInterSpeed;
        ar >> m_nInterAcc;
        ar >> m_nInterDec;
    
        ar >> m_nInterProp;
        ar >> m_nInterInt;
        ar >> m_nInterDer;
        ar >> m_nInterFeedForward;
        ar >> m_nInterFeedForwardAcc;
    
        ar >> m_nInterRampType;

        ar >> m_fpLimEAConc;
        ar >> m_fpLimEADisc;
        ar >> m_nEnableLimAxesPos;
        ar >> m_nEnableLimAxesNeg;
        ar >> m_nLimAxisPos;
        ar >> m_nLimAxisNeg;

        ar >> m_fpWinPos;
        ar >> m_fpWinNeg;

        ar >> m_nServoLimitType;

        CString str;
        ar >> str;
        strcpy (m_szName, str);

        for (i=0; i<MAX_AXIS_CUSTOM; i++)   {
            ar >> m_stCD[i].nType;
            if (m_stCD[i].nType!=-1)   {
                int n;
                ar >> str;
                strcpy (m_stCD[i].szName, str);
                switch (m_stCD[i].nType)   {
                    case LUA_TNUMBER :
                        ar >> m_stCD[i].uData.fp; break;
                    case LUA_TSTRING :
                        ar >> str;
                        strcpy (m_stCD[i].uData.str, str); break;
                    case LUA_TBOOLEAN :
                        ar >> n; m_stCD[i].uData.b=n!=0; break;
                }
            }
        }

        ar >> m_nDimBufferIntegrale;
        ar >> m_fpPercentualeRampaTrapezoidale;
        ar >> m_fpInterPercentualeRampaTrapezoidale;
        ar >> m_szGName;
        ar >> m_nGSlot;
        ar >> m_nAsseGantry;
        ar >> m_nPropGantry;
        ar >> m_nIntGantry;
        ar >> m_nDerGantry;
        ar >> m_nDisplay;
        ar >> m_nTIC;
        ar >> m_nMT;
		ar >> m_nACAD;
		ar >> m_nEnableRecuperoGioco;
		ar >> m_fpRecuperoGioco;
		ar >> m_fpTempoDiRecupero;
		ar >> m_fpRangeQuotaNeg;
		ar >> m_fpRangeQuotaPos;
        ar >> m_nPercTorque;
		ar >> m_fpFiltraRotanti;
		ar >> m_nTempoArrivoInQuota;
		ar >> m_nTempoAsseFermo;
		ar >> m_fpCostanteDiTempoAzionamento;
		ar >> m_fpVelocitaDiCorrezione;
		ar >> m_fpLimEAMin;
		ar >> m_nEnableCompensazioneEA;
		ar >> m_fpCompensazioneEA;
		ar >> m_nCorrezioneDinamica;
		ar >> m_fpIntKT;
		ar >> m_nEnableSwitchedInt;
		ar >> m_fpSwitchedIntSpeed;
		ar >> m_fpDerN;
		ar >> m_fpInterIntKT;
		ar >> m_nInterEnableSwitchedInt;
		ar >> m_fpInterSwitchedIntSpeed;
		ar >> m_fpInterDerN;
		ar >> m_wFiltroCollegamentoEncoder;
		ar >> m_wFiltroServoError;
		ar >> m_fpCompensazioneEAA;
        for(i=0;i<MAX_AXIS_SPARE_AXES;++i)
            ar >> m_cSpare[i];
    }
}

CAdamoAxis& CAdamoAxis::operator= (const CAdamoAxis& aa)
{
    CAdamoBase::operator = (aa);
    //m_strAlias=aa.m_strAlias;
    m_nResolution=aa.m_nResolution;
    m_nAxisType=aa.m_nAxisType;
    m_nAxesCategory=aa.m_nAxesCategory;
    m_nUnit=aa.m_nUnit;
    m_nInvPhase=aa.m_nInvPhase;
    m_nInvRif=aa.m_nInvRif;
    m_nWaitStopAxis=aa.m_nWaitStopAxis;
    m_nEnableNick=aa.m_nEnableNick;
    m_nEnableJerk=aa.m_nEnableJerk;
    m_fpRifVoltage=aa.m_fpRifVoltage;

    m_nSpeed=aa.m_nSpeed;
    m_nSpeedMan=aa.m_nSpeedMan;
    m_nAcc=aa.m_nAcc;
    m_nDec=aa.m_nDec;
    
    m_nProp=aa.m_nProp;
    m_nInt=aa.m_nInt;
    m_nDer=aa.m_nDer;
    m_nFeedForward=aa.m_nFeedForward;
    m_nFeedForwardAcc=aa.m_nFeedForwardAcc;

    m_nRampType=aa.m_nRampType;
    
    m_nInterSpeed=aa.m_nInterSpeed;
    m_nInterAcc=aa.m_nInterAcc;
    m_nInterDec=aa.m_nInterDec;
    
    m_nInterProp=aa.m_nInterProp;
    m_nInterInt=aa.m_nInterInt;
    m_nInterDer=aa.m_nInterDer;
    m_nInterFeedForward=aa.m_nInterFeedForward;
    m_nInterFeedForwardAcc=aa.m_nInterFeedForwardAcc;
    
    m_nInterRampType=aa.m_nInterRampType;

    m_fpLimEAConc=aa.m_fpLimEAConc;
    m_fpLimEADisc=aa.m_fpLimEADisc;
    m_fpLimEAMin=aa.m_fpLimEAMin;
    m_nEnableLimAxesPos=aa.m_nEnableLimAxesPos;
    m_nEnableLimAxesNeg=aa.m_nEnableLimAxesNeg;
    m_nLimAxisPos=aa.m_nLimAxisPos;
    m_nLimAxisNeg=aa.m_nLimAxisNeg;

    m_fpWinPos=aa.m_fpWinPos;
    m_fpWinNeg=aa.m_fpWinNeg;

    m_nServoLimitType=aa.m_nServoLimitType;

	m_nDimBufferIntegrale=aa.m_nDimBufferIntegrale;
	m_fpPercentualeRampaTrapezoidale=aa.m_fpPercentualeRampaTrapezoidale;
	m_fpInterPercentualeRampaTrapezoidale=aa.m_fpInterPercentualeRampaTrapezoidale;

    //m_szGName=aa.m_szGName;
    m_nGSlot=aa.m_nGSlot;

    for (int i=0; i<MAX_AXIS_CUSTOM; i++)
        m_stCD[i]=aa.m_stCD[i];

    m_nAsseGantry=aa.m_nAsseGantry;
    m_nPropGantry=aa.m_nPropGantry;
    m_nIntGantry =aa.m_nIntGantry;
    m_nDerGantry =aa.m_nDerGantry;
    m_nDisplay =aa.m_nDisplay;
    m_nTIC =aa.m_nTIC;
	m_nMT = aa.m_nMT;
	m_nACAD = aa.m_nACAD;
	m_nEnableRecuperoGioco = aa.m_nEnableRecuperoGioco;
	m_fpRecuperoGioco = aa.m_fpRecuperoGioco;
	m_fpTempoDiRecupero = aa.m_fpTempoDiRecupero;
	m_fpRangeQuotaNeg = aa.m_fpRangeQuotaNeg;
	m_fpRangeQuotaPos = aa.m_fpRangeQuotaPos;
	m_nPercTorque = aa.m_nPercTorque;
	m_fpFiltraRotanti = aa.m_fpFiltraRotanti;
	m_nTempoArrivoInQuota = aa.m_nTempoArrivoInQuota;
	m_nTempoAsseFermo = aa.m_nTempoAsseFermo;
	m_fpCostanteDiTempoAzionamento = aa.m_fpCostanteDiTempoAzionamento;
	m_fpVelocitaDiCorrezione = aa.m_fpVelocitaDiCorrezione;
	m_nCorrezioneDinamica = aa.m_nCorrezioneDinamica;
	m_nEnableCompensazioneEA = aa.m_nEnableCompensazioneEA;
	m_fpCompensazioneEA = aa.m_fpCompensazioneEA;
	m_fpCompensazioneEAA = aa.m_fpCompensazioneEAA;
	m_wFiltroCollegamentoEncoder = aa.m_wFiltroCollegamentoEncoder;
	m_wFiltroServoError = aa.m_wFiltroServoError;
	m_fpIntKT = aa.m_fpIntKT;
	m_nEnableSwitchedInt = aa.m_nEnableSwitchedInt;
	m_fpSwitchedIntSpeed = aa.m_fpSwitchedIntSpeed;
	m_fpDerN = aa.m_fpDerN;
	m_fpInterIntKT = aa.m_fpInterIntKT;
	m_nInterEnableSwitchedInt = aa.m_nInterEnableSwitchedInt;
	m_fpInterSwitchedIntSpeed = aa.m_fpInterSwitchedIntSpeed;
	m_fpInterDerN = aa.m_fpInterDerN;
    memset (m_cSpare, 0, MAX_AXIS_SPARE_AXES);
    return *this;
}

/*******************
 * Serialize : serializzazione file XML
 */
void CAdamoAxis::Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile)
{
	CString str;

	pFile->WriteString ("<Axis>\n");
	CAdamoBase::Serialize (pAdamoParametri, pFile);
	str.Format ("<Alias>%s</Alias>\n", m_strAlias);
	pFile->WriteString (str);
	str.Format ("<Resolution>%.3f</Resolution>\n", m_nResolution);
	pFile->WriteString (str);
	str.Format ("<Type>%d</Type>\n", m_nAxisType);
	pFile->WriteString (str);
	str.Format ("<Control>%d</Control>\n", m_nTIC);
	pFile->WriteString (str);
	str.Format ("<MoveCategory>%d</MoveCategory>\n", m_nMT);
	pFile->WriteString (str);
	str.Format ("<AxisCategory>%d</AxisCategory>\n", m_nAxesCategory);
	pFile->WriteString (str);
	str.Format ("<Unit>%d</Unit>\n", m_nUnit);
	pFile->WriteString (str);
	str.Format ("<PhaseInversion>%d</PhaseInversion>\n", m_nInvPhase);
	pFile->WriteString (str);
	str.Format ("<ReferenceInversion>%d</ReferenceInversion>\n", m_nInvRif);
	pFile->WriteString (str);
	str.Format ("<WaitStopped>%d</WaitStopped>\n", m_nWaitStopAxis);
	pFile->WriteString (str);
	str.Format ("<EnableMarker>%d</EnableMarker>\n", m_nEnableNick);
	pFile->WriteString (str);
	str.Format ("<EnableJerk>%d</EnableJerk>\n", m_nEnableJerk);
	pFile->WriteString (str);
	str.Format ("<GName>%s</GName>\n", m_szGName);
	pFile->WriteString (str);
	str.Format ("<GInterpolationChannel>%d</GInterpolationChannel>\n", m_nGSlot);
	pFile->WriteString (str);
	str.Format ("<ReferenceVoltage>%.3f</ReferenceVoltage>\n", m_fpRifVoltage);
	pFile->WriteString (str);
	str.Format ("<Speed>%.3f</Speed>\n", m_nSpeed);
	pFile->WriteString (str);
	str.Format ("<SpeedManual>%.3f</SpeedManual>\n", m_nSpeedMan);
	pFile->WriteString (str);
	str.Format ("<Acceleration>%.3f</Acceleration>\n", m_nAcc);
	pFile->WriteString (str);
	str.Format ("<Deceleration>%.3f</Deceleration>\n", m_nDec);
	pFile->WriteString (str);
	str.Format ("<Gain>%.3f</Gain>\n", m_nProp);
	pFile->WriteString (str);
	str.Format ("<Derivative>%.3f</Derivative>\n", m_nDer);
	pFile->WriteString (str);
	str.Format ("<Integrative>%.3f</Integrative>\n", m_nInt);
	pFile->WriteString (str);
	str.Format ("<FeedForward>%.3f</FeedForward>\n", m_nFeedForward);
	pFile->WriteString (str);
	str.Format ("<FeedForwardAcc>%.3f</FeedForwardAcc>\n", m_nFeedForwardAcc);
	pFile->WriteString (str);
	str.Format ("<IntKT>%.3f</IntKT>\n", m_fpIntKT);
	pFile->WriteString (str);
	str.Format ("<EnableSwitchedInt>%d</EnableSwitchedInt>\n", m_nEnableSwitchedInt);
	pFile->WriteString (str);
	str.Format ("<SwitchedIntSpeed>%.3f</SwitchedIntSpeed>\n", m_fpSwitchedIntSpeed);
	pFile->WriteString (str);
	str.Format ("<DerN>%.3f</DerN>\n", m_fpDerN);
	pFile->WriteString (str);
	str.Format ("<RampType>%d</RampType>\n", m_nRampType);
	pFile->WriteString (str);
	str.Format ("<RampFactor>%.3f</RampFactor>\n", m_fpPercentualeRampaTrapezoidale);
	pFile->WriteString (str);
	str.Format ("<SpeedW>%.3f</SpeedW>\n", m_nInterSpeed);
	pFile->WriteString (str);
	str.Format ("<AccelerationW>%.3f</AccelerationW>\n", m_nInterAcc);
	pFile->WriteString (str);
	str.Format ("<DecelerationW>%.3f</DecelerationW>\n", m_nInterDec);
	pFile->WriteString (str);
	str.Format ("<GainW>%.3f</GainW>\n", m_nInterProp);
	pFile->WriteString (str);
	str.Format ("<DerivativeW>%.3f</DerivativeW>\n", m_nInterDer);
	pFile->WriteString (str);
	str.Format ("<IntegrativeW>%.3f</IntegrativeW>\n", m_nInterInt);
	pFile->WriteString (str);
	str.Format ("<FeedForwardW>%.3f</FeedForwardW>\n", m_nInterFeedForward);
	pFile->WriteString (str);
	str.Format ("<FeedForwardAccW>%.3f</FeedForwardAccW>\n", m_nInterFeedForwardAcc);
	pFile->WriteString (str);
	str.Format ("<IntKTW>%.3f</IntKTW>\n", m_fpInterIntKT);
	pFile->WriteString (str);
	str.Format ("<EnableSwitchedIntW>%d</EnableSwitchedIntW>\n", m_nInterEnableSwitchedInt);
	pFile->WriteString (str);
	str.Format ("<SwitchedIntSpeedW>%.3f</SwitchedIntSpeedW>\n", m_fpInterSwitchedIntSpeed);
	pFile->WriteString (str);
	str.Format ("<DerNW>%.3f</DerNW>\n", m_fpInterDerN);
	pFile->WriteString (str);
	str.Format ("<RampTypeW>%d</RampTypeW>\n", m_nInterRampType);
	pFile->WriteString (str);
	str.Format ("<RampFactorW>%.3f</RampFactorW>\n", m_fpInterPercentualeRampaTrapezoidale);
	pFile->WriteString (str);
	str.Format ("<ServoLimitType>%d</ServoLimitType>\n", m_nServoLimitType);
	pFile->WriteString (str);
	str.Format ("<EALimitConc>%.3f</EALimitConc>\n", m_fpLimEAConc);
	pFile->WriteString (str);
	str.Format ("<EALimitDisc>%.3f</EALimitDisc>\n", m_fpLimEADisc);
	pFile->WriteString (str);
	str.Format ("<EALimitMin>%.3f</EALimitMin>\n", m_fpLimEAMin);
	pFile->WriteString (str);
	str.Format ("<EnablePosLimit>%d</EnablePosLimit>\n", m_nEnableLimAxesPos);
	pFile->WriteString (str);
	str.Format ("<EnableNegLimit>%d</EnableNegLimit>\n", m_nEnableLimAxesNeg);
	pFile->WriteString (str);
	str.Format ("<PosLimit>%.3f</PosLimit>\n", m_nLimAxisPos);
	pFile->WriteString (str);
	str.Format ("<NegLimit>%.3f</NegLimit>\n", m_nLimAxisNeg);
	pFile->WriteString (str);
	str.Format ("<WinPos>%.3f</WinPos>\n", m_fpWinPos);
	pFile->WriteString (str);
	str.Format ("<WinNeg>%.3f</WinNeg>\n", m_fpWinNeg);
	pFile->WriteString (str);
	str.Format ("<WinInPositionPos>%.3f</WinInPositionPos>\n", m_fpRangeQuotaPos);
	pFile->WriteString (str);
	str.Format ("<WinInPositionNeg>%.3f</WinInPositionNeg>\n", m_fpRangeQuotaNeg);
	pFile->WriteString (str);
	str.Format ("<IntegralBuffer>%d</IntegralBuffer>\n", m_nDimBufferIntegrale);
	pFile->WriteString (str);
	str.Format ("<WMBCA>%d</WMBCA>\n", m_nACAD);
	pFile->WriteString (str);
	str.Format ("<KLC>%.3f</KLC>\n", m_fpCostanteDiTempoAzionamento);
	pFile->WriteString (str);
	str.Format ("<WaitInPosition>%d</WaitInPosition>\n", m_nTempoArrivoInQuota);
	pFile->WriteString (str);
	str.Format ("<WaitStopped>%d</WaitStopped>\n", m_nTempoAsseFermo);
	pFile->WriteString (str);
	str.Format ("<GantryAxis>%d</GantryAxis>\n", m_nAsseGantry);
	pFile->WriteString (str);
	str.Format ("<GantryGain>%.3f</GantryGain>\n", m_nPropGantry);
	pFile->WriteString (str);
	str.Format ("<GantryDerivative>%.3f</GantryDerivative>\n", m_nDerGantry);
	pFile->WriteString (str);
	str.Format ("<GantryIntegrative>%.3f</GantryIntegrative>\n", m_nIntGantry);
	pFile->WriteString (str);
	str.Format ("<RotationFilter>%.3f</RotationFilter>\n", m_fpFiltraRotanti);
	pFile->WriteString (str);
	str.Format ("<CCName>%s</CCName>\n", m_szName);
	pFile->WriteString (str);
	str.Format ("<EnableBacklashComp>%d</EnableBacklashComp>\n", m_nEnableRecuperoGioco);
	pFile->WriteString (str);
	str.Format ("<BacklaskComp>%.3f</BacklaskComp>\n", m_fpRecuperoGioco);
	pFile->WriteString (str);
	str.Format ("<BacklaskCompTime>%.3f</BacklaskCompTime>\n", m_fpTempoDiRecupero);
	pFile->WriteString (str);
	str.Format ("<CCSpeed>%.3f</CCSpeed>\n", m_fpVelocitaDiCorrezione);
	pFile->WriteString (str);
	str.Format ("<CCDynamic>%d</CCDynamic>\n", m_nCorrezioneDinamica);
	pFile->WriteString (str);
	str.Format ("<EnableEACompensation>%d</EnableEACompensation>\n", m_nEnableCompensazioneEA);
	pFile->WriteString (str);
	str.Format ("<EACompensation>%.3f</EACompensation>\n", m_fpCompensazioneEA);
	pFile->WriteString (str);
	str.Format ("<EAACompensation>%.3f</EAACompensation>\n", m_fpCompensazioneEAA);
	pFile->WriteString (str);
	str.Format ("<FilterOutControl>%d</FilterOutControl>\n", m_wFiltroCollegamentoEncoder);
	pFile->WriteString (str);
	str.Format ("<FilterFollowingError>%d</FilterFollowingError>\n", m_wFiltroServoError);
	pFile->WriteString (str);
	str.Format ("<CustomParameter>\n");
	pFile->WriteString (str);
	for (int i = 0; i<MAX_AXIS_CUSTOM; i++)   {
		if (m_stCD[i].nType != -1)   {
			str.Format ("<Parameter Number=\"%d\">\n", i);
			pFile->WriteString (str);
			str.Format ("<Name>%s</Name>\n", m_stCD[i].szName);
			pFile->WriteString (str);
			str.Format ("<Type>%d</Type>\n", m_stCD[i].nType);
			pFile->WriteString (str);
			switch (m_stCD[i].nType)   {
				case LUA_TNUMBER :
					str.Format ("<Value>%.3f</Value>\n", m_stCD[i].uData.fp);
					break;
				case LUA_TBOOLEAN :
					str.Format ("<Value>%d</Value>\n", m_stCD[i].uData.b);
					break;
				case LUA_TSTRING :
					str.Format ("<Value>%s</Value>\n", m_stCD[i].uData.str);
					break;
			}
			pFile->WriteString (str);
			str.Format ("</Parameter>\n");
			pFile->WriteString (str);
		}
	}
	str.Format ("</CustomParameter>\n");
	pFile->WriteString (str);
	str.Format ("<Visible>%d</Visible>\n", m_nDisplay);
	pFile->WriteString (str);
	str.Format ("<Torque>%d</Torque>\n", m_nPercTorque);
	pFile->WriteString (str);
	pFile->WriteString ("</Axis>\n");
}
