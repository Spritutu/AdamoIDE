// DockDevProperty.h: interface for the CDockDevProperty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKDEVPROPERTY_H__5A714FE0_B3B5_4869_A43E_E82B796CC53A__INCLUDED_)
#define AFX_DOCKDEVPROPERTY_H__5A714FE0_B3B5_4869_A43E_E82B796CC53A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridView.h"

class CDockDevProperty  
{
public:
	CDockDevProperty();
	virtual ~CDockDevProperty();
	void SetObject(CObject *pObj);
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane){m_pDockingPane = pPane;};
    CXTPDockingPane *GetPtrDockingPane(){return m_pDockingPane;};
    CGridView *GetView(){return m_pGridView;};
protected:
    CGridView *m_pGridView;
    CXTPDockingPane *m_pDockingPane;

};

#endif // !defined(AFX_DOCKDEVPROPERTY_H__5A714FE0_B3B5_4869_A43E_E82B796CC53A__INCLUDED_)
