// ScintillaBar.h: interface for the CScintillaBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCINTILLABAR_H__377FAA54_BEAC_412F_8A77_86A79CBF577C__INCLUDED_)
#define AFX_SCINTILLABAR_H__377FAA54_BEAC_412F_8A77_86A79CBF577C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScintillaView;
struct SCNotification;

class CScintillaBar
{
public:
    CScintillaBar(){};
    virtual ~CScintillaBar(){};
	virtual int OnSci(CWnd* pView, SCNotification* pNotify) = 0;
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
protected:
    CXTPDockingPane *m_pDockingPane;
};

#endif // !defined(AFX_SCINTILLABAR_H__377FAA54_BEAC_412F_8A77_86A79CBF577C__INCLUDED_)
