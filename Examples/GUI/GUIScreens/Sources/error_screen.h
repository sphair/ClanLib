
#pragma once

#include "ui_screen.h"

class ErrorScreen : public UIScreen
{
public:
	ErrorScreen(CL_GUIManager *gui);
	~ErrorScreen();

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &rect);
	void on_button_click();

	CL_Label *label;
	CL_PushButton *button;
};
