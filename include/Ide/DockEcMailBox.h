/*
** DockEcMailBox.h
*/

#ifndef __ADAMO_DOCKECMAILBOX__
#define __ADAMO_DOCKECMAILBOX__

#include "DGEcMailBoxView.h"

class CDockEcMailBox
{
public:
	CDockEcMailBox();
	virtual ~CDockEcMailBox();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    CDGEcMailBoxView *GetView()   { return m_pDGEcMailBoxView; }
protected:
    CDGEcMailBoxView *m_pDGEcMailBoxView;
    CXTPDockingPane *m_pDockingPane;
};

#endif
