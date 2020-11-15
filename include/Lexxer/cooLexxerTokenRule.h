// cooLexxerTokenRule.h: interface for the cooLexxerTokenRule class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerTokenRule
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Base class for rules for the lexical analyzer 'lexxer'
*********************************************************************/

#if !defined(AFX_COOLEXXERTOKENRULE_H__CE5A4B37_A2E3_44BD_BFB7_099D65CD9B7F__INCLUDED_)
#define AFX_COOLEXXERTOKENRULE_H__CE5A4B37_A2E3_44BD_BFB7_099D65CD9B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerContext;

class cooLexxerTokenRule : 
	public ctkFlagsMixin<ooltr_flags>,
	public ctkCheckValid
{
// Construction/Destruction
protected:
	// lpszToken==NULL: Not a 'character' - Rule
	// lpszToken!=NULL: Character - Rule
	cooLexxerTokenRule(int nIDValue, ooltr_flags ltrf, LPCTSTR lpszToken);

public: 
	virtual ~cooLexxerTokenRule();

// Attributes
private:
	std::tstring		m_strTokenString;
    int                 m_nIDValue;

// Operations
public:
    int                 nGetIDValue() const
                        { return m_nIDValue; };

	virtual	bool		fCheckValid() const
						{ return true; };

	virtual std::tstring strGetTokenString() const;

	virtual bool		fDoesApply(	const cooLexxerContext *ppcContext, TCHAR tcChar, bool *pfComplete, 
									int nStartPos, int nCurPos) const = 0;
	virtual void		vApplied( cooLexxerContext *ppcContext, TCHAR tcChar, bool fComplete, 
								  int nStartPos, int nCurPos) const = 0;
	virtual bool		fGetResultString(const cooLexxerContext *ppcContext, std::tstring& strResult) const = 0;
};

#endif // !defined(AFX_COOLEXXERTOKENRULE_H__CE5A4B37_A2E3_44BD_BFB7_099D65CD9B7F__INCLUDED_)
