// PlantManage.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "mainframe.h"
#include "Workspace.h"
#include "PlantManage.h"
#include "NewPlant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CPlantManage dialog


CPlantManage::CPlantManage(CWnd* pParent /*=NULL*/,CString strPathPlant)
	: CDialog(CPlantManage::IDD, pParent),m_strNamePlant("")
{
	//{{AFX_DATA_INIT(CPlantManage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_strPathPlant = strPathPlant;
}

void CPlantManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlantManage)
	DDX_Control(pDX, IDC_LIST_PLANT, m_ctrListPlant);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlantManage, CDialog)
	//{{AFX_MSG_MAP(CPlantManage)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PLANT, OnDblclkListPlant)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PLANT, OnItemChangedPlantManage)
	ON_WM_DESTROY()
  	ON_BN_CLICKED(IDC_BUTADD, OnAddPlant)
  	ON_BN_CLICKED(IDC_BUTDELETE, OnDeletePlant)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTCOPY, &CPlantManage::OnBnClickedButCopy)
	ON_BN_CLICKED(IDC_BUTRENAME, &CPlantManage::OnBnClickedButRename)
	ON_BN_CLICKED(IDC_BUTMODIFY, &CPlantManage::OnBnClickedButmodify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlantManage message handlers

BOOL CPlantManage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ctrListPlant.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
    
    m_iIcons.Create (IDB_CONF_TREE, 16, 1, RGB(128,0,128));
    m_ctrListPlant.SetImageList (&m_iIcons, LVSIL_SMALL);

    m_ctrListPlant.InsertColumn(0,LOADSTRING (IDS_ADAMOPLANTMANAGE_1),LVCFMT_LEFT,200);	
    m_ctrListPlant.InsertColumn (1, LOADSTRING (IDS_ADAMOPLANTMANAGE_2), LVCFMT_LEFT, 420);
    
    InitListView();
    CheckCurrentPlant ();
    m_ctrListPlant.SetFocus ();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlantManage::InitListView()
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    int n=0;

    CString strPath = m_strPathPlant;
    if (strPath.Right (1) != _T ("\\"))
        strPath += _T ("\\");
    strPath += _T ("*.*");

    if ((hFind = ::FindFirstFile (strPath, &fd)) == INVALID_HANDLE_VALUE)
        return;

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CString strComp = (LPCTSTR) &fd.cFileName;
            if ((strComp != _T (".")) && (strComp != _T ("..")) && strComp != DIR_LIBRARIES) {
                               
                CString strNewPath = m_strPathPlant;
                if (strNewPath.Right (1) != _T ("\\"))
                    strNewPath += _T ("\\");

                strNewPath += (LPCTSTR) &fd.cFileName;

                AddItem (n++, strComp, LoadDesc (strNewPath + "\\" + strComp + ".lwr"));
            }
        }
    } while (::FindNextFile (hFind, &fd));

    ::FindClose (hFind);
    /* selezioniamo il primo elemento del nome di file */
    m_ctrListPlant.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
}

CString CPlantManage::GetPlantName()
{
    return m_strNamePlant;
}

void CPlantManage::OnOK() 
{
    bool bChecked=false;

	int nItem = m_ctrListPlant.GetNextItem(-1, LVNI_SELECTED|LVNI_ALL);
    if ( nItem==-1 )	{
		AfxMessageBox(LOADSTRING (IDS_ADAMOPLANTMANAGE_3));
		return;
	}
    m_strNamePlant = m_ctrListPlant.GetItemText(nItem,0);
    nItem=m_ctrListPlant.GetNextItem (-1, LVNI_ALL);
    while (nItem!=-1)   {
        if (m_ctrListPlant.GetCheck (nItem))   {
            bChecked=true; break;
        }
        nItem=m_ctrListPlant.GetNextItem (nItem, LVNI_ALL);
    }
    if (bChecked)
		GETOPT()->SetPathCurrent (m_ctrListPlant.GetItemText (nItem, 0));
    else
        GETOPT()->SetPathCurrent ("");
	GETOPT()->SaveOptions ();
    CDialog::OnOK();
}

void CPlantManage::OnDblclkListPlant(NMHDR* pNMHDR, LRESULT* pResult) 
{
    OnOK();
	*pResult = 0;
}

int CPlantManage::AddItem(int nIndex, CString strName, CString strDesc)
{
    LVITEM lvi;
    int nItem;
    char szBuffer[256];

    wsprintf (szBuffer, "%s", strName);
    lvi.mask = LVIF_TEXT | LVIF_IMAGE; 
    lvi.iItem = nIndex;
    lvi.iSubItem = 0; 
    lvi.iImage = 0;
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    nItem=m_ctrListPlant.InsertItem (&lvi);

    wsprintf (szBuffer, "%s", strDesc);
    lvi.mask = LVIF_TEXT; 
    lvi.iItem = nItem;
    lvi.iSubItem = 1; 
    lvi.iImage = 0;
    lvi.pszText = szBuffer;
    lvi.cchTextMax=256;
    m_ctrListPlant.SetItem (&lvi);
    return nItem;
}

CString CPlantManage::LoadDesc(CString strPath)
{
    CString strDummy,strDesc;
    CFile fin;

	if (fin.Open(strPath, CFile::modeRead)) 	{
	    CArchive ar(&fin, CArchive::load);
        ar >> strDummy;
        ar >> strDesc;
        ar.Close ();
        fin.Close ();
	}
    return strDesc;
}

void CPlantManage::OnDestroy() 
{
	CDialog::OnDestroy();
}

/*
** OnAddPlant : aggiunge un impianto
*/
void CPlantManage::OnAddPlant ()
{
    CString strName, strDescription;
    /* creiamo il nuovo workspace */
    if (New (strName, strDescription))   {
        /* aggiungiamolo al list view */
		AddItem (strName, strDescription);
    }
}

/*
** OnDeletePlant : la funzione cancella un impianto
*/
void CPlantManage::OnDeletePlant ()
{
	CString str;
	int nItem = m_ctrListPlant.GetNextItem(-1, LVNI_SELECTED|LVNI_ALL), nItemToSelect;
    if (nItem!=-1)   {
        m_strNamePlant = m_ctrListPlant.GetItemText(nItem,0);
		str = LOADSTRING (IDS_ADAMOPLANTMANAGE_4) + " " + m_strNamePlant + "?";
		if (AfxMessageBox (str, MB_YESNO) == IDYES)   {
			if (CWorkspace::Delete (m_strNamePlant))   {
				if (m_ctrListPlant.GetNextItem (nItem, LVNI_BELOW))
					nItemToSelect=nItem;
				else
					nItemToSelect=m_ctrListPlant.GetNextItem (nItem, LVNI_ABOVE);
				m_ctrListPlant.DeleteItem (nItem);
				if (nItemToSelect)
					m_ctrListPlant.SetItemState (nItemToSelect, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
    }
    else
		AfxMessageBox(LOADSTRING (IDS_ADAMOPLANTMANAGE_3));
    m_ctrListPlant.SetFocus ();
}

void CPlantManage::OnItemChangedPlantManage (NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	/* vediamo innanzitutto se e' una notifica per selezionare o per deselezionare */
    if (pNMListView->uNewState==0x2000)   {
        /* e'una notifica per selezionare, deselezioniamo tutti gli altri */
        int nItem=m_ctrListPlant.GetNextItem (-1, LVNI_ALL);
        while (nItem!=-1)   {
            if (nItem!=pNMListView->iItem)
                m_ctrListPlant.SetCheck (nItem, FALSE);
            nItem=m_ctrListPlant.GetNextItem (nItem, LVNI_ALL);
        }
        /* selezioniamo ora l'item corrispondente al check box */
        m_ctrListPlant.SetItemState (pNMListView->iItem, LVIS_SELECTED, LVIS_SELECTED);
    }
    else
        if (pNMListView->uOldState==0x2000)
            m_ctrListPlant.SetItemState (pNMListView->iItem, LVIS_SELECTED, LVIS_SELECTED);
	*pResult = 0;
}

/*
** CheckCurrentPlant :
*/
void CPlantManage::CheckCurrentPlant ()
{
    int  nItem;
    bool bFound=false;
    char szBuffer[256];

	CString strPathCurrent = GETOPT()->GetPathCurrent ();
	strcpy (szBuffer, strPathCurrent);
    if (szBuffer[0] != '\0')   {
        nItem=m_ctrListPlant.GetNextItem (-1, LVNI_ALL);
        while (nItem!=-1)   {
            if (!strcmp (m_ctrListPlant.GetItemText (nItem, 0), szBuffer))   {
                m_ctrListPlant.SetCheck (nItem); bFound=true; break;
            }
            nItem=m_ctrListPlant.GetNextItem (nItem, LVNI_ALL);
        }
        if (!bFound)   {
            nItem=m_ctrListPlant.GetNextItem (-1, LVNI_ALL);
            if (nItem!=-1)
                m_ctrListPlant.SetItemState (nItem, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
    else   {
        nItem=m_ctrListPlant.GetNextItem (-1, LVNI_ALL);
        if (nItem!=-1)
            m_ctrListPlant.SetItemState (nItem, LVIS_SELECTED, LVIS_SELECTED);
    }
}

/*
** New : gestisce l'inserimento di un nuovo impianto
*/
int CPlantManage::New (CString& strName, CString& strDescription)
{
	CNewPlant pn;

	if (pn.DoModal()!=IDOK )
		return FALSE;
    strName=pn.GetName ();
    strDescription=pn.GetDesc ();
    CString strPath (m_strPathPlant+"\\"+strName);

    if (_access (strPath, 00))
        if(!CreateDirectory(strPath, NULL))
            return FALSE;
        else
            return Save(strPath+"\\"+strName+".lwr", strName, strDescription);
    else   {
        AfxMessageBox (LOADSTRING (IDS_ADAMOPLANTMANAGE_5));
        return FALSE;
    }
}

/*
** Save : salva un impianto vuoto appena creato.
*/
BOOL CPlantManage::Save(CString strPathName, CString strName, CString strDescription)
{
	CFile fout;
	if ( !fout.Open(strPathName, CFile::modeCreate|CFile::modeWrite) )
	{
		AfxMessageBox(LOADSTRING (IDS_ADAMOPLANTMANAGE_6));
		return FALSE;
	}

	CArchive ar(&fout, CArchive::store);

    ar << strName;
    ar << strDescription;
    ar << (int)0;

	ar.Close ();
	fout.Close ();
    return TRUE;
}

/*
** ModifyLWR : salva un impianto vuoto appena creato.
*/
bool CPlantManage::ModifyLWR (CString strPathName, CString strName, CString strDescription)
{
	CFile fin, fout;
	CString str, strN, strD, astr[NMAXMACHINES];
    int nMachines[NMAXMACHINES], i;
	int num;
	bool b = false;

    if (fin.Open(strPathName, CFile::modeRead) )    {
		CArchive ar(&fin, CArchive::load);
		ar >> strN;
		ar >> strD;
		ar >> num;
		for (i=0; i<num; i++)   {
			ar >> astr[i];
			ar >> nMachines[i];
		}
		ar.Close ();
		fin.Close ();
		if (fout.Open (strPathName, CFile::modeCreate | CFile::modeWrite))    {
			CArchive ar(&fout, CArchive::store);
			ar << strName;
			ar << strDescription;
			ar << num;
			for (i=0; i<num; i++)   {
				ar << astr[i];
				ar << nMachines[i];
			}
			ar.Close ();
			fout.Close ();
		}
		b = true;
	}
	return b;
}

void CPlantManage::OnBnClickedButRename()
{
	// TODO: Add your control notification handler code here
	int nItem = m_ctrListPlant.GetNextItem(-1, LVNI_SELECTED|LVNI_ALL);
    if (nItem!=-1)   {
        m_strNamePlant = m_ctrListPlant.GetItemText(nItem,0);
		CWorkspace::Rename (m_strNamePlant);
	}
}

void CPlantManage::OnBnClickedButCopy()
{
	CString strName, strDescription;
	// TODO: Add your control notification handler code here
	int nItem = m_ctrListPlant.GetNextItem(-1, LVNI_SELECTED|LVNI_ALL);
    if (nItem!=-1)   {
        m_strNamePlant = m_ctrListPlant.GetItemText(nItem,0);
		if (CWorkspace::Copy (m_strNamePlant, strName, strDescription))
			AddItem (strName, strDescription);
	}
}

void CPlantManage::AddItem (CString strName, CString strDescription)
{
    int nItem=AddItem (m_ctrListPlant.GetItemCount (), strName, strDescription);
    m_ctrListPlant.SetItemState  (nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED); 
    m_ctrListPlant.EnsureVisible (nItem, FALSE);
    m_ctrListPlant.SetFocus ();
}

void CPlantManage::OnBnClickedButmodify()
{
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    CString strName, strDescription;
	int nItem;

	if ((nItem = GetPlantInfo (strName, strDescription)) != -1)
		/* creiamo il nuovo workspace */
		if (Modify (strName, strDescription))
			/* aggiungiamolo al list view */
			ModifyItem (strName, strDescription, nItem);
}

int CPlantManage::GetPlantInfo (CString &strName, CString &strDescription)
{
	int nItem = m_ctrListPlant.GetNextItem(-1, LVNI_SELECTED|LVNI_ALL);
    if (nItem!=-1)   {
        strName = m_ctrListPlant.GetItemText (nItem,0);
		strDescription = m_ctrListPlant.GetItemText (nItem,1);
	}
	return nItem;
}

bool CPlantManage::Modify (CString &strName, CString &strDescription)
{
	CNewPlant pn;
	CString strPath;
	bool b = false;

	pn.SetName (strName);
	pn.SetDesc (strDescription);
	if (pn.DoModal() == IDOK )   {
		strDescription = pn.GetDesc ();
		strPath = m_strPathPlant+"\\"+strName;
		if (!_access (strPath, 00))   {
			strPath += "\\"+strName+".lwr";
			ModifyLWR (strPath, strName, strDescription);
			b = true;
		}
		else
			AfxMessageBox (LOADSTRING (IDS_ADAMOPLANTMANAGE_7));
	}
	return b;
}

void CPlantManage::ModifyItem (CString &strName, CString &strDescription, int nItem)
{	
	m_ctrListPlant.SetItemText (nItem,0, strName);
	m_ctrListPlant.SetItemText (nItem,1, strDescription);
}
