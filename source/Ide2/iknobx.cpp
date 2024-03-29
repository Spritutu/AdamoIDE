// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "iknobx.h"

// Dispatch interfaces referenced by this interface
#include "font.h"
#include "picture.h"

/////////////////////////////////////////////////////////////////////////////
// CiKnobX

IMPLEMENT_DYNCREATE(CiKnobX, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CiKnobX properties

/////////////////////////////////////////////////////////////////////////////
// CiKnobX operations

COleFont CiKnobX::GetPositionDisplayFont()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return COleFont(pDispatch);
}

void CiKnobX::SetPositionDisplayFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetPositionDisplayPrecision()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPositionDisplayPrecision(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString CiKnobX::GetPositionDisplayUnits()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPositionDisplayUnits(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

unsigned long CiKnobX::GetIndicatorActiveColor()
{
	unsigned long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetIndicatorActiveColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CiKnobX::GetIndicatorInactiveColor()
{
	unsigned long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetIndicatorInactiveColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetIndicatorMargin()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetIndicatorMargin(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetIndicatorSize()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetIndicatorSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetIndicatorStyle()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetIndicatorStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

double CiKnobX::GetKeyArrowStepSize()
{
	double result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetKeyArrowStepSize(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetKeyPageStepSize()
{
	double result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetKeyPageStepSize(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetKnobEdgeWidth()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetKnobEdgeWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetKnobRadius()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetKnobRadius(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetKnobStyle()
{
	long result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetKnobStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetRotationMaxDegrees()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetRotationMaxDegrees(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetRotationStartDegrees()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetRotationStartDegrees(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiKnobX::GetShowFocusRect()
{
	BOOL result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetShowFocusRect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiKnobX::GetShowPostionDisplay()
{
	BOOL result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetShowPostionDisplay(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long CiKnobX::GetBackGroundColor()
{
	unsigned long result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetBackGroundColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetOffsetX()
{
	long result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetOffsetX(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetOffsetY()
{
	long result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetOffsetY(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiKnobX::GetShowTicksMajor()
{
	BOOL result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetShowTicksMajor(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiKnobX::GetShowTicksMinor()
{
	BOOL result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetShowTicksMinor(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiKnobX::GetShowTickLabels()
{
	BOOL result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetShowTickLabels(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiKnobX::GetTickMajorCount()
{
	long result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMajorCount(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CiKnobX::GetTickMajorColor()
{
	unsigned long result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMajorColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetTickMajorLength()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMajorLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetTickMinorAlignment()
{
	long result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMinorAlignment(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetTickMinorCount()
{
	long result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMinorCount(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CiKnobX::GetTickMinorColor()
{
	unsigned long result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMinorColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetTickMinorLength()
{
	long result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMinorLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetTickMargin()
{
	long result;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickMargin(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetTickLabelMargin()
{
	long result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickLabelMargin(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

COleFont CiKnobX::GetTickLabelFont()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return COleFont(pDispatch);
}

void CiKnobX::SetTickLabelFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CiKnobX::GetTickLabelPrecision()
{
	long result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickLabelPrecision(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CiKnobX::ResetMinMax()
{
	InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

double CiKnobX::GetCurrentMax()
{
	double result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetCurrentMax(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetCurrentMin()
{
	double result;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetCurrentMin(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetPositionPercent()
{
	double result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPositionPercent(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetPosition()
{
	double result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPosition(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetPositionMax()
{
	double result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPositionMax(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetPositionMin()
{
	double result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPositionMin(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CiKnobX::GetVisible()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiKnobX::GetEnabled()
{
	BOOL result;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiKnobX::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

unsigned long CiKnobX::GetPositionDisplayFontColor()
{
	unsigned long result;
	InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPositionDisplayFontColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long CiKnobX::GetTickLabelFontColor()
{
	unsigned long result;
	InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTickLabelFontColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CPicture CiKnobX::GetBackGroundPicture()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPicture(pDispatch);
}

void CiKnobX::SetBackGroundPicture(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void CiKnobX::ShowPropertyEditor()
{
	InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiKnobX::SetPositionNoEvent(double Value)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

BOOL CiKnobX::GetMinMaxFixed()
{
	BOOL result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetMinMaxFixed(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiKnobX::GetPrecisionStyle()
{
	long result;
	InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetPrecisionStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiKnobX::GetTransparent()
{
	BOOL result;
	InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetTransparent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x31, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiKnobX::RepaintAll()
{
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiKnobX::BeginUpdate()
{
	InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiKnobX::EndUpdate()
{
	InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CiKnobX::GetAutoScaleDesiredTicks()
{
	long result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetAutoScaleDesiredTicks(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x35, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetAutoScaleMaxTicks()
{
	long result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetAutoScaleMaxTicks(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x36, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiKnobX::GetAutoScaleEnabled()
{
	BOOL result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetAutoScaleEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x38, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiKnobX::GetAutoScaleStyle()
{
	long result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetAutoScaleStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x39, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CiKnobX::Lock()
{
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiKnobX::Unlock()
{
	InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CiKnobX::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiKnobX::GetOuterMargin()
{
	long result;
	InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetOuterMargin(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetUpdateFrameRate()
{
	long result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetUpdateFrameRate(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetComponentHandle()
{
	long result;
	InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SaveImageToBitmap(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiKnobX::SaveImageToMetaFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiKnobX::SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BOOL;
	InvokeHelper(0x41, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Compression, Progressive);
}

BOOL CiKnobX::GetUseKeyboard()
{
	BOOL result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetUseKeyboard(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiKnobX::OPCAddItem()
{
	long result;
	InvokeHelper(0x1001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CiKnobX::OPCItemCount()
{
	long result;
	InvokeHelper(0x1002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::OPCRemoveAllItems()
{
	InvokeHelper(0x1003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiKnobX::OPCItemActivate(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiKnobX::OPCItemDeactivate(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

CString CiKnobX::GetOPCItemComputerName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1006, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiKnobX::GetOPCItemServerName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1007, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiKnobX::GetOPCItemItemName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1008, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiKnobX::GetOPCItemPropertyName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1009, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

BOOL CiKnobX::GetOPCItemAutoConnect(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

long CiKnobX::GetOPCItemUpdateRate(long Index)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Index);
	return result;
}

void CiKnobX::SetOPCItemComputerName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::SetOPCItemServerName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::SetOPCItemItemName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::SetOPCItemPropertyName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::SetOPCItemAutoConnect(long Index, BOOL Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x1010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::SetOPCItemUpdateRate(long Index, long Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::OPCItemLoaded(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

BOOL CiKnobX::GetCachedDrawing()
{
	BOOL result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetCachedDrawing(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiKnobX::GetObjectPointer()
{
	long result;
	InvokeHelper(0x1068, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT CiKnobX::GetBytesJPEG(long Compression, BOOL Progressive)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x1069, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Compression, Progressive);
	return result;
}

void CiKnobX::SavePropertiesToFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x106a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiKnobX::LoadPropertiesFromFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x106b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

BOOL CiKnobX::GetOptionSaveAllProperties()
{
	BOOL result;
	InvokeHelper(0x106d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetOptionSaveAllProperties(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

double CiKnobX::GetMouseWheelStepSize()
{
	double result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetMouseWheelStepSize(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x44, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CiKnobX::GetAutoFrameRate()
{
	BOOL result;
	InvokeHelper(0x106e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetAutoFrameRate(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiKnobX::iPaintToDC(long X, long Y, BOOL Transparent, long DC)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x10cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, Transparent, DC);
}

long CiKnobX::GetWidth()
{
	long result;
	InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiKnobX::GetHeight()
{
	long result;
	InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CiKnobX::OPCItemUpdateResume(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiKnobX::OPCItemUpdateSuspend(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CiKnobX::GetOPCItemData(long Index)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1015, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Index);
	return result;
}

short CiKnobX::GetOPCItemQuality(long Index)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1016, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Index);
	return result;
}

double CiKnobX::GetOPCItemTimeStamp(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1017, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

void CiKnobX::SetOPCItemData(long Index, const VARIANT& Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x1018, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, &Value);
}

BOOL CiKnobX::GetOPCItemActive(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1019, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

CString CiKnobX::GetOPCItemGroupName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

void CiKnobX::SetOPCItemGroupName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x101d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::OPCDeleteItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiKnobX::OPCItemActivateSuspend(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CiKnobX::GetBytesPNG(long Compression)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1020, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Compression);
	return result;
}

void CiKnobX::SaveImageToPNG(LPCTSTR FileName, long Compression)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x1021, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Compression);
}

VARIANT CiKnobX::GetOPCItemDataNow(long Index, short* Quality)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_PI2;
	InvokeHelper(0x1022, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Index, Quality);
	return result;
}

BOOL CiKnobX::GetOPCItemUpdateSuspended(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1023, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

unsigned long CiKnobX::GetColor()
{
	unsigned long result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CiKnobX::GetOPCItemScalar(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1024, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

double CiKnobX::GetOPCItemOffset(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1025, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

void CiKnobX::SetOPCItemScalar(long Index, double Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_R8;
	InvokeHelper(0x1026, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiKnobX::SetOPCItemOffset(long Index, double Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_R8;
	InvokeHelper(0x1027, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

BOOL CiKnobX::GetShowDisabledState()
{
	BOOL result;
	InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiKnobX::SetShowDisabledState(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}
