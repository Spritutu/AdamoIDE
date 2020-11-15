/*
** CSyntaxContainer.h
*/
#pragma once

#include <list>

class CAdamoParameter
{
	public :
		CAdamoParameter () : m_bOptional (false)   {
		}
		CString		m_strName;
		CString		m_strDescription;
		CString     m_strEnum;
		CStringList m_strValues;
		bool		m_bOptional;
};

class CAdamoFunction
{
	public :
		CAdamoFunction ();
		~CAdamoFunction ();
	private :
		CString m_strName;
		CString m_strDescription;
		CString m_strHelp;
		int     m_nLine;
		list<CAdamoParameter *> m_listParameters;
	public :
		void SetName (CString strName)	{
			m_strName = strName;
		}
		void SetDescription (CString strDescription)   {
			m_strDescription = strDescription;
		}
		void SetHelp (CString strHelp)   {
			m_strHelp = strHelp;
		}
		void SetLine (int nLine)   {
			m_nLine = nLine;
		}
		CString GetName ()   {
			return m_strName;
		}
		CString GetDescription ()   {
			return m_strDescription;
		}
		CString GetHelp ()   {
			return m_strHelp;
		}
		int GetLine ()   {
			return m_nLine;
		}
		void AddParameter (CAdamoParameter* pAdamoParameter);
		void GetParameterList (CStringList& lParameters, CStringList& lDescription);
		void GetParameterValues (CString strParameter, CStringList& lValues);
		void GetParameterEnum (CString strParameter, CString& strEnum);
		int GetCount ()		{
			return m_listParameters.size ();
		}
		list<CAdamoParameter *>* GetListParameters ()   {
			return &m_listParameters;
		}
		CAdamoFunction& operator = (CAdamoFunction& function);
};

class CAdamoLibrary
{
	public :
		CAdamoLibrary ();
		~CAdamoLibrary ();
	private :
		CString m_strName;
		CString m_strDescription;
		list<CAdamoFunction *> m_listFunctions;
	public :
		void SetName (CString strName)   {
			m_strName = strName;
		}
		void SetDescription (CString strDescription)   {
			m_strDescription = strDescription;
		}
		CString GetName ()   {
			return m_strName;
		}
		CString GetDescription ()   {
			return m_strDescription;
		}
		int GetCount ()   {
			return m_listFunctions.size ();
		}
		list<CAdamoFunction *>* GetListFunctions ()   {
			return &m_listFunctions;
		}
		void AddFunction (CAdamoFunction* pAdamoFunction);
		void GetFunctionsList (CStringList& lFunctions, CStringList& lDescription);
		void GetParametersList (CString strFunction, CStringList& lParameters, CStringList& lDescription);
		void GetParametersValues (CString strFunction, CString strParameter, CStringList& lValues);
		void GetParametersEnum (CString strFunction, CString strParameter, CString& strEnum);
		bool ExistFunction (CString strFunction);
		bool DeleteFunction (CString strFunction);
		bool GetFunctionHelp (CString strFunction, CString& strHelp);
		bool GetFunctionLine (CString strFunction, int& nLine);
		void UpdateLineNumbers (int nStartLine, int nUpdateLines);
};

class CAdamoEnumValues
{
public :
	CString m_strName;
	CString m_strValue;
};

class CAdamoEnum
{
	public :
		CAdamoEnum ();
		~CAdamoEnum ();
	private :
		CString m_strName;
		CString m_strDescription;
		CString m_strValue;
		list<CAdamoEnumValues *> m_listEnumValues;
	public :
		void SetName (CString strName)   {
			m_strName = strName;
		}
		void SetDescription (CString strDescription)   {
			m_strDescription = strDescription;
		}
		CString GetName ()   {
			return m_strName;
		}
		CString GetDescription ()   {
			return m_strDescription;
		}
		int GetCount ()   {
			return m_listEnumValues.size ();
		}
		void AddEnumValue (CAdamoEnumValues* pEnumValues);
		void GetEnumList (CStringList& lEnum, CStringList& lEnumValue);
};

class CSyntaxContainer
{
	public :
		CSyntaxContainer ();
		~CSyntaxContainer ();
	protected :
		CString m_strName;
		list<CAdamoLibrary *> m_listLibraries;
		list<CAdamoEnum *>    m_listEnums;
		map<CString, CString> m_mapEnumValues;
	public :
		void AddLibrary (CAdamoLibrary* pLibrary);
		void AddEnum (CAdamoEnum *pEnum);
		HRESULT Load ();
		int  GetCount ()   {
			return m_listLibraries.size ();
		}
		void GetLibrariesList (CStringList& lOptions, CStringList& lDescription);
		void GetFunctionsList (CString strLibrary, CStringList& lFunctions, CStringList& lDescription);
		void GetParametersList (CString strLibrary, CString strFunction, CStringList& lParameters, CStringList& lDescription);
		void GetParametersValues (CString strLibrary, CString strFunction, CString strParameter, CStringList& lValues);
		void GetParametersEnum (CString strLibrary, CString strFunction, CString strParameter, CString& strEnum);
		bool GetFunctionHelp (CString strLibrary, CString strFunction, CString& strHelp);
		bool GetFunctionLine (CString strLibrary, CString strFunction, int& nLine);
		void BuildEnumMap ();
		void GetEnumList (CStringList& lEnumValue);
		bool GetEnumValue (CString strEnumKey, CString& strEnumValue);
		void GetEnumValues (CString strEnumKey, CStringList& strEnum, CStringList& strEnumValue);
		bool ExistFunction (CString strLibrary, CString strFunction);
		virtual CString GetPath ();
		virtual CString GetWorkPath ();
		list<CAdamoLibrary *>* GetListLibraries ()	{ return &m_listLibraries; }
	protected :
		CString GetLanguageExtension ();
		void LoadSyntax (DocXmlPtr pDocXml);
};


