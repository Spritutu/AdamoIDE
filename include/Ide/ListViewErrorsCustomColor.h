#pragma once
// KosListErrors.cpp : implementation file
//
#include "stdafx.h"
#include "afxcview.h"
#include "PaneColorSet.h"

class CListViewCustomHeader : public CHeaderCtrl
{
	public :
		CListViewCustomHeader ();
		DECLARE_MESSAGE_MAP ();
		void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	private :
		LRESULT SetSkin (AppTheme nSkin);
	private :
		COLORREF m_foreColor;
		COLORREF m_bckColor;
};

class CListViewErrorsCustomColor :	public CListCtrl
{
	public:
		CListViewErrorsCustomColor(void);
		~CListViewErrorsCustomColor(void);
	protected :
		DECLARE_MESSAGE_MAP ();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		void DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct);
virtual void PreSubclassWindow();
	public:
		enum EHighlight { HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW };
	protected:
		CListViewCustomHeader m_HeaderCtrl;
		int m_nHighlight;
};
