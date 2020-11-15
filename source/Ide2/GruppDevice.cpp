// GruppDevice.cpp: implementation of the CGruppDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GruppDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGruppDevice::CGruppDevice()
{

}

CGruppDevice::~CGruppDevice()
{

}

/*
** SetName : Retrieves  device name
*/
void CGruppDevice::SetName(CString strNew)
{
	m_strName = strNew;
}

/*
** SetDeviceName : Retrieves device string
*/
void CGruppDevice::SetDeviceName(CString strNew)
{
	m_strDeviceName = strNew;
}

/*
** SetDescription : Retrieves device description
*/
void CGruppDevice::SetDescription(CString strNew)
{
	m_strDescription = strNew;
}

/*
** SetOptionField : Retrieves device options 
*/
void CGruppDevice::SetOptionField(CString strNew)
{
	m_OptionField.Add(strNew);
}
/*
** SetOptionField : Retrieves device Connector 
*/
void CGruppDevice::SetConnector(StructConnector* pStructConn)
{
	m_objConnector.AddTail(pStructConn);
}
/*
** SetNameAtt : Retrieves device attribute
*/
void CGruppDevice::SetNameAtt(CString strNew)
{
	m_NameAtt = strNew;
}

/*
** SetOptionSel : Retrieves device option selected 
*/
void CGruppDevice::SetOptionSel(CString strNew)
{
	m_strOptionSel = strNew;
}
/*
** SetPathAtt : Retrieves device attribute
*/
void CGruppDevice::SetPathAtt(CString strNew)
{
	m_PathAtt = strNew;
}

/*
** GetConnector : Returns device Connector
*/
CPtrList& CGruppDevice::GetConnector()
{
	return m_objConnector;
}
/*
** GetOptionField : Returns device options
*/
CStringArray& CGruppDevice::GetOptionField()
{
	return m_OptionField;
}
/*
** GetOptionSel : Returns device options
*/
CString CGruppDevice::GetOptionSel()
{
	return m_strOptionSel;
}
/*
** GetAttName : Returns device attribute
*/
CString CGruppDevice::GetNameAtt()
{
	return m_NameAtt;
}

/*
** GetPathAtt : Returns device attribute
*/
CString CGruppDevice::GetPathAtt()
{
	return m_PathAtt;
}


/*
** GetDescription : Returns device description
*/
CString CGruppDevice::GetDescription()
{
	return m_strDescription;
}

/*
** GetName : Returns device name
*/
CString CGruppDevice::GetName()
{
    return m_strName;
}

/*
** GetDeviceName : Returns device string
*/
LPCTSTR CGruppDevice::GetDeviceName()
{
	return m_strDeviceName;
}
