/*
** AdamoDgtOut.h
*/ 

#if !defined(AFX_ADAMODGTOUT_H__B186E62D_C2A3_43EE_8D88_5E2F7425AA95__INCLUDED_)
#define AFX_ADAMODGTOUT_H__B186E62D_C2A3_43EE_8D88_5E2F7425AA95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoDgtOut : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoDgtOut)
public:
	CAdamoDgtOut();
	virtual ~CAdamoDgtOut();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
    void SetIsMonoStable(int num)       { m_nIsMonoStable = num; }
    int  GetIsMonoStable()              { return m_nIsMonoStable; }
    virtual enDispositivi GetType()     { return enOutputDigitale; }
    int  GetNumOutput ()                { return m_nNumOutput; } 
    void SetNumOutput (int nNumOutput)  { m_nNumOutput=nNumOutput; }
    void SetTimeOut (int nTimeOut)      { m_nTimeOut=nTimeOut; }
    int  GetTimeOut ()                  { return m_nTimeOut; }
protected:
    int m_nIsMonoStable;
    int m_nTimeOut;
    int m_nNumOutput;
};

#endif // !defined(AFX_ADAMODGTOUT_H__B186E62D_C2A3_43EE_8D88_5E2F7425AA95__INCLUDED_)
