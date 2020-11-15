#if !defined(AFX_ADAMOTABLES_H__4AF8E3CF_9A9B_4B5F_AC65_F1458871437C__INCLUDED_)
#define AFX_ADAMOTABLES_H__4AF8E3CF_9A9B_4B5F_AC65_F1458871437C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamotables.h : header file
//

#include "AdamoTablesSupergrid.h"

#define ID_TABLESTIMER 401
#define ID_REPCTRL_ADAMOTABLES 1005

/////////////////////////////////////////////////////////////////////////////
// CAdamoTables dialog

class CAdamoTables : public CDialog
{
    friend class CTablesSuperGrid;
// Construction
public:
	CAdamoTables(CWnd* pParent = NULL);   // standard constructor
    void SetData (const char* caption, const char* str)   {
        m_strCaption=caption; m_strVariableName=str;
    }
    void SetGL (int nGL)                               { m_nGL=nGL; }
    void SetLocalData (int nLevel, int nProg)          { m_nLevel=nLevel; m_nProg=nProg; }

// Dialog Data
	//{{AFX_DATA(CAdamoTables)
	enum { IDD = IDD_DLG_TABLE };
	CStatic	m_stName;
	CStatic	m_stFrame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoTables)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoTables)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnCloseDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void OnOK ();
private :
    CTablesSuperGrid m_wndReport;
    CString m_strVariableName;
    CString m_strCaption;
    int     m_nGL;
    int     m_nLevel;
    int     m_nProg;
    UINT    m_nTimer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOTABLES_H__4AF8E3CF_9A9B_4B5F_AC65_F1458871437C__INCLUDED_)
