#if !defined(AFX_GRIDVIEW_H__D2761E17_312F_4829_9771_FDC1F652B126__INCLUDED_)
#define AFX_GRIDVIEW_H__D2761E17_312F_4829_9771_FDC1F652B126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridView.h : header file
//
#include "CharCheck.h"
#include "PaneView.h"

/////////////////////////////////////////////////////////////////////////////
// CGridView view

class CGridView : public CPaneView
{
    friend class CCustomItemAxisButtonText;
protected:
	CGridView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGridView)
	CXTPToolBar m_wndToolBar;

// Attributes
public:

// Operations
public:
	void SetObject(CObject *pObj);
    void SetPswLevel (ePswLevel newPsw)   { m_ePswLevel=newPsw; }
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
    void SetModifiedFlag();
    bool IsValidName(CString strName);
    void StringControlName(CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    void CheckInput(CXTPPropertyGridInplaceEdit* pEdit);
	void SwitchForDgtIn(CXTPPropertyGridItem *pItem);
	void SwitchForDgtOut(CXTPPropertyGridItem *pItem);
	void SwitchForAnalogIn(CXTPPropertyGridItem *pItem);
	void SwitchForAnalogOut(CXTPPropertyGridItem *pItem);
	void SwitchForAxis(CXTPPropertyGridItem *pItem);
    void SwitchForCounter(CXTPPropertyGridItem *pItem);
    void SwitchForTimer(CXTPPropertyGridItem *pItem);
    void SwitchForModule (CXTPPropertyGridItem *pItem);
    CXTPPropertyGridItem *AddGridItemCheckBox(int iId,CString strCaption, BOOL bValue,CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridItemd(int iId,CString strCaption, double dValue,CXTPPropertyGridItem *pItemNode,BOOL bReadOnly = FALSE);
    CXTPPropertyGridItem *AddGridItemld(int iId,CString strCaption, double dValue,CXTPPropertyGridItem *pItemNode,BOOL bReadOnly = FALSE);
    CXTPPropertyGridItem *AddGridItemdHighRes(int iId,CString strCaption, double dValue,CXTPPropertyGridItem *pItemNode,BOOL bReadOnly, CString strRes) ;
	CXTPPropertyGridItem *AddGridItemi(int iId,CString strCaption,int iValue,CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridItem(int iId,CString strCaption,CString strValue,CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridItemButtonText (int iId,CString strCaption, void* pAxis, CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboItemAxisType(int iValue,CXTPPropertyGridItem *pItemNode);
    CXTPPropertyGridItem *AddGridComboItemAxisControlType (int iValue,CXTPPropertyGridItem *pItemNode);
	CXTPPropertyGridItem *AddGridComboSmootherType (int iValue,CXTPPropertyGridItem *pItemNode);

	void AddGridComboItemAxesCategory(int iValue,CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemAxesUnit(int iValue,CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemAxesRampType(int iId,CString strCaption,int iValue,CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemCounterType(int iValue,CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemServoLimitType(int iValue,CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemAxisMaster (CAdamoAxis *pAxis, CString strMasterName, CXTPPropertyGridItem *pItemNode);
    void AddGridComboModule (CProject *pPrj, CXTPPropertyGridItem *pItemNode);
    void AddGridComboNumber (CProject *pPrj, CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemAxisMovementCategory (int iValue,CXTPPropertyGridItem *pItemNode);
    void AddGridComboItemGSlot (int nGSlot, CXTPPropertyGridItem *pItemNode);
	void SetForAxis(CAdamoAxis *pAxis,CXTPPropertyGridItem *pRoot);
    void AddGridComboFrequency (CProject *pPrj, CXTPPropertyGridItem *pItemNode);
    void AddGridComboPswLevel (int iValue,CXTPPropertyGridItem *pItemNode, int nCaption);
	void SetForAnalogIn(CAdamoAnalogIn *pAnalogIn,CXTPPropertyGridItem *pRoot);
	void SetForAnalogOut(CAdamoAnalogOut *pAnalogOut,CXTPPropertyGridItem *pRoot);
	void SetForDgtIn(CAdamoDgtIn *pDgtIn,CXTPPropertyGridItem *pRoot);
	void SetForDgtOut(CAdamoDgtOut *pDgtOut,CXTPPropertyGridItem *pRoot);
	void SetForCounter(CAdamoCounter *pCounter,CXTPPropertyGridItem *pRoot);
	void SetForTimer(CAdamoTimer *pTimer,CXTPPropertyGridItem *pRoot);
    void SetForModule (CProject *pPrj,CXTPPropertyGridItem *pRoot);
	void RefreshFrame ();
	virtual void RefreshMetrics(AppTheme nTheme);
    CCharCheck m_objCheckAlphaNum;
    CCharCheck m_objCheckName;
    CCharCheck m_objCheckNumbers;
    CCharCheck m_objCheckReal;
    CString Trim(CString strString);
	CXTPPropertyGrid m_wndPropertyGrid;
    CObject  *m_pCurrentObj;
    ePswLevel m_ePswLevel;
	//{{AFX_MSG(CGridView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private :
    CProject* GetRootPrj ();
    void SwitchMachine (CProject* pPrj);
    eTypeMachine GetMachineType();
    bool IsValidNameAxis (CString strValue, int nType, int nGSlot=_NO_GSLOT);
    bool Iterator(CAdamoGroup *pGrp, CString strValue, int nType, int nGSlot);
    void IsoAxisControl(CXTPPropertyGridInplaceEdit* pEdit);
    bool IsValidIsoAxis (char chIsoAxisName );
    bool IsValidDouble (CString strValue, double fpMinRange, double fpMaxRange, double *fp);
    bool IsValidInteger (CString strValue, int fpMinRange, int fpMaxRange, int *n);
    void CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength);
    CString GetGantryMasterName (int nAxis);
    int  GetGantryMasterName (CString str);
    void RinominaFileDeiCorrettori (CAdamoAxis *pAxis, CString strNewName);
    void GestionePsw (CXTPPropertyGridItem* pItem, ePswLevel e);
	CString AggiungiUnitaDiMisura (CString str, enum enUnitaMisura um, int nTipo);
	CString CorreggiUnitaDiMisura (enum enUnitaMisura um);
	void FilterValue (int nID, CAdamoData& ad);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDVIEW_H__D2761E17_312F_4829_9771_FDC1F652B126__INCLUDED_)
