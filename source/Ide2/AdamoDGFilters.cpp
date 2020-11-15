// AdamoDGFilters.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "adamotemplates.h"
#include "paredit.h"
#include "AdamoDGDocument.h"
#include "AdamoDGFilters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGFilters dialog

CAdamoDGFilters::CAdamoDGFilters(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDGFilters::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoDGFilters)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CAdamoDGFilters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoDGFilters)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoDGFilters, CDialog)
	//{{AFX_MSG_MAP(CAdamoDGFilters)
	ON_BN_CLICKED(IDC_BUTTON_RESETTAFILTRI, OnButtonResettaFiltri)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGFilters message handlers

BOOL CAdamoDGFilters::OnInitDialog() 
{
    CAdamoDGDocument* pDoc;
    int nFilterDown, nFilterUp, anFilterTypes[NMAX_FILTERTYPES], i;
    char szBuffer[256];
	CDialog::OnInitDialog();
	
    /* subclassiamo i controlli */
    m_editFilterDown.SubclassEdit (IDC_EDIT_DG_IDMINORE,   this, PES_NUMBERS);
    m_editFilterUp.SubclassEdit   (IDC_EDIT_DG_IDMAGGIORE, this, PES_NUMBERS);
    /* carichiamo i controlli */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enDiagnosticTemplate);
    if (pTmpl)   {
        pDoc=(CAdamoDGDocument*)pTmpl->GetDocument ("");
        if (pDoc)   {
            pDoc->GetFilters (&nFilterDown, &nFilterUp, anFilterTypes);
            if (nFilterDown!=-1&&nFilterUp!=-1)   {
                sprintf (szBuffer, "%d", nFilterDown);
                m_editFilterDown.SetWindowText (szBuffer);
                sprintf (szBuffer, "%d", nFilterUp);
                m_editFilterUp.SetWindowText (szBuffer);
            }
            for (i=0; i<NMAX_FILTERTYPES; i++)
                ((CButton*)GetDlgItem (IDC_CHECK_DOWNLOAD+i))->SetCheck (anFilterTypes[i]);
        }
    }
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnOK :
*/
void CAdamoDGFilters::OnOK()
{
    CString strFilterDown, strFilterUp;
    CAdamoDGDocument* pDoc;
    int anFilterTypes[NMAX_FILTERTYPES];
    int i, nFilterDown=-1, nFilterUp=-1;

    m_editFilterDown.GetWindowText (strFilterDown);
    m_editFilterUp.GetWindowText (strFilterUp);
    if (strFilterDown!="")   {
        nFilterDown=atoi (strFilterDown);
        if (nFilterDown<0||nFilterDown>NMAX_COMMANDS-1)   {
            AfxMessageBox ("Filtro Minore troppo piccolo");
            return;
        }
    }
    if (strFilterUp!="")   {
        nFilterUp=atoi (strFilterUp);
        if (nFilterUp<1||nFilterUp>NMAX_COMMANDS)   {
            AfxMessageBox ("Filtro Maggiore troppo grande");
            return;
        }
    }
    for (i=0; i<NMAX_FILTERTYPES; i++)
        anFilterTypes[i]=((CButton*)GetDlgItem (IDC_CHECK_DOWNLOAD+i))->GetCheck ();
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enDiagnosticTemplate);
    if (pTmpl)   {
        pDoc=(CAdamoDGDocument*)pTmpl->GetDocument ("");
        if (pDoc)
            pDoc->SetFilters (m_tv, nFilterDown, nFilterUp, anFilterTypes);
    }
    CDialog::OnOK ();
}

/*
** OnButtonResettaFiltri
*/
void CAdamoDGFilters::OnButtonResettaFiltri () 
{
    CAdamoDGDocument* pDoc;
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enDiagnosticTemplate);
    int anFilterTypes[NMAX_FILTERTYPES];

    for (int i=0; i<NMAX_FILTERTYPES; i++)
        anFilterTypes[i]=1;
    if (pTmpl)   {
        pDoc=(CAdamoDGDocument*)pTmpl->GetDocument ("");
        if (pDoc)
            pDoc->SetFilters (m_tv, -1, -1, anFilterTypes);
    }
}
