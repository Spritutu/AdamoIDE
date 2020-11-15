#if !defined(AFX_DLGSAMPLES_H__B7C9304E_D28F_4704_BF8E_F211A8CDDE4D__INCLUDED_)
#define AFX_DLGSAMPLES_H__B7C9304E_D28F_4704_BF8E_F211A8CDDE4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSamples.h : header file
//

#include "SamplesGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSamples dialog

class CDlgOscilloscope;

class CDlgSamples : public CDialog
{
// Construction
public:
	CDlgSamples(CWnd* pParent = NULL);   // standard constructor
    void AddDevice (CAdamoBase* pab);
    int  GetSamples (CPtrList& lSamples);
    CSamplesGrid& GetSamplesGrid ()   {
        return m_Grid;
    }
    void SettaDlgOscilloscope (CDlgOscilloscope* pDlgOscilloscope) {
        m_pDlgOscilloscope=pDlgOscilloscope;;
    }
    void DeleteAllRows ();
    CAdamoBase* AddFromFile (CString str, int nTipoDev, int nDatoAsse);
    int         GetGridCount ()   { return m_Grid.GetCount (); }
    bool        IsModified ()     { return m_Grid.IsModified (); }
    void        ResetModified ()  { m_Grid.ResetModified (); }
	void        EndEdit ()		  { m_Grid.EndEdit (); }
// Dialog Data
	//{{AFX_DATA(CDlgSamples)
	enum { IDD = IDD_DLGSAMPLES };
	CButton	m_btnDelete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSamples)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSamples)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButSamplesDelete();
	afx_msg void OnDestroy();
    afx_msg void OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
    LONG DragOscDevice(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
private :
	CSamplesGrid m_Grid;
    CImageList m_ImageList;
    bool       m_bInitialized; 
    CDlgOscilloscope* m_pDlgOscilloscope;
    int m_nCurRow;
    int m_nCurColumn;
	bool m_bGridButtonPressed;
private :
    void OnOK ();
    void OnCancel ();
    void FillColumnsCaption ();
    void SetColumnSize ();
    void InitStrings ();
    void DestroyStrings ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAMPLES_H__B7C9304E_D28F_4704_BF8E_F211A8CDDE4D__INCLUDED_)
