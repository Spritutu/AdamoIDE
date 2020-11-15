#if !defined(AFX_USERSTRINGS_H__CCCC6A4A_FD23_496E_84CE_94FE1F15C2E5__INCLUDED_)
#define AFX_USERSTRINGS_H__CCCC6A4A_FD23_496E_84CE_94FE1F15C2E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserStrings.h : header file
//

#include "paredit.h"

/////////////////////////////////////////////////////////////////////////////
// CUserStrings dialog

class CUserStrings : public CDialog
{
// Construction
public:
	CUserStrings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserStrings)
	enum { IDD = IDD_DLG_USERSTRINGS };
	CParsedEdit	m_strDescrizione;
	CParsedEdit	m_strID;
	CParsedEdit	m_strMessaggio;
	CParsedEdit	m_strValore;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserStrings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public  :
    void SetId          (const char* ID)            { m_strValoreID=ID;}
    void SetNumero      (const char* Numero)        { m_strValoreNumero=Numero;}
    void SetMessaggio   (const char* Messaggio)     { m_strValoreMessaggio=Messaggio;}
    void SetDescrizione (const char* Descrizione)   { m_strValoreDescrizione=Descrizione;}
    void SetModify      ()                          { m_bModify=true;}
    CString GetId          ()  { m_strValoreID.TrimLeft(); return m_strValoreID; }
    CString GetNumero      ()  { m_strValoreNumero.TrimLeft(); return m_strValoreNumero; }
    CString GetMessaggio   ()  { m_strValoreMessaggio.TrimLeft(); return m_strValoreMessaggio; }
    CString GetDescrizione ()  { m_strValoreDescrizione.TrimLeft(); return m_strValoreDescrizione; }
    void OnOK ();
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserStrings)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CString m_strValoreID;
    CString m_strValoreNumero;
    CString m_strValoreMessaggio;
    CString m_strValoreDescrizione;
    bool    m_bModify;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSTRINGS_H__CCCC6A4A_FD23_496E_84CE_94FE1F15C2E5__INCLUDED_)
