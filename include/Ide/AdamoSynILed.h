#pragma once
#include "iangulargaugex.h"

class CAdamoSynILed :
	public CWnd, public CAdamoRTSynCtrl
{
	public:
		CAdamoSynILed(void);
		~CAdamoSynILed(void);
		eRSCtrl KindOf ()		{ return RSILed; }
		int Create ();
		int Destroy ();
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
	private :
		eLedType		m_eLedType;
		bool			m_bTransparent;
		bool			m_bShowReflection;
		COLORREF		m_activecolor;
		eLedStyle		m_eBevelStyle;
		int				m_nSpacingHorizontal;
		int				m_nSpacingVertical;
		int				m_nRowCount;
		int				m_nColCount;
		eLedStyle		m_eBorderStyle;
		COLORREF		m_backcolor;
		CWnd			*m_pParent;
	public :
		void SetLedType (eLedType e)				{ m_eLedType = e; }
		void SetTransparent (bool bTransparent)		{ m_bTransparent = bTransparent; }
		void SetShowReflection (bool bReflection)	{ m_bShowReflection = bReflection; }
		void SetActiveColor (COLORREF col)			{ m_activecolor = col; }
		void SetBevelStyle (eLedStyle e)			{ m_eBevelStyle = e; }
		void SetHorizontalSpacing (int nHS)			{ m_nSpacingHorizontal = nHS; }
		void SetVerticalSpacing (int nHS)			{ m_nSpacingVertical = nHS; }
		void SetRowCount (int nRow)					{ m_nRowCount = nRow; }
		void SetColCount (int nCol)					{ m_nColCount = nCol; }
		void SetBorderStyle (eLedStyle e)			{ m_eBorderStyle = e; }
		void SetBackgroundColor (COLORREF col)		{ m_backcolor = col; }
		eLedType GetLedType ()							{ return m_eLedType; }
		bool GetTransparent ()						{ return m_bTransparent; }
		bool GetShowReflection ()					{ return m_bShowReflection; }
		COLORREF GetActiveColor ()					{ return m_activecolor; }
		eLedStyle GetBevelStyle ()					{ return m_eBevelStyle; }
		int GetHorizontalSpacing ()					{ return m_nSpacingHorizontal; }
		int GetVerticalSpacing ()					{ return m_nSpacingVertical; }
		int GetRowCount ()							{ return m_nRowCount; }
		int GetColCount ()							{ return m_nColCount; }
		eLedStyle GetBorderStyle ()					{ return m_eBorderStyle; }
		COLORREF GetBackgroundColor ()				{ return m_backcolor; }
		void SetParentWnd (CWnd *pParent)			{ m_pParent = pParent; }
	private :
		int UpdateRectangularProperties ();
		int UpdateDiamondProperties ();
		int UpdateCircularProperties ();
};
