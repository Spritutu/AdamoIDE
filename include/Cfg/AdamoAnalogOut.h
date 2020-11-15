/*
** AdamoAnalogOut.h
*/ 

#if !defined(AFX_ADAMOANALOGOUT_H__C01C8471_CD26_4913_9439_DA4CDD83D88D__INCLUDED_)
#define AFX_ADAMOANALOGOUT_H__C01C8471_CD26_4913_9439_DA4CDD83D88D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoAnalogOut : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoAnalogOut)
public:
	CAdamoAnalogOut();
	virtual ~CAdamoAnalogOut();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
    virtual enDispositivi GetType()    { return enOutputAnalogico; }
    void SetLLInf (double fpLLInf)     { m_fpLLInf=fpLLInf; }
    void SetLLSup (double fpLLSup)     { m_fpLLSup=fpLLSup; }
    double GetLLInf ()                 { return m_fpLLInf; }
    double GetLLSup ()                 { return m_fpLLSup; }
private :
    double m_fpLLInf;
    double m_fpLLSup;
};

#endif // !defined(AFX_ADAMOANALOGOUT_H__C01C8471_CD26_4913_9439_DA4CDD83D88D__INCLUDED_)
