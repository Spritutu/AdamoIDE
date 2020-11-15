// DockConfigTree.h: interface for the CDockConfigTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKCONFIGTREE_H__88D77AA5_20C3_4357_97E9_9B72606B74C6__INCLUDED_)
#define AFX_DOCKCONFIGTREE_H__88D77AA5_20C3_4357_97E9_9B72606B74C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfTreeView.h"

class CDockConfigTree  
{
public:
	void Enable(BOOL bEnable);
	CDockConfigTree();
	virtual ~CDockConfigTree();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane){m_pDockingPane = pPane;};
    CXTPDockingPane *GetPtrDockingPane(){return m_pDockingPane;};
    CConfTreeView *GetView(){return m_pTreeView;};
protected:
    CConfTreeView *m_pTreeView;
    CXTPDockingPane *m_pDockingPane;

};

#endif // !defined(AFX_DOCKCONFIGTREE_H__88D77AA5_20C3_4357_97E9_9B72606B74C6__INCLUDED_)
