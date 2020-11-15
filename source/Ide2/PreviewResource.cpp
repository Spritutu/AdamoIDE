// PreviewResource.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSDoc.h"
#include "PreviewResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewResource

IMPLEMENT_DYNCREATE(CPreviewResource, CView)

CPreviewResource::CPreviewResource()
{
	//{{AFX_DATA_INIT(CPreviewResource)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPreviewResource::~CPreviewResource()
{
}

void CPreviewResource::DoDataExchange(CDataExchange* pDX)
{
	CView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewResource)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewResource, CView)
	//{{AFX_MSG_MAP(CPreviewResource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPreviewResource drawing

void CPreviewResource::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////
// CPreviewResource diagnostics

#ifdef _DEBUG
void CPreviewResource::AssertValid() const
{
	CView::AssertValid();
}

void CPreviewResource::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPreviewResource message handlers

/*
** LoadPreviewPage : 
*/
int CPreviewResource::LoadPreviewPage (CString strResource, CString strXML)
{
	CRect rc;
	int nR;

	GetClientRect (&rc);
	nR = m_SynContainer.LoadStringXML (strResource, strXML);
	return nR;
}

/*
** OnInitialUpdate :
*/
void CPreviewResource::OnInitialUpdate ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc *pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pDoc)   {
		m_SynContainer.SetParentWnd (this);
		m_SynContainer.SetResourceFile (pDoc->GetRSFile ());
		m_SynContainer.SetMachine (pPrj->GetMachine ());
		AddContainerStrings ();
	}
}

/*
** OnAdamoTimer :
*/
void CPreviewResource::OnAdamoTimer ()
{
	m_SynContainer.OnTimer ();
}

/*
** AddContainerStrings :
*/
void CPreviewResource::AddContainerStrings ()
{
	m_SynContainer.AddStrings (IDS_SYNCONT_0, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_1));
	m_SynContainer.AddStrings (IDS_SYNCONT_1, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_2));
	m_SynContainer.AddStrings (IDS_SYNCONT_2, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_3));
	m_SynContainer.AddStrings (IDS_SYNCONT_3, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_5));
	m_SynContainer.AddStrings (IDS_SYNCONT_4, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_6));
	m_SynContainer.AddStrings (IDS_SYNCONT_5, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_7));
	m_SynContainer.AddStrings (IDS_SYNCONT_6, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_8));
	m_SynContainer.AddStrings (IDS_SYNCONT_7, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_9));
	m_SynContainer.AddStrings (IDS_SYNCONT_8, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_10));
	m_SynContainer.AddStrings (IDS_SYNCONT_9, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_11));
	m_SynContainer.AddStrings (IDS_SYNCONT_10, LOADSTRING (IDS_ADAMOMDCOMMANDBAR_12));
	m_SynContainer.AddStrings (IDS_SYNCONT_11, LOADSTRING (IDS_ADAMOVERO));
	m_SynContainer.AddStrings (IDS_SYNCONT_12, LOADSTRING (IDS_ADAMOFALSO));
}

