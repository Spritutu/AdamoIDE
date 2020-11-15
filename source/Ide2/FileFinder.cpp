//////////////////////////////////////////////////////////////////////
//	Implemented by Samuel Gonzalo 
//
//	You may freely use or modify this code 
//////////////////////////////////////////////////////////////////////
//
// FileFinder.cpp: implementation of the CFileFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileFinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileFinder::CFileFinder()
{
	_bStopSearch = false;
	_pFileFinderProc = NULL;
	_pCustomParam = NULL;
}

CFileFinder::~CFileFinder()
{

}

int CFileFinder::FindFiles(LPCTSTR szBaseFolder, LPCTSTR szFileMask, BOOL bSubFolders)
{
	CFindOpts opts;

	opts.sBaseFolder = szBaseFolder;
	opts.sFileMask = szFileMask;
	opts.bSubfolders = bSubFolders;

	// Get all files, but no directories
	opts.FindAllFiles();

	Find(opts);

	return GetFileCount();
}

int CFileFinder::Find(CFileFinder::CFindOpts &opts)
{
	CFileFind	finder;
	CString		sFullMask;
	CFindOpts	subOpts;
	BOOL		bFound, bValidFile;
	CTime		timeFile;

	_bStopSearch = false;

	opts.sBaseFolder = AddBackSlash(opts.sBaseFolder);

	// Find directories
	if (opts.bSubfolders)
	{
		sFullMask = opts.sBaseFolder + CString("*.*");
		bFound = finder.FindFile(sFullMask);
		while ((bFound) && (!_bStopSearch))
		{
			bFound = finder.FindNextFile();
			if ((finder.IsDirectory()) && (!finder.IsDots()))
			{
				subOpts = opts;
				subOpts.sBaseFolder = opts.sBaseFolder + finder.GetFileName();

				// Recursive call
				Find(subOpts);
			}
		}
	}

	finder.Close();

	_sSearchingFolder = opts.sBaseFolder;

	// Call callback procedure
	if (_pFileFinderProc != NULL)
		_pFileFinderProc(this, FF_FOLDER, _pCustomParam);

	sFullMask = opts.sBaseFolder + opts.sFileMask;
	bFound = finder.FindFile(sFullMask);
	while ((bFound) && (!_bStopSearch))
	{
		bFound = finder.FindNextFile();
		if (!finder.IsDots())
		{
			// check constrains
			bValidFile = TRUE;
			if (opts.dwOptionsFlags & FIND_ATTRIBUTES)
			{
				bValidFile = finder.MatchesMask(opts.dwFileAttributes);
			}

			if (bValidFile && (opts.dwOptionsFlags & FIND_SIZE))
			{
				bValidFile = ((opts.nMinSize <= (__int64)finder.GetLength()) &&
							(opts.nMaxSize >= (__int64)finder.GetLength()));
			}

			if (bValidFile && (opts.dwOptionsFlags & FIND_DATEMODIFIED))
			{
				finder.GetLastWriteTime(timeFile);
				bValidFile = ((timeFile >= opts.tMinModified) && 
								(timeFile <= opts.tMaxModified));
			}

			if (bValidFile && (opts.dwOptionsFlags & FIND_DATECREATED))
			{
				finder.GetCreationTime(timeFile);
				bValidFile = ((timeFile >= opts.tMinCreated) && 
								(timeFile <= opts.tMaxCreated));
			}

			if (bValidFile && (opts.dwOptionsFlags & FIND_DATEACCESSED))
			{
				finder.GetLastAccessTime(timeFile);
				bValidFile = ((timeFile >= opts.tMinAccessed) && 
								(timeFile <= opts.tMaxAccessed));
			}

			if (bValidFile && (opts.dwOptionsFlags & FIND_TEXT))
			{
				if (IsFileInProject (finder.GetFilePath()))
					bValidFile = FindTextInFile(finder.GetFilePath(), opts);
			}

			// Add file to list
			if (bValidFile)
			{
				CString sName = finder.GetFilePath();
				if (finder.IsDirectory()) sName += "\\";
				_aFilesFound.Add(sName);
			}

			// Call callback procedure
			if (_pFileFinderProc != NULL)
				_pFileFinderProc(this, bValidFile ? FF_FOUND : FF_DISCARDED, _pCustomParam);
		}
	}

	return GetFileCount();
}

BOOL CFileFinder::FindTextInFile (LPCTSTR szFile, CFileFinder::CFindOpts& opts)
{
	CStdioFile file;
	CString	   sFindText(opts.sFindText), sText, sMem;
    int nRow, nLoopCount=0;;
	bool       bTextFound=false;
    char* p, *pTextBuf;

	if (opts.sFindText == "") 
        return FALSE;
	if (!file.Open(szFile, CFile::modeRead)) 
        return FALSE;
    if (!opts.bMatchCase)
	    sFindText.MakeUpper();
    nRow=1;
	while (file.ReadString (sText))   {
        sMem=sText;
        if (!opts.bMatchCase)
	        sText.MakeUpper();
        pTextBuf=sText.GetBuffer (sText.GetLength()+1);
        if (IsBinary ((unsigned char*)pTextBuf, sText.GetLength ()))
            break;
        p=pTextBuf;
        while (p=strstr (p, sFindText))   {
            if (opts.bMatchWord && !IsWord (pTextBuf, p, sFindText.GetLength()))
                p+=sFindText.GetLength();
            else   {
                AddFoundText (sMem, nRow);
                p+=sFindText.GetLength();
                bTextFound=true;
            }
        }
		sText.ReleaseBuffer(-1);
		if (_pFileFinderProc != NULL)   {
			nLoopCount++;
			if (nLoopCount == 10)   {
				nLoopCount = 0;
				_pFileFinderProc(this, FF_FINDTEXT, _pCustomParam);
			}
		}
        nRow++;
	}
	file.Close();
	return bTextFound;
}

void CFileFinder::AddFoundText (const char* pTextBuf, int nRow)
{
    CString str;

    str.Format ("(%d) %s", nRow, pTextBuf);
    m_strFound.AddTail (str);
}

BOOL CFileFinder::FindTextInFile2(LPCTSTR szFile, CFileFinder::CFindOpts& opts)
{
	if (opts.sFindText == "") return FALSE;

	CFile file;

	if (!file.Open(szFile, CFile::modeRead)) return FALSE;

	const UINT	nCMinBufSize = 128;
	CString		sText;
	CString		sFindText(opts.sFindText);
	UINT		nFindTextLen = sFindText.GetLength();
	UINT		nBufSize = 128;
	UINT		nReadSize;
	UINT		nCharRead;
    UINT        i, o, n;
	LPSTR		pTextBuf;
	BOOL		bTextFound=FALSE;
	int			nLoopCount = 0;
    int         nRow=1;
    int         nLastRow;
    bool        bFound, bIsCR;
    CString     strLastRow;
    unsigned char        * pLastRow=NULL, c;

	if ((2 * nFindTextLen) > nCMinBufSize)
		nBufSize = (2 * nFindTextLen);
    pLastRow = new unsigned char[nBufSize];
    pLastRow[0]='\0';
    nLastRow=0;
	nReadSize = nBufSize - nFindTextLen;
    if (!opts.bMatchCase)
	    sFindText.MakeUpper();
    bFound=false; bIsCR=false;
	do 	 {
		pTextBuf = sText.GetBuffer(nBufSize);
        pLastRow[nLastRow]='\0';
        strLastRow+=(char *)pLastRow;
        nLastRow=0;
		if (pTextBuf[0] == 0x0)
			memset(pTextBuf, ' ', nFindTextLen);
		else
			memcpy(pTextBuf, pTextBuf + (nBufSize - nFindTextLen), nFindTextLen);
		nCharRead = file.Read(pTextBuf + nFindTextLen, nReadSize);
        if (IsBinary ((unsigned char*)pTextBuf + nFindTextLen, nCharRead))
            break;
        i=0;
        while (i<nBufSize)   {
            if (i>=nFindTextLen)   {
                if (i<nBufSize-1)
                    if ((pTextBuf[i]==0x0d&&pTextBuf[i+1]==0x0a)||
                        (bIsCR&&i==nFindTextLen&&pTextBuf[i]==0x0a))  {
                        pLastRow[nLastRow]='\0';
                        strLastRow+=(char *)pLastRow;
                        if (bFound)   {
                            CString str;
                            str.Format ("(%d) %s", nRow, strLastRow);
                            m_strFound.AddTail (str);
                            bTextFound=TRUE;
                            bFound=false;
                        }
                        nRow++; i+= bIsCR ? 1 : 2;
                        nLastRow=0; strLastRow="";
                        continue;
                    }
                bIsCR=false;
                if (i==nBufSize-1&&pTextBuf[i]==0x0d)
                    bIsCR=true;
                else
                    pLastRow[nLastRow++]=pTextBuf[i];
            }
            if (!bFound)   {
                c=pTextBuf[i];
                if (!opts.bMatchCase)
                    c=toupper (pTextBuf[i]);
                if (c==sFindText[0]&&i<=nCharRead)   {
                    bFound=true;
                    n=i+1; o=1;
                    while (o<nFindTextLen)   {
                        c=pTextBuf[n];
                        if (!opts.bMatchCase)
                            c=toupper (pTextBuf[n]);
                        if (c!=sFindText[o])   {
                            bFound=false;
                            break;
                        }
                        n++; o++;
                    }
                    if (bFound)   {
                        for (n=i+1; n<i+nFindTextLen; n++)
                            pLastRow[nLastRow++]=pTextBuf[n];
                        i+=nFindTextLen-1;
                        if (opts.bMatchWord)   {
                            double fpDummy;
                            fpDummy=0.0;
                        }
                    }
                }
            }
            i++;
        }
		sText.ReleaseBuffer(nFindTextLen + nCharRead);
		if (_pFileFinderProc != NULL)
		{
			nLoopCount++;
			if (nLoopCount == 10)
			{
				nLoopCount = 0;
				_pFileFinderProc(this, FF_FINDTEXT, _pCustomParam);
			}
		}
	} 	while ((nCharRead == nReadSize));
    delete pLastRow;
	file.Close();
	return bTextFound;
}

int CFileFinder::GetFileCount()
{
	return _aFilesFound.GetSize();
}

int CFileFinder::FindPathItem(LPCTSTR szPath)
{
	bool	bFound;
	int		nIndex;

	for (nIndex = 0; nIndex < _aFilesFound.GetSize(); nIndex++)
	{
		bFound = (_aFilesFound[nIndex].CompareNoCase(szPath) == 0);
		if (bFound) break;
	}

	return (bFound ? nIndex : -1);
}

CString CFileFinder::GetFilePath(int nIndex)
{
	if ((nIndex < 0) || (nIndex >= GetFileCount())) return "";

	return _aFilesFound[nIndex];
}

LPCTSTR	CFileFinder::GetSearchingFolder()
{
	return _sSearchingFolder;
}

void CFileFinder::RemoveAt(int nIndex)
{
	if ((nIndex < 0) || (nIndex >= GetFileCount())) return;

	_aFilesFound.RemoveAt(nIndex);
}

void CFileFinder::RemoveAll()
{
	if (GetFileCount() > 0) _aFilesFound.RemoveAll();
    m_strFound.RemoveAll ();
	m_listFilesInProject.RemoveAll ();
}

void CFileFinder::SetCallback(FILEFINDERPROC pFileFinderProc, void *pCustomParam)
{
	_pFileFinderProc = pFileFinderProc;
	_pCustomParam = pCustomParam;
}

void CFileFinder::StopSearch()
{
	_bStopSearch = true;
}

CString CFileFinder::AddBackSlash(LPCTSTR szFolderPath, BOOL bInverted)
{
	CString	sResult(szFolderPath);
	int		nLastChar = sResult.GetLength() - 1;

	if (nLastChar >= 0)
	{
		if ((sResult[nLastChar] != '\\') && (sResult[nLastChar] != '/'))
			sResult += bInverted ? '/' : '\\';
	}

	return sResult;
}

BOOL CFileFinder::IsBinary (const unsigned char* p, int nLen)
{
    BOOL b=FALSE;
    for (int i=0; i<nLen; i++)
        if (p[i]<0x020)
            if (p[i]<0x07||p[i]>0x0d)
                if (p[i]!=0x1a)
                    b=TRUE;
    return b;
}

BOOL CFileFinder::IsWord (const char *pText, char *p, int nLen)
{
    char *q=p-1;
    BOOL b=FALSE;

    if (q>=pText)    {
        if (IsWordCharForSel (*q))   {
            q=p+nLen;
            if (IsWordCharForSel (*q))   {
                b=TRUE;
            }
        }
    }
    return b;
}

BOOL CFileFinder::IsWordCharForSel(char ch)
{
	return (BOOL) strchr("\t\n\r !\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~", ch) != NULL;
}

void CFileFinder::AddProjectFile (CString strFileProject)
{
	m_listFilesInProject.AddTail (strFileProject);
}

bool CFileFinder::IsFileInProject (CString strFileProject)
{
	int nIndex = strFileProject.ReverseFind ('\\');
	bool bFound = false;
	CString str = strFileProject.Right (strFileProject.GetLength () - nIndex - 1);
	for (POSITION pos = m_listFilesInProject.GetHeadPosition (); pos; ) 	{
		CString strProject = m_listFilesInProject.GetNext (pos);
		strProject += ".ada";
		if (strProject.MakeUpper () == str.MakeUpper ())		{
			bFound = true;
			break;
		}
	}
	return bFound;
}
