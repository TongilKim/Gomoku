/*
*	   Program: Console.hpp
*	    Author: Tongil Kim
*	      Date: Mar 4, 2019
*	   Purpose: Outer Facade class.
*/

#ifndef CONSOLE_HEADER
#define CONSOLE_HEADER
#pragma once
#include "WindowsAPI.hpp"



/* OUTER FACADE */
class Console
{

private:
	WindowsAPI winAPI;
	WORD WINDOW_WIDTH = 40;
	WORD WINDOW_HEIGHT = 40;

	typedef bool(*CtrlHandler)(DWORD);

public:
	Console();
	~Console();

public:
	WindowsAPI getInstance();
	void Save();
	void SetCTRLHandler(CtrlHandler handleer, bool b);
	void SetTitle(std::string title);
	void ResizeWindow(unsigned short width, unsigned short height);
	void ResetBackGoundColor(WORD color);
	void HideCursor();
	void SetColorAtOneSell(WORD color, COORD c);
	void Restore();
	void SetConsoleMode();

	
};
#endif