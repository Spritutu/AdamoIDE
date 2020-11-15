// DockLangView.h: interface for the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "HelpOnLineHtmlView.h"

class CDockHelpOnLineHtml
{
// Construction
public:
	CDockHelpOnLineHtml();
	virtual ~CDockHelpOnLineHtml();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CHtmlHelpOnLineView *GetView()                  { return m_pHelpOnLineHtmlView; }
protected:
    CHtmlHelpOnLineView *m_pHelpOnLineHtmlView;
    CXTPDockingPane *m_pDockingPane;
};
