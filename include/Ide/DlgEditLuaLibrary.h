#pragma once

// finestra di dialogo CAdamoDlgLuaLibraries
#include "BtnST.h"
#include "resource.h"

/* forward reference */
class CAdamoLuaLibrary;

class CAdamoDlgEditLuaLibrary : public CDialog
{
	DECLARE_DYNAMIC(CAdamoDlgEditLuaLibrary)

public:
	CAdamoDlgEditLuaLibrary(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CAdamoDlgEditLuaLibrary();
	void SetLibrary (CString strLib)   { m_strLibrary = strLib; }
	// Dati della finestra di dialogo
	enum { IDD = IDD_DLG_EDITA_LIBRERIE };

private :
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV
	//{{AFX_MSG(CAdamoAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnAddToLibrary();
	afx_msg void OnRemoveFromLibrary();
	afx_msg void OnMoveUpModule();
	afx_msg void OnMoveDownModule();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected :
	void OnOK ();
private :
	void InitListViews ();
	void FillListViews ();
	void FillListViewPrj ();
	void FillListViewLib ();
	int  OpenLibraryFile ();
	int  CloseLibraryFile ();
	CString GetModulePathName (CProject* pPrj, CString strModuleName);
	CString GetBinaryPathName (CProject* pPrj, CString strModuleName);
	void AddFunctions (CProject* pPrj, CString strModule, CAdamoLuaLibraryModule *pModule);
	void AddObjects (CProject* pPrj, CString strModule, CAdamoLuaLibraryModule *pModule);
	void InitButtons ();
	void InitCaption ();
private :
	CAdamoLuaLibrary* m_pLuaLibrary;
	CString m_strLibrary;
	CListCtrl m_listPrjMod;
	CListCtrl m_listLibMod;
	CImageList m_imagesPrjMod;
	CImageList m_imagesLibMod;
	CButtonST m_btnAddToLibrary;
	CButtonST m_btnRemoveFromLibrary;
	CButtonST m_btnMoveModuleUp;
	CButtonST m_btnMoveModuleDown;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;
};
