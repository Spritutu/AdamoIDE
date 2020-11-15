#pragma once

// finestra di dialogo CAdamoDlgLuaLibraries
#include "resource.h"

class CAdamoDlgLuaLibraries : public CDialog
{
	DECLARE_DYNAMIC(CAdamoDlgLuaLibraries)

public:
	CAdamoDlgLuaLibraries(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CAdamoDlgLuaLibraries();

	// Dati della finestra di dialogo
	enum { IDD = IDD_DLG_LIBRERIE };

private :
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnNewLibrary();
	afx_msg void OnEditLibrary();
	afx_msg void OnDeleteLibrary();
	DECLARE_MESSAGE_MAP()
private :
	CString CreateLibraryDirectory ();
	void InitListView ();
	void SetImageList ();
	void FillListView (CString strPathLib);
	void AddListViewItem (CString strName, CString strDesc);
private :
	CListCtrl m_listLibraries;
	CImageList m_images;
};
