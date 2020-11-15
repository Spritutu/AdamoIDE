#if !defined(AFX_OLEDROPSOURCEEX_H__14E24D20_0894_4474_B58A_C79E3EB98F6B__INCLUDED_)
#define AFX_OLEDROPSOURCEEX_H__14E24D20_0894_4474_B58A_C79E3EB98F6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OleDropSourceEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COleDropSourceEx document

class COleDropSourceEx : public COleDropSource
{
// Attributes
public:
	COleDropSourceEx();           // protected constructor used by dynamic creation

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COleDropSourceEx)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual SCODE GiveFeedback(DROPEFFECT dropEffect);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COleDropSourceEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLEDROPSOURCEEX_H__14E24D20_0894_4474_B58A_C79E3EB98F6B__INCLUDED_)
