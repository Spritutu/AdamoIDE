#include "cooLexxerException.h"
#include <stdarg.h>

#ifndef	_LEXERR_DEFINED
#define _LEXERR_DEFINED

#ifndef TRACE
#define	ASSERT(x)	{if(!(x)) DebugBreak();}
#define VERIFY(x)	{if(!(x)) DebugBreak();}
#define TRACE		OutputDebugStringHlp
#endif

extern	void		OutputDebugStringHlp(LPCTSTR lpszFormat, ...);

class __rgError;

#ifdef	_DECLARE_ERRORS
std::map<HRESULT,const __rgError*>
	g_mapErrList;

const __rgError*	LookupError(HRESULT hr)
	{
	std::map<HRESULT,const __rgError*>::const_iterator it;

	it		=g_mapErrList.find(hr);
	if(it==g_mapErrList.end())
		return NULL;

	return (*it).second;
	}

void	OutputDebugStringHlp(LPCTSTR lpszFormat, ...)
	{
	static	TCHAR	szTemp[2048];
	va_list	vaptr;
	va_start(vaptr,lpszFormat);
	_vstprintf(szTemp,lpszFormat,vaptr);
	va_end(vaptr);

	OutputDebugString(szTemp);
	}
#else
extern std::map<HRESULT,const __rgError*>
	g_mapErrList;

const __rgError*	LookupError(HRESULT hr);
#endif

class	__rgError
{
public:
	HRESULT		hr;
	LPCTSTR		lpszDesc;

public:
	__rgError(LPCTSTR x,UINT sev,UINT fac,UINT code)
		{
		hr		=MAKE_HRESULT(sev,fac,code);
		lpszDesc=x;
		g_mapErrList.insert(std::map<HRESULT,const __rgError*>::value_type(hr,this));
		};

	operator HRESULT() const
		{
		return hr;
		}
};

#define	FACILITY_GENERAL		301
#define	FACILITY_LEXICAL		302
#define	FACILITY_ANALYZER		303

#ifdef	_DECLARE_ERRORS
#define	DEF_ERROR(x,sev,fac,code)	\
	__rgError			x(_T(#x),sev,fac,code);	\

#else
#define	DEF_ERROR(x,sev,fac,code)	\
	extern	__rgError	x;	\

#endif

// Generelle Fehler
DEF_ERROR(ERR_COMPILER_UNSPECIFIED,3,FACILITY_GENERAL,0xFFFF)
DEF_ERROR(ERR_UNSUPPORTED,3,FACILITY_GENERAL,0)
DEF_ERROR(ERR_INVALID_RULE,3,FACILITY_GENERAL,1)
DEF_ERROR(ERR_INVALID_TOKEN,3,FACILITY_GENERAL,2)
DEF_ERROR(ERR_UNEXPECTED_TOKEN,3,FACILITY_GENERAL,3)
DEF_ERROR(ERR_PUTBACK_UNEXPECTED,3,FACILITY_GENERAL,4)
DEF_ERROR(ERR_CONTEXT_DOES_NOT_EXIST,3,FACILITY_GENERAL,5)
DEF_ERROR(ERR_TOKEN_RULE_INIT,3,FACILITY_GENERAL,6)
DEF_ERROR(ERR_UNEXPECTED_NULL_PARAMETER,3,FACILITY_GENERAL,7)
DEF_ERROR(ERR_READ_PAST_EOF,3,FACILITY_GENERAL,8)
DEF_ERROR(ERR_INVALID_RULE_SET,3,FACILITY_GENERAL,9)

// Lexikalische Fehler
DEF_ERROR(LEXERR_BRACE_LEVEL_TOO_HIGH,1,FACILITY_LEXICAL,1)
DEF_ERROR(LEXERR_BRACE_LEVEL_WRONG_CLOSING_BRACE,1,FACILITY_LEXICAL,2)
DEF_ERROR(LEXERR_BRACE_LEVEL_PAST_LAST,1,FACILITY_LEXICAL,3)
DEF_ERROR(LEXERR_INVALID_ESCAPE_SEQUENCE,1,FACILITY_LEXICAL,4)

// Analyzer Fehler
DEF_ERROR(ANERR_RULEDEF_WRONG,3,FACILITY_ANALYZER,1)
DEF_ERROR(ANERR_RULEDEF_EXPR_WRONG,3,FACILITY_ANALYZER,2)
DEF_ERROR(ANERR_RULEDEF_EXPR_EMPTY,3,FACILITY_ANALYZER,3)
DEF_ERROR(ANERR_RULEDEF_PART_UNDEFINED,3,FACILITY_ANALYZER,4)
DEF_ERROR(ANERR_UNKNOWN_RULE,3,FACILITY_ANALYZER,5)
DEF_ERROR(ANERR_RULEDEF_INVALID,3,FACILITY_ANALYZER,6)
#endif