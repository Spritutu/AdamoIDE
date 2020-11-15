#if !defined(AFX_DLGCONNECTTOTCP_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_)
#define AFX_DLGCONNECTTOTCP_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConnectToTcp.h : header file
//

#include "ParEdit.h"

struct stNodeConnectTcpData
{
	CString strRemoteName;
	DWORD   dwDisplayType;
};

/////////////////////////////////////////////////////////////////////////////
// CDlgConnectToTcp dialog

class CDlgConnectToTcp : public CDialog
{
// Construction
public:
	CDlgConnectToTcp(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CAdamoAboutBox)
	enum { IDD = IDD_DLG_CONNECT_TCP };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdamoAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void LoadImages ();
    BOOL WINAPI EnumerateFunc(LPNETRESOURCE lpnr, HTREEITEM hItem);
    HTREEITEM   InsertItem (LPNETRESOURCE lpnrLocal, HTREEITEM hParentItem);
    void        OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
    void        ExpandAll();
    HTREEITEM   GetNextTreeItem(HTREEITEM hItem);
	void		FreeNetList ();
    void        InitControls ();
private :
    CTreeCtrl *m_pTreeCtrl;
    CIPAddressCtrl *m_pIPAddressCtrl;
	CImageList m_images;
	CPtrList   m_listPNetResource;
	CParsedEdit m_editPort;
    CAdamoOptions *m_opt;
public:
    afx_msg void OnBnClickedCtConnect();
	afx_msg void OnBnClickedCtConnectLocal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONNECTTOTCP_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_)
