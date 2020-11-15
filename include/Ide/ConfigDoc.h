#if !defined(AFX_CONFIGDOC_H__4B35E33A_2398_471C_93B3_DA97E0B0598B__INCLUDED_)
#define AFX_CONFIGDOC_H__4B35E33A_2398_471C_93B3_DA97E0B0598B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDoc document

class CConfigDoc : public CDocument
{
protected:
	CConfigDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConfigDoc)

// Attributes
public:
    void OnSaveConfiguration ();
    //void LoadHWConfig ();
    void SaveHWConfig ();
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
  	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConfigDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CConfigDoc)
	afx_msg void OnSaveMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void CopyHWFiles (CProject *pPrj);
    void CopyCCFiles (CProject *pPrj);
    void RestoreHWFiles (CProject *pPrj);
    void DeleteHWFiles (CProject *pPrj);
    void DeleteCCFiles (CProject *pPrj);
    bool AskToSave (CProject *pPrj);
    void RemoveNewCCFiles (CProject *pPrj);
    void RestoreCCFiles (CProject *pPrj);
    void FlushData ();
	void SaveModConfig (CProject *pPrj);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDOC_H__4B35E33A_2398_471C_93B3_DA97E0B0598B__INCLUDED_)
