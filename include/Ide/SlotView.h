#if !defined(AFX_SLOTVIEW_H__C957D9D7_9E44_45F3_9F5E_4BB30C1E2B54__INCLUDED_)
#define AFX_SLOTVIEW_H__C957D9D7_9E44_45F3_9F5E_4BB30C1E2B54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SlotView.h : header file
//

/* include files */
#include "adamo.h"
#include "adamoint.h"
#include "adamocomm.h"
#include "adamomachine.h"

/* constants definitions */
#define ID_SLOTTIMER 400

/////////////////////////////////////////////////////////////////////////////
// CSlotView window

class CSlotView : public CListCtrl
{
// Construction
public:
	CSlotView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlotView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSlotView();
    void SetCurrentMachine (CAdamoMachine* pMachine)   {
        m_pMachine=pMachine;
    }
    int  InitSlotView ();
    void InitListControl ();
    void FillListControl (int nSlotIni, int nSlotFin);
    int  GetSelectedSlot (stTaskData* td);
    void Freeze ();
    void Refresh ();
    void DaUnaRinfrescata ();
	// Generated message map functions
protected:
	//{{AFX_MSG(CSlotView)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private :
    CAdamoMachine* m_pMachine;
    CImageList* m_hciml;
    CImageList* m_hlviml;
    CBitmap* m_bmpHdrSlot;
    CBitmap* m_bmpHdrNome;
    CBitmap* m_bmpHdrStato;
    CBitmap* m_bmpHdrEvento;
    CBitmap* m_bmpSSNotExist;
    CBitmap* m_bmpSSRun;
    CBitmap* m_bmpSSSuspended;
    CBitmap* m_bmpSSZombie;
    CBitmap* m_bmpSSWaitEvent;
    CBitmap* m_bmpSSBreak;
    UINT m_nTimer;
    stTaskData* m_pSlotsData;
    bool     m_bFreeze;
    int      m_nLastSelection;
    void DestroyListControl ();
    void AddItem (int nItem);
    void ModifyItem (stTaskData* pSlotData, int nItem);
    void GetStatusData (stTaskData* pTaskData, int* nIndex, char* strStatus, char* strEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLOTVIEW_H__C957D9D7_9E44_45F3_9F5E_4BB30C1E2B54__INCLUDED_)
