#pragma once

#include "../view.h"

class WelcomeView : public View
{
public:
	WelcomeView(CL_GUIComponent *parent, MainWindow *mainwindow);
	~WelcomeView();

private:
	void on_resized();
};
