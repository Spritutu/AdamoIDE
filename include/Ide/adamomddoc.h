#if !defined(AFX_ADAMOMDDOC_H__5DF88291_D9DA_4420_A66E_59976F5A0916__INCLUDED_)
#define AFX_ADAMOMDDOC_H__5DF88291_D9DA_4420_A66E_59976F5A0916__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDDoc.h : header file
//

/* include files */
#include "adamo.h"
#include "adamoint.h"
#include "adamocomm.h"
#include "adamomachine.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDDoc document

class CAdamoMDDoc : public CDocument
{
protected:
	CAdamoMDDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoMDDoc)

// Attributes
public:
    void SetMachine (CAdamoMachine* pMachine)   { m_pMachine=pMachine; }
    CAdamoMachine* GetMachine ()   { return m_pMachine; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdamoMDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoMDDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoMachine* m_pMachine;
    CAdamoMachineSettings* m_pSettings;
private :
    void InitSettings ();
    void SaveSettings ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDDOC_H__5DF88291_D9DA_4420_A66E_59976F5A0916__INCLUDED_)
