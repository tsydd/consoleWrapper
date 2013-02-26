#pragma once

#include <windows.h>

class CConsole
{
	friend DWORD WINAPI consoleReaderThreadFunction(LPVOID);
private:

	PROCESS_INFORMATION m_piProcInfo;

	HANDLE m_hChildStd_IN_Rd;
	HANDLE m_hChildStd_IN_Wr;
	HANDLE m_hChildStd_OUT_Wr;
	HANDLE m_hChildStd_OUT_Rd;

	CConsole(const CConsole& console);
	CConsole& operator=(const CConsole&);

	void createConsoleProcess(void);
	void initPipes(void);
public:
	CConsole(void);
	~CConsole(void);

	void Start(void);

	void write(CHAR*);
	void write(CHAR*, int length);
};