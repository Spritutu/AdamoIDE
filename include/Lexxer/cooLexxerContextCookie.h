// cooLexxerContextCookie.h: interface for the cooLexxerContextCookie class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerContextCookie
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    This class is the base class for custom cookies.
            See cooLexxerStringContextCookie, 
            cooLexxerNumberContextCookie for applications.
*********************************************************************/

#if !defined(AFX_COOLEXXERCONTEXTCOOKIE_H__C591692E_3C3D_4E2B_A9F2_C7697F22EED5__INCLUDED_)
#define AFX_COOLEXXERCONTEXTCOOKIE_H__C591692E_3C3D_4E2B_A9F2_C7697F22EED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerContextCookie :
	public ctkCheckValid
{
// Construction/Destruction
public:
	cooLexxerContextCookie()
        {};
	virtual ~cooLexxerContextCookie()
        {};

// Attributes
protected:
	DWORD			m_dwMyCookieID;

// Operations
public:
	virtual	bool	fCheckValid() const
					{ return true; };
	DWORD			dwGetCookieID()
					{	return m_dwMyCookieID;	};

	friend class cooLexxerContext;
};

#endif // !defined(AFX_COOLEXXERCONTEXTCOOKIE_H__C591692E_3C3D_4E2B_A9F2_C7697F22EED5__INCLUDED_)
