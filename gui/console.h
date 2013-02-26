#pragma once

#include <windows.h>

class Console {

private:
	static Console* instance;

	Console();
public:
	static Console getInstance();
};

Console Console::getInstance() {
	if (Console::instance == NULL) {
		Console::instance = new Console();
	}
	return *Console::instance;
}

void startConsole(void);
void stopConsole(void);
boolean writeToConsole(CHAR* chBuf);
boolean writeToConsole(CHAR*, int);

void processConsoleOutput();