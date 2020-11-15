// cooLexxerInputStream.h: interface for the cooLexxerInputStream class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerInputStream
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Base class for the input stream for the lexxer.
            Must support 'ctkExternalObjectPointer'.
            See 'cooLexxerTextInputStream' for an application.
*********************************************************************/

#if !defined(AFX_COOLEXXERINPUTSTREAM_H__2EEE2883_3AE8_42FD_9387_38FF266EAD49__INCLUDED_)
#define AFX_COOLEXXERINPUTSTREAM_H__2EEE2883_3AE8_42FD_9387_38FF266EAD49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerInputStream :
	public ctkExternalObjectPointer,
	public ctkCheckValid
{
// Construction
public:
	cooLexxerInputStream()
        {};
	virtual ~cooLexxerInputStream()
        {};

// Attributes
protected:

// Operations
public:
	virtual TCHAR	tcGetNextCharacter() = 0;
	virtual bool	fPutBack(LPCTSTR lpszText) = 0;
	virtual bool	fPutBack(TCHAR tcChar) = 0;
	virtual bool	fIsEofReached() const = 0;
};

#endif // !defined(AFX_COOLEXXERINPUTSTREAM_H__2EEE2883_3AE8_42FD_9387_38FF266EAD49__INCLUDED_)
