/*
** AdamoAnalogIn.h
*/ 

#if !defined(AFX_ADAMOANALOGIN_H__85BAD639_AA52_4FF5_9D0B_81DC102491A1__INCLUDED_)
#define AFX_ADAMOANALOGIN_H__85BAD639_AA52_4FF5_9D0B_81DC102491A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoAnalogIn : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoAnalogIn)
public:
	CAdamoAnalogIn();
	virtual ~CAdamoAnalogIn();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
    void SetAnalogType(int num)   { m_nType = num; }
    int GetAnalogType ()          { return m_nType; }
    virtual enDispositivi GetType()    { return enInputAnalogico; }
    void SetLLInf (double fpLLInf)     { m_fpLLInf=fpLLInf; }
    void SetLLSup (double fpLLSup)     { m_fpLLSup=fpLLSup; }
    double GetLLInf ()                 { return m_fpLLInf; }
    double GetLLSup ()                 { return m_fpLLSup; }
protected:
    int m_nType;
    double m_fpLLInf;
    double m_fpLLSup;
};

#endif // !defined(AFX_ADAMOANALOGIN_H__85BAD639_AA52_4FF5_9D0B_81DC102491A1__INCLUDED_)
