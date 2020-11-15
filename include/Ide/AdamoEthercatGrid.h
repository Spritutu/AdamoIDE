// AdamoEthercatGrid.h: interface for the CAdamoEthercatGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADAMOETHERCATGRID_H__044D72B3_517F_403A_99F3_49485433C0FA__INCLUDED_)
#define AFX_ADAMOETHERCATGRID_H__044D72B3_517F_403A_99F3_49485433C0FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "supergridctrl.h"

class CAdamoEthercatItemInfo : public CItemInfo
{
    public :
        CAdamoEthercatItemInfo () : m_nSlave (-1), m_nStatus (-1), m_nActualStatus (-1)	{ }
		void SetStatus (int nStatus)	{ m_nStatus=nStatus; }
		int  GetStatus ()				{ return m_nStatus; }
		void SetActualStatus (int nStatus)	{ m_nActualStatus=nStatus; }
		int  GetActualStatus ()				{ return m_nActualStatus; }
		void SetSlave (int nSlave)			{ m_nSlave=nSlave; }
		int  GetSlave ()					{ return m_nSlave; }
	private :
		int m_nSlave;
		int m_nStatus;
		int m_nActualStatus;
};

class CAdamoEthercatGrid : public CSuperGridCtrl
{
public:
	CAdamoEthercatGrid();
	virtual ~CAdamoEthercatGrid();
	void InitializeGrid();
	void RinfrescaInformazioni ();
	void RequestState (int nRequestState, bool bSingleSlave);
	int GetSelectedSlave ();
protected :
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
private :
    CAdamoMachine*  m_pMachine;
    CImageList      m_image;
	CBitmap			m_bmp;
private :
	void AddEthercatSlave ();
	void SetMachine ();
	void FillColumns ();
	CPtrArray &GetEthercatSlave ();
	void OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB (void);
	void OnPaintColumn (LPDRAWITEMSTRUCT lpDrawItemStruct, int nColumn, CRect* rc);
	void DisegnaStato (CAdamoEthercatItemInfo *status, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc);
};

#endif // !defined(AFX_ADAMOETHERCATGRID_H__044D72B3_517F_403A_99F3_49485433C0FA__INCLUDED_)
