/*
*	   Program: GomokuModel.cpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include "GomokuModel.hpp"

void GomokuModel::SetColor(int color, int bgcolor)
{
	color &= 0xf; // 0xf is black
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
	
}

void GomokuModel::gotoxy(short x, short y)

{
	COORD XY = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), XY);
}

void GomokuModel::SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int GomokuModel::getLineNumber(int x, int y)
{
	{
		// LEFT vertical line
		if (x == 0)
		{
			// if x, y == 0 then it's LEFT upper corner
			if (y == 0) return 0;
			// LEFT down corner
			else if (y == SIZE - 1) return 6;
			// LEFT vertical line
			else return 3;
		}
		// RIGHT vertical line 
		else if (x == SIZE - 1)
		{
			// RIGHT upper corner 
			if (y == 0) return 2;
			// RIGHT down corner
			else if (y == SIZE - 1) return 8;
			// RIGHT vertical line 
			else return 5;
		}
		// LEFT horizontal line
		else if (y == 0)
		{
			return 1;
		}
		// DOWN horizontal line
		else if (y == SIZE - 1)
		{

			return 7;
		}
		// CENTER 
		else return 4;
	}
}

void GomokuModel::cursorMove(int & x, int & y)
{
	if (x < 0)          x = 0;
	else if (y < 0)     y = 0;
	else if (x >= SIZE) x = SIZE - 1;
	else if (y >= SIZE) y = SIZE - 1;
	gotoxy((x + nX) * 2, y + nY);
}

void GomokuModel::setXY(int ax, int ay)
{
	x = ax;
	y = ay;
}


bool GomokuModel::isOccupy()
{

	return arrBoard[y][x] != -1;

}

void GomokuModel::undoSet()
{
		undoflag = true;
		lx = x;
		ly = y;
}

void GomokuModel::saveBoard(int stone)
{
	printBoard(x, y, stone);
	arrBoard[y][x] = stone;
	undoSet();

	setStatus(BOARDVIEW);
	Notify();
}

void GomokuModel::printBoard(int x, int y, int type)
{
	const int color[] = { BLACK, WHITE, BLACK, BLACK, BLACK };
	const wchar_t *shape[] = { L"┌", L"┬", L"┐", L"├", L"┼", L"┤", L"└", L"┴", L"┘", L"●", L"●" };

	// set the console background colour
	SetColor(color[type], DARK_YELLOW);
	// the line shapes are 9 different kinds so
	// get the position by using getLineNumber() and draw the line shape that matched.
	if (type == LINE) type = getLineNumber(x, y);
	// line index range is 0 - 8, after that are just stones for players so add 9 for stones.
	else type += 9;
	// set the console position to draw the shape
	gotoxy((x + nX) * 2, y + nY);
	/* these unkwon below two lines are used to print out the unicode of shape.*/
	fflush(stdout);
	_setmode(_fileno(stdout), _O_WTEXT);
	// print them
	wprintf(L"%2s", shape[type]);
	SetColor(GRAY);
}



int GomokuModel::rowCheck(int dx, int dy, int cnt, int stone)
{
	while (dx >= 0 && arrBoard[dy][dx] == stone) dx--;
	for (dx++; dx < SIZE && arrBoard[dy][dx] == stone; dx++) cnt++;
	return cnt;
	
}

int GomokuModel::colCheck(int dx, int dy, int cnt, int stone)
{
	while (arrBoard[dy][dx] == stone) dy--;
	for (dy++; arrBoard[dy][dx] == stone; dy++) cnt++;
	return cnt;

}

int GomokuModel::leftUpCheck(int dx, int dy, int cnt, int stone)
{
	for (; arrBoard[dy][dx] == stone; dx--, dy--);
	for (dx++, dy++; arrBoard[dy][dx] == stone; dx++, dy++) cnt++;
	return cnt;
}

int GomokuModel::rightUpCheck(int dx, int dy, int cnt, int stone)
{
	for (; arrBoard[dy][dx] == stone; dx--, dy++);
	for (dx++, dy--; arrBoard[dy][dx] == stone; dx++, dy--) cnt++;
	return cnt;
}

bool GomokuModel::checkGameOver(int stone)
{
	int dx = x;
	int dy = y;
	int cnt = 0;

	int row = rowCheck(dx, dy, cnt, stone);
	int col = rowCheck(dx, dy, cnt, stone);
	int leftUp = leftUpCheck(dx, dy, cnt, stone);
	int rightUp = rightUpCheck(dx, dy, cnt, stone);
	int checkArry [] = { row, col, leftUp, rightUp };
	
	for (int i = 0; i < 4; i++)
		if (5 <= checkArry[i]) return true;
		
	return false;

	
}

int GomokuModel::placement(int ax, int ay, int stone)
{
	// white stone = 1 , black stone = 0
	setXY(ax, ay);
	if (isOccupy()) { return OCCUPIED; }
	else
	{
		saveBoard(stone);
		//showMovesMsg(0);
		
		setStatus(MOVEVIEW);
		Notify();
		
		if (stone == 0) player_1_Score += 1;
		if (stone == 1) player_2_Score += 1;
		setStatus(SCOREVIEW);
		Notify();
		
	}
	
	if (checkGameOver(stone)) { return GAMEOVER; }
	return CHANGE;
}

int GomokuModel::undo(int & stone)
{

	if (!undoflag) return NOTUNDO;

	printBoard(lx, ly, LINE);
	arrBoard[ly][lx] = -1;
	//if (stone == 0)
	//{
	//	SetColor(BLACK, DARK_YELLOW);
	//	gotoxy(p1_scoreList.back().first, p1_scoreList.back().second);
	//	std::wcout << L"             ";
	//	
	//	//std::wcout << p1_scoreList.back().first << std::endl;
	//}
	//if (stone == 1)
	//{
	//	SetColor(BLACK, DARK_YELLOW);
	//	gotoxy(p2_scoreList.back().first, p2_scoreList.back().second);
	//	std::wcout << L"             ";
	//}
	undoflag = false;

	setStatus(BOARDVIEW);
	Notify();
	setStatus(SCOREVIEW);
	Notify();
	setStatus(MOVEVIEW);
	Notify();

	return CHANGE;
	
}
int GomokuModel::resetGame()
{
	// RESET player's score
	player_1_Score = 0;
	player_2_Score = 0;

	// Player 1
	SetColor(GRAY);
	gotoxy((7 + SIZE), 3);
	std::wcout << L"Score:  ";

	// Player 2
	SetColor(BLACK, WHITE);
	gotoxy((25 + SIZE), 3);
	std::wcout << L"Score:  ";

	// RESET the game board
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			printBoard(j, i, LINE);
		}
	}
	// RESET board array
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			arrBoard[i][j] = -1;
		}
	}
	// RESET move message list

	for (int i = 0; i < p2_scoreList.size(); i++)
	{
		SetColor(BLACK, DARK_YELLOW);
		gotoxy(p2_scoreList[i].first, p2_scoreList[i].second);
		std::wcout << L"             ";
	}

	for (int i = 0; i < p1_scoreList.size(); i++)
	{
		SetColor(BLACK, DARK_YELLOW);
		gotoxy(p1_scoreList[i].first, p1_scoreList[i].second);

		std::wcout << L"          ";
	}
	//std::wcout << L"SIZE =  " << p2_scoreList.size() << L", " << p1_scoreList.size() << std::endl;
	p1_scoreList.clear();
	p2_scoreList.clear();

	curStone = NULL;
	black_movesMsgPosition = 4;
	white_movesMsgPosition = 4;
	
	x = CENTER;
	y = CENTER;
	setStatus(BOARDVIEW);
	Notify();
	setStatus(SCOREVIEW);
	Notify();
	setStatus(MOVEVIEW);
	Notify();

	return EXIT_SUCCESS;
}
bool GomokuModel::changeStone()
{

	curStone = (curStone + 1) % 2;

	// By indicating the message, the cursor has moved, so get the cursor position back to board.
	cursorMove(x, y);
	return true;
}

bool GomokuModel::endMsg(int stone)
{
	const wchar_t *winner[] = { L"    Player1 WIN.\n    Play again?     ",
							L"    Player2 WIN.\n    Play again?     " };

	if (MessageBox(NULL, LPCWSTR(winner[stone]), L"ALERT", MB_YESNO) == IDYES)
	{

		resetGame();
		return true;
	}
	else return false;
}

int GomokuModel::getKey()
{
	int ch;

	ch = _getch();
	if (ch == 0 || ch == 0xE0) ch = _getch();

	return ch;
}

