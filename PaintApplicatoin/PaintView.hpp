#pragma once
#include "PaintModel.hpp"

class PaintView
{
private:
	PaintModel* model;
public:
	virtual void update();
	PaintView(PaintModel& m) { model = &m; }


};