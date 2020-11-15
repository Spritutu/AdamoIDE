/* @doc
 * @module ReplaceDlg.h | definition of a modeless search/replace dialog for use with the scintilla edit control
 */
#ifndef __REPLACEDLG_H__
#define __REPLACEDLG_H__

#include "ide2.h"
#include "LuaEditor.h"
////////////////////////////////////
// @class CReplaceDlg | Class of a modeless search/replace dialog for use with the scintilla edit control
// @base public | CDialog
//
class CReplaceDlg : public CDialog
{
public:
// @access public constructor
// @cmember empty Constructor
	CReplaceDlg(CWnd* pParent = NULL);

public:
// @access public data members
// @cmember init dialog
   virtual BOOL InitDialog();
// @cmember store new search string to global strings
   void StoreSearchString(CIdeApp *app, LPCSTR szSearch);
// @cmember store new search string to global strings
   void StoreReplaceString(CIdeApp *app, LPCSTR szReplace);

public:
// @access public data members
// @cmember a pointer to the scintilla edit control
	CLuaEditor *m_pEdit;   // editcontrol to use
// @cmember a flag for the default serach direction
	BOOL          m_bForwardSearch; 

public:
// Dialogfelddaten
	//{{AFX_DATA(CReplaceDlg)
	enum { IDD = IDD_REPLACE };
	CComboBox	m_ctrlSearchtext;
	CComboBox	m_ctrlReplacetext;
	BOOL	m_bMatchcase;
	BOOL	m_bMatchregex;
	BOOL	m_bMatchwords;
	BOOL	m_bSelection;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CReplaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL
   void OnOK ();
   void OnCancel ();
// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CReplaceDlg)
	afx_msg void OnNext();
	afx_msg void OnPrev();
	afx_msg void OnReplace();
	afx_msg void OnReplaceall();
	afx_msg void OnMatchselection();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_REPLACEDLG_H__31BB518C_B58B_498E_A338_FC992EB24B75__INCLUDED_
