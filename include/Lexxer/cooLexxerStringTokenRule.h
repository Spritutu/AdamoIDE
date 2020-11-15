// cooLexxerStringTokenRule.h: interface for the cooLexxerStringTokenRule class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerStringTokenRule
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Computed rule which recognizes string tokens.
            Processes strings embraced by "". Accepts escape
            sequences like \", \x0d.
*********************************************************************/

#if !defined(AFX_COOLEXXERSTRINGTOKENRULE_H__79C1EEB5_347E_4722_9223_11E25DC0E2EB__INCLUDED_)
#define AFX_COOLEXXERSTRINGTOKENRULE_H__79C1EEB5_347E_4722_9223_11E25DC0E2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum	spcc_escape_type {
	esc_begin=0,
	esc_octal,
	esc_hex
	};

class cooLexxerStringContextCookie : public cooLexxerContextCookie
{
// Construction/Destruction
public:
	cooLexxerStringContextCookie()
		{
		fCompleted	=false;
		fInEscape	=false;
		};

// Attributes
public:
	bool				fCompleted;
	bool				fInEscape;
	spcc_escape_type	esct;

	std::tstring		strResult;
	std::tstring		strEsc;
};

class cooLexxerStringTokenRule : public cooLexxerTokenRule  
{
// Construction/Destruction
public:
	cooLexxerStringTokenRule(int nIDValue, ooltr_flags ltrf);
	virtual ~cooLexxerStringTokenRule();

// Attributes
protected:

// Operations
public:
	virtual std::tstring strGetTokenString() const	{ return _T("string"); };

	virtual	bool	fCheckValid() const
					{ return true; };

	virtual bool	fDoesApply(	const cooLexxerContext *plcContext, TCHAR tcChar, bool *pfComplete, 
								int nStartPos, int nCurPos) const;
	virtual void	vApplied( cooLexxerContext *plcContext, TCHAR tcChar, bool fComplete, 
							  int nStartPos, int nCurPos) const;
	virtual bool	fGetResultString(const cooLexxerContext *plcContext, std::tstring& strResult) const;
};


#endif // !defined(AFX_COOLEXXERSTRINGTOKENRULE_H__79C1EEB5_347E_4722_9223_11E25DC0E2EB__INCLUDED_)
