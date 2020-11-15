#pragma once
#include "adamosynbasectrl.h"

class CAdamoSynComboBox : public CComboBox, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynComboBox)
	public:
		CAdamoSynComboBox(void);
		~CAdamoSynComboBox(void);
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSCombo;
		}
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()							{ return 0; }
        bool IsAutoUpdate ()					{ return m_bIsAutoUpdate; }
        void SetAutoUpdate (bool bIsAutoUpdate) { m_bIsAutoUpdate=bIsAutoUpdate; }
        void    GetRSOptions (stRSOpzioni* pOptions, int* nOptions);
        void    AddRSOption (stRSOpzioni* pOpt, int nPos);
        void    DeleteAllRSOption ()                            { m_nOptions=0; }
		void    FillOptions ();
	private :
		void CreateFont ();
	protected:
		DECLARE_MESSAGE_MAP()
        stRSOpzioni m_stOpzioni[MAX_RSOPTIONS];
        int         m_nOptions;
		CWnd	*m_pParent;
		CAdamoData m_ad;
		CFont	*m_pFont;
		bool    m_bIsAutoUpdate;
};
