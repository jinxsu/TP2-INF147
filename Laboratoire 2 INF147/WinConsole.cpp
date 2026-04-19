// WinConsole.CPP : Windows text console management function library

//the following line is necessary for the GetConsoleWindow() function to work!
//it basically says that you are running this program on Windows 2000 or higher
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <time.h>        //necessary for the "clock()" function in "delay()" 
#include "WinConsole.h"

static int color = 15;   //current text color, default = WHITE
static int bkcolor = 0;  //current background color, default = BLACK

// --- Function definitions ---

/*-------------------------clrscr()-----------------------------------*/

void clrscr(void)  //clear screen: clears the contents of the console
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;	 //variable declarations
	HANDLE	hConsoleOut;
	COORD	Home = { 0,0 };
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	SetConsoleTextAttribute(hConsoleOut, WHITE);

	//fill the entire screen with blank caracters (' ')
	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;									//cursor position X coordinate reset to 0
	csbiInfo.dwCursorPosition.Y = 0;									//cursor position Y coordinate reset to 0
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);	//replace cursor at its initial coordinate
}

/*-------------------------clreol()-----------------------------------*/

void clreol(void)  //clear end of line: clears the remainder of current output line
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;	 //variable declarations
	HANDLE	hConsoleOut;
	COORD	Home, pos;
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	SetConsoleTextAttribute(hConsoleOut, WHITE);

	Home = csbiInfo.dwCursorPosition;
	pos.X = WIDTH_CONSOLE - csbiInfo.dwCursorPosition.X;   //length of remainder of line to be erased (= 80 - ..)

	//fill the remainder of the line, starting at current (Home) position, with blanks (' ')
	FillConsoleOutputCharacter(hConsoleOut, ' ', pos.X, Home, &dummy);
}

/*-------------------------clreoscr()----------------------------------*/

void clreoscr(void)  //clear end of screen: clears the remainder of current screen
{
	CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;    //variable declarations
	HANDLE	hConsoleOut;
	COORD	Home;
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	SetConsoleTextAttribute(hConsoleOut, WHITE);

	Home = csbiInfo.dwCursorPosition;

	//fill the remainder of the screen, starting at current (Home) position, with blanks (' ')
	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
}

/*-------------------------gotoxy()------------------------------------*/

void gotoxy(int x, int y)  //positions text cursor at (x, y) screen position
{
	CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;     //variable declarations
	HANDLE	hConsoleOut;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

	csbiInfo.dwCursorPosition.X = x;									//cursor position X coordinate 
	csbiInfo.dwCursorPosition.Y = y;									//cursor position Y coordinate 
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);	//set cursor at the given screen coordinate
}

// Following code from http://forums.microsoft.com/MSDN/ShowPost.aspx?PostID=590775&SiteId=1

/*-------------------------wherex()------------------------------------*/

int wherex()      //returns current text cursor (x) coordinate
{
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	return csbiInfo.dwCursorPosition.X;
}

/*-------------------------wherey()----------------------------------*/

int wherey()      //returns current text cursor (y) coordinate
{
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	return csbiInfo.dwCursorPosition.Y;
}

/*-------------------------delay()-----------------------------------*/

void delay(unsigned int delay_time)  //will force execution to "sleep" for given milliseconds
{
	clock_t end_time;

	end_time = delay_time + clock();
	while (end_time > clock()) {};
}

/*-------------------------textmode()--------------------------------*/

void textmode(int nblines)   //will resize console window to the given number of lines
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);  //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, r.right - r.left, nblines * 12 + 38, TRUE);

	/* to retrieve previous console dimensions (with: RECT r) */
	//MoveWindow(console, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
}

/*-------------------------textcolor()-------------------------------*/

void textcolor(int k)        //sets text color attribute
{
	HANDLE  hConsole;

	color = k;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//console color value [0..255] = (bkcolor*16 + color)
	SetConsoleTextAttribute(hConsole, bkcolor * 16 + color);
}

/*-------------------------textbackground()--------------------------*/

void textbackground(int bk)  //sets background color attribute
{
	HANDLE  hConsole;

	bkcolor = bk;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//console color value [0..255] = (bkcolor*16 + color)
	SetConsoleTextAttribute(hConsole, bkcolor * 16 + color);
}

/*--------------------------fullscreen()-----------------------------*/

void fullscreen()			//sets the console in full-screen mode (ALT-ENTER)
{
	keybd_event(VK_MENU, 0xb8, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0xb8, KEYEVENTF_KEYUP, 0);
}
/*-------------------------------------------------------------------*/
