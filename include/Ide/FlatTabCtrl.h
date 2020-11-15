// FlatTabCtrl.h: interface for the CFlatTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLATTABCTRL_H__E2863AF3_7529_40A7_8A2A_103810FB7D28__INCLUDED_)
#define AFX_FLATTABCTRL_H__E2863AF3_7529_40A7_8A2A_103810FB7D28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlatTabCtrl : public CXTFlatTabCtrl  
{
public:
	CFlatTabCtrl();
	CFlatTabCtrl(CView *pView);
	DECLARE_DYNAMIC(CFlatTabCtrl)
	virtual ~CFlatTabCtrl();
protected:
    CView *m_pView;
    CObject *SendGridCan(NMHDR *pNotifyStruct);
    CObject *SendGrid(NMHDR *pNotifyStruct);
    CView *GetDockProperty();
    CString Trim(CString strString);
    void ComboDev(NM_GRIDVIEW* pItem);
    void ComboDevCan(NM_GRIDVIEW* pItem);
    afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridBeginEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FLATTABCTRL_H__E2863AF3_7529_40A7_8A2A_103810FB7D28__INCLUDED_)
