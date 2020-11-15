// cooLexxerNumberTokenRule.h: interface for the cooLexxerNumberTokenRule class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerNumberTokenRule
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Computed rule which recognizes numerical tokens.
*********************************************************************/

#if !defined(AFX_COOLEXXERNUMBERTOKENRULE_H__8F7C94B5_9790_48E6_9AFF_7410E39CF591__INCLUDED_)
#define AFX_COOLEXXERNUMBERTOKENRULE_H__8F7C94B5_9790_48E6_9AFF_7410E39CF591__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerNumberContextCookie : public cooLexxerContextCookie
{
// Construction/Destruction
public:
	cooLexxerNumberContextCookie()
		{
		fCompleted	=false;
		};

// Attributes
public:
	bool				fCompleted;
	std::tstring		strResult;
};

class cooLexxerNumberTokenRule  : public cooLexxerTokenRule
{
// Construction/Destruction
public:
	cooLexxerNumberTokenRule(int nIDValue, ooltr_flags ptrf);
	virtual ~cooLexxerNumberTokenRule();

// Attributes
protected:

// Operations
public:
	virtual std::tstring strGetTokenString() const	{ return _T("number"); };

	virtual	bool	fCheckValid() const
					{ return true; };

	virtual bool	fDoesApply(	const cooLexxerContext *ppcContext, TCHAR tcChar, bool *pfComplete, 
								int nStartPos, int nCurPos) const;
	virtual void	vApplied( cooLexxerContext *ppcContext, TCHAR tcChar, bool fComplete, 
							  int nStartPos, int nCurPos) const;
	virtual bool	fGetResultString(const cooLexxerContext *ppcContext, std::tstring& strResult) const;
};

#endif // !defined(AFX_COOLEXXERNUMBERTOKENRULE_H__8F7C94B5_9790_48E6_9AFF_7410E39CF591__INCLUDED_)
