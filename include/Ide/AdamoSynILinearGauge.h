#pragma once
#include "CIlinearGaugeX.h"

class CAdamoSynILinearGauge : 
	public CiLinearGaugeX, public CAdamoRTSynCtrl
{
	public:
		CAdamoSynILinearGauge(void);
		~CAdamoSynILinearGauge(void);
		eRSCtrl KindOf ()		{ return RSILinearGauge; }
		int Create ();
		int Destroy ();
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
	private :
		/* general */
		bool	m_bTransparent;
		int     m_nPosMin;
		int		m_nPosMax;
		bool    m_bReverseScale;
		eOrientation m_eOrientation;
		eOrientationTicks m_eOrientationTicks;
		eLinearGaugeStyle m_eBorderStyle;
		COLORREF m_rgbBackGround;
		/* pointers */
		eLinearGaugePointerStyle m_ePointerStyle;
		int		m_nPointerSize;
		int     m_nPointerOffset;
		COLORREF m_rgbPointer;
		bool    m_bPointer3D;
		bool    m_bDrawScaleSide;
		/* ticks */
		bool	m_bShowTickAxes;
		int 	m_nTickMargin;
		bool	m_bShowLabels;
		int 	m_nLabelMargin;
		stRSFont m_fntLabelFont;
		/* tick major */
		bool    m_bTickMajorShow;
		int     m_nTickMajorNumber;
		int     m_nTickMajorLenght;
		COLORREF m_rgbTickMajor;
		/* tick minor */
		bool    m_bTickMinorShow;
		int     m_nTickMinorNumber;
		int     m_nTickMinorLenght;
		COLORREF m_rgbTickMinor;
		/* sections */
		int     m_nSectionsNumber;
		COLORREF m_rgbSec1;
		COLORREF m_rgbSec2;
		COLORREF m_rgbSec3;
		COLORREF m_rgbSec4;
		COLORREF m_rgbSec5;
		int      m_nEndSec1;
		int      m_nEndSec2;
		int      m_nEndSec3;
		int      m_nEndSec4;
		CWnd	*m_pParent;
	public :
		/* generali */
		void SetTransparent (bool b)				{ m_bTransparent = b; }
		void SetPosMin (int n)						{ m_nPosMin = n; }
		void SetPosMax (int n)						{ m_nPosMax = n; }
		void SetReverseScale (bool b)				{ m_bReverseScale = b; }
		void SetOrientation (eOrientation e)		{ m_eOrientation = e; }
		void SetOrientationTicks (eOrientationTicks e)	{ m_eOrientationTicks = e; }
		void SetBorderStyle (eLinearGaugeStyle e)	{ m_eBorderStyle = e; }
		void SetBackGroundColor (COLORREF rgb)		{ m_rgbBackGround = rgb; }
		/* pointers */
		void SetPointerStyle (eLinearGaugePointerStyle ePointerStyle)	{ m_ePointerStyle = ePointerStyle; }
		void SetPointerSize (int nPointerSize)		{ m_nPointerSize = nPointerSize; }
		void SetPointerOffset (int nPointerOffset)	{ m_nPointerOffset = nPointerOffset; }
		void SetPointerColor (COLORREF rgbPointer)	{ m_rgbPointer = rgbPointer; }
		void SetPointer3D (bool bPointer3D)			{ m_bPointer3D = bPointer3D; }
		void SetDrawScaleSide (bool bDrawScaleSide)	{ m_bDrawScaleSide = bDrawScaleSide; }
		/* ticks */
		void ShowTickAxes (bool bShowTickAxes)		{ m_bShowTickAxes = bShowTickAxes; }
		void SetTickMargin (int nTickMargin)		{ m_nTickMargin = nTickMargin; }
		void ShowLabels (bool bShowLabels)			{ m_bShowLabels = bShowLabels; }
		void SetLabelMargin (int nLabelMargin)		{ m_nLabelMargin = nLabelMargin; }
		void SetTickLabelFont (stRSFont &fnt)		{ m_fntLabelFont = fnt; }
		/* tick major */
		void ShowTickMajor (bool b)					{ m_bTickMajorShow = b; }
		void SetTickMajorNumber (int n)				{ m_nTickMajorNumber = n; }
		void SetTickMajorLenght (int n)				{ m_nTickMajorLenght = n; }
		void SetTickMajorColor (COLORREF rgb)		{ m_rgbTickMajor = rgb; }
		/* tick minor */
		void ShowTickMinor (bool b)					{ m_bTickMinorShow = b; }
		void SetTickMinorNumber (int n)				{ m_nTickMinorNumber = n; }
		void SetTickMinorLenght (int n)				{ m_nTickMinorLenght = n; }
		void SetTickMinorColor (COLORREF rgb)		{ m_rgbTickMinor = rgb; }
		/* sections */
		void SetSectionsNumber (int n)				{ m_nSectionsNumber = n; }
		void SetSection1Color (COLORREF rgb)		{ m_rgbSec1 = rgb; }
		void SetSection2Color (COLORREF rgb)		{ m_rgbSec2 = rgb; }
		void SetSection3Color (COLORREF rgb)		{ m_rgbSec3 = rgb; }
		void SetSection4Color (COLORREF rgb)		{ m_rgbSec4 = rgb; }
		void SetSection5Color (COLORREF rgb)		{ m_rgbSec5 = rgb; }
		void SetEndSection1 (int n)					{ m_nEndSec1 = n; }
		void SetEndSection2 (int n)					{ m_nEndSec2 = n; }
		void SetEndSection3 (int n)					{ m_nEndSec3 = n; }
		void SetEndSection4 (int n)					{ m_nEndSec4 = n; }
		/* general */
		bool IsTransparent ()						{ return m_bTransparent; }
		int  GetPosMin ()							{ return m_nPosMin; }
		int  GetPosMax ()							{ return m_nPosMax; }
		bool IsReverseScale ()						{ return m_bReverseScale; }
		eOrientation GetOrientation ()				{ return m_eOrientation; }
		eOrientationTicks GetOrientationTicks ()	{ return m_eOrientationTicks; }
		eLinearGaugeStyle GetBorderStyle ()			{ return m_eBorderStyle; }
		COLORREF GetBackGroundColor ()				{ return m_rgbBackGround; }
		/* pointers */
		eLinearGaugePointerStyle GetPointerStyle ()	{ return m_ePointerStyle; }
		int  GetPointerSize ()						{ return m_nPointerSize; }
		int  GetPointerOffset ()					{ return m_nPointerOffset; }
		COLORREF GetPointerColor ()					{ return m_rgbPointer; }
		bool GetPointer3D ()						{ return m_bPointer3D; }
		bool GetDrawScaleSide ()					{ return m_bDrawScaleSide; }
		/* ticks */
		bool IsTickAxes ()							{ return m_bShowTickAxes; }
		int  GetTickMargin ()						{ return m_nTickMargin; }
		bool IsShowLabels ()						{ return m_bShowLabels; }
		int  GetLabelMargin ()						{ return m_nLabelMargin; }
		stRSFont& GetTickLabelFont ()				{ return m_fntLabelFont; }
		/* tick major */
		bool IsTickMajor ()							{ return m_bTickMajorShow; }
		int GetTickMajorNumber ()					{ return m_nTickMajorNumber; }
		int GetTickMajorLenght ()					{ return m_nTickMajorLenght; }
		COLORREF GetTickMajorColor ()				{ return m_rgbTickMajor; }
		/* tick minor */
		bool IsTickMinor ()							{ return m_bTickMinorShow; }
		int GetTickMinorNumber ()					{ return m_nTickMinorNumber; }
		int GetTickMinorLenght ()					{ return m_nTickMinorLenght; }
		COLORREF GetTickMinorColor ()				{ return m_rgbTickMinor; }
		/* sections */
		int GetSectionsNumber ()					{ return m_nSectionsNumber; }
		COLORREF GetSection1Color ()				{ return m_rgbSec1; }
		COLORREF GetSection2Color ()				{ return m_rgbSec2; }
		COLORREF GetSection3Color ()				{ return m_rgbSec3; }
		COLORREF GetSection4Color ()				{ return m_rgbSec4; }
		COLORREF GetSection5Color ()				{ return m_rgbSec5; }
		int GetEndSection1 ()						{ return m_nEndSec1; }
		int GetEndSection2 ()						{ return m_nEndSec2; }
		int GetEndSection3 ()						{ return m_nEndSec3; }
		int GetEndSection4 ()						{ return m_nEndSec4; }
		void SetParentWnd (CWnd *pParent)			{ m_pParent = pParent; }
	private :
		int UpdateProperties ();

};
