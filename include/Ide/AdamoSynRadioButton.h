#pragma once

/* include files */
#include "stdafx.h"

// CAdamoSynRadioButton
class CAdamoSynRadioButton : public CButton, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynRadioButton)

	public:
		CAdamoSynRadioButton();
		virtual ~CAdamoSynRadioButton();
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSInputRadio;
		}
		void SetChecked (bool bChecked)		{ m_bIsChecked=bChecked; }
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()						{ return 0; }
        bool IsAutoUpdate ()				{ return m_bIsAutoUpdate; }
        void SetAutoUpdate (bool bIsAutoUpdate)  { m_bIsAutoUpdate=bIsAutoUpdate; }
		void SetGroup (CString strGruppo)	{ m_strGroup = strGruppo; }
		CString GetGroup ()					{ return m_strGroup; }
	protected:
		DECLARE_MESSAGE_MAP()
	private :
		CWnd	*m_pParent;
		CFont	*m_pFont;
		CString m_strGroup;
		bool    m_bIsChecked;
		bool    m_bIsAutoUpdate;
	private :
		void	 CreateFont ();
	protected :
		afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		void PreSubclassWindow();
};


