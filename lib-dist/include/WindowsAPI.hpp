/*
*	   Program: WindowsAPI.hpp
*	    Author: Tongil Kim
*	      Date: Mar 4, 2019
*	   Purpose: Inner Facade class
*/
#pragma once

#include <Windows.h>
#undef min

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#if defined(_DEBUG) && defined(_DLL) // Debug
#pragma comment (lib, "lib-mt-gd.lib")
#elif defined(_DEBUG) && !defined(_DLL) // Debug (Static)
#pragma comment (lib, "lib-mt-sgd.lib")
#elif !defined(_DEBUG) && defined(_DLL) // Release
#pragma comment (lib, "lib-mt.lib")
#elif !defined(_DEBUG) && !defined(_DLL) // Release (Static)
#pragma comment (lib, "lib-smt.lib")
#endif

//=======================================================
// Error exception class and utilities
//=======================================================
#pragma region XError

/* ErrorDescription */
std::string ErrorDescription(DWORD dwMessageID);

/* Console error exception class. */
class XError {
public:
	using id_type = decltype(GetLastError());
	using file_type = char const *;
	using string_type = std::string;
private:
	id_type code_;
	int	line_;
	file_type file_;
public:
	XError(int line, file_type file) : code_(GetLastError()), line_(line), file_(file) {}
	auto code() const -> id_type { return code_; }
	auto line() const -> int { return line_; }
	auto file() const -> file_type { return file_; }

	string_type msg() const;
};

/* Console error exception throw helper macro. */
#define THROW_IF_CONSOLE_ERROR(res) if(!res) throw XError(__LINE__,__FILE__)
#define THROW_CONSOLE_ERROR() throw XError(__LINE__,__FILE__)

#pragma endregion

/* INNER FACADE */
class WindowsAPI
{
public:
	WindowsAPI();
	~WindowsAPI();
public:
	// Colours
	WORD const FOREGROUND_BLACK = 0;
	WORD const FOREGROUND_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	WORD const BACKGROUND_BLACK = 0;
	WORD const BACKGROUND_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

	WORD const B_RED = BACKGROUND_RED | BACKGROUND_INTENSITY;
	WORD const B_GREEN = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	WORD const B_BLUE = BACKGROUND_BLUE | BACKGROUND_INTENSITY;

	// System Data
	CONSOLE_SCREEN_BUFFER_INFO	originalCSBI;
	CONSOLE_CURSOR_INFO			originalCCI;
	vector<CHAR_INFO>			originalBuffer;
	COORD						originalBufferCoord;
	DWORD						originalConsoleMode;
	WORD						currentConsoleWidth = 0;
	SMALL_RECT					bufferRect{ 0 };
	string			    originalTitle;
	SMALL_RECT					sr;


	HANDLE hConsoleInput;
	HANDLE hConsoleOutput;
	
	void SaveConsoleState();

	LPSTR GetConsoleTitleALib(DWORD size);
	void SetConsoleTitleALib(string temp);
    BOOL SetConsoleWindowInfoLib(_In_ BOOL bAbsolute,_In_ CONST SMALL_RECT* lpConsoleWindow);
    BOOL SetConsoleScreenBufferSizeLib(_In_ COORD dwSize);
	BOOL GetConsoleScreenBufferInfoLib(_Out_ PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo);
	BOOL FillConsoleOutputCharacterALib(_In_ CHAR lpCharacter, _In_  DWORD   nLength, _In_  COORD   dwWriteCoord);
	BOOL FillConsoleOutputAttributeLib(_In_ WORD wAttribute,_In_ DWORD nLength,_In_ COORD dwWriteCoord);
	BOOL SetConsoleCursorInfoLib(CONSOLE_CURSOR_INFO * lpConsoleCursorInfo);
	BOOL SetConsoleModeMyLib();

	void RestoreWindow();
	void HideTheCursor();
	void PaintScreen(WORD Colour);
	long ReadConsoleValue(vector<INPUT_RECORD>& buffer);
	//void Execute();

};

