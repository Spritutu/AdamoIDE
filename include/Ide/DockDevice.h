// DockDevice.h: interface for the CDockDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKDEVICE_H__010AF968_B9E5_48C7_9F18_A4123ECEE025__INCLUDED_)
#define AFX_DOCKDEVICE_H__010AF968_B9E5_48C7_9F18_A4123ECEE025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DockDevListView.h"

class CDockDevice  
{
public:
    CDockDevice();
	virtual ~CDockDevice();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane){m_pDockingPane = pPane;};
    CXTPDockingPane *GetPtrDockingPane(){return m_pDockingPane;};
    CDockDevListView *GetView(){return m_pDevListView;};
protected:
    CDockDevListView *m_pDevListView;
    CXTPDockingPane *m_pDockingPane;
};

#endif // !defined(AFX_DOCKDEVICE_H__010AF968_B9E5_48C7_9F18_A4123ECEE025__INCLUDED_)
