// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "iswitchleverx.h"

/////////////////////////////////////////////////////////////////////////////
// CiSwitchLeverX

IMPLEMENT_DYNCREATE(CiSwitchLeverX, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CiSwitchLeverX properties

/////////////////////////////////////////////////////////////////////////////
// CiSwitchLeverX operations

BOOL CiSwitchLeverX::GetActive()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetActive(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiSwitchLeverX::GetMouseControlStyle()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetMouseControlStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CiSwitchLeverX::GetShowFocusRect()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetShowFocusRect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long CiSwitchLeverX::GetBackGroundColor()
{
	unsigned long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetBackGroundColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void CiSwitchLeverX::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSwitchLeverX::ShowPropertyEditor()
{
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CiSwitchLeverX::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiSwitchLeverX::SetActiveNoEvent(BOOL Value)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

BOOL CiSwitchLeverX::GetTransparent()
{
	BOOL result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetTransparent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiSwitchLeverX::RepaintAll()
{
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSwitchLeverX::BeginUpdate()
{
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSwitchLeverX::EndUpdate()
{
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSwitchLeverX::Lock()
{
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSwitchLeverX::Unlock()
{
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CiSwitchLeverX::GetUpdateFrameRate()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetUpdateFrameRate(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSwitchLeverX::GetComponentHandle()
{
	long result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SaveImageToBitmap(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiSwitchLeverX::SaveImageToMetaFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiSwitchLeverX::SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Compression, Progressive);
}

long CiSwitchLeverX::OPCAddItem()
{
	long result;
	InvokeHelper(0x1001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CiSwitchLeverX::OPCItemCount()
{
	long result;
	InvokeHelper(0x1002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::OPCRemoveAllItems()
{
	InvokeHelper(0x1003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CiSwitchLeverX::OPCItemActivate(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiSwitchLeverX::OPCItemDeactivate(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

CString CiSwitchLeverX::GetOPCItemComputerName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1006, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiSwitchLeverX::GetOPCItemServerName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1007, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiSwitchLeverX::GetOPCItemItemName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1008, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

CString CiSwitchLeverX::GetOPCItemPropertyName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1009, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

BOOL CiSwitchLeverX::GetOPCItemAutoConnect(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

long CiSwitchLeverX::GetOPCItemUpdateRate(long Index)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x100b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Index);
	return result;
}

void CiSwitchLeverX::SetOPCItemComputerName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::SetOPCItemServerName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::SetOPCItemItemName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::SetOPCItemPropertyName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x100f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::SetOPCItemAutoConnect(long Index, BOOL Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x1010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::SetOPCItemUpdateRate(long Index, long Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::OPCItemLoaded(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

long CiSwitchLeverX::GetObjectPointer()
{
	long result;
	InvokeHelper(0x1068, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

VARIANT CiSwitchLeverX::GetBytesJPEG(long Compression, BOOL Progressive)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x1069, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Compression, Progressive);
	return result;
}

void CiSwitchLeverX::SavePropertiesToFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x106a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

void CiSwitchLeverX::LoadPropertiesFromFile(LPCTSTR FileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x106b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName);
}

BOOL CiSwitchLeverX::GetOptionSaveAllProperties()
{
	BOOL result;
	InvokeHelper(0x106d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetOptionSaveAllProperties(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CiSwitchLeverX::GetAutoFrameRate()
{
	BOOL result;
	InvokeHelper(0x106e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetAutoFrameRate(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x106e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CiSwitchLeverX::iPaintToDC(long X, long Y, BOOL Transparent, long DC)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x10cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, Transparent, DC);
}

BOOL CiSwitchLeverX::GetVisible()
{
	BOOL result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CiSwitchLeverX::GetWidth()
{
	long result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CiSwitchLeverX::GetHeight()
{
	long result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CiSwitchLeverX::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CiSwitchLeverX::OPCItemUpdateResume(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiSwitchLeverX::OPCItemUpdateSuspend(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CiSwitchLeverX::GetOPCItemData(long Index)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1015, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Index);
	return result;
}

short CiSwitchLeverX::GetOPCItemQuality(long Index)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1016, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Index);
	return result;
}

double CiSwitchLeverX::GetOPCItemTimeStamp(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1017, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

void CiSwitchLeverX::SetOPCItemData(long Index, const VARIANT& Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0x1018, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, &Value);
}

BOOL CiSwitchLeverX::GetOPCItemActive(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1019, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

CString CiSwitchLeverX::GetOPCItemGroupName(long Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

void CiSwitchLeverX::SetOPCItemGroupName(long Index, LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR;
	InvokeHelper(0x101d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::OPCDeleteItem(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

void CiSwitchLeverX::OPCItemActivateSuspend(long Index)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x101f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index);
}

VARIANT CiSwitchLeverX::GetBytesPNG(long Compression)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1020, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Compression);
	return result;
}

void CiSwitchLeverX::SaveImageToPNG(LPCTSTR FileName, long Compression)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x1021, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Compression);
}

VARIANT CiSwitchLeverX::GetOPCItemDataNow(long Index, short* Quality)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4 VTS_PI2;
	InvokeHelper(0x1022, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Index, Quality);
	return result;
}

BOOL CiSwitchLeverX::GetOPCItemUpdateSuspended(long Index)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1023, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Index);
	return result;
}

double CiSwitchLeverX::GetOPCItemScalar(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1024, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

double CiSwitchLeverX::GetOPCItemOffset(long Index)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1025, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		Index);
	return result;
}

void CiSwitchLeverX::SetOPCItemScalar(long Index, double Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_R8;
	InvokeHelper(0x1026, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}

void CiSwitchLeverX::SetOPCItemOffset(long Index, double Value)
{
	static BYTE parms[] =
		VTS_I4 VTS_R8;
	InvokeHelper(0x1027, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Index, Value);
}