#pragma once

/* include files */
#include "stdafx.h"

// CAdamoSynIPicture
class CAdamoSynIPicture : public CStatic, public CAdamoRTSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynIPicture)

	public:
		CAdamoSynIPicture();
		virtual ~CAdamoSynIPicture();
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	{ m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSIPicture;
		}
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ();
        void SetStretch (bool bStretch)     { m_bStretch = bStretch; }
        bool IsStretched ()                 { return m_bStretch; }
        void SetImageID (int nIndex, CString strID)      { if (nIndex >=0 && nIndex < 8) m_strID[nIndex] = strID; }
        CString GetImageID (int nIndex)   { CString str; if (nIndex >=0 && nIndex < 8)  str = m_strID[nIndex]; return str; }
        void SetTransparent (bool b)        { m_bTransparent = b; }
		void SetTransparentColor (COLORREF col)			{ m_clrTransparentColor = col; }
        bool IsTransparent ()               { return m_bTransparent; }
        COLORREF GetTransparentColor ()     { return m_clrTransparentColor; }
        void OnEraseBackGround(CDC* pDC);
	protected:
		DECLARE_MESSAGE_MAP()
    private :
        void DrawItem(LPDRAWITEMSTRUCT);
        int  SetCurrentImage (int nIndex);
	private :
		CWnd	    *m_pParent;
        bool        m_bStretch;
        bool        m_bTransparent;
        COLORREF    m_clrTransparentColor;
        CString     m_strID[NMAX_IPICTURE_IMAGES];
        CxImage     *m_paImage[NMAX_IPICTURE_IMAGES];
        CxImage     *m_pImage;
};


