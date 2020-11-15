#pragma once

/* include files */
#include "stdafx.h"
#include "paredit.h"

// CAdamoSynEdit
class CAdamoSynEdit : public CParsedEdit, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynEdit)

	public:
		CAdamoSynEdit();
		virtual ~CAdamoSynEdit();
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSInputText;
		}
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnEnter ();
		int OnTimer ()							{ return 0; }
        bool IsAutoUpdate ()					{ return m_bIsAutoUpdate; }
        void SetAutoUpdate (bool bIsAutoUpdate) { m_bIsAutoUpdate=bIsAutoUpdate; }
		void SetType (eEditType e)				{ m_eType = e; }
	private :
		void	 CreateFont ();
	protected:
		DECLARE_MESSAGE_MAP()
		CWnd	*m_pParent;
		CFont	*m_pFont;
		eEditType m_eType;
		bool    m_bIsAutoUpdate;
		CAdamoData m_ad;
};


