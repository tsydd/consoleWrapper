#include "Console.h"

#include "util.h"


CConsole::CConsole(void)
{
	log("constructor\n");
}


CConsole::~CConsole(void)
{
	log("destructor\n");
}

const CConsole& CConsole::Instance()
{
	static CConsole instance;
	return instance;
}