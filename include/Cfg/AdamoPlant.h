/*
** AdamoPlant.h
*/ 

#if !defined(AFX_ADAMOPLANT_H__9CC35624_08B8_4A2E_99D8_D42D7A8D5E51__INCLUDED_)
#define AFX_ADAMOPLANT_H__9CC35624_08B8_4A2E_99D8_D42D7A8D5E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "project.h"

typedef CTypedPtrList<CObList,CProject*> CAdamoProjectList;

class CAdamoPlant : public CObject
{
    DECLARE_SERIAL(CAdamoPlant)
public:
	void Close ();
    void DeleteObject(CObject *pObj);
	int GetCount();
    CProject* GetFirst();
    CProject* GetFirst(POSITION& pos);
	void Init();
	CObject* GetNext();
	CObject* GetNext(POSITION& pos);
	BOOL More();
	void AddTail(CObject *mdl);
	CAdamoPlant();
	virtual ~CAdamoPlant();
    virtual void Serialize(CArchive &ar);
	CProject *GetProject (int nModuleNumber);
protected:
    CAdamoProjectList m_lAdamoModule;
    POSITION m_curPos;
};

#endif // !defined(AFX_ADAMOPLANT_H__9CC35624_08B8_4A2E_99D8_D42D7A8D5E51__INCLUDED_)
