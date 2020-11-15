#if !defined(__ADAMO_CShowGlobalsSuperGrid__)
#define __ADAMO_CShowGlobalsSuperGrid__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGCards.h : header file
//

#include "supergridctrl.h"

class CShowGlobalsItemInfo : public CItemInfo
{
    public :
        CShowGlobalsItemInfo () : m_gd (NULL)        { }
        ~CShowGlobalsItemInfo ()                     { }
        void SetGD (stGlobalDescriptor* gd)          { m_gd=gd; }
        stGlobalDescriptor* GetGD ()                 { return m_gd; }
        void SetRow (int nRow)                       { m_nRow=nRow; }
        int  GetRow ()                               { return m_nRow; }
        void SetCol (int nCol)                       { m_nCol=nCol; }
        int  GetCol ()                               { return m_nCol; }
    private :
        stGlobalDescriptor* m_gd;
        int                 m_nRow;
        int                 m_nCol;
};

/////////////////////////////////////////////////////////////////////////////
// CTablesSuperGrid view

class CShowGlobalsSuperGrid : public CSuperGridCtrl
{
    public :
	    CShowGlobalsSuperGrid();           // protected constructor used by dynamic creation
        void InitializeGrid ();
        int  GetResults (CString* strName, int *nRow, CString *strColumn, int *nType);
    protected:

    // Attributes
        public:

    // Operations
        public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CTablesSuperGrid)
	    //}}AFX_VIRTUAL

    // Implementation
	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CTablesSuperGrid)
	    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    private :
        CImageList      m_image;
    private :
        void FillColumns ();
        void FillGrid ();
        void FillVariable (stGlobalDescriptor* gd);
        void FillArray (stGlobalDescriptor* gd);
        void FillMatrix (stGlobalDescriptor* gd);
        int  GetSGImage (int n);
        int  GetIcon(const CTreeItem* pItem);
        COLORREF GetCellRGB(void);
        BOOL OnVkReturn(void);
		void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGCARDS_H__8257F340_A369_48F7_8F4E_ABC64D37600D__INCLUDED_)
