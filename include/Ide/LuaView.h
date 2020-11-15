// LuaView.h : interface of the CLuaView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LUAVIEW_H__F7E3BD0D_1558_484D_9634_B138CC9D9B31__INCLUDED_)
#define AFX_LUAVIEW_H__F7E3BD0D_1558_484D_9634_B138CC9D9B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LuaEditor.h"

class CLuaView : public CView
{
protected: // create from serialization only
	CLuaView();
	DECLARE_DYNCREATE(CLuaView)

// Attributes
public:
	CLuaDoc* GetDocument();
	CLuaEditor* GetEditor() { return &m_editor; };
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Operations
	CLuaView* GotoDefinition (CString strSymbol);
	void GotoReference (CString strSymbol);
	void GotoHelpSymbol ();
	CLuaView* GotoNextReference ();
	CLuaView* GotoPrevReference ();
	int  GetReferenceData (stBrowserDescriptor** pReferenceData);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ToggleBreakPoint(int nLine);
	void CloseFrame();
	void Activate();
	virtual ~CLuaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CLuaEditor m_editor;
	int OnSci(SCNotification* pNotify);
// Generated message map functions
protected:
	//{{AFX_MSG(CLuaView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnToggleBreakpoint();
	afx_msg void OnUpdateToggleBreakpoint(CCmdUI* pCmdUI);
	afx_msg void OnEditCompleteword();
	afx_msg void OnEditShowEnums();
	afx_msg void OnEditShowParams();
   	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnBookmarkToggle();
	afx_msg void OnBookmarkNext();
	afx_msg void OnBookmarkPrev();
	afx_msg void OnBookmarkClearall();
	afx_msg void OnCommentToggle();
	afx_msg void OnCommentBlock();
	afx_msg void OnUncommentBlock();
	afx_msg void OnMatchBrace ();
	afx_msg void OnSelectMatchBrace ();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditFoldAll();
	afx_msg void OnEditUnfoldAll();
private :
	int FunzioniCancellate (CString strText, CStringList& ListaFunzioni);
};

#ifndef _DEBUG  // debug version in LuaView.cpp
inline CLuaDoc* CLuaView::GetDocument()
   { return (CLuaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAVIEW_H__F7E3BD0D_1558_484D_9634_B138CC9D9B31__INCLUDED_)
