#pragma once

/* include files */
#include "IOSuperGrid.h"

// CAdamoSelectDevice dialog

class CAdamoSelectDevice : public CDialog
{
	DECLARE_DYNAMIC(CAdamoSelectDevice)

public:
	CAdamoSelectDevice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdamoSelectDevice();
	
	void SetPath (CString str)		{ m_strDevicePath = str; }
	void SetDevice (enDispositivi e){ m_eDev = e; }
	void SetID (int nID)			{ m_nID = nID; }
	CString GetPath ()				{ return m_strDevicePath; }
	enDispositivi GetDevice ()		{ return m_eDev; }
	int GetID ()					{ return m_nID; }
// Dialog Data
	enum { IDD = IDD_DLG_SELECT_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() ;
protected :
	CIOSuperGrid m_list;
	afx_msg void OnDestroy();
	void OnOK ();
	DECLARE_MESSAGE_MAP()
private :
	CString m_strDevicePath;
	enDispositivi m_eDev;
	int m_nID;
};
