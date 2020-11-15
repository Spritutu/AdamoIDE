// cooLexxerMapData.h: interface for the cooLexxerMapData class.
//
//////////////////////////////////////////////////////////////////////

/*********************************************************************
Class:      cooLexxerMapData
Author:     Alexander Berthold
Copyright:  Alexander Berthold
Date:       19.4.2001
Purpose:    Contains the data of one node in the tree of tokens.
            See cooLexxerMap for more details.
*********************************************************************/

#if !defined(AFX_COOLEXXERMAPDATA_H__91BAE3D7_529C_42AB_B513_8396B264530D__INCLUDED_)
#define AFX_COOLEXXERMAPDATA_H__91BAE3D7_529C_42AB_B513_8396B264530D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cooLexxerTokenRule;
class cooLexxerContext;

class cooLexxerMapData : 
	public std::multimap<TCHAR,cooLexxerMapData*>,
	public ctkCheckValid
{
// Construction/Destruction
public:
	cooLexxerMapData();
	virtual ~cooLexxerMapData();

// Attributes
protected:
	bool					m_fComputed;
	cooLexxerTokenRule*		m_pRule;		// m_pRule: The rule (if any)

// Operations
public:

#ifdef _DEBUG
	virtual bool			fCheckValid() const;
	static bool				fRunDiagnostics();
#else
	virtual bool			fCheckValid() const { return true; };
#endif

	bool				fIsLeaf()		{	return (m_pRule!=NULL);	};
	cooLexxerTokenRule*	pltrGetRule()	{	return m_pRule; };
	const cooLexxerTokenRule* pltrGetRuleConst() const { return m_pRule; };

	cooLexxerMapData	*plmdInsert(TCHAR tcChar, cooLexxerTokenRule* pRule);
	cooLexxerMapData	*plmdDoesApply(const cooLexxerContext* plcContextInfo, TCHAR tcChar, bool *pfComplete, 
									   int nStartPos, int nCurPos);
	void				vApplied(cooLexxerContext* plcContextInfo, TCHAR tcChar, bool fComplete, 
								 int nStartPos, int nCurPos);

	friend class cooLexxerMap;
};

#endif // !defined(AFX_COOLEXXERMAPDATA_H__91BAE3D7_529C_42AB_B513_8396B264530D__INCLUDED_)
