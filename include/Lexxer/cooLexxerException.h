// cooLexxerException.h: interface for the cooLexxerException class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerException
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Any exceptions thrown in this package use this
            exception class. The exception code itself is based
            on a HRESULT code. There is also a possibility to reverse
            lookup the error code as text.
*********************************************************************/

#if !defined(AFX_COOLEXXEREXCEPTION_H__2A2ED892_5CBE_41BC_90E3_533C3A4AF76C__INCLUDED_)
#define AFX_COOLEXXEREXCEPTION_H__2A2ED892_5CBE_41BC_90E3_533C3A4AF76C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "lexxer_error.h"

class cooLexxerException  
{
// Construction/Destruction
public:
	cooLexxerException(HRESULT hr)
       	{	m_hr=hr; };

	virtual ~cooLexxerException()
        {};

// Attributes
protected:
	HRESULT		m_hr;

// Operatoins
public:
	HRESULT		Error() const
		{ return m_hr; };
    LPCTSTR     ErrorCode() const
        { return LookupError(m_hr)->lpszDesc; };
};

#endif // !defined(AFX_COOLEXXEREXCEPTION_H__2A2ED892_5CBE_41BC_90E3_533C3A4AF76C__INCLUDED_)
