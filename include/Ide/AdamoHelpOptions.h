/*
** AdamoHelpOptions.h
*/

#pragma once

/* include files */
#include "stdafx.h"

class CAdamoHelpOptions : public CListCtrl
{
	public :
		CAdamoHelpOptions (int nMode);
	public:
		void SetData (CStringList* strOptions, CStringList* strDescOptions, int nType);
		void SetCtrl ();
		void LoadData ();
		void SelectLastItem ();
		void SelectWord (CString str);
		CString GetSelectedOption ();
		void MoveSelectedOptionUp();
		void MoveSelectedOptionDown();
	protected:
		//{{AFX_MSG(CSlotView)
		afx_msg void OnKillFocus (CWnd* pNewWnd);
		//}}AFX_MSG
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
	protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	private :
		CImageList m_il;
		CStringList* m_strOptions;
		CStringList* m_strDescOptions;
		int m_nType;
		int m_nMode;
		int m_nLastItems[4];
	private :
		void DestroyHelpOptionsWindow ();
	public:
		enum EHighlight { HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW };
	protected:
		int m_nHighlight;
};