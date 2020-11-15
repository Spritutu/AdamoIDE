// AdamoOptions.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

/* import directive */
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <io.h>
#include "AdamoOptions.h"

const char* achSkinStyles[OPTIONS_SKIN_STYLES] = {
	"Style Blue 1",		// themeVS2010
	"Style Light 1",	// themeVS2012Light
	"Style Dark 1",		// themeVS2012Dark
	"Style Blue 2",		// themeVS2015Light
	"Style Light 2",	// themeVS2015Dark
	"Style Dark 2"		// themeVS2015Blue
};

CAdamoOptions::CAdamoOptions () : m_nPort (DEFAULT_TCPIP_PORT), m_nTypeConnection (0), m_bKillMachine (false), m_strTcpIPAddress (DEFAULT_TCPIP_ADDRESS), 
	m_bLoadVirtualMachine (false), m_nRTARetry (30), m_bViewPreprocessedFile (false),
	m_dwConnectionTimeout (0xffffffff), m_dwCommunicationTimeout (0xffffffff), m_bDisablePswManagement (false), 
	m_nTimeout (1000)
{
	m_strPathSimulator = "AdamoContainer.exe";
}

int CAdamoOptions::LoadOptions (CString strOptionsPath)
{
	DocXmlPtr pDocXml;
	HRESULT hr = S_FALSE;
	m_strOptionsPath = strOptionsPath + "\\" + OPTIONS_FILE;
	if (!_access (m_strOptionsPath, 00))   {
		hr = pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
		if (SUCCEEDED(hr))   {
			pDocXml->async = false;
			try   {
				if (pDocXml->load (m_strOptionsPath.AllocSysString ()))   {
					LoadData (pDocXml);
					hr=S_OK;
				}
			}
			catch (...) {
				hr = S_FALSE;
			}
			pDocXml.Release ();
		}
	}
	else
		SaveOptions ();
    return hr == S_OK ? 0 : -1;
}

int CAdamoOptions::SaveOptions ()
{
	SaveData ();
	return 0;
}

void CAdamoOptions::SetTcpIpAddress (BYTE a, BYTE b, BYTE c, BYTE d)
{
	m_strTcpIPAddress.Format ("%d.%d.%d.%d", a, b, c, d);
}

void CAdamoOptions::GetTcpIpAddress (BYTE &a, BYTE &b, BYTE &c, BYTE &d)
{
	CString str[4];
	int PunticinoPrima = 0, nPunticinoDopo;

	for (int i = 0; i<4; i++)   {
		nPunticinoDopo = m_strTcpIPAddress.Find ('.', PunticinoPrima);
		if (nPunticinoDopo == -1)
			nPunticinoDopo = m_strTcpIPAddress.GetLength ();
		str[i] = m_strTcpIPAddress.Mid (PunticinoPrima, nPunticinoDopo - PunticinoPrima);
		PunticinoPrima = nPunticinoDopo + 1;
	}
	a = (BYTE) strtol (str[0], NULL, 10);
	b = (BYTE) strtol (str[1], NULL, 10);
	c = (BYTE) strtol (str[2], NULL, 10);
	d = (BYTE) strtol (str[3], NULL, 10);
}

int CAdamoOptions::LoadData (DocXmlPtr pDocXml)
{
    ElementXmlPtr  pEl=pDocXml->documentElement;
	NodeListXmlPtr  pRootChild;
    CString str=(LPCTSTR)pEl->nodeName;
	if (str=="OPTIONS")   {
		if (pEl->hasChildNodes())	  {
			NodeListXmlPtr  pRootChild;
			pRootChild = pEl->childNodes;
			int nItems=pRootChild->length;
			int n = 0;
			while (n<nItems)   {
				str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
				if (str == "GLOBAL")   {
					pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
						while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (str=="ConnectionType")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_nTypeConnection = atoi (str);
							}
							else
							if (str=="TcpIpAddress")   {
								m_strTcpIPAddress = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="TcpIpPort")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_nPort = atoi (str);
							}
							else
							if (str=="TcpIpTimeout")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_nTimeout = atoi (str);
							}
							else
							if (str=="PathBin")   {
								m_strPathBin = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathCurrent")   {
								m_strPathCurrent = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathHTML")   {
								m_strPathHTML = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathImages")   {
								m_strPathImages = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathLibraryHelp")   {
								m_strPathLibraryHelp = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathLicense")   {
								m_strPathLicense = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathPlant")   {
								m_strPathPlant = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="PathSimulator")   {
								m_strPathSimulator = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							n++;
						}
					}
				}
				else
				if (str == "THIRA")   {
					pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
						while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (str=="ViewPreprocessedFile")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_bViewPreprocessedFile = atoi (str) != 0;
							}
							else
							if (str=="LoadVirtualMachine")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_bLoadVirtualMachine = atoi (str) != 0;
							}
							else
							if (str=="KillMachine")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_bKillMachine = atoi (str) != 0;
							}
							else
							if (str=="Skin")   {
								m_strSkin = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							n++;
						}
					}
				}
				else
				if (str == "THIRALINK")   {
					pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
						while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (str=="ConnectionTimeout")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_dwConnectionTimeout = strtoul (str, NULL, 16);
							}
							else
							if (str=="CommunicationTimeout")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_dwCommunicationTimeout = strtoul (str, NULL, 16);
							}
							else
							if (str=="RTARetry")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_nRTARetry = atoi (str);
							}
							else
							if (str=="DisablePswManagement")   {
								str=(LPCTSTR)pRootChild->Getitem(n)->text;
								m_bDisablePswManagement = atoi (str) != 0;
							}
							n++;
						}
					}
				}
				else
				if (str == "LANGUAGES")   {
					pEl=pRootChild->Getitem(n);
					if (pEl->hasChildNodes())	  {
						NodeListXmlPtr  pRootChild;
						pRootChild = pEl->childNodes;
						int nItems=pRootChild->length;
						int n = 0;
						while (n<nItems)   {
							str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
							if (str=="CurLanguage")   {
								m_strCurLanguage = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="Thira")   {
								m_strLanguageThira = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="CNCLink")   {
								m_strLanguageCNCLink = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							else
							if (str=="Resources")   {
								m_strLanguageResources = (LPCTSTR)pRootChild->Getitem(n)->text;
							}
							n++;
						}
					}
				}
				n++;
			}
		}
	}
    return 0;
}

int CAdamoOptions::SaveData ()
{
	CStdioFile f;

	if (f.Open (m_strOptionsPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))   {
		/* salviamo il prologo */
		SavePrologue (&f);
		/* salviamo i controlli */
		SaveOptions  (&f);
		/* salviamo l'epilogo */
		SaveEpilogue (&f);
	}
	return 0;
}

/*
** SavePrologue :
*/
void CAdamoOptions::SavePrologue (CStdioFile* pStdioFile)
{
	pStdioFile->WriteString ("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	pStdioFile->WriteString ("<OPTIONS>\n");
}

/*
** SaveOptions :
*/
void CAdamoOptions::SaveOptions (CStdioFile* pStdioFile)
{
	CString str, strNumber;

	pStdioFile->WriteString ("<GLOBAL>\n");
	strNumber.Format ("%d", m_nTypeConnection);
	str = "\t<ConnectionType>" + strNumber + "</ConnectionType>\n";
	pStdioFile->WriteString (str);

	str = "\t<TcpIpAddress>" + m_strTcpIPAddress + "</TcpIpAddress>\n";
	pStdioFile->WriteString (str);

	strNumber.Format ("%d", m_nPort);
	str = "\t<TcpIpPort>" + strNumber + "</TcpIpPort>\n";
	pStdioFile->WriteString (str);
		
	strNumber.Format ("%d", m_nTimeout);
	str = "\t<TcpIpTimeout>" + strNumber + "</TcpIpTimeout>\n";
	pStdioFile->WriteString (str);

	str = "\t<PathBin>" + m_strPathBin + "</PathBin>\n";
	pStdioFile->WriteString (str);

	str = "\t<PathCurrent>" + m_strPathCurrent + "</PathCurrent>\n";
	pStdioFile->WriteString (str);

	str = "\t<PathHTML>" + m_strPathHTML + "</PathHTML>\n";
	pStdioFile->WriteString (str);

	str = "\t<PathImages>" + m_strPathImages + "</PathImages>\n";
	pStdioFile->WriteString (str);

	str = "\t<PathLibraryHelp>" + m_strPathLibraryHelp + "</PathLibraryHelp>\n";
	pStdioFile->WriteString (str);
	
	str = "\t<PathLicense>" + m_strPathLicense + "</PathLicense>\n";
	pStdioFile->WriteString (str);

	str = "\t<PathPlant>" + m_strPathPlant + "</PathPlant>\n";
	pStdioFile->WriteString (str);
	
	str = "\t<PathSimulator>" + m_strPathSimulator + "</PathSimulator>\n";
	pStdioFile->WriteString (str);

	pStdioFile->WriteString ("</GLOBAL>\n");
	pStdioFile->WriteString ("<THIRA>\n");

	strNumber.Format ("%d", m_bKillMachine);
	str = "\t<KillMachine>" + strNumber + "</KillMachine>\n";
	pStdioFile->WriteString (str);

	strNumber.Format ("%d", m_bLoadVirtualMachine);
	str = "\t<LoadVirtualMachine>" + strNumber + "</LoadVirtualMachine>\n";
	pStdioFile->WriteString (str);

	strNumber.Format ("%d", m_bViewPreprocessedFile);
	str = "\t<ViewPreprocessedFile>" + strNumber + "</ViewPreprocessedFile>\n";
	pStdioFile->WriteString (str);

	str = "\t<Skin>" + m_strSkin + "</Skin>\n";
	pStdioFile->WriteString (str);

	pStdioFile->WriteString ("</THIRA>\n");
	pStdioFile->WriteString ("<THIRALINK>\n");

	strNumber.Format ("%8X", m_dwConnectionTimeout);
	str = "\t<ConnectionTimeout>" + strNumber + "</ConnectionTimeout>\n";
	pStdioFile->WriteString (str);

	strNumber.Format ("%8X", m_dwCommunicationTimeout);
	str = "\t<CommunicationTimeout>" + strNumber + "</CommunicationTimeout>\n";
	pStdioFile->WriteString (str);

	strNumber.Format ("%d", m_nRTARetry);
	str = "\t<RTARetry>" + strNumber + "</RTARetry>\n";
	pStdioFile->WriteString (str);

	strNumber.Format ("%d", m_bDisablePswManagement);
	str = "\t<DisablePswManagement>" + strNumber + "</DisablePswManagement>\n";
	pStdioFile->WriteString (str);
	
	pStdioFile->WriteString ("</THIRALINK>\n");

	pStdioFile->WriteString ("<LANGUAGES>\n");
	str = "\t<CurLanguage>" + m_strCurLanguage + "</CurLanguage>\n";
	pStdioFile->WriteString (str);
	str = "\t<ThiraLanguage>" + m_strLanguageThira + "</ThiraLanguage>\n";
	pStdioFile->WriteString (str);
	str = "\t<CNCLinkLanguage>" + m_strLanguageCNCLink + "</CNCLinkLanguage>\n";
	pStdioFile->WriteString (str);
	str = "\t<Resources>" + m_strLanguageResources + "</Resources>\n";
	pStdioFile->WriteString (str);

	pStdioFile->WriteString ("</LANGUAGES>\n");
}

/*
** SaveEpilogue :
*/
void CAdamoOptions::SaveEpilogue (CStdioFile* pStdioFile)
{
	pStdioFile->WriteString ("</OPTIONS>\n");
}
