#pragma once
/*
*	   Program: Command.hpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include <Windows.h>

class Command
{
public:
	virtual void ExecuteGame() = 0;
	virtual int ProcessKeyEvent(KEY_EVENT_RECORD const & ker) = 0;
	virtual void MouseEventProc(MOUSE_EVENT_RECORD const & mer) = 0;
};