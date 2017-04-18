#include <stdio.h>
#include "Config.h"

#ifndef WINDOWS

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

HANDLE GetStdHandle(DWORD handle) {return nullptr; };
bool SetConsoleTextAttribute(HANDLE handle, WORD att) { return false; };
bool SetConsoleCursorPosition(HANDLE handle, COORD coord) { return false; };
bool SetConsoleCursorInfo(HANDLE handle, const CONSOLE_CURSOR_INFO* cursor) { return false; };

void Sleep(unsigned long) {};
int _getch(void) { return 0; };
int _kbhit(void) { return 0; };

#endif
