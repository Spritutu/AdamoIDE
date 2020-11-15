// DockObjects.h: interface for the CDockObjects class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "ThiraLuaObjectsView.h"

class CDockThiraLuaObjects
{
// Construction
public:
	CDockThiraLuaObjects();
	virtual ~CDockThiraLuaObjects();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CThiraLuaObjectsView *GetView()                     { return m_pThiraLuaObjectsView; }
protected:
    CThiraLuaObjectsView *m_pThiraLuaObjectsView;
    CXTPDockingPane *m_pDockingPane;
};
