// AdamoSEDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "workspace.h"
#include "mainframe.h"
#include "AdamoSEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char* vettMonths[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEDoc

IMPLEMENT_DYNCREATE(CAdamoSEDoc, CDocument)

CAdamoSEDoc::CAdamoSEDoc() : m_pMachine (NULL), m_nMonth (-1), m_pADF (NULL), m_nRecord (0)
{
}

BOOL CAdamoSEDoc::OnNewDocument()
{
    SetTitle (LOADSTRING (IDS_ADAMOSEDOC_1));

	if (!CDocument::OnNewDocument())
		return FALSE;
	OpenAdamoDatiFile ();
	SetMachine ();
	return TRUE;
}

CAdamoSEDoc::~CAdamoSEDoc()
{
}


BEGIN_MESSAGE_MAP(CAdamoSEDoc, CDocument)
	//{{AFX_MSG_MAP(CAdamoSEDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEDoc diagnostics

#ifdef _DEBUG
void CAdamoSEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdamoSEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEDoc serialization

void CAdamoSEDoc::Serialize(CArchive& ar)
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
// CAdamoSEDoc commands

void CAdamoSEDoc::OnCloseDocument() 
{
	/* settiamo la view nel template a NULL */
    CShowErrorsTemplate* tmpl=(CShowErrorsTemplate*) GetDocTemplate ();
    if (tmpl)
        tmpl->SetActiveView (NULL);
	CloseAdamoDatiFile ();
	CDocument::OnCloseDocument();
}

/*
** OpenLogFile : 
*/
int CAdamoSEDoc::OpenLogFile (int nMonth)
{
	m_nRecord = -1;
	if (m_pMachine)
		m_nRecord = m_pMachine->OpenLogFile (nMonth);
    return m_nRecord;
}

/*
** ReadLogFile :
*/
int CAdamoSEDoc::ReadLogFile (int nDA, int nA, BYTE *pAdamoErrore)
{
	int nBytesLetti = 0;
	if (m_pMachine)
		nBytesLetti = m_pMachine->ReadLogFile (nDA, nA, pAdamoErrore);
	return nBytesLetti;
}

/*
** CloseLogFile : 
*/
void CAdamoSEDoc::CloseLogFile ()
{
	if (m_pMachine)
	m_pMachine->CloseLogFile ();
}

/*
** OpenAdamoDatiFile : 
*/
void CAdamoSEDoc::OpenAdamoDatiFile ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)   {
        if (!m_pADF)   {
            m_pADF=new CAdamoDatiFile;
            if (m_pADF)
                if (m_pADF->Open (pPrj->GetParamsPathName(), TRUE))   {
                    m_pADF->ReadData ();    
                    m_pADF->CloseFile ();
                }
                else
                    _delete (m_pADF);
        }
    }    
}

/*
** CloseAdamoDatiFile : 
*/
void CAdamoSEDoc::CloseAdamoDatiFile ()
{
    _delete (m_pADF);
}

/*
** GetDeviceName :
*/
int CAdamoSEDoc::GetDeviceName (int nDevice, int nTypeDevice, char* szBuffer)
{
    int nRet;
    bool bEraChiuso = false;

    if (!m_pADF)   {
        OpenAdamoDatiFile ();
        bEraChiuso = true;
    }
    nRet = m_pADF->GetDeviceName (nDevice, nTypeDevice, szBuffer);
    if (bEraChiuso)
        CloseAdamoDatiFile ();
    return nRet;
}

/*
** SetMachine :
*/
void CAdamoSEDoc::SetMachine ()
{
    CProject *pActivePrj = GETWS()->GetActiveProject();
    if (pActivePrj)
        m_pMachine=pActivePrj->GetMachine();
}

