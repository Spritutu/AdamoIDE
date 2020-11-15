#pragma once

/* include files */
#include "stdafx.h"

class CAdamoSynGroupBox : public CButton, public CAdamoSynCtrl
{
		DECLARE_DYNAMIC(CAdamoSynGroupBox)
	public:
		CAdamoSynGroupBox(void);
		virtual ~CAdamoSynGroupBox(void);
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)			{ m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSInputCheck;
		}
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()								{ return 0; }
	protected:
		DECLARE_MESSAGE_MAP()
	private :
		CWnd	*m_pParent;
		CFont	*m_pFont;
	private :
		void	 CreateFont ();
	protected :
		afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		void PreSubclassWindow();
};
