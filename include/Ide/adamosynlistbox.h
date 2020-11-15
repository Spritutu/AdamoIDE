#pragma once
#include "adamosynbasectrl.h"

class CAdamoSynListBox : public CListBox, public CAdamoSynCtrl
{
	DECLARE_DYNAMIC(CAdamoSynListBox)
	public:
		CAdamoSynListBox(void);
		virtual ~CAdamoSynListBox(void);
		BOOL Create ();
		BOOL Destroy ();
		void SetParentWnd (CWnd *pParent)	       { m_pParent = pParent; }
		virtual eRSCtrl KindOf ()	{
			return RSList;
		}
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnEnter ();
		int OnTimer ()							{ return 0; }
        bool IsAutoUpdate ()					{ return m_bIsAutoUpdate; }
        void SetAutoUpdate (bool bIsAutoUpdate) { m_bIsAutoUpdate=bIsAutoUpdate; }
        void    GetRSOptions (stRSOpzioni* pOptions, int* nOptions);
        void    AddRSOption (stRSOpzioni* pOpt, int nPos);
        void    DeleteAllRSOption ()  { m_nOptions=0; }
        void    SetSize (int nSize)   { m_nSize=nSize; }
        int     GetSize ()            { return m_nSize; }
		void    FillOptions ();
	private :
		void CreateFont ();
	protected:
		DECLARE_MESSAGE_MAP()
        stRSOpzioni m_stOpzioni[MAX_RSOPTIONS];
        int         m_nOptions;
		int			m_nSize;
		CWnd	*m_pParent;
		CAdamoData m_ad;
		CFont	*m_pFont;
		bool    m_bIsAutoUpdate;
};
