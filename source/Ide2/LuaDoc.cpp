// LuaDoc.cpp : implementation of the CLuaDoc class
//

#include "stdafx.h"
#include "ide2.h"

#include "LuaDoc.h"

#include "LuaView.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc

IMPLEMENT_DYNCREATE(CLuaDoc, CDocument)

BEGIN_MESSAGE_MAP(CLuaDoc, CDocument)
	//{{AFX_MSG_MAP(CLuaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc construction/destruction

CLuaDoc::CLuaDoc()
{
	// TODO: add one-time construction code here

}

CLuaDoc::~CLuaDoc()
{
}

BOOL CLuaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLuaDoc serialization

void CLuaDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CLuaEditor* pEditor = GetView()->GetEditor();
		pEditor->Save(ar.GetFile());
	}
	else
	{
		// TODO: add loading code here
		CLuaEditor* pEditor = GetView()->GetEditor();
		pEditor->Load(ar.GetFile());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc diagnostics

#ifdef _DEBUG
void CLuaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLuaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLuaDoc commands

CLuaView* CLuaDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	if (pos != NULL)
		return (CLuaView*)GetNextView(pos); // get first one

	return NULL;
}

BOOL CLuaDoc::IsInProject()
{
	return GetProjectFile()!=NULL;
}

CProjectFile* CLuaDoc::GetProjectFile()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

    if(pFrame->GetProject())
        return pFrame->GetProject()->GetProjectFile(GetPathName());

    return NULL;        
}

BOOL CLuaDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

    CProject* pPrj = pFrame->GetProject();

    if(pPrj){
	    CProjectFile* pPF = pPrj->GetProjectFile(lpszPathName);
	    if ( pPF )
		    pPF->SetBreakPointsIn(GetView()->GetEditor());
    }
    
	return TRUE;
}

BOOL CLuaDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CKSFileDialog dlgFile(FALSE);
	dlgFile.SetDocumentPointer(this);
	dlgFile.SetAppPointer(AfxGetApp());
	dlgFile.SetMyParent(AfxGetMainWnd());
	return dlgFile.DoSave(lpszPathName,bReplace);
}

void CLuaDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
    CEditDebugTemplate* tmpl=(CEditDebugTemplate*) GetDocTemplate ();
    int nDocCount=tmpl->GetDocumentCount ();
    if (nDocCount!=1||((CMainFrame*) AfxGetMainWnd ())->WhatIAmGoingToOpen ()==enConfTemplate||((CMainFrame*) AfxGetMainWnd ())->WhatIAmGoingToDo ()!=eDone)   {
        POSITION pos=GetFirstViewPosition ();
        CLuaView *pView=(CLuaView *)GetNextView (pos), *pPrevView;
        if (pView)   {
            pPrevView=((CMainFrame *)AfxGetMainWnd ())->GetPrevEditView (pView);
            if (pPrevView==pView)
                pPrevView=NULL;
        }
        SaveModified ();
	    CDocument::OnCloseDocument();
        if (nDocCount!=1)   {
            if (pPrevView)
                pPrevView->Activate ();
            else   {
                CDocument *pDoc=tmpl->GetDocument ("");
                if (pDoc)   {
                    CView *pView=tmpl->GetFirstView (pDoc);
                    if (pView)
                        ((CLuaView*)pView)->Activate ();
                }
            }
        }
        else
            tmpl->SetActiveView (NULL);
    }
}

BOOL CLuaDoc::SaveModified()
{
    char szBuffer[256];

	if (!IsModified())
		return TRUE;        // ok to continue
    sprintf (szBuffer, "%s %s ?", (const char*) LOADSTRING (IDS_ADAMOLUADOC_1), (const char *) m_strTitle);
    if (AfxMessageBox (szBuffer, MB_YESNO)==IDYES)
        DoFileSave ();
    return TRUE;
}

BOOL CLuaDoc::DoFileSave()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CProject* pPrj = pFrame->GetProject();
    BOOL b=FALSE;

    if (pPrj)   {
        CString strPathName=GetPathName();
        CString strCompilingFile=pPrj->GetCompilingFile ();
        if (strCompilingFile!=strPathName)
            b=CDocument::DoFileSave();
    }
    return b;
};
