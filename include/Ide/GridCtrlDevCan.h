// GridCtrlDevCan.h: interface for the CGridCtrlDevCan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCTRLDEVCAN_H__39EBB24D_ED0F_4A7F_83E4_137C6C93FE6E__INCLUDED_)
#define AFX_GRIDCTRLDEVCAN_H__39EBB24D_ED0F_4A7F_83E4_137C6C93FE6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridDeviceBase.h"

class CGridCtrlDevCan : public CGridDeviceBase
{
public:
	CGridCtrlDevCan();
	DECLARE_DYNAMIC(CGridCtrlDevCan)
	virtual ~CGridCtrlDevCan();

public:
	virtual void InitView();
    virtual void AddItem(int nIndex,CObject *pObj);
    virtual void DeleteAllItems();
    virtual void Modify(int nRow,CObject *pObj);
    CString GetStrDataType(int nType);
protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRIDCTRLDEVCAN_H__39EBB24D_ED0F_4A7F_83E4_137C6C93FE6E__INCLUDED_)
