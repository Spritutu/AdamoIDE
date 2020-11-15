#if !defined(AFX_ADAMODGCOMM_H__27626160_C561_4942_B0AF_59EE106F6601__INCLUDED_)
#define AFX_ADAMODGCOMM_H__27626160_C561_4942_B0AF_59EE106F6601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGComm.h : header file
//

class CAdamoDGSearch;
class CAdamoDGDocument;

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGCommTrace view


class CAdamoDGCommTrace : public CTreeView
{
public  :
	CAdamoDGCommTrace();           // protected constructor used by dynamic creation
	~CAdamoDGCommTrace();
	DECLARE_DYNCREATE(CAdamoDGCommTrace)

// Attributes
public:
    void OnCloseDGView ();
    int  FindNext (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase);
    int  FindPrev (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGCommTrace)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDGCommTrace)
	afx_msg void OnDGStartTrace();
	afx_msg void OnDGStopTrace();
	afx_msg void OnUpdateDGStartTrace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDGStopTrace(CCmdUI* pCmdUI);
	afx_msg void OnDGRefresh();
	afx_msg void OnDestroy();
	afx_msg void OnDGClearFilter();
	afx_msg void OnUpdateDGClearFilter(CCmdUI* pCmdUI);
	afx_msg void OnDGFilterErrors();
	afx_msg void OnUpdateDGFilterErrors(CCmdUI* pCmdUI);
	afx_msg void OnDGFind();
	afx_msg void OnUpdateDGFind(CCmdUI* pCmdUI);
	afx_msg void OnDGInterpreta();
	afx_msg void OnUpdateDGInterpreta(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDGRefresh(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
    void OnCloseSearchDialog ();
    int  Trovato (HTREEITEM ht, const char* str, bool bMatchWholeWord, bool bUpperCase);
    HTREEITEM FindLastElement ();
private :

    CAdamoDGDocument* m_pDoc;
    CFont* m_fnt;
    CBitmap     m_bmpTree;
    CImageList  m_imgList;
    bool    m_bEnabled;
    CAdamoDGSearch* m_pDlgSearch;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGCOMM_H__27626160_C561_4942_B0AF_59EE106F6601__INCLUDED_)
