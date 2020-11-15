/*
**
*/

#include <array>

#define NMAXLUALIBMODULES 64
#define HEADER_LUA_FILES  "@@@__THIRALIB__@@@"
#define VERSION_LUA_FILES 0x02090006

class CAdamoLuaLibraryModule
{
	public :
		CAdamoLuaLibraryModule () : m_pObjCode (NULL), m_nXmlLenght (0)  { }
		~CAdamoLuaLibraryModule ()  { Clear (); }
		void SetName (CString strName)   { m_strName = strName; }
		CString GetName ()				 { return m_strName; }
		int AddObjCode (unsigned char *pObjCode, int nSize);
		int AddFunction (CAdamoFunction* pAdamoFunction);
		int AddObject   (CAdamoObj* pAdamoObject);
		int ReadObjCode (CString strPath);
		int	SerializeWrite (CFile* file);
		int	SerializeRead (CFile* file);
		list<CAdamoFunction*>* GetListFunctions ()	{ return &m_listFunctions; }
		list<CAdamoObj*>*	   GetListObjects ()	{ return &m_listObjects; }
	private :
		void Clear ();
		CAdamoFunction* ExistFunction (CString strName);
		CAdamoObj*		ExistObj (CString strName);
		void			ClearFunctions ();
		void			ClearObjects ();
		int				WriteName (CFile* file);
		int				WriteCode (CFile* file);
		int				WriteXml (CFile* file);
		int				WriteXmlPrologue (CFile* file);
		int				WriteXmlFunctions (CFile* file);
		int				WriteXmlObjects (CFile* file);
		int				WriteXmlEpilogue (CFile* file);
		int				WriteString (CFile* file, CString str);
		int				ReadName (CFile* file);
		int				ReadCode (CFile* file);
		int				ReadXml (CFile* file);
		int				ParseXML ();
	private :
		CString m_strName;
		unsigned char* m_pObjCode;
		int m_nObjCodeSize;
		list<CAdamoFunction*> m_listFunctions;
		list<CAdamoObj*> m_listObjects;
		int	m_nXmlLenght;
        DocXmlPtr m_pDocXml;
};

class CAdamoLuaLibrary
{
	public :
		CAdamoLuaLibrary () : m_nNumModules (0)  {
			for (int i = 0; i<NMAXLUALIBMODULES; i++)
				m_arrayLibraryModules.at (i) = NULL;
		}
		~CAdamoLuaLibrary ()   {
			for (int i = 0; i<m_nNumModules; i++)
				delete m_arrayLibraryModules[i];
		}
		void SetDescription (CString strDescription)	{ m_strDescription = strDescription; }
		CString GetDescription ()						{ return m_strDescription; }
		int AddLibraryModule (CString strName, CAdamoLuaLibraryModule* pLuaModule);
		int RemoveLibraryModule (CString strName);
		CAdamoLuaLibraryModule* ExistLibraryModule (CString strModuleName);
		int Load (CString strPath);
		int Save (CString strPath);
		int GetLibraryModules (CStringList& listModules);
		int GetLibraryModulesCount ()									{ return m_nNumModules; }
		CAdamoLuaLibraryModule* GetLibraryModules (int nModuleIndex)	{ return m_arrayLibraryModules[nModuleIndex]; }
		static int IsLuaLibraryFile (CString strPath, int& nVersion, CString& strDescription);
		void MoveModuleUp (CString strModuleName);
		void MoveModuleDown (CString strModuleName);
	private :
		int ReadDescription (CFile* file);
		int ReadModules (CFile* file);
		int WriteDescription (CFile* file);
		int WriteModules (CFile* file);
		int ReadHeader (CFile* file);
		int ReadVersion (CFile* file);
		int WriteHeader (CFile* file);
		int WriteVersion (CFile* file);
	private :
		tr1::array<CAdamoLuaLibraryModule*, NMAXLUALIBMODULES> m_arrayLibraryModules;
		CString m_strPathName;
		CString m_strDescription;
		int m_nNumModules;
		int m_nVersion;
};
