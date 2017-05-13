#pragma once

/*  When submitting in MAMA, put the line below in remark and make sure your project compiles in Linux!
    (this is not mandatory, but can add bonus points)
    if you put the above #define WINDOWS in comment for compilation in MAMA - remind us in the readme file that we need to take it out of comment!
 */
#define WINDOWS

#define DEBUG 0

#ifdef WINDOWS

#include <Windows.h>
#include <conio.h>

#else

#ifndef FALSE
    #define FALSE   0
#endif

#ifndef TRUE
    #define TRUE    1
#endif

    typedef unsigned short WORD;
    typedef unsigned long DWORD;

    typedef void *HANDLE;

    typedef struct _COORD {
        short X;
        short Y;
    } COORD;

    typedef struct _CONSOLE_CURSOR_INFO {
        DWORD dwSize;
        bool bVisible;
    } CONSOLE_CURSOR_INFO;

    #define STD_OUTPUT_HANDLE  ((DWORD)-11)

    HANDLE GetStdHandle(DWORD handle);
    bool SetConsoleTextAttribute(HANDLE handle, WORD att);
    bool SetConsoleCursorPosition(HANDLE handle, COORD coord);
    bool SetConsoleCursorInfo(HANDLE handle, const CONSOLE_CURSOR_INFO* cursor);

    void Sleep(unsigned long);
    int _getch(void);
    int _kbhit(void);

#endif




