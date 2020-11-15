// cooLexxer.h: interface for the cooLexxer class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxer
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    - Main class for the lexical analyzer.
            - Scans the input character stream for tokens.
            - Calls a call-back class ('cooLexxerListener') for each
              recognized token.
            - Uses an incremental scan; should be pretty fast.
            - Can also parse computed tokens, like "text" and 1234.

    You may freely use this code in your projects. If you find
    any bugs or if you make other corrections/enhancements, i'd
    appreciate if you'd let me know about that. My email is

     alexander-berthold@web.de

    If you share this code, please do not remove this text.
*********************************************************************/

#if !defined(AFX_COOLEXXER_H__D79EE6C7_40E0_4452_A7A4_20278169F438__INCLUDED_)
#define AFX_COOLEXXER_H__D79EE6C7_40E0_4452_A7A4_20278169F438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerInputStream;

class cooLexxerListener :
	public ctkCheckValid,
	public ctkExternalObjectPointer
{
// Operations
public:
	virtual	void			vRegisterToken(const std::tstring& strTokenText, const cooLexxerTokenRule* pltrRule) = 0;
};

class cooLexxer
{
// Construction/Destruction
public:
	cooLexxer(cooLexxerInputStream* plisData,
			  cooLexxerMap* plmLexxerMap,
			  cooLexxerListener* pllReceiver);
	virtual ~cooLexxer();

// Attributes
protected:
	// Contains the state of the lexxer
	cooLexxerContext		m_lcContextInfo;
	// The 'tree' containing the tokens
	cooLexxerMap			*m_plmLexxerMap;
	// The input stream
	cooLexxerInputStream	*m_plisData;
	// The listener to the token output
	cooLexxerListener		*m_pllReceiver;

// ctkCheckValid operations
public:
#ifdef _DEBUG
	virtual bool			fCheckValid() const;
	static bool				fRunDiagnostics();
#else
	virtual bool			fCheckValid() const { return true; };
#endif

// Protected operations
protected:
	/*** Internal token recognition methods ***/

	// A delimeting token has been recognized
	void					vDelimTokenRecognized();
	// A non-delimeting token has been recognized
	void					vRecognizeNonDelimTokens(std::tstring strTokenText);


	/*** Post token recognition methods ***/

	// Is called by the lexxer after a token has been recognized
	void					vRegisterToken(const std::tstring& strTokenText, cooLexxerTokenRule* pptrRule);


	/*** Helper functions ***/

	// Find the longest currently active (completed) rule
	cooLexxerContext::cooListEntry	
							*pleFindEarliestCompletedToken();

	// Find the longest applied rule, not yet invalidated rule
	cooLexxerContext::cooListEntry	
							*pleFindEarliestAppliedToken();

	// Mark all active rules except the given one for deletion
	void					vMarkAllForDeletionExcept(const cooLexxerContext::cooListEntry* pEntry);

// Operations
public:
	// Parse the next character, 'fOverrideIsLastChar' tells if the next
	// char is to be treated as it was the last of the stream.
	void					vParseCharacter(bool fOverrideIsLastChar = false);
};

#endif // !defined(AFX_COOLEXXER_H__D79EE6C7_40E0_4452_A7A4_20278169F438__INCLUDED_)
