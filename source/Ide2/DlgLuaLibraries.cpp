// DlgLuaLibraries.cpp : file di implementazione
//
#include "stdafx.h"
#include "mainframe.h"
#include "adamolibrary.h"
#include "DlgLuaLibraries.h"
#include "DlgNewLuaLibrary.h"
#include "DlgEditLuaLibrary.h"

IMPLEMENT_DYNAMIC(CAdamoDlgLuaLibraries, CDialog)

CAdamoDlgLuaLibraries::CAdamoDlgLuaLibraries(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDlgLuaLibraries::IDD, pParent)
{

}

CAdamoDlgLuaLibraries::~CAdamoDlgLuaLibraries()
{
}

void CAdamoDlgLuaLibraries::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LIBRERIE, m_listLibraries);
}


BEGIN_MESSAGE_MAP(CAdamoDlgLuaLibraries, CDialog)
	ON_COMMAND(IDC_BTN_NEW_LIBRARY, OnNewLibrary)
	ON_COMMAND(IDC_BTN_EDIT_LIBRARY, OnEditLibrary)
	ON_COMMAND(IDC_BTN_DELETE_LIBRARY, OnDeleteLibrary)
END_MESSAGE_MAP()

BOOL CAdamoDlgLuaLibraries::OnInitDialog()
{
	CDialog::OnInitDialog();
	/* prendiamo la directory delle librerie */
	CString strPathLibrary = CreateLibraryDirectory ();
	/* Inizializziamo il list view */
	InitListView ();
	/* riempiamo il list view */
	FillListView (strPathLibrary);
	return TRUE;
}

CString CAdamoDlgLuaLibraries::CreateLibraryDirectory ()
{
	/* prendiamo la directory delle librerie */
	CString strLibraryPathName = ((CMainFrame*) AfxGetMainWnd ())->GetLibrariesPathName ();
    if (_access (strLibraryPathName, 00))
        CreateDirectory(strLibraryPathName, NULL);
    return strLibraryPathName;
}

void CAdamoDlgLuaLibraries::InitListView ()
{
    CRect rect;

    m_listLibraries.GetClientRect (&rect);
    m_listLibraries.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_listLibraries.InsertColumn(0,LOADSTRING (IDS_LIBRARY_NAME),LVCFMT_LEFT,200);
    m_listLibraries.InsertColumn(1,LOADSTRING (IDS_LIBRARY_DESCRIPTION),LVCFMT_LEFT,rect.Width() - 200);
	SetImageList ();
}

void CAdamoDlgLuaLibraries::SetImageList ()
{
    HDITEM hd;
	m_images.Create(MAKEINTRESOURCE(IDB_BMP_LIBRARIES), 16, 1, RGB(0,128,128));
	m_listLibraries.GetHeaderCtrl ()->SetImageList (&m_images);
    m_listLibraries.GetHeaderCtrl ()->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    m_listLibraries.GetHeaderCtrl ()->SetItem(0, &hd);
    m_listLibraries.GetHeaderCtrl ()->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    m_listLibraries.GetHeaderCtrl ()->SetItem(1, &hd);
}

void CAdamoDlgLuaLibraries::FillListView (CString strPathLib)
{
	CFileFind ff;
    LVITEM lvi;
	int nIndex = 0;
    char szBuffer[256];
	bool bInserted = false;

    strPathLib += "\\*.lib";
    BOOL res = ff.FindFile(strPathLib);
    while(res)   {
	    res = ff.FindNextFile();
		if (!ff.IsDots() && !ff.IsDirectory())   {
            CString strLibFile = ff.GetFilePath(), strDescription;
			int nVersion;
			if (CAdamoLuaLibrary::IsLuaLibraryFile (strLibFile, nVersion, strDescription))   {
				strcpy (szBuffer, strLibFile);
				char *p = strrchr (szBuffer, '\\');
				if (p)   {
					p++;
					char* q = strrchr (p, '.');
					if (q)   {
						*q = '\0';
						lvi.mask = LVIF_TEXT; 
						lvi.iItem = nIndex;
						lvi.iSubItem = 0; 
						lvi.pszText = p;
						lvi.cchTextMax=256;
						m_listLibraries.InsertItem (&lvi);
						lvi.mask = LVIF_TEXT; 
						lvi.iItem = nIndex;
						lvi.iSubItem = 1; 
						lvi.pszText = strDescription.GetBuffer ();
						lvi.cchTextMax=strDescription.GetLength ();
						m_listLibraries.SetItem (&lvi);
						bInserted = true;
					}
				}
			}
		}
    }
	if (bInserted)
		m_listLibraries.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
}

void CAdamoDlgLuaLibraries::OnNewLibrary ()
{
	CAdamoDlgNewLuaLibrary dlg;
	if (dlg.DoModal () == IDOK)   {
		CString strLibraryPathName = ((CMainFrame*) AfxGetMainWnd ())->GetLibrariesPathName ();
		CString strLibName;
		if (dlg.GetName () != "")   {
			strLibName.Format ("%s\\%s.lib", strLibraryPathName, dlg.GetName ());
			if (_access (strLibName, 00))   {
				CAdamoLuaLibrary* pLuaLibrary = new CAdamoLuaLibrary;
				pLuaLibrary->SetDescription (dlg.GetDesc ());
				pLuaLibrary->Save (strLibName);
				AddListViewItem (dlg.GetName (), dlg.GetDesc ());
			}
		}
	}
}

void CAdamoDlgLuaLibraries::OnEditLibrary ()
{
	CAdamoDlgEditLuaLibrary dlg;
	int index = m_listLibraries.GetNextItem(-1, LVNI_SELECTED);
	if (index != -1)   {
		CString strLibraryPathName = ((CMainFrame*) AfxGetMainWnd ())->GetLibrariesPathName ();
		CString strLib = m_listLibraries.GetItemText (index, 0);
		CString strLibName;
		strLibName.Format ("%s\\%s.lib", strLibraryPathName, strLib);
		dlg.SetLibrary (strLibName);
		if (dlg.DoModal () == IDOK)   {
		}
	}
}

void CAdamoDlgLuaLibraries::OnDeleteLibrary ()
{
	int index = m_listLibraries.GetNextItem(-1, LVNI_SELECTED);
	if (index != -1)   {
		CString strLibraryPathName = ((CMainFrame*) AfxGetMainWnd ())->GetLibrariesPathName ();
		CString strLib = m_listLibraries.GetItemText (index, 0);
		CString strLibName;
		strLibName.Format ("%s\\%s.lib", strLibraryPathName, strLib);
		if (!_access (strLibName, 00))   {
			CFile::Remove (strLibName);
			m_listLibraries.DeleteItem (index);
			if (m_listLibraries.GetItemCount () > 0)   {
				if (index > m_listLibraries.GetItemCount () - 1)
					index = m_listLibraries.GetItemCount () - 1;
				m_listLibraries.SetItemState (index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
				m_listLibraries.SetFocus ();
			}
		}
	}
}

void CAdamoDlgLuaLibraries::AddListViewItem (CString strName, CString strDesc)
{
	int nIndex = m_listLibraries.GetItemCount ();
    LVITEM lvi;
	lvi.mask = LVIF_TEXT; 
	lvi.iItem = nIndex;
	lvi.iSubItem = 0; 
	lvi.pszText = strName.GetBuffer ();
	lvi.cchTextMax=strName.GetLength ();
	m_listLibraries.InsertItem (&lvi);
	lvi.mask = LVIF_TEXT; 
	lvi.iItem = nIndex;
	lvi.iSubItem = 1; 
	lvi.pszText = strDesc.GetBuffer ();
	lvi.cchTextMax=strDesc.GetLength ();
	m_listLibraries.SetItem (&lvi);
	m_listLibraries.SetItemState (nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_listLibraries.SetFocus ();
}
