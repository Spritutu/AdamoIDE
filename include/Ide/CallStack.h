// CallStack.h: interface for the CCallStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLSTACK_H__1BC34BF3_8062_4E58_AAEC_8DA50AB351A2__INCLUDED_)
#define AFX_CALLSTACK_H__1BC34BF3_8062_4E58_AAEC_8DA50AB351A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct SCNotification;

#include "ScintillaView.h"
#include "ScintillaBar.h"

class CCallStack : public CScintillaBar
{
public:
	void Init(CWnd *pWnd, int nId);
    CCallStack():m_pSciView(NULL){};
    virtual ~CCallStack(){};
	void GotoStackTraceLevel(int nLevel);
	void Add(const char* szDesc, const char* szFile, int nLine);
	void Clear();
	virtual int OnSci(CWnd* pView, SCNotification* pNotify);
	int GetLevel() { return m_nCurrentLevel; };
    CScintillaView *GetSciView(){return m_pSciView;};
protected:
	int m_nCurrentLevel;
	CUIntArray	m_levels, m_lines;
	CStringArray m_files;
    CScintillaView *m_pSciView;
};

#endif // !defined(AFX_CALLSTACK_H__1BC34BF3_8062_4E58_AAEC_8DA50AB351A2__INCLUDED_)
