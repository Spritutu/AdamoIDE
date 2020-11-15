#pragma once

// finestra di dialogo CAdamoDlgLuaLibraries
#include "resource.h"

class CAdamoDlgNewLuaLibrary : public CDialog
{
	DECLARE_DYNAMIC(CAdamoDlgNewLuaLibrary)

public:
	CAdamoDlgNewLuaLibrary(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CAdamoDlgNewLuaLibrary();

	// Dati della finestra di dialogo
	enum { IDD = IDD_NEW_LIBRARY };
    void	SetName (CString strName)			{ m_strName = strName; }
    void    SetDesc (CString strDescription)	{ m_strDesc = strDescription; }
    CString GetName ()   { return m_strName; }
    CString GetDesc ()   { return m_strDesc; }
private :
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK ();
	DECLARE_MESSAGE_MAP()
private :
	CParsedEdit	m_ctrlName;
	CParsedEdit	m_ctrlDesc;
    CString m_strName;
    CString m_strDesc;
};
