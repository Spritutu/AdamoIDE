// Project.cpp: implementation of the CProject class.
//
//////////////////////////////////////////////////////////////////////

/* include files */
#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "Project.h"
#include "path.h"
#include "WorkspaceWnd.h"
#include "TreeViewFiles.h"
#include "LuaDoc.h"
#include "LuaView.h"
#include "MainFrame.h"
#include "ProjectProperties.h"
#include "ScintillaView.h"
#include "Executor.h"
#include "ProjectNew.h"
#include "KSFileDialog.h"
#include "adapsw.h"
#include "AdamoIdeEthercat.h"
#include "AdamoVersion.h"
#include "AdamoSink.h"
#include "DlgLuaLibraries.h"
#include "AdamoLibrary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* constants definitions */
#define _MAINOBJ "_mainobj.out"
#define _OOPOBJ  "_oop.out"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CProject, CObject, 1)

CProject::CProject() : m_objMachine (NULL), m_bIsModified (false), m_bIsMachineEmpty (true), m_bAbortCompilation (false), 
	m_bInCompilation (false), m_pCompiledFile (NULL), m_ePswLevel (ePswKinema), m_bConfigHWModified (false),
	m_adf (NULL), m_hwcf (NULL), m_nActiveMode (-1), m_nLevel (0)
{
    m_ppr.SetVariableContainer (&m_vc);
    m_ppr.SetBrowserContainer (&m_bc);
	m_ppr.SetSyntaxContainer (&m_sc);
}

CProject::~CProject()
{
}

//--------------------------------------------------------------------------------------------------
//- file and directory functions
//--------------------------------------------------------------------------------------------------

CString CProject::GetName()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	return CString(fname);
}

CString CProject::GetNameExt()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	return CString(fname)+ext;
}

BOOL CProject::CreateDir(CString strPath)
{
	CreateDirectory(strPath, NULL);
	return TRUE;
}

BOOL CProject::CreateIntermediateDir()
{
	CreateDirectory(m_strIntermediateDirRoot, NULL);
	return TRUE;
}

BOOL CProject::CreateOutputDir()
{
	CreateDirectory(m_strOutputDirRoot, NULL);
	return TRUE;
}

CString CProject::GetProjectDir()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( m_strPathName, drive, dir, fname, ext );
	if ( dir[strlen(dir)-1]=='\\' )
		dir[strlen(dir)-1] = '\0';
	return CString(drive)+dir;	
}

//--------------------------------------------------------------------------------------------------
//- project files functions
//--------------------------------------------------------------------------------------------------

void CProject::RedrawFilesTree(CTreeViewFiles* pTree)
{
	pTree->AddRoot(GetName(),FALSE);

	int nFiles = m_files.GetSize();
	for ( int i=0; i<nFiles; ++i )
		pTree->AddProjectFile(m_files[i]->GetNameExt(), (long)m_files[i]);
}


CProjectFile* CProject::GetProjectFile(CString strPathName, int* pnModule)
{
	int nSize = m_files.GetSize();
	for ( int i=0; i<nSize; ++i )
		if ( m_files[i]->HasFile(strPathName) )    {
            if (pnModule)
                *pnModule=i;
			return m_files[i];
        }
	return NULL;
}

CProjectFile* CProject::GetProjectFile(int nIndex)
{
    if (nIndex<m_files.GetSize())
    	return m_files[nIndex];
	return NULL;
}

void CProject::AddFiles()
{
	CKSFileDialog fd(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, 
		"Adamo files (*.ada)|*.ada|All files (*.*)|*.*||", AfxGetMainWnd());

	if ( fd.DoModal()==IDOK )
	{
		POSITION pos = fd.GetStartPosition();
		while (pos)
		{
			CString strPathName = fd.GetNextPathName(pos);
			AddFile(strPathName);
		}
	}

}
void CProject::ImportFiles()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	CKSFileDialog fd(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, 
		LOADSTRING (IDS_ADAMOPROJECT_1), AfxGetMainWnd());

	if ( fd.DoModal()==IDOK )
	{
		POSITION pos = fd.GetStartPosition();
		while (pos)
		{
			CString strPathName = fd.GetNextPathName(pos);

	        _splitpath( strPathName, drive, dir, fname, ext );
	        if ( dir[strlen(dir)-1]=='\\' )
		        dir[strlen(dir)-1] = '\0';
	        CString strPath=CString(drive)+dir;	
            CString strProjectDir=GetProjectDir ();
            CString strFileNew=strProjectDir+"\\"+fd.GetFileName ();
            strPath.MakeUpper ();
            strProjectDir.MakeUpper ();
            if (strPath!=strProjectDir)   {
                if (CopyFile (strPathName, strFileNew, TRUE))
			        AddFile(strFileNew);
                else    {
                    DWORD dw=GetLastError ();
                    char szBuffer[256];
                    if (dw==ERROR_FILE_EXISTS)   {
                        int nMod;
                        if (GetProjectFile (strFileNew, &nMod))
                            AfxMessageBox (LOADSTRING (IDS_ADAMOPROJECT_2), MB_ICONSTOP);
                        else   {
                            if (AfxMessageBox (LOADSTRING (IDS_ADAMOPROJECT_3), MB_YESNO|MB_ICONQUESTION)==IDYES)   {
                                if (CopyFile (strPathName, strFileNew, FALSE))
			                        AddFile(strFileNew);
                                else    {
                                    dw=GetLastError ();
                                    sprintf (szBuffer, (const char *)LOADSTRING (IDS_ADAMOPROJECT_4), dw);
                                    AfxMessageBox (szBuffer, MB_ICONSTOP);
                                }
                            }
                        }
                    }
                    else   {
                        sprintf (szBuffer, (const char *)LOADSTRING (IDS_ADAMOPROJECT_4), dw);
                        AfxMessageBox (szBuffer, MB_ICONSTOP);
		            }
		        }
		    }
            else
                AddFile(strFileNew);
		}
	}
}

CProjectFile* CProject::AddFile(CString strPathName)
{
 	CProjectFile* pPF;
	if ( (pPF=GetProjectFile(strPathName)) != NULL )
		return pPF;

	pPF = new CProjectFile;
	pPF->SetPathName(strPathName,this);

	AddFile(pPF);
    return pPF;
}

void CProject::AddFile(CProjectFile* pPF)
{
	m_files.Add(pPF);

	CWorkspaceWnd* pWorkspace = ((CMainFrame*)AfxGetMainWnd())->GetWorkspaceWnd();
	CTreeViewFiles* pTree = pWorkspace->GetTreeViewFiles();
	pTree->AddProjectFile(pPF->GetNameExt(), (long)pPF);

    SetIsModified (true);
}

void CProject::RemoveFile(CProjectFile *pPF)
{
	int nSize = m_files.GetSize();
	for ( int i=0; i<nSize; ++i )
		if ( m_files[i] == pPF )
		{
			m_files.RemoveAt(i);
            // da verificare se da problemi
            delete pPF;
			break;
		}

    SetIsModified (true);
}

void CProject::RemoveProjectFiles()
{
	int nSize = m_files.GetSize(), i;
	for ( i=0; i<nSize; ++i )
	{
		CLuaView *pView = theApp.FindProjectFilesView(m_files[i]);
		if ( pView )
			pView->CloseFrame();
	}

	for ( i=0; i<nSize; ++i )
		delete m_files[i];

	m_files.RemoveAll();
}

void CProject::Properties()
{
	CProjectProperties dlg;

	dlg.m_bGenerateListing = m_bGenerateListing;
	dlg.m_bSyntaxChecking = m_bSyntaxChecking;
	dlg.m_bPropagateMessages = m_bPropagateMessages;
	dlg.m_strIntermediateDir = m_strIntermediateDir;
	dlg.m_strOutputDir = m_strOutputDir;
	dlg.m_strOutputPrefix = m_strOutputPrefix;
	if ( dlg.DoModal() == IDOK )
	{
		m_bGenerateListing = dlg.m_bGenerateListing;
		m_strIntermediateDir = dlg.m_strIntermediateDir;
		m_strOutputDir = dlg.m_strOutputDir;
		m_strOutputPrefix = dlg.m_strOutputPrefix;
		m_bSyntaxChecking = dlg.m_bSyntaxChecking;
		m_bPropagateMessages = dlg.m_bPropagateMessages;
		m_tr.PropagaMessaggio (m_bPropagateMessages != 0, true);
		m_listLibraries.RemoveAll ();
		for (POSITION pos = dlg.m_listLibraries.GetHeadPosition (); pos; )   { 
			CString str = dlg.m_listLibraries.GetNext (pos);
			m_listLibraries.AddTail (str);
		}
		InstallLibraries ();
		UpdateDirectories();
        SetIsModified (true);
	}
}

void CProject::UpdateDirectories()
{
	if ( !PathIsRelative(m_strOutputDir) )
	{
		PathRemoveBackslash(m_strOutputDir.GetBuffer(0));
		m_strOutputDir.ReleaseBuffer();
		m_strOutputDirRoot = m_strOutputDir;
	}
	else
	{
		m_strOutputDirRoot = GetProjectDir();
		PathAppend(m_strOutputDirRoot.GetBuffer(MAX_PATH), m_strOutputDir);
		m_strOutputDirRoot.ReleaseBuffer();
		PathRemoveBackslash(m_strOutputDirRoot.GetBuffer(0));
		m_strOutputDirRoot.ReleaseBuffer();
	}

	if ( !PathIsRelative(m_strIntermediateDir) )
	{
		PathRemoveBackslash(m_strIntermediateDir.GetBuffer(0));
		m_strIntermediateDir.ReleaseBuffer();
		m_strIntermediateDirRoot = m_strIntermediateDir;
	}
	else
	{
		m_strIntermediateDirRoot = GetProjectDir();
		PathAppend(m_strIntermediateDirRoot.GetBuffer(MAX_PATH), m_strIntermediateDir);
		m_strIntermediateDirRoot.ReleaseBuffer();
		PathRemoveBackslash(m_strIntermediateDirRoot.GetBuffer(0));
		m_strIntermediateDirRoot.ReleaseBuffer();
	}
	m_ppr.SetIntermediateDir (m_strIntermediateDirRoot);
}

//--------------------------------------------------------------------------------------------------
//- project new/save/load/close functions
//--------------------------------------------------------------------------------------------------


BOOL CProject::New(CProjectNew &pn,CString strPlantPath)
{
    CWorkspaceWnd* pWorkspaceWnd = ((CMainFrame*)AfxGetMainWnd())->GetWorkspaceWnd();
	CTreeViewFiles* pTree = pWorkspaceWnd->GetTreeViewFiles();

    CreateDir(strPlantPath+"\\"+pn.GetProjectName());
	m_strPathName = strPlantPath+"\\"+pn.GetProjectName()+"\\"+pn.GetProjectName()+".lpr";
	m_strOutputPrefix = pn.GetProjectName();
    m_vc.SetProjectDirectory (GetProjectDir ());
    m_bc.SetProjectDirectory (GetProjectDir ());

	m_strOutputDir = "Output";
	m_strIntermediateDir = "Intermediate";
	m_bGenerateListing = FALSE;
	m_bSyntaxChecking = FALSE;
	m_bPropagateMessages = FALSE;
    if (!m_objMachine)   {
        m_objMachine=new CAdamoVMachine(this);
        ((CAdamoCfgMachine*)m_objMachine)->GetAdamoModule()->SetName(GetName());
        ((CAdamoCfgMachine*)m_objMachine)->GetAdamoModule()->SetParentObj(NULL);
        ((CAdamoCfgMachine*)m_objMachine)->SetMachineType (pn.GetTypeMachine ());
        ((CAdamoCfgMachine*)m_objMachine)->SetAdamoOptions (((CIdeApp*)AfxGetApp ())->GetAdamoOptions ());
        m_objMachine->SetModuleNumber (pn.GetModuleNumber ());
        m_objMachine->OpenMachine (GetProjectDir());
    }
    
	m_ppr.SetAdamoMachine (m_objMachine);
    m_ppr.SetTraduzioniObj (&m_tr);

    UpdateDirectories();

    m_tr.CreaFileDiLingua (GetLanguagePathName ());
    m_tr.ApriFileDiLingua (GetLanguagePathName (), 0);
    /* settiamo l'oggetto delle traduzioni nella macchina corrente */
    ((CAdamoCfgMachine*)m_objMachine)->SetTraduzioniObj (&m_tr);

    CAdamoDatiFile* padf=new CAdamoDatiFile;
    if (padf)   {
        padf->Open (GetParamsPathName (), 0);
        padf->Close ();
        delete padf;
    }

    CAdamoHWConfigFile* hwcf=new CAdamoHWConfigFile;
    if (hwcf)   {
        hwcf->Open (GetHWConfigPathName (), 0);
        hwcf->Close ();
        delete hwcf;
    }

    SetMachineModifiedFlag (true);

    Save();
    return TRUE;
}

BOOL CProject::Close()
{
	SaveModified();

    m_tr.ChiudiFileDiLingua ();
    SaveBrowserInfo ();
	SaveFunctionsInfo ();
	SaveObjectsInfo ();

	RemoveProjectFiles();

	m_strPathName = "";

    if (m_objMachine) {
        m_objMachine->CloseMachine ();
        delete m_objMachine;
        m_objMachine=NULL;
    }
	ClearSlaveArray ();
	CloseHWConfig ();
    return TRUE;
}

BOOL CProject::Load(CString strPathName, int nModule)
{
	CFile fin;
    BOOL nB;

    if (!m_objMachine)   {
        if (fin.Open(strPathName, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))   {
            /* carichiamo i dati del progetto */
            m_objMachine=new CAdamoVMachine (this);
            m_objMachine->SetModuleNumber (nModule);
			((CAdamoCfgMachine*)m_objMachine)->SetAdamoOptions (((CIdeApp*)AfxGetApp ())->GetAdamoOptions ());
            /* caricamento della configurazione */
	        CArchive ar(&fin, CArchive::load);
	        nB = LoadProjectData (ar);
            ar.Close ();
            fin.Close ();
            /* verifica di file e directory */
            SetIsModified (false);
	        UpdateDirectories();
            CheckFiles ();
			/* apriamo la configurazione hw */
			LoadHWConfig ();
            /* settiamo il preprocessore */
	        m_ppr.SetAdamoMachine (m_objMachine);
            m_ppr.SetTraduzioniObj (&m_tr);
            /* variabili globali */
            m_vc.SetProjectDirectory (GetProjectDir ());
            m_vc.Load ();
            /* browser */
            m_bc.SetProjectDirectory (GetProjectDir ());
            m_bc.Load ();
            /* function native dictionary */
            m_sc.Load ();
            /* function custom dictionary */
			m_fc.SetPath (GetProjectDir ());
            m_fc.Load ();
            /* Objects custom dictionary */
			m_oc.SetPath (GetProjectDir ());
            m_oc.Load ();
			/* aggiorniamo la visualizzazione dei dati del progetto */
            ((CMainFrame*)AfxGetMainWnd())->UpdateActiveProject();
            /* Password level */
            LoadPswLevel ();
			/* ethercat */
			LoadEthercatXMLFile ();
            /* apriamo la macchina */
            if (!OpenMachine ())
				ErroreDiConnessione ();
			/* aggiorniamo le docking bars */
			((CMainFrame*)AfxGetMainWnd())->LoadDockingProject (this);
        }
        else    {
		    AfxMessageBox(LOADSTRING (IDS_ADAMOPROJECT_5));
		    nB = FALSE;
        }
	}
    else
        nB=FALSE;
	return nB;
}

BOOL CProject::LoadConfigData (CString strPathName, CString strParamsPathName)
{
	CFile fin;
    CFileException e;
	if (!_access (strPathName, 00))   {
		if (!fin.Open(strPathName, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone,&e))   {
			AfxMessageBox(LOADSTRING (IDS_ADAMOPROJECT_5));
			return FALSE;
		}
		CArchive ar(&fin, CArchive::load);
		if (m_objMachine)   {
			((CAdamoCfgMachine*)m_objMachine)->Serialize(ar);
			((CAdamoCfgMachine*)m_objMachine)->GestisciMappaEId (strParamsPathName);
			GestisciVersione ();
			m_bIsMachineEmpty=((CAdamoCfgMachine*)m_objMachine)->IsLogicModuleEmpty();
		}
		ar.Close ();
		fin.Close ();
	}
    return TRUE;
}

BOOL CProject::LoadProjectData (CArchive &ar)
{
	CString strHeaderFile;
    int nFiles, nFilesLoaded;

    /* carichiamo le opzioni del progetto */
	ar >> m_strPathName;
	ar >> m_strOutputDir;
	ar >> m_strIntermediateDir;
	ar >> m_strOutputPrefix;
	ar >> m_bGenerateListing;
	ar >> nFiles;
    
	m_bSyntaxChecking = bittest (m_bGenerateListing, 31);
	m_bPropagateMessages = bittest (m_bGenerateListing, 30);
	bitres (m_bGenerateListing, 31);
	bitres (m_bPropagateMessages, 31);
    /* correggiamo il percorso da assoluto a relativo */
    m_strPathName=((CMainFrame *)AfxGetMainWnd ())->CorreggiPercorsoAssoluto (GETWS ()->GetBasePathName (), m_strPathName);
    /* opzioni e configurazione dei singoli files */
    nFilesLoaded=0;
	for ( int i=0; i<nFiles; ++i )
	{
		CProjectFile *pPF = new CProjectFile;
        if (pPF->Load(ar,this))   {
            m_files.Add(pPF);
            nFilesLoaded++;
        }
	}
    nFiles=nFilesLoaded;
	/* carichiamo le librerie */
	LoadLibraries (ar);
    /* apriamo i file di lingua */
	m_tr.PropagaMessaggio (m_bPropagateMessages != 0, false);
    if (!m_tr.ApriFileDiLingua (GetLanguagePathName (), 0))   {
        m_tr.CreaFileDiLingua (GetLanguagePathName ());
        m_tr.ApriFileDiLingua (GetLanguagePathName (), 0);
    }
    /* settiamo l'oggetto delle traduzioni nella macchina corrente */
    ((CAdamoCfgMachine*)m_objMachine)->SetTraduzioniObj (&m_tr);
    /* carichiamo la configurazione */
    LoadConfigData (GetProjectDir()+"\\"+GetName()+".cfg", GetParamsPathName());
	return TRUE;
}

BOOL CProject::Save()
{
	return Save(m_strPathName);
}

BOOL CProject::Save(CString strPathName)
{
	CFile fout;
    CFileException e;
    BOOL bResult(FALSE);

	if ( !fout.Open(strPathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary, &e) )   {
		AfxMessageBox(LOADSTRING (IDS_ADAMOPROJECT_5));
		return FALSE;
	}
	CArchive ar(&fout, CArchive::store);
    bResult = Save(ar);
    ar.Close ();
    fout.Close ();
    SetIsModified (false);
	return bResult;
}

BOOL CProject::SaveConfig()
{
    return SaveConfig(m_strPathName);
}

/*
** SaveConfig:BOOL bHardware default = FALSE se sono in hardware = TRUE*/
BOOL CProject::SaveConfig(CString strPathName,BOOL bHardware)
{
	CFile fout;
    int n=0;

    if (!ApriFileDiConfigurazione (strPathName, &fout))
        return FALSE;
	CArchive ar(&fout, CArchive::store);
    if (m_objMachine)   {
        ((CAdamoCfgMachine*)m_objMachine)->Serialize(ar);
        ((CAdamoCfgMachine*)m_objMachine)->GestisciMappaEId (GetParamsPathName(), false);
        if (((CAdamoCfgMachine*)m_objMachine)->SalvaDescrizioneTradotta (&m_tr))
            AfxMessageBox (LOADSTRING (IDS_ADAMOMODULE_1));
    }
    ar.Close ();
    fout.Close ();
	/* non faccio la SetMachineModifiedFlag se sono in hardware */
	if (!bHardware)
        SetMachineModifiedFlag (false);
	return TRUE;
}

/*
** SaveConfig:BOOL bHardware default = FALSE se sono in hardware = TRUE
*/
BOOL CProject::SaveConfigXML (CString strPathName)
{
	CStdioFile xmlFile;
	/* apriamo il file xml di destinazione */
	if (xmlFile.Open (strPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))   {
		/* apriamo ora il file dei parametri */
        CAdamoDatiFile *adf=new CAdamoDatiFile;
		if (adf->Open (GetParamsPathName(), TRUE))   {
			adf->ReadData();
			adf->CloseFile();
			WritePrologue (&xmlFile);
			((CAdamoCfgMachine*)m_objMachine)->SaveConfigXML (adf->GetAdamoParametri (), &xmlFile);
			WriteEpilogue (&xmlFile);
		}
		delete adf;
		xmlFile.Close ();
	}
	return TRUE;
}
	
BOOL CProject::Save(CArchive &ar)
{
	if (m_bSyntaxChecking)
		bitset (m_bGenerateListing, 31);
	else
		bitres (m_bGenerateListing, 31);
	if (m_bPropagateMessages)
		bitset (m_bGenerateListing, 30);
	else
		bitres (m_bGenerateListing, 30);
	ar << m_strPathName;
	ar << m_strOutputDir;
	ar << m_strIntermediateDir;
	ar << m_strOutputPrefix;
	ar << m_bGenerateListing;

	long nFiles = m_files.GetSize();
	ar << nFiles;

	for ( int i=0; i<nFiles; ++i )
		m_files[i]->Save(ar);
	SaveLibraries (ar);
	return TRUE;
}

void CProject::SaveModified()
{
	if (GetIsModified ())
		//if ( AfxMessageBox("Machine '" + GetName() + "' has changed. Do you want to save it?", MB_YESNO)==IDYES )
        Save();
}

void CProject::SaveModConfig()
{
    /* salviamo le informazioni della configurazione */
	SaveConfigXML (GetProjectDir()+"\\"+GetName()+"_config.xml");
	SaveConfig (GetProjectDir()+"\\"+GetName()+".cfg");
}

void CProject::ExportInFileXml(CString strPath)
{
	SaveConfigXML (strPath);
}

//--------------------------------------------------------------------------------------------------
//- compile/build functions
//--------------------------------------------------------------------------------------------------

BOOL CProject::PositionBreakPoints()
{
	BOOL bModified = FALSE;
	int nFiles = m_files.GetSize();
	for ( int i=0; i<nFiles; ++i )
	{
		if ( m_files[i]->PositionBreakPoints() )
		{
			bModified = TRUE;
			CLuaView* pView = theApp.FindProjectFilesView(m_files[i]);
			if ( pView )
				m_files[i]->SetBreakPointsIn(pView->GetEditor());
		}
	}

	if ( bModified )
        SetIsModified (true);

	return bModified;
}

BOOL CProject::BuildIntermediateFiles()
{
	long nFiles = m_files.GetSize();
	int nErrors = 0, n, i;
    bool bIsModified=false, bExit=false;

    StartCompilation ();
    ((CMainFrame *)AfxGetMainWnd ())->GetCommandBars()->UpdateCommandBars ();
	/* prima passata, riempiamo il file del browser con le function */
    bIsModified=CheckMainModified ();
	for (i=0; i<nFiles; ++i )   {
		if (m_files[i]->IsModified() || i == 0)  {
			ClearBrowserInfo (m_files[i]->GetPathName ());
			ClearFunctionsInfo (m_files[i]->GetPathName ());
			ClearObjectsInfo (m_files[i]->GetPathName ());
			m_files[i]->Browse ();
		}
	}
	/* salviamo le informazioni del browser */
	SaveBrowserInfo ();
	/* salviamo le informazioni del container delle functions */
	SaveFunctionsInfo ();
	/* salviamo le informazioni del container degli oggetti */
	SaveObjectsInfo ();
	((CMainFrame*) AfxGetMainWnd ())->UpdateDockingProject (this);
	/* seconda passata */
	for (i=1; i<nFiles; ++i )
		if (m_files[i]->IsModified() && !m_files[i]->InBrowseError ())  {
			ClearGlobalVariables (m_files[i]->GetPathName ());
            bIsModified=true;
        }
    if (bIsModified)   {
        if (CompilaMain ()!=-2)   {
	        for ( i=1; i<nFiles; ++i )	{
		        if (m_files[i]->IsModified() && !m_files[i]->InBrowseError ())  {
                    m_pCompiledFile=m_files[i];
                    n=m_files[i]->Compile();
                    switch (n)   {
                        case -1 :
				            nErrors++; break;
                        case -2 :
                            nErrors=-1; bExit=true; break;
                        case -3 :
				            nErrors++; break;
                        case -4 :
				            nErrors++; break;
                    }
                    if (bExit)
                        break;
		        }
	        }
        }
        else
            nErrors=-1;
    }
    EndCompilation ();
	return nErrors;
}

void CProject::StartCompilation ()
{
	long nFiles = m_files.GetSize();
	m_bAbortCompilation=false;
	m_bInCompilation=true;
	m_pCompiledFile=NULL; 
	for (int i = 0; i < nFiles; i++)
		m_files[i]->ResetBrowseError ();
}

BOOL CProject::CheckBeforeBuild()
{
	COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();
    COutputWnd* pDebug = ((CMainFrame*)AfxGetMainWnd())->GetDebugWnd();
    ((CMainFrame*)AfxGetMainWnd())->GetPaneManager()->ShowPane (ID_VIEW_OUTPUT);
    ((CMainFrame*)AfxGetMainWnd())->GetPaneManager()->RedrawPanes ();

	pOutput->Clear ();
    pDebug->Clear ();

	CString strMsg;
	strMsg.Format("------------------ Project %s -------------------\n", GetName());
	pOutput->Write (strMsg);

	if ( !CreateIntermediateDir() )
	{
		pOutput->Write(LOADSTRING (IDS_ADAMOPROJECT_7));
		return FALSE;
	}

	if ( !CreateOutputDir() )
	{
		pOutput->Write(LOADSTRING (IDS_ADAMOPROJECT_8));
		return FALSE;
	}

	return TRUE;
}

BOOL CProject::Compile(CProjectFile *pPF)
{
	if ( !CheckBeforeBuild() )
		return FALSE;
	ClearGlobalVariables (pPF->GetPathName ());
	ClearBrowserInfo (pPF->GetPathName ());
    m_pCompiledFile=pPF;
	return pPF->Compile();
}

BOOL CProject::BuildOutputFiles()
{
	COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();

	CExecutor m_exe;
	CString strCmdLine, strOutput, strPathLib;

	if (m_listLibraries.GetCount ())
		strPathLib = ((CMainFrame *)AfxGetMainWnd ())->GetLibrariesPathName ();
	long nFiles = m_files.GetSize();
	for ( int i=0; i<nFiles; ++i )
		if ( theApp.FirstFileIsNewer(m_files[i]->GetOutputPathNameExt(), GetDebugPathNameExt()) )
		{
			pOutput->Write("Linking\n");
            PompaIMessaggi ();

			strCmdLine.Format("\"%s\" -o \"%s\" -i \"%s\"", 
				theApp.GetModuleDir() + "\\" + "luac.exe", GetDebugPathNameExt(), GetIntermediateDir ());

			for (POSITION pos = m_listLibraries.GetHeadPosition (); pos; )   {
				CString strLib = m_listLibraries.GetNext (pos);
				strCmdLine += " -lib";
				strCmdLine += " \"" + strPathLib + "\\" + strLib + "\"";
			}

            GenerateOOPFile ();
	        strCmdLine+= " \"" + GetIntermediateDir() + "\\" + _OOPOBJ + "\""; 

			long nFiles = m_files.GetSize();
			for ( int i=0; i<nFiles; ++i )
				strCmdLine += " \"" + m_files[i]->GetOutputPathNameExt() + "\"";

			GenerateMainObjFile ();
	        strCmdLine+= " \"" + GetIntermediateDir() + "\\" + _MAINOBJ + "\""; 

			m_exe.Execute(strCmdLine);

			DeleteMainObjFile ();    
			DeleteOOPFile ();

			strOutput = m_exe.GetOutputString();
			if ( !strOutput.IsEmpty() )
			{
				pOutput->Write(strOutput);
				return FALSE;
			}

			break;
		}

	if ( theApp.FirstFileIsNewer(GetDebugPathNameExt(), GetReleasePathNameExt()) )
	{
		strCmdLine.Format("\"%s\" -s -o \"%s\" \"%s\"", 
			theApp.GetModuleDir() + "\\" + "luac.exe", GetReleasePathNameExt(), GetDebugPathNameExt());

		m_exe.Execute(strCmdLine);
		strOutput = m_exe.GetOutputString();
		if ( !strOutput.IsEmpty() )
		{
			pOutput->Write(strOutput);
			return FALSE;
		}
	}

	if ( m_bGenerateListing && theApp.FirstFileIsNewer(GetDebugPathNameExt(), GetListingPathNameExt()) )
	{
		strCmdLine.Format("\"%s\" -p -l \"%s\"", 
			theApp.GetModuleDir() + "\\" + "luac.exe", GetDebugPathNameExt());

		m_exe.Execute(strCmdLine);
		m_exe.SaveOutput(GetListingPathNameExt());
	}

	pOutput->Write("Link complete\n");
	return TRUE;
}

BOOL CProject::Build()
{
	COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();
	CString strMsg;
	strMsg.Format("------------------ Building project %s -------------------\n", GetName());
	pOutput->Write(strMsg);

	if ( !CheckBeforeBuild() )
		return FALSE;

	int nErrors = BuildIntermediateFiles();
	
	if ( nErrors!=0 )
	{
        if (nErrors==-1)
            nErrors=0;
		strMsg.Format("%d error(s), build terminated\n", nErrors);
		pOutput->Write (strMsg);
		return FALSE;
	}

	return BuildOutputFiles();
}

void CProject::CleanIntermediateAndOutputFiles(bool bMode)
{
    COutputWnd* pOutput = ((CMainFrame*)AfxGetMainWnd())->GetOutputWnd();
    if (!bMode&&pOutput)   {
	    pOutput->Clear();
	    pOutput->Write(LOADSTRING (IDS_ADAMOPROJECT_9));
    }
	long nFiles = m_files.GetSize();
	for ( int i=0; i<nFiles; ++i )
		m_files[i]->DeleteIntermediateFiles();
	if (!bMode&&pOutput)
        pOutput->Write(LOADSTRING (IDS_ADAMOPROJECT_10));
	DeleteFile(GetDebugPathNameExt());
	DeleteFile(GetReleasePathNameExt());
}

BOOL CProject::HasBreakPoint(const char *szFile, int nLine)
{
	CProjectFile *pPF = GetProjectFile(szFile);
	if ( pPF==NULL )
		return FALSE;

	return pPF->HasBreakPoint(nLine);
}

void CProject::GenerateMainObjFile ()
{
	FILE* pF;
    int nLen;
    char szBuffer[256];
    char szBuf2[256];

     unsigned char szText1[] = {	
		0x1b, 0x4c, 0x75, 0x61, 0x52, 0x00, 0x01, 0x04,
		0x04, 0x04, 0x08, 0x00, 0x19, 0x93, 0x0d, 0x0a,
		0x1a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x00, 0x00,
		0x00, 0x08, 0x40, 0x40, 0x80, 0x06, 0x80, 0x40,
		0x00, 0x1d, 0x40, 0x80, 0x00, 0x1f, 0x00, 0x80,
		0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x0b, 0x00,
		0x00, 0x00, 0x5f, 0x5f, 0x49, 0x6e, 0x4d, 0x61,
		0x69, 0x6e, 0x5f, 0x5f, 0x00, 0x01, 0x01, 0x04,
		0x05, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x01, 0x00 
	};

	unsigned char szText2[] = {		
		0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x05, 0x00, 0x00, 0x00, 0x5f, 0x45, 0x4e, 0x56,
		0x00
	};

    strcpy (szBuffer, GetIntermediateDir() + "\\" + _MAINOBJ);
    strcpy (szBuf2, "@c:\\main.ada");
    nLen=strlen (szBuf2)+1;
    if (pF=fopen (szBuffer, "wb"))    {
        fwrite (szText1, sizeof (szText1), 1, pF);
        fwrite (&nLen, sizeof (int), 1, pF);
        fwrite (szBuf2, strlen (szBuf2)+1, 1, pF);
        fwrite (szText2, sizeof (szText2), 1, pF);
        fclose (pF);
    }
}

void CProject::DeleteMainObjFile ()
{
    CFile::Remove (GetIntermediateDir() + "\\" + _MAINOBJ);
}

void CProject::DeleteOOPFile ()
{
    CFile::Remove (GetIntermediateDir() + "\\" + _OOPOBJ);
}

void CProject::CheckFiles ()
{
	int nFiles = m_files.GetSize();
	for ( int i=0; i<nFiles; ++i )
		m_files[i]->CheckFiles ();
}

void CProject::SaveGlobalVariables ()
{
    m_vc.Save ();
}

void CProject::SaveBrowserInfo ()
{
    m_bc.Save ();
}

void CProject::SaveFunctionsInfo ()
{
    m_fc.Save ();
}

void CProject::SaveObjectsInfo ()
{
    m_oc.Save ();
}

void CProject::ClearGlobalVariables (const char* szModule)
{
    m_vc.RemoveModule (szModule);
}

void CProject::ClearBrowserInfo (const char* szModule)
{
    m_bc.RemoveModule (szModule);
}

void CProject::ClearFunctionsInfo (const char* szModule)
{
    m_fc.RemoveModule (szModule);
}

void CProject::ClearObjectsInfo (const char* szModule)
{
    m_oc.RemoveModule (szModule);
}

void CProject::DeleteObject(CObject *pObj)
{
    CAdamoLogicModule *pModule = ((CAdamoCfgMachine*)m_objMachine)->GetAdamoModule();
    if(pModule->DeleteObject(pObj))
        SetMachineModifiedFlag (true);
}

void CProject::CreateByType()
{
	CString strHelloWorld = "function main ()\n \nend\n ";

	CString strFile = GetProjectDir() + "\\" + GetName() + ".ada";
	CFile pf;
	pf.Open(strFile, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate);
	pf.Write(strHelloWorld, strHelloWorld.GetLength());
	pf.Close();
	AddFile(strFile);
}

/*
** SaveDataConfig: salva configurazione file dat sempre
*/
void CProject::SaveDataConfig()
{
    SaveConfig(GetProjectDir()+"\\"+GetName()+".cfg",TRUE);
}

/*
** LanciaTemporary
*/
void CProject::LanciaTemporary (CString strFile, CString strName)
{
    using namespace nsPath;
    CPath path (strFile);
	CExecutor m_exe;
	CString strCmdLine, strFileOut, strFileDebug;

    path.RenameExtension ("out");
    /* copiamo in un buffer di lavoro il nome del file */
    strFileOut=GetIntermediateDir() + "\\" + path.GetName();
    /* generiamo il modulo oggetto */
    strCmdLine.Format("\"%s\" -o \"%s\" \"%s\"", 
		theApp.GetModuleDir() + "\\" + "luac.exe", strFileOut, strFile);
	m_exe.Execute(strCmdLine);
    /* ora linkiamo */
    path.RenameExtension ("debug");
    strFileDebug=GetOutputDir() + "\\" + path.GetName();
	strCmdLine.Format("\"%s\" -o \"%s\"", 
		theApp.GetModuleDir() + "\\" + "luac.exe", strFileDebug);
    strCmdLine += " \"" + strFileOut + "\"";
    m_exe.Execute(strCmdLine);
    /* ora spediamo il programma giu' */
    m_objMachine->DownLoadTemporary (strFileDebug, strName);
    /* ora ripuliamo la spazzatura */
    CFile::Remove (strFileOut);
    CFile::Remove (strFileDebug);
}

/*
** TerminaTemporary :
*/
void CProject::TerminaTemporary (CString strName)
{
    int nSlot=m_objMachine->GetSlotNumber (strName);
    if (nSlot!=-1)   {
        stTaskData td;
        td.nTask=nSlot;
        strlimcpy (td.szTaskName, strName, NMAXIDENTIFIER+1);
        m_objMachine->SlotDelete (&td);
    }
}

/*
** CheckMainModified :
*/
bool CProject::CheckMainModified ()
{
    bool bMainModified=false;
    /* verifichiamo se il file main e' stato modificato */
	if (m_files[0]->GetName()==GetName()&&m_files[0]->IsModified())  {
		CleanIntermediateAndOutputFiles(); bMainModified=true;
	}
    return bMainModified;
}

/*
** ReloadConfiguration :
*/
void CProject::ReloadConfiguration ()
{
    /* cancelliamo la configurazione di macchina */
    DeleteConfigData ();
    /* ricarichiamo la configurazione */
    LoadConfigData (GetProjectDir()+"\\"+GetName()+".cfg", GetParamsPathName());
}

/*
** DeleteConfigData 
*/
void CProject::DeleteConfigData ()
{
    ((CAdamoCfgMachine*)m_objMachine)->DeleteConfigData ();
    ((CAdamoCfgMachine*)m_objMachine)->NewModuleObject (this);
}

/*
** StoreConfigData : 
*/
bool CProject::StoreConfigData ()
{
    bool b=false;
    if (m_objMachine)   {
        CFile fout;
        CFileException e;
        CPath pPath;
        pPath=GetProjectDir()+"\\"+GetName()+".cfg";
        if (ApriFileDiConfigurazione ((const char *)pPath, &fout))   {
	        CArchive ar(&fout, CArchive::store);
            ((CAdamoCfgMachine*)m_objMachine)->Serialize(ar);
            ((CAdamoCfgMachine*)m_objMachine)->GestisciMappaEId (GetParamsPathName(), false);
            ar.Close ();
            fout.Close ();
            b=true;
        }
    }
    return b;
}

/*
** ChangeLanguage : chiamata dal workspace per cambiare la lingua corrente.
*/
int CProject::ChangeLanguage ()
{
    /* cambiamo la tabella del file delle traduzioni */
    m_tr.ChiudiFileDiLingua ();
    m_tr.ApriFileDiLingua (GetLanguagePathName (), 0);
    /* cambiamo la lingua al firmware */
    m_objMachine->ChangeLanguage ();
    return 0;
}

/*
** CompilaMain :
*/
int CProject::CompilaMain ()
{
    int nB=0;
    bool bOnlyPreprocess=false;

    if (m_files.GetSize())   {
        if (!m_files[0]->IsModified() || m_files[0]->InBrowseError ())
            bOnlyPreprocess=true;
		ClearGlobalVariables (m_files[0]->GetPathName ());
        m_pCompiledFile=m_files[0];
        nB=m_files[0]->Compile (bOnlyPreprocess);
    }
    return nB;
}

/*
** GestisciMappaEId :
*/
void CProject::GestisciMappaEId ()
{
    ((CAdamoCfgMachine*)m_objMachine)->GestisciMappaEId (GetParamsPathName());
}

/*
** LoadPswLevel :
*/
void CProject::LoadPswLevel ()
{
    CAdamoPswFile pswFile;
    ePswLevel e = ((CIdeApp*)AfxGetApp ())->GetPswLevel ();
	if (e == (ePswLevel) -1)   {
		pswFile.SetMachinePath (GetProjectDir());
		if (pswFile.Open ())   {
			m_ePswLevel=pswFile.GetCurrentLevel ();
			pswFile.ClosePsw ();
		}
		else
			m_ePswLevel = ePswUser;
	}
	else
		m_ePswLevel = e;
}

/*
** LoadEthercatXMLFile :
*/
bool CProject::LoadEthercatXMLFile (CString strPathName)
{
	HRESULT hr;
	int nSlaves, nRealSlaves, nVariables;
	bool b=false;
	/* carichiamo ora il file XML */

	CAdamoHWConfigFile *pHWConfigFile=new CAdamoHWConfigFile;
	if (pHWConfigFile)   {
		if (pHWConfigFile->Open (GetHWConfigPathName(), TRUE))   {
			pHWConfigFile->ReadData ();
			if (strPathName=="")
				strPathName=pHWConfigFile->GetEthercatMasterXML ();
			if (strPathName!="")   {
				strPathName=GetEthercatPathName() + "\\" + strPathName;
				try   {
					MSXML2::IXMLDOMDocument *pDoc=NULL;
					MSXML2::IXMLDOMElement* pRoot = NULL;
					MSXML2::IXMLDOMNode* pImage = NULL;
					MSXML2::IXMLDOMNode* pNode = NULL;
					MSXML2::IXMLDOMNodeList *pSlaves = NULL;
					MSXML2::IXMLDOMNodeList *pProcessImage = NULL;
					/* creiamo il componente */
					CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, __uuidof(MSXML2::IXMLDOMDocument), (void**)&pDoc);
					if( pDoc != NULL )
						/* carichiamo il file XML */
						if( pDoc->load((_variant_t) _bstr_t(strPathName))  == VARIANT_TRUE )   {
							/* prendiamo il nodo da cui tutto discende */
							pDoc->get_documentElement(&pRoot);
							if (pRoot)   {
								/* vediamo quanti slave abbiamo */
								pRoot->raw_selectNodes(L"Config/Slave", &pSlaves);
								if( pSlaves != NULL )   {
									nRealSlaves = 0;
									nSlaves = pSlaves->length;
									m_slaveArray.SetSize (nSlaves);
									/* carichiamo tutti gli slave */
									for( long i=0; i<nSlaves; i++)	 {																
										pSlaves->get_item(i, &pNode);	
										if( pNode )   {
											if (FillSlaveInfo (nRealSlaves, pNode))
												nRealSlaves++;
									}
									}
									m_slaveArray.SetSize (nRealSlaves);
									pSlaves->Release ();
									pRoot->raw_selectNodes(L"Config/ProcessImage/Inputs/Variable", &pProcessImage);
									/* vediamo le variabili esistenti e aggiungiamole agli slave corretti */
									if (pProcessImage != NULL)   {
										nVariables = pProcessImage->length;
										for( long i=0; i<nVariables; i++)	 {
											pProcessImage->get_item(i, &pNode);
											if (pNode)
												FillSlaveVariables (i, pNode, 0);
										}
										pProcessImage->Release ();
										pRoot->raw_selectNodes(L"Config/ProcessImage/Outputs/Variable", &pProcessImage);
										if (pProcessImage != NULL)   {
											nVariables = pProcessImage->length;
											for( long i=0; i<nVariables; i++)	 {
												pProcessImage->get_item(i, &pNode);
												if (pNode)
													FillSlaveVariables (i, pNode, 1);
											}
											pProcessImage->Release ();
											/* carichiamo anche il tipo di dispositivo */
											LoadHWConfiguration (pHWConfigFile);
											/* updatiamo le docking */
											UpdateXMLDocking ();
											/* ritorniamo tutto ok */
											b=true;
										}
									}
								}
							}
						}
						else   {
							AfxMessageBox(_T(LOADSTRING (IDS_CONFIG_HW_CANNOTPARSEXMLFILE)));
						}
					else   {
						AfxMessageBox(_T(LOADSTRING (IDS_CONFIG_HW_CANNOTCREATEXMLCOM)));
						return false;
					}
				}
				catch( _com_error &e)
				{
					AfxMessageBox(_T("Com Error"));
					hr = e.Error();
				}
			}
			pHWConfigFile->CloseFile ();
			delete pHWConfigFile;
		}
	}
	return b;
}

/*
** ClearSlaveArray :
*/
void CProject::ClearSlaveArray ()
{
	for (int i=0; i<m_slaveArray.GetSize (); i++ )   {
		delete ((stAdamoEthSlave *)m_slaveArray[i]);
	}
	m_slaveArray.SetSize (0);
}

/*
** FillSlaveInfo :
*/
int CProject::FillSlaveInfo (int i, MSXML2::IXMLDOMNode *pSlave)
{
	CString str;
	int nB = 0;
	MSXML2::IXMLDOMNodePtr spNode;
	stAdamoEthSlave *pSlaveStruct=new stAdamoEthSlave;

	strncpy(pSlaveStruct->szName, (char*)(_bstr_t)pSlave->selectSingleNode(L"Info/Name")->Gettext(), 
		ECAT_DEVICE_NAMESIZE);
	pSlaveStruct->szName[ECAT_DEVICE_NAMESIZE] = 0;
	spNode=pSlave->selectSingleNode ("Info/PhysAddr");
	pSlaveStruct->nPhysAddress=0;
	if (spNode)   {
		pSlaveStruct->nPhysAddress=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("Info/VendorId");
	pSlaveStruct->nVendorID=0;
	if (spNode)   {
		pSlaveStruct->nVendorID=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("Info/ProductCode");
	pSlaveStruct->nProductCode=0;
	if (spNode)   {
		pSlaveStruct->nProductCode=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("Info/RevisionNo");
	pSlaveStruct->nRevisionNumber=0;
	if (spNode)   {
		pSlaveStruct->nRevisionNumber=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("Info/SerialNo");
	pSlaveStruct->nSerialNumber=0;
	if (spNode)   {
		pSlaveStruct->nSerialNumber=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("Info/ProductRevision");
	pSlaveStruct->szProductRevision[0]='\0';
	if (spNode)   {
		strncpy(pSlaveStruct->szProductRevision, (char*)(_bstr_t)spNode->Gettext(), ECAT_DEVICE_REVISION);
	}
	spNode=pSlave->selectSingleNode ("ProcessData/Send/BitStart");
	if (spNode)   {
		pSlaveStruct->nBitStartSend=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("ProcessData/Send/BitLength");
	if (spNode)   {
		pSlaveStruct->nBitLenghtSend=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("ProcessData/Recv/BitStart");
	if (spNode)   {
		pSlaveStruct->nBitStartReceive=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pSlave->selectSingleNode ("ProcessData/Recv/BitLength");
	if (spNode)   {
		pSlaveStruct->nBitLenghtReceive=(int)(long)(_variant_t)spNode->Gettext ();
	}
	if (pSlaveStruct->nPhysAddress != 0)   {
	m_slaveArray[i]=pSlaveStruct;
		nB = 1;
	}
	return nB;
}

/*
** FillSlaveVariables :
*/
void CProject::FillSlaveVariables (int i, MSXML2::IXMLDOMNode *pVariable, int nInOut)
{
	MSXML2::IXMLDOMNodePtr spNode;
	stAdamoEthSlaveVariable *pEthSlaveVariable;

	pEthSlaveVariable = (stAdamoEthSlaveVariable *) new stAdamoEthSlaveVariable;
	spNode=pVariable->selectSingleNode ("Name");
	if (spNode)   {
		strncpy(pEthSlaveVariable->szName, (char*)(_bstr_t)spNode->Gettext(), ECAT_VARIABLE_NAMESIZE);
		pEthSlaveVariable->szName[ECAT_VARIABLE_NAMESIZE]='\0';
	}
	spNode=pVariable->selectSingleNode ("DataType");
	if (spNode)   {
		char szBuffer[256];
		strncpy(szBuffer, (char*)(_bstr_t)spNode->Gettext(), 256);
		szBuffer[255]='\0';
		pEthSlaveVariable->nDataType=ExtractDataType (szBuffer);
	}
	spNode=pVariable->selectSingleNode ("BitSize");
	if (spNode)   {
		pEthSlaveVariable->nBitSize=(int)(long)(_variant_t)spNode->Gettext ();
	}
	spNode=pVariable->selectSingleNode ("BitOffs");
	if (spNode)   {
		pEthSlaveVariable->nBitOffs=(int)(long)(_variant_t)spNode->Gettext ();
	}
	pEthSlaveVariable->nInputOutput=nInOut;
	if (!AssignToSlave (pEthSlaveVariable))
		delete pEthSlaveVariable;
}

/*
** ExtractDataType : 
*/
int CProject::ExtractDataType (const char *strDataType)
{
	int n=-1;
	if (!strcmp (strDataType, "BOOL"))
		n=IDETHTYPE_BOOL;
	if (!strcmp (strDataType, "INT"))
		n=IDETHTYPE_INT;
	if (!strcmp (strDataType, "UINT"))
		n=IDETHTYPE_UINT;
	if (!strcmp (strDataType, "DINT"))
		n=IDETHTYPE_DINT;
	if (!strcmp (strDataType, "UDINT"))
		n=IDETHTYPE_DUINT;
	return n;
}

/*
** AssignToSlave :
*/
int CProject::AssignToSlave (stAdamoEthSlaveVariable *pEthVariable)
{
	int n=0;
	for (int i=0; i<m_slaveArray.GetSize (); i++)   {
		if (pEthVariable->nInputOutput==0)   {
			if (pEthVariable->nBitOffs>=((stAdamoEthSlave *)m_slaveArray[i])->nBitStartReceive &&
				pEthVariable->nBitOffs<((stAdamoEthSlave *)m_slaveArray[i])->nBitStartReceive+((stAdamoEthSlave *)m_slaveArray[i])->nBitLenghtReceive)   {
				((stAdamoEthSlave *)m_slaveArray[i])->AddTail (pEthVariable); n=1; break;
			}
		}
		else   {
			if (pEthVariable->nBitOffs>=((stAdamoEthSlave *)m_slaveArray[i])->nBitStartSend &&
				pEthVariable->nBitOffs<((stAdamoEthSlave *)m_slaveArray[i])->nBitStartSend+((stAdamoEthSlave *)m_slaveArray[i])->nBitLenghtSend)   {
				((stAdamoEthSlave *)m_slaveArray[i])->AddTail (pEthVariable); n=1; break;
			}
		}
	}
	return n;
}

/*
** LoadHWConfiguration 
*/
void CProject::LoadHWConfiguration (CAdamoHWConfigFile *pHWConfigFile)
{
	stHWEthVarMask iEnableEthercatVariables;
	int nRequestedState=pHWConfigFile->GetRequestedState();

	for (int i=0; i<m_slaveArray.GetSize (); i++)
		for (int n=0; n<MAX_ETHERCAT_SLAVE_TYPE; n++)   {
			((stAdamoEthSlave *)m_slaveArray[i])->nDeviceType[n]=(eEthercatDeviceType)pHWConfigFile->GetEthercatDeviceType (i, n);
			pHWConfigFile->GetEnabledEthercatVariables (i, n, &iEnableEthercatVariables);
			((stAdamoEthSlave *)m_slaveArray[i])->liEnableVariables[n]=iEnableEthercatVariables;
			((stAdamoEthSlave *)m_slaveArray[i])->nRequestedState=nRequestedState;
		}
}

/*
** UpdateXMLDocking :
*/
void CProject::UpdateXMLDocking ()
{
	/* prendiamo la docking della configurazione hardware */
	CDockConfigHW* pDockHW=((CMainFrame *) AfxGetMainWnd ())->GetDockConfigHW ();
	if (pDockHW)   {
		CAdamoConfigHWView *pConfigHWView=pDockHW->GetView ();
		if (pConfigHWView)
			pConfigHWView->LoadEthercatXMLFile (this, m_slaveArray);
	}
	/* ora prendiamo la docking delle mailbox Ethercat */
	CDockEcMailBox* pEcMailBox=((CMainFrame *) AfxGetMainWnd ())->GetDockEcMailBox ();
	if (pEcMailBox)   {
		CDGEcMailBoxView *pMailBoxView=pEcMailBox->GetView ();
		if (pMailBoxView)
			pMailBoxView->LoadEthercatXMLFile (m_slaveArray);
	}
}

/*
** GestisciVersione :
*/
void CProject::GestisciVersione ()
{
    int nOldVersion=GETWS()->GetDataVersion ();
    int nVersion=GETWS()->GetProgramVersion ();
    bool bConverted=false;

    ((CAdamoCfgMachine*)m_objMachine)->GestisciVersione ();
    if (nOldVersion<VERSIONE_1_3)   {
        From1_1_1To1_1_3 ();
        bConverted=true;
    }
    if (nOldVersion<VERSIONE_2_0)   {
        bConverted=true;
        From1_1_3To2_0_0 ();
    }
    if (nOldVersion<VERSIONE_2_6)   {
        bConverted=true;
        From2_0_0To2_6_0 ();
    }
    if (bConverted)   {
        GETWS ()->SetDataVersion (GETWS ()->GetProgramVersion ());
        GETWS ()->SavePlantSettings ();
    }
}

/*
** ApriFileDiConfigurazione :
*/
bool CProject::ApriFileDiConfigurazione (CString str, CFile *fout)
{
    CFileException e;
    int n=0;
    bool nB=true;

    while (n<3)   {
        if (!fout->Open(str, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::shareExclusive, &e) )
            n++;
        else
            break;
    }
    if (n==3)   {
        AfxMessageBox(LOADSTRING (IDS_ADAMOPROJECT_5));
		nB=false;
    }
    return nB;
}

int CProject::OpenMachine ()
{
	eTypeConnection eTC = enLocalConnection;
	BYTE achIPAddress[IPADDRESS_BUFFER];

	if (m_objMachine->GetTypeModule () == enEpsilon)
		eTC = enTCPConnection;
	else   {
    CAdamoOptions *pOpt = ((CIdeApp*) AfxGetApp ())->GetAdamoOptions ();
		if (pOpt->GetTypeConnection () == enTCPConnection)   {
			pOpt->GetTcpIpAddress (achIPAddress[0], achIPAddress[1], achIPAddress[2], achIPAddress[3]);
			m_objMachine->SetTcpIpAddress (achIPAddress);
			m_objMachine->SetPort (pOpt->GetTcpIpPort ());
			eTC = enTCPConnection;
		}
    }
    return m_objMachine->OpenMachine (GetProjectDir(), -1, eTC);
}

int CProject::ReconnectMachine (eTypeConnection eTC)
{
    return m_objMachine->ReconnectMachine (eTC);
}

void CProject::LoadHWConfig ()
{
	bool b=true;

	if (!m_adf)
        m_adf=new CAdamoDatiFile;
    if (m_adf->Open (GetParamsPathName(), TRUE))   {
        m_adf->ReadData();
        m_adf->CloseFile();
        if (!m_hwcf)
            m_hwcf=new CAdamoHWConfigFile;
        if (m_hwcf->Open (GetHWConfigPathName(), b))   {
            m_hwcf->ReadData ();
            m_hwcf->CloseFile();
        }
    }
}

void CProject::SaveHWConfig ()
{
    if (m_adf && m_adf->Open (GetParamsPathName(), TRUE))   {
        m_adf->WriteData();
        m_adf->CloseFile();
        CheckHWConfigFile (GetHWConfigPathName());
		SaveHWConfigXML (GetProjectDir()+"\\"+"Hardware"+"_config.xml");
        if (m_hwcf->Open (GetHWConfigPathName(), TRUE))   {
            m_hwcf->WriteData ();
            m_hwcf->CloseFile();
        }
    }
}

void CProject::CloseHWConfig ()
{
    _delete (m_adf);
    _delete (m_hwcf);
}

/*
** CheckHWConfigFile :
*/
void CProject::CheckHWConfigFile (CPath pPath)
{
    int nS, i, s, t;

    /* se non siamo riusciti ad aprirlo, proviamo a crearlo */
    if (_access (pPath, 00))    {
        CAdamoHWConfigFile *pF=new CAdamoHWConfigFile;
		for (nS = 0; nS<NMAX_SCHEDE; nS++)
			for (i=0; i<NMAXMGASTATION; i++)
				for (s=0; s<NMAXMGA030PARAMETERSTYPES; s++)
					for (t=0; t<NMAXMGA030PARAMETERS; t++)
						pF->SetSSCNETPar (nS, i, s, t, m_hwcf->GetSSCNETPar (nS, i, s, t));
        m_hwcf->Open (GetHWConfigPathName(), FALSE);
        m_hwcf->CloseFile ();
		for (nS = 0; nS<NMAX_SCHEDE; nS++)
			for (i=0; i<NMAXMGASTATION; i++)
				for (s=0; s<NMAXMGA030PARAMETERSTYPES; s++)
					for (t=0; t<NMAXMGA030PARAMETERS; t++)
						m_hwcf->SetSSCNETPar (nS, i, s, t, pF->GetSSCNETPar (nS, i, s, t));
        _delete (pF);
    }
    }

/*
** ErroreDiConnessione :
*/
void CProject::ErroreDiConnessione ()
{
	CString strErrore;
	stThiraErrore* TE = new stThiraErrore;

	/* componiamo la stringa di errore */
	strErrore.Format ("%s - %s", ((CAdamoCfgMachine*)m_objMachine)->GetAdamoModule()->GetName(), LOADSTRING (IDS_CONNECTION_FAILED));
	/* aggiorniamo la status bar */
	((CMainFrame *) AfxGetMainWnd ())->AddErrorBar (strErrore, TE_HARDWARE);
}
/*
** SaveHWConfigXML :
*/
void CProject::SaveHWConfigXML (CString strPathName)
{
	CStdioFile xmlFile;
	/* apriamo il file xml di destinazione */
	if (xmlFile.Open (strPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))   {
		m_hwcf->WriteHWConfigXML (&xmlFile);
		xmlFile.Close ();
	}
}

/*
** WritePrologue :
*/
void CProject::WritePrologue (CStdioFile *pStdioFile)
{
	CString str;

	pStdioFile->WriteString ("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	pStdioFile->WriteString ("<Configuration>\n");
	pStdioFile->WriteString ("<Project>\n");
	str.Format ("<MachineName>%s</MachineName>\n", GetName ());
	pStdioFile->WriteString (str);
	str.Format ("<PlantName>%s</PlantName>\n", ((CMainFrame *) AfxGetMainWnd ())->GetWorkSpace()->GetName());
	pStdioFile->WriteString (str);
	str.Format ("<PathName>%s</PathName>\n", m_strPathName);
	pStdioFile->WriteString (str);
	str.Format ("<OutputDir>%s</OutputDir>\n", m_strOutputDir);
	pStdioFile->WriteString (str);
	str.Format ("<IntermediateDir>%s</IntermediateDir>\n", m_strIntermediateDir);
	pStdioFile->WriteString (str);
	str.Format ("<OutputPrefix>%s</OutputPrefix>\n", m_strOutputPrefix);
	pStdioFile->WriteString (str);
	str.Format ("<GenerateListing>%d</GenerateListing>\n", m_bGenerateListing);
	pStdioFile->WriteString (str);
	pStdioFile->WriteString ("<Files>\n");
	for (int i=0; i<m_files.GetSize (); i++)   {
		str.Format ("<File_%d>%s</File_%d>\n", i, m_files[i]->GetRelPathName (), i);
		pStdioFile->WriteString (str);
	}
	pStdioFile->WriteString ("</Files>\n");
	pStdioFile->WriteString ("</Project>\n");
}

/*
** WriteEpilogue :
*/
void CProject::WriteEpilogue (CStdioFile *pStdioFile)
{
	pStdioFile->WriteString ("</Configuration>");
}

/*
** ImportFromXml
*/
void CProject::ImportFromXml (CString strProjectDir, CString strIntermediateDir, CString strName)
{
	/* backuppiamo i files che andiamo a modificare */
	BackupImportedFiles (strProjectDir, strIntermediateDir);
	/* importiamo il file cfg */
	ImportCfgFile (strProjectDir, strName, "");
	/* importiamo il file cfg */
	ImportHWFile (strProjectDir, strName, "");
}

/*
** ImportFromFileXml
*/
void CProject::ImportFromFileXml (CString strProjectDir, CString strIntermediateDir, CString strName, CString strPath)
{
	CString strCfgFileName = strName.MakeLower () + "_config";
	CPath path (strPath);
	/* backuppiamo i files che andiamo a modificare */
	BackupImportedFiles (strProjectDir, strIntermediateDir);
	CString str = path.GetName ();
	if (str.MakeLower () == "hardware_config.xml") 
		ImportHWFile (strProjectDir, strName, strPath);
	else
	if (str == strCfgFileName) 
		ImportCfgFile (strProjectDir, strName, strPath);
}

/*
** BackupImportedFiles :
*/
void CProject::BackupImportedFiles (CString strProjectDir, CString strIntermediateDir)
{
	/* copiamo il file congif.cfg e hardware.dat */
	CString strDa = strProjectDir + "\\" + GetName () + ".cfg";
	CString strA = strIntermediateDir + "\\" + GetName () + ".cfg";
	CopyFile (strDa, strA, FALSE);
	strDa = strProjectDir + "\\" + HWCONFIG_FILE;
	strA = strIntermediateDir + "\\" + HWCONFIG_FILE;
	CopyFile (strDa, strA, FALSE);
	strDa = strProjectDir + "\\" + DEVICE_FILE;
	strA = strIntermediateDir + "\\" + DEVICE_FILE;
	CopyFile (strDa, strA, FALSE);
}

/*
** ImportCfgFile :
*/
void CProject::ImportCfgFile (CString strProjectDir, CString strName, CString strPath)
{
	CString strPathSource;
	if (strPath == "")
		strPathSource = GetProjectDir () + "\\" + GetName () + "_config.xml";
	else
		strPathSource = strPath;
	/* carichiamo per prima cosa il file xml */
    HRESULT hr = m_pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        m_pDocXml->async = false;
        if (m_pDocXml->load(strPathSource.AllocSysString ()))   {
            ParseCfgFile (m_pDocXml, strProjectDir, strName);
            hr=S_OK;
        }
        m_pDocXml.Release ();
    }
}

/*
** ImportHWFile :
*/
void CProject::ImportHWFile (CString strProjectDir, CString strName, CString strPath)
{
	CString strPathSource;
	if (strPath == "")
		strPathSource = GetProjectDir () + "\\" + "Hardware" + "_config.xml";
	else
		strPathSource = strPath;
	/* carichiamo per prima cosa il file xml */
    HRESULT hr = m_pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        m_pDocXml->async = false;
        if (m_pDocXml->load(strPathSource.AllocSysString ()))   {
            ParseHWFile (m_pDocXml, strProjectDir, strName);
            hr=S_OK;
        }
        m_pDocXml.Release ();
    }
}

/*
** ParseCfgFile :
*/
void CProject::ParseCfgFile (DocXmlPtr pDocXml, CString strProjectDir, CString strName)
{
	CAdamoCfgMachine *pMachine = new CAdamoCfgMachine;
	int nItems, n, nB = 0;
    ElementXmlPtr  pEl=pDocXml->documentElement;
    CString str=(LPCTSTR)pEl->nodeName;
	bool bError = false;

    if (str=="Configuration")   {
        if (pEl->hasChildNodes())	  {
			NodeListXmlPtr  pRootChild;
            pRootChild = pEl->childNodes;
            nItems=pRootChild->length;
			n = 0;
            while (n<nItems)   {
                str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
                if (str=="Machine")   {
                    pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
			            while (n<nItems)   {
							if ((nB = ImportMachineData (pRootChild->Getitem(n), pMachine, strProjectDir)) == -1)
								break;
							n++;
						}
					}
				}
				else
				if (str=="Project")   {
                    pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
			            while (n<nItems)   {
							pEl = pRootChild->Getitem(n);
							CString str = (LPCSTR)pEl->nodeName;
							if (str == "PlantName")   {
								CString strText = (LPCSTR)pEl->text;
								if (strcmp (((CMainFrame *) AfxGetMainWnd ())->GetWorkSpace()->GetName().MakeUpper (), strText.MakeUpper ()))   {
									bError = true; break;
								}
							}
							else
							if (str == "MachineName")   {
								CString strText = (LPCSTR)pEl->text;
								if (strcmp (GetName ().MakeUpper (), strText.MakeUpper ()))   {
									bError = true; break;
								}
							}
							n++;
						}
					}
				}
				n++;
			}
		}
		if (!bError)   {
			SalvaNuovaMacchina (pMachine, strProjectDir, strName);
			RicaricaNuovaMacchina ();
		}
	}
}

/*
** ParseHWFile :
*/
void CProject::ParseHWFile (DocXmlPtr pDocXml, CString strProjectDir, CString strName)
{
	CAdamoHWConfigFile *pHWConfigFile = new CAdamoHWConfigFile;
	int nItems, q, nB = 0;
    ElementXmlPtr  pEl=pDocXml->documentElement;
    CString str=(LPCTSTR)pEl->nodeName;
	bool bError = false;

    if (str=="Machine")   {
        if (pEl->hasChildNodes())	  {
			NodeListXmlPtr  pRootChild;
            pRootChild = pEl->childNodes;
            nItems=pRootChild->length;
			q = 0;
            while (q<nItems)   {
				CString strChannel;
				strChannel.Format ("%s%d", "Channel_", q);
                str=(LPCTSTR)pRootChild->Getitem(q)->nodeName;
                if (str==strChannel)   {
                    pEl=pRootChild->Getitem(q);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
			            while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (!strcmp (str, "CanOpen"))   {
								ImportHWCanOpen (pRootChild->Getitem(n), pHWConfigFile, q);
							}
							else
							if (!strcmp (str, "SSCNET"))   {
								ImportHWSSCNET (pRootChild->Getitem(n), pHWConfigFile, q);
							}
							n++;
						}
					}
				}
				else
				if (str=="Ethercat")   {
                    pEl=pRootChild->Getitem(q);
					if (pEl->hasChildNodes())	  {
						ImportHWEthercat (pEl, pHWConfigFile);
					}
				}
				q++;
			}
		}
		/* salviamo la configurazione */
		SaveHWImportedFile (pHWConfigFile, strProjectDir, strName);
	}
}

/*
** ImportMachineData :
*/
int CProject::ImportMachineData (ElementXmlPtr pEl, CAdamoCfgMachine *pMachine, CString strProjectDir)
{
	int nB = 0;
	CString str = (LPCSTR)pEl->nodeName;
	if (str == "Name")   {
		CString strText = (LPCSTR)pEl->text;
		pMachine->GetAdamoModule()->SetName (strText);
	}
	else
	if (str == "Description")   {
		CString strText = (LPCSTR)pEl->text;
		pMachine->GetAdamoModule()->SetDesc (strText);
	}
	else
	if (str == "ModuleNumber")   {
		CString strText = (LPCSTR)pEl->text;
		int nModuleNumber = atoi (strText);
		pMachine->SetModuleNumber (nModuleNumber);
	}
	else
	if (str == "MachineType")   {
		CString strText = (LPCSTR)pEl->text;
		int nMachineType = atoi (strText);
		pMachine->SetMachineType ((eTypeMachine)nMachineType);
	}
	else
	if (str == "ModuleType")   {
		CString strText = (LPCSTR)pEl->text;
		int nModuleType = atoi (strText);
		pMachine->SetTypeModule ((eTypeModule)nModuleType);
	}
	else
	if (str == "PortNumber")   {
		CString strText = (LPCSTR)pEl->text;
		int nPortNumber = atoi (strText);
		pMachine->SetPort (nPortNumber);
	}
	else
	if (str == "IPAddress")   {
		CString strText = (LPCSTR)pEl->text;
		BYTE achIpAddress[4];
		GetTcpIpAddress (strText, achIpAddress[0], achIpAddress[1], achIpAddress[2], achIpAddress[3]);
		pMachine->SetTcpIpAddress (achIpAddress);
	}
	else
	if (str == "Rate")   {
		CString strText = (LPCSTR)pEl->text;
		int nRate = atoi (strText);
		pMachine->SetRate (nRate);
	}
	else
	if (str == "Frequency")   {
		CString strText = (LPCSTR)pEl->text;
		int nFrequency = atoi (strText);
		pMachine->SetFrequency (nFrequency);
	}
	else
	if (str == "Epsilon")   {
		CString strText = (LPCSTR)pEl->text;
		double fpEpsilon = atof (strText);
		pMachine->SetPrecision (fpEpsilon);
	}
	else
	if (str == "EpsilonRaggi")   {
		CString strText = (LPCSTR)pEl->text;
		double fpEpsilonRaggi = atof (strText);
		pMachine->SetEpsilonRaggi (fpEpsilonRaggi);
	}
	else
	if (str == "ContAngle")   {
		CString strText = (LPCSTR)pEl->text;
		double fpContAngle = atof (strText);
		pMachine->SetContureAngle (fpContAngle);
	}
	else
	if (str == "FilterMessagesOnStart")   {
		CString strText = (LPCSTR)pEl->text;
		int nFilterMessagesOnStart = atoi (strText);
		pMachine->SetMessageFilteringOnStart (nFilterMessagesOnStart);
	}
	else
	if (str == "StopAxisOnError")   {
		CString strText = (LPCSTR)pEl->text;
		int nStopAxisOnError = atoi (strText);
		pMachine->SetStopAxisOnError (nStopAxisOnError);
	}
	else
	if (str == "InvertG2G3")   {
		CString strText = (LPCSTR)pEl->text;
		int nInvertG2G3 = atoi (strText);
		pMachine->SetSensoRotazioneCircolari (nInvertG2G3);
	}
	else
	if (str == "KAC")   {
		CString strText = (LPCSTR)pEl->text;
		double fpKAC = atof (strText);
		pMachine->SetKAC (fpKAC);
	}
	else
	if (str == "Smoothing")   {
		CString strText = (LPCSTR)pEl->text;
		double fpSmoothing = atof (strText);
		pMachine->SetSmoothing (fpSmoothing);
	}
	else
	if (str == "BowThickening")   {
		CString strText = (LPCSTR)pEl->text;
		double fpBowThickening = atof (strText);
		pMachine->SetBowThickening (fpBowThickening);
	}
	else
	if (str == "LinearThickening")   {
		CString strText = (LPCSTR)pEl->text;
		double fpLinearThickening = atof (strText);
		pMachine->SetLinearThickening (fpLinearThickening);
	}
	else
	if (str == "VelProfile")   {
		CString strText = (LPCSTR)pEl->text;
		double fpVelProfile = atof (strText);
		pMachine->SetVelProfile (fpVelProfile);
	}
	else
	if (str == "AccProfile")   {
		CString strText = (LPCSTR)pEl->text;
		long lAccProfile = atol (strText);
		pMachine->SetAccProfile (lAccProfile);
	}
	else
	if (str == "DecProfile")   {
		CString strText = (LPCSTR)pEl->text;
		long lDecProfile = atol (strText);
		pMachine->SetDecProfile (lDecProfile);
	}
	else
	if (str == "AInterpDefault")   {
		CString strText = (LPCSTR)pEl->text;
		int nAInterpDefault = atoi (strText);
		pMachine->SetAInterpDefault (nAInterpDefault);
	}
	else
	if (str == "SpikeDetector")   {
		CString strText = (LPCSTR)pEl->text;
		bool bSpikeDetector = atoi (strText) != 0;
		pMachine->SetSpikeDetector (bSpikeDetector);
	}
	else
	if (str == "MaxUTurnPrecision")   {
		CString strText = (LPCSTR)pEl->text;
		bool bMaxUTurnPrecision = atoi (strText) != 0;
		pMachine->SetMaxUTurnPrecision (bMaxUTurnPrecision);
	}
	else
	if (str == "SmootherType")   {
		CString strText = (LPCSTR)pEl->text;
		int nSmootherType = atoi (strText);
		pMachine->SetSmootherType ((enSmootherType) nSmootherType);
	}
	else
	if (str == "SingleSpikeSmootherWeight")   {
		CString strText = (LPCSTR)pEl->text;
		double fpSingleSpikeSmootherWeight = atof (strText);
		pMachine->SetSingleSpikeSmootherWeight (fpSingleSpikeSmootherWeight);
	}
	else
	if (str == "SingleSpikeFactor")   {
		CString strText = (LPCSTR)pEl->text;
		double fpSingleSpikeFactor = atof (strText);
		pMachine->SetSingleSpikeFactor (fpSingleSpikeFactor);
	}
	else
	if (str == "GeometrySpikeFactor")   {
		CString strText = (LPCSTR)pEl->text;
		double fpMultipleSpikeFactor = atof (strText);
		pMachine->SetGeometrySpikeFactor (fpMultipleSpikeFactor);
	}
	else
	if (str == "DetectJointUTurn")   {
		CString strText = (LPCSTR)pEl->text;
		bool bDetectJointUTurn = atoi (strText) != 0;
		pMachine->SetDetectJointUTurn (bDetectJointUTurn);
	}
	else
	if (str == "StopOnAngle")   {
		CString strText = (LPCSTR)pEl->text;
		bool bStopOnAngle = atoi (strText) != 0;
		pMachine->SetStopOnAngle (bStopOnAngle);
	}
	else
	if (str == "MinStopOnAngle")   {
		CString strText = (LPCSTR)pEl->text;
		double fpMinStopOnAngle = atof (strText);
		pMachine->SetMinStopAngle (fpMinStopOnAngle);
	}
	else
	if (str == "RamDisk")   {
		CString strText = (LPCSTR)pEl->text;
		int nRamDisk = atoi (strText);
		pMachine->SetRamDisk (nRamDisk != 0);
	}
	else
	if (str == "RamDiskBufferSize")   {
		CString strText = (LPCSTR)pEl->text;
		int nRamDiskBufferSize = atoi (strText);
		pMachine->SetRamDiskBufferSize (nRamDiskBufferSize);
	}
	else
	if (str == "G0Fillet")   {
		CString strText = (LPCSTR)pEl->text;
		int nG0Raggiata = atoi (strText);
		pMachine->SetG0Raggiata (nG0Raggiata != 0);
	}
	else
	if (str == "G0FilletFitting")   {
		CString strText = (LPCSTR)pEl->text;
		double fpG0Infittimento = atof (strText);
		pMachine->SetG0RaggiataInfittimento (fpG0Infittimento);
	}
	else
	if (str == "G0FilletRadius")   {
		CString strText = (LPCSTR)pEl->text;
		double fpG0Raggio = atof (strText);
		pMachine->SetG0RaggiataRaggio (fpG0Raggio);
	}
	if (str == "SimulationMode")   {
		CString strText = (LPCSTR)pEl->text;
		bool bSimulationMode = atoi (strText) != 0;
		pMachine->SetSimulationMode (bSimulationMode);
	}
	if (str == "MaxJitter")   {
		CString strText = (LPCSTR)pEl->text;
		int nMaxJitter = atoi (strText);
		pMachine->SetMaxJitter (nMaxJitter);
	}
	else
	if (str == "Group")   {
		nB = IteraGruppo (pEl, pMachine, NULL, NULL, strProjectDir);
	}
	return nB;
}

/*
** GetTcpIpAddress :
*/
void CProject::GetTcpIpAddress (CString strTcpIPAddress, BYTE &a, BYTE &b, BYTE &c, BYTE &d)
{
	CString str[4];
	int PunticinoPrima = 0, nPunticinoDopo;

	for (int i = 0; i<4; i++)   {
		nPunticinoDopo = strTcpIPAddress.Find ('.', PunticinoPrima);
		if (nPunticinoDopo == -1)
			nPunticinoDopo = strTcpIPAddress.GetLength ();
		str[i] = strTcpIPAddress.Mid (PunticinoPrima, nPunticinoDopo - PunticinoPrima);
		PunticinoPrima = nPunticinoDopo + 1;
	}
	a = (BYTE) strtol (str[0], NULL, 10);
	b = (BYTE) strtol (str[1], NULL, 10);
	c = (BYTE) strtol (str[2], NULL, 10);
	d = (BYTE) strtol (str[3], NULL, 10);
}

/*
** IteraGruppo :
*/
int CProject::IteraGruppo (ElementXmlPtr pEl, CAdamoCfgMachine *pMachine, CAdamoGroup *pParent, CAdamoDatiFile *pFileDati, CString strProjectDir)
{
	CAdamoGroup* pGroup;
	int nItems;
    NodeListXmlPtr  pRootChild;
    CString str=(LPCTSTR)pEl->nodeName;

	if (m_nLevel == 0)   {
		pGroup = new CAdamoLogicModule;
		pFileDati = new CAdamoDatiFile;
		if (pFileDati->Open (strProjectDir + "\\" + DEVICE_FILE, 1) != 1)
			return -1;
	}
	else
		pGroup = new CAdamoGroup;
    pGroup->Init();
    pGroup->DevInit();
	pGroup->SetParentObj (pParent);
	m_nLevel++;
    if (str=="Group")   {
        if (pEl->hasChildNodes())	  {
            pRootChild = pEl->childNodes;
            nItems=pRootChild->length;
            int n=0;
            while (n<nItems)   {
				ElementXmlPtr pEl = pRootChild->Getitem(n);
				if (ImportGroup (pEl, pGroup, pParent, pFileDati) == 1)
					IteraGruppo (pEl, pMachine, pGroup, pFileDati, strProjectDir);
				n++;
			}
		}
	}
	m_nLevel--;
	if (m_nLevel == 0)   {
		pMachine->SetAdamoModule ((CAdamoLogicModule *)pGroup);
		pFileDati->WriteData ();
		pFileDati->Close ();
		delete pFileDati;
	}
	else
		pParent->AddTail (pGroup);
	return 0;
}

/*
** ImportGroup :
*/
int CProject::ImportGroup (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoGroup *pParent, CAdamoDatiFile *pFileDati)
{
	int nB = 0;
	CString str = (LPCSTR)pEl->nodeName;
	if (str == "Name")   {
		CString strText = (LPCSTR)pEl->text;
		pGroup->SetName (strText);
	}
	else
	if (str == "Description")   {
		CString strText = (LPCSTR)pEl->text;
		pGroup->SetDesc (strText);
	}
	else
	if (str == "DigitalInput")
		NewDigitalInput (pEl, pGroup, pFileDati);
	else
	if (str == "DigitalOutput")
		NewDigitalOutput (pEl, pGroup, pFileDati);
	else
	if (str == "AnalogInput")
		NewAnalogInput (pEl, pGroup, pFileDati);
	else
	if (str == "AnalogOutput")
		NewAnalogInput (pEl, pGroup, pFileDati);
	else
	if (str == "Axis")
		NewAxis (pEl, pGroup, pFileDati);
	else
	if (str == "Timer")
		NewTimer (pEl, pGroup);
	else
	if (str == "Counter")
		NewCounter (pEl, pGroup);
	else
	if (str == "Group")
		nB = 1;
	return nB;
}

/*
** NewDigitalInput :
*/
void CProject::NewDigitalInput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati)
{
	CString str, strHWAddress;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoDgtIn* pDgtIn = new CAdamoDgtIn;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pDgtIn))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "Number")   {
				CString strText = (LPCSTR)pNEl->text;
				pDgtIn->SetNumInput (atoi (strText));
			}
			else
			if (str == "HWAddress")
				strHWAddress = (LPCSTR)pNEl->text;
		}
		n++;
	}
	pDgtIn->SetParentObj (pGroup);
	pFileDati->ForceAdamoDevice (pDgtIn, strHWAddress);
	pGroup->AddTail (pDgtIn);
}

/*
** NewDigitalOutput :
*/
void CProject::NewDigitalOutput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati)
{
	CString str, strHWAddress;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoDgtOut* pDgtOut = new CAdamoDgtOut;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pDgtOut))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "Number")   {
				CString strText = (LPCSTR)pNEl->text;
				pDgtOut->SetNumOutput (atoi (strText));
			}
			else
			if (str == "MonoStable")   {
				CString strText = (LPCSTR)pNEl->text;
				pDgtOut->SetIsMonoStable (atoi (strText));
			}
			else
			if (str == "Timeout")   {
				CString strText = (LPCSTR)pNEl->text;
				pDgtOut->SetTimeOut (atoi (strText));
			}
			else
			if (str == "HWAddress")
				strHWAddress = (LPCSTR)pNEl->text;
		}
		n++;
	}
	pDgtOut->SetParentObj (pGroup);
	pFileDati->ForceAdamoDevice (pDgtOut, strHWAddress);
	pGroup->AddTail (pDgtOut);
}

/*
** NewAnalogInput :
*/
void CProject::NewAnalogInput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati)
{
	CString str, strHWAddress;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoAnalogIn* pAnaIn = new CAdamoAnalogIn;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pAnaIn))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "Type")   {
				CString strText = (LPCSTR)pNEl->text;
				pAnaIn->SetAnalogType (atoi (strText));
			}
			else
			if (str == "LowRange")   {
				CString strText = (LPCSTR)pNEl->text;
				pAnaIn->SetLLInf (atof (strText));
			}
			else
			if (str == "UpperRange")   {
				CString strText = (LPCSTR)pNEl->text;
				pAnaIn->SetLLSup (atof (strText));
			}
			else
			if (str == "HWAddress")
				strHWAddress = (LPCSTR)pNEl->text;
		}
		n++;
	}
	pAnaIn->SetParentObj (pGroup);
	pFileDati->ForceAdamoDevice (pAnaIn, strHWAddress);
	pGroup->AddTail (pAnaIn);
}

/*
** NewAnalogOutput :
*/
void CProject::NewAnalogOutput (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati)
{
	CString str, strHWAddress;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoAnalogOut* pAnaOut = new CAdamoAnalogOut;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pAnaOut))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "LowRange")   {
				CString strText = (LPCSTR)pNEl->text;
				pAnaOut->SetLLInf (atof (strText));
			}
			else
			if (str == "UpperRange")   {
				CString strText = (LPCSTR)pNEl->text;
				pAnaOut->SetLLSup (atof (strText));
			}
			else
			if (str == "HWAddress")
				strHWAddress = (LPCSTR)pNEl->text;
		}
		n++;
	}
	pAnaOut->SetParentObj (pGroup);
	pFileDati->ForceAdamoDevice (pAnaOut, strHWAddress);
	pGroup->AddTail (pAnaOut);
}

/*
** NewAnalogOutput :
*/
void CProject::NewAxis (ElementXmlPtr pEl, CAdamoGroup *pGroup, CAdamoDatiFile *pFileDati)
{
	CString str, strHWAddress;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoAxis* pAxis = new CAdamoAxis;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pAxis))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "Alias")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetAlias (strText);
			}
			else
			if (str == "Resolution")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetResolution (atof (strText));
			}
			else
			if (str == "Type")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetAxisType (atoi (strText));
			}
			else
			if (str == "Control")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetTIC (atoi (strText));
			}
			else
			if (str == "MoveCategory")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetMovementCategory (atoi (strText));
			}
			else
			if (str == "AxisCategory")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetAxesCategory (atoi (strText));
			}
			else
			if (str == "Unit")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetUnit (atoi (strText));
			}
			else
			if (str == "PhaseInversion")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInvPhase (atoi (strText));
			}
			else
			if (str == "WaitStopped")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetWaitStopAxis (atoi (strText));
			}
			else
			if (str == "EnableMarker")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetEnableNick (atoi (strText));
			}
			else
			if (str == "EnableJerk")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetEnableJerk (atoi (strText));
			}
			else
			if (str == "GName")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetGName (strText);
			}
			else
			if (str == "GInterpolationChannel")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetGSlot (atoi (strText));
			}
			else
			if (str == "ReferenceVoltage")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetRif (atof (strText));
			}
			else
			if (str == "Speed")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetSpeed (atof (strText));
			}
			else
			if (str == "SpeedManual")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetSpeedMan (atof (strText));
			}
			else
			if (str == "Acceleration")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetAcc (atof (strText));
			}
			else
			if (str == "Deceleration")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetDec (atof (strText));
			}
			else
			if (str == "Gain")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetProp (atof (strText));
			}
			else
			if (str == "Derivative")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetDer (atof (strText));
			}
			else
			if (str == "Integrative")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInt (atof (strText));
			}
			else
			if (str == "FeedForward")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetFeedForward (atof (strText));
			}
			else
			if (str == "FeedForwardAcc")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetFeedForwardAcc (atof (strText));
			}
			else
			if (str == "RampType")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetRampType (atoi (strText));
			}
			else
			if (str == "RampFactor")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetPercRT (atof (strText));
			}
			else
			if (str == "SpeedW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterSpeed (atof (strText));
			}
			else
			if (str == "AccelerationW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterAcc (atof (strText));
			}
			else
			if (str == "DecelerationW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterDec (atof (strText));
			}
			else
			if (str == "GainW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterProp (atof (strText));
			}
			else
			if (str == "DerivativeW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterDer (atof (strText));
			}			
			else
			if (str == "IntegrativeW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterInt (atof (strText));
			}
			else
			if (str == "FeedForwardW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterFeedForward (atof (strText));
			}
			else
			if (str == "FeedForwardAccW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterFeedForwardAcc (atof (strText));
			}
			else
			if (str == "RampTypeW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterRampType (atoi (strText));
			}
			else
			if (str == "RampFactorW")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetInterPercRT (atof (strText));
			}
			else
			if (str == "ServoLimitType")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetServoLimitType (atoi (strText));
			}
			else
			if (str == "EALimitConc")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetLimEAConc (atof (strText));
			}
			else
			if (str == "EALimitDisc")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetLimEADisc (atof (strText));
			}
			else
			if (str == "EALimitMin")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetLimEAMin (atof (strText));
			}
			else
			if (str == "EnablePosLimit")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetEnableLimAxesPos (atoi (strText));
			}
			else
			if (str == "EnableNegLimit")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetEnableLimAxesNeg (atoi (strText));
			}
			else
			if (str == "PosLimit")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetLimAxisPos (atof (strText));
			}
			else
			if (str == "NegLimit")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetLimAxisNeg (atof (strText));
			}
			else
			if (str == "WinPos")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetWinPos (atof (strText));
			}
			else
			if (str == "WinNeg")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetWinNeg (atof (strText));
			}
			else
			if (str == "WinInPositionPos")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetRangeQuotaPos (atof (strText));
			}
			else
			if (str == "WinInPositionNeg")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetRangeQuotaNeg (atof (strText));
			}
			else
			if (str == "IntegralBuffer")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetDimBufferIntegrale (atoi (strText));
			}
			else
			if (str == "WMBCA")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetStopOnAccDecChange (atoi (strText));
			}
			else
			if (str == "KLC")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetCostanteDiTempoAzionamento (atof (strText));
			}
			else
			if (str == "WaitInPosition")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetTempoArrivoInQuota (atoi (strText));
			}
			else
			if (str == "WaitStopped")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetTempoAsseFermo (atoi (strText));
			}
			else
			if (str == "GantryAxis")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetGantryAxes (atoi (strText));
			}
			else
			if (str == "GantryGain")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetPropGantry (atoi (strText));
			}
			else
			if (str == "GantryDerivative")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetDerGantry (atof (strText));
			}
			else
			if (str == "RotationFilter")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetFiltroRotanti (atof (strText));
			}
			else
			if (str == "CCName")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetCCName (strText);
			}
			else
			if (str == "EnableBacklashComp")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetEnableRecuperoGioco (atoi (strText));
			}
			else
			if (str == "BacklaskComp")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetRecuperoGioco (atof (strText));
			}
			else
			if (str == "BacklaskCompTime")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetTempoDiRecupero (atof (strText));
			}
			else
			if (str == "CCSpeed")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetVelocitaDiCorrezione (atof (strText));
			}
			else
			if (str == "CCDynamic")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetCorrezioneDinamica (atoi (strText));
			}
			else
			if (str == "EnableEACompensation")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetEnableCompensazioneEA (atoi (strText));
			}
			else
			if (str == "EACompensation")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetCompensazioneEA (atof (strText));
			}
			else
			if (str == "Visible")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetDisplay (atoi (strText));
			}
			else
			if (str == "Torque")   {
				CString strText = (LPCSTR)pNEl->text;
				pAxis->SetPercTorque (atoi (strText));
			}
			else
			if (str == "CustomParameter")   {
				if (pEl->hasChildNodes())	  {
					int nItems, n;
					NodeListXmlPtr  pRootChild;
					ElementXmlPtr pNEl;
					pRootChild = pEl->childNodes;
					nItems=pRootChild->length;
					n=0;
					while (n<nItems)   {
						pNEl = pRootChild->Getitem(n);
						CString strText = (LPCSTR)pNEl->text;
						COleVariant v;
						if (strText == "Parameter")   {
							v=pNEl->getAttribute("Number");
							if (v.vt==VT_BSTR)   {
								stCustomData CD;
								CString str (v.bstrVal);
								int nParameterNumber = atoi (str);
								int nItems, n;
								NodeListXmlPtr  pRootChild;
								ElementXmlPtr pNElParameters;
								pRootChild = pNEl->childNodes;
								nItems=pRootChild->length;
								n=0;
								while (n<nItems)   {
									pNElParameters = pRootChild->Getitem(n);
									str = (LPCSTR)pNElParameters->nodeName;
									if (str == "Name")
										strcpy (CD.szName, pNElParameters->text);
									if (str == "Type")
										CD.nType = atoi (pNElParameters->text);
									else
									if (str == "Value")   {
										switch (CD.nType)   {
											case LUA_TNUMBER :
												CD.uData.fp = atof (pNElParameters->text);
												break;
											case LUA_TBOOLEAN :
												CD.uData.b = atoi (pNElParameters->text);
												break;
											case LUA_TSTRING :
												strcpy (CD.uData.str, pNElParameters->text);
												break;
										}
									}
									n++;
								}
							}
						}
						n++;
					}
				}
			}
			else
			if (str == "HWAddress")
				strHWAddress = (LPCSTR)pNEl->text;
		}
		n++;
	}
	pAxis->SetParentObj (pGroup);
	pFileDati->ForceAdamoDevice (pAxis, strHWAddress);
	pGroup->AddTail (pAxis);
}

/*
** NewTimer :
*/
void CProject::NewTimer (ElementXmlPtr pEl, CAdamoGroup *pGroup)
{
	CString str;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoTimer* pTimer = new CAdamoTimer;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pTimer))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "Period")   {
				CString strText = (LPCSTR)pNEl->text;
				pTimer->SetTime (atoi (strText));
			}
			else
			if (str == "Enabled")   {
				CString strText = (LPCSTR)pNEl->text;
				pTimer->SetEnabled (atoi (strText));
			}
			else
			if (str == "UpDown")   {
				CString strText = (LPCSTR)pNEl->text;
				pTimer->SetUpDown (atoi (strText));
			}
			else
			if (str == "Stored")   {
				CString strText = (LPCSTR)pNEl->text;
				pTimer->SetStored (atoi (strText));
			}
		}
		n++;
	}
	pTimer->SetParentObj (pGroup);
	pGroup->AddTail (pTimer);
}

/*
** NewCounter :
*/
void CProject::NewCounter (ElementXmlPtr pEl, CAdamoGroup *pGroup)
{
	CString str;
	int nItems, n;
    NodeListXmlPtr  pRootChild;
	ElementXmlPtr pNEl;

	CAdamoCounter* pCounter = new CAdamoCounter;
	pRootChild = pEl->childNodes;
    nItems=pRootChild->length;
    n=0;
    while (n<nItems)   {
		pNEl = pRootChild->Getitem(n);
		if (!SetBaseData (pNEl, pCounter))   {
			str = (LPCSTR)pNEl->nodeName;
			if (str == "Type")   {
				CString strText = (LPCSTR)pNEl->text;
				pCounter->SetCounterType (atoi (strText));
			}
		}
		n++;
	}
	pCounter->SetParentObj (pGroup);
	pGroup->AddTail (pCounter);
}

/*
** SetBaseData
*/
int CProject::SetBaseData (ElementXmlPtr pEl, CAdamoBase *pBase)
{
	int nB = 1;
	CString str = (LPCSTR)pEl->nodeName;
	if (str == "Name")   {
		CString strText = (LPCSTR)pEl->text;
		pBase->SetName (strText);
	}
	else
	if (str == "Description")   {
		CString strText = (LPCSTR)pEl->text;
		pBase->SetDesc (strText);
	}
	else
	if (str == "ReadAccess")   {
		CString strText = (LPCSTR)pEl->text;
		pBase->SetRAcces (atoi (strText));
	}
	else
	if (str == "WriteAccess")   {
		CString strText = (LPCSTR)pEl->text;
		pBase->SetWAcces (atoi (strText));
	}
	else
	if (str == "Public")   {
		CString strText = (LPCSTR)pEl->text;
		pBase->SetIsPublic (atoi (strText));
	}
	else
	if (str == "ID")   {
		CString strText = (LPCSTR)pEl->text;
		pBase->SetDeviceID (atoi (strText));
	}
	else
		nB = 0;
	return nB;
}

/*
** SalvaNuovaMacchina :
*/
int CProject::SalvaNuovaMacchina (CAdamoCfgMachine* pMachine, CString strProjectDir, CString strName)
{
	CString strPath = strProjectDir + "\\" + strName + ".cfg";
	CFile fout;
    int n=0;

    if (!ApriFileDiConfigurazione (strPath, &fout))
        return FALSE;
	CArchive ar(&fout, CArchive::store);
    pMachine->Serialize(ar);
    pMachine->GestisciMappaEId (GetParamsPathName(), false);
    if (pMachine->SalvaDescrizioneTradotta (&m_tr))
        AfxMessageBox (LOADSTRING (IDS_ADAMOMODULE_1));
    ar.Close ();
    fout.Close ();
	return 0;
}

/*
** RicaricaNuovaMacchina :
*/
void CProject::RicaricaNuovaMacchina ()
{
	ReloadConfiguration ();
}

/*
** AddFunctionsLibrary :
*/
void CProject::AddFunctionsLibrary (CAdamoLibrary* pLibrary)
{
	m_fc.AddLibrary (pLibrary);
}

/*
** AddObjectsLibrary :
*/
void CProject::AddObjectsLibrary (CObjectsLibrary* pObjectsLibrary)
{
	m_oc.AddObjectsLibrary (pObjectsLibrary);
}

/*
** ClearProjectInfo :
*/
void CProject::ClearProjectInfo()
{
	m_vc.ClearAll ();
	m_bc.ClearAll ();
	m_fc.ClearAll ();
}

/*
** GenerateMainObjFile :
*/
void CProject::GenerateOOPFile ()
{
    FILE* pF;
    char szBuffer[256];
	unsigned char szText[] = {
		0x1b, 0x4c, 0x75, 0x61, 0x52, 0x00, 0x01, 0x04, 0x04, 0x04, 0x08, 0x00, 0x19, 0x93, 0x0d, 0x0a,
		0x1a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0x4e, 0x00, 0x00,
		0x00, 0x0b, 0x00, 0x00, 0x00, 0x46, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x00, 0xcb, 0x40, 0x00,
		0x00, 0xca, 0x80, 0xc0, 0x80, 0x5d, 0x40, 0x80, 0x01, 0x44, 0x00, 0x80, 0x00, 0xe5, 0x00, 0x00,
		0x00, 0x25, 0x41, 0x00, 0x00, 0x4b, 0x81, 0x03, 0x00, 0xa5, 0x81, 0x00, 0x00, 0x4a, 0x81, 0x01,
		0x82, 0xa5, 0xc1, 0x00, 0x00, 0x4a, 0x81, 0x81, 0x82, 0xa5, 0x01, 0x01, 0x00, 0x4a, 0x81, 0x01,
		0x83, 0xa5, 0x41, 0x01, 0x00, 0x4a, 0x81, 0x81, 0x83, 0xa5, 0x81, 0x01, 0x00, 0x4a, 0x81, 0x01,
		0x84, 0xa5, 0xc1, 0x01, 0x00, 0x4a, 0x81, 0x81, 0x84, 0xa5, 0x01, 0x02, 0x00, 0x4a, 0x81, 0x01,
		0x85, 0xa5, 0x41, 0x02, 0x00, 0x4a, 0x81, 0x81, 0x85, 0xa5, 0x81, 0x02, 0x00, 0x4a, 0x81, 0x01,
		0x86, 0xa5, 0xc1, 0x02, 0x00, 0x4a, 0x81, 0x81, 0x86, 0xa5, 0x01, 0x03, 0x00, 0x4a, 0x81, 0x01,
		0x87, 0xa5, 0x41, 0x03, 0x00, 0x4a, 0x81, 0x81, 0x87, 0xa5, 0x81, 0x03, 0x00, 0x4a, 0x81, 0x01,
		0x88, 0xa5, 0xc1, 0x03, 0x00, 0x4a, 0x81, 0x81, 0x88, 0x08, 0x40, 0x81, 0x81, 0x46, 0xc1, 0x40,
		0x00, 0x4c, 0xc1, 0xc3, 0x02, 0xc3, 0x01, 0x00, 0x00, 0x5d, 0x41, 0x80, 0x01, 0x46, 0xc1, 0x40,
		0x00, 0x8b, 0xc1, 0x00, 0x00, 0xe5, 0x01, 0x04, 0x00, 0x8a, 0xc1, 0x81, 0x89, 0xe5, 0x41, 0x04,
		0x00, 0x8a, 0xc1, 0x01, 0x8a, 0xe5, 0x81, 0x04, 0x00, 0x8a, 0xc1, 0x81, 0x8a, 0x4a, 0x81, 0x01,
		0x89, 0x46, 0xc1, 0x40, 0x00, 0x47, 0x81, 0xc4, 0x02, 0x86, 0xc1, 0x40, 0x00, 0x87, 0x81, 0x44,
		0x03, 0x4a, 0x81, 0x01, 0x8b, 0x46, 0x01, 0x40, 0x00, 0x86, 0xc1, 0x40, 0x00, 0xcb, 0xc1, 0x00,
		0x00, 0x06, 0xc2, 0x40, 0x00, 0x07, 0x02, 0x42, 0x04, 0xca, 0x01, 0x82, 0x8b, 0x25, 0xc2, 0x04,
		0x00, 0xca, 0x01, 0x02, 0x8a, 0x06, 0xc2, 0x40, 0x00, 0x07, 0x42, 0x44, 0x04, 0xca, 0x01, 0x02,
		0x8c, 0x5d, 0x41, 0x80, 0x01, 0x46, 0xc1, 0x40, 0x00, 0x8b, 0x41, 0x00, 0x00, 0xcb, 0x01, 0x00,
		0x00, 0x8a, 0xc1, 0x81, 0x8c, 0x0a, 0x80, 0x81, 0x02, 0x65, 0x01, 0x05, 0x00, 0x08, 0x40, 0x81,
		0x8a, 0x57, 0xc0, 0xff, 0x7f, 0x1f, 0x00, 0x80, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x00,
		0x00, 0x00, 0x73, 0x65, 0x74, 0x6d, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x00, 0x04,
		0x07, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00,
		0x6b, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x00, 0x04, 0x05,
		0x00, 0x00, 0x00, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x73, 0x75, 0x70,
		0x65, 0x72, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x68, 0x65, 0x72, 0x69, 0x74, 0x73,
		0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x00, 0x04, 0x04,
		0x00, 0x00, 0x00, 0x6e, 0x65, 0x77, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x74, 0x72, 0x79, 0x43,
		0x61, 0x73, 0x74, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x63, 0x61, 0x73, 0x74, 0x00, 0x04, 0x05,
		0x00, 0x00, 0x00, 0x6d, 0x61, 0x64, 0x65, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x73, 0x75, 0x62,
		0x43, 0x6c, 0x61, 0x73, 0x73, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x65, 0x78, 0x74, 0x65, 0x6e,
		0x64, 0x73, 0x00, 0x04, 0x0d, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x6b, 0x65, 0x49, 0x6e, 0x73, 0x74,
		0x61, 0x6e, 0x63, 0x65, 0x00, 0x04, 0x12, 0x00, 0x00, 0x00, 0x73, 0x65, 0x74, 0x44, 0x65, 0x66,
		0x61, 0x75, 0x6c, 0x74, 0x56, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x00, 0x04, 0x12, 0x00, 0x00,
		0x00, 0x67, 0x65, 0x74, 0x44, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x56, 0x69, 0x72, 0x74, 0x75,
		0x61, 0x6c, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x6e, 0x65, 0x77, 0x4d, 0x65, 0x74, 0x68, 0x6f,
		0x64, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x73, 0x74, 0x61, 0x74, 0x69, 0x63, 0x00, 0x04, 0x05,
		0x00, 0x00, 0x00, 0x69, 0x6e, 0x69, 0x74, 0x00, 0x04, 0x0b, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x74,
		0x6f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x63, 0x6c, 0x61,
		0x73, 0x73, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x00,
		0x04, 0x07, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x63, 0x61, 0x6c, 0x6c, 0x00, 0x04, 0x0b, 0x00, 0x00,
		0x00, 0x5f, 0x5f, 0x6e, 0x65, 0x77, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x00, 0x04, 0x09, 0x00, 0x00,
		0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x73, 0x00, 0x15, 0x00, 0x00, 0x00, 0x17, 0x00,
		0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x01, 0x01, 0x04, 0x06, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00,
		0x00, 0x85, 0x00, 0x80, 0x00, 0xe6, 0x00, 0x00, 0x00, 0x5e, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00,
		0x00, 0x1f, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x03, 0x00, 0x09,
		0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0xc0, 0x00, 0x17, 0x40, 0x00, 0x80, 0xcb, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x80, 0x01, 0x18, 0x00, 0x40, 0x01, 0x17, 0xc0, 0x01, 0x80, 0xc6, 0x40, 0x40, 0x00,
		0x00, 0x01, 0x00, 0x00, 0xdd, 0x00, 0x01, 0x01, 0x17, 0x00, 0x00, 0x80, 0x4a, 0xc0, 0x01, 0x03,
		0xe2, 0x80, 0x00, 0x00, 0x63, 0x01, 0xff, 0x7f, 0x17, 0xc0, 0x01, 0x80, 0xc6, 0x40, 0x40, 0x00,
		0x00, 0x01, 0x00, 0x01, 0xdd, 0x00, 0x01, 0x01, 0x17, 0x40, 0x00, 0x80, 0x07, 0xc2, 0x01, 0x00,
		0x4a, 0x00, 0x82, 0x03, 0xe2, 0x80, 0x00, 0x00, 0x63, 0xc1, 0xfe, 0x7f, 0x5f, 0x00, 0x00, 0x01,
		0x1f, 0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x70, 0x61,
		0x69, 0x72, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x00,
		0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00,
		0x00, 0x5f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00,
		0x00, 0x00, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x2a, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00,
		0x44, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x9f, 0x00, 0x00, 0x01, 0x1f,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2d,
		0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0a, 0x19, 0x00, 0x00, 0x00, 0x86, 0x00,
		0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x9d, 0x80, 0x00, 0x01, 0x18, 0x40, 0x40, 0x01, 0x17, 0x40,
		0x02, 0x80, 0x86, 0x80, 0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x9d, 0x00, 0x01, 0x01, 0x17, 0x80,
		0x00, 0x80, 0xcc, 0xc1, 0x40, 0x00, 0x40, 0x02, 0x80, 0x02, 0xdd, 0x41, 0x80, 0x01, 0xa2, 0x80,
		0x00, 0x00, 0x23, 0x81, 0xfe, 0x7f, 0x17, 0x00, 0x02, 0x80, 0x87, 0x00, 0x41, 0x00, 0x87, 0x40,
		0x00, 0x01, 0x18, 0x40, 0x41, 0x01, 0x17, 0x40, 0x00, 0x80, 0xe5, 0x00, 0x00, 0x00, 0x80, 0x00,
		0x80, 0x01, 0xc6, 0x00, 0x80, 0x00, 0xc7, 0x80, 0xc1, 0x01, 0xca, 0x80, 0x80, 0x00, 0x1f, 0x00,
		0x80, 0x00, 0x07, 0x00, 0x00, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x00,
		0x04, 0x06, 0x00, 0x00, 0x00, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00,
		0x70, 0x61, 0x69, 0x72, 0x73, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x76, 0x69, 0x72, 0x74, 0x75,
		0x61, 0x6c, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x73, 0x74, 0x61, 0x74, 0x69, 0x63, 0x00, 0x00,
		0x04, 0x09, 0x00, 0x00, 0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x73, 0x00, 0x01, 0x00,
		0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00,
		0x00, 0x06, 0x00, 0x40, 0x00, 0x41, 0x40, 0x00, 0x00, 0x85, 0x00, 0x80, 0x00, 0xc1, 0x80, 0x00,
		0x00, 0x56, 0xc0, 0x80, 0x00, 0x1d, 0x40, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x03, 0x00, 0x00,
		0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x00, 0x04, 0x2f, 0x00, 0x00,
		0x00, 0x41, 0x74, 0x74, 0x65, 0x6d, 0x70, 0x74, 0x20, 0x74, 0x6f, 0x20, 0x63, 0x61, 0x6c, 0x6c,
		0x20, 0x61, 0x6e, 0x20, 0x75, 0x6e, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x64, 0x20, 0x61, 0x62,
		0x73, 0x74, 0x72, 0x61, 0x63, 0x74, 0x20, 0x6d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x20, 0x27, 0x00,
		0x04, 0x02, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x3d,
		0x00, 0x00, 0x00, 0x01, 0x01, 0x05, 0x09, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x40, 0x00, 0xc6, 0x00,
		0x00, 0x00, 0xc7, 0x40, 0xc0, 0x01, 0x5d, 0x80, 0x80, 0x01, 0x8c, 0x80, 0xc0, 0x00, 0x26, 0x01,
		0x00, 0x00, 0x9d, 0x40, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x03, 0x00,
		0x00, 0x00, 0x04, 0x0d, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x6b, 0x65, 0x49, 0x6e, 0x73, 0x74, 0x61,
		0x6e, 0x63, 0x65, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c,
		0x73, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x02, 0x00,
		0x04, 0x1c, 0x00, 0x00, 0x00, 0x86, 0x40, 0x00, 0x00, 0xc7, 0x00, 0x40, 0x01, 0x18, 0x00, 0x80,
		0x01, 0x17, 0x00, 0x00, 0x80, 0x5f, 0x00, 0x00, 0x01, 0x58, 0x40, 0x40, 0x01, 0x17, 0x80, 0x01,
		0x80, 0xc7, 0x00, 0x40, 0x01, 0x18, 0x00, 0x80, 0x01, 0x17, 0x40, 0x00, 0x80, 0xc7, 0x80, 0x40,
		0x01, 0xdf, 0x00, 0x00, 0x01, 0x87, 0xc0, 0x40, 0x01, 0x17, 0x80, 0xfd, 0x7f, 0xc6, 0x40, 0x00,
		0x00, 0x87, 0x00, 0xc1, 0x01, 0x58, 0x40, 0x40, 0x01, 0x17, 0x80, 0x01, 0x80, 0xc7, 0x00, 0x40,
		0x01, 0x18, 0x00, 0x80, 0x01, 0x17, 0x40, 0x00, 0x80, 0xc7, 0x80, 0x40, 0x01, 0xdf, 0x00, 0x00,
		0x01, 0x87, 0x00, 0x41, 0x01, 0x17, 0x80, 0xfd, 0x7f, 0xc4, 0x00, 0x00, 0x00, 0xdf, 0x00, 0x00,
		0x01, 0x1f, 0x00, 0x80, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x63, 0x6c,
		0x61, 0x73, 0x73, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x6f, 0x62, 0x6a, 0x00, 0x04, 0x06,
		0x00, 0x00, 0x00, 0x6c, 0x6f, 0x77, 0x65, 0x72, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x73, 0x75,
		0x70, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x00,
		0x00, 0x00, 0x5a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x12, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x40,
		0x00, 0x00, 0x01, 0x80, 0x00, 0x9d, 0x80, 0x80, 0x01, 0x18, 0x40, 0x40, 0x01, 0x17, 0x80, 0x02,
		0x80, 0xc6, 0x80, 0x40, 0x00, 0x01, 0xc1, 0x00, 0x00, 0x46, 0x01, 0x41, 0x00, 0x80, 0x01, 0x80,
		0x00, 0x5d, 0x81, 0x00, 0x01, 0x81, 0x41, 0x01, 0x00, 0xc6, 0x81, 0x41, 0x00, 0xcc, 0xc1, 0xc1,
		0x03, 0xdd, 0x81, 0x00, 0x01, 0x16, 0xc1, 0x01, 0x02, 0xdd, 0x40, 0x00, 0x01, 0x9f, 0x00, 0x00,
		0x01, 0x1f, 0x00, 0x80, 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x74, 0x72,
		0x79, 0x43, 0x61, 0x73, 0x74, 0x00, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x65, 0x72, 0x72, 0x6f,
		0x72, 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x46, 0x61, 0x69, 0x6c, 0x65, 0x64, 0x20, 0x74, 0x6f,
		0x20, 0x63, 0x61, 0x73, 0x74, 0x20, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x74, 0x6f, 0x73, 0x74,
		0x72, 0x69, 0x6e, 0x67, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x20, 0x74, 0x6f, 0x20, 0x61, 0x20,
		0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x00, 0x04, 0x05, 0x00, 0x00,
		0x00, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x5d, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0x0e, 0x00, 0x00, 0x00, 0x86,
		0x40, 0x00, 0x00, 0x18, 0x00, 0x40, 0x01, 0x17, 0x40, 0x00, 0x80, 0x83, 0x00, 0x00, 0x00, 0x9f,
		0x00, 0x00, 0x01, 0x8c, 0x40, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x9d, 0x80, 0x80, 0x01, 0x18,
		0x00, 0x40, 0x01, 0x17, 0x00, 0x00, 0x80, 0x83, 0x40, 0x00, 0x00, 0x83, 0x00, 0x80, 0x00, 0x9f,
		0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00,
		0x00, 0x74, 0x72, 0x79, 0x43, 0x61, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x16, 0x4f, 0x00,
		0x00, 0x00, 0x86, 0x00, 0x40, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x9d, 0x80, 0x00, 0x01, 0x58, 0x40,
		0x40, 0x01, 0x17, 0x00, 0x00, 0x80, 0x41, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xcb, 0xc0,
		0x00, 0x00, 0x25, 0x01, 0x00, 0x00, 0xca, 0x00, 0x81, 0x81, 0x25, 0x41, 0x00, 0x00, 0xca, 0x00,
		0x01, 0x82, 0x25, 0x81, 0x00, 0x00, 0xca, 0x00, 0x81, 0x82, 0x05, 0x01, 0x80, 0x00, 0x40, 0x01,
		0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0xcb, 0x01, 0x80, 0x05, 0x01, 0x82, 0x01, 0x00, 0x41, 0xc2,
		0x01, 0x00, 0x81, 0x02, 0x02, 0x00, 0xc1, 0x42, 0x02, 0x00, 0x01, 0x83, 0x02, 0x00, 0x41, 0xc3,
		0x02, 0x00, 0x81, 0x03, 0x03, 0x00, 0xc1, 0x43, 0x03, 0x00, 0x01, 0x84, 0x03, 0x00, 0x41, 0xc4,
		0x03, 0x00, 0x81, 0x04, 0x04, 0x00, 0xe4, 0x41, 0x80, 0x05, 0x1d, 0x41, 0x00, 0x02, 0x0b, 0x81,
		0x00, 0x00, 0x65, 0xc1, 0x00, 0x00, 0x0a, 0x41, 0x01, 0x89, 0x65, 0x01, 0x01, 0x00, 0x0a, 0x41,
		0x81, 0x89, 0xca, 0x00, 0x81, 0x88, 0x07, 0x41, 0xc4, 0x01, 0x65, 0x41, 0x01, 0x00, 0x0a, 0x41,
		0x01, 0x8a, 0x05, 0x01, 0x80, 0x00, 0x47, 0x41, 0x44, 0x01, 0x87, 0x41, 0xc4, 0x01, 0xcb, 0x01,
		0x00, 0x07, 0x01, 0x42, 0x05, 0x00, 0x41, 0x82, 0x05, 0x00, 0x81, 0xc2, 0x05, 0x00, 0xc1, 0x02,
		0x06, 0x00, 0x01, 0x43, 0x06, 0x00, 0x41, 0x83, 0x06, 0x00, 0x81, 0xc3, 0x06, 0x00, 0xc1, 0x03,
		0x07, 0x00, 0x01, 0x44, 0x07, 0x00, 0x41, 0x84, 0x07, 0x00, 0x81, 0xc4, 0x07, 0x00, 0xc1, 0x04,
		0x08, 0x00, 0x01, 0x45, 0x08, 0x00, 0x41, 0x85, 0x08, 0x00, 0xe4, 0x41, 0x00, 0x07, 0x1d, 0x41,
		0x00, 0x02, 0x06, 0xc1, 0x48, 0x00, 0x40, 0x01, 0x80, 0x01, 0x8b, 0xc1, 0x00, 0x00, 0xc7, 0xc1,
		0xc2, 0x01, 0x8a, 0xc1, 0x01, 0x8b, 0xe5, 0x81, 0x01, 0x00, 0x8a, 0xc1, 0x81, 0x90, 0xc7, 0x81,
		0xc2, 0x01, 0x8a, 0xc1, 0x81, 0x89, 0x1d, 0x41, 0x80, 0x01, 0x0b, 0x41, 0x00, 0x00, 0x45, 0x01,
		0x80, 0x00, 0x86, 0x81, 0x80, 0x02, 0x87, 0x01, 0x49, 0x03, 0x5d, 0x81, 0x00, 0x01, 0x0a, 0x41,
		0x01, 0x92, 0x48, 0x01, 0x81, 0x01, 0xdf, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x25, 0x00,
		0x00, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x00, 0x04, 0x07, 0x00, 0x00,
		0x00, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x55, 0x6e, 0x6e,
		0x61, 0x6d, 0x65, 0x64, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x6e, 0x61, 0x6d, 0x65, 0x00, 0x04,
		0x06, 0x00, 0x00, 0x00, 0x73, 0x75, 0x70, 0x65, 0x72, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x69,
		0x6e, 0x68, 0x65, 0x72, 0x69, 0x74, 0x73, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x63, 0x61, 0x73,
		0x74, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x65, 0x78, 0x74, 0x65, 0x6e, 0x64, 0x73, 0x00, 0x04,
		0x05, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x64, 0x65, 0x00, 0x04, 0x0d, 0x00, 0x00, 0x00, 0x6d, 0x61,
		0x6b, 0x65, 0x49, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x00,
		0x6e, 0x65, 0x77, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x6e,
		0x65, 0x77, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x73, 0x75, 0x62, 0x43, 0x6c, 0x61, 0x73, 0x73,
		0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x74, 0x72, 0x79, 0x43, 0x61, 0x73, 0x74, 0x00, 0x04, 0x12,
		0x00, 0x00, 0x00, 0x73, 0x65, 0x74, 0x44, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x56, 0x69, 0x72,
		0x74, 0x75, 0x61, 0x6c, 0x00, 0x04, 0x12, 0x00, 0x00, 0x00, 0x67, 0x65, 0x74, 0x44, 0x65, 0x66,
		0x61, 0x75, 0x6c, 0x74, 0x56, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x00, 0x04, 0x08, 0x00, 0x00,
		0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x73, 0x74,
		0x61, 0x74, 0x69, 0x63, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x00,
		0x04, 0x0b, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x6e, 0x65, 0x77, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x00,
		0x04, 0x08, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x00, 0x04, 0x06, 0x00,
		0x00, 0x00, 0x5f, 0x5f, 0x61, 0x64, 0x64, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x63,
		0x61, 0x6c, 0x6c, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x63, 0x6f, 0x6e, 0x63, 0x61,
		0x74, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x64, 0x69, 0x76, 0x00, 0x04, 0x05, 0x00,
		0x00, 0x00, 0x5f, 0x5f, 0x65, 0x71, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x6c, 0x65,
		0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x6c, 0x65, 0x6e, 0x00, 0x04, 0x05, 0x00, 0x00,
		0x00, 0x5f, 0x5f, 0x6c, 0x74, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x6d, 0x6f, 0x64,
		0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x6d, 0x75, 0x6c, 0x00, 0x04, 0x06, 0x00, 0x00,
		0x00, 0x5f, 0x5f, 0x70, 0x6f, 0x77, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x73, 0x75,
		0x62, 0x00, 0x04, 0x0b, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x74, 0x6f, 0x73, 0x74, 0x72, 0x69, 0x6e,
		0x67, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x5f, 0x5f, 0x75, 0x6e, 0x6d, 0x00, 0x04, 0x0d, 0x00,
		0x00, 0x00, 0x73, 0x65, 0x74, 0x6d, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x00, 0x04,
		0x09, 0x00, 0x00, 0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x73, 0x00, 0x07, 0x00, 0x00,
		0x00, 0x69, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00,
		0x45, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0x00, 0x00, 0x00, 0x6a, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00,
		0x01, 0x1f, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0x0a, 0x00,
		0x00, 0x00, 0x58, 0x40, 0x00, 0x00, 0x17, 0x00, 0x01, 0x80, 0x8c, 0x00, 0x40, 0x00, 0x00, 0x01,
		0x80, 0x00, 0x9d, 0x80, 0x80, 0x01, 0x17, 0x40, 0x00, 0x80, 0x83, 0x40, 0x00, 0x00, 0x83, 0x00,
		0x80, 0x00, 0x9f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x09,
		0x00, 0x00, 0x00, 0x69, 0x6e, 0x68, 0x65, 0x72, 0x69, 0x74, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03,
		0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x0d, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x40, 0x00,
		0xc7, 0x40, 0x80, 0x01, 0x58, 0x40, 0xc0, 0x01, 0x17, 0x80, 0x00, 0x80, 0xc7, 0x00, 0x40, 0x00,
		0xca, 0x80, 0x80, 0x00, 0x17, 0x00, 0x01, 0x80, 0xc6, 0x80, 0x40, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x40, 0x01, 0x80, 0x00, 0x80, 0x01, 0x00, 0x01, 0xdd, 0x40, 0x00, 0x02, 0x1f, 0x00, 0x80, 0x00,
		0x03, 0x00, 0x00, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x73, 0x75, 0x70, 0x65, 0x72, 0x00, 0x00,
		0x04, 0x07, 0x00, 0x00, 0x00, 0x72, 0x61, 0x77, 0x73, 0x65, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x8f, 0x00, 0x00, 0x00, 0x02, 0x00,
		0x05, 0x16, 0x00, 0x00, 0x00, 0x86, 0x00, 0x40, 0x00, 0x87, 0x40, 0x00, 0x01, 0x58, 0x40, 0x40,
		0x01, 0x17, 0x00, 0x00, 0x80, 0x9f, 0x00, 0x00, 0x01, 0xc7, 0x80, 0x40, 0x00, 0x87, 0x40, 0x80,
		0x01, 0xc6, 0xc0, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x01, 0xdd, 0x80, 0x00, 0x01, 0x18, 0x00, 0xc1,
		0x01, 0x17, 0xc0, 0x01, 0x80, 0xc5, 0x00, 0x00, 0x01, 0x58, 0xc0, 0x00, 0x01, 0x17, 0x00, 0x01,
		0x80, 0xc7, 0x80, 0x40, 0x00, 0xc9, 0x00, 0x80, 0x01, 0x89, 0x00, 0x00, 0x02, 0xc5, 0x00, 0x00,
		0x01, 0xdf, 0x00, 0x00, 0x01, 0x9f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x05, 0x00, 0x00,
		0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x73, 0x74, 0x61, 0x74, 0x69, 0x63, 0x00, 0x00, 0x04, 0x06,
		0x00, 0x00, 0x00, 0x73, 0x75, 0x70, 0x65, 0x72, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x74, 0x79,
		0x70, 0x65, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00,
		0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x97, 0x00, 0x00, 0x00, 0x97, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x05, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x16, 0x40, 0x00, 0x00, 0x1f, 0x00,
		0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x63,
		0x6c, 0x61, 0x73, 0x73, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x01, 0x03, 0x01, 0x01, 0x01, 0x02, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xa3, 0x00, 0x00, 0x00, 0xa6, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0x0b, 0x00, 0x00, 0x00, 0x85,
		0x00, 0x00, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x9d, 0x40, 0x80, 0x01, 0x87,
		0x00, 0xc0, 0x00, 0x58, 0x40, 0x40, 0x01, 0x17, 0x80, 0x00, 0x80, 0x8c, 0x00, 0xc0, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x9d, 0x40, 0x80, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04,
		0x09, 0x00, 0x00, 0x00, 0x65, 0x78, 0x74, 0x65, 0x6e, 0x64, 0x65, 0x64, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x07, 0x26, 0x00, 0x00, 0x00, 0x58, 0x00, 0xc0, 0x00, 0x17, 0x40, 0x00, 0x80, 0x5b,
		0x40, 0x00, 0x00, 0x17, 0x40, 0x00, 0x80, 0x86, 0x00, 0x00, 0x00, 0x47, 0x40, 0x40, 0x01, 0x85,
		0x00, 0x80, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x9d, 0x80, 0x00, 0x01, 0xcb, 0x80, 0x00, 0x00, 0xca,
		0x80, 0x00, 0x81, 0xca, 0x00, 0x80, 0x81, 0x08, 0xc0, 0x00, 0x01, 0xcc, 0x00, 0x41, 0x00, 0xdd,
		0x80, 0x00, 0x01, 0x58, 0x00, 0xc0, 0x01, 0x17, 0x00, 0x03, 0x80, 0x0c, 0x41, 0xc1, 0x01, 0x80,
		0x01, 0x80, 0x00, 0x1d, 0x81, 0x80, 0x01, 0x8a, 0x00, 0x01, 0x82, 0x06, 0x81, 0x00, 0x00, 0x47,
		0x01, 0x41, 0x01, 0x46, 0x41, 0x01, 0x00, 0x0a, 0x41, 0x01, 0x82, 0x07, 0x01, 0x41, 0x01, 0x06,
		0x01, 0x01, 0x00, 0x46, 0x81, 0x00, 0x00, 0x0a, 0x41, 0x01, 0x83, 0x17, 0x40, 0x00, 0x80, 0x0b,
		0x01, 0x00, 0x00, 0x8a, 0x00, 0x01, 0x82, 0x06, 0xc1, 0x41, 0x01, 0x40, 0x01, 0x00, 0x01, 0x87,
		0x01, 0x42, 0x00, 0x1d, 0x41, 0x80, 0x01, 0x9f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x09,
		0x00, 0x00, 0x00, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c,
		0x73, 0x00, 0x04, 0x04, 0x00, 0x00, 0x00, 0x6f, 0x62, 0x6a, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00,
		0x63, 0x6c, 0x61, 0x73, 0x73, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x73, 0x75, 0x70, 0x65, 0x72,
		0x00, 0x04, 0x0d, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x6b, 0x65, 0x49, 0x6e, 0x73, 0x74, 0x61, 0x6e,
		0x63, 0x65, 0x00, 0x04, 0x06, 0x00, 0x00, 0x00, 0x6c, 0x6f, 0x77, 0x65, 0x72, 0x00, 0x04, 0x0d,
		0x00, 0x00, 0x00, 0x73, 0x65, 0x74, 0x6d, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6c, 0x65, 0x00,
		0x04, 0x07, 0x00, 0x00, 0x00, 0x73, 0x74, 0x61, 0x74, 0x69, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x00, 0x00, 0x00, 0xbf, 0x00,
		0x00, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x0a, 0x40, 0x00, 0x80, 0x1f, 0x00, 0x80,
		0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x0f, 0x00, 0x00, 0x00, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c,
		0x74, 0x56, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xc1, 0x00, 0x00, 0x00, 0xc3, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00,
		0x47, 0x00, 0x40, 0x00, 0x5f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x04, 0x0f, 0x00, 0x00, 0x00, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x56, 0x69, 0x72, 0x74,
		0x75, 0x61, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0x00, 0x00, 0x00,
		0xcb, 0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x0e, 0x00, 0x00, 0x00, 0xc7, 0x00, 0x40, 0x00, 0xca,
		0x80, 0x80, 0x00, 0xc7, 0x40, 0x40, 0x00, 0x58, 0x80, 0xc0, 0x01, 0x17, 0x00, 0x01, 0x80, 0xc6,
		0x00, 0x00, 0x00, 0xc7, 0xc0, 0xc0, 0x01, 0xc7, 0x40, 0x80, 0x01, 0x58, 0x00, 0xc1, 0x01, 0x17,
		0x80, 0x00, 0x80, 0xc6, 0x00, 0x00, 0x00, 0xc7, 0xc0, 0xc0, 0x01, 0xca, 0x80, 0x80, 0x00, 0x1f,
		0x00, 0x80, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x73, 0x74, 0x61, 0x74,
		0x69, 0x63, 0x00, 0x04, 0x0f, 0x00, 0x00, 0x00, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x56,
		0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x00, 0x01, 0x01, 0x04, 0x09, 0x00, 0x00, 0x00, 0x76, 0x69,
		0x72, 0x74, 0x75, 0x61, 0x6c, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xd1, 0x00, 0x00, 0x00, 0xd1, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00,
		0x00, 0x1f, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0xd2, 0x00, 0x00, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00,
		0x41, 0x00, 0x00, 0x00, 0x8c, 0x40, 0x40, 0x00, 0x9d, 0x80, 0x00, 0x01, 0x8c, 0x80, 0x40, 0x01,
		0x9d, 0x80, 0x00, 0x01, 0x56, 0x80, 0x80, 0x00, 0x5f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00,
		0x03, 0x00, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x00, 0x61, 0x20, 0x00, 0x04, 0x06, 0x00, 0x00,
		0x00, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x6e, 0x61, 0x6d, 0x65,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd3, 0x00, 0x00, 0x00, 0xd3, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x40, 0x00, 0x1f, 0x00, 0x00, 0x01,
		0x1f, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x4f, 0x62, 0x6a,
		0x65, 0x63, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xda, 0x00,
		0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x00, 0x1f, 0x00, 0x00, 0x01, 0x1f, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x00,
		0x00, 0x00, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x20, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x00, 0x00, 0x00, 0xe6, 0x00, 0x00, 0x00, 0x02,
		0x00, 0x05, 0x08, 0x00, 0x00, 0x00, 0x5b, 0x40, 0x00, 0x00, 0x17, 0x00, 0x00, 0x80, 0x46, 0x00,
		0x40, 0x00, 0x8c, 0x40, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x00, 0x9e, 0x00, 0x80, 0x01, 0x9f, 0x00,
		0x00, 0x00, 0x1f, 0x00, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x4f,
		0x62, 0x6a, 0x65, 0x63, 0x74, 0x00, 0x04, 0x09, 0x00, 0x00, 0x00, 0x73, 0x75, 0x62, 0x43, 0x6c,
		0x61, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};
    strcpy (szBuffer, GetIntermediateDir() + "\\" + _OOPOBJ);
    if (pF=fopen (szBuffer, "wb"))    {
        fwrite (szText, sizeof (szText), 1, pF);
        fclose (pF);
    }
}

/*
** AddLibrary :
*/
void CProject::AddLibrary ()
{
	CAdamoDlgLuaLibraries dlg;

	dlg.DoModal ();
}

/*
** LoadLibraries :
*/
void CProject::LoadLibraries (CArchive &ar)
{
	try {
		/* uela, ci sono librerie, leggiamole */
		CString str;
		int nLibCount;
		ar >> nLibCount;
		while (nLibCount > 0)   {
			ar >> str;
			m_listLibraries.AddTail (str);
			nLibCount--;
		}
	}
	catch (...)   {
		/* se viene generata un eccezzione, vuol dire che era una versione vecchia, non aveva le librerie, niente paura */
	}
}

/*
** SaveLibraries :
*/
void CProject::SaveLibraries (CArchive &ar)
{
	int nLibraries = m_listLibraries.GetCount ();
	ar << nLibraries;
	for (POSITION pos = m_listLibraries.GetHeadPosition (); pos; )   {
		CString strLibrary = m_listLibraries.GetNext (pos);
		ar << strLibrary;
	}
}

/*
** InstallLibraries :
*/
void CProject::InstallLibraries ()
{
	CString strBasePathName = GETWS ()->GetBasePathName (), strDescription;
	int nVersion;
	/* prendiamo il view delle funzioni */
	CHelpOnLineView* pHelpOnLineFunctions = ((CMainFrame*) AfxGetMainWnd ())->GetDockHelpOnLine ()->GetView ();
	/* prendiamo il view degli oggetti */
	CThiraLuaObjectsView* pHelpOnLineObjects = ((CMainFrame*) AfxGetMainWnd ())->GetDockThiraLuaObjects ()->GetView ();
	/* ripuliamo i nodi esistenti */
	pHelpOnLineFunctions->DeleteLibraryFunctions ();
	pHelpOnLineObjects->DeleteLibraryObjects ();
	/* aggiungiamo ora le functions di libreria */
	for (POSITION pos = m_listLibraries.GetHeadPosition (); pos; )   {
		CString strLibrary = m_listLibraries.GetNext (pos);
		CString strPathName = strBasePathName + "\\" + DIR_LIBRARIES + "\\" + strLibrary;
		if (CAdamoLuaLibrary::IsLuaLibraryFile (strPathName, nVersion, strDescription))   {
			/* apriamo il file e leggiamo l'xml */
			CAdamoLuaLibrary* pLuaLibrary = new CAdamoLuaLibrary;
			if (!pLuaLibrary->Load (strPathName))   {
				/* caricato la libreria, ora accediamo ai moduli */
				for (int i = 0; i<pLuaLibrary->GetLibraryModulesCount (); i++)   {
					CAdamoLuaLibraryModule* pLuaModule = pLuaLibrary->GetLibraryModules (i);
					pHelpOnLineFunctions->AddLibraryFunction (strLibrary, pLuaModule->GetName (), pLuaModule->GetListFunctions ());
					pHelpOnLineObjects->AddLibraryObjects (strLibrary, pLuaModule->GetName (), pLuaModule->GetListObjects ());
				}
			}
		}
	}
}

/*
** GetProjectFiles :
*/
void CProject::GetProjectFiles (CStringList& listProjectFiles)
{
	for (int i = 0; i<NofFiles (); i++)
		listProjectFiles.AddTail (m_files[i]->GetName ());
}

/*
** ImportHWCanOpen :
*/
int CProject::ImportHWCanOpen (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nCanale)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr   pElChild;
	pRootChild = pEl->childNodes;
	CString str, strNode, strText;
	int nItems=pRootChild->length;
	int n = 0;
    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "Baudrate")   {
			strText = (LPCSTR)pElChild->text;
			int nBaudrate = atoi (strText);
			pHWConfigFile->SetCanBaudrate (nCanale, nBaudrate);
		}
		else
		if (str == "SyncFrequency")   {
			strText = (LPCSTR)pElChild->text;
			int nSyncFrequency = atoi (strText);
			pHWConfigFile->SetSyncFrequency (nCanale, nSyncFrequency);
		}
		else
		if (str == "SDOTimeout")   {
			strText = (LPCSTR)pElChild->text;
			int nSDOTimeout = atoi (strText);
			pHWConfigFile->SetSDOTimeOut (nCanale, nSDOTimeout);
		}
		else
		if (str == "GuardTime")   {
			strText = (LPCSTR)pElChild->text;
			int nGuardTime = atoi (strText);
			pHWConfigFile->SetGuardTime (nCanale, nGuardTime);
		}
		else
		if (str == "LifeTime")   {
			strText = (LPCSTR)pElChild->text;
			int nLifeTime = atoi (strText);
			pHWConfigFile->SetLifeTime (nCanale, nLifeTime);
		}
		else
		if (str == "EnableWarning")   {
			strText = (LPCSTR)pElChild->text;
			int nEnableWarning = atoi (strText);
			pHWConfigFile->SetEnableWarnings (nCanale, nEnableWarning);
		}
		else
			for (int i = 0; i<NMAX_CANNODES; i++)   {
				strNode.Format ("Node_%d", i+1);
				if (str == strNode)   {
					ImportHWCanOpenNode (pElChild, pHWConfigFile, i, nCanale);
				}
			}
		n++;
	}
	return 0;
}

/*
** ImportHWSSCNET :
*/
int CProject::ImportHWSSCNET (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nCanale)
{
	DWORD adwOneRevPos[NMAXMGASTATION];
	WORD wMultiRevCounter[NMAXMGASTATION];
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strNode, strText, strEAE, strHP, strORP, strMRC;
	int nItems=pRootChild->length;
	int n = 0;

	memset (adwOneRevPos, 0, sizeof (adwOneRevPos));
	memset (wMultiRevCounter, 0, sizeof (wMultiRevCounter));
    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "Frequency")   {
			strText = (LPCSTR)pElChild->text;
			int nFrequency = atoi (strText);
			pHWConfigFile->SetSSCNETFrequency (nCanale, nFrequency);	
		}
		else
		if (str == "EnableWarnings")   {
			strText = (LPCSTR)pElChild->text;
			int nEnableWarnings = atoi (strText);
			pHWConfigFile->EnableSSCNETWarnings (nCanale, nEnableWarnings);	
		}
		else
		if (str == "NetworkEnabled")   {
			strText = (LPCSTR)pElChild->text;
			bool bNetworkEnabled = atoi (strText) != 0;
			pHWConfigFile->EnableSSCNETNetwork (nCanale, bNetworkEnabled);	
		}
		else
		if (str == "NetworkType")   {
			strText = (LPCSTR)pElChild->text;
			bool bNetworkType = atoi (strText) != 0;
			pHWConfigFile->SetSSCNETNetworkType (nCanale, bNetworkType);	
		}
		else   {
			strText = (LPCSTR)pElChild->text;
			for (int i = 0; i<NMAXMGASTATION; i++)   {
				strEAE.Format ("EnableAbsoluteEncoder_Station%d", i+1);
				if (str == strEAE)   {
					bool bEAE = atoi (strText) != 0;
					pHWConfigFile->GetSSCNETStation (nCanale, i+1).m_bAbsoluteEncoder = bEAE;
					break;
				}
				else   {
					strHP.Format ("HomePosition_Station%d", i+1);
					if (str == strHP)   {
						double fpHP = atof (strText);
						pHWConfigFile->GetSSCNETStation (nCanale, i+1).m_fpHomePosition = fpHP;
						break;
					}
					else   {
						strORP.Format ("OPOneRevPos_Station%d", i+1);
						if (str == strORP)   {
							DWORD dw = (DWORD) atol (strText);
							adwOneRevPos[i] = dw;
							break;
						}
						else   {
							strMRC.Format ("MultiRevCounter_Station%d", i+1);
							if (str == strMRC)   {
								WORD w = (WORD) atol (strText);
								wMultiRevCounter[i] = w;
								break;
							}
						}
					}
				}
			}
		}
		n++;
	}
	for (int i = 0; i<NMAXMGASTATION; i++)
		pHWConfigFile->SetAbsPosition (nCanale, i+1, wMultiRevCounter[i], adwOneRevPos[i]);
	return 0;
}

/*
** ImportHWCanOpenNode :
*/
int CProject::ImportHWCanOpenNode (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nNode, int nCanale)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr   pElChild;
	pRootChild = pEl->childNodes;
	CString str, strNode, strText;
	int nItems=pRootChild->length;
	int n = 0;
    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "NodeGuarding")   {
			strText = (LPCSTR)pElChild->text;
			DWORD dwNodeGuarding = (DWORD) atoi (strText);
			pHWConfigFile->GetCanNode (nCanale, nNode).dwNodeGuarding = dwNodeGuarding;
		}
		else
		if (str == "HeartBeatTime")   {
			strText = (LPCSTR)pElChild->text;
			DWORD dwHeartBeatTime = (DWORD) atoi (strText);
			pHWConfigFile->GetCanNode (nCanale, nNode).dwHeartBeatTime = dwHeartBeatTime;
		}
		else
		if (str == "Virtual")   {
			strText = (LPCSTR)pElChild->text;
			bool bIsVirtual = atoi (strText) != 0;
			pHWConfigFile->GetCanNode (nCanale, nNode).bIsVirtual = bIsVirtual;
		}
		else
		if (str == "Disabled")   {
			strText = (LPCSTR)pElChild->text;
			bool bIsDisabled = atoi (strText) != 0;
			pHWConfigFile->GetCanNode (nCanale, nNode).bIsDisabled = bIsDisabled;
		}
		else
		if (str == "Polling")   {
			strText = (LPCSTR)pElChild->text;
			bool bIsVirtual = atoi (strText) != 0;
			pHWConfigFile->GetCanNode (nCanale, nNode).bIsVirtual = bIsVirtual;
		}
		else   {
			CString strAnalogue;
			for (int i = 0 ; i < 12; i++)   {
				strAnalogue.Format ("AnalogInput_%d", i+1);
				if (str == strAnalogue)   {
					ImportHWCanOpenInputAnalogue (pRootChild->Getitem(n), pHWConfigFile, nNode, nCanale, i);
					break;
				}
			}
			for (int i = 0 ; i < 12; i++)   {
				strAnalogue.Format ("AnalogOutput_%d", i+1);
				if (str == strAnalogue)   {
					ImportHWCanOpenOutputAnalogue (pRootChild->Getitem(n), pHWConfigFile, nNode, nCanale, i);
					break;
				}
			}
		}
		n++;
	}
	return 0;
}

/*
** ImportHWEthercat :
*/
int CProject::ImportHWEthercat (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText, strAxis, strSlave;
	int nItems=pRootChild->length;
	int n = 0;
    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "Master")   {
			strText = (LPCSTR)pElChild->text;
			pHWConfigFile->SetEthercatMasterXML (strText);
		}
		else
		if (str == "Slave")   {
			strText = (LPCSTR)pElChild->text;
			int nSlave = atoi (strText);
			pHWConfigFile->SetEthercatSlave (nSlave);
		}
		else
		if (str == "RequestedState")   {
			strText = (LPCSTR)pElChild->text;
			int nRequestedState = atoi (strText);
			pHWConfigFile->SetRequestedState (nRequestedState);
		}
		else
		if (str == "Frequency")   {
			strText = (LPCSTR)pElChild->text;
			int nFrequency = atoi (strText);
			pHWConfigFile->SetEthercatFrequency (nFrequency);
		}
		else
		if (str == "ContinousMeasuring")   {
			strText = (LPCSTR)pElChild->text;
			int nContinousMeasuring = atoi (strText);
			pHWConfigFile->SetEthercatContinousMeasuring (nContinousMeasuring);
		}
		else
		if (str == "StartDCTiming")   {
			strText = (LPCSTR)pElChild->text;
			__int64 nStartDCTiming = _atoi64 (strText);
			pHWConfigFile->SetEthercatDCStartTime (nStartDCTiming);
		}
		else
		if (str == "HPEDriver")   {
			strText = (LPCSTR)pElChild->text;
			pHWConfigFile->SetHpeDriver (strText);
		}
		else
		if (str == "OffsetDCTiming")   {
			strText = (LPCSTR)pElChild->text;
			int nOffsetDCTiming = atoi (strText);
			pHWConfigFile->SetEthercatDCOffsetTime (nOffsetDCTiming);
		}
		else   {
			bool bFound = false;
			for (int i = 0; i<NMAXAXIS; i++)   {
				strAxis.Format ("Axis_%d", i);
				if (str == strAxis)   {
					ImportHWEthercatAxis (pElChild, pHWConfigFile, i);
					bFound = true;
					break;
				}
			}
			if (!bFound)   {
				for (int i = 0; i<MAX_ETHERCAT_SLAVE; i++)   {
					strSlave.Format ("Slave_%d", i);
					if (str == strSlave)   {
						ImportHWEthercatSlave (pElChild, pHWConfigFile->GetEthercatSlaveInfo (i));
						break;
					}
				}
			}
		}
		n++;
	}
	return 0;
}

/*
** ImportHWCanOpenInputAnalogue :
*/
int CProject::ImportHWCanOpenInputAnalogue (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nNode, int nCanale, int nAnalogue)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText;
	double fpAnalogueInput;
	int nItems=pRootChild->length;
	int n = 0;
    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "LowerLimit")   {
			strText = (LPCSTR)pElChild->text;
			fpAnalogueInput = atof (strText);
			pHWConfigFile->GetCanNode (nCanale, nNode).afpLLIAInf[nAnalogue] = fpAnalogueInput;
		}
		else
		if (str == "UpperLimit")   {
			strText = (LPCSTR)pElChild->text;
			fpAnalogueInput = atof (strText);
			pHWConfigFile->GetCanNode (nCanale, nNode).afpLLIASup[nAnalogue] = fpAnalogueInput;
		}
		n++;
	}
	return 0;
}

/*
** ImportHWCanOpenOutputAnalogue :
*/
int CProject::ImportHWCanOpenOutputAnalogue (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nNode, int nCanale, int nAnalogue)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText;
	double fpAnalogueOutput;
	int nItems=pRootChild->length;
	int n = 0;
    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "LowerLimit")   {
			strText = (LPCSTR)pElChild->text;
			fpAnalogueOutput = atof (strText);
			pHWConfigFile->GetCanNode (nCanale, nNode).afpLLOAInf[nAnalogue] = fpAnalogueOutput;
		}
		else
		if (str == "UpperLimit")   {
			strText = (LPCSTR)pElChild->text;
			fpAnalogueOutput = atof (strText);
			pHWConfigFile->GetCanNode (nCanale, nNode).afpLLOASup[nAnalogue] = fpAnalogueOutput;
		}
		n++;
	}
	return 0;
}

/*
** ImportHWEthercatAxis :
*/
int CProject::ImportHWEthercatAxis (ElementXmlPtr pEl, CAdamoHWConfigFile *pHWConfigFile, int nAsse)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText;
	int nItems=pRootChild->length;
	int n = 0;

    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "InpStatus")
			ImportHWEthercatAxisData (pElChild, pHWConfigFile->GetEthercatAxInfo (nAsse).m_inpStatus);
		else
		if (str == "InpControl")
			ImportHWEthercatAxisData (pElChild, pHWConfigFile->GetEthercatAxInfo (nAsse).m_inpControl);
		else
		if (str == "OutStatus")
			ImportHWEthercatAxisData (pElChild, pHWConfigFile->GetEthercatAxInfo (nAsse).m_outStatus);
		else
		if (str == "OutControl")
			ImportHWEthercatAxisData (pElChild, pHWConfigFile->GetEthercatAxInfo (nAsse).m_outControl);
		n++;
	}
	return 0;
}

/*
** ImportHWEthercatAxisData :
*/
int CProject::ImportHWEthercatAxisData (ElementXmlPtr pEl, stHWEthAxData& HWEthAxData)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText;
	int nItems=pRootChild->length;
	int n = 0;

    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "Configured")   {
			strText = (LPCSTR)pElChild->text;
			bool bConfigured = atoi (strText) != 0;
			HWEthAxData.m_bConfigured = bConfigured;
		}
		else
		if (str == "Type")   {
			strText = (LPCSTR)pElChild->text;
			int nType = atoi (strText);
			HWEthAxData.m_nType = nType;
		}
		else
		if (str == "Offset")   {
			strText = (LPCSTR)pElChild->text;
			int nOffset = atoi (strText);
			HWEthAxData.m_nOffset = nOffset;
		}
		else
		if (str == "Length")   {
			strText = (LPCSTR)pElChild->text;
			int nLength = atoi (strText);
			HWEthAxData.m_nLength = nLength;
		}
		n++;
	}
	return 0;
}

/*
** ImportHWEthercatSlave :
*/
int CProject::ImportHWEthercatSlave (ElementXmlPtr pEl, stHWEthSlave& HWEthSlaveData)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText, strIndex;
	int nItems=pRootChild->length;
	int n = 0;

    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		for (int i = 0; i<4; i++)   {
			strIndex.Format ("Type_%d", i);
			if (str == strIndex)   {
				strText = (LPCSTR)pElChild->text;
				int nType = atoi (strText);
				HWEthSlaveData.nDeviceType[i] = nType;
				break;
			}
			else   {
				for (int q = 0; q<4; q++)   {
					strIndex.Format ("EnableVariables_%d_%d", i, q);
					if (str == strIndex)   {
						char* p;
						strText = (LPCSTR)pElChild->text;
						__int64 nMask = _strtoi64 (strText, &p, 16);
						HWEthSlaveData.EnableVariables[i].m_mask[q] = nMask;
						break;
					}
				}
			}
		}
		for (int i = 0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)   {
			strIndex.Format ("Analogue%d", i);
			if (str == strIndex)   {
				ImportHWEthercatSlaveAnalogue (pElChild, HWEthSlaveData, i);
				break;
			}
		}
		n++;
	}
	return 0;
}

/*
** ImportHWEthercatSlaveAnalogue :
*/
int CProject::ImportHWEthercatSlaveAnalogue (ElementXmlPtr pEl, stHWEthSlave& HWEthSlaveData, int nAnalogue)
{
	NodeListXmlPtr  pRootChild;
	ElementXmlPtr pElChild;
	pRootChild = pEl->childNodes;
	CString str, strText, strIndex;
	int nItems=pRootChild->length;
	int n = 0;

    while (n<nItems)   {
		pElChild = pRootChild->Getitem(n);
		str=(LPCTSTR)pElChild->nodeName;
		if (str == "Input_HWAddress")   {
			strText = (LPCSTR)pElChild->text;
			int nHWAddress = atoi (strText);
			HWEthSlaveData.EthAnalogDevice[nAnalogue].AnalogInput.nHWAddress = nHWAddress;
		}
		else
		if (str == "Input_LimiteInferiore")   {
			strText = (LPCSTR)pElChild->text;
			double fpLimiteInferiore = atof (strText);
			HWEthSlaveData.EthAnalogDevice[nAnalogue].AnalogInput.afpLLIAInf = fpLimiteInferiore;
		}
		else
		if (str == "Input_LimiteSuperiore")   {
			strText = (LPCSTR)pElChild->text;
			double fpLimiteSuperiore = atof (strText);
			HWEthSlaveData.EthAnalogDevice[nAnalogue].AnalogInput.afpLLIASup = fpLimiteSuperiore;
		}
		else
		if (str == "Output_HWAddress")   {
			strText = (LPCSTR)pElChild->text;
			int nHWAddress = atoi (strText);
			HWEthSlaveData.EthAnalogDevice[nAnalogue].AnalogOutput.nHWAddress = nHWAddress;
		}
		else
		if (str == "Output_LimiteInferiore")   {
			strText = (LPCSTR)pElChild->text;
			double fpLimiteInferiore = atof (strText);
			HWEthSlaveData.EthAnalogDevice[nAnalogue].AnalogOutput.afpLLIAInf = fpLimiteInferiore;
		}
		else
		if (str == "Output_LimiteSuperiore")   {
			strText = (LPCSTR)pElChild->text;
			double fpLimiteSuperiore = atof (strText);
			HWEthSlaveData.EthAnalogDevice[nAnalogue].AnalogOutput.afpLLIASup = fpLimiteSuperiore;
		}
		n++;
	}
	return 0;
}

/*
** SaveHWImportedFile :
*/
int CProject::SaveHWImportedFile (CAdamoHWConfigFile *pHWConfigFile, CString strProjectDir, CString strName)
{
	CString strPath = strProjectDir + "\\" + HWCONFIG_FILE;
	if (((CFile*) pHWConfigFile)->Open (GetHWConfigPathName(), CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))   {
		pHWConfigFile->WriteData ();
		((CFile*) pHWConfigFile)->Close ();
	}
	return 0;
}

