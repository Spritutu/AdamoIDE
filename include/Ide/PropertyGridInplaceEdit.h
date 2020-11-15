// PropertyGridInplaceEdit.h: interface for the CPropertyGridInplaceEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYGRIDINPLACEEDIT_H__4447B5CF_8E01_4898_9B5E_2733FA4E57A8__INCLUDED_)
#define AFX_PROPERTYGRIDINPLACEEDIT_H__4447B5CF_8E01_4898_9B5E_2733FA4E57A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VisValEdit.h"

class CPropertyGridInplaceEdit : public CVisValidEdit
{
   	DECLARE_DYNAMIC(CPropertyGridInplaceEdit)
public:
    enum enuType {ED_ALL, ED_NAME, ED_DESC, ED_DECIMAL, ED_FLOAT};   
    CPropertyGridInplaceEdit();    
    void SetControlType(enuType enumType);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PROPERTYGRIDINPLACEEDIT_H__4447B5CF_8E01_4898_9B5E_2733FA4E57A8__INCLUDED_)
