#if !defined(__ADAMO_CADAMORSEDITINDEXSUPERGRID__)
#define __ADAMO_CADAMORSEDITINDEXSUPERGRID__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGCards.h : header file
//

#include "supergridctrl.h"

class CAdamoRSEditTableIndex;

/////////////////////////////////////////////////////////////////////////////
// CTablesSuperGrid view

class CAdamoRSEditIndexSuperGrid : public CSuperGridCtrl
{
    public :
	    CAdamoRSEditIndexSuperGrid();           // protected constructor used by dynamic creation
        void InitializeGrid ();
        void SetDBData (CString strTable, CString strIndex, CString strIndexValue)   { m_strTable=strTable; m_strIndex=strIndex; m_strIndexValue=strIndexValue; }
        CString  GetResults ();
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
        CTreeItem*      m_pRootItem;
        CImageList      m_image;
        CString         m_strNomeFile;
        CString         m_strTable;
        CString         m_strIndex;
        CString         m_strIndexValue;
    private :
        void FillColumns ();
        void FillGrid ();
        void OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
        int  GetIcon(const CTreeItem* pItem);
        COLORREF GetCellRGB(void);
        BOOL OnVkReturn(void);
        int  GetSubIndexList (CString strTable, CString strIndex, CStringList& slSubIndex);
        void GetSubValueList (CString strIndexValue, CStringList& sl);
        void ScomponiInStringhe (CStringArray& ar, CString str, char ch);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGCARDS_H__8257F340_A369_48F7_8F4E_ABC64D37600D__INCLUDED_)
