#if !defined(AFX_DLGSLOTVIEW_H__9500F393_B9BA_4445_BE5A_E1D5CB5F27C4__INCLUDED_)
#define AFX_DLGSLOTVIEW_H__9500F393_B9BA_4445_BE5A_E1D5CB5F27C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSlotView.h : header file
//

#include "slotview.h"
#include "adamomachine.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSlotView dialog

class CDlgSlotView : public CDialog
{
// Construction
public:
	CDlgSlotView(CWnd* pParent = NULL);   // standard constructor
    void SetCurrentMachine (CAdamoMachine* pMachine)   {
        m_pMachine=pMachine;
        m_slotView.SetCurrentMachine (pMachine);
    }

// Dialog Data
	//{{AFX_DATA(CDlgSlotView)
	enum { IDD = IDD_SLOTVIEW };
	CButton	m_ctrlButBreak;
	CButton	m_ctrlButSuspend;
	CButton	m_ctrlButRun;
	CButton	m_ctrlButDelete;
	CButton	m_ctrlButFreeze;
	CSlotView	m_slotView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSlotView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSlotView)
	virtual BOOL OnInitDialog();
	afx_msg void OnSlotDelete();
	afx_msg void OnSlotStateRun();
	afx_msg void OnSlotStateSuspended();
	afx_msg void OnSlotFreeze();
	afx_msg void OnSlotBreak();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoMachine* m_pMachine;
    bool               m_bFreeze;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSLOTVIEW_H__9500F393_B9BA_4445_BE5A_E1D5CB5F27C4__INCLUDED_)
