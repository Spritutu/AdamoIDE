// LuaFrame.h : interface of the CLuaFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LUAFRAME_H__5C37E669_4D8A_474B_88BD_43C4D9FB071D__INCLUDED_)
#define AFX_LUAFRAME_H__5C37E669_4D8A_474B_88BD_43C4D9FB071D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLuaFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLuaFrame)
public:
	CLuaFrame();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLuaFrame();
    static CLuaFrame* GetLuaFrame ()   { return m_pThis; }
    void   OnTimer ();
    void   OnClose ();
	CXTWindowPos m_wndPosition;
	CString      m_strWindowPos;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CLuaFrame)
                afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    static CLuaFrame* m_pThis;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAFRAME_H__5C37E669_4D8A_474B_88BD_43C4D9FB071D__INCLUDED_)
