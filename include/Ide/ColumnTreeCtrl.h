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


class CColumnTreeCtrl : public CXTTreeCtrl
{
	DECLARE_DYNAMIC(CColumnTreeCtrl)
public:
	CColumnTreeCtrl();
	virtual ~CColumnTreeCtrl();

protected:
	BOOL   CheckHit(CPoint point);
    bool   CheckPoint (UINT nFlags, CPoint point);
    bool   CheckPoint2 (UINT nFlags, CPoint point);
protected:
	int m_cxFirstCol;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	friend class CColumnTreeView;
};
