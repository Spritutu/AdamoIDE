#pragma once
#include "iangulargaugex.h"

class CAdamoSynIAngularGauge :
	public CiAngularGaugeX, public CAdamoRTSynCtrl
{
	public:
		CAdamoSynIAngularGauge(void);
		~CAdamoSynIAngularGauge(void);
		eRSCtrl KindOf ()		{ return RSIAngularGauge; }
		int Create ();
		int Destroy ();
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
	private :
		/* Generali */
		bool    m_bTransparent;
		int     m_nPosMin;
		int		m_nPosMax;
		bool    m_bShowInnerRadius;
		bool    m_bShowOuterRadius;
		bool    m_bReverseScale;
		eBorderStyle m_eBorderStyle;
		COLORREF m_rgbBackGround;
		/* Pointers */
		ePointerStyle m_ePointerStyle;
		int		m_nPointerSize;
		int     m_nPointerMargin;
		COLORREF m_rgbPointer;
		/* arc */
		int		m_nArcRadius;
		int     m_nArcStartAngle;
		int     m_nAngle;
		/* hub */
		bool    m_bHubShow;
		bool    m_bHub3D;
		int		m_nHubSize;
		COLORREF m_rgbHub;
		/* face */
		bool    m_bFaceShow;
		eStyle  m_eFaceStyle;
		eStyle  m_eBevelStyle;
		int     m_nBevelSize;
		COLORREF m_rgbFace;
		/* tick */
		int		m_nTickMargin;
		/* tick label */
		int		m_nTickLabelMargin;
		stRSFont m_fntTickLabelFont;
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
		void ShowInnerRadius (bool b)				{ m_bShowInnerRadius = b; }
		void ShowOuterRadius (bool b)				{ m_bShowOuterRadius = b; }
		void SetReverseScale (bool b)				{ m_bReverseScale = b; }
		void SetBorderStyle (eBorderStyle e)		{ m_eBorderStyle = e; }
		void SetBackGroundColor (COLORREF rgb)		{ m_rgbBackGround = rgb; }
		/* Pointers */
		void SetPointerStyle (ePointerStyle e)		{ m_ePointerStyle = e; }
		void SetPointerSize (int n)					{ m_nPointerSize = n; }
		void SetPointerMargin (int n)				{ m_nPointerMargin = n; }
		void SetPointerColor (COLORREF rgb)		{ m_rgbPointer = rgb; }
		/* arc */
		void SetArcRadius (int n)					{ m_nArcRadius = n; }
		void SetArcStartAngle (int n)				{ m_nArcStartAngle = n; }
		void SetArcAngle (int n)					{ m_nAngle = n; }
		/* hub */
		void ShowHub (bool b)						{ m_bHubShow = b; }
		void ShowHub3D (bool b)						{ m_bHub3D = b; }
		void SetHubSize (int n)						{ m_nHubSize = n; }
		void SetHubColor (COLORREF rgb)				{ m_rgbHub = rgb; }
		/* face */
		void ShowFace (bool b)						{ m_bFaceShow = b; }
		void SetFaceStyle (eStyle e)				{ m_eFaceStyle = e; }
		void SetBevelStyle (eStyle e)				{ m_eBevelStyle = e; }
		void SetBevelSize (int n)					{ m_nBevelSize = n; }
		void SetFaceColor (COLORREF rgb)			{ m_rgbFace = rgb; }
		/* tick */
		void SetTickMargin (int n)					{ m_nTickMargin = n; }
		/* tick label */
		void SetTickLabelMargin (int n)				{ m_nTickLabelMargin = n; }
		void SetTickLabelFont (stRSFont &fnt)		{ m_fntTickLabelFont = fnt; }
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
		/* generale */
		bool IsTransparent ()						{ return m_bTransparent; }
		int  GetPosMin ()							{ return m_nPosMin; }
		int  GetPosMax ()							{ return m_nPosMax; }
		bool IsInnerRadius ()						{ return m_bShowInnerRadius; }
		bool IsOuterRadius ()						{ return m_bShowOuterRadius; }
		bool IsReverseScale ()						{ return m_bReverseScale; }
		eBorderStyle GetBorderStyle ()				{ return m_eBorderStyle; }
		COLORREF GetBackGroundColor ()				{ return m_rgbBackGround; }
		/* Pointers */
		ePointerStyle GetPointerStyle ()			{ return m_ePointerStyle; }
		int GetPointerSize ()						{ return m_nPointerSize; }
		int GetPointerMargin ()						{ return m_nPointerMargin; }
		COLORREF GetPointerColor ()					{ return m_rgbPointer; }
		/* arc */
		int GetArcRadius ()							{ return m_nArcRadius; }
		int GetArcStartAngle ()						{ return m_nArcStartAngle; }
		int GetArcAngle ()							{ return m_nAngle; }
		/* hub */
		bool IsHub ()								{ return m_bHubShow; }
		bool IsHub3D ()								{ return m_bHub3D; }
		int  GetHubSize ()							{ return m_nHubSize; }
		COLORREF GetHubColor ()						{ return m_rgbHub; }
		/* face */
		bool IsFace ()								{ return m_bFaceShow; }
		eStyle GetFaceStyle ()						{ return m_eFaceStyle; }
		eStyle GetBevelStyle ()						{ return m_eBevelStyle; }
		int  GetBevelSize ()						{ return m_nBevelSize; }
		COLORREF GetFaceColor ()					{ return m_rgbFace; }
		/* tick */
		int GetTickMargin ()						{ return m_nTickMargin; }
		/* tick label */
		int GetTickLabelMargin ()					{ return m_nTickLabelMargin; }
		stRSFont& GetTickLabelFont (stRSFont &fnt)	{ return m_fntTickLabelFont; }
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
