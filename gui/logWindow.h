#pragma once

#include <Windows.h>

int initLogWindow(HWND parent, HINSTANCE hInstance, int nCmdShow);

HWND createLogTextArea(HWND parentWindow, HINSTANCE hInstance);

void appendLogText(LPCTSTR newText);