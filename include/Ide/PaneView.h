// PaneWnd.h : header file
//
// (c)1998-2019 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(__PANEWND_H__)
#	define __PANEWND_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//===========================================================================
class CPaneView : public CView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//-----------------------------------------------------------------------
	CPaneView();

	//-----------------------------------------------------------------------
	// Summary:
	//-----------------------------------------------------------------------
	virtual ~CPaneView();

	//-----------------------------------------------------------------------
	// Summary:
	// Parameters:
	//-----------------------------------------------------------------------
	void SetOwnerPane(CXTPDockingPane* pPane);

	//{{AFX_VIRTUAL(CPaneWnd)
	//}}AFX_VIRTUAL

protected:
	CRect m_rcMargin;		  //
	CXTPDockingPane* m_pPane; //

	//-----------------------------------------------------------------------
	// Summary:
	// Parameters:
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics(AppTheme nTheme);

	//-----------------------------------------------------------------------
	// Summary:
	// Parameters:
	//-----------------------------------------------------------------------
	virtual void MoveChildren(CXTPToolBar* pToolBar, CWnd* pChildWnd, int cx, int cy);

	//{{AFX_MSG(CPaneWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

AFX_INLINE void CPaneView::SetOwnerPane(CXTPDockingPane* pPane)
{
	m_pPane = pPane;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PANEWND_H__)
