#pragma once
#include "supergridctrl.h"

class CShowModulesSuperGrid :
	public CSuperGridCtrl
{
	public:
		CShowModulesSuperGrid(void);
		~CShowModulesSuperGrid(void);
		void InitializeGrid ();
		CProject *GetSelectedPrj ();
		void RefreshData ();
    protected:
	    //{{AFX_MSG(CTablesSuperGrid)
	    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    private :
        CImageList      m_image;
		bool			m_bMultimodule;
		int				m_nNumMachines;
	private :
		void FillColumns ();
		void FillData ();
		void AddMachine (CProject *pPrj);
		int  GetIcon(const CTreeItem* pItem);
		COLORREF GetCellRGB (void);
		void OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
		BOOL OnVkReturn(void);
};
