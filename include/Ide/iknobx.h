#if !defined(AFX_IKNOBX_H__E7C423AE_FF2B_4CCF_B612_3889F56AB554__INCLUDED_)
#define AFX_IKNOBX_H__E7C423AE_FF2B_4CCF_B612_3889F56AB554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class COleFont;
class CPicture;

/////////////////////////////////////////////////////////////////////////////
// CiKnobX wrapper class

class CiKnobX : public CWnd
{
protected:
	DECLARE_DYNCREATE(CiKnobX)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xc5412dc7, 0x2e2f, 0x11d3, { 0x85, 0xbf, 0x0, 0x10, 0x5a, 0xc8, 0xb7, 0x15 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	COleFont GetPositionDisplayFont();
	void SetPositionDisplayFont(LPDISPATCH newValue);
	long GetPositionDisplayPrecision();
	void SetPositionDisplayPrecision(long nNewValue);
	CString GetPositionDisplayUnits();
	void SetPositionDisplayUnits(LPCTSTR lpszNewValue);
	unsigned long GetIndicatorActiveColor();
	void SetIndicatorActiveColor(unsigned long newValue);
	unsigned long GetIndicatorInactiveColor();
	void SetIndicatorInactiveColor(unsigned long newValue);
	long GetIndicatorMargin();
	void SetIndicatorMargin(long nNewValue);
	long GetIndicatorSize();
	void SetIndicatorSize(long nNewValue);
	long GetIndicatorStyle();
	void SetIndicatorStyle(long nNewValue);
	double GetKeyArrowStepSize();
	void SetKeyArrowStepSize(double newValue);
	double GetKeyPageStepSize();
	void SetKeyPageStepSize(double newValue);
	long GetKnobEdgeWidth();
	void SetKnobEdgeWidth(long nNewValue);
	long GetKnobRadius();
	void SetKnobRadius(long nNewValue);
	long GetKnobStyle();
	void SetKnobStyle(long nNewValue);
	long GetRotationMaxDegrees();
	void SetRotationMaxDegrees(long nNewValue);
	long GetRotationStartDegrees();
	void SetRotationStartDegrees(long nNewValue);
	BOOL GetShowFocusRect();
	void SetShowFocusRect(BOOL bNewValue);
	BOOL GetShowPostionDisplay();
	void SetShowPostionDisplay(BOOL bNewValue);
	unsigned long GetBackGroundColor();
	void SetBackGroundColor(unsigned long newValue);
	long GetOffsetX();
	void SetOffsetX(long nNewValue);
	long GetOffsetY();
	void SetOffsetY(long nNewValue);
	BOOL GetShowTicksMajor();
	void SetShowTicksMajor(BOOL bNewValue);
	BOOL GetShowTicksMinor();
	void SetShowTicksMinor(BOOL bNewValue);
	BOOL GetShowTickLabels();
	void SetShowTickLabels(BOOL bNewValue);
	long GetTickMajorCount();
	void SetTickMajorCount(long nNewValue);
	unsigned long GetTickMajorColor();
	void SetTickMajorColor(unsigned long newValue);
	long GetTickMajorLength();
	void SetTickMajorLength(long nNewValue);
	long GetTickMinorAlignment();
	void SetTickMinorAlignment(long nNewValue);
	long GetTickMinorCount();
	void SetTickMinorCount(long nNewValue);
	unsigned long GetTickMinorColor();
	void SetTickMinorColor(unsigned long newValue);
	long GetTickMinorLength();
	void SetTickMinorLength(long nNewValue);
	long GetTickMargin();
	void SetTickMargin(long nNewValue);
	long GetTickLabelMargin();
	void SetTickLabelMargin(long nNewValue);
	COleFont GetTickLabelFont();
	void SetTickLabelFont(LPDISPATCH newValue);
	long GetTickLabelPrecision();
	void SetTickLabelPrecision(long nNewValue);
	void ResetMinMax();
	double GetCurrentMax();
	void SetCurrentMax(double newValue);
	double GetCurrentMin();
	void SetCurrentMin(double newValue);
	double GetPositionPercent();
	void SetPositionPercent(double newValue);
	double GetPosition();
	void SetPosition(double newValue);
	double GetPositionMax();
	void SetPositionMax(double newValue);
	double GetPositionMin();
	void SetPositionMin(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	void AboutBox();
	unsigned long GetPositionDisplayFontColor();
	void SetPositionDisplayFontColor(unsigned long newValue);
	unsigned long GetTickLabelFontColor();
	void SetTickLabelFontColor(unsigned long newValue);
	CPicture GetBackGroundPicture();
	void SetBackGroundPicture(LPDISPATCH newValue);
	void ShowPropertyEditor();
	void SetPositionNoEvent(double Value);
	BOOL GetMinMaxFixed();
	void SetMinMaxFixed(BOOL bNewValue);
	long GetPrecisionStyle();
	void SetPrecisionStyle(long nNewValue);
	BOOL GetTransparent();
	void SetTransparent(BOOL bNewValue);
	void RepaintAll();
	void BeginUpdate();
	void EndUpdate();
	long GetAutoScaleDesiredTicks();
	void SetAutoScaleDesiredTicks(long nNewValue);
	long GetAutoScaleMaxTicks();
	void SetAutoScaleMaxTicks(long nNewValue);
	BOOL GetAutoScaleEnabled();
	void SetAutoScaleEnabled(BOOL bNewValue);
	long GetAutoScaleStyle();
	void SetAutoScaleStyle(long nNewValue);
	void Lock();
	void Unlock();
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	long GetOuterMargin();
	void SetOuterMargin(long nNewValue);
	long GetUpdateFrameRate();
	void SetUpdateFrameRate(long nNewValue);
	long GetComponentHandle();
	void SaveImageToBitmap(LPCTSTR FileName);
	void SaveImageToMetaFile(LPCTSTR FileName);
	void SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive);
	BOOL GetUseKeyboard();
	void SetUseKeyboard(BOOL bNewValue);
	long OPCAddItem();
	long OPCItemCount();
	void OPCRemoveAllItems();
	void OPCItemActivate(long Index);
	void OPCItemDeactivate(long Index);
	CString GetOPCItemComputerName(long Index);
	CString GetOPCItemServerName(long Index);
	CString GetOPCItemItemName(long Index);
	CString GetOPCItemPropertyName(long Index);
	BOOL GetOPCItemAutoConnect(long Index);
	long GetOPCItemUpdateRate(long Index);
	void SetOPCItemComputerName(long Index, LPCTSTR Value);
	void SetOPCItemServerName(long Index, LPCTSTR Value);
	void SetOPCItemItemName(long Index, LPCTSTR Value);
	void SetOPCItemPropertyName(long Index, LPCTSTR Value);
	void SetOPCItemAutoConnect(long Index, BOOL Value);
	void SetOPCItemUpdateRate(long Index, long Value);
	void OPCItemLoaded(long Index);
	BOOL GetCachedDrawing();
	void SetCachedDrawing(BOOL bNewValue);
	long GetObjectPointer();
	VARIANT GetBytesJPEG(long Compression, BOOL Progressive);
	void SavePropertiesToFile(LPCTSTR FileName);
	void LoadPropertiesFromFile(LPCTSTR FileName);
	BOOL GetOptionSaveAllProperties();
	void SetOptionSaveAllProperties(BOOL bNewValue);
	double GetMouseWheelStepSize();
	void SetMouseWheelStepSize(double newValue);
	BOOL GetAutoFrameRate();
	void SetAutoFrameRate(BOOL bNewValue);
	void iPaintToDC(long X, long Y, BOOL Transparent, long DC);
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetHeight();
	void SetHeight(long nNewValue);
	void OPCItemUpdateResume(long Index);
	void OPCItemUpdateSuspend(long Index);
	VARIANT GetOPCItemData(long Index);
	short GetOPCItemQuality(long Index);
	double GetOPCItemTimeStamp(long Index);
	void SetOPCItemData(long Index, const VARIANT& Value);
	BOOL GetOPCItemActive(long Index);
	CString GetOPCItemGroupName(long Index);
	void SetOPCItemGroupName(long Index, LPCTSTR Value);
	void OPCDeleteItem(long Index);
	void OPCItemActivateSuspend(long Index);
	VARIANT GetBytesPNG(long Compression);
	void SaveImageToPNG(LPCTSTR FileName, long Compression);
	VARIANT GetOPCItemDataNow(long Index, short* Quality);
	BOOL GetOPCItemUpdateSuspended(long Index);
	unsigned long GetColor();
	void SetColor(unsigned long newValue);
	double GetOPCItemScalar(long Index);
	double GetOPCItemOffset(long Index);
	void SetOPCItemScalar(long Index, double Value);
	void SetOPCItemOffset(long Index, double Value);
	BOOL GetShowDisabledState();
	void SetShowDisabledState(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IKNOBX_H__E7C423AE_FF2B_4CCF_B612_3889F56AB554__INCLUDED_)
