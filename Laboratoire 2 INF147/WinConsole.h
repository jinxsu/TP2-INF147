#pragma once
// WinConsole.H : Windows text console management function library

#define WIDTH_CONSOLE  110      //width of the console in number of chars

// --- Function Prototypes ---
void fullscreen();              //full-screen console
void clrscr(void);				//clears the contents of the console
void clreol(void);				//clears the remainder of current output line
void clreoscr(void);			//clears the remainder of current screen
void gotoxy(int, int);		    //positions text cursor at (x, y) screen position
int  wherex();					//returns current text cursor (x) coordinate
int  wherey();					//returns current text cursor (y) coordinate
void delay(unsigned int);	    //will force execution to "sleep" for given milliseconds
void textmode(int);				//will resize console window to the given number of lines
void textcolor(int);			//sets text color attribute
void textbackground(int);       //sets background color attribute

//List of 16 color constants (values: 0..15)
enum colors {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY,
    LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};

/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/

