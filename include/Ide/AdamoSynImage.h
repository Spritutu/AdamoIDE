#pragma once

/* include files */
#include "stdafx.h"

// CAdamoSynRadioButton
class CAdamoSynImage : public CStatic, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynImage)

	public:
		CAdamoSynImage();
		virtual ~CAdamoSynImage();
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	{ m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSImage;
		}
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()					    { return 0; }
        void SetStretch (bool bStretch)     { m_bStretch = bStretch; }
        bool IsStretched ()                 { return m_bStretch; }
        void SetIDImage (CString strID)     { m_strID = strID; }
        CString GetIDImage ()               { return m_strID; }
        void SetTransparent (bool b)        { m_bTransparent = b; }
		void SetTransparentColor (COLORREF col)			{ m_clrTransparentColor = col; }
        bool IsTransparent ()               { return m_bTransparent; }
        COLORREF GetTransparentColor ()     { return m_clrTransparentColor; }
        void OnEraseBackGround(CDC *pDC);
	protected:
		DECLARE_MESSAGE_MAP()
    private :
        void DrawItem(LPDRAWITEMSTRUCT);
	private :
		CWnd	    *m_pParent;
        bool        m_bStretch;
        bool        m_bTransparent;
        COLORREF    m_clrTransparentColor;
        CString     m_strID;
        CxImage     *m_pImage;
};


