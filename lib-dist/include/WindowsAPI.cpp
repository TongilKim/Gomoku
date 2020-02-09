/*
*	   Program: WindowsAPI.cpp
*	    Author: Tongil Kim
*	      Date: Mar 4, 2019
*	   Purpose: Inner Facade  class.
*/
#include "WindowsAPI.hpp"

//=======================================================
// Application
//=======================================================

// Window Layout
#pragma region WindowLayout


WORD WINDOW_WIDTH = 40;
WORD WINDOW_HEIGHT = 40;
#pragma endregion

#pragma region ApplicationData
// Application data
bool applicationQuitting = false;
DWORD terminationEventIdx = -1;

bool editControlHasFocus = false;
string editControlString;
string::size_type editControlCursorPos = 0;
decltype(editControlCursorPos) editControlAperture = 0;

bool buttonState = false;
#pragma endregion




//=======================================================
// Event Handlers
//=======================================================


WindowsAPI::WindowsAPI()
{
	hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

WindowsAPI::~WindowsAPI()
{
	hConsoleInput = nullptr;
	hConsoleOutput = nullptr;
}


LPSTR WindowsAPI::GetConsoleTitleALib(DWORD size)
{
	LPSTR lpConsoleT = new CHAR[size];
	GetConsoleTitleA(lpConsoleT, size);
	return lpConsoleT;
}

void WindowsAPI::SetConsoleTitleALib(string temp)
{
	THROW_IF_CONSOLE_ERROR(SetConsoleTitleA(temp.c_str()));
}

BOOL WindowsAPI::SetConsoleWindowInfoLib(BOOL bAbsolute, const SMALL_RECT * lpConsoleWindow)
{
	return SetConsoleWindowInfo(hConsoleOutput, bAbsolute, lpConsoleWindow);
}

BOOL WindowsAPI::SetConsoleScreenBufferSizeLib(COORD dwSize)
{
	return SetConsoleScreenBufferSize(hConsoleOutput, dwSize);
}

BOOL WindowsAPI::GetConsoleScreenBufferInfoLib(PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo)
{
	return GetConsoleScreenBufferInfo(hConsoleOutput, lpConsoleScreenBufferInfo);
}

BOOL WindowsAPI::FillConsoleOutputCharacterALib(CHAR lpCharacter, DWORD nLength, COORD dwWriteCoord)
{
	DWORD lpNumberOfCharsWritten;
	return FillConsoleOutputCharacterA(hConsoleOutput, lpCharacter, nLength, dwWriteCoord, &lpNumberOfCharsWritten);
}

BOOL WindowsAPI::FillConsoleOutputAttributeLib(WORD wAttribute, DWORD nLength, COORD dwWriteCoord)
{
	DWORD lpNumberOfCharsWritten;
	return FillConsoleOutputAttribute(hConsoleOutput, wAttribute, nLength, dwWriteCoord, &lpNumberOfCharsWritten);
}

BOOL WindowsAPI::SetConsoleCursorInfoLib(CONSOLE_CURSOR_INFO * lpConsoleCursorInfo)
{
	return SetConsoleCursorInfo(hConsoleOutput, lpConsoleCursorInfo);
}

BOOL WindowsAPI::SetConsoleModeMyLib()
{
	DWORD consoleMode = /*ENABLE_WINDOW_INPUT | */ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT;
	consoleMode |= ENABLE_EXTENDED_FLAGS;	// Stop windows from taking over the mouse.

	if (!SetConsoleMode(hConsoleInput, consoleMode)) {
		cerr << "\nERROR: could not set console mode.\n";
		return EXIT_FAILURE;
	}
	else
	{
		return true;
	}

	
}


void WindowsAPI::SaveConsoleState()
{
	// Save the old title
	/*originalTitle.resize(size_t(GetConsoleTitleA((DWORD)(64 * 1024)) + 1);
	originalTitle.shrink_to_fit();*/
	originalTitle = GetConsoleTitleALib(64 * 1024);

	// Get the old window/buffer size
	//GetConsoleScreenBufferInfo
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hConsoleOutput, &originalCSBI));

	// Save the desktop
	originalBuffer.resize(size_t(originalCSBI.dwSize.X)*originalCSBI.dwSize.Y);
	originalBufferCoord = COORD{ 0 };
	bufferRect.Right = originalCSBI.dwSize.X - 1;
	bufferRect.Bottom = originalCSBI.dwSize.Y - 1;
	THROW_IF_CONSOLE_ERROR(ReadConsoleOutputA(hConsoleOutput, originalBuffer.data(), originalCSBI.dwSize, originalBufferCoord, &bufferRect));

	// Save the cursor
	THROW_IF_CONSOLE_ERROR(GetConsoleCursorInfo(hConsoleOutput, &originalCCI));

	//  Save the console mode 
	// Configure the console mode
	THROW_IF_CONSOLE_ERROR(GetConsoleMode(hConsoleInput, &originalConsoleMode));

}



void WindowsAPI::RestoreWindow()
{
	// Restore the original settings/size
	sr = SMALL_RECT{ 0 };
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &sr));
	THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hConsoleOutput, originalCSBI.dwSize));
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hConsoleOutput, TRUE, &originalCSBI.srWindow));

	// Restore the original title
	SetConsoleTitleA(originalTitle.data());

	// Restore the desktop contents
	bufferRect = SMALL_RECT{ 0 };
	bufferRect.Right = originalCSBI.dwSize.X - 1;
	bufferRect.Bottom = originalCSBI.dwSize.Y - 1;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputA(hConsoleOutput, originalBuffer.data(), originalCSBI.dwSize, originalBufferCoord, &bufferRect));
	


	// Restore the cursor
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hConsoleOutput, &originalCCI));
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hConsoleOutput, originalCSBI.dwCursorPosition));

	SetConsoleMode(hConsoleInput, originalConsoleMode);
}

void WindowsAPI::HideTheCursor()
{
	auto newCCI = originalCCI;
	newCCI.bVisible = FALSE;
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hConsoleOutput, &newCCI));
}

void WindowsAPI::PaintScreen(WORD Colour)
{
	// Get the number of character cells in the current buffer
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hConsoleOutput, &csbi));

	// Fill the entire screen area white
	DWORD charsWritten;
	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD cursorHomeCoord{ 0, 0 };
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(hConsoleOutput, ' ', consoleSize, cursorHomeCoord, &charsWritten));
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hConsoleOutput, Colour, consoleSize, cursorHomeCoord, &charsWritten));

}

long WindowsAPI::ReadConsoleValue(vector<INPUT_RECORD>& buffer)
{
	DWORD lpNumberOfEventsRead;
	THROW_IF_CONSOLE_ERROR(ReadConsoleInput(hConsoleInput, buffer.data(), (DWORD)buffer.size(), &lpNumberOfEventsRead));
	return lpNumberOfEventsRead;
}



string XError::msg() const
{
	ostringstream oss;
	oss << "Error: " << code() << "\n";
	oss << ErrorDescription(code()) << "\n";
	oss << "In: " << file() << "\n";
	oss << "Line: " << line() << "\n";
	return oss.str();
}

std::string ErrorDescription(DWORD dwMessageID)
{
	char* msg;
	auto c = FormatMessageA(
		/* flags */			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		/* source*/			NULL,
		/* message ID */	dwMessageID,
		/* language */		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		/* buffer */		(LPSTR)&msg,
		/* size */			0,
		/* args */			NULL
	);

	std::string strMsg = (c == 0)
		? "unknown"
		: msg;
	LocalFree(msg);
	return strMsg;
}
