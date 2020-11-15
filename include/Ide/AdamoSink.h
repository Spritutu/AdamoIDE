#if !defined(AFX_ADAMOSINK_H__2306CAF2_341D_4DD6_8C22_F7BE88A81AD8__INCLUDED_)
#define AFX_ADAMOSINK_H__2306CAF2_341D_4DD6_8C22_F7BE88A81AD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoSink.h : header file
//

#include "AdamoMachine.h"

#define NMAX_ERRORSDB   1024
#define NMAX_STRINGSSDB 1024

/////////////////////////////////////////////////////////////////////////////
// CAdamoSink command target

class CAdamoSink : public CCmdTarget
{
	DECLARE_DYNCREATE(CAdamoSink)
	CAdamoSink();           // protected constructor used by dynamic creation
    void ClearDBErrors ();
    void ClearDBStrings ();
    void CloseDialogErrors ();
// Attributes
public:
    void SetImpianto (CAdamoMachine* pMachine);
    void SetParentWnd (CWnd* pParentWnd);
    CPtrList& GetDBErrors ();
    CPtrList& GetDBStrings ();
    void ForceDisconnection ();
// Operations
public:
	virtual ~CAdamoSink();

private :
    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSink)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAdamoSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
    int OnAdamoError (LPUNKNOWN p);
    int OnAdamoStop ();
    int OnAdamoBreaked ();
    int OnAdamoConnected ();
    int OnAdamoDisconnected ();
    int OnAdamoString (long nPriority, DATE dt, BSTR str);
    int OnAdamoMessage (long nID, BSTR szMsgQueue, BSTR szMsgSender, BSTR szMsgSlot, VARIANT *v);
    int OnAdamoResources ();
    int OnAdamoChangePsw (BSTR szUser, LONG nPswLevel);
	int OnAdamoStartGDownload (long nGSlot, BSTR strPath);
	int OnAdamoGDownloaded (long nGSlot);
    int OnAdamoUploadCCPlane (LONG nAsse, BSTR bstrArea);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private :
    CAdamoMachine* m_pImpianto;
    CPtrList  m_lErrorsDB;
    CPtrList  m_lStringDB;
    CWnd*     m_pParentWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSINK_H__2306CAF2_341D_4DD6_8C22_F7BE88A81AD8__INCLUDED_)
