// DockLangView.cpp: implementation of the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mainframe.h"
#include "DockThiraLuaObjects.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockLangView

CDockThiraLuaObjects::CDockThiraLuaObjects() : m_pThiraLuaObjectsView (NULL)
{
}

CDockThiraLuaObjects::~CDockThiraLuaObjects()
{
}

void CDockThiraLuaObjects::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);

	if (!pPane)   {
		pPane=((CMainFrame *)AfxGetMainWnd ())->GetPaneManager ()->CreatePane(ID_VIEW_THIRA_LUA_OBJECTS, CRect(0, 0,200, 120), xtpPaneDockRight);
		m_pThiraLuaObjectsView = (CThiraLuaObjectsView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CThiraLuaObjectsView));
	}
	else
		m_pThiraLuaObjectsView = (CThiraLuaObjectsView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CThiraLuaObjectsView));
}

