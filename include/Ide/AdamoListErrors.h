#pragma once

class CAdamoListAllErrors : public CView
{
	DECLARE_DYNCREATE(CAdamoListAllErrors)
	public :
		CAdamoListAllErrors ();
		virtual ~CAdamoListAllErrors ();
		void AddMessage (CString str, CString strModule, int nPriority, time_t dt);
		void AddError (stThiraErrore *ae);
		CListViewErrorsCustomColor& GetListCtrl()	{ return m_list; }
		void DeleteErrors ();
	private :
		void OnInitialUpdate ();
		void CreateColumnHeader ();
		void CreateImageList ();
		void FillError (stThiraErrore* pE, int nItem);
		void FillMessage (CString str, CString strModule, int nPriority, time_t dt, int nItem);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDraw(CDC* pDC);
	private :
		CListViewErrorsCustomColor m_list;
		CImageList* m_lviml;
		CImageList* m_hciml;
		CBitmap* m_bmpHdrErrorType;
		CBitmap* m_bmpHdrError;
		CBitmap* m_bmpHdrModule;
		CBitmap* m_bmpHdrTask;
		CBitmap* m_bmpHdrDate;
		CBitmap* m_bmpHdrTime;
		int m_nItem;
		bool m_bInitialized;
};

class CAdamoListInterpreterErrors : public CView
{
	DECLARE_DYNCREATE(CAdamoListInterpreterErrors)
	public :
		CAdamoListInterpreterErrors ();
		virtual ~CAdamoListInterpreterErrors ();
		void AddMessage (CString str, CString strModule, int nPriority, DATE dt);
		void AddError (stThiraErrore *ae);
		CListViewErrorsCustomColor& GetListCtrl()	{ return m_list; }
		void DeleteErrors ();
	private :
		void OnInitialUpdate ();
		void CreateColumnHeader ();
		void CreateImageList ();
		void FillError (stThiraErrore* pE, int nItem);
		void FillMessage (CString str, CString strModule, int nPriority, DATE dt, int nItem);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDraw(CDC* pDC);
	private :
		CListViewErrorsCustomColor m_list;
	    CImageList* m_lviml;
		CImageList* m_hciml;
		CBitmap* m_bmpHdrTime;
		CBitmap* m_bmpHdrDate;
		CBitmap* m_bmpHdrIdentifier;
		CBitmap* m_bmpHdrError;
		CBitmap* m_bmpHdrModule;
		CBitmap* m_bmpHdrTask;
		CBitmap* m_bmpHdrPath;
		CBitmap* m_bmpHdrLine;
		int m_nItem;
		bool m_bInitialized;
};

class CAdamoListPLCErrors : public CView
{
	DECLARE_DYNCREATE(CAdamoListPLCErrors)
	public :
		CAdamoListPLCErrors ();
		virtual ~CAdamoListPLCErrors ();
		void AddMessage (CString str, CString strModule, int nPriority, DATE dt);
		void AddError (stThiraErrore *ae);
		CListViewErrorsCustomColor& GetListCtrl()	{ return m_list; }
		void DeleteErrors ();
	private :
		void OnInitialUpdate ();
		void CreateColumnHeader ();
		void CreateImageList ();
		void FillError (stThiraErrore* pE, int nItem);
		void FillMessage (CString str, CString strModule, int nPriority, DATE dt, int nItem);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDraw(CDC* pDC);
	private :
		CListViewErrorsCustomColor m_list;
		CImageList* m_lviml;
		CImageList* m_hciml;
		CBitmap* m_bmpHdrError;
		CBitmap* m_bmpHdrModule;
		CBitmap* m_bmpHdrTask;
		CBitmap* m_bmpHdrDate;
		CBitmap* m_bmpHdrTime;
		CBitmap* m_bmpHdrIdentifier;
		CBitmap* m_bmpHdrPriority;
		CBitmap* m_bmpHdrDisk;
		int m_nItem;
		bool m_bInitialized;
};

class CAdamoListISOErrors : public CView
{
	DECLARE_DYNCREATE(CAdamoListISOErrors)
	public :
		CAdamoListISOErrors ();
		virtual ~CAdamoListISOErrors ();
		void AddMessage (CString str, CString strModule, int nPriority, DATE dt);
		void AddError (stThiraErrore *ae);
		CListViewErrorsCustomColor& GetListCtrl()	{ return m_list; }
		void DeleteErrors ();
	private :
		void OnInitialUpdate ();
		void CreateColumnHeader ();
		void CreateImageList ();
		void FillError (stThiraErrore* pE, int nItem);
		void FillMessage (CString str, CString strModule, int nPriority, DATE dt, int nItem);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDraw(CDC* pDC);
	private :
		CListViewErrorsCustomColor m_list;
		CImageList* m_lviml;
		CImageList* m_hciml;
		CBitmap* m_bmpHdrTask;
		CBitmap* m_bmpHdrDate;
		CBitmap* m_bmpHdrTime;
		CBitmap* m_bmpHdrIdentifier;
		CBitmap* m_bmpHdrModule;
		CBitmap* m_bmpHdrError;
		CBitmap* m_bmpHdrLine;
		CBitmap* m_bmpHdrPath;
		int m_nItem;
		bool m_bInitialized;
};

class CAdamoListHardwareErrors : public CView
{
	DECLARE_DYNCREATE(CAdamoListHardwareErrors)
	public :
		CAdamoListHardwareErrors ();
		virtual ~CAdamoListHardwareErrors ();
		void AddMessage (CString str, CString strModule, int nPriority, DATE dt);
		void AddError (stThiraErrore *ae, bool bOpenDatiFile = true);
		CListViewErrorsCustomColor& GetListCtrl()	{ return m_list; }
		void DeleteErrors ();
	private :
		void OnInitialUpdate ();
		void CreateColumnHeader ();
		void CreateImageList ();
		void FillError (stThiraErrore* pE, int nItem);
		void FillMessage (CString str, CString strModule, int nPriority, DATE dt, int nItem);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDraw(CDC* pDC);
	private :
		CListViewErrorsCustomColor m_list;
		CAdamoDatiFile* m_df;
		CImageList* m_lviml;
		CImageList* m_hciml;
		CBitmap* m_bmpHdrTask;
		CBitmap* m_bmpHdrDate;
		CBitmap* m_bmpHdrTime;
		CBitmap* m_bmpHdrIdentifier;
		CBitmap* m_bmpHdrModule;
		CBitmap* m_bmpHdrError;
		CBitmap* m_bmpHdrDevice;
		CBitmap* m_bmpHdrDeviceType;
		int m_nItem;
		bool m_bInitialized;
	private :
		void OpenDatiFile ();
		void GetTypeDeviceErrorString (int nTypeDevice, char* szBuffer);
		CString GetDeviceString (int nTypeDevice);
		void GetDeviceErrorString (int nDevice, int nTypeDevice, char* szBuffer);
};

class CAdamoListMessages : public CView
{
	DECLARE_DYNCREATE(CAdamoListMessages)
	public :
		CAdamoListMessages ();
		virtual ~CAdamoListMessages ();
		void AddMessage (CString str, CString strModule, int nPriority, time_t dt);
		void AddError (stThiraErrore *ae);
		CListViewErrorsCustomColor& GetListCtrl()	{ return m_list; }
		void DeleteMessages ();
	private :
		void OnInitialUpdate ();
		void CreateColumnHeader ();
		void CreateImageList ();
		void FillError (stThiraErrore* pE, int nItem);
		void FillMessage (CString str, CString strModule, int nPriority, time_t dt, int nItem);
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
virtual void OnDraw(CDC* pDC);
	private :
		CListViewErrorsCustomColor m_list;
		CAdamoDatiFile* m_df;
		CImageList* m_lviml;
		CImageList* m_hciml;
		CBitmap* m_bmpHdrDate;
		CBitmap* m_bmpHdrTime;
		CBitmap* m_bmpHdrModule;
		CBitmap* m_bmpHdrError;
		CBitmap* m_bmpHdrPriority;
		int m_nItem;
		bool m_bInitialized;
};
