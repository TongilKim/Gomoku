#pragma once
#include <Command.hpp>
#include "PaintModel.hpp"
#include "PaintView.hpp"

class PaintController : public Command
{
private:
	PaintModel* model;
	PaintView* view;
public:
	PaintController();
	virtual void ExecuteGame() override;
	virtual int ProcessKeyEvent(KEY_EVENT_RECORD const & ker) override;
	virtual void MouseEventProc(MOUSE_EVENT_RECORD const & mer) override;

};