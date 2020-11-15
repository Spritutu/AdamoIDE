#if !defined(AFX_ADAMOSEDOC_H__E5D653CD_9A89_486C_90B3_D6A33514C87B__INCLUDED_)
#define AFX_ADAMOSEDOC_H__E5D653CD_9A89_486C_90B3_D6A33514C87B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoSEDoc.h : header file
//

/* include files */
#include "AdamoErr.h"

/* constants definitions */
#define NMAXCACHE    128
#define NMAXLOGCHUNK 1024
#define TE_ALL -1
#define ADAMOERRORS_FRAME 20

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEDoc document

class CAdamoSEDoc : public CDocument
{
protected:
	CAdamoSEDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoSEDoc)

// Attributes
public :
    int  OpenLogFile (int nMonth);
	int  ReadLogFile (int nDA, int nA, BYTE *pAdamoErrore);
    void CloseLogFile ();
    int  GetMonth ()                     { return m_nMonth; }
    void OpenAdamoDatiFile ();
    void CloseAdamoDatiFile ();
    int  GetDeviceName (int nDevice, int nTypeDevice, char* szBuffer);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSEDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdamoSEDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoSEDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CAdamoMachine* m_pMachine;
    CAdamoDatiFile* m_pADF;
    int m_nMonth;
	unsigned m_nRecord;
private :
	void SetMachine ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSEDOC_H__E5D653CD_9A89_486C_90B3_D6A33514C87B__INCLUDED_)
