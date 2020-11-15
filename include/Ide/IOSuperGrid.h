#if !defined(AFX_IOSUPERGRID_H__5BA9E4E6_90CC_439A_96C2_AFCF000F43C8__INCLUDED_)
#define AFX_IOSUPERGRID_H__5BA9E4E6_90CC_439A_96C2_AFCF000F43C8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IOSuperGrid.h : header file
//

#include "SuperGridCtrl.h"
#include "AdamoBase.h"
#include "XImage.h"

class CXTPReportRecordItemViewDevices : public CXTPReportRecordItemText
{
		friend class CIOSuperGrid;
		friend class CIOItemInfo;
	public:
		CXTPReportRecordItemViewDevices () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/////////////////////////////////////////////////////////////////////////////
// CIOSuperGrid window
class CIOItemInfo
{
    public :
        CIOItemInfo () : m_pDev (NULL), m_pData (NULL), m_dwStatus (0)   { m_pData=new CAdamoData; m_pDataDiPrima=new CAdamoData; memset (m_pData, 0, sizeof (CAdamoData)); memset (m_pDataDiPrima, 0xff, sizeof (CAdamoData)); } 
        ~CIOItemInfo ()   { SAFE_DELETE (m_pData); SAFE_DELETE (m_pDataDiPrima); } 
        void SetDevice (CAdamoBase* pDev)   { m_pDev=pDev; } 
        CAdamoBase* GetDevice ()            { return m_pDev; } 
        CAdamoData* GetDeviceData ()        { return m_pData; } 
        CAdamoData* GetDeviceDataDiPrima () { return m_pDataDiPrima; } 
		void		SetStatus (int nBit)	{ m_dwStatusPrima = m_dwStatus; bitset (m_dwStatus, nBit); }
		void		ResetStatus (int nBit)	{ m_dwStatusPrima = m_dwStatus; bitres (m_dwStatus, nBit); }
		bool		IsStatusChanged ()		{ return m_dwStatusPrima != m_dwStatus; }
		int         Status (int nBit)		{ return bittest (m_dwStatus, nBit); }
    private :
        CAdamoBase* m_pDev;
        CAdamoData* m_pData;
        CAdamoData* m_pDataDiPrima;
		DWORD	    m_dwStatus;
		DWORD	    m_dwStatusPrima;
};

class CIOSuperGrid : public CXTPReportControl
{
	friend class CXTPReportRecordItemViewDevices;
	// Construction
	public:
		CIOSuperGrid();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CIOSuperGrid)
		//}}AFX_VIRTUAL

	// Implementation
		void InitializeGrid();
		void SetMode (int nMode)					{ m_nMode = nMode; }
		void SetMachine (CAdamoMachine* pMachine)   { m_pMachine=pMachine; }
		void RinfrescaDispositivi ();
		void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
		void GetSelectedDevice (CString& strDevicePath, enDispositivi& e, int& nID);
		void SelectDevice (enDispositivi e, int nID);
	public:
		virtual ~CIOSuperGrid();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CIOSuperGrid)
			// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg void OnForceSingleInput();
		afx_msg void OnUnForceSingleInput();
		afx_msg void OnForceAllInputsOn();
		afx_msg void OnForceAllInputsOff();
		afx_msg void OnReportLButtonDown(NMHDR* pNotifyStruct, LRESULT* pResult);
		afx_msg void OnReportRButtonDown(NMHDR* pNotifyStruct, LRESULT* pResult);
		afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
		afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
		afx_msg void OnMouseMove (UINT nFlags, CPoint point);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	private :
		CXTPImageList	m_image;
		CBitmap			m_bmpIO[5];
		int				m_nItemHeight;
		CAdamoMachine*	m_pMachine;
		int				m_nDragIndex;
		bool			m_bDragging;
		CImageList*		m_pDragImage;
		int				m_nMode;
		bool			m_bRepaintOnTimer;
		ePswLevel		m_ePswLevel;
		bool			m_bInitialized;
		CxImage			*m_pngIO[5];
		int				m_nInput;
		bool			m_bForced;
		short			m_cxImage;
		short			m_cyImage;
		CXTPReportRecord* m_pRecord;
		bool			m_ButtonDown;
		bool			m_Dragging;
		CPoint			m_point;
	private :
		/* overrided */
		BOOL OnPreviewKeyDown(UINT& rnChar, UINT nRepCnt, UINT nFlags);
		void OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
		void FillGrid ();
		void Iterator(CAdamoGroup *pGrp, CXTPReportRecord* pRecord, CAdamoDatiFile *pFile, bool bIsPhysical);
		CXTPReportRecord* AddItem (CAdamoBase* pDev, CXTPReportRecord* pParentRecord);
		void FillColumns ();
		int  SelectImageIndex (CAdamoBase* pDev);
		void AccendiInputDig (HDC hDC, CAdamoDgtIn* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void AccendiOutputDig (HDC hDC, CAdamoDgtOut* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawInputAnag (HDC hDC, CAdamoAnalogIn* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawOutputAnag (HDC hDC, CAdamoAnalogOut* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawCounter (HDC hDC, CAdamoCounter* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawTimer (HDC hDC, CAdamoTimer* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawAxes (HDC hDC, CAdamoAxis* pDev, CIOItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void SelettaInputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtIn* pInp);
		void SelettaOutputDigitale (UINT nFlags, CPoint point, int nColmunX, CAdamoDgtOut* pOut);
		void SelettaInputAnalogico (UINT nFlags, CAdamoAnalogIn* pInAna);
		void SelettaOutputAnalogico (UINT nFlags, CAdamoAnalogOut* pOutAna);
		void SelettaTimer (UINT nFlags, CAdamoTimer* pTimer);
		void SelettaCounter (UINT nFlags, CAdamoCounter* pCounter);
		void CallIterator (CProject *pPrj);
		void GetIndex (CXTPReportRecord* pRecord, CAdamoBase* pDev, int& nIndex);
		void TogliIGruppiSenzaFigli ();
		void TogliIGruppiSenzaFigli (CXTPReportRecord* pRecord);
		int  IteratorSearch (CXTPReportRecord* pRecord, enDispositivi e, int nID);
		int  GetDeviceOrder (CObject *pObj);
		LRESULT SetSkin (AppTheme nSkin);
		void OnDrag (CXTPReportRecord* pRecord, CPoint point);
		void SelectRecord (CXTPReportRecord* pRecord);
		void CleanData (CXTPReportRecord* pRecord);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IOSUPERGRID_H__5BA9E4E6_90CC_439A_96C2_AFCF000F43C8__INCLUDED_)
