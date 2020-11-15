// AdamoRSEditGrid.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoRSEditGrid.h"
#include "AdamoRSColumns.h"

// CAdamoRSEditGrid dialog

IMPLEMENT_DYNAMIC(CAdamoRSEditGrid, CDialog)

CAdamoRSEditGrid::CAdamoRSEditGrid(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoRSEditGrid::IDD, pParent), m_nRows (0), m_nCols (0)
{
	for (int i = 0; i < MAXCOLUMNS; i++)
		m_gcd[i].m_nType = LUA_TNUMBER;
}

CAdamoRSEditGrid::~CAdamoRSEditGrid()
{
}

void CAdamoRSEditGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RS_LISTCOL, m_listColumns);
}


BEGIN_MESSAGE_MAP(CAdamoRSEditGrid, CDialog)
	ON_BN_CLICKED(IDC_BTN_RS_AGGIUNGI, &CAdamoRSEditGrid::OnBnClickedBtnRsAggiungi)
	ON_BN_CLICKED(IDC_BTN_RS_MODIFICA, &CAdamoRSEditGrid::OnBnClickedBtnRsModifica)
	ON_BN_CLICKED(IDC_BTN_RS_RIMUOVI, &CAdamoRSEditGrid::OnBnClickedBtnRsRimuovi)
	ON_BN_CLICKED(IDC_BTN_RS_SPOSTA_SU, &CAdamoRSEditGrid::OnBnClickedBtnRsSpostaSu)
	ON_BN_CLICKED(IDC_BTN_RS_SPOSTA_GIU, &CAdamoRSEditGrid::OnBnClickedBtnRsSpostaGiu)
END_MESSAGE_MAP()

void CAdamoRSEditGrid::SetGridData (int nRows, int nColumns)
{
	m_nRows = nRows;
	m_nCols = nColumns;
}

void CAdamoRSEditGrid::SetColumnData (int nColumn, stGridColumnData *pgcd)
{
	m_gcd[nColumn].m_nType = pgcd->m_nType;
	m_gcd[nColumn].m_strName = pgcd->m_strName;
	m_gcd[nColumn].m_strImage = pgcd->m_strImage;
}

void CAdamoRSEditGrid::GetGridData (int *pnRow, int *pnColumns)
{
	*pnRow = m_nRows;
	*pnColumns = m_nCols;
}

void CAdamoRSEditGrid::GetColumnData (int nColumn, stGridColumnData *pgcd)
{
	pgcd->m_nType = m_gcd[nColumn].m_nType;
	pgcd->m_strName = m_gcd[nColumn].m_strName;
	pgcd->m_strImage = m_gcd[nColumn].m_strImage;
}

// CAdamoRSEditGrid message handlers
BOOL CAdamoRSEditGrid::OnInitDialog()
{
    CRect rect;
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	/* numero di righe e colonne */
	m_editRows.SubclassEdit(IDC_RS_RIGHE, this, PES_NUMBERS);
	m_editCols.SubclassEdit(IDC_RS_COLONNE, this, PES_NUMBERS);
    /* inseriamo le colonne */
    m_listColumns.GetWindowRect(&rect);
    /* inseriamo le colonne */
	m_listColumns.InsertColumn(0, LOADSTRING (IDS_ADAMORSEDITGRID_1), LVCFMT_LEFT,
		rect.Width() * 40/100, 0);
	m_listColumns.InsertColumn(1, LOADSTRING (IDS_ADAMORSEDITGRID_2), LVCFMT_LEFT,
		rect.Width() * 30/100, 1);
	m_listColumns.InsertColumn(2, LOADSTRING (IDS_ADAMORSEDITGRID_3), LVCFMT_LEFT,
		rect.Width() * 30/100, 2);
    /* settiamo lo stile full row al listview */
    DWORD dw=m_listColumns.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_listColumns.SetExtendedStyle (dw);
    /* riempiamo il listview */
	FillRows ();
    if (FillColumns ())
		/* selezioniamo sempre la prima voce */
		m_listColumns.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CAdamoRSEditGrid::FillRows ()
{
	CString str;

	str.Format ("%d", m_nRows);
	m_editRows.SetWindowText (str);
	str.Format ("%d", m_nCols);
	m_editCols.SetWindowText (str);
    return 0;
}

int CAdamoRSEditGrid::FillColumns ()
{
	int i;

	for (i=0; i<m_nCols; i++)
		AddItem (i, m_gcd[i].m_strName, m_gcd[i].m_nType, m_gcd[i].m_strImage);
	m_listColumns.SetFocus ();
    return i;
}

int CAdamoRSEditGrid::AddItem (int i, CString strName, int nType, CString strImage)
{
	CString strType;
	int nId, nNewIndex;

	if (i == -1)
		i = m_listColumns.GetItemCount ();
	nNewIndex = m_listColumns.InsertItem (i, strName);
	switch (nType)   {
		case LUA_TNUMBER :
			nId = IDS_ADAMORSEDITGRID_4; break;
		case LUA_TBOOLEAN :
			nId = IDS_ADAMORSEDITGRID_5; break;
		case LUA_TSTRING :
			nId = IDS_ADAMORSEDITGRID_6; break;
	}
	strType = LOADSTRING (nId);
	m_listColumns.SetItemText (nNewIndex, 1, strType);
	m_listColumns.SetItemText (nNewIndex, 2, strImage);
	return nNewIndex;
}

void CAdamoRSEditGrid::GetItem (int nIndex, CString& strName, int& nType, CString& strImage)
{
    CString str;

    strName=m_listColumns.GetItemText (nIndex, 0);
    str=m_listColumns.GetItemText (nIndex, 1);
	if (!strcmp (str, LOADSTRING (IDS_ADAMORSEDITGRID_4)))
		nType = LUA_TNUMBER;
	else
	if (!strcmp (str, LOADSTRING (IDS_ADAMORSEDITGRID_5)))
		nType = LUA_TBOOLEAN;
	else
	if (!strcmp (str, LOADSTRING (IDS_ADAMORSEDITGRID_6)))
		nType = LUA_TSTRING;
    strImage=m_listColumns.GetItemText (nIndex, 2);
}

void CAdamoRSEditGrid::OnBnClickedBtnRsAggiungi()
{
	CAdamoRSColumns rsColumns;
	stGridColumnData cd;
    int n;

    rsColumns.SetImagesList (m_slImages);
    if (rsColumns.DoModal ()==IDOK)   {
		rsColumns.GetColumnsData (&cd);
        /* prendiamo la riga selezionata */
        if ((n=m_listColumns.GetNextItem (-1, LVIS_SELECTED))!=-1)
            AddItem (n + 1, cd.m_strName, cd.m_nType, cd.m_strImage);
		else
            AddItem (-1, cd.m_strName, cd.m_nType, cd.m_strImage);
		m_gcd[m_nCols] = cd;
		m_nCols++;
		UpdateColumnsText ();
    }
}

void CAdamoRSEditGrid::OnBnClickedBtnRsModifica()
{
	CAdamoRSColumns rsColumns;
    int nId, n;

    if ((n=m_listColumns.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
		rsColumns.SetColumnsData (&m_gcd[n]);
		rsColumns.SetImagesList (m_slImages);
        if (rsColumns.DoModal ()==IDOK)   {
			rsColumns.GetColumnsData (&m_gcd[n]);
            LVITEM lv;
            lv.iItem=n;
            lv.iSubItem=0;
            lv.mask = LVIF_TEXT;
            m_listColumns.GetItem (&lv);
            lv.pszText = m_gcd[n].m_strName.GetBuffer ();
            m_listColumns.SetItem (&lv);
            lv.iItem=n;
            lv.iSubItem=1;
            lv.mask = LVIF_TEXT;
            m_listColumns.GetItem (&lv);
			switch (m_gcd[n].m_nType)   {
				case LUA_TNUMBER :
					nId = IDS_ADAMORSEDITGRID_4; break;
				case LUA_TBOOLEAN :
					nId = IDS_ADAMORSEDITGRID_5; break;
				case LUA_TSTRING :
					nId = IDS_ADAMORSEDITGRID_6; break;
			}
			CString strTest = LOADSTRING (nId);
            lv.pszText = strTest.GetBuffer ();
            m_listColumns.SetItem (&lv);
            lv.iItem=n;
            lv.iSubItem=2;
            lv.mask = LVIF_TEXT;
            m_listColumns.GetItem (&lv);
            lv.pszText = m_gcd[n].m_strImage.GetBuffer ();
            m_listColumns.SetItem (&lv);
        }
    }
    m_listColumns.SetFocus ();
}

void CAdamoRSEditGrid::OnBnClickedBtnRsRimuovi()
{
	int i, n;

	if ((n=m_listColumns.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
        m_listColumns.DeleteItem (n);
		for (i = n; i<m_nCols-1; i++)
			m_gcd[i] = m_gcd[i+1];
		m_nCols--;
		UpdateColumnsText ();
	}
    m_listColumns.SetFocus ();
}

void CAdamoRSEditGrid::OnBnClickedBtnRsSpostaSu()
{
    CString strName, strImage;
    int nType;
    int n, m, q;

	if (m_listColumns.GetItemCount ()>1)   {
        if ((n=m_listColumns.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            m=m_listColumns.GetNextItem (n, LVNI_ABOVE);
            if (m!=-1)   {
                GetItem (n, strName, nType, strImage);
                m_listColumns.DeleteItem (n);
                q=AddItem (m, strName, nType, strImage);
                m_listColumns.SetItemState (q, LVIS_SELECTED, LVIS_SELECTED);
            }
        }
    }
    m_listColumns.SetFocus ();
}

void CAdamoRSEditGrid::OnBnClickedBtnRsSpostaGiu()
{
    CString strName, strImage;
    int nType;
    int n, m, q;

	if (m_listColumns.GetItemCount ()>1)   {
        if ((n=m_listColumns.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            m=m_listColumns.GetNextItem (n, LVNI_ALL);
            if (m!=-1)   {
                GetItem (n, strName, nType, strImage);
                m_listColumns.DeleteItem (n);
                q=AddItem (m, strName, nType, strImage);
                m_listColumns.SetItemState (q, LVIS_SELECTED, LVIS_SELECTED);
            }
        }
    }
    m_listColumns.SetFocus ();
}

void CAdamoRSEditGrid::SetImagesList (CStringList *images)
{
	m_slImages = images;
}

void CAdamoRSEditGrid::UpdateColumnsText ()
{
	CString str;

	str.Format ("%d", m_nCols);
	GetDlgItem (IDC_RS_COLONNE)->SetWindowText (str);
}

void CAdamoRSEditGrid::OnOK ()
{
	CString str;

	GetDlgItem (IDC_RS_RIGHE)->GetWindowText (str);
	m_nRows = atoi (str);
	CDialog::OnOK ();
}
