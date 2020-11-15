#pragma once
#include "CISevenSegmentAnalogX.h"

class CAdamoSynIDisplay :
	public CiSevenSegmentAnalogX	, public CAdamoRTSynCtrl
{
	public:
		CAdamoSynIDisplay(void);
		~CAdamoSynIDisplay(void);
		eRSCtrl KindOf ()		{ return RSIDisplay; }
		int Create ();
		int Destroy ();
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
	private :
		/* general */
		bool    m_bShowOffSegments;
		bool	m_bTransparent;
		bool	m_bShowSign;
		bool	m_bAutoSegmentOffColor;
		int		m_nPrecision;
		eDisplayStyle m_eBorderStyle;
		eDisplayLeadingStyle m_eLeadingStyle;
		COLORREF m_rgbBackGround;
		/* display */
		int		m_nCount;
		int		m_nSpacing;
		int		m_nSize;
		int		m_nSeparation;
		int		m_nMargin;
		COLORREF m_rgbColor;
		COLORREF m_rgbOffColor;
		CWnd	*m_pParent;
	public :
		/* general */
		void ShowOffSegment (bool bShowOffSegments)		{ m_bShowOffSegments = bShowOffSegments; }
		void SetTransparent (bool bTransparent)			{ m_bTransparent = bTransparent; }
		void ShowSign (bool bShowSign)					{ m_bShowSign = bShowSign; }
		void SetAutoSegmentOffColor (bool bAutoSegmentOffColor) { m_bAutoSegmentOffColor = bAutoSegmentOffColor; }
		void SetPrecision (int nPrecision)				{ m_nPrecision = nPrecision; }
		void SetDisplayStyle (eDisplayStyle eBorderStyle) { m_eBorderStyle = eBorderStyle; }
		void SetLeadingStyle (eDisplayLeadingStyle eLeadingStyle) { m_eLeadingStyle = eLeadingStyle; }
		void SetBackGroundColor (COLORREF rgb)			{ m_rgbBackGround = rgb; }
		/* display */
		void SetDisplayCount (int nCount)				{ m_nCount = nCount; }
		void SetDisplaySpacing (int nSpacing)			{ m_nSpacing = nSpacing; }
		void SetDisplaySize (int nSize)					{ m_nSize = nSize; }
		void SetDisplaySeparation (int nSeparation)		{ m_nSeparation = nSeparation; }
		void SetDisplayMargin (int nMargin)				{ m_nMargin = nMargin; }
		void SetDisplayColor (COLORREF rgbColor)		{ m_rgbColor = rgbColor; }
		void SetDisplayOffColor (COLORREF rgbOffColor)	{ m_rgbOffColor = rgbOffColor; }
		/* general */
		bool IsOffSegment ()							{ return m_bShowOffSegments; }
		bool IsTransparent ()							{ return m_bTransparent; }
		bool IsSign ()									{ return m_bShowSign; }
		bool IsAutoSegmentOffColor ()					{ return m_bAutoSegmentOffColor; }
		int  GetPrecision ()							{ return m_nPrecision; }
		eDisplayStyle GetDisplayStyle ()				{ return m_eBorderStyle; }
		eDisplayLeadingStyle GetLeadingStyle ()			{ return m_eLeadingStyle; }
		COLORREF GetBackGroundColor ()					{ return m_rgbBackGround; }
		/* display */
		int  GetDisplayCount ()							{ return m_nCount; }
		int  GetDisplaySpacing ()						{ return m_nSpacing; }
		int  GetDisplaySize ()							{ return m_nSize; }
		int  GetDisplaySeparation ()					{ return m_nSeparation; }
		int  GetDisplayMargin ()						{ return m_nMargin; }
		COLORREF GetDisplayColor ()						{ return m_rgbColor; }
		COLORREF GetDisplayOffColor ()					{ return m_rgbOffColor; }
		void SetParentWnd (CWnd *pParent)				{ m_pParent = pParent; }
	private :
		int UpdateProperties ();
};
