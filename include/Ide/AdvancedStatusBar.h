/*
** AdvancedStatusBar.h
*/

#ifndef __ADAMO__ADVANCED__STATUSBAR__
#define __ADAMO__ADVANCED__STATUSBAR__

class CTransparentImage : public CStatic 
{
	public :
		CTransparentImage ();
		~CTransparentImage ();
		void SetColor1 (COLORREF clr)		{ m_clr1 = clr; }
		void SetColor2 (COLORREF clr)		{ m_clr2 = clr; }
		void SetActualColor (bool b)		{ m_clrActual = b ? m_clr2 : m_clr1; }
	protected :
		afx_msg void OnPaint ();
		DECLARE_MESSAGE_MAP()
	private :
		COLORREF m_clr1;
		COLORREF m_clr2;
		COLORREF m_clrActual;
};

class CMyCXTPStatusBarPane : public CXTPStatusBarPane
{
	public :
		CMyCXTPStatusBarPane (int nBitmap) : m_pBitmap (NULL), m_bSignaled (false)   {
			m_pBitmap = new CBitmap;
			m_pBitmap->LoadBitmap (nBitmap);
		}
		~CMyCXTPStatusBarPane ()   {
			delete m_pBitmap;
		}
		void OnDraw (CDC* pDC, CRect rcItem);
		void OnLButtonDown (CPoint point);
		bool m_bSignaled;
	private :
		CBitmap* m_pBitmap;
};

class CAdamoStatusBar : public CXTPStatusBar
{
    public :
        CAdamoStatusBar ();
        int InitializeStatusBar ();
        void Connect (bool bConnected=true);
        void InitCmd ();
        void EndCmd ();
        void OnTimer ();
		void AddIconConnection ();
		void AddIconCommunication ();
		void CreaBottoncini ();
		void ShowAlarm (bool bShow = true);
		void ShowMessage (bool bShow = true);
	protected :
		DECLARE_MESSAGE_MAP ()
		afx_msg void OnLButtonDown(UINT nFlags, CPoint pt );
    private :
        int CreaLeddini ();
    private :
        bool                    m_bLedOn;
        int                     m_nCounter;
		int						m_nCounterErrors;
		int						m_nCounterMessages;
		CTransparentImage		m_wndImageConnection;
		CTransparentImage		m_wndImageCommunication;
};

#endif