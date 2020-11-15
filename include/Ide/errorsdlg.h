#if !defined(AFX_ERRORSDLG_H__4F530486_2374_40CF_AA3D_634F3A6EFD5D__INCLUDED_)
#define AFX_ERRORSDLG_H__4F530486_2374_40CF_AA3D_634F3A6EFD5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorsDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CErrorsDlg dialog

class CErrorsDlg : public CDialog
{
// Construction
public:
	CErrorsDlg(CWnd* pParent = NULL);   // standard constructor
    void SetData (CPtrList* pData);
    void Refresh ();
// Dialog Data
	//{{AFX_DATA(CErrorsDlg)
	enum { IDD = IDD_LISTERRORS };
	CButton	    m_ctrlButFreeze;
	CListCtrl	m_ctrlListErrors;
	CTabCtrl	m_ctrlTabErrors;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CErrorsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTaberrors(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnGotoLine();
	afx_msg void OnClearAll();
	afx_msg void OnFreeze();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void FillList (int nTypeError);
    void DeleteTabResource ();
    void DeleteHeaderResource ();
    void LoadHeaderHardware ();
    void LoadHeaderInterpreter ();
    void LoadHeaderISO ();
    void LoadHeaderCycle ();
    void FillInterpreter (stAdamoErrore* p, int i);
    void FillISO (stAdamoErrore* p, int i);
    void FillCycle (stAdamoErrore* p, int i);
    void FillHardware (stAdamoErrore* p, int i, CAdamoDatiFile* pADF);
    void LoadErrorsInListBox (int nSel);
private :
    CImageList* m_tbiml;
    CImageList* m_hciml;
    CBitmap*  m_bmpTblEH;
    CBitmap*  m_bmpTblEI;
    CBitmap*  m_bmpTblEC;
    CBitmap*  m_bmpTblEISO;
    CBitmap*  m_bmpHdrDate;
    CBitmap*  m_bmpHdrError;
    CBitmap*  m_bmpHdrTask;
    CBitmap*  m_bmpHdrPath;
    CBitmap*  m_bmpHdrLine;
    CBitmap*  m_bmpHdrDevice;
    CBitmap*  m_bmpHdrDeviceType;
    CBitmap*  m_bmpHdrIdentifier;
    CBitmap*  m_bmpHdrPriority;
    CBitmap*  m_bmpHdrDisk;

    CPtrList* m_pData;
    bool     m_bFreeze;
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORSDLG_H__4F530486_2374_40CF_AA3D_634F3A6EFD5D__INCLUDED_)
