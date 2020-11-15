// OpenSource.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "mainframe.h"
#include "Workspace.h"
#include "OpenSource.h"
#include "project.h"
#include "projectfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenSource dialog


COpenSource::COpenSource(CWnd* pParent /*=NULL*/)
	: CDialog(COpenSource::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenSource)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COpenSource::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenSource)
	DDX_Control(pDX, IDC_LWSHOWSOURCE, m_lvShowSource);
	DDX_Control(pDX, IDC_LVTYPESOURCE, m_lvTypeSource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenSource, CDialog)
	//{{AFX_MSG_MAP(COpenSource)
	ON_WM_CREATE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LVTYPESOURCE, OnTSItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_LWSHOWSOURCE, OnDblClkShowSource)
    ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COpenSource::OnInitDialog() 
{
	CDialog::OnInitDialog();
    /* settiamo lo stile del list view */
    SetLvStyle ();
	/* aggiungiamo per prima cosa l'item al list view */
    AddLvItems ();
    /* selezioniamo il primo elemento del tipo di sorgente */
    m_lvTypeSource.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
    /* selezioniamo il primo elemento del nome di file */
    m_lvShowSource.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** SetLvStyle
*/
void COpenSource::SetLvStyle ()
{
    /* creiamo la colonna per visualizzare i dati, e' comunque invisibile */
    m_lvTypeSource.InsertColumn(0,"",LVCFMT_LEFT,1024);	
    m_lvShowSource.InsertColumn(0,"",LVCFMT_LEFT,1024);	
    /* settiamo lo stile full row al listview */
    DWORD dw=m_lvTypeSource.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_lvShowSource.SetExtendedStyle (dw);
    dw=m_lvTypeSource.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_lvTypeSource.SetExtendedStyle (dw);
    /* ora i bitmap del listview */
    m_hlviml=new CImageList;
    m_bmpSAda=new CBitmap;
    m_bmpSAda->LoadBitmap(IDB_BMPFILE);
    m_hlviml->Create (16, 16, ILC_MASK, 0, 1);
    m_hlviml->Add(m_bmpSAda, RGB(0, 128,128));
    m_lvTypeSource.SetImageList(m_hlviml, LVSIL_SMALL);
    m_lvShowSource.SetImageList(m_hlviml, LVSIL_SMALL);
}

/*
** AddItems : la funzione aggiunge gli item al list view.
*/
void COpenSource::AddLvItems ()
{
    LVITEM lv;
    char szBuffer[256];

    memset (&lv, 0, sizeof (LVITEM));
    lv.mask=LVIF_TEXT | LVIF_IMAGE;
    lv.iItem=0;
    lv.iSubItem=0;
    lv.iImage=0;
    sprintf (szBuffer, (const char *)LOADSTRING (IDS_ADAMOOPENSOURCE_1));
    lv.pszText=szBuffer;
    lv.cchTextMax=256;
    m_lvTypeSource.InsertItem (&lv);
}

/*
** OnOK : 
*/
void COpenSource::OnOK ()
{
    CString str;
    CString strExtension;
    int n;
    char szBuffer[256];

	/* leggiamo dal edit il nome del file */
    if ((n=m_lvShowSource.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
        m_lvShowSource.GetItemText (n, 0, szBuffer, 256);
        /* costruiamo il path completo */
        CString strPath=GETWS()->GetActiveProject()->GetProjectDir()+"\\"+CString(szBuffer)+strExtension;
        /* verifichiamo che il file non esista gia o non abbia altri remolazzi */
        if (!_access (strPath, 0))   {
            /* memorizziamo il risultato */
            m_strResult=strPath;
	        CDialog::OnOK();
        }
    }
}

/*
** OnTSItemChanged : notifica che viene mandata quando viene selezionata una
**                   nuova voce nel list-view.
*/
void COpenSource::OnTSItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* nm = (NM_LISTVIEW*)pNMHDR;
    if (nm->iItem>=0 && (nm->uNewState & LVIS_SELECTED)) {
        switch (nm->iItem)   {
            case 0 :
                LoadFiles ("ada");
                break;
        }
    }	
	*pResult = 0;
}

/*
** LoadFiles : carica i files nel list view aventi l'estensione passata per parametro
*/
void COpenSource::LoadFiles (CString strExtension)
{
    int n=0;
    CProject* pPrj=GETWS()->GetActiveProject();
    /* solo se c'e' un progetto attivo */    
    if (pPrj)   {
        CString strFileName;
        /* svuotiamo il list view */
        m_lvShowSource.DeleteAllItems ();
        /* cerchiamo tutti i files con estensione passata per parametro e carcichiamoli */
        CProjectFile* pPrjFile=pPrj->GetProjectFile (n);
        /* carichiamo tutti i files appartenenti al progetto */
        while (pPrjFile)   {
            AddItem (pPrjFile->GetNameExt(), n);
            pPrjFile=pPrj->GetProjectFile (++n);
        }
    }
}

/*
** AddItem : aggiunge un elemento al list view che contiene l'elenco dei file
**           di una certa tipologia.
*/
void COpenSource::AddItem (CString strFileName, int nIndex)
{
    LVITEM lvi;
    char szBuffer[256];
    wsprintf (szBuffer, "%s", strFileName);
    lvi.mask = LVIF_TEXT | LVIF_IMAGE; 
    lvi.iItem = nIndex;
    lvi.iSubItem = 0; 
    lvi.iImage = 0;
    lvi.pszText = szBuffer;
    m_lvShowSource.InsertItem (&lvi);
}

/*
** OnDblClkShowSource : gestione del doppio click.
*/
void COpenSource::OnDblClkShowSource (NMHDR* pNMHDR, LRESULT* pResult) 
{
    OnOK();
}

/*
** OnDestroy :
*/
void COpenSource::OnDestroy() 
{
    _delete (m_hlviml);
    _delete (m_bmpSAda);
}
