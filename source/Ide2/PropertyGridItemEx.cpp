// PropertyGridItemEx.cpp: implementation of the CPropertyGridItemEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "PropertyGridItemEx.h"
#include "AdamoCC.h"
#include "gridview.h"
#include "AdamoCPDlg.h"
#include "AdamoRSOptions.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomItemRSVar::CCustomItemRSVar (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasEdit|xtpGridItemHasExpandButton;
}

void CCustomItemRSVar::SetResults (CString strName, int nRow, CString strCol, int nType)
{
    m_nType=nType;
    m_strName=strName;
    m_nRow=nRow;
    m_strCol=strCol;
}

void CCustomItemRSVar::GetResults (CString *strName, int *nRow, CString *strCol, int *nType)
{
    *nType=m_nType;
    *strName=m_strName;
    *nRow=m_nRow;
    *strCol=m_strCol;
}

CCustomItemRSMatrix::CCustomItemRSMatrix (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasEdit|xtpGridItemHasExpandButton;
}

void CCustomItemRSMatrix::SetResults (CString strName)
{
    m_strName=strName;
}

void CCustomItemRSMatrix::GetResults (CString *strName)
{
    *strName=m_strName;
}

void CCustomItemRSMatrix::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
    ((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ()->OnEditMatrix (pButton);
}

CCustomItemRSDevice::CCustomItemRSDevice (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasExpandButton;
}

void CCustomItemRSDevice::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
    ((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ()->OnEditDevice (pButton);
}

CCustomItemRSTableIndex::CCustomItemRSTableIndex (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasExpandButton;
}

CCustomItemRSList::CCustomItemRSList (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasExpandButton;
}

void CCustomItemRSList::OnInplaceButtonDown(CXTPPropertyGridInplaceButton *pItem)
{
    ((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ()->OnEditListOptions();
};

CCustomItemRSCombo::CCustomItemRSCombo (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasExpandButton;
}

void CCustomItemRSCombo::OnInplaceButtonDown(CXTPPropertyGridInplaceButton *pItem)
{
    ((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ()->OnEditComboOptions();
};

class CCustomItemColorPopup: public CXTColorPopup 
{
	friend class CCustomItemColor;
public:
	CCustomItemColorPopup() : CXTColorPopup(TRUE) {}
private:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
	
	CCustomItemColor* m_pItem;
};

BEGIN_MESSAGE_MAP(CCustomItemColorPopup, CXTColorPopup)
	 ON_MESSAGE(CPN_XT_SELENDOK, OnSelEndOK)
END_MESSAGE_MAP()

	
LRESULT CCustomItemColorPopup::OnSelEndOK(WPARAM wParam, LPARAM /*lParam*/)
{
	m_pItem->OnValueChanged(m_pItem->RGBToString((COLORREF)wParam));
	return 0;
}

CCustomItemColor::CCustomItemColor(UINT nID, COLORREF clr)
	: CXTPPropertyGridItemColor(nID, clr)
{
	m_nFlags = xtpGridItemHasComboButton|xtpGridItemHasEdit;
	SetColor(clr);
}


void CCustomItemColor::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	CCustomItemColorPopup *pColorPopup = new CCustomItemColorPopup();
	
	CRect rcItem= GetItemRect();
	m_pGrid->ClientToScreen(&rcItem);
	rcItem.left = rcItem.right - 158; // small hack. need to add CPS_XT_LEFTALLIGN

	pColorPopup->Create(rcItem, m_pGrid, /*CPS_XT_USERCOLORS|*/CPS_XT_EXTENDED|CPS_XT_MORECOLORS|CPS_XT_SHOW3DSELECTION|CPS_XT_SHOWHEXVALUE, GetColor(), GetColor());
	pColorPopup->SetOwner(m_pGrid);
	pColorPopup->SetFocus();	
	pColorPopup->AddListener(pColorPopup->GetSafeHwnd());
	pColorPopup->m_pItem = this;
}

BEGIN_MESSAGE_MAP(CCustomItemSpinInplaceButton, CSpinButtonCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
END_MESSAGE_MAP()

void CCustomItemSpinInplaceButton::OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_pItem->OnValidateEdit();
	long nValue = m_pItem->GetNumber() + pNMUpDown->iDelta;
    if (nValue<m_pItem->m_fpMin||nValue>m_pItem->m_fpMax);
    else	{
	    CString str;
	    str.Format(_T("%i"), nValue);		
	    m_pItem->OnValueChanged(str);
    }	
	*pResult = 1;
}

CCustomItemSpin::CCustomItemSpin(UINT nID)
	: CXTPPropertyGridItemNumber(nID), m_fpMin (0.0), m_fpMax (0.0)
{
	m_wndSpin.m_pItem = this;
}
void CCustomItemSpin::OnDeselect()
{
	CXTPPropertyGridItemNumber::OnDeselect();

	if (m_wndSpin.m_hWnd) m_wndSpin.ShowWindow(SW_HIDE);
}

void CCustomItemSpin::OnSelect()
{
	CXTPPropertyGridItem::OnSelect();

	if (!m_bReadOnly)
	{	
		CRect rc = GetItemRect();
		rc.left = rc.right - 15;
		if (!m_wndSpin.m_hWnd)
		{
			m_wndSpin.Create(UDS_ARROWKEYS|WS_CHILD, rc, (CWnd*)m_pGrid, 0);
			m_wndSpin.SetRange(0, 100);
		}
		m_wndSpin.MoveWindow(rc);	
		m_wndSpin.ShowWindow(SW_SHOW);
	}
}
CRect CCustomItemSpin::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	rcValue.right -= 17;
	return rcValue;
}

CCustomItemButtonText::CCustomItemButtonText (UINT nID, CString strCaption, bool bOpen) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasExpandButton;
    m_bOpen=bOpen;
}

void CCustomItemButtonText::OnInplaceButtonDown()
{
    CFileDialog fd (m_bOpen, NULL, NULL, NULL);
    
    if (fd.DoModal ()==IDOK)  {
        OnValueChanged( fd.GetPathName());
        m_pGrid->Invalidate( FALSE );
    }
}

CCustomItemRSGrid::CCustomItemRSGrid (UINT nID, CString strCaption) : CXTPPropertyGridItem(nID, strCaption)
{
    m_nID=nID;
	m_nFlags = xtpGridItemHasEdit|xtpGridItemHasExpandButton;
}

void CCustomItemRSGrid::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
    ((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ()->OnEditGrid();
}
