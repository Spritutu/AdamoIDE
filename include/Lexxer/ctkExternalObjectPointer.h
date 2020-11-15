// ctkExternalObjectPointer.h: interface for the ctkExternalObjectPointer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTKEXTERNALOBJECTPOINTER_H__49346FC5_909A_4E7E_8DC4_82A333DBA7FA__INCLUDED_)
#define AFX_CTKEXTERNALOBJECTPOINTER_H__49346FC5_909A_4E7E_8DC4_82A333DBA7FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ctkExternalObjectPointer  
{
public:
	virtual	bool	fShouldDelete() const = 0;
};

template<bool fShouldDelete>
class ctkExternalObjectPointerF
: public ctkExternalObjectPointer
{
public:
	virtual bool	fShouldDelete() const
		{
		return fShouldDelete;
		};
};

#endif // !defined(AFX_CTKEXTERNALOBJECTPOINTER_H__49346FC5_909A_4E7E_8DC4_82A333DBA7FA__INCLUDED_)
