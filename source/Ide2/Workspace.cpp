// Workspace.cpp: implementation of the CWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "Workspace.h"
#include "MainFrame.h"
#include "TreeViewFiles.h"
#include "ProjectNew.h"
#include "PlantManage.h"
#include "AdamoVersion.h"
#include "AdamoRenameMachine.h"
#include "AdamoCopyPlant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString CWorkspace::m_strPlantPath;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkspace* CWorkspace::m_this;

CWorkspace::CWorkspace():m_pActiveProject(NULL),m_bModified(FALSE), m_nProgramVersion (VERSIONE_2_6)
{
   m_this=this;
}

CWorkspace::~CWorkspace()
{
    m_this=NULL;
}

bool CWorkspace::Load()
{
    CStringList sl;
    CString str;
    int nMachines[NMAXMACHINES], i;
    bool b=false;

    CPlantManage dlgPlant(NULL,m_strPlantPath);
	if (dlgPlant.DoModal()==IDOK)   {
        CString newPlant=m_strPlantPath+"\\"+dlgPlant.GetPlantName();
        CWorkspace* pWs=GETWS ();
	    if (pWs&&newPlant!=pWs->GetWorkspacePathName()||!pWs)   {
            if (pWs&&AfxMessageBox (LOADSTRING (IDS_ADAMOWORKSPACE_1), MB_YESNO|MB_ICONQUESTION)==IDYES||!pWs)   {
                ((CIdeApp*)AfxGetApp ())->CloseCurrentWorkspace ();    
	            SaveModified();
                SaveModConfig();
                int num(0);
                CFile fin;
                CString strPath = m_strPlantPath+"\\"+dlgPlant.GetPlantName()+"\\"+dlgPlant.GetPlantName()+".lwr";
	            if ( !fin.Open(strPath, CFile::modeRead) )
	            {
		            AfxMessageBox(LOADSTRING (IDS_ADAMOWORKSPACE_2));
		            return FALSE;
	            }

	            CArchive ar(&fin, CArchive::load);

                RemoveProject();
                ar >> m_strName;
                ar >> m_strDesc;
                ar >> num;

                for(i=0;i<num;++i)   {
                    ar >> str;
					str=((CMainFrame *)AfxGetMainWnd ())->CorreggiPercorsoAssoluto (m_strPlantPath, str);
                    ar >> nMachines[i];
                    sl.AddTail (str);
                }    
                ar.Close ();
                fin.Close ();

                i=0;

                ((CIdeApp *)AfxGetApp ())->SetCurrentWorkspace (this);
                LoadPlantSettings ();
                for (POSITION pos=sl.GetHeadPosition (); pos; ){
                    str = sl.GetNext (pos);
                    CProject *prj = new CProject();
                    if (b = prj->Load(str, nMachines[i++]) != 0)
						m_objPlant.AddTail(prj);
					else
						break;
                }
				if (num == 0)
					b = true;
				if (b)
					((CMainFrame*)AfxGetMainWnd ())->IHaveDoneIt ();
            }
        }
    }
    return b;
}

BOOL CWorkspace::SaveAs()
{
    CString strPath = m_strPlantPath + "\\" + m_strName + "\\" + m_strName;

	CKSFileDialog fd(FALSE, "lwr", strPath, OFN_PATHMUSTEXIST, LOADSTRING (IDS_ADAMOWORKSPACE_3), AfxGetMainWnd());

	if ( fd.DoModal()!=IDOK )
		return FALSE;

    return Save(fd.GetPathName());
}

BOOL CWorkspace::Save(CString strPathName,BOOL bSaveIsModified)
{
	CFile fout;
	if ( !fout.Open(strPathName, CFile::modeCreate|CFile::modeWrite) )
	{
		AfxMessageBox(LOADSTRING (IDS_ADAMOWORKSPACE_2));
		return FALSE;
	}

	CArchive ar(&fout, CArchive::store);

    ar << m_strName;
    ar << m_strDesc;
    ar << m_objPlant.GetCount();

    m_objPlant.Init();

    while(m_objPlant.More())  {
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        ar << pPrj->GetPathName();
        ar << pPrj->GetModuleNumber ();
    }

    m_bModified = FALSE;
    
    return TRUE;
}

BOOL CWorkspace::Close()
{
	SaveModified();
    m_objPlant.Close ();
    m_pActiveProject=NULL;
	return TRUE;
}

BOOL CWorkspace::Add()
{
    m_pActiveProject = new CProject();

    m_objPlant.AddTail(m_pActiveProject);

    return TRUE;
}

BOOL CWorkspace::Save()
{
    CString strPath = m_strPlantPath + "\\" + m_strName + "\\" + m_strName + ".lwr";

    return Save(strPath);
}

CProject* CWorkspace::GetPtrProject(CString strText)
{
    m_objPlant.Init();

    int start = strText.Find("'",0);
    int count = strText.GetLength() - strText.Find("'",0);

    strText = strText.Mid(start + 1,count-2);

    while(m_objPlant.More()){
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        if ( pPrj->GetName() == strText)
            return pPrj;
    }

    return NULL;
}

CProject* CWorkspace::GetProjectByName (CString strText)
{
    m_objPlant.Init();

    while(m_objPlant.More()){
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        if ( pPrj->GetName() == strText)
            return pPrj;
    }

    return NULL;
}

CProject* CWorkspace::NewProject()
{
    CProject* pPrj;

	CProjectNew pn;
	if ( pn.DoModal()!=IDOK )
		return FALSE;
    pPrj = new CProject();
    if((pPrj)->New(pn,m_strPlantPath+"\\"+m_strName)){
        AddTail(pPrj);
	    SetModified(TRUE);
        pPrj->SetMachineModifiedFlag (true);
        ((CMainFrame*) AfxGetMainWnd ())->NewProject (pPrj);
    }
    else
        _delete (pPrj);
    return pPrj;
}

void CWorkspace::SaveModified()
{
	if ( m_bModified )	{
        CString strPath = m_strPlantPath + "\\" + m_strName + "\\" + m_strName + ".lwr";
		Save(strPath,TRUE);
    }
}

void CWorkspace::SaveModConfig()
{
    m_objPlant.Init();
    while(m_objPlant.More()){
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        pPrj->SaveModConfig();
	}
}

void CWorkspace::RemoveProject()
{
    m_strName = "";
    m_strDesc = "";

	CWorkspaceWnd* pWorkspaceWnd = ((CMainFrame*)AfxGetMainWnd())->GetWorkspaceWnd();
	CTreeViewFiles* pTree = pWorkspaceWnd->GetTreeViewFiles();
    if (pTree)
        pTree->RemoveAll();

    m_objPlant.Close ();

    m_pActiveProject = NULL;
}

void CWorkspace::AddTail(CProject *pPrj)
{
    m_objPlant.AddTail(pPrj);
}

void CWorkspace::SetModified(BOOL bMod)
{
    m_bModified = bMod;
}

BOOL CWorkspace::CreateProjectDir(CString strPath)
{
	if ( GetFileAttributes(strPath)!=FILE_ATTRIBUTE_DIRECTORY )
		return CreateDirectory(strPath, NULL);

	return TRUE;
}

void CWorkspace::DeleteObject(CObject *pObj)
{
	CString strMachinePath, strName;
    m_objPlant.Init();

    if(pObj->IsKindOf(RUNTIME_CLASS(CProject))){
		/* disattiviamo per il momento la macchina attiva */
		m_pActiveProject = NULL;
		/* cancelliamo tutti i files della macchina */
		strName = GetName () + "\\" + ((CProject *) pObj)->GetName ();
		/* cancelliamo il progetto dal plant */
        m_objPlant.DeleteObject(pObj);
		CWorkspace::Delete (strName);
		/* attiviamo un altra macchina se c'e', altrimenti azzeriamo la macchina attiva */
        if(GETWS()->GetActiveProject()==(CProject*)pObj){
            CProject *pPrj = m_objPlant.GetFirst();
            if(pPrj){
                ((CMainFrame*)AfxGetMainWnd())->GetWorkspaceWnd()->
                                        GetTreeViewFiles()->SetItemBold(pPrj->GetName(),TRUE);
                SetActiveProject(pPrj);
                ((CMainFrame*)AfxGetMainWnd())->UpdateActiveProject();
            }
        }
    }else{
        while(m_objPlant.More())
            ((CProject*)m_objPlant.GetNext())->DeleteObject(pObj);
    }

}

void CWorkspace::LoadPlantSettings ()
{
    m_ps.Open ();
    m_ps.ReadPlantSettings ();
    m_ps.Close ();
}

void CWorkspace::Activate ()
{
    /* queste due chiamate erano girate, attenzione se vuoi ancora girarle */
    ActivateMachine (m_ps.GetActiveModule ());
    ((CMainFrame*)AfxGetMainWnd())->OpenTemplate (m_ps.GetMode());
}

int CWorkspace::ActivateMachine (int nModule)
{
    CProject* pPrj;
    bool bFound=false;
    /* cerchiamo tra tutti i progetti con il modulo passato per parametro */
    m_objPlant.Init();
    while(m_objPlant.More())   {
        pPrj = (CProject*)m_objPlant.GetNext();
        if (pPrj->GetModuleNumber ()==nModule)   {
            bFound=true;
            break;
        }
    }
    if (bFound)   {
        if (!(GetActiveProject()&&GetActiveProject()->GetModuleNumber()==nModule))   {
            /* settiamolo come progetto corrente */
            SetActiveProject (pPrj);
            /* mandiamo un messaggio al ConfTreeView */
            CConfTreeView* pView=(CConfTreeView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
            if (pView)
                pView->ActivateMachine (nModule);
            /* messaggio alla docking CTreeViewFiles */
	        CWorkspaceWnd* pWorkspace = ((CMainFrame*)AfxGetMainWnd())->GetWorkspaceWnd();
            if (pWorkspace)   {
	            CTreeViewFiles* pTree = pWorkspace->GetTreeViewFiles();
                if (pTree)
                    pTree->ActivateMachine (nModule);
            }
            /* carichiamo i messaggi della macchina attiva */
            CLangView* pLangView=((CMainFrame*)AfxGetMainWnd ())->GetDockLangView ()->GetView();
            if (pLangView)
                pLangView->ActivateMachine ();
            /* facciamo un giretto per forzare la connessione */
            CAdamoMachine* pMachine=pPrj->GetMachine ();
            if (pMachine)
                pMachine->Run ();
        }
    }
    return 0;
}

bool CWorkspace::ExistModule (int nModule)
{
    m_objPlant.Init();
    while(m_objPlant.More())  {
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        if (pPrj->GetModuleNumber ()==nModule)
            return true;
    }
    return false;
}

bool CWorkspace::Delete (CString strPlant)
{
	CString str;
	bool b = false;
	if (GETWS ())   {
		if (GETWS()->GetName() == strPlant)   {
			((CIdeApp *)AfxGetApp ())->ClosePlant ();
			((CIdeApp *)AfxGetApp ())->OnIdle (-1);
		}
		b = DeletePlant (strPlant);
	}
	else
		b = DeletePlant (strPlant);
	return b;
}

bool CWorkspace::DeletePlant (CString strPlant)
{
	bool b = false;
    strPlant=m_strPlantPath+"\\"+strPlant;
    if (!_access (strPlant, 00))   {
        /* il percorso esiste, cancelliamolo */
        RecursiveDelete (strPlant);
        RemoveDirectory (strPlant);
		b = true;
    }
	return b;
}

void CWorkspace::RecursiveDelete (CString szPath)
{
	CFileFind ff;
	CString path = szPath;
	
	if(path.Right(1) != "\\")
		path += "\\";

	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots() && !ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else if (!ff.IsDots() && ff.IsDirectory())
		{
			path = ff.GetFilePath();
			RecursiveDelete(path);
			RemoveDirectory(path);
		}
	}
}

/*
** SaveDatConfig: salva la configurazione file dat sempre 
*/
void CWorkspace::SaveDatConfig()
{
    m_objPlant.Init();
    while(m_objPlant.More()){
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        pPrj->SaveDataConfig();
	}
   
}

/*
** InitialLoad : esegue il caricamento iniziale dell'impianto corrente
*/
bool CWorkspace::InitialLoad()
{
    CStringList sl;
    CString str;
    int nMachines[NMAXMACHINES];
    int num, i;
    CString strPlantName;
	CFile fin;
    bool b=false;

    if (GetCurrentPlant (strPlantName))   {
        CString strPath = m_strPlantPath+"\\"+strPlantName+"\\"+strPlantName+".lwr";
	    if ( fin.Open(strPath, CFile::modeRead) )	    {
	        CArchive ar(&fin, CArchive::load);

            RemoveProject();
            ar >> m_strName;
            ar >> m_strDesc;
            ar >> num;

            for(i=0;i<num;++i)   {
                ar >> str;
                str=((CMainFrame *)AfxGetMainWnd ())->CorreggiPercorsoAssoluto (m_strPlantPath, str);
                ar >> nMachines[i];
                sl.AddTail (str);
            }    
            ar.Close ();
            fin.Close ();
            i=0;
            LoadPlantSettings ();
            for (POSITION pos=sl.GetHeadPosition (); pos; ){
                str = sl.GetNext (pos);
                CProject *prj = new CProject();
                prj->Load(str, nMachines[i++]);
                m_objPlant.AddTail(prj);
            }
            b=true;
        }
        else
		    AfxMessageBox(LOADSTRING (IDS_ADAMOWORKSPACE_2));
    }
    return b;
}

/*
** GetCurrentPlant :
*/
bool CWorkspace::GetCurrentPlant (CString& str)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    int n=0;
    bool b=false;

    /* vediamo se c'e' un unico direttorio */
    CString strPath = m_strPlantPath, strComp;
    if (strPath.Right (1) != _T ("\\"))
        strPath += _T ("\\");
    strPath += _T ("*.*");
    if ((hFind = ::FindFirstFile (strPath, &fd)) == INVALID_HANDLE_VALUE)
        return false;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            strComp = (LPCTSTR) &fd.cFileName;
            if ((strComp != _T (".")) && (strComp != _T ("..")))
                n++;
        }
    } while (::FindNextFile (hFind, &fd));
    ::FindClose (hFind);
    /* se c'e' piu' di un direttorio vediamo se c'e' la variabile nel registry */
    if (n>1)   {
		str = GETOPT()->GetPathCurrent ();
		if (str != "" && !_access (m_strPlantPath+"\\"+str, 00))
            b = true;
    }
    else   {
        str=strComp;
        b=true;
    }
    return b;
}

/*
** LoadPlanPath :
*/
bool CWorkspace::LoadPlantPath ()
{
	m_strPlantPath = GETOPT()->GetPathPlant ();
    return m_strPlantPath != "";
}

int CWorkspace::ChangeLanguage ()
{
    m_objPlant.Init();
    while(m_objPlant.More()){
        CProject *pPrj = (CProject*)m_objPlant.GetNext();
        pPrj->ChangeLanguage ();
    }
    return 0;
}

void CWorkspace::SavePlantSettings ()
{
    m_ps.Open ();
    m_ps.SetMode(((CMainFrame*)AfxGetMainWnd())->GetTemplate ());
    if (m_pActiveProject)
        m_ps.SetActiveModule (m_pActiveProject->GetModuleNumber ());
    m_ps.SetDataVersion (GetProgramVersion ());
    m_ps.WritePlantSettings ();
    m_ps.Close ();
}

bool CWorkspace::CloseCurrentProject ()
{
	bool b = true;
	if (GETWS ())
		if (AfxMessageBox (LOADSTRING (IDS_CLOSE_CURRENT_TEMPLATE), MB_ICONQUESTION|MB_YESNO) == IDYES)   {
            ((CIdeApp *)AfxGetApp ())->ClosePlant ();
			((CIdeApp *)AfxGetApp ())->OnIdle (-1);
		}
		else
			b = false;
	return b;
}

void CWorkspace::RenameMachine (CString strImpianto, CString strOldMachine, CString strMachine)
{
	/* cambiamo il contenuto del file lwr */
	ChangeLWR (strImpianto, strOldMachine, strMachine);
	/* cambiamo il file .lpr */
	ChangeLPR (strImpianto, strOldMachine, strMachine);
	/* cambiamo il file .cfg */
	ChangeCFG (strImpianto, strOldMachine, strMachine);
	/* cambiamo il file .dat */
	ChangeDAT (strImpianto, strOldMachine, strMachine);
	/* rinominiamo gli ultimi files */
	RenameLastFiles (strImpianto, strOldMachine, strMachine);
}

void CWorkspace::ChangeLWR (CString strImpianto, CString strOldMachine, CString strMachine)
{
	CFile fin, fout;
	CString str, strName, strDesc, astr[NMAXMACHINES];
    int nMachines[NMAXMACHINES], i;
	int num, n;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	/* apriamo il file .lwr */
    CString strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strImpianto + ".lwr";
    if (fin.Open(strPath, CFile::modeRead) )    {
		CArchive ar(&fin, CArchive::load);
		ar >> strName;
		ar >> strDesc;
		ar >> num;
		for (i=0; i<num; i++)   {
			ar >> astr[i];
			ar >> nMachines[i];
			_splitpath( astr[i], drive, dir, fname, ext );
			if (CString (fname).MakeUpper () == strOldMachine.MakeUpper ())   {
				n = i; break;
			}
		}    
		ar.Close ();
		fin.Close ();
	    str = m_strPlantPath + "\\" + strImpianto + "\\" + strMachine + "\\" + strMachine + ".lpr";
		if (fout.Open (strPath, CFile::modeCreate | CFile::modeWrite))    {
			CArchive ar(&fout, CArchive::store);
			ar << strName;
			ar << strDesc;
			ar << num;
			for (i=0; i<num; i++)   {
				if (i == n)	
					ar << str;
				else
					ar << astr[i];
				ar << nMachines[i];
			}
			ar.Close ();
			fout.Close ();
		}
	}
}

bool CWorkspace::Copy (CString strPlant, CString &strName, CString &strDescription)
{
	CAdamoCopyPlant cp;
	CStringArray astrMachine;
	CString strMachine, strNewPlant, strNewMachine;
	bool b = false;

	/* chiudiamo il template corrente */
	if (CloseCurrentProject ())   {
		if (GetMachineName (strPlant, astrMachine))   {
			strMachine = astrMachine[0];
			cp.SetPlant (strPlant);
			cp.SetMachine (strMachine);
			if (cp.DoModal () == IDOK)   {
				strNewPlant = cp.GetPlant ();
				strNewMachine = cp.GetMachine ();
				if (CopyPlant (strPlant, strMachine, strNewPlant, strNewMachine))   {
					strName = strNewPlant;
					strDescription = GetPlantDescription (strNewPlant);
					b = true;
				}
			}
		}
	}
	return b;
}

void CWorkspace::Rename (CString strPlant)
{
	CAdamoRenameMachine rm;
	CStringArray astrMachine;
	CString strActualMachine, strNewMachine;

	/* chiudiamo il template corrente */
	if (CloseCurrentProject ())   {
		/* ora cerchiamo il nome della macchina nel file LWR */
		if (GetMachineName (strPlant, astrMachine))   {
			strActualMachine = astrMachine[0];
			rm.SetActualMachine (strActualMachine);
			/* facciamoci dire il nuovo nome della macchina */
			if (rm.DoModal () == IDOK)   {
				strNewMachine = rm.GetNewMachine ();
				RenameMachine (strPlant, strActualMachine, strNewMachine);
			}
		}
	}
}

bool CWorkspace::GetMachineName (CString strPlant, CStringArray &astrMachine)
{
	CFile fin;
	CString str, strName, strDesc;
    int nMachines[NMAXMACHINES], i;
	int num;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	bool b = false;

	/* apriamo il file .lwr */
    CString strPath = m_strPlantPath + "\\" + strPlant + "\\" + strPlant + ".lwr";
	astrMachine.SetSize (NMAXMACHINES);
    if (fin.Open(strPath, CFile::modeRead) )    {
		CArchive ar(&fin, CArchive::load);
		ar >> strName;
		ar >> strDesc;
		ar >> num;
		for (i=0; i<num; i++)   {
			ar >> str;
			ar >> nMachines[i];
			_splitpath( str, drive, dir, fname, ext );
			astrMachine[i] = CString (fname);
			b = true;
		}
		ar.Close ();
		fin.Close ();
	}
	return b;
}

void CWorkspace::ChangeLPR (CString strImpianto, CString strOldMachine, CString strMachine)
{
	CFile f;
	CStringArray astrFiles;
	CString str, strPath, strPathName, strOutputDir, strIntermediateDir, strOutputPrefix, strMain, strNewPath;
	BOOL bGenerateListing;
	long nFiles, nMinBreakPoint, nMaxBreakPoint, nBreakPoints;

    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + ".lpr";
	if (f.Open(strPath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))    {
	    CArchive ar(&f, CArchive::load);
		/* carichiamo le opzioni del progetto */
		ar >> strPathName;
		ar >> strOutputDir;
		ar >> strIntermediateDir;
		ar >> strOutputPrefix;
		strOutputPrefix = strMachine;
		ar >> bGenerateListing;
		ar >> nFiles;
		astrFiles.SetSize (nFiles);
		/* opzioni e configurazione dei singoli files */
		for ( int i=0; i<nFiles; ++i )
		{
			ar >> astrFiles[i];
			str = astrFiles[i]; strMain = ".\\" + strOldMachine + ".ada";
			if (str.MakeUpper () == strMain.MakeUpper ())
				astrFiles[i] = ".\\" + strMachine + ".ada";
			ar >> nMinBreakPoint;
			ar >> nMaxBreakPoint;
			ar >> nBreakPoints;

			for ( int i=0; i<nBreakPoints; ++i )
			{
				int nLine;
				ar >> nLine;
			}
			nMinBreakPoint = 2147483647;
			nMaxBreakPoint = nBreakPoints = 0;
		}
        ar.Close ();
        f.Close ();
		strPathName = m_strPlantPath + "\\" + strImpianto + "\\" + strMachine + "\\" + strMachine + ".lpr";
		if (f.Open(strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )   {
			/* scriviamo i dati */
			CArchive ar(&f, CArchive::store);
			ar << strPathName;
			ar << strOutputDir;
			ar << strIntermediateDir;
			ar << strOutputPrefix;
			ar << bGenerateListing;
			ar << nFiles;
			for ( int i=0; i < nFiles; ++i )   {
				ar << astrFiles[i];
				ar << nMinBreakPoint;
				ar << nMaxBreakPoint;
				ar << nBreakPoints;
			}
			ar.Close ();
			f.Close ();
			/* ora rinominiamo l'lpr */
			strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strMachine + ".lpr";
			CFile::Rename (strPath, strNewPath);
		}
	}
}

void CWorkspace::ChangeCFG (CString strImpianto, CString strOldMachine, CString strMachine)
{
    CAdamoCfgMachine* pMachine;
	CFile f;
	CString strPath, strNewPath;

    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + ".cfg";
	if (f.Open(strPath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))   {
		/* leggiamo la macchina */
		CArchive ar(&f, CArchive::load);
		pMachine = new CAdamoCfgMachine;
		pMachine->Serialize(ar);
		/* cambiamo il nome al primo gruppo */
		CAdamoLogicModule *pModule = pMachine->GetAdamoModule ();
		if (pModule)
			pModule->SetName (strMachine);
		/* chiudiamo i files */
		ar.Close ();
		f.Close ();
		/* ora salviamo i nuovi dati */
		if (f.Open(strPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )   {
			CArchive ar(&f, CArchive::store);
			pMachine->Serialize(ar);	
			ar.Close ();
			f.Close ();
			/* ora rinominiamo il file .cfg */
			strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strMachine + ".cfg";
			CFile::Rename (strPath, strNewPath);
		}		
		delete pMachine;
	}
}

void CWorkspace::ChangeDAT (CString strImpianto, CString strOldMachine, CString strMachine)
{
	CFile f;
	CString strPath;

	strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + "device.dat";
	if (f.Open(strPath, CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone))   {
		stAdamoParametri *pAdamoParametri = new stAdamoParametri;
        f.Read (pAdamoParametri, sizeof (stAdamoParametri));
		ChangeDATRecords (pAdamoParametri, strOldMachine, strMachine);
		f.SeekToBegin ();
		f.Write (pAdamoParametri, sizeof (stAdamoParametri));
		delete pAdamoParametri;
		f.Close ();
	}
}

void CWorkspace::RenameLastFiles (CString strImpianto, CString strOldMachine, CString strMachine)
{
	CString strPath, strNewPath;

	/* rinominiamo l'MDB */
    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + ".mdb";
    strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strMachine + ".mdb";
	if (!_access (strPath, 00))
		CFile::Rename (strPath, strNewPath);
	/* rinominiamo il file ada principale */
    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + ".ada";
    strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strMachine + ".ada";
	if (!_access (strPath, 00))
		CFile::Rename (strPath, strNewPath);
	/* rinominiamo il file xml */
    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + ".xml";
    strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strMachine + ".xml";
	if (!_access (strPath, 00))
		CFile::Rename (strPath, strNewPath);
	/* rinominiamo il file efg */
    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + ".efg";
    strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strMachine + ".efg";
	if (!_access (strPath, 00))
		CFile::Rename (strPath, strNewPath);
	/* cancelliamo il file xml di back up della macchina */
    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine + "\\" + strOldMachine + "_config.xml";
	if (!_access (strPath, 00))
		CFile::Remove (strPath);
	/* cambiamo ora il direttorio della macchina */
    strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strOldMachine;
    strNewPath = m_strPlantPath + "\\" + strImpianto + "\\" + strMachine;
	CFile::Rename (strPath, strNewPath);
}

bool CWorkspace::XCopy(LPCTSTR lpSource, LPCTSTR lpDestination)
{
	TCHAR szSource[MAX_PATH + 2] = _T("");
	TCHAR szDestination[MAX_PATH + 2] = _T("");
	::_tcsncpy(szSource, lpSource, MAX_PATH);
	::_tcsncpy(szDestination, lpDestination, MAX_PATH);

	SHFILEOPSTRUCT fs;
	::memset(&fs, 0, sizeof(SHFILEOPSTRUCT));

	fs.pFrom = szSource;
	fs.pTo = szDestination;
	fs.wFunc = FO_COPY;
	fs.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);

	return ::SHFileOperation(&fs) == 0;
}

bool CWorkspace::CopyPlant (CString strPlant, CString strMachine, CString strNewPlant, CString strNewMachine)
{
	/* verifichiamo che il nuovo impianto non esista gia */
    CString strPathFrom, strPathTo;
	bool b = false;

	strPathFrom = m_strPlantPath + "\\" + strPlant;
	strPathTo = m_strPlantPath + "\\" + strNewPlant;
	if (_access (strPathTo, 00))   {
		/* ok, non esiste, copiamo il file */
		if (XCopy (strPathFrom, strPathTo))
			/* rimominiamo il file lpw */
			NewLWR (strPlant, strMachine, strNewPlant, strNewMachine);
			/* aggiorniamo il file lpr */
			ChangeLPR (strNewPlant, strMachine, strNewMachine);
			if (strMachine != strNewMachine)
				/* rinominiamo la macchina se dobbiamo rinominarla */
				RenameMachine (strNewPlant, strMachine, strNewMachine);
			b = true;
	}
	else
		AfxMessageBox (LOADSTRING (IDS_ADAMOWORKSPACE_4));
	return b;
}

void CWorkspace::NewLWR (CString strImpianto, CString strMachine, CString strNewImpianto, CString strNewMachine)
{
	CFile fin, fout;
	CString str, strName, strDesc, astr[NMAXMACHINES];
    int nMachines[NMAXMACHINES], i;
	int num;

	/* apriamo il file .lwr */
    CString strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strImpianto + ".lwr";
    CString strNewPath = m_strPlantPath + "\\" + strNewImpianto + "\\" + strNewImpianto + ".lwr";
    CString strOldPath = m_strPlantPath + "\\" + strNewImpianto + "\\" + strImpianto + ".lwr";
	CString strMachinePath = m_strPlantPath + "\\" + strImpianto + "\\" + strMachine + "\\" + strMachine + ".lpr";
	CString strNewMachinePath = m_strPlantPath + "\\" + strNewImpianto + "\\" + strNewMachine + "\\" + strNewMachine + ".lpr";
    if (fin.Open(strPath, CFile::modeRead) )    {
		CArchive ar(&fin, CArchive::load);
		ar >> strName;
		ar >> strDesc;
		ar >> num;
		for (i=0; i<num; i++)   {
			ar >> astr[i];
			if (astr[i].MakeUpper () == strMachinePath.MakeUpper ())
				astr[i] = strNewMachinePath;
			ar >> nMachines[i];
		}
		ar.Close ();
		fin.Close ();
		if (fout.Open (strNewPath, CFile::modeCreate | CFile::modeWrite))    {
			CArchive ar(&fout, CArchive::store);
			ar << strNewImpianto;
			ar << strDesc;
			ar << num;
			for (i=0; i<num; i++)   {
				ar << astr[i];
				ar << nMachines[i];
			}
			ar.Close ();
			fout.Close ();
		}
		/* cancelliamo il vecchio file lwr */
		CFile::Remove (strOldPath);
	}
}

CString CWorkspace::GetPlantDescription (CString strImpianto)
{
	CFile f;
	CString strName, strDescription;

    CString strPath = m_strPlantPath + "\\" + strImpianto + "\\" + strImpianto + ".lwr";
    if (f.Open (strPath, CFile::modeRead) )    {
		CArchive ar(&f, CArchive::load);
		ar >> strName;
		ar >> strDescription;
		ar.Close ();
		f.Close ();
	}
	return strDescription;
}

void CWorkspace::ChangeDATRecords (stAdamoParametri *pAdamoParametri, CString strOldMachine, CString strMachine)
{
	int i;

    for (i=0; i<NMAXINPDIG; i++)
        if (pAdamoParametri->AdamoInputDig[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoInputDig[i].szNome, strOldMachine, strMachine);
    /* output digitali */
    for (i=0; i<NMAXOUTDIG; i++)
        if (pAdamoParametri->AdamoOutputDig[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoOutputDig[i].szNome, strOldMachine, strMachine);
    /* input analogici */
    for (i=0; i<NMAXINPANA; i++)
        if (pAdamoParametri->AdamoInputAnag[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoInputAnag[i].szNome, strOldMachine, strMachine);
    /* output analogici */
    for (i=0; i<NMAXOUTANA; i++)
        if (pAdamoParametri->AdamoOutputAnag[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoOutputAnag[i].szNome, strOldMachine, strMachine);
    /* contatori */
    for (i=0; i<NMAXCOUNTER; i++)
        if (pAdamoParametri->AdamoCounter[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoCounter[i].szNome, strOldMachine, strMachine);
    /* timer */
    for (i=0; i<NMAXTIMER; i++)
        if (pAdamoParametri->AdamoTimer[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoTimer[i].szNome, strOldMachine, strMachine);
    /* generic can device */
    for (i=0; i<NMAXGENERICCAN; i++)
        if (pAdamoParametri->AdamoCanDevice[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoCanDevice[i].szNome, strOldMachine, strMachine);
    /* axes */
    for (i=0; i<NMAXAXIS; i++)
        if (pAdamoParametri->AdamoAsse[i].szNome[0]!='\0')
			ChangeDATName (pAdamoParametri->AdamoAsse[i].szNome, strOldMachine, strMachine);
}

void CWorkspace::ChangeDATName (char *szName, const char* szOldMachine, const char* szMachine)
{
	char szBuffer[MAXPATH];
	char *p = strchr (szName, '.');
	if (p)  {
		*p++ = '\0';
		if (!strcmp (szName, szOldMachine))   {
			sprintf (szBuffer, "%s.%s", szMachine, p);
			strcpy (szName, szBuffer);
		}
	}
}

