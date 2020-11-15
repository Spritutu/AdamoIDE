// AdamoOptions.h : interface of the CAdamoOptions class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

#define OPTIONS_FILE "Options.xml"
#define OPTIONS_SKIN_STYLES		6
#define DEFAULT_TCPIP_ADDRESS   "192.168.0.1"
#define DEFAULT_TCPIP_PORT      2000
#define DEFAULT_KILLMACHINE     1
#define DEFAULT_CONNECTION_TYPE 0
#define DEFAULT_LOAD_ALWAYS_VIRTUAL 0

class CAdamoOptions
{
	public :
		CAdamoOptions ();
		int  LoadOptions (CString strOptionsPath);
		int  SaveOptions ();
		void SetTypeConnection (int nConnType)					{ m_nTypeConnection = nConnType; }
		void SetTcpIpAddress (CString strTcpIpAddress)			{ m_strTcpIPAddress = strTcpIpAddress; }
		void SetTcpIpPort (int nPort)							{ m_nPort = nPort; }
		void SetTcpIpTimeout (int nTimeout)						{ m_nTimeout = nTimeout; }
		int  GetTypeConnection ()								{ return m_nTypeConnection; }
		CString GetTcpIpAddress ()								{ return m_strTcpIPAddress; }
		int  GetTcpIpPort ()									{ return m_nPort; }
		int  GetTcpIpTimeout ()									{ return m_nTimeout; }
		void SetTcpIpAddress (BYTE a, BYTE b, BYTE c, BYTE d);
		void GetTcpIpAddress (BYTE &a, BYTE &b, BYTE &c, BYTE &d);
		void SetKillMachine (bool bKillMachine)					{ m_bKillMachine = bKillMachine; }
		bool GetKillMachine ()									{ return m_bKillMachine; }
		void SetLoadVirtualMachine (bool bLoadVirtualMachine)	{ m_bLoadVirtualMachine = bLoadVirtualMachine; }
		bool GetLoadVirtualMachine ()							{ return m_bLoadVirtualMachine; }
		CString GetPathBin ()									{ return m_strPathBin; }
		void    SetPathBin (CString strPathBin)					{ m_strPathBin = strPathBin; }
		CString GetPathCurrent ()								{ return m_strPathCurrent; }
		void    SetPathCurrent (CString strPathCurrent)			{ m_strPathCurrent = strPathCurrent; }
		CString GetPathHTML ()									{ return m_strPathHTML; }
		void    SetPathHTML (CString strPathHTML)				{ m_strPathHTML = strPathHTML; }
		CString GetPathImages ()								{ return m_strPathImages; }
		void    SetPathImages (CString strPathImages)			{ m_strPathImages = strPathImages; }
		CString GetPathLibraryHelp ()							{ return m_strPathLibraryHelp; }
		void    SetPathLibraryHelp (CString strPathLibraryHelp) { m_strPathLibraryHelp = strPathLibraryHelp; }
		CString GetPathLicense ()								{ return m_strPathLicense; }
		void    SetPathLicense (CString strPathLicense)			{ m_strPathLicense = strPathLicense; }
		CString GetPathPlant ()									{ return m_strPathPlant; }
		void    SetPathPlant (CString strPathPlant)				{ m_strPathLicense = m_strPathPlant; }
		CString GetPathSimulator ()								{ return m_strPathSimulator; }
		void    SetPathSimulator (CString strPathSimulator)		{ m_strPathSimulator = strPathSimulator; }
		int     GetRTARetry ()									{ return m_nRTARetry; }
		void    SetRTARetry (int nRTARetry)						{ m_nRTARetry = nRTARetry; }
		bool    GetViewPreprocessedFile ()								{ return m_bViewPreprocessedFile; }
		void    SetViewPreprocessedFile (bool bViewPreprocessedFile)	{ m_bViewPreprocessedFile = bViewPreprocessedFile; }
		DWORD   GetConnectionTimeout ()								{ return m_dwConnectionTimeout; }
		void    SetConnectionTimeout (DWORD dwConnectionTimeout)	{ m_dwConnectionTimeout = dwConnectionTimeout; }
		DWORD   GetCommunicationTimeout ()								{ return m_dwCommunicationTimeout; }
		void    SetCommunicationTimeout (DWORD dwCommunicationTimeout)	{ m_dwCommunicationTimeout = dwCommunicationTimeout; }
		bool    GetDisablePswManagement ()								{ return m_bDisablePswManagement; }
		void    SetDisablePswManagement (bool bDisablePswManagement)	{ m_bDisablePswManagement = bDisablePswManagement; }
		CString GetCurLanguage ()								{ return m_strCurLanguage; }
		void    SetCurLanguage (CString strCurLanguage)			{ m_strCurLanguage = strCurLanguage; }
		CString GetLanguageThira ()								{ return m_strLanguageThira; }
		void    SetLanguageThira (CString strLanguageThira)		{ m_strLanguageThira = strLanguageThira; }
		CString GetLanguageCNCLink ()							{ return m_strLanguageCNCLink; }
		void    SetLanguageCNCLink (CString strLanguageCNCLink)	{ m_strLanguageCNCLink = strLanguageCNCLink; }
		CString GetLanguageResources ()								{ return m_strLanguageResources; }
		void    SetLanguageResources (CString strLanguageResources)	{ m_strLanguageResources = strLanguageResources; }
		CString GetSkin ()										{ return m_strSkin; }
		void    SetSkin (CString strSkin)						{ m_strSkin = strSkin; }
	private :
		int LoadData (DocXmlPtr pDocXml);
		int SaveData ();
		void SavePrologue (CStdioFile* pStdioFile);
		void SaveOptions (CStdioFile* pStdioFile);
		void SaveEpilogue (CStdioFile* pStdioFile);
	private :
		CString m_strOptionsPath;
		int		m_nTypeConnection;
		CString m_strTcpIPAddress;
		int     m_nPort;
		int     m_nTimeout;
		bool m_bKillMachine;
		bool m_bLoadVirtualMachine;
		CString m_strPathBin;
		CString m_strPathCurrent;
		CString m_strPathHTML;
		CString m_strPathImages;
		CString m_strPathLibraryHelp;
		CString m_strPathLicense;
		CString m_strPathPlant;
		CString m_strPathSimulator;
		int     m_nRTARetry;
		bool	m_bViewPreprocessedFile;
		DWORD   m_dwConnectionTimeout;
		DWORD   m_dwCommunicationTimeout;
		bool    m_bDisablePswManagement;
		CString m_strCurLanguage;
		CString m_strLanguageThira;
		CString m_strLanguageCNCLink;
		CString m_strLanguageResources;
		CString m_strSkin;
};
