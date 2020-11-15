/*********************************************************
* Multi-Column Tree View
* Version: 1.1
* Date: October 22, 2003
* Author: Michal Mecinski
* E-mail: mimec@mimec.w.pl
* WWW: http://www.mimec.w.pl
*
* You may freely use and modify this code, but don't remove
* this copyright note.
*
* There is no warranty of any kind, express or implied, for this class.
* The author does not take the responsibility for any damage
* resulting from the use of it.
*
* Let me know if you find this code useful, and
* send me any modifications and bug reports.
*
* Copyright (C) 2003 by Michal Mecinski
*********************************************************/

#pragma once

#include "ColumnTreeCtrl.h"
#include "paneview.h"


class CColumnTreeView : public CPaneView
{
	DECLARE_DYNCREATE(CColumnTreeView)
protected:
	CColumnTreeView();

public:
	void UpdateColumns();
    CXTTreeCtrl& GetTreeCtrl() { return m_Tree; }
	CHeaderCtrl& GetHeaderCtrl() { return m_Header; }
    void SetNoHeader(BOOL bNoHeader);
public:
	virtual ~CColumnTreeView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void SetItemColor(HTREEITEM, COLORREF);
	COLORREF GetItemColor(HTREEITEM);
    void DeleteMapColor();
protected:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC) {}
protected:
	void UpdateScroller();
	void RepositionControls();
    void SetColumnImages ();
    void UpdateLastColumn (int cx);
protected:
	CColumnTreeCtrl m_Tree;
	CHeaderCtrl m_Header;
	int m_cyHeader;
	int m_cxTotal;
	int m_xPos;
	int m_arrColWidths[16];
	int m_xOffset;
    BOOL m_bNoHeader;
	struct Color_Font {
		COLORREF color;
		LOGFONT  logfont;
	};

	CMap <void*, void*, Color_Font, Color_Font&> m_mapColorFont;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDestroy();
private :
    CImageList* m_hIL;
    CBitmap*    m_bmpDeviceTree;
    CBitmap*    m_bmpDeviceDesc;
    CFont*      m_pBoldFont;
};
