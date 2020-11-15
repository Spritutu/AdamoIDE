#if !defined(AFX_ADAMOSELISTVIEW_H__9FC6F8A2_3F80_4572_9F88_115DC01C009E__INCLUDED_)
#define AFX_ADAMOSELISTVIEW_H__9FC6F8A2_3F80_4572_9F88_115DC01C009E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoSEListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEListView view

class CAdamoSEDoc;
class CAdamoSEView;

class CAdamoSEHeaderListCtrl : public CHeaderCtrl
{
	public :
		CAdamoSEHeaderListCtrl ();
		DECLARE_MESSAGE_MAP ();
		void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	protected :
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	public:
		LRESULT SetSkin (AppTheme nSkin);
	private :
		COLORREF m_foreColor;
		COLORREF m_bckColor;
		bool m_bFirstTime;
};

class CAdamoSEListCtrl : public CXTPListCtrl
{
	public :
		CAdamoSEListCtrl () : m_nHighlight (HIGHLIGHT_ROW)   {
		}
		//{{AFX_MSG(CAdamoSEListCtrl)
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		CAdamoSEHeaderListCtrl& GetSEHeaderCtrl ()	{ return m_HeaderCtrl; }
	protected :
		void DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct);
virtual void PreSubclassWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	public :
		enum EHighlight { HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW };
		LRESULT SetSkin (AppTheme nSkin);
	private :
		CAdamoSEHeaderListCtrl m_HeaderCtrl;
		int m_nHighlight;
		COLORREF m_foreColor;
		COLORREF m_bckColor;
		COLORREF m_foreSelectionColor;
		COLORREF m_bckSelectionColor;
};

class CAdamoSEListView : public CView
{
protected:
	CAdamoSEListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoSEListView)

// Attributes
public:
    void SetAdamoSEDoc (CAdamoSEDoc* pDoc)   { m_pSEDoc=pDoc; }
    void SetAdamoSEView (CAdamoSEView* pView)   { m_pSEView=pView; }
    void SetErrorType (int nErrorType)       { m_nErrorType=nErrorType; }
    void CreateColumnHeader ();
    int  GetErrorType ()                     { return m_nErrorType; }
    int  GetSelection ()                     { return m_list.GetNextItem (-1, LVNI_SELECTED); }
    void SetSelection (int nItem)            { m_list.SetItemState (nItem, LVIS_SELECTED|LVIS_FOCUSED, 0x000f); m_list.EnsureVisible (nItem, FALSE); }
	void AddError (stAdamoErrore *p, int nItem);
	void GotoLine ();
	void DeleteLVParam ();
	void ClearAllItems ();
	CAdamoSEListCtrl& GetListCtrl ()		 { return m_list; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSEListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoSEListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoSEListView)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateSeFind(CCmdUI* pCmdUI);
	afx_msg void OnSeFind();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CAdamoSEListCtrl m_list;
	int m_listID;
	static int m_listGID;
    CAdamoSEDoc* m_pSEDoc;
    CAdamoSEView* m_pSEView;
    int m_nErrorType;
    CImageList* m_lviml;
    CImageList* m_hciml;
    CBitmap*  m_bmpHdrTime;
    CBitmap*  m_bmpHdrEvent;
    CBitmap*  m_bmpHdrDate;
    CBitmap*  m_bmpHdrError;
    CBitmap*  m_bmpHdrTask;
    CBitmap*  m_bmpHdrPath;
    CBitmap*  m_bmpHdrLine;
    CBitmap*  m_bmpHdrDevice;
    CBitmap*  m_bmpHdrDeviceType;
    CBitmap*  m_bmpHdrIdentifier;
    CBitmap*  m_bmpHdrPriority;
    CBitmap*  m_bmpHdrDisk;
    CBitmap*  m_bmpErrHardWare;
    CBitmap*  m_bmpErrInterpreter;
    CBitmap*  m_bmpErrCycle;
    CBitmap*  m_bmpErrISO;
    int       m_nItem;
    CString   m_strModule;
    CString   m_strLine;
    CString   m_strError;
private :
    void DeleteHeaderResource ();
    void LoadHeaderHardware ();
    void LoadHeaderInterpreter ();
    void LoadHeaderCycle ();
    void LoadHeaderISO ();
    void LoadHeaderEvents ();
    void LoadAllErrors ();
    void FillHardware (stAdamoErrore* pE, int nItem);
    void FillInterpreter (stAdamoErrore* pE, int nItem);
    void FillCycle (stAdamoErrore* pE, int nItem);
    void FillISO (stAdamoErrore* pE, int nItem);
    void FillEvent (stAdamoErrore* pE, int nItem);
    void FillAll (stAdamoErrore* pE, int nItem);
    void GestisciSENomeDelDispositivo (stAdamoErrore* pE, char* szBuffer);
    void GestisciSETipoDelDispositivo (stAdamoErrore* pE, char* szBuffer);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSELISTVIEW_H__9FC6F8A2_3F80_4572_9F88_115DC01C009E__INCLUDED_)
