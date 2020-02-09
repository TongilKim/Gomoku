/*
*	   Program: GomokuView.cpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include "GomokuView.hpp"

void BoarderView::update()
{
	if (model->getStatus() == GomokuModel::BOARDVIEW)
	{
		model->SetColor(model->BLACK, model->YELLOW);
		model->gotoxy( 0, 5);
		std::wcout << "Move: 'Arrow Keys'" <<"\n\nSet stone: 'Space bar'" << "\n\nUndo: 'u'" << "\n\nRestart: 'r'" << std::endl;
	}
	
}

/* ========================================================================== */
/* shows Players scores*/
void ScoreView::update()
{
	if (model->getStatus() == GomokuModel::SCOREVIEW)
	{

		model->SetColor(model->GRAY);
		model->gotoxy((7 + SIZE), 3);
		//wcout << L"Player1:" << p1 << "		Player2: " << p2 << endl;
		std::wcout << L"Score:" << model->player_1_Score;
		model->SetColor(model->BLACK, model->WHITE);
		model->gotoxy((25 + SIZE), 3);
		std::wcout << L"Score:" << model->player_2_Score;

	}
}
/* ========================================================================== */
/* shows Moves of stone position (list of moves view)*/
void MoveView::update()
{
	if (model->getStatus() == GomokuModel::MOVEVIEW)
	{
		// set Player1's move title
		model->gotoxy((model->nX + SIZE + 10) * 2, 3);
		model->SetColor(model->WHITE, model->BLACK); // black background, white letter // 1P
		std::wcout << "Player1's Move";

		// set Player2's move title
		model->gotoxy((model->nX + SIZE + 1) * 2, 3);
		model->SetColor(model->BLACK, model->WHITE); // white background, black letter // 2P
		std::wcout << "Player2's Move" ;
		
		
		
		//Black : 0 , White : 1
		if (model->curStone == 0)
		{
			model->SetColor(model->WHITE, model->BLACK); // black background, white letter // 1P
			model->gotoxy((model->nX + SIZE + 10) * 2, model->white_movesMsgPosition);

			// Save the current move message position into socreList vector
			// so that you can clear them on the console later when you replay the game.
			int tempX = (model->nX + SIZE + 10) * 2;
			int tempY = model->white_movesMsgPosition;
			model->p1_scoreList.push_back(make_pair(tempX, tempY));

			wchar_t tempChr = 'A' + SIZE;
			wchar_t c = tempChr - (SIZE - model->x);  // never mind these two lines
			
			std::wcout << "X:" << c << " Y:" << SIZE - model->y << std::endl;
			model->white_movesMsgPosition += 1;
		}
		if (model->curStone == 1)
		{
			model->SetColor(model->BLACK, model->WHITE); // white background, black letter // 2P
			model->gotoxy((model->nX + SIZE + 1) * 2, model->black_movesMsgPosition);

			// Save the current move message position into socreList vector
			// so that you can clear them on the console later when you replay the game.
			int tempX = (model->nX + SIZE + 1) * 2;
			int tempY = model->black_movesMsgPosition;
			model->p2_scoreList.push_back(make_pair(tempX, tempY));

			wchar_t tempChr = 'A' + SIZE;
			wchar_t c = tempChr - (SIZE - model->x);

			std::wcout << "X:" << c << " Y:" << SIZE - model->y << std::endl;
			model->black_movesMsgPosition += 1;
		}
		
		
	}
	
}
/* ========================================================================== */
/* shows Players name*/
void PlayerView::update()
{
	if (model->getStatus() == GomokuModel::PLAYERVIEW)
	{

		model->SetColor(model->GRAY);
		model->gotoxy((7 + SIZE), 2);
		std::wcout << L"Player1:" << model->player1;

		model->SetColor(model->BLACK, model->WHITE);
		model->gotoxy((25 + SIZE), 2);
		std::wcout << L"Player2: " << model->player2;
	}
	
}
