#include "PaintController.hpp"

bool done = false;
bool CtrlHandler(DWORD ctrlType)
{
	switch (ctrlType) {
	case CTRL_C_EVENT:
		//cout << "Ctrl-C pressed\n" << endl;
		done = true;
		return TRUE;
	}

	return FALSE;
}

PaintController::PaintController()
{
	model = new PaintModel();
	view = new PaintView(*model);
	//model->console.Save();
	//console.Save();
	model->console.Save();
	model->console.SetTitle("Paint");
	model->console.ResizeWindow(40, 40);
	WORD const BACKGROUND_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	model->console.ResetBackGoundColor(BACKGROUND_WHITE);
	model->console.HideCursor();
	model->console.SetConsoleMode();
	model->console.SetCTRLHandler(CtrlHandler, true);

	model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_RED, 4, { 0,1 });
	model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_GREEN, 4, { 6,1 });
	model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_BLUE, 4, { 12,1 });
	model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_BLUE | BACKGROUND_GREEN, 4, { 18,1 });
	model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_RED | BACKGROUND_GREEN, 4, { 24,1 });
	model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_RED | BACKGROUND_BLUE, 4, { 30,1 });

	ExecuteGame();
}

void PaintController::ExecuteGame()
{
	vector<INPUT_RECORD> inBuffer(128);
	while (!done) {
		DWORD numEvents;
		if (!model->console.getInstance().ReadConsoleValue(inBuffer)) {
			cerr << "Failed to read console input\n";
			break;
		}

		for (size_t iEvent = 0; iEvent < numEvents; ++iEvent) {
			switch (inBuffer[iEvent].EventType) {
				cout << inBuffer[iEvent].EventType << endl;
			case MOUSE_EVENT:
				MouseEventProc(inBuffer[iEvent].Event.MouseEvent);
				break;
			case KEY_EVENT:
				ProcessKeyEvent(inBuffer[iEvent].Event.KeyEvent);
				break;
			}

		}
	}

	model->console.Restore();
}

int PaintController::ProcessKeyEvent(KEY_EVENT_RECORD const & ker)
{
	//cout << ker.uChar.AsciiChar << endl;
	if (ker.uChar.AsciiChar == 'c' || 'C')
	{
		model->console.ResetBackGoundColor(model->console.getInstance().BACKGROUND_WHITE);
		model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_RED, 4, { 0,1 });
		model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_GREEN, 4, { 6,1 });
		model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_BLUE, 4, { 12,1 });
		model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_BLUE | BACKGROUND_GREEN, 4, { 18,1 });
		model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_RED | BACKGROUND_GREEN, 4, { 24,1 });
		model->console.getInstance().FillConsoleOutputAttributeLib(BACKGROUND_RED | BACKGROUND_BLUE, 4, { 30,1 });
	}
	else if ((ker.dwControlKeyState&LEFT_CTRL_PRESSED || ker.dwControlKeyState&RIGHT_CTRL_PRESSED) && ker.uChar.AsciiChar == 'c' || 'C')
	{
		done = true;
		model->console.Restore();
	}

	return EXIT_SUCCESS;
}

void PaintController::MouseEventProc(MOUSE_EVENT_RECORD const & mer)
{
	switch (mer.dwEventFlags)
	{
	case 0:	// button pressed or released
	{
		auto mask = mer.dwButtonState;
		if (mask&FROM_LEFT_1ST_BUTTON_PRESSED)
		{

			if (mer.dwMousePosition.Y == 1)
			{
				model->leftClicked = false;
				if (mer.dwMousePosition.X >= 0 && mer.dwMousePosition.X <= 4)
				{
					model->color = BACKGROUND_RED;
				}
				else if (mer.dwMousePosition.X >= 6 && mer.dwMousePosition.X <= 10)
				{
					model->color = BACKGROUND_GREEN;
				}
				else if (mer.dwMousePosition.X >= 12 && mer.dwMousePosition.X <= 16)
				{
					model->color = BACKGROUND_BLUE;
				}
				else if (mer.dwMousePosition.X >= 18 && mer.dwMousePosition.X <= 22)
				{
					model->color = BACKGROUND_BLUE | BACKGROUND_GREEN;
				}
				else if (mer.dwMousePosition.X >= 24 && mer.dwMousePosition.X <= 28)
				{
					model->color = BACKGROUND_RED | BACKGROUND_GREEN;
				}
				else if (mer.dwMousePosition.X >= 30 && mer.dwMousePosition.X <= 34)
				{
					model->color = BACKGROUND_RED | BACKGROUND_BLUE;
				}


			}
			else
			{
				model->leftClicked = true;

			}
		}
		
		else
		{
			model->leftClicked = false;
		}
	}
	break;
	}

	auto bufferLoc = mer.dwMousePosition;
	if (model->leftClicked)
	{
		model->console.SetColorAtOneSell(model->color, bufferLoc);
		
	}
}
