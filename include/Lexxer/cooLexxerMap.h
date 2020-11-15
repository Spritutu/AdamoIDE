// cooLexxerMap.h: interface for the cooLexxerMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLEXXERMAP_H__F4E70A9E_72B3_4707_85A1_C5FDB8CE26C3__INCLUDED_)
#define AFX_COOLEXXERMAP_H__F4E70A9E_72B3_4707_85A1_C5FDB8CE26C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*********************************************************************
Class:      cooLexxerMap
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Container class for the nested maps of rules.
            A short explanation:

            - There a two different sets of rules: 'first position'
              rules and 'within' rules. 'within' rules can appear
              anywhere, like the C++ bracket ('(') token. 'First position' rules
              are those which apply only to a complete string. Sample:

              return(5);

              Here, '(' is found and breaks the input text into
              'return' and '('. If there is a 'first position' rule which
              matches 'return', it is used and recognized. If not, the
              'NULL' rule applies.

            - To speed up the recognition process, all rules which could
              apply are tested in parallel. C++ operators '=' and '==', 
              for example, both start with the same character. So the
              tokenizer finds if it happens to read a '=' both rules
              could be valid. If the next token is _not_ another '=',
              it recognizes an '=' token. If it _is_ '=', then the
              '==' token is recognized.

            - This is done by traversing a tree structure, which 
              would look like this for the tokens 'if', 'elseif', 'else'
              and 'endif':

              1st  2nd  3rd  4th  5th  6th  7th

              'i'--'f'--(*)

              'e'--'l'--'s'--'e'--(*)
               |              |
               |              +---'i'--'f'--(*)
               |
               +---'n'--'d'--'i'--'f'--(*)

              (*) stands for a recognized token.

                
*********************************************************************/

class cooLexxerMap :
	public ctkCheckValid
{
// Construction/Destruction
public:
	cooLexxerMap();
    cooLexxerMap(std::basic_istream<TCHAR>& input);
	virtual ~cooLexxerMap();

// Typedefs
public:
	typedef std::vector<cooLexxerTokenRule*>
						ltrdel_vec_type;

// Attributes
protected:
	// Map of rules which apply only from the first character of a new token
	// (FP = first position)
	cooLexxerMapData	m_lmdFPRules;
	// Map of rules which can appear also within other tokens (WI=within)
	cooLexxerMapData	m_lmdWIRules;

	ltrdel_vec_type		m_vecRulesToDelete;

// Protected operations
protected:
	bool			fRegisterCharRule(cooLexxerMapData& maplRules, 
									  cooLexxerCharTokenRule* pRule);
	bool			fRegisterComputedRule(cooLexxerMapData& maplRules, 
										  cooLexxerTokenRule* pRule);

// ctkCheckValid operations
public:
#ifdef _DEBUG
	virtual bool	fCheckValid() const;
	static bool		fRunDiagnostics();
#else
	virtual bool	fCheckValid() const { return true; };
#endif

// Operations
public:
	bool			fRegisterRule(cooLexxerTokenRule* pRule);

	const cooLexxerMapData* plmdGetFPRules() const { return &m_lmdFPRules; };
	const cooLexxerMapData* plmdGetWIRules() const { return &m_lmdWIRules; };

	bool			fBuildRule(int nIDValue, std::tstring strToken, bool fSeperator);
	void			vLoadFromStream(std::basic_istream<TCHAR>& input);

	friend class cooLexxer;
};

#endif // !defined(AFX_COOLEXXERMAP_H__F4E70A9E_72B3_4707_85A1_C5FDB8CE26C3__INCLUDED_)
