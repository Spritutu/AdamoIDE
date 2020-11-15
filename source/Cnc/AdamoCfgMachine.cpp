/*
** AdamoCfgMachine.cpp
*/

/* include files */
#include "stdafx.h"
#include "resource.h"
#include "AdamoMachine.h"
#include "AdamoOptions.h"


/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* implementation */
CAdamoCfgMachine::CAdamoCfgMachine () : m_pModule(NULL), m_bModified(false), m_nFrequency (FREQ50), m_nNativeFrequency (FREQ50), m_eNativeTypeMachine (enVirtual), m_eTypeMachine (enVirtual), m_fpPrecision (0.001), m_fpEpsilonRaggi (0.001), m_fpContoureAngle (15.0), m_nRate (20),
	m_fpKAC (0.5), m_lMessageFilteringOnStart (0), m_lStopAxisOnError (0), m_lPrefetchEnabled (0), m_lSensoRotazioneCircolari (1), m_tr (NULL), m_fpSmoothing (0.0),
	m_fpBowThickening (1.0), m_fpLinearThickening (1.0), m_fpVelProfile (10.0), m_dwAccProfile (500), m_dwDecProfile (500), m_nAInterpDefault (false), m_bSpikeDetector (true),
	m_bMaxUTurnPrecision (false), m_eSmootherType (enSmootherParabolic), m_fpSingleSpikeSmootherWeight (0.75), m_fpSingleSpikeFactor (0.9), m_fpGeometrySpikeFactor (6.0),
	m_bDetectJointUTurn (false), m_bStopOnAngle (false), m_fpMinStopAngle (MIN_STOP_ANGLE), m_fpSIP (0.0), m_fpG0RaggiataInfittimento (0.0), m_fpG0RaggiataRaggio (0.0),
	m_lRamDisk (false), m_lRamDiskBufferSize (0), m_bSimulationMode (false), m_lMaxJitter (0), m_bCentroIncrementale (false), m_nPlcBoost (0), m_bStrictAbsPosControl (0),
	m_nStrictAbsPosEncoderSteps (0), m_pOpt (NULL)
{
    m_pModule = new CAdamoLogicModule();
}

CAdamoCfgMachine::CAdamoCfgMachine (CObject *pParent) : m_pModule(NULL), m_bModified (false), m_nFrequency (FREQ50), m_nNativeFrequency (FREQ50), m_eNativeTypeMachine (enVirtual), m_eTypeMachine (enVirtual), m_fpPrecision (0.001), m_fpEpsilonRaggi (0.001), m_fpContoureAngle (15.0), m_nRate (20),
	m_fpKAC (0.5), m_lMessageFilteringOnStart (0), m_lStopAxisOnError (0), m_lPrefetchEnabled (0), m_lSensoRotazioneCircolari (1), m_tr (NULL), m_fpSmoothing (0.0),
	m_fpBowThickening (1.0), m_fpLinearThickening (1.0), m_fpVelProfile (10.0), m_dwAccProfile (500), m_dwDecProfile (500), m_nAInterpDefault (false), m_bSpikeDetector (true),
	m_bMaxUTurnPrecision (false), m_eSmootherType (enSmootherParabolic), m_fpSingleSpikeSmootherWeight (0.75), m_fpSingleSpikeFactor (0.9), m_fpGeometrySpikeFactor (6.0),
	m_bDetectJointUTurn (false), m_bStopOnAngle (false), m_fpMinStopAngle (MIN_STOP_ANGLE),
	m_lRamDisk (false), m_lRamDiskBufferSize (0), m_bSimulationMode (false), m_lMaxJitter (0), m_bCentroIncrementale (false), m_nPlcBoost (0), m_bStrictAbsPosControl (0),
	m_nStrictAbsPosEncoderSteps (0), m_pOpt (NULL)
{
    m_pModule = new CAdamoLogicModule(pParent);
}

CAdamoCfgMachine::~CAdamoCfgMachine()
{
    _delete (m_pModule);
}

void CAdamoCfgMachine::Serialize(CArchive &ar)
{
    int i, n, nTypeModule;
    char ch=0, achIpAddress[IPADDRESS_BUFFER];

	memset (achIpAddress, 0, sizeof (achIpAddress));
    if (ar.IsStoring())   {
       ar << m_eNativeTypeMachine << m_nNativeFrequency;
       ar << m_nRate;
       ar << m_fpPrecision; ar << m_fpEpsilonRaggi;
       ar << m_lMessageFilteringOnStart;
       ar << m_lStopAxisOnError;
	   ar << m_nModule;
	   ar << m_lPrefetchEnabled;
	   ar << m_lSensoRotazioneCircolari;
	   ar << m_fpKAC;
	   ar << m_fpSmoothing;
	   nTypeModule = m_eTypeModule;
	   ar << nTypeModule;
	   ar << m_nHWModuleNumber;
	   ar << m_nPort;
	   for (i = 0; i<IPADDRESS_BUFFER; i++)   
		   ar << m_achTcpIpAddress[i];
	   short nPlcBoost = (short) m_nPlcBoost;
	   ar << nPlcBoost;
	   short nStrictAbsPosControl = (short) m_bStrictAbsPosControl;
	   ar << nStrictAbsPosControl;
	   unsigned short nStrictAbsPosEncoderSteps = (unsigned short) m_nStrictAbsPosEncoderSteps;
	   ar << nStrictAbsPosEncoderSteps;
	   float fpBowThickening = (float) m_fpBowThickening;
	   ar << fpBowThickening ;
	   ar << m_fpVelProfile;
	   ar << m_dwAccProfile;
	   ar << m_dwDecProfile;
	   ar << m_nAInterpDefault;
	   ar << m_fpLinearThickening;
	   ar << m_lRamDisk;
	   ar << m_lRamDiskBufferSize;
	   ar << m_lG0Raggiata;
	   float fpG0RaggiataInfittimento = (float) m_fpG0RaggiataInfittimento;
	   ar << fpG0RaggiataInfittimento;
	   float fpG0RaggiataRaggio = (float) m_fpG0RaggiataRaggio;
	   ar << fpG0RaggiataRaggio;
       for (i=0; i<MAX_CFGMACHINE_SPARE; i++)
          ar << ch;
	   SaveExtendedData (ar);
    }
    else   {
       ar >> n >> m_nFrequency;
       ar >> m_nRate;
       ar >> m_fpPrecision; ar >> m_fpEpsilonRaggi;
       ar >> m_lMessageFilteringOnStart;
       ar >> m_lStopAxisOnError;
	   ar >> m_nModule;
	   ar >> m_lPrefetchEnabled;
	   ar >> m_lSensoRotazioneCircolari;
	   ar >> m_fpKAC;
	   ar >> m_fpSmoothing;
       m_eNativeTypeMachine = m_eTypeMachine = (eTypeMachine) n; 
       m_nNativeFrequency = m_nFrequency;
	   ar >> nTypeModule;
	   m_eTypeModule = (eTypeModule) nTypeModule;
	   ar >> m_nHWModuleNumber;
	   ar >> m_nPort;
	   for (i = 0; i<IPADDRESS_BUFFER; i++)   
		   ar >> m_achTcpIpAddress[i];
	   short nPlcBoost;
	   ar >> nPlcBoost;
	   m_nPlcBoost = nPlcBoost;
	   short nStrictAbsPosControl;
	   ar >> nStrictAbsPosControl;
	   m_bStrictAbsPosControl = nStrictAbsPosControl;
	   unsigned short nStrictAbsPosEncoderSteps;
	   ar >> nStrictAbsPosEncoderSteps;
	   m_nStrictAbsPosEncoderSteps = nStrictAbsPosEncoderSteps;
	   float fpBowThickening;
	   ar >> fpBowThickening;
	   m_fpBowThickening = fpBowThickening;
	   ar >> m_fpVelProfile;
	   ar >> m_dwAccProfile;
	   ar >> m_dwDecProfile;
	   ar >> m_nAInterpDefault;
	   ar >> m_fpLinearThickening;
       m_eTypeMachine=(eTypeMachine) n; 
	   if (m_pOpt && m_pOpt->GetLoadVirtualMachine ())
			m_eTypeMachine = enVirtual;
	   ar >> m_lRamDisk;
	   ar >> m_lRamDiskBufferSize;
	   ar >> m_lG0Raggiata;
	   float fpG0RaggiataInfittimento;
	   ar >> fpG0RaggiataInfittimento;
	   m_fpG0RaggiataInfittimento = (double)fpG0RaggiataInfittimento;
	   float fpG0RaggiataRaggio;
	   ar >> fpG0RaggiataRaggio;
	   m_fpG0RaggiataRaggio	= (double)fpG0RaggiataRaggio;
       for (i=0; i<MAX_CFGMACHINE_SPARE; i++)
          ar >> ch;
	   LoadExtendedData (ar);
    }
    if (m_pModule)
        /* serializziamo i dati */
       m_pModule->Serialize(ar);
}

/*
** GestisciMappaEId : aggiorna la mappa dei dispositivi e aggiorna gli ID.
*/
void CAdamoCfgMachine::GestisciMappaEId (CString strParamsPathName, bool bLoad)
{
    if (m_pModule)   {
        /* creiamo la mappa */
        m_pModule->CreateMap ();
        if (bLoad)
            CaricaDescrizioneTradotta (m_tr);
        /* resettiamo gli ID */
        m_pModule->ResetID ();
    }
    /* riassegnamoli */
    RefreshAdamoDatiFile (strParamsPathName);
}

void CAdamoCfgMachine::AddItem(CString strNodePath,CObject *pObj)
{
    if(m_pModule){
        CObject *pNode = m_pModule->GetItem(strNodePath);

    if(pNode){
        if(pNode->IsKindOf(RUNTIME_CLASS(CAdamoGroup))){
            ((CAdamoGroup*)pNode)->AddTail(pObj);
                m_pModule->SetItem(strNodePath+"."+((CAdamoBase*)pObj)->GetName(),pObj);
            }
        }
    }
}

/*
**
*/
void CAdamoCfgMachine::RemoveItem(CObject *pObj)
{
}

/*
** RefreshAdamoDatiFile :
*/
void CAdamoCfgMachine::RefreshAdamoDatiFile (CString strParamsPathName)
{
    /* calcoliamo il path del file */
    CAdamoDatiFile* padf= new CAdamoDatiFile;
    if (padf)   {
        /* apriamo i files */
        if (!padf->Open (strParamsPathName, TRUE))   {
            padf->Open (strParamsPathName, FALSE);
            padf->WriteData ();
        }
        else
			padf->ReadData ();
        /* ora eseguiamo il refresh dei dati */
        padf->Refresh (m_pModule);
        /* ora memorizziamo i dispositivi non ancora aggiornati */
        m_pModule->WriteFreeDevices (padf);
        padf->Close ();
        delete padf;
    }
}

/*
** SetFrequency : settaggio della frequenza.
*/
void CAdamoCfgMachine::SetFrequency (int nFrequency)
{
    switch (nFrequency)   {
        case FREQ2000 :
        case FREQ1136 :
        case FREQ1000 :
        case FREQ500 :
        case FREQ100 :
        case FREQ50 :
            m_nFrequency=nFrequency;
            break;
        default :
            m_nFrequency = FREQ50;
    }
}

/*
** SalvaDescrizioneTradotta : 
*/
int CAdamoCfgMachine::SalvaDescrizioneTradotta (CTraduzioni* pTr)
{
    return m_pModule->SalvaDescrizioneTradotta (pTr);
}

/*
** CaricaDescrizioneTradotta : 
*/
void CAdamoCfgMachine::CaricaDescrizioneTradotta (CTraduzioni* pTr)
{
    m_pModule->CaricaDescrizioneTradotta (pTr);
}

/*
** DeleteConfigData : 
*/
void CAdamoCfgMachine::DeleteConfigData ()
{
    _delete (m_pModule);
}

/*
** NewModuleObject : 
*/
void CAdamoCfgMachine::NewModuleObject (CObject *pParent)
{
    if (!m_pModule)
        m_pModule=new CAdamoLogicModule(pParent);
}

/*
** SetMachineType :
*/
void CAdamoCfgMachine::SetMachineType (eTypeMachine eTypeMachine)
{
    m_eNativeTypeMachine = m_eTypeMachine = eTypeMachine;
    if (m_eTypeMachine==enVirtual)
        SetFrequency (FREQ50);
    else
        if (m_eTypeMachine==enPhysical)
            SetFrequency (FREQ1000);
}

/*
** SaveConfigXML :
*/
void CAdamoCfgMachine::SaveConfigXML (stAdamoParametri *pAdamoParametri, CStdioFile *pStdioFile)
{
	WritePrologue (pStdioFile);
	m_pModule->IteratorXML (m_pModule, pAdamoParametri, pStdioFile);
	WriteEpilogue (pStdioFile);
}

/*
** WritePrologue :
*/
void CAdamoCfgMachine::WritePrologue (CStdioFile *pStdioFile)
{
	double afpXYZCBCompensation[NMAX_CORR_XYZCB];
	double afpTRTCompensation[NMAX_CORR_TRT];
	CString str;

	pStdioFile->WriteString ("<Machine>\n");
	str.Format ("<Name>%s</Name\n>", m_pModule->GetName ());
	pStdioFile->WriteString (str);
	str.Format ("<Description>%s</Description>\n", m_pModule->GetDesc ());
	pStdioFile->WriteString (str);
	str.Format ("<Rate>%d</Rate>\n", GetRate ());
	pStdioFile->WriteString (str);
	str.Format ("<Frequency>%d</Frequency>\n", GetFrequency ());
	pStdioFile->WriteString (str);
	str.Format ("<Epsilon>%.3f</Epsilon>\n", GetPrecision ());
	pStdioFile->WriteString (str);
	str.Format ("<EpsilonRaggi>%.3f</EpsilonRaggi>\n", GetEpsilonRaggi ());
	pStdioFile->WriteString (str);
	str.Format ("<ContAngle>%.3f</ContAngle>\n", GetContoureAngle ());
	pStdioFile->WriteString (str);
	str.Format ("<FilterMessagesOnStart>%d</FilterMessagesOnStart>\n", GetMessageFilteringOnStart ());
	pStdioFile->WriteString (str);
	str.Format ("<StopAxisOnError>%d</StopAxisOnError>\n", GetStopAxisOnError ());
	pStdioFile->WriteString (str);
	str.Format ("<InvertG2G3>%d</InvertG2G3>\n", GetSensoRotazioneCircolari ());
	pStdioFile->WriteString (str);
	str.Format ("<KAC>%.3f</KAC>\n", GetKAC ());
	pStdioFile->WriteString (str);
	str.Format ("<Smoothing>%.3f</Smoothing>\n", GetSmoothing ());
	pStdioFile->WriteString (str);
	str.Format ("<MachineType>%d</MachineType>\n", GetMachineType ());
	pStdioFile->WriteString (str);
	str.Format ("<ModuleType>%d</ModuleType>\n", GetTypeModule ());
	pStdioFile->WriteString (str);
	str.Format ("<ConnectionType>%d</ConnectionType>\n", GetConnectionType ());
	pStdioFile->WriteString (str);
	str.Format ("<ModuleNumber>%d</ModuleNumber>\n", GetHWModuleNumber ());
	pStdioFile->WriteString (str);
	str.Format ("<PortNumber>%d</PortNumber>\n", GetPort ());
	pStdioFile->WriteString (str);
	str.Format ("<IPAddress>%x.%x.%x.%x</IPAddress>\n",
		GetTcpIpAddress ()[0],
		GetTcpIpAddress ()[1],
		GetTcpIpAddress ()[2],
		GetTcpIpAddress ()[3]
	);
	pStdioFile->WriteString (str);
	str.Format ("<BowThickening>%.3f</BowThickening>\n", GetBowThickening ());
	pStdioFile->WriteString (str);
	str.Format ("<LinearThickening>%.3f</LinearThickening>\n", GetLinearThickening ());
	pStdioFile->WriteString (str);
	str.Format ("<VelProfile>%.3f</VelProfile>\n", GetVelProfile ());
	pStdioFile->WriteString (str);
	str.Format ("<AccProfile>%d</AccProfile>\n", GetAccProfile ());
	pStdioFile->WriteString (str);
	str.Format ("<DecProfile>%d</DecProfile>\n", GetDecProfile ());
	pStdioFile->WriteString (str);
	str.Format ("<AInterpDefault>%d</AInterpDefault>\n", GetAInterpDefault ());
	pStdioFile->WriteString (str);
	str.Format ("<SpikeDetector>%d</SpikeDetector>\n", GetSpikeDetector ());
	pStdioFile->WriteString (str);
	str.Format ("<MaxUTurnPrecision>%d</MaxUTurnPrecision>\n", GetMaxUTurnPrecision ());
	pStdioFile->WriteString (str);
	str.Format ("<SmootherType>%d</SmootherType>\n", GetSmootherType ());
	pStdioFile->WriteString (str);
	str.Format ("<SingleSpikeSmootherWeight>%.3f</SingleSpikeSmootherWeight>\n", GetSingleSpikeSmootherWeight ());
	pStdioFile->WriteString (str);
	str.Format ("<SingleSpikeFactor>%.3f</SingleSpikeFactor>\n", GetSingleSpikeFactor ());
	pStdioFile->WriteString (str);
	str.Format ("<GeometrySpikeFactor>%.3f</GeometrySpikeFactor>\n", GetGeometrySpikeFactor ());
	pStdioFile->WriteString (str);
	str.Format ("<DetectJointUTurn>%d</DetectJointUTurn>\n", GetDetectJointUTurn ());
	pStdioFile->WriteString (str);
	str.Format ("<StopOnAngle>%d</StopOnAngle>\n", GetStopOnAngle ());
	pStdioFile->WriteString (str);
	str.Format ("<MinStopOnAngle>%.3f</MinStopOnAngle>\n", GetMinStopAngle ());
	pStdioFile->WriteString (str);
	str.Format ("<RamDisk>%d</RamDisk>\n", GetRamDisk ());
	pStdioFile->WriteString (str);
	str.Format ("<RamDiskBufferSize>%d</RamDiskBufferSize>\n", GetRamDiskBufferSize ());
	pStdioFile->WriteString (str);
	str.Format ("<G0Fillet>%d</G0Fillet>\n", GetG0Raggiata ());
	pStdioFile->WriteString (str);
	str.Format ("<G0FilletFitting>%.3f</G0FilletFitting>\n", GetG0RaggiataInfittimento ());
	pStdioFile->WriteString (str);
	str.Format ("<G0FilletRadius>%.3f</G0FilletRadius>\n", GetG0RaggiataRaggio ());
	pStdioFile->WriteString (str);
	str.Format ("<SimulationMode>%d</SimulationMode>\n", GetSimulationMode ());
	pStdioFile->WriteString (str);
	str.Format ("<MaxJitter>%d</MaxJitter>\n", GetMaxJitter ());
	pStdioFile->WriteString (str);
	str.Format ("<ActualLookAHead>%d</ActualLookAHead>\n", GetSIP ());
	pStdioFile->WriteString (str);
	GetXYZCBCompensation (afpXYZCBCompensation);
	str.Format ("<OffsetToolHolderX>%.3f</OffsetToolHolderX>\n", afpXYZCBCompensation[0]);
	pStdioFile->WriteString (str);
	str.Format ("<OffsetToolHolderY>%.3f</OffsetToolHolderY>\n", afpXYZCBCompensation[1]);
	pStdioFile->WriteString (str);
	str.Format ("<RotationToolHolder>%.3f</RotationToolHolder>\n", afpXYZCBCompensation[2]);
	pStdioFile->WriteString (str);
	GetTRTCompensation (afpTRTCompensation);
	str.Format ("<TheoricXPivot>%.3f</TheoricXPivot>\n", afpTRTCompensation[0]);
	pStdioFile->WriteString (str);
	str.Format ("<TheoricYPivot>%.3f</TheoricYPivot>\n", afpTRTCompensation[1]);
	pStdioFile->WriteString (str);
	str.Format ("<TheoricZPivot>%.3f</TheoricZPivot>\n", afpTRTCompensation[2]);
	pStdioFile->WriteString (str);
	str.Format ("<OffsetX>%.3f</OffsetX>\n", afpTRTCompensation[3]);
	pStdioFile->WriteString (str);
	str.Format ("<OffsetY>%.3f</OffsetY>\n", afpTRTCompensation[4]);
	pStdioFile->WriteString (str);
	str.Format ("<OffsetZ>%.3f</OffsetZ>\n", afpTRTCompensation[5]);
	pStdioFile->WriteString (str);
	str.Format ("<IncrementalCenter>%d</IncrementalCenter>\n", GetCentroIncrementale ());
	pStdioFile->WriteString (str);
	str.Format ("<PlcBoost>%d</PlcBoost>\n", GetPlcBoost ());
	pStdioFile->WriteString (str);
	str.Format ("<StrictAbsControl>%d</StrictAbsControl>\n", GetStrictAbsPosControl ());
	pStdioFile->WriteString (str);
	str.Format ("<StrictAbsEncSteps>%d</StrictAbsEncSteps>\n", GetStrictAbsPosEncoderSteps ());
	pStdioFile->WriteString (str);
 }

/*
** WriteEpilogue :
*/
void CAdamoCfgMachine::WriteEpilogue (CStdioFile *pStdioFile)
{
	pStdioFile->WriteString ("</Machine>\n");
}

/*
** SaveExtendedData :
*/
void CAdamoCfgMachine::SaveExtendedData (CArchive &ar)
{
	CFile f;
	CString strExtCfgPathName;
	stCfgExtendedData cfged;

	strExtCfgPathName = GetExtCfgPathName (ar);
	if (strExtCfgPathName != "")   {
		if (!_access (strExtCfgPathName, 00))   {
			if (f.Open	(strExtCfgPathName, CFile::modeWrite | CFile::typeBinary))   {
				cfged.m_bSpikeDetector = m_bSpikeDetector;
				cfged.m_bMaxUTurnPrecision = m_bMaxUTurnPrecision;
				cfged.m_eSmootherType = m_eSmootherType;
				cfged.m_fpSingleSpikeSmootherWeight = m_fpSingleSpikeSmootherWeight;
				cfged.m_fpSingleSpikeFactor = m_fpSingleSpikeFactor;
				cfged.m_fpGeometrySpikeFactor = m_fpGeometrySpikeFactor;
				cfged.m_bSimulationMode = m_bSimulationMode;
				cfged.m_nMaxJitter = m_lMaxJitter;
				cfged.m_fpCountourAngle = m_fpContoureAngle;
				cfged.m_bCentroIncrementale = m_bCentroIncrementale;
				cfged.m_bDetectJointUTurn = m_bDetectJointUTurn;
				cfged.m_bStopOnAngle = m_bStopOnAngle;
				cfged.m_fpMinStopAngle = m_fpMinStopAngle;
				cfged.m_fpSIP = m_fpSIP;
				memcpy (cfged.m_afpGeometry_XYZCB, m_afpGeometry_XYZCB, sizeof (m_afpGeometry_XYZCB));
				memcpy (cfged.m_afpGeometry_TRT, m_afpGeometry_TRT, sizeof (m_afpGeometry_TRT));
				f.Write (&cfged, sizeof (stCfgExtendedData));
				f.Close ();
			}
		}
		else   {
			if (f.Open	(strExtCfgPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))   {
				cfged.m_bSpikeDetector = true;
				cfged.m_bMaxUTurnPrecision = false;
				cfged.m_eSmootherType = enSmootherParabolic;
				cfged.m_fpSingleSpikeSmootherWeight = SINGLE_SPIKE_SMOOTHER_WEIGHT;
				cfged.m_fpSingleSpikeFactor = SINGLE_SPIKE_FACTOR;
				cfged.m_fpGeometrySpikeFactor = GEOMETRY_SPIKE_FACTOR;
				cfged.m_bSimulationMode = false;
				cfged.m_nMaxJitter = 0;
				cfged.m_fpCountourAngle = 15.0;
				cfged.m_bCentroIncrementale = false;
				cfged.m_bDetectJointUTurn = false;
				cfged.m_bStopOnAngle = false;
				cfged.m_fpMinStopAngle = MIN_STOP_ANGLE;
				cfged.m_fpSIP = 0.0;
				memset (cfged.m_afpGeometry_XYZCB, 0, sizeof (m_afpGeometry_XYZCB));
				memset (cfged.m_afpGeometry_TRT, 0, sizeof (m_afpGeometry_TRT));
				f.Write (&cfged, sizeof (stCfgExtendedData));
				f.Close ();
			}
		}
	}
}

/*
** LoadExtendedData :
*/
void CAdamoCfgMachine::LoadExtendedData (CArchive &ar)
{
	CFile f;
	CFileStatus fs;
	CString strExtCfgPathName;
	stCfgExtendedData cfged;

	strExtCfgPathName = GetExtCfgPathName (ar);
	if (strExtCfgPathName != "")   {
		if (f.GetStatus (strExtCfgPathName, fs))   {
			int ext = sizeof (cfged.m_spare) + sizeof (cfged.m_fpCountourAngle) + sizeof (cfged.m_nMaxJitter) + sizeof (cfged.m_bSimulationMode);
			if (fs.m_size == sizeof (stCfgExtendedData) - ext)
				CFile::Remove (strExtCfgPathName);
		}
		if (f.Open	(strExtCfgPathName, CFile::modeRead | CFile::typeBinary))   {
			f.Read (&cfged, sizeof (stCfgExtendedData));
			m_bSpikeDetector = cfged.m_bSpikeDetector;
			m_bMaxUTurnPrecision = cfged.m_bMaxUTurnPrecision;
			m_eSmootherType = cfged.m_eSmootherType;
			m_fpSingleSpikeSmootherWeight = cfged.m_fpSingleSpikeSmootherWeight;
			m_fpSingleSpikeFactor = cfged.m_fpSingleSpikeFactor;
			m_fpGeometrySpikeFactor = cfged.m_fpGeometrySpikeFactor;
			m_bSimulationMode = cfged.m_bSimulationMode;
			m_lMaxJitter = cfged.m_nMaxJitter;
			m_fpContoureAngle = cfged.m_fpCountourAngle;
			m_bCentroIncrementale = cfged.m_bCentroIncrementale;
			m_bDetectJointUTurn = cfged.m_bDetectJointUTurn;
			m_bStopOnAngle = cfged.m_bStopOnAngle;
			m_fpMinStopAngle = cfged.m_fpMinStopAngle;
			m_fpSIP = cfged.m_fpSIP;
			memcpy (m_afpGeometry_XYZCB, cfged.m_afpGeometry_XYZCB, sizeof (m_afpGeometry_XYZCB));
			memcpy (m_afpGeometry_TRT, cfged.m_afpGeometry_TRT, sizeof (m_afpGeometry_TRT));
			f.Close ();
		}
		else   {
			if (f.Open	(strExtCfgPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))   {
				m_bSpikeDetector = cfged.m_bSpikeDetector = true;
				m_bMaxUTurnPrecision = cfged.m_bMaxUTurnPrecision = false;
				m_eSmootherType = cfged.m_eSmootherType = enSmootherParabolic;
				m_fpSingleSpikeSmootherWeight = cfged.m_fpSingleSpikeSmootherWeight = SINGLE_SPIKE_SMOOTHER_WEIGHT;
				m_fpSingleSpikeFactor = cfged.m_fpSingleSpikeFactor = SINGLE_SPIKE_FACTOR;
				m_fpGeometrySpikeFactor = cfged.m_fpGeometrySpikeFactor = GEOMETRY_SPIKE_FACTOR;
				m_bSimulationMode = cfged.m_bSimulationMode = false;
				m_lMaxJitter = cfged.m_nMaxJitter = 0;
				m_fpContoureAngle = cfged.m_fpCountourAngle = 15.0;
				m_bCentroIncrementale = cfged.m_bCentroIncrementale = false;
				m_bDetectJointUTurn = cfged.m_bDetectJointUTurn = false;
				m_bStopOnAngle = cfged.m_bStopOnAngle = false;
				m_fpMinStopAngle = cfged.m_fpMinStopAngle = MIN_STOP_ANGLE;
				m_fpSIP = 0.0;
				memset (m_afpGeometry_XYZCB, 0, sizeof (m_afpGeometry_XYZCB));
				memset (m_afpGeometry_TRT, 0, sizeof (m_afpGeometry_TRT));
				memset (cfged.m_spare, 0xff, sizeof (cfged.m_spare));
				f.Write (&cfged, sizeof (stCfgExtendedData));
				f.Close ();
			}
		}
	}
}

/*
** GetExtCfgPathName :
*/
CString CAdamoCfgMachine::GetExtCfgPathName (CArchive &ar)
{
	CString str = ar.GetFile ()->GetFilePath (), strPath;
	int n = str.ReverseFind ('.');
	if (n!= -1)   {
		strPath = str.Left (n);
		strPath += ".efg";
	}
	return strPath;
}

