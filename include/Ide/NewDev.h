// NewDev.h: interface for the CNewDev class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWDEV_H__E3F7A34C_3666_46F5_ABD4_EF9B3420B509__INCLUDED_)
#define AFX_NEWDEV_H__E3F7A34C_3666_46F5_ABD4_EF9B3420B509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNewDev  
{
public:
	CAdamoBase* GetObject();
	CNewDev();
	CNewDev(CRuntimeClass* pClass,CObject *pNode);
	void SetObj(CRuntimeClass* pClass,CObject *pNode);
	virtual ~CNewDev();

protected:
	void NameManageControl(CString strName,CObject *pNode);
	void NameManage(CObject *pNode);
	CAdamoBase* m_pAdamoBase;
};

#endif // !defined(AFX_NEWDEV_H__E3F7A34C_3666_46F5_ABD4_EF9B3420B509__INCLUDED_)
