#if !defined(AFX_ADAMOCPDLG_H__AA3F90E5_30DD_44CC_BEE2_78A98055C465__INCLUDED_)
#define AFX_ADAMOCPDLG_H__AA3F90E5_30DD_44CC_BEE2_78A98055C465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoCPDlg.h : header file
//

#include "GridCtrl.h"
#include "Adamobase.h"
#include "AdamoAxis.h"

/* forward reference */
class CAdamoCPDlg;

/////////////////////////////////////////////////////////////////////////////
// CAdamoCPDlg dialog

class CAdamoCPGrid : public CGridCtrl
{
        CAdamoCPDlg* m_pDlg;
        virtual void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
        virtual void OnEndEditCell(int nRow, int nCol, CString str);
        void ClearAllSelection ();
    public :
        CAdamoCPGrid ();
        void SetCPDlg (CAdamoCPDlg* pDlg)      { m_pDlg=pDlg; }
        void InitControl ();
        void AddLine (int i, int nType, const char* szName, void* pData);
        bool GetLine (int nIndex, int* pnType, char* szName, void* pData);
        void SelectCell (int nRow, int nCol);
    private :
        CStringArray m_slTypeCP;
        bool m_abEdit[MAX_AXIS_CUSTOM];
};

class CCPGridCellCheck : public CGridCellCheck 
{
    DECLARE_DYNCREATE(CCPGridCellCheck)
    public :
        CCPGridCellCheck () : m_pCP (NULL)       { }
        void SetCPDlg (CAdamoCPDlg *pCP)         { m_pCP=pCP; }
    protected :
        void OnClick( CPoint PointCellRelative);
    private :
        CAdamoCPDlg *m_pCP;

};

class CAdamoCPDlg : public CDialog
{
// Construction
public:
	CAdamoCPDlg(CWnd* pParent = NULL);   // standard constructor
    void SetActiveAxis (CAdamoAxis* pAx)        { m_pAx=pAx; }
    bool IsModified ()                          { return m_bIsModified; }
    void SetModified (bool bIsModified)         { m_bIsModified=bIsModified; }
// Dialog Data
	//{{AFX_DATA(CAdamoCPDlg)
	enum { IDD = IDD_DLG_CP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoCPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoCPDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void InitGrid ();
    void FillGrid ();
    bool FillData ();
    void LoadStrings ();
private :
    CAdamoCPGrid m_grid;
    CImageList   m_ImageList;
    CAdamoAxis*  m_pAx;
    bool         m_bIsModified;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOCPDLG_H__AA3F90E5_30DD_44CC_BEE2_78A98055C465__INCLUDED_)
