/*
** AdamoCounter.h
*/ 

#if !defined(AFX_ADAMOCOUNTER_H__A542D63E_460A_4B63_807B_BE736B50A5F4__INCLUDED_)
#define AFX_ADAMOCOUNTER_H__A542D63E_460A_4B63_807B_BE736B50A5F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoCounter : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoCounter)
public:
	CAdamoCounter();
	virtual ~CAdamoCounter();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
    void SetCounterType (int nType)   {
        m_nType=nType;
    }
    int GetCounterType ()   {
        return m_nType;
    }
virtual enDispositivi GetType()    {
        return enCounter;
    }    
private :
    int m_nType;
};

#endif // !defined(AFX_ADAMOCOUNTER_H__A542D63E_460A_4B63_807B_BE736B50A5F4__INCLUDED_)
