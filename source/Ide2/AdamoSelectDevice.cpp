// AdamoSelectDevice.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoSelectDevice.h"


// CAdamoSelectDevice dialog

IMPLEMENT_DYNAMIC(CAdamoSelectDevice, CDialog)

CAdamoSelectDevice::CAdamoSelectDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoSelectDevice::IDD, pParent)
{

}

CAdamoSelectDevice::~CAdamoSelectDevice()
{
}

void CAdamoSelectDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAdamoSelectDevice, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CAdamoSelectDevice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_list.Create(WS_CHILD | WS_VISIBLE | WS_EX_CLIENTEDGE | WS_TABSTOP, CRect(0, 0, 473, 246), this, 0x100);
    m_list.SetMode (1);	
	m_list.InitializeGrid ();
	m_list.SelectDevice (m_eDev, m_nID);
	m_list.SetFocus ();
	return FALSE;
}

/*
** OnDestroy : messaggio di OnDestroy
*/
void CAdamoSelectDevice::OnDestroy ()
{
	CDialog::OnDestroy();
	m_list.ResetContent();
}

/*
** OnOK : bottone di ok
*/
void CAdamoSelectDevice::OnOK ()
{
	m_list.GetSelectedDevice (m_strDevicePath, m_eDev, m_nID);
	__super::OnOK ();
}
