/*
** AdamoInstallLibraries.cpp
*/

#include "stdafx.h"
#include "resource.h"
#include "workspace.h"
#include "mainframe.h"
#include "AdamoInstallLibraries.h"
#include "AdamoLibrary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox dialog


CDlgInstallLibraries::CDlgInstallLibraries (CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInstallLibraries::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdamoAboutBox)
	//}}AFX_DATA_INIT
}

void CDlgInstallLibraries ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LIBRARY_LIB, m_ctrlLibrary);
	DDX_Control(pDX, IDC_LIST_PROJECT_LIB, m_ctrlProject);
	DDX_Control(pDX, IDC_BTN_ADD_TO_PROJECT, m_btnAddToProject);
	DDX_Control(pDX, IDC_BTN_REMOVE_FROM_PROJECT, m_btnRemoveFromProject);
	DDX_Control(pDX, IDC_BTN_MOVE_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BTN_MOVE_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//{{AFX_DATA_MAP(CAdamoAboutBox)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInstallLibraries, CDialog)
	//{{AFX_MSG_MAP(CAdamoAboutBox)
	ON_COMMAND(IDC_BTN_ADD_TO_PROJECT, OnAddToProject)
	ON_COMMAND(IDC_BTN_REMOVE_FROM_PROJECT, OnRemoveFromProject)
	ON_COMMAND(IDC_BTN_MOVE_UP, OnMoveUp)
	ON_COMMAND(IDC_BTN_MOVE_DOWN, OnMoveDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox message handlers
BOOL CDlgInstallLibraries::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitButtons ();
	InitCaption ();
	LoadLibraryLib ();
	LoadProjectLib ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInstallLibraries::OnOK ()
{
	CString strPrj;

	for (int i = 0; i<m_ctrlProject.GetCount (); i++)   {
		m_ctrlProject.GetText (i, strPrj);
		m_listLibraries.AddTail (strPrj);
	}
	__super::OnOK ();
}

void CDlgInstallLibraries::LoadLibraryLib ()
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
	bool bAdded = false;

	CString strBasePathName = GETWS ()->GetBasePathName ();
	CString strSearchPath = strBasePathName + "\\" + DIR_LIBRARIES + "\\" + "*.*";
	if ((hFind = ::FindFirstFile (strSearchPath, &fd)) != INVALID_HANDLE_VALUE)   {
		do {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
				CString strComp = (LPCTSTR) &fd.cFileName;
				CString strNewPath = strBasePathName, strDescription;
				int nVersion;
				strNewPath = strNewPath + "\\" + DIR_LIBRARIES + "\\" + (LPCTSTR) &fd.cFileName;
				if (CAdamoLuaLibrary::IsLuaLibraryFile (strNewPath, nVersion, strDescription))   {
					m_ctrlLibrary.AddString (fd.cFileName);
					bAdded = true;
				}
			}
		} while (::FindNextFile (hFind, &fd));
		::FindClose (hFind);
		if (bAdded)
			m_ctrlLibrary.SetCurSel (0);
	}
}

void CDlgInstallLibraries::LoadProjectLib ()
{
	CProject* pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		CStringList& ll = pPrj->GetLibraries ();
		for (POSITION pos = ll.GetHeadPosition (); pos; )   {
			CString strLibrary = ll.GetNext (pos);
			m_ctrlProject.AddString (strLibrary);
		}
	}
}

void CDlgInstallLibraries::OnAddToProject ()
{
	CString str;
	int nIndex;

	/* prendiamo la libreria selezionata */
	if ((nIndex = m_ctrlLibrary.GetCurSel ()) != LB_ERR)   {
		/* verifichiamo che non esista gia */
		m_ctrlLibrary.GetText (nIndex, str);
		/* scriviamola nel listbox del progetto */
		if (!ExistInProject (str))   {
			nIndex = m_ctrlProject.AddString (str);
			m_ctrlProject.SetCurSel (nIndex);
		}
	}
}

bool CDlgInstallLibraries::ExistInProject (CString& strLib)
{
	CString strPrj;
	bool bFound = false;

	for (int i = 0; i<m_ctrlProject.GetCount (); i++)   {
		m_ctrlProject.GetText (i, strPrj);
		if (strLib == strPrj)   {
			bFound = true; 
			break;
		}
	}
	return bFound;
}

void CDlgInstallLibraries::OnRemoveFromProject ()
{
	int nIndex;

	if ((nIndex = m_ctrlProject.GetCurSel ()) != LB_ERR)   {
		m_ctrlProject.DeleteString (nIndex);
		if (m_ctrlProject.GetCount () > 0)
			m_ctrlProject.SetCurSel (nIndex);
	}
}

void CDlgInstallLibraries::OnMoveUp ()
{
	CString strPrj;
	int nIndex;

	if ((nIndex = m_ctrlProject.GetCurSel ()) != LB_ERR)   {
		if (nIndex > 0)   {
			m_ctrlProject.GetText (nIndex, strPrj);
			m_ctrlProject.DeleteString (nIndex);
			nIndex = m_ctrlProject.InsertString (nIndex - 1, strPrj);
			m_ctrlProject.SetCurSel (nIndex);
		}
	}
}

void CDlgInstallLibraries::OnMoveDown ()
{
	CString strPrj;
	int nIndex;

	if ((nIndex = m_ctrlProject.GetCurSel ()) != LB_ERR)   {
		if (nIndex < m_ctrlProject.GetCount () - 1)   {
			m_ctrlProject.GetText (nIndex, strPrj);
			m_ctrlProject.DeleteString (nIndex);
			nIndex = m_ctrlProject.InsertString (nIndex + 1, strPrj);
			m_ctrlProject.SetCurSel (nIndex);
		}
	}
}

void CDlgInstallLibraries::InitButtons ()
{
	m_btnAddToProject.SetBitmaps (IDB_BMP_ARROW_RIGHT, RGB (255, 255, 255));
	m_btnAddToProject.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnAddToProject.SetWindowText (LOADSTRING (IDS_IL_INSTALLA));
	m_btnRemoveFromProject.SetBitmaps (IDB_BMP_ARROW_LEFT, RGB (255, 255, 255));
	m_btnRemoveFromProject.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnRemoveFromProject.SetWindowText (LOADSTRING (IDS_IL_DISINSTALLA));
	m_btnMoveUp.SetBitmaps (IDB_BMP_ARROW_UP, RGB (255, 255, 255));
	m_btnMoveUp.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnMoveUp.SetWindowText (LOADSTRING (IDS_IL_SPOSTA_SU));
	m_btnMoveDown.SetBitmaps (IDB_BMP_ARROW_DOWN, RGB (255, 255, 255));
	m_btnMoveDown.SetAlign (CButtonST::ST_ALIGN_HORIZ);
	m_btnMoveDown.SetWindowText (LOADSTRING (IDS_IL_SPOSTA_GIU));
	m_btnOk.SetWindowText ("Ok");
	m_btnCancel.SetWindowText ("Cancel");
}

void CDlgInstallLibraries::InitCaption ()
{
	SetWindowText (LOADSTRING (IDS_IL_IL));
	GetDlgItem (IDC_ST_LIB_DISPONIBILI)->SetWindowText (LOADSTRING (IDS_IL_LIB_AVL));
	GetDlgItem (IDC_ST_LIB_INSTALLATE)->SetWindowText (LOADSTRING (IDS_IL_LIB_INST));
}
