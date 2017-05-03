#pragma once

enum { BLACK, DARK_BLUE, DARK_GREEN, DARK_CYAN, DARK_RED, DARK_PURPLE, OLIVE, WHITE, GREY, BLUE, GREEN, CYAN, RED, PURPLE, YELLOW };

void setTextColor(int foreground_color, int background_color);
void setTextColor(int foreground_color);
void clearScreen();
void gotoxy(int x, int y);
void hideCursor();

void waitForAnyKeyToContinue();
