// AdamoRSOptions.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSOptions.h"
#include "AdamoRSValues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSOptions dialog


CAdamoRSOptions::CAdamoRSOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoRSOptions::IDD, pParent), m_pOptions (NULL), m_nNumOptions (0)
{
	//{{AFX_DATA_INIT(CAdamoRSOptions)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoRSOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoRSOptions)
	DDX_Control(pDX, IDC_RS_OPTION, m_listOptions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoRSOptions, CDialog)
	//{{AFX_MSG_MAP(CAdamoRSOptions)
	ON_BN_CLICKED(IDC_BTN_RS_AGGIUNGI, OnBtnRsAggiungi)
	ON_BN_CLICKED(IDC_BTN_RS_RIMUOVI, OnBtnRsRimuovi)
	ON_BN_CLICKED(IDC_BTN_RS_SPOSTA_GIU, OnBtnRsSpostaGiu)
	ON_BN_CLICKED(IDC_BTN_RS_SPOSTA_SU, OnBtnRsSpostaSu)
	ON_BN_CLICKED(IDC_BTN_RS_MODIFICA, OnBtnRsModifica)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSOptions message handlers

BOOL CAdamoRSOptions::OnInitDialog() 
{
    CRect rect;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    /* settiamo il font */
    m_listOptions.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    /* inseriamo le colonne */
    m_listOptions.GetWindowRect(&rect);
    /* inseriamo le colonne */
	m_listOptions.InsertColumn(0, LOADSTRING (IDS_ADAMORSOPTIONS_1), LVCFMT_LEFT,
		rect.Width() * 40/100, 0);
	m_listOptions.InsertColumn(1, LOADSTRING (IDS_ADAMORSOPTIONS_2), LVCFMT_LEFT,
		rect.Width() * 30/100, 1);
	m_listOptions.InsertColumn(2, LOADSTRING (IDS_ADAMORSOPTIONS_3), LVCFMT_LEFT,
		rect.Width() * 30/100, 2);
    /* settiamo lo stile full row al listview */
    DWORD dw=m_listOptions.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_listOptions.SetExtendedStyle (dw);
    /* selezioniamo sempre la prima voce */
    m_listOptions.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
    /* riempiamo il listview */
    FillOptions ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnBtnRsAggiungi :
*/
void CAdamoRSOptions::OnBtnRsAggiungi() 
{
	// TODO: Add your control notification handler code here
	CAdamoRSValues rsValue;
    int n;
    
    if (rsValue.DoModal ()==IDOK)   {
        /* prendiamo la riga selezionata */
        if ((n=m_listOptions.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            AddItem (rsValue.m_strOption, rsValue.m_bSelected, rsValue.m_strValue, n+1);
        }
        else
            AddItem (rsValue.m_strOption, rsValue.m_bSelected, rsValue.m_strValue, 0);
    }
}

/*
** AddItem :
*/
int CAdamoRSOptions::AddItem (CString strOption, bool bSelected, CString strValue, int nIndex)
{
    int nNewIndex=m_listOptions.InsertItem (nIndex, strOption);
    m_listOptions.SetItemText (nNewIndex, 1, bSelected ? LOADSTRING (IDS_ADAMORSOPTIONS_4) : LOADSTRING (IDS_ADAMORSOPTIONS_5));
    m_listOptions.SetItemText (nNewIndex, 2, strValue);
    m_listOptions.SetItemState (nNewIndex, LVIS_SELECTED, LVIS_SELECTED);
    m_listOptions.SetFocus ();
    return nNewIndex;
}

/*
** AddItem :
*/
void CAdamoRSOptions::GetItem (CString& strOption, bool& bSelected, CString& strValue, int nIndex)
{
    CString strSelected;

    bSelected=false;
    strOption=m_listOptions.GetItemText (nIndex, 0);
    strSelected=m_listOptions.GetItemText (nIndex, 1);
    strValue=m_listOptions.GetItemText (nIndex, 2);
    if (strSelected==LOADSTRING (IDS_ADAMORSOPTIONS_4))
        bSelected=true;
}

/*
** OnBtnRsRimuovi :
*/
void CAdamoRSOptions::OnBtnRsRimuovi() 
{
    int n;
	// TODO: Add your control notification handler code here
    if ((n=m_listOptions.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
        m_listOptions.DeleteItem (n);
    }
    m_listOptions.SetFocus ();
}

/*
** OnBtnRsSpostaGiu :
*/
void CAdamoRSOptions::OnBtnRsSpostaGiu() 
{
    int n, m, q;
	// TODO: Add your control notification handler code here
    if (m_listOptions.GetItemCount ()>1)   {
        if ((n=m_listOptions.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            m=m_listOptions.GetNextItem (n, LVNI_ALL);
            if (m!=-1)   {
                CString strOption, strValue;
                bool bSelected;
                GetItem (strOption, bSelected, strValue, n);
                m_listOptions.DeleteItem (n);
                q=AddItem (strOption, bSelected, strValue, m);
                m_listOptions.SetItemState (q, LVIS_SELECTED, LVIS_SELECTED);
            }
        }
    }
    m_listOptions.SetFocus ();
}

/*
** OnBtnRsSpostaSu :
*/
void CAdamoRSOptions::OnBtnRsSpostaSu() 
{
    int n, m, q;
	// TODO: Add your control notification handler code here
    if (m_listOptions.GetItemCount ()>1)   {
        if ((n=m_listOptions.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            m=m_listOptions.GetNextItem (n, LVNI_ABOVE);
            if (m!=-1)   {
                CString strOption, strValue;
                bool bSelected;
                GetItem (strOption, bSelected, strValue, n);
                m_listOptions.DeleteItem (n);
                q=AddItem (strOption, bSelected, strValue, m);
                m_listOptions.SetItemState (q, LVIS_SELECTED, LVIS_SELECTED);
            }
        }
    }
    m_listOptions.SetFocus ();
}

void CAdamoRSOptions::OnBtnRsModifica() 
{
	CAdamoRSValues rsValue;
    CString strSelezionato;
    int n;

	// TODO: Add your control notification handler code here
    if ((n=m_listOptions.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
        GetItem (rsValue.m_strOption, rsValue.m_bSelected, rsValue.m_strValue, n);
        if (rsValue.DoModal ()==IDOK)   {
            strSelezionato=rsValue.m_bSelected ? LOADSTRING (IDS_ADAMORSOPTIONS_4) : LOADSTRING (IDS_ADAMORSOPTIONS_5);
            LVITEM lv;
            lv.iItem=n;
            lv.iSubItem=0;
            lv.mask = LVIF_TEXT;
            m_listOptions.GetItem (&lv);
            lv.pszText = rsValue.m_strOption.GetBuffer (strlen (rsValue.m_strOption)+1);
            m_listOptions.SetItem (&lv);
            lv.iItem=n;
            lv.iSubItem=1;
            lv.mask = LVIF_TEXT;
            m_listOptions.GetItem (&lv);
            lv.pszText = strSelezionato.GetBuffer (strlen (strSelezionato));
            m_listOptions.SetItem (&lv);
            lv.iItem=n;
            lv.iSubItem=2;
            lv.mask = LVIF_TEXT;
            m_listOptions.GetItem (&lv);
            lv.pszText = rsValue.m_strValue.GetBuffer (strlen (rsValue.m_strValue)+1);
            m_listOptions.SetItem (&lv);
        }
    }
    m_listOptions.SetFocus ();
}

void CAdamoRSOptions::OnOK() 
{
    CString strOption, strValue;
    bool bSelected;

	// TODO: Add extra validation here
    m_nNumOptions=0;
    int n=m_listOptions.GetNextItem (-1, LVNI_ALL);
    while (n!=-1)   {
        GetItem (strOption, bSelected, strValue, n);
        m_pOptions[m_nNumOptions].m_bSelected=bSelected;
        m_pOptions[m_nNumOptions].strOpzione=strOption;
        m_pOptions[m_nNumOptions].strValue=strValue;
        n=m_listOptions.GetNextItem (n, LVNI_ALL);
        m_nNumOptions++;
    }
	CDialog::OnOK();
}

/*
** FillOptions :
*/
void CAdamoRSOptions::FillOptions ()
{
    for (int i=0; i<m_nNumOptions; i++)   {
        AddItem (m_pOptions[i].strOpzione, m_pOptions[i].m_bSelected, m_pOptions[i].strValue, i);
    }
}
