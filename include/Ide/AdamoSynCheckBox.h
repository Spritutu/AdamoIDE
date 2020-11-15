#pragma once

/* include files */
#include "stdafx.h"

// CAdamoSynCheckBox
class CAdamoSynCheckBox : public CButton, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynCheckBox)

	public:
		CAdamoSynCheckBox();
		virtual ~CAdamoSynCheckBox();
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSInputCheck;
		}
		void SetChecked (bool bChecked)		{ m_bIsChecked=bChecked; }
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()						{ return 0; }
        bool IsAutoUpdate ()				{ return m_bIsAutoUpdate; }
        void SetAutoUpdate (bool bIsAutoUpdate)  { m_bIsAutoUpdate=bIsAutoUpdate; }
	protected:
		DECLARE_MESSAGE_MAP()
	private :
		CWnd	*m_pParent;
		CFont	*m_pFont;
		bool  m_bIsChecked;
		bool  m_bIsAutoUpdate;
	private :
		void	 CreateFont ();
	protected :
		afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		void PreSubclassWindow();
};


