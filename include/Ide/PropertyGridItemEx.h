// PropertyGridItemEx.h: interface for the CPropertyGridItemEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYGRIDITEMEX_H__062AA830_B5F7_41F0_98C5_70ED23AA21EE__INCLUDED_)
#define AFX_PROPERTYGRIDITEMEX_H__062AA830_B5F7_41F0_98C5_70ED23AA21EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCustomItemCheckBox;

class CInplaceCheckBox : public CButton
{
public:
	afx_msg LRESULT OnCheck(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	
	DECLARE_MESSAGE_MAP()
protected:
	CCustomItemCheckBox* m_pItem;
	COLORREF m_clrBack;
	CBrush m_brBack;

	friend class CCustomItemCheckBox;
};

class CCustomItemButtonText : public CXTPPropertyGridItem
{
    CView*      m_pGrid;
    UINT        m_nID;
    bool        m_bOpen;
public:
	CCustomItemButtonText(UINT nID, CString strCaption, bool bOpen=false);
    void SetGridView (CView* pGrid)    { m_pGrid=pGrid; }
protected:
	virtual void OnInplaceButtonDown();
};

class CCustomItemRSVar : public CXTPPropertyGridItem
{
    UINT        m_nID;
public:
	CCustomItemRSVar (UINT nID, CString strCaption);
    void SetResults (CString  strName, int  nRow, CString  strCol,  int nType);
    void GetResults (CString *strName, int *nRow, CString *strCol, int *nType);
private :
    CString m_strName, m_strCol;
    int m_nRow, m_nType;
};

class CCustomItemRSMatrix : public CXTPPropertyGridItem
{
    UINT        m_nID;
public:
	CCustomItemRSMatrix (UINT nID, CString strCaption);
    void SetResults (CString  strName);
    void GetResults (CString *strName);
private :
    CString m_strName;
protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
};

class CCustomItemRSDevice : public CXTPPropertyGridItem
{
public:
	CCustomItemRSDevice (UINT nID, CString strCaption);
protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
};

class CCustomItemRSTableIndex : public CXTPPropertyGridItem
{
    stRSVariabili *m_var;
public:
	CCustomItemRSTableIndex (UINT nID, CString strCaption);
    void SetDBData (stRSVariabili* var)     { m_var=var; }
    stRSVariabili *GetDBData ()             { return m_var; }
};

class CCustomItemRSList : public CXTPPropertyGridItem
{
    UINT        m_nID;
public:
	CCustomItemRSList (UINT nID, CString strCaption);
protected:
	virtual void OnInplaceButtonDown (CXTPPropertyGridInplaceButton *pItem);
};

class CCustomItemRSCombo : public CXTPPropertyGridItem
{
    UINT        m_nID;
public:
	CCustomItemRSCombo (UINT nID, CString strCaption);
protected:
	virtual void OnInplaceButtonDown (CXTPPropertyGridInplaceButton *pItem);
};

class CCustomItemColor: public CXTPPropertyGridItemColor
{
public:
	CCustomItemColor(UINT nID,  COLORREF clr = 0);
protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
public:
	COLORREF StringToRGB(CString str)	{	
		TCHAR *stopstring;
		int nValue = _tcstoul((LPCTSTR)str, &stopstring, 16);
		return RGB(GetBValue(nValue), GetGValue(nValue), GetRValue(nValue));
	}
	CString RGBToString(COLORREF clr)	{
		CString str;
		str.Format(_T("%2X%2X%2X"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
		str.Replace(_T(" "), _T("0"));
		return str;
	}
	void SetValue(CString strValue)	{	
		SetColor(StringToRGB(strValue));
	}
	void SetColor(COLORREF clr)	{
		m_clrValue = clr;
		CXTPPropertyGridItem::SetValue(RGBToString(clr));
	}
};

class CCustomItemSpin;

class CCustomItemSpinInplaceButton : public CSpinButtonCtrl
{
	friend class CCustomItemSpin;
	CCustomItemSpin* m_pItem;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
};

class CCustomItemSpin : public CXTPPropertyGridItemNumber
{
	friend class CCustomItemSpinInplaceButton;
public:
	CCustomItemSpin(UINT nID);
    void SetRange (double fpMin, double fpMax)   { m_fpMin=fpMin; m_fpMax=fpMax; }

protected:
	virtual void OnDeselect();
	virtual void OnSelect();
	virtual CRect GetValueRect();


private:
	CCustomItemSpinInplaceButton m_wndSpin;
    double m_fpMin, m_fpMax;
};

class CCustomItemRSGrid : public CXTPPropertyGridItem
{
    UINT        m_nID;
public:
	CCustomItemRSGrid (UINT nID, CString strCaption);
protected:
	virtual void OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton);
};

#endif // !defined(AFX_PROPERTYGRIDITEMEX_H__062AA830_B5F7_41F0_98C5_70ED23AA21EE__INCLUDED_)
