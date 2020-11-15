/*
** AdamoTimer.h
*/ 

#if !defined(AFX_ADAMOTIMER_H__B51E0EBC_A5FE_40C8_A859_AB0530A66B88__INCLUDED_)
#define AFX_ADAMOTIMER_H__B51E0EBC_A5FE_40C8_A859_AB0530A66B88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoTimer : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoTimer)
public:
	CAdamoTimer();
	virtual ~CAdamoTimer();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
	virtual enDispositivi GetType()        { return enTimer; }
    void SetTime(int nPeriod)              { m_nPeriod = nPeriod; }
    int  GetTime()                         { return m_nPeriod; }
    void SetEnabled (int bEnabled)         { m_bEnabled=bEnabled; }
    int  GetEnabled ()                     { return m_bEnabled; }
    void SetUpDown (int  nUpDown)          { m_nUpDown=nUpDown; }
    int  GetUpDown ()                      { return m_nUpDown; }
    void SetStored (int  nStore)           { m_nStored=nStore; }
    int  GetStored ()                      { return m_nStored; }
protected:
    int    m_nPeriod;
	int    m_bEnabled;
    int    m_nUpDown;
    int    m_nStored;
};

#endif // !defined(AFX_ADAMOTIMER_H__B51E0EBC_A5FE_40C8_A859_AB0530A66B88__INCLUDED_)
