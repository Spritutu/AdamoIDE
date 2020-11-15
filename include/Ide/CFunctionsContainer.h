/*
** CFunctionsContainer.h
*/
#pragma once

#include <list>
#include "CSyntaxContainer.h"

class CFunctionsContainer : public CSyntaxContainer
{
	private :
		CString m_strPath;
	public :
		CFunctionsContainer ();
		~CFunctionsContainer ();
		int  GetCount ()   {
			return m_listLibraries.size ();
		}
		void Save ();
		void ClearAll ();
		void UpdateLineNumbers (CString strModule, int nStartLine, int nUpdateLines, int nDeleteLine);
		bool ExistFunction (CString strFunction, CString& strModule);
		bool DeleteFunction (CString strFunction);
        bool RemoveModule (CString strModule);
		void SetPath (CString strPath)	{ m_strPath = strPath; }
	protected :
		CString GetPath ();
	private :
		void WritePrologue (CStdioFile* pStdioFile);
		void WriteEpilogue (CStdioFile* pStdioFile);
		void WriteData (CStdioFile* pStdioFile);
};
