// AdamoMDDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "adamotemplates.h"
#include "AdamoMDDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDDoc

IMPLEMENT_DYNCREATE(CAdamoMDDoc, CDocument)

CAdamoMDDoc::CAdamoMDDoc() : m_pMachine (NULL)
{
    CProject *pActivePrj = GETWS()->GetActiveProject();
    if (pActivePrj)
        SetMachine (pActivePrj->GetMachine());    
}

BOOL CAdamoMDDoc::OnNewDocument()
{
    SetTitle(LOADSTRING (IDS_ADAMOMDDOC_1));
	if (!CDocument::OnNewDocument())
		return FALSE;
    InitSettings ();
	return TRUE;
}

CAdamoMDDoc::~CAdamoMDDoc()
{
}


BEGIN_MESSAGE_MAP(CAdamoMDDoc, CDocument)
	//{{AFX_MSG_MAP(CAdamoMDDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDDoc diagnostics

#ifdef _DEBUG
void CAdamoMDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdamoMDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDDoc serialization

void CAdamoMDDoc::Serialize(CArchive& ar)
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
// CAdamoMDDoc commands

void CAdamoMDDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
    SaveSettings ();
    CManDiagTemplate* tmpl=(CManDiagTemplate*)GetDocTemplate ();
    if (tmpl)
        tmpl->SetActiveView (NULL);
	CDocument::OnCloseDocument();
}

void CAdamoMDDoc::InitSettings ()
{
    CAdamoMDView* pView=CAdamoMDView::GetMDView();
    if (pView)   {
        CAdamoMDSplittedView* pIOView=pView->GetIOView();
        CAdamoMDAxesView*     pAxesView=pView->GetAxesView();
        stMDSettings* MDS=NULL;
        __try   {
            MDS=new stMDSettings;
            m_pSettings = GETWS()->GetActiveProject()->GetSettings ();
            if (m_pSettings->Open ())   {
                m_pSettings->ReadMDSettings (MDS);
                if (pIOView)
                    pIOView->OnViewSettings(MDS);
                if (pAxesView)
                    pAxesView->OnViewSettings(MDS);
                m_pSettings->Close ();
            }
        }
        __finally    {
            if (MDS)
                delete MDS;
        }
    }
}

void CAdamoMDDoc::SaveSettings ()
{
    CAdamoMDView* pView=CAdamoMDView::GetMDView();
    if (pView)   {
        CAdamoMDSplittedView* pIOView=pView->GetIOView();
        CAdamoMDAxesView*     pAxesView=pView->GetAxesView();
        if (m_pSettings)   {
            stMDSettings* MDS=NULL;
            __try   {
                MDS=new stMDSettings;
                memset (MDS, 0, sizeof (stMDSettings));
                if (m_pSettings->Open ())   {
                    if (pIOView)
                        pIOView->GetViewSettings (MDS);
                    if (pAxesView)
                        pAxesView->GetViewSettings (MDS);
                    m_pSettings->WriteMDSettings (MDS);
                    m_pSettings->Close ();
                }
                m_pSettings=NULL;        
            }
            __finally    {
                if (MDS)
                    delete MDS;
            }
        }
    }
}
