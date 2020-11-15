// GruppDevice.h: interface for the CGruppDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRUPPDEVICE_H__04910615_28E3_464B_92D5_6B6C1E7F4FBD__INCLUDED_)
#define AFX_GRUPPDEVICE_H__04910615_28E3_464B_92D5_6B6C1E7F4FBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct StructTab              
{
  CPtrList objItem;
  CString  name;
  CString  bitmap;
};

struct StructConnector              
{
  CString   strConnector;
  int       nID;
};

class CGruppDevice  
{
public:
    int m_nOptionSel;
    CString m_strPathFileDat;
    CString GetName();
	CString GetDescription();
	LPCTSTR GetDeviceName();
	CStringArray& GetOptionField();
    CPtrList& GetConnector();
    CString GetNameAtt();
    CString GetPathAtt();
	CString GetOptionSel();
    
    void SetName(CString strNew);
	void SetDescription(CString strNew);
	void SetDeviceName(CString strNew);
	void SetOptionField(CString strNew);
    void SetConnector(StructConnector* pStructConn);
    void SetNameAtt(CString strNew);
	void SetPathAtt(CString strNew);
    void SetOptionSel(CString strNew);

	CGruppDevice();
	virtual ~CGruppDevice();

protected:
	CString         m_strDeviceName;
	CString         m_strDescription;
	CString         m_strName;
    CStringArray    m_OptionField;
    CPtrList        m_objConnector;
    CString         m_NameAtt;
    CString         m_PathAtt;
    CString         m_strOptionSel;
};

#endif // !defined(AFX_GRUPPDEVICE_H__04910615_28E3_464B_92D5_6B6C1E7F4FBD__INCLUDED_)
