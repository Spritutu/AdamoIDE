// DlgLuaLibraries.cpp : file di implementazione
//
#include "stdafx.h"
#include "mainframe.h"
#include "adamolibrary.h"
#include "DlgEditLuaLibrary.h"

IMPLEMENT_DYNAMIC(CAdamoDlgEditLuaLibrary, CDialog)

CAdamoDlgEditLuaLibrary::CAdamoDlgEditLuaLibrary(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDlgEditLuaLibrary::IDD, pParent), m_pLuaLibrary (NULL)
{
	
}

CAdamoDlgEditLuaLibrary::~CAdamoDlgEditLuaLibrary()
{
}

void CAdamoDlgEditLuaLibrary::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECTMODULES, m_listPrjMod);
	DDX_Control(pDX, IDC_LIST_LIBRARYMODULES, m_listLibMod);
	DDX_Control(pDX, IDC_BTN_ADDTOLIBRARY, m_btnAddToLibrary);
	DDX_Control(pDX, IDC_BTN_REMOVEFROMLIBRARY, m_btnRemoveFromLibrary);
	DDX_Control(pDX, IDC_BTN_MODULEUP, m_btnMoveModuleUp);
	DDX_Control(pDX, IDC_BTN_MODULEDOWN, m_btnMoveModuleDown);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CAdamoDlgEditLuaLibrary, CDialog)
	ON_WM_DESTROY()
	ON_COMMAND (IDC_BTN_ADDTOLIBRARY, OnAddToLibrary)
	ON_COMMAND (IDC_BTN_REMOVEFROMLIBRARY, OnRemoveFromLibrary)
	ON_COMMAND (IDC_BTN_MODULEUP, OnMoveUpModule)
	ON_COMMAND (IDC_BTN_MODULEDOWN, OnMoveDownModule)
END_MESSAGE_MAP()

BOOL CAdamoDlgEditLuaLibrary::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (OpenLibraryFile () == 0)   {
		InitCaption ();
		InitButtons ();
		InitListViews ();
		FillListViews ();
	}
	return TRUE;
}

void CAdamoDlgEditLuaLibrary::InitListViews ()
{
    CRect rect;
    HDITEM hd;

    m_listPrjMod.GetClientRect (&rect);
	m_listPrjMod.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_listPrjMod.InsertColumn(0,LOADSTRING (IDS_PROJECT_MODULES),LVCFMT_LEFT, rect.Width ());
    m_listLibMod.GetClientRect (&rect);
    m_listLibMod.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_listLibMod.InsertColumn(0,LOADSTRING (IDS_LIBRARY_MODULES),LVCFMT_LEFT, rect.Width ());
	m_imagesPrjMod.Create(MAKEINTRESOURCE(IDB_CAPPATH), 16, 1, RGB(0,128,128));
	m_listPrjMod.GetHeaderCtrl ()->SetImageList (&m_imagesPrjMod);
    m_listPrjMod.GetHeaderCtrl ()->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    m_listPrjMod.GetHeaderCtrl ()->SetItem(0, &hd);
	m_imagesLibMod.Create(MAKEINTRESOURCE(IDB_CAPLIBRARY), 16, 1, RGB(0,128,128));
	m_listLibMod.GetHeaderCtrl ()->SetImageList (&m_imagesLibMod);
    m_listLibMod.GetHeaderCtrl ()->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    m_listLibMod.GetHeaderCtrl ()->SetItem(0, &hd);
}

void CAdamoDlgEditLuaLibrary::FillListViews ()
{
	FillListViewPrj ();
	FillListViewLib ();
}

void CAdamoDlgEditLuaLibrary::FillListViewPrj ()
{
    LVITEM lvi;
	int i = 0;
	CProject* pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		while (i < pPrj->NofFiles ())   {
			CProjectFile* pFile = pPrj->GetProjectFile (i);
			CString strFile = pFile->GetName ();
			lvi.mask = LVIF_TEXT; 
			lvi.iItem = i;
			lvi.iSubItem = 0; 
			lvi.pszText = strFile.GetBuffer ();
			lvi.cchTextMax=256;
			m_listPrjMod.InsertItem (&lvi);
			strFile.ReleaseBuffer ();
			i++;
		}
	}
	if (i > 0)
		m_listPrjMod.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
}

void CAdamoDlgEditLuaLibrary::FillListViewLib ()
{
	CStringList listModules;
    LVITEM lvi;
	int i = 0;
	CProject* pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		m_pLuaLibrary->GetLibraryModules (listModules);
		for (POSITION pos = listModules.GetHeadPosition (); pos; )   {
			CString strName = listModules.GetNext (pos);
			lvi.mask = LVIF_TEXT; 
			lvi.iItem = i;
			lvi.iSubItem = 0; 
			lvi.pszText = strName.GetBuffer ();
			lvi.cchTextMax=256;
			m_listLibMod.InsertItem (&lvi);
			strName.ReleaseBuffer ();
			i++;
		}
	}
	if (i > 0)
		m_listLibMod.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
}

int CAdamoDlgEditLuaLibrary::OpenLibraryFile ()
{
	m_pLuaLibrary = new CAdamoLuaLibrary;
	return m_pLuaLibrary->Load (m_strLibrary);
}

int CAdamoDlgEditLuaLibrary::CloseLibraryFile ()
{
	return m_pLuaLibrary->Save (m_strLibrary);
}

void CAdamoDlgEditLuaLibrary::OnOK ()
{
	CloseLibraryFile ();
	__super::OnOK ();
}

void CAdamoDlgEditLuaLibrary::OnDestroy() 
{
	_delete (m_pLuaLibrary);
}

void CAdamoDlgEditLuaLibrary::OnAddToLibrary ()
{
	CProject* pPrj = GETWS ()->GetActiveProject ();

	if (pPrj)   {
		/* per prima cosa prendiamo il modulo selezionato */
		int index = m_listPrjMod.GetNextItem(-1, LVNI_SELECTED);
		if (index != -1)   {
			/* formattiamo il path completo */
			CString strModuleName = m_listPrjMod.GetItemText (index, 0);
			CString strModule, strModuleBinary;
			strModule = GetModulePathName (pPrj, strModuleName);
			strModuleBinary = GetBinaryPathName (pPrj, strModuleName);
			/* vediamo che non sia gia stato inserito */
			if (!m_pLuaLibrary->ExistLibraryModule (strModuleName))   {
				/* il aggiungiamo il codice */
				CAdamoLuaLibraryModule *pModule = new CAdamoLuaLibraryModule;
				pModule->SetName (strModuleName);
				pModule->ReadObjCode (strModuleBinary);
				/* aggiungiamo le funzioni e gli oggetti */
				AddFunctions (pPrj, strModule, pModule);
				/* aggiungiamo le funzioni e gli oggetti */
				AddObjects (pPrj, strModule, pModule);
				/* aggiungiamo il modulo alla libreria */
				m_pLuaLibrary->AddLibraryModule (strModuleName, pModule);
				/* aggiungiamo il nodo in fondo al listview */
				if ((index = m_listLibMod.InsertItem (m_listLibMod.GetItemCount (), strModuleName)) != -1)
					m_listLibMod.SetItemState (index, LVIS_SELECTED, LVIS_SELECTED);

			}
		}
	}
}

void CAdamoDlgEditLuaLibrary::OnRemoveFromLibrary ()
{
	CProject* pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		/* per prima cosa prendiamo il modulo selezionato */
		int index = m_listLibMod.GetNextItem(-1, LVNI_SELECTED);
		int nLvIndex = index;
		if (index != -1)   {
			CString strLibModuleName = m_listLibMod.GetItemText (index, 0);
			if (m_pLuaLibrary->ExistLibraryModule (strLibModuleName))   {
				/* rimuoviamo il modulo dalla libreria */
				m_pLuaLibrary->RemoveLibraryModule (strLibModuleName);
				/* ora dal listview */
				if (index == m_listLibMod.GetItemCount ()-1)
					nLvIndex--;
				m_listLibMod.DeleteItem (index);
				if (m_listLibMod.GetItemCount () > 0)
					m_listLibMod.SetItemState (nLvIndex, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
}

CString CAdamoDlgEditLuaLibrary::GetModulePathName (CProject* pPrj, CString strModuleName)
{
	CString strRet;
	int i = 0;

	while (i < pPrj->NofFiles ())   {
		CProjectFile* pFile = pPrj->GetProjectFile (i);
		CString strFile = pFile->GetName ();
		if (strFile == strModuleName)   {
			strRet = pFile->GetPathName ();
			break;
		}
		i++;
	}
	return strRet;
}

void CAdamoDlgEditLuaLibrary::AddFunctions (CProject* pPrj, CString strModule, CAdamoLuaLibraryModule *pModule)
{	
	CFunctionsContainer& fc = pPrj->GetFunctionsContainer ();
	list<CAdamoLibrary *>* pLibraries = fc.GetListLibraries ();
    list<CAdamoLibrary *>::iterator i;
    if (pLibraries->size())   {
        i=pLibraries->begin();
        while (i!=pLibraries->end())   {
			CAdamoLibrary* pLibrary = *i;
			if (pLibrary->GetName () == strModule)   {
				list<CAdamoFunction *>* pFunctions = pLibrary->GetListFunctions ();
				list<CAdamoFunction *>::iterator i;
				i = pFunctions->begin ();
				while (i != pFunctions->end())   {
					CAdamoFunction* pFunction = *i;
					pModule->AddFunction (pFunction);
					i++;
				}
			}
			i++;
		}
	}	
}

void CAdamoDlgEditLuaLibrary::AddObjects (CProject* pPrj, CString strModule, CAdamoLuaLibraryModule *pModule)
{
	CObjectsContainer& oc = pPrj->GetObjectsContainer ();
	list<CObjectsLibrary *>* pLibraries = oc.GetListLibraries ();
    list<CObjectsLibrary *>::iterator i;
    if (pLibraries->size())   {
        i=pLibraries->begin();
        while (i!=pLibraries->end())   {
			CObjectsLibrary* pLibrary = *i;
			if (pLibrary->GetName () == strModule)   {
				list<CAdamoObj *>* pObjects = pLibrary->GetListObjects ();
				list<CAdamoObj *>::iterator i;
				i = pObjects->begin ();
				while (i != pObjects->end())   {
					CAdamoObj* pObj = *i;
					pModule->AddObject (pObj);
					i++;
				}
			}
			i++;
		}
	}	
}

CString CAdamoDlgEditLuaLibrary::GetBinaryPathName (CProject* pPrj, CString strModuleName)
{
	CString str;
	str.Format ("%s\\%s.out", pPrj->GetIntermediateDir (), strModuleName);
	return str;
}

void CAdamoDlgEditLuaLibrary::OnMoveUpModule ()
{
	CString str;
	int nIndex;

	if ((nIndex = m_listLibMod.GetNextItem(-1, LVNI_SELECTED)) != -1)   {
		if (nIndex > 0)   {
			str = m_listLibMod.GetItemText (nIndex, 0);
			m_listLibMod.DeleteItem (nIndex);
			nIndex = m_listLibMod.InsertItem (nIndex - 1, str);
			m_listLibMod.SetItemState (nIndex, LVIS_SELECTED, LVIS_SELECTED);
			m_pLuaLibrary->MoveModuleUp (str);
		}
	}
}

void CAdamoDlgEditLuaLibrary::OnMoveDownModule ()
{
	CString str;
	int nIndex;

	if ((nIndex = m_listLibMod.GetNextItem(-1, LVNI_SELECTED)) != -1)   {
		if (nIndex < m_listLibMod.GetItemCount () - 1)   {
			str = m_listLibMod.GetItemText (nIndex, 0);
			m_listLibMod.DeleteItem (nIndex);
			nIndex = m_listLibMod.InsertItem (nIndex + 1, str);
			m_listLibMod.SetItemState (nIndex, LVIS_SELECTED, LVIS_SELECTED);
			m_pLuaLibrary->MoveModuleDown (str);
		}
	}
}

void CAdamoDlgEditLuaLibrary::InitButtons ()
{
	m_btnAddToLibrary.SetBitmaps (IDB_BMP_ARROW_RIGHT, RGB (255, 255, 255));
	m_btnAddToLibrary.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnAddToLibrary.SetWindowText (LOADSTRING (IDS_EL_INSTALLA_MODULO));
	m_btnRemoveFromLibrary.SetBitmaps (IDB_BMP_ARROW_LEFT, RGB (255, 255, 255));
	m_btnRemoveFromLibrary.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnRemoveFromLibrary.SetWindowText (LOADSTRING (IDS_EL_CANCELLA_MODULO));
	m_btnMoveModuleUp.SetBitmaps (IDB_BMP_ARROW_UP, RGB (255, 255, 255));
	m_btnMoveModuleUp.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnMoveModuleUp.SetWindowText (LOADSTRING (IDS_IL_SPOSTA_SU));
	m_btnMoveModuleDown.SetBitmaps (IDB_BMP_ARROW_DOWN, RGB (255, 255, 255));
	m_btnMoveModuleDown.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnMoveModuleDown.SetWindowText (LOADSTRING (IDS_IL_SPOSTA_GIU));
	m_btnOk.SetWindowText ("Ok");
	m_btnCancel.SetWindowText ("Cancel");
}

void CAdamoDlgEditLuaLibrary::InitCaption ()
{
	SetWindowText (LOADSTRING (IDS_EL_EL));
}
