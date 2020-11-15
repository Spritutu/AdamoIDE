// WatchBar.h: interface for the CWatchBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATCHBAR_H__B3588692_9C51_457B_B360_FCF2CF7336A6__INCLUDED_)
#define AFX_WATCHBAR_H__B3588692_9C51_457B_B360_FCF2CF7336A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WatchListView.h"

class CWatchBar
{
public:
	void Init(CWnd *pWnd, int nId);
    void Redraw();
    CWatchBar():m_pWatchListView(NULL){};
    virtual ~CWatchBar(){};
    void SetPtrDockingPane(CXTPDockingPane *pPane){m_pDockingPane = pPane;};
    CXTPDockingPane *GetPtrDockingPane(){return m_pDockingPane;};
    CWatchListView *GetWatchListView(){return m_pWatchListView;};
protected:
    CWatchListView *m_pWatchListView;
    CXTPDockingPane *m_pDockingPane;
};

#endif // !defined(AFX_WATCHBAR_H__B3588692_9C51_457B_B360_FCF2CF7336A6__INCLUDED_)
