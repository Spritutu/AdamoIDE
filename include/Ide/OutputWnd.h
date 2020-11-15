// OutputWnd.h: interface for the COutputWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTWND_H__804E57BF_EABF_48F9_B600_90503B44A217__INCLUDED_)
#define AFX_OUTPUTWND_H__804E57BF_EABF_48F9_B600_90503B44A217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OutputView.h"
struct SCNotification;

#include "ScintillaBar.h"

class COutputWnd : public CScintillaBar
{
public:
	void Init(CWnd *pWnd, int nId);
    COutputWnd();
    virtual ~COutputWnd ()   {};
	void Write (CString strMsg);
	void Clear ();
    virtual void GotoLine (CString strLine) = 0;
    CScintillaView *GetSciView ()    { return m_pSciView; }
	virtual int OnSci (CWnd* pView, SCNotification* pNotify);
protected:
    CScintillaView *m_pSciView;
};

class CErrorWnd : public COutputWnd
{
public :
    CErrorWnd () : m_nCurErrorLine (0)   { }
    void Clear ();
	void GotoLine (CString strLine);
    void GotoNextError ();
    void GotoPrevError ();
protected :
    int m_nCurErrorLine;
private :
    bool IsValidErrLine (const char* sz);
};

class CDebugWnd : public COutputWnd
{
public :
    CDebugWnd () : m_nCurErrorLine (0)   { }
    void Clear ();
	void GotoLine (CString strLine);
    void GotoNextError ();
    void GotoPrevError ();
protected :
    int m_nCurErrorLine;
};

#endif // !defined(AFX_OUTPUTWND_H__804E57BF_EABF_48F9_B600_90503B44A217__INCLUDED_)
