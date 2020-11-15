// DockLangView.h: interface for the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "HelpOnLineView.h"

class CDockHelpOnLine
{
// Construction
public:
	CDockHelpOnLine();
	virtual ~CDockHelpOnLine();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CHelpOnLineView *GetView()                      { return m_pHelpOnLineView; }
protected:
    CHelpOnLineView *m_pHelpOnLineView;
    CXTPDockingPane *m_pDockingPane;
};
