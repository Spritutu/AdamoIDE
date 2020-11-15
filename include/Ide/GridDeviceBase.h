// GridDeviceBase.h: interface for the CGridDeviceBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDDEVICEBASE_H__D3B1C88E_E2C4_431D_81E6_7659831DC05E__INCLUDED_)
#define AFX_GRIDDEVICEBASE_H__D3B1C88E_E2C4_431D_81E6_7659831DC05E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

class CGridDeviceBase : public CGridCtrl  
{
public:
	CGridDeviceBase();
	DECLARE_DYNAMIC(CGridDeviceBase)
	virtual ~CGridDeviceBase();
public:
    virtual void InitView() = 0;
    virtual void AddItem(int nIndex,CObject *pObj) = 0;
    virtual void DeleteAllItems() = 0;
    virtual void Modify(int nRow,CObject *pObj) = 0;
    int GetMaxRow() { return m_nMaxRow; };
    void DeleteItem(CObject *pObj);
protected:
    CImageList m_ImageList;
    int m_nMaxRow;
    void AddBlankLine(int nLine,int nColl);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRIDDEVICEBASE_H__D3B1C88E_E2C4_431D_81E6_7659831DC05E__INCLUDED_)
