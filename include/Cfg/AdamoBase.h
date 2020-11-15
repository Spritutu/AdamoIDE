// AdamoBase.h: interface for the CAdamoBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADAMOBASE_H__2548CF91_D9FF_4E8C_BC2A_200B3B5052A4__INCLUDED_)
#define AFX_ADAMOBASE_H__2548CF91_D9FF_4E8C_BC2A_200B3B5052A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_DEV_SPARE              32

#define MAX_AXIS_SPARE_AXES     844
#define MAX_AXIS_SPARE_ENCODER  1024
#define MAX_AXIS_SPARE_INP_DIG  32
#define MAX_AXIS_SPARE_OUT_DIG  32
#define MAX_AXIS_SPARE_INP_ANAG 16
#define MAX_AXIS_SPARE_OUT_ANAG 16
#define MAX_AXIS_SPARE_TIMER    32
#define MAX_AXIS_SPARE_COUNTER  32
#define MAX_AXIS_SPARE_CAN      32

class CAdamoBase : public CObject  
{
    DECLARE_SERIAL(CAdamoBase)
public:
   
    CAdamoBase();
    CAdamoBase(CAdamoBase& ab);
	virtual ~CAdamoBase();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);

    void SetParentObj(CObject *pObj);
    CObject *GetParentObj();
    CString GetPathName(bool bNoMachine = false);
    void SetName(CString str)   { m_sName = str; }
    void SetDesc(CString str)   { m_sDesc = str; }
    void SetRAcces(int num)     { m_nRAcces = num; }
    void SetWAcces(int num)     { m_nWAcces = num; }
    void SetIsPublic(int num)   { m_isPublic = num; }
    CString GetName()           { return m_sName; }
    CString GetDesc()           { return m_sDesc; }
    int GetRAcces()             { return m_nRAcces; }
    int GetWAcces()             { return m_nWAcces; }
    int GetIsPublic()           { return m_isPublic; }
    void SetDeviceID (int nID)  { m_nID=nID; }
    int  GetDeviceID ()         { return m_nID; }
    virtual enDispositivi GetType()    { return enBase; }
    CAdamoBase& operator= (const CAdamoBase& ab);
protected:
    CString m_sName;
	CString m_sDesc;
    int m_nRAcces;
    int m_nWAcces;
    int m_isPublic;
    int m_nID;
    CObject *m_pObjParent;
    char m_cSpare[MAX_DEV_SPARE];
private :
	CString GetHWAddress (stAdamoParametri *pAdamoParametri);
};

#endif // !defined(AFX_ADAMOBASE_H__2548CF91_D9FF_4E8C_BC2A_200B3B5052A4__INCLUDED_)
