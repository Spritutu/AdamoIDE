/*
** AdamoBaseList.h
*/ 

#if !defined(AFX_ADAMOBASELIST_H__818DA051_CB59_4971_812B_8AEF051CF412__INCLUDED_)
#define AFX_ADAMOBASELIST_H__818DA051_CB59_4971_812B_8AEF051CF412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdamoBase.h"

typedef CTypedPtrList<CObList,CObject*> CAdamoList;

class CAdamoExList
{
public:
    CAdamoExList(CObList *list){ m_curList = list; };
    virtual ~CAdamoExList(){};
	CObject* Current();
	CObject* Next();
	BOOL More();
    void Init();
private:
    POSITION m_curPos;
    CObList *m_curList;
};

class CAdamoBaseList : public CAdamoBase
{
    DECLARE_SERIAL(CAdamoBaseList)
public:
	void AddTail(CObject *obj);
	CAdamoBaseList();
	virtual ~CAdamoBaseList();
    virtual void Serialize(CArchive &ar);
protected:
    CAdamoList m_lAdamoList;
private:
    POSITION m_curPos;
    CObList *m_curList;
};

#endif // !defined(AFX_ADAMOBASELIST_H__818DA051_CB59_4971_812B_8AEF051CF412__INCLUDED_)
