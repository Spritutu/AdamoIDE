#if !defined(AFX_ADAMODGDOCUMENT_H__97FCDACB_BD9A_4B45_83A7_4E7F24BF63C2__INCLUDED_)
#define AFX_ADAMODGDOCUMENT_H__97FCDACB_BD9A_4B45_83A7_4E7F24BF63C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGDocument.h : header file
//

#define NMAX_TRACECOMMRECORDS   10240
#define NMAX_FILTERTYPES        5
#define NMAX_COMMANDS           59
#define TYPE_DOWNLOAD           0
#define TYPE_COMMAND            1
#define TYPE_REQUEST            2
#define TYPE_ANSWER_REQUEST     3
#define TYPE_RECEIVE            4
#define TYPE_DATA               5

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDocument document

class CAdamoDGTraceCommFile : public CFile  {
        long m_f;
        long m_n;
        long m_fp[NMAX_TRACECOMMRECORDS];
        int  m_nFilterDown;
        int  m_nFilterUp;
        int  m_anFilterTypes[NMAX_FILTERTYPES];
    public :
        CAdamoDGTraceCommFile () : m_f (0), m_n (0), m_nFilterDown (-1), m_nFilterUp (-1) 
            { for (int i=0; i<NMAX_FILTERTYPES; i++) m_anFilterTypes[i]=1; }
        int UpdateTree (CTreeView* tv);
        int ExplodeTree (CTreeView* tv, HTREEITEM ht);
        void SetFilters (CTreeView* tv, int nFilterDown, int nFilterUp, int* anFilterTypes);
        void GetFilters (int* nFilterDown, int* nFilterUp, int* anFilterTypes);
        void ResetFilters ();
        void Reset ()                       { m_f=0; m_n=0; }
    private :
        CString GetPathName ();
        CString FormatItem (time_t* t, CAdamoCommHeader* ach, char* pBuffer);
        int  GetCommandType   (CAdamoCommHeader* ach, int* nType);
        int  GetCommandImage  (int nType, int* nImage);
        int  GetCommandString (int nCmd, CString& strCmd);
        bool FiltersOk (int nCmd, int nType);
};

class CAdamoDGDocument : public CDocument
{
protected:
	CAdamoDGDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDGDocument)

// Attributes
public:

// Operations
public:
    int  UpdateTree  (CTreeView* tv)                        { return m_tcf.UpdateTree (tv); }
    int  ExplodeTree (CTreeView* tv, HTREEITEM ht)          { return m_tcf.ExplodeTree (tv, ht); }
    void Reset ()                           { m_tcf.Reset ();               }
    void SetFilters (CTreeView* tv, int nFilterDown, int nFilterUp, int* anFilterTypes)     { m_tcf.SetFilters (tv, nFilterDown, nFilterUp, anFilterTypes); }
    void GetFilters (int* nFilterDown, int* nFilterUp, int* anFilterTypes)   { m_tcf.GetFilters (nFilterDown, nFilterUp, anFilterTypes); }
    void ResetFilters ()                                                     { m_tcf.ResetFilters (); }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdamoDGDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDGDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoDGTraceCommFile m_tcf;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGDOCUMENT_H__97FCDACB_BD9A_4B45_83A7_4E7F24BF63C2__INCLUDED_)
