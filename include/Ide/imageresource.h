#if !defined(AFX_IMAGERESOURCE_H__D88EFEE1_3D3C_4003_B930_443143B5D8A0__INCLUDED_)
#define AFX_IMAGERESOURCE_H__D88EFEE1_3D3C_4003_B930_443143B5D8A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// imageresource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageResource view

class CAdamoRSDoc;

class CImageResource : public CView
{
protected:
	CImageResource();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CImageResource)

// Attributes
public:
    CString AddRsBimap ();
    bool    DeleteRsBimap (CString strID);
    void    SelectRSBitmap (CString strID);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageResource)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CImageResource();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageResource)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    bool LeggiFile (CPath strPath, char** pBuf, DWORD* dwLength);
    void CenterImage (CRect& rcCentered);
private :
    CAdamoRSDoc* m_pDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGERESOURCE_H__D88EFEE1_3D3C_4003_B930_443143B5D8A0__INCLUDED_)
