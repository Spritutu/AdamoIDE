/*
** AdamoDgtIn.h
*/ 

#if !defined(AFX_DGTIN_H__EF3C3325_4AAB_434E_91C9_E295F8A61635__INCLUDED_)
#define AFX_DGTIN_H__EF3C3325_4AAB_434E_91C9_E295F8A61635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoDgtIn : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoDgtIn)
    int m_nNumInput;
public:
	CAdamoDgtIn();
	virtual ~CAdamoDgtIn();
    virtual enDispositivi GetType() { return enInputDigitale; }
    int  GetNumInput ()              { return m_nNumInput; } 
    void SetNumInput (int nNumInput) { m_nNumInput=nNumInput; } 
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
};

#endif // !defined(AFX_DGTIN_H__EF3C3325_4AAB_434E_91C9_E295F8A61635__INCLUDED_)
