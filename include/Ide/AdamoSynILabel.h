#pragma once

class CAdamoSynILabel :
	public CiLabelX, public CAdamoRTSynCtrl
{
	public:
		CAdamoSynILabel(void);
		~CAdamoSynILabel(void);
		eRSCtrl KindOf ()		{ return RSILabel; }
		int Create ();
		int Destroy ();
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
	private :
		CString m_strPath;
		enDispositivi m_eDev;
		int     m_nID;
		int     m_nData;
		/* general */
        CString m_strValue;
		bool	m_bTransparent;
		bool	m_bAutoSize;
		eLabelStyle m_eBorderStyle;
		eLabelAlignment m_eAlignment;
		stRSFont m_fntFont;
		bool	m_bShadow;
		int		m_nOffsetX;
		int		m_nOffsetY;
		COLORREF m_rgbColor;
		COLORREF m_rgbBackColor;
		COLORREF m_rgbShadowColor;
		CWnd	*m_pParent;
	public :
		void SetTransparent (bool bTransparent)				{ m_bTransparent = bTransparent; }
		void SetAutoSize (bool bAutoSize)					{ m_bAutoSize = bAutoSize; }
		void SetStyle (eLabelStyle eBorderStyle)			{ m_eBorderStyle = eBorderStyle; }
		void SetAlignment (eLabelAlignment eAlignment)		{ m_eAlignment = eAlignment; }
		void SetFont (stRSFont &fnt)						{ m_fntFont = fnt; }
		void ShowShadow (bool bShadow)						{ m_bShadow = bShadow; }
		void SetOffsetX (int nOffsetX)						{ m_nOffsetX = nOffsetX; }
		void SetOffsetY (int nOffsetY)						{ m_nOffsetY = nOffsetY; }
		void SetColor (COLORREF rgbColor)					{ m_rgbColor = rgbColor; }
		void SetBackColor (COLORREF rgbBackColor)			{ m_rgbBackColor = rgbBackColor; }
		void SetShadowColor (COLORREF rgbShadowColor)		{ m_rgbShadowColor = rgbShadowColor; }
		bool IsTransparent ()								{ return m_bTransparent; }
		bool IsAutoSize ()									{ return m_bAutoSize; }
		eLabelStyle GetStyle ()								{ return m_eBorderStyle; }
		eLabelAlignment GetAlignment ()						{ return m_eAlignment; }
		stRSFont& GetFont ()								{ return m_fntFont; }
		bool IsShadow ()									{ return m_bShadow; }
		int GetOffsetX ()									{ return m_nOffsetX; }
		int GetOffsetY ()									{ return m_nOffsetY; }
		COLORREF GetColor ()								{ return m_rgbColor; }
		COLORREF GetBackColor ()							{ return m_rgbBackColor; }
		COLORREF GetShadowColor ()							{ return m_rgbShadowColor; }
		void SetParentWnd (CWnd *pParent)					{ m_pParent = pParent; }
	private :
		int UpdateProperties ();
};
