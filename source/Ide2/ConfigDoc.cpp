// ConfigDoc.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "AdamoTemplates.h"
#include "ConfigDoc.h"
#include "AdamoConfHWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDoc

IMPLEMENT_DYNCREATE(CConfigDoc, CDocument)

CConfigDoc::CConfigDoc()
{
}

BOOL CConfigDoc::OnNewDocument()
{
    SetTitle(LOADSTRING (IDS_ADAMOCONFIGDOC_1));
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		CopyHWFiles (pPrj);
		CopyCCFiles (pPrj);
		pPrj = (CProject *) pPlant->GetNext ();
	}
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

CConfigDoc::~CConfigDoc()
{
}


BEGIN_MESSAGE_MAP(CConfigDoc, CDocument)
	//{{AFX_MSG_MAP(CConfigDoc)
	ON_COMMAND(IDM_SAVE_MODE, OnSaveConfiguration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDoc diagnostics

#ifdef _DEBUG
void CConfigDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CConfigDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConfigDoc serialization

void CConfigDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CConfigDoc commands

/*
** OnOpenDocument : apre il documento della configurazione.
*/
BOOL CConfigDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	return TRUE;
}

/*
** Gestisce il salvataggio della configurazione.
*/
void CConfigDoc::OnSaveMode() 
{
	CString str;
	POSITION pos;
    bool bAskToSave;

	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst (pos);
	while (pPrj)   {
		bAskToSave = false;
		if (pPrj)   {
			if (pPrj->IsMachineModified () )	{        
				if (AskToSave(pPrj))   {
					bAskToSave=true;
				}
				else
					pPrj->SaveModConfig();
			}
			if (pPrj->IsHWConfigModified ())
				bAskToSave=true;
			if (bAskToSave)   {
				str.Format ("%s %s %s", LOADSTRING (IDS_ADAMOCONFIGDOC_2), pPrj->GetName(), LOADSTRING (IDS_ADAMOCONFIGDOC_3));
				if (AfxMessageBox (str, MB_YESNO)==IDYES)   {
					if (pPrj->IsMachineModified ())   {
						SaveModConfig (pPrj);
						pPrj->SetMachineModifiedFlag (false);
					}
					if (pPrj->IsHWConfigModified ())   {
						SaveHWConfig ();
						pPrj->ResetHWConfigModified ();
					}
				}
				else   {
					RemoveNewCCFiles (pPrj);
					RestoreHWFiles (pPrj);
					pPrj->ReloadConfiguration ();
					RestoreCCFiles (pPrj);
				}
			}
			pPrj->SetMachineModifiedFlag (false);
		}
		DeleteHWFiles (pPrj);
		DeleteCCFiles (pPrj);
		pPrj = (CProject *) pPlant->GetNext (pos);
	}
}

/*
** OnSaveConfiguration :
*/
void CConfigDoc::OnSaveConfiguration () 
{
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		BeginWaitCursor ();
		if (pPrj->IsMachineModified () )	{
			SaveModConfig (pPrj);
			pPrj->SetMachineModifiedFlag (false);
		}
		if (pPrj->IsHWConfigModified ())   {
			SaveHWConfig ();
			pPrj->ResetHWConfigModified ();
		}
		EndWaitCursor ();
		DeleteHWFiles (pPrj);
		DeleteCCFiles (pPrj);
		CopyHWFiles (pPrj);
		CopyCCFiles (pPrj);
		pPrj = (CProject *) pPlant->GetNext ();
	}
}

/*
** OnCloseDocument : la funzione prima di chiudere il documento chiede se si
**                   desidera salvare.
*/
void CConfigDoc::OnCloseDocument ()
{
	OnSaveMode ();
    CConfTemplate* tmpl=(CConfTemplate*)GetDocTemplate ();
    if (tmpl)
        tmpl->SetActiveView (NULL);
	CDocument::OnCloseDocument();
}

/*
** CopyHWFiles : copia i files DEVICE.DAT e HARDWARE.XML
*/
void CConfigDoc::CopyHWFiles (CProject *pPrj)
{
    if (pPrj)   {
        /* file DEVICE.DAT */
        CPath pathSource=pPrj->GetParamsPathName(), pathDest;
        if (!_access (pathSource, 0))   {
            /* il file esiste, backuppiamolo */
            pathDest=pathSource;
            pathDest.RenameExtension ("bck");
            CopyFile ((const char*)pathSource, (const char*)pathDest, false);
        }
        /* file HARDWARE.DEF */
        pathSource=pPrj->GetHWConfigPathName();
        if (!_access (pathSource, 0))   {
            /* il file esiste, backuppiamolo */
            pathDest=pathSource;
            pathDest.RenameExtension ("bck");
            CopyFile ((const char*)pathSource, (const char*)pathDest, false);
        }
        /* file delle traduzioni */
        pathSource=pPrj->GetLanguagePathName();
        if (!_access (pathSource, 0))   {
            /* il file esiste, backuppiamolo */
            CTraduzioni* pTr=pPrj->GetTraduzioni ();
            bool bWasOpened=false;

            if (pTr->IsOpen ())   {
                pTr->ChiudiFileDiLingua ();
                bWasOpened=true;
            }
            /* il file esiste, backuppiamolo */
            pathDest=pathSource;
            pathDest.RenameExtension ("bck");
            CopyFile ((const char*)pathSource, (const char*)pathDest, false);
            if (bWasOpened)
                pTr->ApriFileDiLingua ((const char*)pathSource, 0);
        }
    }
}

/*
** RestoreHWFiles : ricopia i files bacappati precedentemente.
*/
void CConfigDoc::RestoreHWFiles (CProject *pPrj)
{
    if (pPrj)   {
        CTraduzioni* pTr=pPrj->GetTraduzioni ();
        bool bWasOpened=false;

        CPath pathSourceParamsPathName=pPrj->GetParamsPathName();
        CPath pathSourceHWConfigPathName=pPrj->GetHWConfigPathName();
        CPath pathSourceLanguagePathName=pPrj->GetLanguagePathName();
        CPath pathDestParamsPathName=pPrj->GetParamsPathName();
        CPath pathDestHWConfigPathName=pPrj->GetHWConfigPathName();
        CPath pathDestLanguagePathName=pPrj->GetLanguagePathName();
        pathSourceParamsPathName.RenameExtension ("bck");
        pathSourceHWConfigPathName.RenameExtension ("bck");
        pathSourceLanguagePathName.RenameExtension ("bck");
        if (!_access (pathSourceParamsPathName, 0) && !_access (pathSourceHWConfigPathName, 0) && !_access (pathSourceLanguagePathName, 0))   {
            CopyFile ((const char*)pathSourceParamsPathName, (const char*)pathDestParamsPathName, false);
            CopyFile ((const char*)pathSourceHWConfigPathName, (const char*)pathDestHWConfigPathName, false);
            if (pTr->IsOpen ())   {
                pTr->ChiudiFileDiLingua ();
                bWasOpened=true;
            }
            CopyFile ((const char*)pathSourceLanguagePathName, (const char*)pathDestLanguagePathName, false);
            if (bWasOpened)
                pTr->ApriFileDiLingua (pathSourceLanguagePathName, 0);
        }
    }
}

/*
** DeleteHWFiles : cancella i files bacappati precedentemente.
*/
void CConfigDoc::DeleteHWFiles (CProject *pPrj)
{
    if (pPrj)   {
        CPath pathSourceParamsPathName=pPrj->GetParamsPathName();
        CPath pathSourceHWConfigPathName=pPrj->GetHWConfigPathName();
        CPath pathSourceLanguagePathName=pPrj->GetLanguagePathName();
        pathSourceParamsPathName.RenameExtension ("bck");
        pathSourceHWConfigPathName.RenameExtension ("bck");
        pathSourceLanguagePathName.RenameExtension ("bck");
        DeleteFile (pathSourceParamsPathName);
        DeleteFile (pathSourceHWConfigPathName);
        DeleteFile (pathSourceLanguagePathName);
    }
}

/*
** AskToSave 
*/
bool CConfigDoc::AskToSave (CProject *pPrj)
{
    bool b=true;
    if (pPrj)   {
        if (pPrj->GetMachineModule()->IsLogicModuleEmpty())
            if (pPrj->WasMachineEmpty ())
                b=false;
    }
    return b;
}

/*
** CopyCCFiles :
*/
void CConfigDoc::CopyCCFiles (CProject *pPrj)
{
    CPtrList listAxis;

    if (pPrj)   {
        ((CMainFrame *)AfxGetMainWnd ())->CreateAxesList (listAxis);
        for (POSITION pos=listAxis.GetHeadPosition (); pos; )   {
            CAdamoAxis *pAxis=(CAdamoAxis *) listAxis.GetNext (pos);
            if (pAxis->GetCCName ()!="")   {
                CPath path=pPrj->GetParamsPath ();
                path&=pAxis->GetName ();
                CPath pathDest=pPrj->GetParamsPath ();
                pathDest&=pAxis->GetName ();
                pathDest.RenameExtension ("bcc");
                CopyFile ((const char*)path, (const char*)pathDest, false);
            }
        }
    }
}

/*
** DeleteCCFiles :
*/
void CConfigDoc::DeleteCCFiles (CProject *pPrj)
{
	CFileFind ff;
	
    if (pPrj)   {
        CPath path=pPrj->GetParamsPath ();
        path &= "\\*.bcc";
	    BOOL res = ff.FindFile(path);
	    while(res)   {
		    res = ff.FindNextFile();
            if (!ff.IsDots() && !ff.IsDirectory())
                DeleteFile(ff.GetFilePath());
        }
    }
}

/*
** RemoveNewCCFiles :
*/
void CConfigDoc::RemoveNewCCFiles (CProject *pPrj)
{
    CPtrList listAxis;
    if (pPrj)   {
        ((CMainFrame *)AfxGetMainWnd ())->CreateAxesList (listAxis);
        for (POSITION pos=listAxis.GetHeadPosition (); pos; )   {
            CAdamoAxis *pAxis=(CAdamoAxis *) listAxis.GetNext (pos);
            if (pAxis->GetCCName ()!="")   {
                CPath path=pPrj->GetParamsPath ();
                path&=pAxis->GetName ();
                DeleteFile (path);
            }
        }
    }
}

/*
** RestoreCCFiles :
*/
void CConfigDoc::RestoreCCFiles (CProject *pPrj)
{
    CPtrList listAxis;
    CPath pathSource, pathDest;

	if (pPrj)   {
        ((CMainFrame *)AfxGetMainWnd ())->CreateAxesList (listAxis);
        for (POSITION pos=listAxis.GetHeadPosition (); pos; )   {
            CAdamoAxis *pAxis=(CAdamoAxis *) listAxis.GetNext (pos);
            if (pAxis->GetCCName ()!="")   {
                pathSource=pPrj->GetParamsPath ();
                pathSource&=pAxis->GetName ();
                pathDest=pathSource;
                pathSource.RenameExtension (".bcc");
                if (!_access (pathSource, 0))   {
                    CopyFile (pathSource, pathDest, false);
                    DeleteFile (pathSource);
                }
            }
        }
    }
}

/*
** LoadHWConfig :
*/
/*
void CConfigDoc::LoadHWConfig ()
{
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		pPrj->LoadHWConfig ();
		pPrj = (CProject *) pPlant->GetNext ();
    }
}
*/
/*
** SaveHWConfig :
*/
void CConfigDoc::SaveHWConfig ()
{
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst ();
	FlushData ();
	while (pPrj)   {
		pPrj->SaveHWConfig ();
		pPrj = (CProject *) pPlant->GetNext ();
    }
}

/*
** FlushData :
*/
void CConfigDoc::FlushData ()
{
    for (POSITION pos=GetFirstViewPosition (); pos; )   {
        CView *pView=GetNextView (pos);
        if (pView->IsKindOf (RUNTIME_CLASS (CConfTreeView)))
            ((CConfTreeView *)pView)->FlushData ();
        else
            ((CAdamoConfHWView *)pView)->FlushData ();
    }
}

/*
** SaveModConfig :
*/
void CConfigDoc::SaveModConfig (CProject *pPrj)
{
	FlushData ();
	pPrj->SaveModConfig();
}
