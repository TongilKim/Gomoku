/*
*	   Program: GomokuController.cpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include "GomokuController.hpp"
using namespace std;

GomokuController::GomokuController()
{
	model = new GomokuModel();
	bv = new BoarderView(*model);
	sv = new ScoreView(*model);
	pv = new PlayerView(*model);
	mv = new MoveView(*model);

	model->Attach(bv);
	model->Attach(sv);
	model->Attach(pv);
	model->Attach(mv);

}

void GomokuController::ExecuteGame()
{
	model->console.ResizeWindow(90, 22);
	model->console.SetTitle("Gomoku Game");

	cout << "Enter name for the 1P: ";
	wcin >> model->player1;
	cout << "Enter name for the 2P: ";
	wcin >> model->player2;
	
	model->SetColor(model->BLACK, model->DARK_YELLOW);
	model->x = CENTER;
	model->y = CENTER;
	system("cls");


	// print X lines ( A, B, C, D ...) 
	for (int i = 0; i < SIZE; i++)
	{
		model->gotoxy((model->nX + i) * 2, model->nY + SIZE);
		printf("%2c", i + 'A');
	}
	// print Y lines ( 1, 2, 3, 4 ...)
	for (int i = 0, j = SIZE; i < SIZE; i++, j--)
	{
		model->gotoxy((model->nX - 1) * 2, model->nY + j - 1);
		printf("%2d", i + 1);
	}
	model->SetColor(model->GRAY);

	model->setStatus(GomokuModel::BOARDVIEW);
	model->Notify();

	//// Draw the game board
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			model->printBoard(j, i, model->LINE);
		}
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			model->arrBoard[i][j] = -1;
		}
	}

	model->setStatus(GomokuModel::SCOREVIEW);
	model->Notify();

	model->setStatus(GomokuModel::PLAYERVIEW);
	model->Notify();

	model->cursorMove(model->x, model->y);

	int result = 0;
	vector<INPUT_RECORD> inBuffer(128);
	while (true)
	{
		//result = checkKey();
		// isnt it result?
		// just let me check mine one sec
		//return = ProcessKeyEvent(KEY_EVENT_RECORD const & ker);
		// read input buffer repeatedly
		DWORD numEvents = model->console.getInstance().ReadConsoleValue(inBuffer);

		for (size_t iEvent = 0; iEvent < numEvents; ++iEvent)
		{
			result = ProcessKeyEvent(inBuffer[iEvent].Event.KeyEvent);
			switch (result)
			{
			case GomokuModel::GAMEOVER:
				model->endMsg(model->curStone);
				break;
			case GomokuModel::CHANGE:
				result = ProcessKeyEvent(inBuffer[iEvent].Event.KeyEvent);
				model->changeStone();			
				break;
			}
		}
		//switch (result)
		//{
		//	// When user pressed ESC, the game turns off. 
		////case FINISH: return false;
		//case GomokuModel::GAMEOVER: model->endMsg(model->curStone);
		//case GomokuModel::CHANGE: 
		//	model->changeStone();
		//	break;

		//default: break;
		//}
		//Sleep(20);
	}

}

int GomokuController::ProcessKeyEvent(KEY_EVENT_RECORD const & ker)
{
	int ch;

	if (true) ch = model->getKey();
	else return 0;

	switch (ch)
	{
	case UP:
		model->cursorMove(model->x, --model->y);
		break;

	case DOWN:
		model->cursorMove(model->x, ++model->y);
		break;

	case LEFT:
		model->cursorMove(--model->x, model->y);
		break;

	case RIGHT:
		model->cursorMove(++model->x, model->y);
		break;

	case ESC: return FINISH;
	case ' ': return model->placement(model->x, model->y, model->curStone);
	case 'u':
	case 'U': return model->undo(model->curStone);
	case 'r':
	case 'R': return model->resetGame();
	default: break;
	}
	return 0;
}

void GomokuController::MouseEventProc(MOUSE_EVENT_RECORD const & mer)
{
}

//int GomokuController::checkKey()
//{
//	int ch;
//
//	if (true) ch = getKey();
//	else return 0;
//
//	switch (ch)
//	{
//	case UP:
//		model->cursorMove(model->x, --model->y);
//		break;
//
//	case DOWN:
//		model->cursorMove(model->x, ++model->y);
//		break;
//
//	case LEFT:
//		model->cursorMove(--model->x, model->y);
//		break;
//
//	case RIGHT:
//		model->cursorMove(++model->x, model->y);
//		break;
//
//	case ESC: return FINISH;
//	case ' ': return model->placement(model->x, model->y, model->curStone);
//	case 'u':
//	case 'U': return model->undo(model->curStone);
//
//	default: break;
//	}
//	return 0;
//}
