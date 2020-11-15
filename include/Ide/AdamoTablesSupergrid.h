#if !defined(__ADAMO_CTABLESSUPERGRID__)
#define __ADAMO_CTABLESSUPERGRID__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGCards.h : header file
//
#include "supergridctrl.h"
#include "AdamoBase.h"
#include "XImage.h"

class CAdamoTables;

/////////////////////////////////////////////////////////////////////////////
// CTablesSuperGrid view

class CXTPViewTableDevices : public CXTPReportRecordItemText
{
		friend class CIOSuperGrid;
		friend class CIOItemInfo;
	public:
		CXTPViewTableDevices () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

class CVariableInfo
{
public :
	CVariableInfo () : m_nGL (0), m_nLevel (-1), m_nProg (-1)  {
	}
	CString m_strName;
	int m_nGL;
	int m_nLevel;
	int m_nProg;
};

class CTablesItemInfo
{
    public :
		CTablesItemInfo () : m_pDev (NULL), m_pVarInfo (NULL)     { }
        ~CTablesItemInfo ()                     { }
        void    SetPath (CString strPath)       { m_strPath=strPath; }
        CString GetPath ()                      { return m_strPath;  }
        void    SetData (CAdamoData& data)      { m_data=data; }
        CAdamoData& GetData ()                  { return m_data; }
        void    SetKey  (CAdamoValori& key )    { m_key=key; }
        CAdamoValori& GetKey ()                 { return m_key; }
		void	SetStatus (int nBit)	{ m_dwStatusPrima = m_dwStatus; bitset (m_dwStatus, nBit); }
		void	ResetStatus (int nBit)	{ m_dwStatusPrima = m_dwStatus; bitres (m_dwStatus, nBit); }
		bool	IsStatusChanged ()		{ return m_dwStatusPrima != m_dwStatus; }
		int     Status (int nBit)		{ return bittest (m_dwStatus, nBit); }
		void    SetDataType (int nType) { m_nDataType = nType; }
		int     GetDataType ()			{ return m_nDataType; }
		void    SetDevice (CAdamoBase* pDev)				{ m_pDev = pDev; }
		void    SetVariableInfo (CVariableInfo* pVarInfo)	{ m_pVarInfo = pVarInfo; }
		CAdamoBase* GetDevice ()		{ return m_pDev; }
        CAdamoData* GetDataDiPrima ()	{ return &m_DataDiPrima; } 
		CVariableInfo* GetVariableInfo () { return m_pVarInfo; }
    private :
        CString      m_strPath;
        CAdamoData   m_data;
        CAdamoData   m_DataDiPrima;
        CAdamoValori m_key;
		int          m_nDataType;
		DWORD	     m_dwStatus;
		DWORD	     m_dwStatusPrima;
		CAdamoBase*	 m_pDev;
		CVariableInfo* m_pVarInfo;
};

class CTablesSuperGrid : public CXTPReportControl
{
	friend class CXTPViewTableDevices;
    public :
	    CTablesSuperGrid();           // protected constructor used by dynamic creation
        void RinfrescaInformazioni ();
        void InitializeGrid ();
		void AddVariable (CString strVariableName, int nGL, int nLevel, int nProg);
        void FillGrid ();
		void Clear ();
		void SetMachine (CAdamoMachine* pMachine)	{ m_pMachine = pMachine; }
    protected:

    // Attributes
        public:

    // Operations
        public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CTablesSuperGrid)
	    //}}AFX_VIRTUAL

    // Implementation
	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CTablesSuperGrid)
	    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg void OnValueChanged(NMHDR* pNotifyStruct, LRESULT* result);
		afx_msg void OnRequestEdit (NMHDR* pNotifyStruct, LRESULT* result);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    private :
        void FillColumns ();
        void    SetMachine ();
        void    OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
        void RiempiIlSuperGrid (CXTPReportRecord* pRecord, CVariableInfo* pVarInfo, CString strPath, CAdamoValori *pResult, int* pValueType, int nElements);
        int      GetSGImage (int nType);
        CString  GetSGKey (CAdamoValori *pResult);
        CXTPReportRecord* AggiungiIlPrimoNodo (CVariableInfo* pVarInfo);
        void ConcludiOperazioni (CXTPReportRecord* pRecord);
        void IteraLeTabelle (CXTPReportRecord* pRecord, CVariableInfo* pVarInfo);
        CString CreaIlPath (CString strPath, CAdamoValori *pKey);
        static int SortTables (const void* p1, const void* p2);
        //virtual void Sort(CTreeItem* pParent, BOOL bSortChildren, int (__cdecl *pSortFun)(const void *, const void *));
        virtual BOOL OnVkReturn(void);
        //virtual void OnUpdateListViewItem (CTreeItem* lpItem, LVITEM *plvItem);
        void SelettaBoolean (CPoint point, LVHITTESTINFO& ht);
		void GetObjectKeys (CString strClassName, CAdamoValori* pResult, int *pValueType, int *pnElements);
		void RinfrescaDispositivi (CTablesItemInfo** pInfo, CDeviceData* pData, int nIndexInBuffer);
		bool IsDevice (int nType);
		void LoadBitmaps ();
		void DrawData (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, CString str, bool bForceRefresh=true);
		void DrawBoolean (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, CString str, bool b, bool bForceRefresh=true);
		void AccendiInputDig (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void AccendiOutputDig (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawInputAnag (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawOutputAnag (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawTimer (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawCounter (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void DrawAxes (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh=true);
		void SelettaInputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtIn* pInp);
		void SelettaOutputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtOut* pOut);
		void SelettaInputAnalogico (UINT nFlags, CAdamoAnalogIn* pInAna);
		void SelettaOutputAnalogico (UINT nFlags, CAdamoAnalogOut* pOutAna);
		void SelettaTimer (UINT nFlags, CAdamoTimer* pTimer);
		void SelettaCounter (UINT nFlags, CAdamoCounter* pCounter);
		LRESULT SetSkin (AppTheme nSkin);
    private :
        CAdamoMachine*  m_pMachine;
	    CBitmap         m_bmpIO[5];
		CxImage			*m_pngIO[5];
		bool			m_bRinfrescato;
		ePswLevel		m_ePswLevel;
		CPtrList		m_listVarInfo;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGCARDS_H__8257F340_A369_48F7_8F4E_ABC64D37600D__INCLUDED_)
