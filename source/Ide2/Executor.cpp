// Executor.cpp: implementation of the CExecutor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef ADAMO_THIRACC
#include "ThiraCC.h"
#else
#include "ide2.h"
#endif
#include "Executor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExecutor::CExecutor()
{
}

CExecutor::~CExecutor()
{

}

BOOL CExecutor::Execute(CString strCmdLine, CMemFile* pInput)
{
	HANDLE hOutputReadTmp,hOutputWrite;
	HANDLE hInputWriteTmp,hInputRead;
	HANDLE hErrorWrite;

	// Set up the security attributes struct.
	SECURITY_ATTRIBUTES sa;
	sa.nLength= sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	// Create the child output pipe.
	if (!CreatePipe(&hOutputReadTmp,&hOutputWrite,&sa,0))
		theApp.FormatMessage("CExecutor::Execute::CreatePipe");

	// Create a duplicate of the output write handle for the std error
	// write handle. This is necessary in case the child application
	// closes one of its std output handles.
	if (!DuplicateHandle(GetCurrentProcess(),hOutputWrite,
					   GetCurrentProcess(),&hErrorWrite,0,
					   TRUE,DUPLICATE_SAME_ACCESS))
		theApp.FormatMessage("CExecutor::Execute::DuplicateHandle");


	// Create the child input pipe.
	if (!CreatePipe(&hInputRead,&hInputWriteTmp,&sa,0))
		theApp.FormatMessage("CExecutor::Execute::CreatePipe");


	// Create new output read handle and the input write handles. Set
	// the Properties to FALSE. Otherwise, the child inherits the
	// properties and, as a result, non-closeable handles to the pipes
	// are created.
	if (!DuplicateHandle(GetCurrentProcess(),hOutputReadTmp,
					   GetCurrentProcess(),
					   &m_hOutputRead, // Address of new handle.
					   0,FALSE, // Make it uninheritable.
					   DUPLICATE_SAME_ACCESS))
		theApp.FormatMessage("CExecutor::Execute::DupliateHandle");

	if (!DuplicateHandle(GetCurrentProcess(),hInputWriteTmp,
					   GetCurrentProcess(),
					   &m_hInputWrite, // Address of new handle.
					   0,FALSE, // Make it uninheritable.
					   DUPLICATE_SAME_ACCESS))
		theApp.FormatMessage("DupliateHandle");


	// Close inheritable copies of the handles you do not want to be
	// inherited.
	if (!CloseHandle(hOutputReadTmp)) theApp.FormatMessage("CExecutor::Execute::CloseHandle");
	if (!CloseHandle(hInputWriteTmp)) theApp.FormatMessage("CExecutor::Execute::CloseHandle");

	PrepAndLaunchRedirectedChild(hOutputWrite, hInputRead, hErrorWrite, strCmdLine);

	// Close pipe handles (do not continue to modify the parent).
	// You need to make sure that no handles to the write end of the
	// output pipe are maintained in this process or else the pipe will
	// not close when the child process exits and the ReadFile will hang.
	if (!CloseHandle(hOutputWrite)) theApp.FormatMessage("CExecutor::Execute::CloseHandle");
	if (!CloseHandle(hInputRead )) theApp.FormatMessage("CExecutor::Execute::CloseHandle");
	if (!CloseHandle(hErrorWrite)) theApp.FormatMessage("CExecutor::Execute::CloseHandle");

	// Launch the thread that gets the input and sends it to the child.
	DWORD ThreadId;
	m_hThread = CreateThread(NULL, 0, ReadAndHandleOutput, (LPVOID)this, 0, &ThreadId);
	if (m_hThread == NULL) theApp.FormatMessage("CExecutor::Execute::CreateThread");

	if ( pInput!=NULL )
	{
		LPBYTE buf = (LPBYTE)malloc((unsigned int)pInput->GetLength());
		pInput->SeekToBegin();
		pInput->Read(buf, (unsigned int)pInput->GetLength());

		Write(buf, (unsigned int)pInput->GetLength());
		Close();

		free(buf);
	}
	else
		Close();

	return TRUE;
}

void CExecutor::Close()
{
	if (WaitForSingleObject(m_hThread,INFINITE) == WAIT_FAILED)
		theApp.FormatMessage("CExecutor::Close::WaitForSingleObject");

	if (!CloseHandle(m_hInputWrite)) theApp.FormatMessage("CExecutor::Close::CloseHandle");
}

void CExecutor::PrepAndLaunchRedirectedChild(HANDLE hChildStdOut, HANDLE hChildStdIn, HANDLE hChildStdErr,
											 CString strCmdLine)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	// Set up the start up info struct.
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = hChildStdOut;
	si.hStdInput  = hChildStdIn;
	si.hStdError  = hChildStdErr;
	// Use this if you want to hide the child:
	si.wShowWindow = SW_HIDE;
	// Note that dwFlags must include STARTF_USESHOWWINDOW if you want to
	// use the wShowWindow flags.

	// Launch the process that you want to redirect (in this case,
	// Child.exe). Make sure Child.exe is in the same directory as
	// redirect.c launch redirect from a command line to prevent location
	// confusion.
	if (!CreateProcess(NULL, strCmdLine.GetBuffer(0), NULL, NULL, TRUE,
					 0, NULL, NULL, &si, &pi))
		theApp.FormatMessage("CExecutor::PrepAndLaunchRedirectedChild::CreateProcess");


	// Close any unnecessary handles.
	if (!CloseHandle(pi.hThread)) theApp.FormatMessage("CExecutor::PrepAndLaunchRedirectedChild::CloseHandle");
}

BOOL CExecutor::Write(LPBYTE lpData, int nSize)
{
	DWORD nBytesWrote;
	if (!WriteFile(m_hInputWrite, lpData, nSize, &nBytesWrote, NULL))
	{
		if (GetLastError() != ERROR_NO_DATA)
			theApp.FormatMessage("CExecutor::WriteWriteFile");

		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CExecutor::ReadAndHandleOutput(LPVOID lpvThreadParam)
{
	BYTE lpBuffer[256];
	DWORD nBytesRead;
	CExecutor* pExe = (CExecutor*)lpvThreadParam;

	pExe->m_output.SetLength(0);

	while(TRUE)
	{
		if (!ReadFile(pExe->m_hOutputRead, lpBuffer, sizeof(lpBuffer), &nBytesRead, NULL) || !nBytesRead)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
				break; // pipe done - normal exit path.
			else
				theApp.FormatMessage("CExecutor::ReadAndHandleOutput::ReadFile"); // Something bad happened.
		}
		else
			pExe->m_output.Write(lpBuffer, nBytesRead);

	}

	if (!CloseHandle(pExe->m_hOutputRead)) theApp.FormatMessage("CExecutor::ReadAndHandleOutput::CloseHandle");

	return 1;
}

CString CExecutor::GetOutputString()
{
	CString strOutput;

	m_output.SeekToBegin();
	m_output.Read(strOutput.GetBuffer((unsigned int)m_output.GetLength()), (unsigned int)m_output.GetLength());
	strOutput.ReleaseBuffer();

	return strOutput;
}

BOOL CExecutor::SaveOutput(CString strPathName)
{
	CFile fout;
	if ( !fout.Open(strPathName, CFile::modeCreate | CFile::modeWrite) )
		return FALSE;
	CString strOutput = GetOutputString();
	
	fout.Write(strOutput.GetBuffer(0), strOutput.GetLength());
	fout.Close();

	return TRUE;
}
