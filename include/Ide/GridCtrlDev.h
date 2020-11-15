// GridCtrlDev.h: interface for the CGridCtrlDev class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCTRLDEV_H__A2602BD2_3615_4082_8BBB_669B7BB18C96__INCLUDED_)
#define AFX_GRIDCTRLDEV_H__A2602BD2_3615_4082_8BBB_669B7BB18C96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridDeviceBase.h"

class CGridCtrlDev : public CGridDeviceBase
{
public:
	CGridCtrlDev();
	DECLARE_DYNAMIC(CGridCtrlDev)
	virtual ~CGridCtrlDev();
public:
    virtual void InitView();
    virtual void AddItem(int nIndex,CObject *pObj);
    virtual void DeleteAllItems();
    virtual void Modify(int nRow,CObject *pObj);
protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRIDCTRLDEV_H__A2602BD2_3615_4082_8BBB_669B7BB18C96__INCLUDED_)
