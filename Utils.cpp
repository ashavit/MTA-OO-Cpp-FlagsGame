#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Config.h"

#include "Utils.h"

using namespace std;

void setTextColor(int foreground_color, int background_color) {
	int color = foreground_color | background_color * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setTextColor(int foreground_color) {
	setTextColor(foreground_color, 0);
}

void clearScreen() {
	system("cls");
}

// function definition -- requires windows.h
void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void hideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void waitForAnyKeyToContinue()
{
	cout << endl << "Press any key to continue..." << endl;
	while (!_kbhit()) {	}
}