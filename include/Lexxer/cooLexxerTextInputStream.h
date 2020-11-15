// cooLexxerTextInputStream.h: interface for the cooLexxerTextInputStream class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerTextInputStream
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Sample input stream class for the parser.
            Takes a LPCTSTR as constructor argument and creates
            a cooLexxerInputStream compatible stream out of it.
*********************************************************************/

#if !defined(AFX_COOLEXXERTEXTINPUTSTREAM_H__CB2EA5EA_2239_4926_B7BF_6CC6972A3E14__INCLUDED_)
#define AFX_COOLEXXERTEXTINPUTSTREAM_H__CB2EA5EA_2239_4926_B7BF_6CC6972A3E14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerTextInputStream : public cooLexxerInputStream  
{
// Construction/Destruction
public:
	cooLexxerTextInputStream(const TCHAR* lpszInputData);
	virtual ~cooLexxerTextInputStream();

// Attributes
protected:
	std::tstring	m_strInputData;
	int				m_nInputPos;
	std::tstring	m_strPutBackData;
	int				m_nPutBackPos;

// Operations
public:
	virtual	bool	fCheckValid() const;
	virtual TCHAR	tcGetNextCharacter();
	virtual bool	fPutBack(LPCTSTR lpszText);
	virtual bool	fPutBack(TCHAR tcChar);
	virtual bool	fIsEofReached() const;
	virtual bool	fShouldDelete() const;
};

#endif // !defined(AFX_COOLEXXERTEXTINPUTSTREAM_H__CB2EA5EA_2239_4926_B7BF_6CC6972A3E14__INCLUDED_)
