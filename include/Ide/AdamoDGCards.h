#if !defined(AFX_ADAMODGCARDS_H__8257F340_A369_48F7_8F4E_ABC64D37600D__INCLUDED_)
#define AFX_ADAMODGCARDS_H__8257F340_A369_48F7_8F4E_ABC64D37600D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGCards.h : header file
//
#include "supergridctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGCards view

class CDGCardsItemInfo : public CItemInfo
{
    public :
        CDGCardsItemInfo () : m_pCI (NULL)   { m_pCI=new CCmdGetCardInfo; memset (m_pCI, 0, sizeof (CCmdGetCardInfo)); } 
        ~CDGCardsItemInfo ()   { SAFE_DELETE (m_pCI); } 
        void SetCI (CCmdGetCardInfo* pCI)   { *m_pCI=*pCI; } 
        CCmdGetCardInfo* GetCI ()            { return m_pCI; } 
    private :
        CCmdGetCardInfo* m_pCI;
};

class CAdamoDGCards : public CSuperGridCtrl
{
    public :
	    CAdamoDGCards();           // protected constructor used by dynamic creation
        void RinfrescaInformazioni ();
        void InitializeGrid ();
		void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
    protected:

    // Attributes
        public:

    // Operations
        public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CAdamoDGCards)
	    //}}AFX_VIRTUAL

    // Implementation
	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CAdamoDGCards)
	    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    private :
        void FillColumns ();
        CString GetCardName (CCmdGetCardInfo* pCI);
        CString GetCardID (CCmdGetCardInfo* pCI);
        CString GetPosizionePCI (CCmdGetCardInfo* pCI);
        CString GetCardVersion (CCmdGetCardInfo* pCI);
        CString GetCardDate (CCmdGetCardInfo* pCI);
        void    OnPaintColumn (LPDRAWITEMSTRUCT lpDrawItemStruct, int nColumn, CRect* rc); 
        void    Disegna24Volt (int nPresenza24Volt, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc);
        void    DisegnaContatore (int nContatore, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc);
        void    SetMachine ();
        void    OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
        int     GetIcon(const CTreeItem* pItem);
        COLORREF GetCellRGB(void);
        void    AddCardLine (CCmdGetCardInfo *pCI);
    private :
        CAdamoMachine*  m_pMachine;
        CImageList      m_image;
        int             m_nCount;
        CBitmap         m_bmp24VoltOn;
        CBitmap         m_bmp24VoltOff;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGCARDS_H__8257F340_A369_48F7_8F4E_ABC64D37600D__INCLUDED_)
