#pragma once

/* include files */
#include "stdafx.h"
#include "BtnSt.h"

class CAdamoSynButton :	public CButtonST, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynButton)
	public:
		CAdamoSynButton(void);
		~CAdamoSynButton(void);
		int Create ();
		int Destroy ();
	virtual eRSCtrl KindOf ()	{
			return RSButton;
		}
		void SetButtonType (eRSButtonType eType)   { m_eType = eType; }
		eRSButtonType GetButtonType ()			   { return m_eType; }
		void SetButtonImage (CString strImage)	   { m_strImage = strImage; }
		CString GetButtonImage ()				   { return m_strImage; }
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()							   { return 0; }
		void EnableDownloadAction (bool b = true)  { if (b) bitset (m_dwAction, 0); else bitres (m_dwAction, 0); }
		void EnableLoadAction (bool b = true)      { if (b) bitset (m_dwAction, 1); else bitres (m_dwAction, 1); }
		void EnableSaveAction (bool b = true)      { if (b) bitset (m_dwAction, 2); else bitres (m_dwAction, 2); }
		void EnableChangePageAction (bool b = true){ if (b) bitset (m_dwAction, 3); else bitres (m_dwAction, 3); }
		bool IsDownloadActionEnabled ()			   { return bittest (m_dwAction, 0); }
		bool IsLoadActionEnabled ()				   { return bittest (m_dwAction, 1); }
		bool IsSaveActionEnabled ()				   { return bittest (m_dwAction, 2); }
		bool IsChangePageActionEnabled ()		   { return bittest (m_dwAction, 3); }
		void SetNextPage (CString strPage)		   { m_strPage = strPage; }
		CString GetNextPage ()					   { return m_strPage; }
		void SetTransparency (bool b)			   { m_bTransparency = b; }
		bool IsTransparent ()					   { return m_bTransparency; }
		void OnEraseBackGround (CDC* pDC);
		DECLARE_MESSAGE_MAP()
	private :
		eRSButtonType m_eType;
		CString		  m_strImage;
		CBitmap       m_bmp;
		CWnd		 *m_pParent;
		CFont		 *m_pFont;
		DWORD		  m_dwAction;
		CString		  m_strPage;
		bool          m_bTransparency;
	private :
		COLORREF GetTransparencyRGB ();
		void	 CreateFont ();
};

