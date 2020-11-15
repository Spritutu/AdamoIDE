#if !defined(AFX_ADAMOSSCNETGRID_H__4952E2F8_B78A_41EF_A53F_4D71CD2F65FA__INCLUDED_)
#define AFX_ADAMOSSCNETGRID_H__4952E2F8_B78A_41EF_A53F_4D71CD2F65FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamosscnetgrid.h : header file
//

#include "supergridctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoSSCNETGrid window

class CAdamoSSCNETItemInfo : public CItemInfo
{
    public :
        CAdamoSSCNETItemInfo () : m_pMachine (NULL), m_wAlarm (0), m_dwDisplay (0), m_bInAlarm (false) {} 
        void SetDisplay (DWORD dwStatus);
        void SetStatus (DWORD dwStatus);
        void SetMachine (CAdamoMachine *pMachine)   { m_pMachine=pMachine; }
        void SetCard (int nCard)                    { m_nCard=nCard; }
        void SetStation (int nStation)              { m_nStation=nStation; }
        CString GetStatus ()                          { return m_strStatus; }
        CString GetAlarm ()                         { return m_strAlarm; }
		bool IsInAlarm ()							{ return m_bInAlarm; }
    private :
        CAdamoMachine *m_pMachine;
        int     m_nCard;
        int     m_nStation;
		DWORD   m_dwStatus;
		DWORD   m_dwDisplay;
        WORD    m_wAlarm;
        CString m_strStatus;
        CString m_strAlarm;
		bool    m_bInAlarm;
};

class CAdamoSSCNETGrid : public CSuperGridCtrl
{
// Construction
public:
	CAdamoSSCNETGrid();
    void InitializeGrid ();
    void RinfrescaInformazioni ();
    void InizializzaInformazioni ();
    int  GetCurrentStation ();
    void SetCard (int nCard)        { m_nCard = nCard; }

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSSCNETGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdamoSSCNETGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoSSCNETGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void FillColumns ();
    void FillAxesArray ();
    void SetMachine ();
    void AddSSCNETStation (int nStation, DWORD dwStatus, DWORD dwDisplay);
    void OnPaintColumn (LPDRAWITEMSTRUCT lpDrawItemStruct, int nColumn, CRect* rc); 
    void DrawStatus (CString strStatus, bool bInAlarm, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc);
    void DrawAlarm  (CString strAlarm, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc);
    void OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
    int  GetIcon(const CTreeItem* pItem);
    COLORREF GetCellRGB(void);
	BOOL OnVkReturn(void);
private :
    CAdamoMachine*  m_pMachine;
    CStringArray    m_aAxes;
    CImageList      m_image;
    int             m_nCount;
    int             m_nCard;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSSCNETGRID_H__4952E2F8_B78A_41EF_A53F_4D71CD2F65FA__INCLUDED_)
