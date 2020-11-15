// DockLangView.h: interface for the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCKLANGVIEW_H__799B3A47_6D23_4BF9_9334_063410E39E39__INCLUDED_)
#define AFX_DOCKLANGVIEW_H__799B3A47_6D23_4BF9_9334_063410E39E39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LangView.h"

class CDockLangView
{
// Construction
public:
	CDockLangView();
	virtual ~CDockLangView();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CLangView *GetView()                            { return m_pLangView; }
protected:
    CLangView *m_pLangView;
    CXTPDockingPane *m_pDockingPane;
};

#endif // !defined(AFX_DOCKLANGVIEW_H__799B3A47_6D23_4BF9_9334_063410E39E39__INCLUDED_)
