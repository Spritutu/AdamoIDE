#if !defined(AFX_GLOBALVARIABLE_H__409ADF2B_8367_47D4_97C6_1BB465CD9435__INCLUDED_)
#define AFX_GLOBALVARIABLE_H__409ADF2B_8367_47D4_97C6_1BB465CD9435__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalVariable.h : header file
//

#include "gridctrl.h"
#include "gridcellcheck.h"
#include "adamoint.h"
#include "lobject.h"
#include "adamomachine.h"
#include "paredit.h"
#include "mainframe.h"
#include "basedialog.h"

#define ID_GLOBALVARIBLETIMER           400
#define ID_GLOBALVARIBLETIMER_PERIOD    100

/////////////////////////////////////////////////////////////////////////////
// CGlobalVariable dialog

class CGlobalVariable : public CAdamoDialog
{
// Construction
public:
	CGlobalVariable(CWnd* pParent = NULL);   // standard constructor
    void SetData (const char* caption, const char* strKey, const char *str, int nType, bool bLocalTimer=false)   {
        SetKey (strKey);
        SetVariableName (str);
        m_strCaption=caption; m_nType=nType;
        m_bLocalTimer=bLocalTimer;
    }
    void RunAlways ()                                  { m_bRunAlways=true; }
    void SetGL (int nGL)                               { m_nGL=nGL; }
    void SetLocalData (int nLevel, int nProg)          { m_nLevel=nLevel; m_nProg=nProg; }
    void OnTimer ();
	void SetParentWnd (CWnd* pWnd)					   { m_pParentWnd = pWnd; }
	CWnd* GetParentWnd ()							   { return m_pParentWnd; }
// Dialog Data
	//{{AFX_DATA(CGlobalVariable)
	enum { IDD = IDD_GLOBALVARIABLE };
	CStatic	m_ctrlStaticValue;
	CStatic	m_ctrlStaticName;
	CButton	m_ctrlButFreeze;
	CStatic	m_ctrlName;
	CButton m_ctrlButOk;
	//}}AFX_DATA
protected :
    virtual bool ReadData (COleVariant& v);
    virtual bool WriteData (COleVariant& v);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlobalVariable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGlobalVariable)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnFreezeVariable();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private :
    void FillCaption ();
    void OnOK ();
    void OnCancel ();
protected :
    int     m_nType;
private :
    CImageList m_ImageList;
    CString m_strCaption;
    CGridCtrl m_Grid;
    bool    m_bFreeze;
    bool    m_bRunAlways;
    int     m_nGL;
    int     m_nLevel;
    int     m_nProg;
    bool    m_bLocalTimer;
    int     m_nTimer;
	CWnd*   m_pParentWnd;
public:
	afx_msg void OnBnClickedUpdatevariable();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALVARIABLE_H__409ADF2B_8367_47D4_97C6_1BB465CD9435__INCLUDED_)
