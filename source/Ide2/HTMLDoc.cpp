// HTMLDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoTemplates.h"
#include "HTMLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLDoc

IMPLEMENT_DYNCREATE(CHTMLDoc, CDocument)

CHTMLDoc::CHTMLDoc()
{
}

BOOL CHTMLDoc::OnNewDocument()
{
    SetTitle("Parametric");

    if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CHTMLDoc::~CHTMLDoc()
{
}


BEGIN_MESSAGE_MAP(CHTMLDoc, CDocument)
	//{{AFX_MSG_MAP(CHTMLDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLDoc diagnostics

#ifdef _DEBUG
void CHTMLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHTMLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTMLDoc serialization

void CHTMLDoc::Serialize(CArchive& ar)
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
// CHTMLDoc commands
void CHTMLDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
    CHTMLTemplate* tmpl=(CHTMLTemplate*)GetDocTemplate ();
    if (tmpl)
        tmpl->SetActiveView (NULL);
	CDocument::OnCloseDocument();
}
