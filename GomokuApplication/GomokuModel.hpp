/*
*	   Program: GomokuModel.hpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <Subject.hpp>

#include <cstdlib>
#include <Console.hpp>
/* NOTE: required include this libs to use 'UNI code'.
 */
#include <fcntl.h>
#include <io.h>
#include <string>
#include <vector>
#include <conio.h>
#define SIZE 15
#define CENTER (SIZE / 2)
#define BACK_SPACE       8
#define    ENTER           13
#define    ESC                27
#define UP                72
#define DOWN            80
#define LEFT            75
#define RIGHT            77
#define PAGE_UP            73
#define PAGE_DOWN        81
#define DEL                83
#define FINISH          -1

class GomokuModel : public Subject
{
public:
	enum { BLACK_STONE, WHITE_STONE, BLACKWIN, WHITEWIN, LINE };
	enum { OCCUPIED = 10, NOTUNDO, GAMEOVER, CHANGE };
	enum {
		BLACK,
		DARK_BLUE,
		DARK_GREEN,
		DARK_SKY_BLUE,
		DARK_RED,
		DARK_VIOLET,
		DARK_YELLOW,
		GRAY,
		DARK_GRAY,
		BLUE,
		GREEN,
		SKY_BLUE,
		RED,
		VIOLET,
		YELLOW,
		WHITE,
	};

	enum Status {
		BOARDVIEW,
		SCOREVIEW,
		PLAYERVIEW,
		MOVEVIEW
	};
	/* ========================================================================== */
	/* Model instances */
	Status status;
	Console console;

	/* ========================================================================== */
	/* Model variables */
	int nX = (40 - SIZE - 1) / 2;
	int nY = 11 - (SIZE / 2);
	int x, y;
	bool   undoflag = false;
	int    lx, ly;
	int     curStone = NULL;
	std::wstring player1, player2;
	int player_1_Score = 0, player_2_Score = 0;
	vector< pair <int, int> > p1_scoreList;
	vector< pair <int, int> > p2_scoreList;
	int type; // to figure out if it is used for line or stones.
	int white_movesMsgPosition = 4; // this variable needs to move the list of moves position. 
	int black_movesMsgPosition = 4;
	

	/* ========================================================================== */
	/* Used to save the every position of placed stones */
	int arrBoard[SIZE][SIZE];
	/* ========================================================================== */
	/* CONSOLE Letter & Background COLOUR */
	void SetColor(int color, int bgcolor);
	/* ========================================================================== */
	/* CONSOLE CURSOR POSITION */
	void gotoxy(short x, short y);
	/* ========================================================================== */
	/* CONSOLE Letter COLOUR */
	void SetColor(int color);

	/* ========================================================================== */
	/* return the console position number to draw a BOARD LINE */
	int getLineNumber(int x, int y);

	/* ========================================================================== */
	/* function for let the cursor moves when user pressed the arrow keys */
	void cursorMove(int &x, int &y);

	/* ========================================================================== */
	/* Set the X,Y position */
	void setXY(int ax, int ay);

	/* ========================================================================== */
	/* Checking whether the current cursor position is occupied to set the stone by another player or not. */
	bool isOccupy();

	void undoSet();
	
	/* ========================================================================== */
	/* Save the the stone into board array and draw the stone on the board*/
	void saveBoard(int stone);

	/* ========================================================================== */
	/* Print the Board or Stone*/
	void printBoard(int x, int y, int type);
	int rowCheck(int dx, int dy, int cnt, int stone);
	int colCheck(int dx, int dy, int cnt, int stone);
	int leftUpCheck(int dx, int dy, int cnt, int stone);
	int rightUpCheck(int dx, int dy, int cnt, int stone);

	bool checkGameOver(int stone);

	/* ========================================================================== */
	/* firstly, check if current cursor position is available to place the stone,
		and once it has placed then save that into board array. After all, check if
		game is over by the placed position.
	*/
	int placement(int ax, int ay, int stone);

	int undo(int &stone);

	int resetGame();

	/* ========================================================================== */
/* change the play turn between the two players*/
	bool changeStone();

	bool endMsg(int stone);
	Status getStatus() { return status; }
	void setStatus(Status s) { status = s; }

	int getKey();

};

#endif