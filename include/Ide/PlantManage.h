#if !defined(AFX_PLANTMANAGE_H__A93EFA05_886E_44E7_ACB9_B57E72AC60EF__INCLUDED_)
#define AFX_PLANTMANAGE_H__A93EFA05_886E_44E7_ACB9_B57E72AC60EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlantManage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlantManage dialog

class CPlantManage : public CDialog
{

// Construction
public:
	CString GetPlantName();
	CPlantManage(CWnd* pParent,CString strPathPlant);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlantManage)
	enum { IDD = IDD_DLG_PLANT_MANAGE };
	CListCtrl	m_ctrListPlant;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlantManage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString LoadDesc(CString strPath);
	int  AddItem(int nIndex,CString strName,CString strDesc);
	void InitListView();
    CImageList m_iIcons;
    CString m_strPathPlant;
    CString m_strNamePlant;
	// Generated message map functions
	//{{AFX_MSG(CPlantManage)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListPlant(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
    afx_msg void OnAddPlant ();
    afx_msg void OnDeletePlant ();
	afx_msg void OnItemChangedPlantManage(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void CheckCurrentPlant ();
	void AddItem (CString strName, CString strDescription);
    int New (CString& strName, CString& strDescription);
    BOOL Save(CString strPathName, CString strName, CString strDescription);
	int  GetPlantInfo (CString &strName, CString &strDescription);
	bool Modify (CString &strName, CString &strDescription);
	void ModifyItem (CString &strName, CString &strDescription, int nItem);
	bool ModifyLWR (CString strPathName, CString strName, CString strDescription);
public:
	afx_msg void OnBnClickedButRename();
	afx_msg void OnBnClickedButCopy();
	afx_msg void OnBnClickedButmodify();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLANTMANAGE_H__A93EFA05_886E_44E7_ACB9_B57E72AC60EF__INCLUDED_)
