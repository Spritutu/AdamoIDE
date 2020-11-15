#if !defined(AFX_ADAMODGDEVICE_H__96ECEAAC_AF96_4286_83C6_630A8E4B33F4__INCLUDED_)
#define AFX_ADAMODGDEVICE_H__96ECEAAC_AF96_4286_83C6_630A8E4B33F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGDevice.h : header file
//

#include "supergridctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDevice view

class CDGDeviceItemInfo : public CItemInfo
{
    public :
        CDGDeviceItemInfo () : m_pDI (NULL), m_nNode (-1)   { m_pDI=new CCmdGetDeviceInfo; memset (m_pDI, 0, sizeof (CCmdGetDeviceInfo)); }
        ~CDGDeviceItemInfo ()                 { SAFE_DELETE (m_pDI); }
        void SetDI (CCmdGetDeviceInfo* pCI)   { *m_pDI=*pCI; }
        CCmdGetDeviceInfo* GetDI ()           { return m_pDI; }
        void SetNode (int nNode)              { m_nNode=nNode; }
        int  GetNode ()                       { return m_nNode; }
    private :
        CCmdGetDeviceInfo* m_pDI;
        int m_nNode;
};

class CAdamoDGDevice : public CSuperGridCtrl
{
    public:
	    CAdamoDGDevice();           // protected constructor used by dynamic creation
		~CAdamoDGDevice();
        void InitializeGrid ();
        void OnRefresh ();
        void OnSetOperational (int nMode);
        void OnSetPreOperational (int nMode);
        void OnSetInitialize ();
        void OnSetStart ();
        void OnSetStop ();
        void RinfrescaInformazioni ();
		void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
    protected:

    // Attributes
    public:

    // Operations
    public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CAdamoDGDevice)
	    //}}AFX_VIRTUAL

    // Implementation
    protected:

	// Generated message map functions
    protected:
	    //{{AFX_MSG(CAdamoDGDevice)
	    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    private :
        CAdamoMachine*  m_pMachine;
        CImageList      m_image;
        int             m_nCanScanStatus;
    private :
        void SetMachine ();
        void FillColumns ();
        void FillGrid ();
        void FillGridWithDeviceInfo (CCmdGetDeviceInfo *pDeviceInfo);
        CString GetProfile (CCmdGetDeviceInfo *pDeviceInfo);
        void OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht);
        COLORREF GetCellRGB (void);
		void SetProgressPosition (int nPosition);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGDEVICE_H__96ECEAAC_AF96_4286_83C6_630A8E4B33F4__INCLUDED_)
