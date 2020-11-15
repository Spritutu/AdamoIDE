// WorkspaceWnd.h: interface for the CWorkspaceWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEWND_H__1609FA7C_19A2_49B5_A86E_65E866932D97__INCLUDED_)
#define AFX_WORKSPACEWND_H__1609FA7C_19A2_49B5_A86E_65E866932D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeViewFiles.h"

class CWorkspaceWnd
{
public:
	void Init(CWnd *pWnd, int nId);
    CWorkspaceWnd();
    virtual ~CWorkspaceWnd(){};
	void Enable(BOOL bEnable);
	CTreeViewFiles* GetTreeViewFiles();
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()             { return m_pDockingPane; }
protected:
    CXTPDockingPane *m_pDockingPane;
    CTreeViewFiles *m_pTreeView;
};

#endif // !defined(AFX_WORKSPACEWND_H__1609FA7C_19A2_49B5_A86E_65E866932D97__INCLUDED_)
