/*
** AdamoGeneric.h
*/ 

#if !defined(AFX_DGTIN_H__EF3C3325_4AAB_434E_91C9_E235F8A61635__INCLUDED_)
#define AFX_DGTIN_H__EF3C3325_4AAB_434E_91C9_E235F8A61635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdamoGenericCan : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoGenericCan)
public:
	CAdamoGenericCan();
	virtual ~CAdamoGenericCan();
    virtual void Serialize(CArchive &ar);
	virtual void Serialize(stAdamoParametri *pAdamoParametri, CStdioFile *pFile);
    virtual enDispositivi GetType()    {
        return enGenericCan;
    }
};

#endif // !defined(AFX_DGTIN_H__EF3C3325_4AAB_434E_91C9_E235F8A61635__INCLUDED_)
