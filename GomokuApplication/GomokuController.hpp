/*
*	   Program: GomokuController.hpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include <Command.hpp>
#include "GomokuModel.hpp"
#include "GomokuView.hpp"

class GomokuController : public Command
{
private:
	GomokuModel* model;
	PlayerView* pv;
	ScoreView* sv;
	BoarderView* bv;
	MoveView* mv;

public:
	GomokuController();
	virtual void ExecuteGame() override;

	virtual int ProcessKeyEvent(KEY_EVENT_RECORD const & ker) override;
	virtual void MouseEventProc(MOUSE_EVENT_RECORD const & mer) override;

	//int checkKey();


};

