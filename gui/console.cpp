#include "console.h"

#include "util.h"

CConsole::CConsole(void)
{
	initPipes();
}

CConsole::~CConsole(void)
{	
	CloseHandle(m_hChildStd_IN_Rd);
	CloseHandle(m_hChildStd_IN_Wr);
	CloseHandle(m_hChildStd_OUT_Rd);
	CloseHandle(m_hChildStd_OUT_Wr);

	TerminateProcess(m_piProcInfo.hProcess, -1);

	CloseHandle(m_piProcInfo.hProcess);
	CloseHandle(m_piProcInfo.hThread);
}

void CConsole::Start()
{
	createConsoleProcess();
	//runConsoleReaderThread();
}

void CConsole::createConsoleProcess()
{
	TCHAR szCmdline[]=TEXT("cmd");
	//PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE; 

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory( &m_piProcInfo, sizeof(PROCESS_INFORMATION) );

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.hStdError = m_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = m_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = m_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
	siStartInfo.wShowWindow = SW_HIDE;

	// Create the child process. 

	bSuccess = CreateProcess(NULL, 
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&m_piProcInfo);  // receives PROCESS_INFORMATION 

	// If an error occurs, exit the application. 
	if ( ! bSuccess ) 
		ErrorExit(TEXT("CreateProcess"));
}

void CConsole::initPipes()
{
	SECURITY_ATTRIBUTES saAttr; 

	// Set the bInheritHandle flag so pipe handles are inherited. 

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 

	// Create a pipe for the child process's STDOUT. 

	if ( ! CreatePipe(&m_hChildStd_OUT_Rd, &m_hChildStd_OUT_Wr, &saAttr, 0) ) 
		ErrorExit(TEXT("StdoutRd CreatePipe")); 

	// Ensure the read handle to the pipe for STDOUT is not inherited.

	if ( ! SetHandleInformation(m_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
		ErrorExit(TEXT("Stdout SetHandleInformation")); 

	// Create a pipe for the child process's STDIN. 

	if (! CreatePipe(&m_hChildStd_IN_Rd, &m_hChildStd_IN_Wr, &saAttr, 0)) 
		ErrorExit(TEXT("Stdin CreatePipe")); 

	// Ensure the write handle to the pipe for STDIN is not inherited. 

	if ( ! SetHandleInformation(m_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
		ErrorExit(TEXT("Stdin SetHandleInformation")); 
}

void CConsole::write(CHAR* chBuf)
{
	write(chBuf, strlen(chBuf));
}

void CConsole::write(CHAR* chBuf, int length)
{
	DWORD dwWritten; 
	WriteFile(m_hChildStd_IN_Wr, chBuf, length, &dwWritten, NULL);
}

//void WriteToPipe(void) 
//
//	// Read from a file and write its contents to the pipe for the child's STDIN.
//	// Stop when there is no more data. 
//{ 
//
//	DWORD dwWritten; 
//	CHAR chBuf[1];
//	BOOL bSuccess = FALSE;
//
//	for (;;) 
//	{    
//		chBuf[0] = fgetc(stdin);
//		bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, 1, &dwWritten, NULL);
//		if ( ! bSuccess ) break; 
//	} 
//
//	// Close the pipe handle so the child process stops reading. 
//
//	if ( ! CloseHandle(g_hChildStd_IN_Wr) ) 
//		ErrorExit(TEXT("StdInWr CloseHandle")); 
//} 

//void CConsole::runConsoleReaderThread()
//{
//	DWORD returnCode;
//	m_hReaderThreadHandle = CreateThread( 
//		NULL,                   // default security attributes
//		0,                      // use default stack size  
//		consoleReaderThreadFunction,       // thread function name
//		this,          // argument to thread function 
//		0,                      // use default creation flags 
//		&returnCode);   // returns the thread identifier 
//
//	if (m_hReaderThreadHandle == NULL) 
//	{
//		ErrorExit(TEXT("CreateThread"));
//		ExitProcess(3);
//	}
//}
