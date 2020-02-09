/*
*	   Program: Console.cpp
*	    Author: Tongil Kim
*	      Date: Mar 4, 2019
*	   Purpose: Outer Facade class
*/
#include "Console.hpp"



Console::Console() 
{
	// Save Console
	WindowsAPI win;
	Save();
}

Console::~Console() 
{
	// when exit, restore to old console state
	Restore();
}

/*
*	   Name: getInstance()
*	Accepts: nothing
*	Returns: nothing
*	Purpose: return the WindowsAPI's instance.
*/
WindowsAPI Console::getInstance()
{
	return winAPI;
}

/*
*	   Name: Save()
*	Accepts: nothing
*	Returns: nothing
*	Purpose: wrap save console state.
*/
void Console::Save() {
	winAPI.SaveConsoleState();
}

void Console::SetCTRLHandler(CtrlHandler handleer, bool b)
{
	// Install a control handler to trap ^C
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)handleer, b)) {
		cout << "Terminate program with Ctrl-C\n";
	}
	else {
		cerr << "ERROR: failed to install control handler." << endl;
	}

}

void Console::SetTitle(std::string title)
{
	winAPI.SetConsoleTitleALib(title);
}

void Console::ResizeWindow(unsigned short width, unsigned short height)
{

	SMALL_RECT sr{ 0 };
	winAPI.SetConsoleWindowInfoLib(TRUE, &sr);
	//THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &sr));

	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	winAPI.SetConsoleScreenBufferSizeLib(bufferSize);
		//THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hConsoleOutput, bufferSize));

	CONSOLE_SCREEN_BUFFER_INFO sbi;
	winAPI.GetConsoleScreenBufferInfoLib(&sbi);
	//THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hConsoleOutput, &sbi));

	sr.Top = sr.Left = 0;
	width = std::min((SHORT)width, sbi.dwMaximumWindowSize.X);
	height = std::min((SHORT)height, sbi.dwMaximumWindowSize.Y);
	sr.Right = width - 1;
	sr.Bottom = height - 1;

	winAPI.SetConsoleWindowInfoLib(TRUE, &sr);
	//THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &sr));
	
	
}

void Console::ResetBackGoundColor(WORD color)
{
	winAPI.FillConsoleOutputCharacterALib(' ', WINDOW_WIDTH * WINDOW_HEIGHT, {0,0});
	winAPI.FillConsoleOutputAttributeLib(color, WINDOW_WIDTH * WINDOW_HEIGHT, {0,0});

}

void Console::HideCursor()
{
	auto newCCI = winAPI.originalCCI;
	newCCI.bVisible = FALSE;
	winAPI.SetConsoleCursorInfoLib(&newCCI);
}

void Console::SetColorAtOneSell(WORD color, COORD c)
{

	winAPI.FillConsoleOutputAttributeLib(color, 1, c);
}

/*
*	   Name: Restore()
*	Accepts: nothing
*	Returns: nothing
*	Purpose: wrap restore console state.
*/
void Console::Restore() {
	winAPI.RestoreWindow();
}

void Console::SetConsoleMode()
{
	winAPI.SetConsoleModeMyLib();
}






