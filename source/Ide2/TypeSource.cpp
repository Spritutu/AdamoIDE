// TypeSource.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "TypeSource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeSource dialog


CTypeSource::CTypeSource(CWnd* pParent /*=NULL*/)
	: CDialog(CTypeSource::IDD, pParent), m_hlviml (NULL), m_bmpSAda (NULL)
{
	//{{AFX_DATA_INIT(CTypeSource)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTypeSource::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTypeSource)
	DDX_Control(pDX, IDC_LVTYPESOURCE, m_lvTypeSource);
    DDX_Control(pDX, IDC_EDITNAMESOURCE, m_editFileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTypeSource, CDialog)
	//{{AFX_MSG_MAP(CTypeSource)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeSource message handlers

BOOL CTypeSource::OnInitDialog() 
{
	CDialog::OnInitDialog();
    /* settiamo lo stile del list view */
    SetLvStyle ();
	/* aggiungiamo per prima cosa l'item al list view */
    AddLvItems ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** SetLvStyle
*/
void CTypeSource::SetLvStyle ()
{
    /* creiamo la colonna per visualizzare i dati, e' comunque invisibile */
    m_lvTypeSource.InsertColumn(0,"",LVCFMT_LEFT,1024);	
    /* settiamo lo stile full row al listview */
    DWORD dw=m_lvTypeSource.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_lvTypeSource.SetExtendedStyle (dw);
    /* ora i bitmap del listview */
    m_hlviml=new CImageList;
    m_bmpSAda=new CBitmap;
    m_bmpSAda->LoadBitmap(IDB_BMPFILE);
    m_hlviml->Create (16, 16, ILC_MASK, 0, 1);
    m_hlviml->Add(m_bmpSAda, RGB(0, 128,128));
    m_lvTypeSource.SetImageList(m_hlviml, LVSIL_SMALL);
}

/*
** AddItems : la funzione aggiunge gli item al list view.
*/
void CTypeSource::AddLvItems ()
{
    LVITEM lv;
    char szBuffer[256];

    memset (&lv, 0, sizeof (LVITEM));
    lv.mask=LVIF_TEXT;// | LVIF_IMAGE;
    lv.iItem=0;
    lv.iSubItem=0;
    lv.iImage=0;
    sprintf (szBuffer, LOADSTRING (IDS_ADAMOTYPESOURCE_1));
    lv.pszText=szBuffer;
    lv.cchTextMax=256;
    m_lvTypeSource.InsertItem (&lv);
    m_lvTypeSource.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
}

/*
** OnDestroy : chiamata sulla distruzione della finestra, distrucce immagine bitmap
**             e ImageList.
*/
void CTypeSource::OnDestroy() 
{
	CDialog::OnDestroy();
    /* cancelliamo la image list */
    delete m_hlviml;
    m_hlviml=NULL;
    /* cancelliamo il bitmap */
    delete m_bmpSAda;
    m_bmpSAda=NULL;
}

/*
** OnOK : memorizza il file inputtato solo se questo non esiste gia'
*/
void CTypeSource::OnOK() 
{
    CString str;
    CString strExtension;
    /* stabiliamo che estensione deve avere il file */
    if (m_lvTypeSource.GetItemState (0, LVIS_SELECTED)==LVIS_SELECTED)
        strExtension=".ada";
    else
        strExtension=".cla";
	/* leggiamo dal edit il nome del file */
	m_editFileName.GetWindowText(str);
    if (str!="")   {
        /* costruiamo il path completo */
        CString strPath=GETWS()->GetActiveProject()->GetProjectDir()+"\\"+str+strExtension;
        /* verifichiamo che il file non esista gia o non abbia altri remolazzi */
        if (_access (strPath, 0))   {
            /* memorizziamo il risultato */
            m_strResult=strPath;
    	    CDialog::OnOK();
        }
        else
            AfxMessageBox (LOADSTRING (IDS_ADAMOTYPESOURCE_2), MB_ICONSTOP);   
    }
}
