#if !defined(AFX_ADAMOMDAXESVIEW_H__90751285_5B31_4580_88D7_6C54EF71E3CC__INCLUDED_)
#define AFX_ADAMOMDAXESVIEW_H__90751285_5B31_4580_88D7_6C54EF71E3CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDAxesView.h : header file
//

#include "AllAxesView.h"
#include "AdamoMDCommandBar.h"
#include "AdamoMDAxSettings.h"
#include "AdamoMDAxStatus.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxesView view

class CAdamoMDAxesView : public CView
{
protected:
	CAdamoMDAxesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoMDAxesView)

// Attributes
public:
    static CAdamoMDAxesView* GetAdamoMDAxesView ()   { return m_pThis; }
    CAdamoMDCommandBar*      GetCommandBar ()        { return m_pCmdBar; }
    CAdamoMDAxStatus*        GetAxStatus ()          { return m_pAllAxesView; }
    void                     OnViewSettings (stMDSettings* MDS);
    void                     GetViewSettings (stMDSettings* MDS);
    void                     OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Operations
public:
    void OnAdamoTimer ();
    void OnClose ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDAxesView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoMDAxesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoMDAxesView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    static CAdamoMDAxesView* m_pThis;
    CPtrList                 m_listAxes;
    CXTPTabControl*			 m_pFlatTabCtrl;
    CAdamoMDAxStatus*        m_pAllAxesView;
    CAdamoMDAxSettings*      m_pSingleAxesView[NMAXAXIS];
    CAdamoMDCommandBar*      m_pCmdBar;
    CAdamoMachine*           m_pMachine;
    int                      m_nIndex;
    ePswLevel                m_ePswLevel;
private :
    void CreateTab ();
    void UpdateTab ();
    void CreateViews();
    void CreateAxesList ();
    void DeleteListAxes ();
    void InsertAx (CAdamoAxis *pDev);
    void Iterator(CAdamoGroup *pGrp, CAdamoDatiFile *pFile, bool bIsPhysical);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDAXESVIEW_H__90751285_5B31_4580_88D7_6C54EF71E3CC__INCLUDED_)
