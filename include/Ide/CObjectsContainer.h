/*
** CObjectsContainer.h
*/
#pragma once

#include <list>
#include "CSyntaxContainer.h"

#define OBJ_DIGITAL_INPUT	100
#define OBJ_DIGITAL_OUPUT	101
#define OBJ_ANALOG_INPUT	102
#define OBJ_ANALOG_OUTPUT	103
#define OBJ_AXIS			104
#define OBJ_ENCODER			105
#define OBJ_TIMER			106
#define OBJ_MEMORY			107
#define OBJ_GENERIC_CAN		108

enum eObjectElement
{
	eNoObjEl,
	eData,
	eMethod
};

class CAdamoObjectElement
{
	public :
		CAdamoObjectElement () : m_nDataLine (-1), m_bIsPublic (false)  {
		}
		virtual ~CAdamoObjectElement ()   {
		}
		virtual eObjectElement KindOf () = 0;
	public :
		void SetName (CString strName)
		{
			m_strName = strName;
		}
		void SetDescription (CString strDescription)   {
			m_strDescription = strDescription;
		}
		void SetPublic (bool bPublic)   {
			m_bIsPublic = bPublic;
		}
		void SetDataLine (int nDataLine)   {
			m_nDataLine = nDataLine;
		}
		CString GetName ()   {
			return m_strName;
		}
		CString GetDescription ()   {
			return m_strDescription;
		}
		bool GetPublic ()   {
			return m_bIsPublic;
		}
		int GetDataLine ()   {
			return m_nDataLine;
		}
	private :
		CString m_strName;
		CString m_strDescription;
		bool m_bIsPublic;
		int m_nDataLine;
};

class CAdamoObjectData : public CAdamoObjectElement
{
	public :
		CAdamoObjectData ()	  {
		}
		~CAdamoObjectData ()   {
		}
		eObjectElement KindOf ()   {
			return eData;
		}
		void SetData (CAdamoData& ad)   {
			m_ad = ad;
		}
		CAdamoData& GetData ()   {
			return m_ad;
		}
		CAdamoObjectData& operator = (CAdamoObjectData& p);
	private :
		CAdamoData m_ad;
};

class CAdamoObjectMethod : public CAdamoObjectElement, public CAdamoFunction
{
	public :
		CAdamoObjectMethod () : m_bIsVirtual (false)  {
		}
		~CAdamoObjectMethod ()   {
		}
		eObjectElement KindOf ()   {
			return eMethod;
		}
		void SetVirtual (bool bVirtual)   {
			m_bIsVirtual = bVirtual;
		}
		bool GetVirtual ()   {
			return m_bIsVirtual;
		}
		CAdamoObjectMethod& operator = (CAdamoObjectMethod& p);
	private :
		bool m_bIsVirtual;
};

class CAdamoObj
{
	private :
		list<CAdamoObjectElement *> m_listObjectElement;
		CString m_strName;
		CString m_strDescription;
		CString m_strHelp;
		CString m_strBasedObj;
		int     m_nLine;
	public :
		CAdamoObj ();
		~CAdamoObj ();
		void SetName (CString strName)
		{
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
		void SetBasedObj (CString strBasedObj)   {
			m_strBasedObj = strBasedObj;
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
		CString GetBasedObj ()   {
			return m_strBasedObj;
		}
		int GetCount ()   {
			return m_listObjectElement.size ();
		}
		list<CAdamoObjectElement *>& GetListObjectElements ()   {
			return m_listObjectElement;
		}
		void AddElement (CAdamoObjectElement *pElement);
		void GetParameterList (CString strMethod, CStringList& lParameters, CStringList& lDescription);
		bool GetObjectElementLine (CString strObjectElement, int& nLine);
		CAdamoObj& operator = (CAdamoObj&);
};

class CObjectsLibrary
{
	private :
		CString m_strName;
		CString m_strDescription;
		list<CAdamoObj*> m_listObjects;
	public :
		CObjectsLibrary ();
		~CObjectsLibrary ();
		void AddObject (CAdamoObj* pObject);
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
			return m_listObjects.size ();
		}
		list<CAdamoObj *>* GetListObjects ()   {
			return &m_listObjects;
		}
		void GetObjectsList (CStringList& lObjects, CStringList& lDescription);
		void GetObjectElements (CString strObject, list<CAdamoObjectElement *>& lObjectElement);
		void GetParameterList (CString strObject, CString strMethod, CStringList& lParameters, CStringList& lDescription);
		bool GetObjectLine (CString strObject, int& nLine);
		bool GetObjectElementLine (CString strObject, CString strObjectElement, int& nLine);
		CAdamoObj* GetObject (CString strObject);
};


class CObjectsContainer
{
	private :
		list<CObjectsLibrary *> m_listObjectLibraries;
		CString m_strPath;
	public :
		CObjectsContainer ();
		~CObjectsContainer ();
		int  GetCount ()   {
			return m_listObjectLibraries.size ();
		}
		HRESULT Load();
		void Save ();
		void ClearAll ();
		void SetPath (CString strPath)	{ m_strPath = strPath; }
		void AddObjectsLibrary (CObjectsLibrary* pObjectsLibrary);
		bool RemoveModule (CString strModule);
		void GetLibrariesList (CStringList& lOptions, CStringList& lDescription);
		void GetObjectsList (CString strLibrary, CStringList& lObjects, CStringList& lDescription);
		void GetObjectsElements (CString strLibrary, CString strObject, list<CAdamoObjectElement *>& lObjectElement);
		void GetParameterList (CString strLibrary, CString strObject, CString strMethod, CStringList& lParameters, CStringList& lDescription);
		bool GetObjectLine (CString strLibrary, CString strObject, int& nLine);
		bool GetObjectElementLine (CString strLibrary, CString strObject, CString strObjectElement, int& nLine);
		CAdamoObj* GetObject (CString strObject);
		list<CObjectsLibrary *>* GetListLibraries ()	{ return &m_listObjectLibraries; }
	private :
		void LoadObjects (DocXmlPtr pDocXml);
		void WritePrologue (CStdioFile* pStdioFile);
		void WriteData (CStdioFile* pStdioFile);
		void WriteEpilogue (CStdioFile* pStdioFile);
		CString GetPath ();
};
