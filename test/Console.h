#pragma once

class CConsole
{
private:
	CConsole(void);
	CConsole(const CConsole& console);
	CConsole& operator=(const CConsole&);
public:
	static const CConsole& Instance();
	~CConsole(void);
};