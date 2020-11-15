// ctkCheckValid.h: interface for the ctkCheckValid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTKCHECKVALID_H__D93BEE27_F124_4065_992B_F5B7A39D4D24__INCLUDED_)
#define AFX_CTKCHECKVALID_H__D93BEE27_F124_4065_992B_F5B7A39D4D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ctkCheckValid  
{
public:
	virtual	bool	fCheckValid() const = 0;

#ifdef _DEBUG
	static bool		fRunDiagnostics()
		{
		DebugBreak();
		}
#endif
};

#endif // !defined(AFX_CTKCHECKVALID_H__D93BEE27_F124_4065_992B_F5B7A39D4D24__INCLUDED_)
