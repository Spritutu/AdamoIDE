// Project.h: interface for the CProject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECT_H__07580CB8_BA8B_47B6_9813_78E51B8C971C__INCLUDED_)
#define AFX_PROJECT_H__07580CB8_BA8B_47B6_9813_78E51B8C971C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/* include files */
#include "ProjectFile.h"
#include "Preprocessor.h"
#include "CVariableContainer.h"
#include "CBrowserContainer.h"
#include "CSyntaxContainer.h"
#include "CFunctionsContainer.h"
#include "CObjectsContainer.h"
#include "AdamoMachine.h"
#include "AdamoTraduzione.h"
#include "RegAdamoHtml.h"

/* constants definitions */
#define LUA_TOBJECT 8

/* versioning */
#define HEADER_PROJECT_FILE "@@@__PROJECT__@@@"
#define VERSION_PROJECT_FILE 0x02090006

class CProjectNew;
class CWorkspaceWnd;
class CTreeViewFiles;
class CAdamoFileDati;

typedef CTypedPtrArray<CPtrArray, CProjectFile*> CProjectFileArray;

class CProject : public CObject
{
    DECLARE_SERIAL(CProject)
public:
	CProject();
	virtual ~CProject();
    void SaveDataConfig();
	void DeleteObject(CObject *pObj);
	void RemoveFile(CProjectFile* pPF);
	BOOL Close();
	BOOL New(CProjectNew &pn,CString strPlantPath);
	void UpdateDirectories();
	BOOL HasBreakPoint(const char* szFile, int nLine);
	BOOL PositionBreakPoints();
	void CleanIntermediateAndOutputFiles(bool bMode=false);
	BOOL CheckBeforeBuild();
	BOOL Compile(CProjectFile* pPF);
	BOOL BuildOutputFiles();
	BOOL BuildIntermediateFiles();
	BOOL Build();
	void SaveModified();
	void SaveModConfig();
	void ExportInFileXml(CString strPath);
	BOOL Save();
	BOOL Save(CString strPathName);
	BOOL SaveConfig(CString strPathName,BOOL bHardware = FALSE/*lo uso per l'hardware*/);
	BOOL SaveConfigXML (CString strPathName);
	void SaveHWConfigXML (CString strPathName);
	BOOL SaveConfig();
	BOOL Save(CArchive& ar);
	BOOL Load(CString strPathName, int nModule);
	bool LoadConfigData(CString strPathName, CString strParamsPathName);
	bool LoadProjectData(CArchive& ar);
	void RemoveProjectFiles();
	CProjectFile* AddFile(CString strPathName);
	void AddFile(CProjectFile* pPF);
	int NofFiles() { return m_files.GetSize(); };
	void Properties();
	void AddFiles();
	void ImportFiles();
	CProjectFile* GetProjectFile(CString strPathName, int* pnModule=NULL);
	CProjectFile* GetProjectFile(int nIndex);
	void RedrawFilesTree(CTreeViewFiles* pTree);
    CPreProcessor& GetPreprocessor() { return m_ppr; }
    CVariableContainer& GetVariableContainer () { return m_vc; }
    CBrowserContainer&  GetBrowserContainer ()  { return m_bc; }
    CSyntaxContainer&  GetSyntaxContainer ()    { return m_sc; }
    CFunctionsContainer&  GetFunctionsContainer () { return m_fc; }
    CObjectsContainer&  GetObjectsContainer ()  { return m_oc; }
    CTraduzioni* GetTraduzioni ()               { return &m_tr; }
    void CheckFiles ();
    void ClearGlobalVariables (const char* szModule);
    void ClearBrowserInfo (const char* szModule);
	void ClearFunctionsInfo (const char* szModule);
	void ClearObjectsInfo (const char* szModule);
	void SetPathName(CString strPathName) { m_strPathName=strPathName; }
    void SetIsModified(bool bIsModified) {   m_bIsModified = bIsModified;   }
	CString GetProjectDir();
	CString GetPathName() { return m_strPathName; };
	CString GetName();
	CString GetNameExt();
	CString GetDebugNameExt() { return m_strOutputPrefix + ".debug"; }
	CString GetReleaseNameExt() { return m_strOutputPrefix + ".release"; }
	CString GetListingNameExt() { return m_strOutputPrefix + ".listing"; }
	CString GetDebugPathNameExt() { return m_strOutputDirRoot + "\\" + GetDebugNameExt(); }
	CString GetReleasePathNameExt() { return m_strOutputDirRoot + "\\" + GetReleaseNameExt(); }
	CString GetListingPathNameExt() { return m_strOutputDirRoot + "\\" + GetListingNameExt(); }
	CString GetOutputDir() { return m_strOutputDirRoot; }
	CString GetIntermediateDir() { return m_strIntermediateDirRoot; }
    CString GetParamsPath()		{ return GetProjectDir(); }
    CString GetParamsPathName()   { return GetProjectDir() + "\\" + DEVICE_FILE; }
    CString GetHWConfigPathName() { return GetProjectDir() + "\\" + HWCONFIG_FILE; }
    CString GetSettingsPathName() { return GetProjectDir() + "\\" + MACHINE_SETTINGS_FILE; }
    CString GetLanguagePathName() { return GetProjectDir() + "\\" + GetName () + ".MDB"; }
    CString GetResourcesPathName(){ return GetProjectDir() + "\\" + RESOURCES_FILE; }
    CString GetCCPathName ()      { return GetProjectDir() + "\\"; }
    CString GetConfXMLPathName () { return GetProjectDir() + "\\" + GetName () + ".XML"; }
	CString GetEthercatPathName (){ return GetProjectDir() + "\\" + DIRETHERCAT; }
    CAdamoLogicModule* GetMachineModule(){ return ((CAdamoCfgMachine*)m_objMachine)->GetAdamoModule();};
    bool    GetIsModified() { return m_bIsModified; };
	BOOL    CreateDir(CString strPath);
	BOOL    CreateIntermediateDir();
	BOOL    CreateOutputDir();
    bool    IsMachineModified ()   { return ((CAdamoCfgMachine*)m_objMachine)->IsModified (); }
    bool    WasMachineEmpty ()      { return m_bIsMachineEmpty; }
    void    SetMachineModifiedFlag (bool bModified) { ((CAdamoCfgMachine*)m_objMachine)->SetModifiedFlag (bModified); if (!bModified) m_bIsMachineEmpty=GetMachineModule()->IsLogicModuleEmpty(); }
    CAdamoMachine* GetMachine ()     { return m_objMachine; }
    eTypeMachine   GetMachineType () { return ((CAdamoCfgMachine*)m_objMachine)->GetMachineType (); }
    void    SetMachineType (eTypeMachine TM)   { ((CAdamoCfgMachine*)m_objMachine)->SetMachineType (TM); }
    short   GetRate () { return ((CAdamoCfgMachine*)m_objMachine)->GetRate (); }
    void    SetRate (short nRate)   { ((CAdamoCfgMachine*)m_objMachine)->SetRate (nRate); }
    int     GetFrequency () { return ((CAdamoCfgMachine*)m_objMachine)->GetFrequency (); }
    void    SetFrequency (int nFrequency)   { ((CAdamoCfgMachine*)m_objMachine)->SetFrequency (nFrequency); }
    double  GetPrecision ()                     { return ((CAdamoCfgMachine*)m_objMachine)->GetPrecision (); }
    void    SetPrecision (double fpPrecision)   { ((CAdamoCfgMachine*)m_objMachine)->SetPrecision (fpPrecision); }
    double  GetEpsilonRaggi ()                  { return ((CAdamoCfgMachine*)m_objMachine)->GetEpsilonRaggi (); }
    void    SetEpsilonRaggi (double fpEpsilonRaggi)   { ((CAdamoCfgMachine*)m_objMachine)->SetEpsilonRaggi (fpEpsilonRaggi); }
    double  GetContoureAngle ()                 { return ((CAdamoCfgMachine*)m_objMachine)->GetContoureAngle (); }
    void    SetContureAngle (double fpContureAngle)   { ((CAdamoCfgMachine*)m_objMachine)->SetContureAngle (fpContureAngle);  }
    void    SetModuleNumber (int nModule)   { m_objMachine->SetModuleNumber (nModule); }
    int     GetModuleNumber ()              { return m_objMachine->GetModuleNumber (); }
    void    SetMessageFilteringOnStart (long lMessageFilteringOnStart)     { ((CAdamoCfgMachine*)m_objMachine)->SetMessageFilteringOnStart (lMessageFilteringOnStart); }
    int     GetMessageFilteringOnStart ()                { return ((CAdamoCfgMachine*)m_objMachine)->GetMessageFilteringOnStart (); }
    void    SetStopAxisOnError (long lStopAxisOnError)   { ((CAdamoCfgMachine*)m_objMachine)->SetStopAxisOnError (lStopAxisOnError); }
    int     GetStopAxisOnError ()              { return ((CAdamoCfgMachine*)m_objMachine)->GetStopAxisOnError (); }
    void    SetPrefetchSplineEnabled (long nPrefetchEnabled)   { ((CAdamoCfgMachine*)m_objMachine)->SetPrefetchSplineEnabled (nPrefetchEnabled); }
    int     GetPrefetchSplineEnabled ()              { return ((CAdamoCfgMachine*)m_objMachine)->GetPrefetchSplineEnabled (); }
	void	SetSensoRotazioneCircolari (long lSensoRotazioneCircolari)	{ ((CAdamoCfgMachine*)m_objMachine)->SetSensoRotazioneCircolari (lSensoRotazioneCircolari); }
	int		GetSensoRotazioneCircolari ()		{ return ((CAdamoCfgMachine*)m_objMachine)->GetSensoRotazioneCircolari (); }
	void	SetKAC (double fpKAC)				{ ((CAdamoCfgMachine*)m_objMachine)->SetKAC (fpKAC); }
	double	GetKAC ()							{ return ((CAdamoCfgMachine*)m_objMachine)->GetKAC (); }
	void	SetSmoothing (double fpSmoothing)	{ ((CAdamoCfgMachine*)m_objMachine)->SetSmoothing (fpSmoothing); }
	double	GetSmoothing ()						{ return ((CAdamoCfgMachine*)m_objMachine)->GetSmoothing (); }
	void	SetBowThickening (double fpBowThickening)	{ ((CAdamoCfgMachine*)m_objMachine)->SetBowThickening (fpBowThickening); }
	double	GetBowThickening ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetBowThickening (); }
	void	SetLinearThickening (double fpLinearThickening)	{ ((CAdamoCfgMachine*)m_objMachine)->SetLinearThickening (fpLinearThickening); }
	double	GetLinearThickening ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetLinearThickening (); }
	void	SetVelProfile (double fpVelProfile)	{ ((CAdamoCfgMachine*)m_objMachine)->SetVelProfile (fpVelProfile); }
	double	GetVelProfile ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetVelProfile (); }
	void	SetAccProfile (long dwAccProfile)	{ ((CAdamoCfgMachine*)m_objMachine)->SetAccProfile (dwAccProfile); }
	long	GetAccProfile ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetAccProfile (); }
	void	SetDecProfile (long dwDecProfile)	{ ((CAdamoCfgMachine*)m_objMachine)->SetDecProfile (dwDecProfile); }
	long	GetDecProfile ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetDecProfile (); }
	void	SetAInterpDefault (long nAInterpDefault)	{ ((CAdamoCfgMachine*)m_objMachine)->SetAInterpDefault (nAInterpDefault); }
	long	GetAInterpDefault ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetAInterpDefault (); }
	double	GetSingleSpikeFactor ()						{ return ((CAdamoCfgMachine*)m_objMachine)->GetSingleSpikeFactor (); }
	void	SetSingleSpikeFactor (double SSF)			{ ((CAdamoCfgMachine*)m_objMachine)->SetSingleSpikeFactor (SSF); }
	double	GetGeometrySpikeFactor ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetGeometrySpikeFactor (); }
	void	SetGeometrySpikeFactor (double MSF)			{ ((CAdamoCfgMachine*)m_objMachine)->SetGeometrySpikeFactor (MSF); }
	bool    GetSpikeDetector ()							{ return ((CAdamoCfgMachine*)m_objMachine)->GetSpikeDetector (); }
	void    SetSpikeDetector (bool SpikeDetector)		{ ((CAdamoCfgMachine*)m_objMachine)->SetSpikeDetector (SpikeDetector); }
	bool    GetMaxUTurnPrecision ()						{ return ((CAdamoCfgMachine*)m_objMachine)->GetMaxUTurnPrecision (); }
	void    SetMaxUTurnPrecision (bool MaxUTurnPrecision)	{ ((CAdamoCfgMachine*)m_objMachine)->SetMaxUTurnPrecision (MaxUTurnPrecision); }
	enSmootherType GetSmootherType ()					{ return ((CAdamoCfgMachine*)m_objMachine)->GetSmootherType (); }
	void    SetSmootherType (enSmootherType SmootherType)	{ ((CAdamoCfgMachine*)m_objMachine)->SetSmootherType (SmootherType); }
	double  GetSingleSpikeSmootherWeight ()				{ return ((CAdamoCfgMachine*)m_objMachine)->GetSingleSpikeSmootherWeight (); }
	void    SetSingleSpikeSmootherWeight (double SingleSpikeSmootherWeight)		{ ((CAdamoCfgMachine*)m_objMachine)->SetSingleSpikeSmootherWeight (SingleSpikeSmootherWeight); }
	bool    GetDetectJointUTurn ()						{ return ((CAdamoCfgMachine*)m_objMachine)->GetDetectJointUTurn (); }
	void    SetDetectJointUTurn (bool DetectJointUTurn)	{ ((CAdamoCfgMachine*)m_objMachine)->SetDetectJointUTurn (DetectJointUTurn); }
	bool    GetStopOnAngle ()							{ return ((CAdamoCfgMachine*)m_objMachine)->GetStopOnAngle (); }
	void    SetStopOnAngle (bool StopOnAngle)			{ ((CAdamoCfgMachine*)m_objMachine)->SetStopOnAngle (StopOnAngle); }
	double  GetMinStopAngle ()							{ return ((CAdamoCfgMachine*)m_objMachine)->GetMinStopAngle (); }
	void    SetMinStopAngle (double MinStopAngle)		{ ((CAdamoCfgMachine*)m_objMachine)->SetMinStopAngle (MinStopAngle); }
	bool    GetRamDisk ()						{ return ((CAdamoCfgMachine*)m_objMachine)->GetRamDisk () != 0; }
	void    SetRamDisk (bool bEnable)			{ ((CAdamoCfgMachine*)m_objMachine)->SetRamDisk (bEnable); }
	int     GetRamDiskBufferSize ()				{ return ((CAdamoCfgMachine*)m_objMachine)->GetRamDiskBufferSize (); }
	void    SetRamDiskBufferSize (int nRamDiskBufferSize)	{ ((CAdamoCfgMachine*)m_objMachine)->SetRamDiskBufferSize (nRamDiskBufferSize); }
	int     GetG0Raggiata ()												{ return ((CAdamoCfgMachine*)m_objMachine)->GetG0Raggiata (); }
	void    SetG0Raggiata (bool bG0Raggiata)								{ ((CAdamoCfgMachine*)m_objMachine)->SetG0Raggiata (bG0Raggiata); }
	double  GetG0RaggiataInfittimento ()									{ return ((CAdamoCfgMachine*)m_objMachine)->GetG0RaggiataInfittimento (); }
	void    SetG0RaggiataInfittimento (double fpG0RaggiataInfittimento)		{ ((CAdamoCfgMachine*)m_objMachine)->SetG0RaggiataInfittimento (fpG0RaggiataInfittimento); }
	double  GetG0RaggiataRaggio ()											{ return ((CAdamoCfgMachine*)m_objMachine)->GetG0RaggiataRaggio (); }
	void    SetG0RaggiataRaggio (double fpG0RaggiataRaggio)					{ ((CAdamoCfgMachine*)m_objMachine)->SetG0RaggiataRaggio (fpG0RaggiataRaggio); }

	bool    GetSimulationMode ()											{ return ((CAdamoCfgMachine*)m_objMachine)->GetSimulationMode (); }
	void    SetSimulationMode (bool bSimulationMode)						{ ((CAdamoCfgMachine*)m_objMachine)->SetSimulationMode (bSimulationMode); }
	int     GetMaxJitter ()													{ return ((CAdamoCfgMachine*)m_objMachine)->GetMaxJitter (); }
	void    SetMaxJitter (int nMaxJitter)									{ ((CAdamoCfgMachine*)m_objMachine)->SetMaxJitter (nMaxJitter); }
	bool    GetCentroIncrementale ()										{ return ((CAdamoCfgMachine*)m_objMachine)->GetCentroIncrementale (); }
	void    SetCentroIncrementale (bool bCentroIncrementale)				{ ((CAdamoCfgMachine*)m_objMachine)->SetCentroIncrementale (bCentroIncrementale); }
	int     GetPlcBoost ()													{ return ((CAdamoCfgMachine*)m_objMachine)->GetPlcBoost (); }
	void    SetPlcBoost (int nPlcBoost)										{ ((CAdamoCfgMachine*)m_objMachine)->SetPlcBoost (nPlcBoost); }
	double  GetSIP ()														{ return ((CAdamoCfgMachine*)m_objMachine)->GetSIP (); }
	void    SetSIP (double SIP)												{ ((CAdamoCfgMachine*)m_objMachine)->SetSIP (SIP); }
	void    GetXYZCBCompensation (double* afpC)								{ ((CAdamoCfgMachine*)m_objMachine)->GetXYZCBCompensation (afpC); }
	void    SetXYZCBCompensation (double* afpC)								{ ((CAdamoCfgMachine*)m_objMachine)->SetXYZCBCompensation (afpC); }
	void    GetTRTCompensation (double* afpC)								{ ((CAdamoCfgMachine*)m_objMachine)->GetTRTCompensation (afpC); }
	void    SetTRTCompensation (double* afpC)								{ ((CAdamoCfgMachine*)m_objMachine)->SetTRTCompensation (afpC); }
	bool    GetStrictAbsPosControl ()										{ return ((CAdamoCfgMachine*)m_objMachine)->GetStrictAbsPosControl (); }
	void    SetStrictAbsPosControl (bool bSAPC)								{ ((CAdamoCfgMachine*)m_objMachine)->SetStrictAbsPosControl (bSAPC); }
	int     GetStrictAbsPosEncoderSteps ()									{ return ((CAdamoCfgMachine*)m_objMachine)->GetStrictAbsPosEncoderSteps (); }
	void    SetStrictAbsPosEncoderSteps (int nSAPES)						{ ((CAdamoCfgMachine*)m_objMachine)->SetStrictAbsPosEncoderSteps (nSAPES); }
	void	CreateByType();
    CAdamoMachineSettings* GetSettings ()  { return &m_fSettings; }
    void    LanciaTemporary (CString strFile, CString strName);
    void    TerminaTemporary (CString strName);
    void    ReloadConfiguration ();
    bool    StoreConfigData ();
    int     ChangeLanguage ();
    void    GestisciMappaEId ();
    void    StartCompilation ();
    void    EndCompilation ()           { m_bInCompilation=false; m_pCompiledFile=NULL; m_vc.Save (); }
    void    AbortCompilation ()         { m_bAbortCompilation=true; m_ppr.AbortCompilation (); }
    bool    IsCompilationAborted ()     { return m_bAbortCompilation; }
    bool    IsCompiling ()              { return m_bInCompilation; }
    CString GetCompilingFile ()         { CString str; if (m_pCompiledFile) str=m_pCompiledFile->GetPathName(); return str; }
	BOOL	SyntaxCheckingEnabled ()	{ return m_bSyntaxChecking; }
    int     CompilaMain ();
    ePswLevel GetPswLevel ()            { return m_ePswLevel; }
	bool    LoadEthercatXMLFile (CString strPathName="");
	CPtrArray &GetSlaveArray ()			{ return m_slaveArray; }
	void	ClearSlaveArray ();
	void    SaveGlobalVariables ();
	void	SaveBrowserInfo ();
    int     ReconnectMachine (eTypeConnection eTC);
    void	SetHWConfigModified ()      { m_bConfigHWModified=true; }
    void	ResetHWConfigModified ()    { m_bConfigHWModified=false; }
    bool	IsHWConfigModified ()       { return m_bConfigHWModified; }
	void    LoadHWConfig ();
	void    SaveHWConfig ();
	void	CloseHWConfig ();
	CAdamoDatiFile* GetAdamoDatiFile () { return m_adf; }
	CAdamoHWConfigFile* GetHWConfigFile () { return m_hwcf; }
	int		GetActiveMode ()			{ return m_nActiveMode; }
	void	SetActiveMode (int nMode)	{ m_nActiveMode = nMode; }
	void	ImportFromXml (CString strProjectDir, CString strIntermediateDir, CString strName);
	void	ImportFromFileXml (CString strProjectDir, CString strIntermediateDir, CString strName, CString strPath);
	void    AddFunctionsLibrary (CAdamoLibrary* pLibrary);
	void    AddObjectsLibrary (CObjectsLibrary* pObjectsLibrary);
	void	ClearProjectInfo();
	void	AddLibrary ();
	CStringList& GetLibraries ()		{ return m_listLibraries; }
	void GetProjectFiles (CStringList& listProjectFiles);
protected:
    bool m_bIsModified;
    bool m_bConfigHWModified;
    bool m_bIsMachineEmpty;
	CProjectFileArray	m_files;
	CString m_strPathName;
	CString m_strIntermediateDir;
	CString m_strOutputDir;
	CString m_strOutputPrefix;
	BOOL m_bGenerateListing;
	BOOL m_bSyntaxChecking;
	BOOL m_bPropagateMessages;
	CString m_strOutputDirRoot, m_strIntermediateDirRoot;
    CPreProcessor m_ppr;
    CVariableContainer m_vc;
    CBrowserContainer  m_bc;
	CSyntaxContainer m_sc;
	CFunctionsContainer m_fc;
	CObjectsContainer m_oc;
    CTraduzioni m_tr;
    CAdamoMachine* m_objMachine;
    CAdamoMachineSettings  m_fSettings;
    bool m_bInCompilation;
    bool m_bAbortCompilation;
    CProjectFile *m_pCompiledFile;
    ePswLevel     m_ePswLevel;
	CPtrArray  m_slaveArray;
    CAdamoDatiFile      *m_adf;
    CAdamoHWConfigFile  *m_hwcf;
	int	m_nActiveMode;
	DocXmlPtr m_pDocXml;
	int m_nLevel;
	CStringList m_listLibraries;
private :
    void GenerateMainObjFile ();
	void GenerateOOPFile ();
	void DeleteMainObjFile ();
	void DeleteOOPFile ();
    bool CheckMainModified ();
    void DeleteConfigData ();
    void LoadPswLevel ();
    void GestisciVersione ();
    int  From1_1_1To1_1_3 ();
    int  From1_1_3To2_0_0 ();
    int  From2_0_0To2_6_0 ();
	int  FillSlaveInfo (int i, MSXML2::IXMLDOMNode *pSlave);
	void FillSlaveVariables (int i, MSXML2::IXMLDOMNode *pVariable, int nInOut);
	int  ExtractDataType (const char *strDataType);
	int  AssignToSlave (stAdamoEthSlaveVariable *pEthVariable);
	void LoadHWConfiguration (CAdamoHWConfigFile *pHWConfigFile);
	void UpdateXMLDocking ();
    bool ApriFileDiConfigurazione (CString str, CFile *fout);
    int  OpenMachine ();
	void CheckHWConfigFile (CPath pPath);
	void ErroreDiConnessione ();
	void WritePrologue (CStdioFile *pStdioFile);
	void WriteEpilogue (CStdioFile *pStdioFile);
	void GetTcpIpAddress (CString strTcpIPAddress, BYTE &a, BYTE &b, BYTE &c, BYTE &d);
	void BackupImportedFiles (CString strProjectDir, CString strIntermediateDir);
	void ImportCfgFile (CString strProjectDir, CString strName, CString strPath);
	void ImportHWFile (CString strProjectDir, CString strName, CString strPath);
	void ParseCfgFile (DocXmlPtr pDocXml, CString strProjectDir, CString strName);
	void ParseHWFile (DocXmlPtr pDocXml, CString strProjectDir, CString strName);
	int  ImportMachineData (ElementXmlPtr pEl, CAdamoCfgMachine *pMachine, CString strProjectDir);
	int  IteraGruppo (ElementXmlPtr pEl, CAdamoCfgMachine *pMachine, CAdamoGroup *pParent, CAdamoDatiFile *pFileDati, CString strProjectDir);
	int  ImportGroup (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoGroup *pParent, CAdamoDatiFile *pFileDati);
	int  SetBaseData (ElementXmlPtr pEl, CAdamoBase *pBase);
	void NewDigitalInput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati);
	void NewDigitalOutput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati);
	void NewAnalogInput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati);
	void NewAnalogOutput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati);
	void NewAxis (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati);
	void NewTimer (ElementXmlPtr pEl, CAdamoGroup *pGroup);
	void NewCounter (ElementXmlPtr pEl, CAdamoGroup *pGroup);
	int  SalvaNuovaMacchina (CAdamoCfgMachine* pMachine, CString strProjectDir, CString strName);
	void RicaricaNuovaMacchina ();
	void SaveFunctionsInfo ();
	void SaveObjectsInfo ();
	void LoadLibraries (CArchive &ar);
	void SaveLibraries (CArchive &ar);
	void InstallLibraries ();
	int  ImportHWCanOpen (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nCanale);
	int  ImportHWSSCNET (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nCanale);
	int  ImportHWCanOpenNode (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nNode, int nCanale);
	int  ImportHWEthercat (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile);
	int  ImportHWCanOpenInputAnalogue (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nNode, int nCanale, int nAnalogue);
	int  ImportHWCanOpenOutputAnalogue (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nNode, int nCanale, int nAnalogue);
	int  ImportHWEthercatAxis (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nAsse);
	int  ImportHWEthercatAxisData (ElementXmlPtr pElChild, stHWEthAxData& HWEthAxData);
	int  ImportHWEthercatSlave (ElementXmlPtr pEl, stHWEthSlave& HWEthSlaveData);
	int  ImportHWEthercatSlaveAnalogue (ElementXmlPtr pEl, stHWEthSlave& HWEthSlaveData, int nAnalogue);
	int  SaveHWImportedFile (CAdamoHWConfigFile *pHWConfigFile);
};

#endif // !defined(AFX_PROJECT_H__07580CB8_BA8B_47B6_9813_78E51B8C971C__INCLUDED_)
