#if !defined(AFX_OPTIONDLG_H__FC86658B_A4A3_440D_A8C1_B71C3DED29B8__INCLUDED_)
#define AFX_OPTIONDLG_H__FC86658B_A4A3_440D_A8C1_B71C3DED29B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//

/* cnstants definitions */
#define ID_OPT_LANGUAGE     100
#define ID_OPT_CONNECTION   101
#define ID_OPT_TCPIP_ADDRESS 102
#define ID_OPT_TCP_IP_PORT  103
#define ID_OPT_KILLMACHINE  104
#define ID_OPT_LOADVIRTUAL  105
#define ID_OPT_GRIDSTYLE    106

#include "AdamoOptions.h"
#include "CharCheck.h"

class CPropertyGridEx : public CXTPPropertyGrid
{
public:
	HTREEITEM m_hItem;
};

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog
class COptionDlg : public CDialog
{
	class CPropertyTree : public CTreeCtrl	{
	public:
		CPropertyTree::CPropertyTree()
		{
			m_imgList.Create(IDB_OPTIONS, 16, 4, RGB(0, 0xFF, 0));			
		}
		void PreSubclassWindow()
		{
			ModifyStyle(TVS_SHOWSELALWAYS, TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS |TVS_SINGLEEXPAND);
			SetImageList(&m_imgList, TVSIL_NORMAL);
			
			CTreeCtrl::PreSubclassWindow();
		}
		CImageList m_imgList;
	};
// Construction
public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor
	CPropertyTree m_wndProperties;
	CRect m_rcWorkspace;
	CPropertyGridEx* m_pSelected;
protected:
	void CreateGridThira (HTREEITEM hItemCategory);
	void CreateGridCommunicationServer (HTREEITEM hItemCategory);
	void CreateGridConnection (HTREEITEM hItemCategory);
	void CreateGridInternational(HTREEITEM hItemCategory);
	void CreateGridSkin (HTREEITEM hItemCategory);
	CPropertyGridEx m_wndGridThira;
	CPropertyGridEx m_wndGridCommunicationServer;
	CPropertyGridEx m_wndGridInternational;
	CPropertyGridEx m_wndGridConnection;
	CPropertyGridEx m_wndGridSkin;
    // Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListOptions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedListOptions(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private :
    void AddGridItemButtonText (int iId, CString strCaption, CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridComboConnectionType (CXTPPropertyGridItem* pGridItem, eTypeConnection eConnType);
    CXTPPropertyGridItem *AddGridItem (int iId,CString strCaption, CString strValue, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridItemCheckBox (int iId, CString strCaption, BOOL bValue, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboSkin (CXTPPropertyGridItem* pGridItem, CString strSkin);
    void InitCharCheck ();
    void CheckInput (int nID, CXTPPropertyGridInplaceEdit* pEdit);
    void CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    void CheckIpAddress (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
private :
    CAdamoOptions *m_opt;
    CCharCheck     m_objCheckNumbers;
    CCharCheck     m_objCheckAddressIP;
protected:
    virtual void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__FC86658B_A4A3_440D_A8C1_B71C3DED29B8__INCLUDED_)
