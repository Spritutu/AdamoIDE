#if !defined(AFX_DLGTRIGGERS_H__1FB66E18_7045_4606_9816_85165BD56239__INCLUDED_)
#define AFX_DLGTRIGGERS_H__1FB66E18_7045_4606_9816_85165BD56239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTriggers.h : header file
//

#include "triggergrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggers dialog

class CDlgTriggers : public CDialog
{
// Construction
public:
	CDlgTriggers(CWnd* pParent = NULL);   // standard constructor
    int GetTriggers (CPtrList& lTriggers);
    void DeleteAllRows ();
    CAdamoBase* AddFromFile (CString str, int nTipoDev, int nDatoAsse, int nCmp, int nValue, double fpValue);
    int         GetGridCount ()   { return m_Grid.GetCount (); }
    bool        IsModified ()     { return m_Grid.IsModified (); }
    void        ResetModified ()  { m_Grid.ResetModified (); }

// Dialog Data
	//{{AFX_DATA(CDlgTriggers)
	enum { IDD = IDD_DLGTRIGGERS };
	CButton	m_btnDelete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTriggers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTriggers)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButTriggersDelete();
	//}}AFX_MSG
    LONG DragOscDevice(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
private :                  
    CTriggerGrid m_Grid;
    CImageList m_ImageList;
    bool       m_bInitialized; 
private :
    void FillColumnsCaption ();
    int  AddDevice (CAdamoBase* pab);
    void AddAD (CAdamoBase* pab);
    void AddCompare (CAdamoBase* pab);
    void AddAsse (CAdamoBase* pab);
    void OnOK ();
    void OnCancel ();
    void SetColumnSize ();
    void InitStrings ();
    void DestroyStrings ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRIGGERS_H__1FB66E18_7045_4606_9816_85165BD56239__INCLUDED_)
