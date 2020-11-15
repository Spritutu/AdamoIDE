// cooLexxerCharTokenRule.h: interface for the cooLexxerCharTokenRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLEXXERCHARTOKENRULE_H__D823B834_7F51_4D60_942E_5269478B9FC1__INCLUDED_)
#define AFX_COOLEXXERCHARTOKENRULE_H__D823B834_7F51_4D60_942E_5269478B9FC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerCharTokenRule : public cooLexxerTokenRule  
{
// Construction/destruction
public:
	cooLexxerCharTokenRule(int nIDValue, ooltr_flags ltrf, LPCTSTR lpszToken);
	virtual ~cooLexxerCharTokenRule();

// Operations
public:
	// cooParserTokenRule operations
	virtual bool	fDoesApply(const cooLexxerContext *plcContext, TCHAR tcChar, bool *pfCompleted, 
							   int nStartPos, int nCurPos) const;
	virtual void	vApplied( cooLexxerContext *plcContext, TCHAR tcChar, bool fComplete, 
							  int nStartPos, int nCurPos) const;
	virtual bool	fGetResultString(const cooLexxerContext *plcContext, std::tstring& strResult) const
					{ return false; };
};

#endif // !defined(AFX_COOLEXXERCHARTOKENRULE_H__D823B834_7F51_4D60_942E_5269478B9FC1__INCLUDED_)
