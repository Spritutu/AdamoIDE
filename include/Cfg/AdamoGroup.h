/*
** AdamoGroup.h
*/ 

#if !defined(AFX_ADAMOGROUP_H__B4CC4709_1156_449F_89D2_757B643E316D__INCLUDED_)
#define AFX_ADAMOGROUP_H__B4CC4709_1156_449F_89D2_757B643E316D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdamoBaseList.h"

class CAdamoGroup : public CAdamoBaseList
{
    DECLARE_SERIAL(CAdamoGroup)
public:
	void Init();
	CObject* GetNext();
	BOOL More();
	void DevInit();
	CObject* DevGetNext();
	BOOL DevMore();
	void AddTail(CObject *obj);
    BOOL DeleteObject(CObject *pObj);
	CAdamoGroup();
	virtual ~CAdamoGroup();
    virtual void Serialize(CArchive &ar);
    int GetCount ()   { return m_lAdamoGroup.GetCount (); }
    int GetDevCount ()   { return m_lAdamoList.GetCount (); }
protected:
	CAdamoExList *m_pEx;
    CAdamoExList *m_pExList;
    CTypedPtrList<CObList,CAdamoGroup*> m_lAdamoGroup;
};

#endif // !defined(AFX_ADAMOGROUP_H__B4CC4709_1156_449F_89D2_757B643E316D__INCLUDED_)
