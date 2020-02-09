/*
*	   Program: GomokuView.hpp
*	    Author: Tongil Kim
*	      Date: Apr 19, 2019
*/
#include "GomokuModel.hpp"
#include <iostream>


class BoarderView : public Observer
{
private:
	GomokuModel* model;
public:
	virtual void update();
	BoarderView(GomokuModel& gm) { model = &gm; }
};

class ScoreView : public Observer
{
private:
	GomokuModel* model;
public:
	virtual void update();
	ScoreView(GomokuModel& gm) { model = &gm; }
};

class MoveView : public Observer
{
private:
	GomokuModel* model;
public:
	virtual void update();
	MoveView(GomokuModel& gm) { model = &gm; }

};

class PlayerView : public Observer
{
private:
	GomokuModel* model;
public:
	virtual void update();
	PlayerView(GomokuModel& gm) { model = &gm; }

};




