
#pragma once

#include "../Workspace/dockable_component.h"

class WelcomePage : public DockableComponent
{
public:
	WelcomePage();

private:
	void on_resized();

	clan::Label *welcome;
	clan::ImageView *logo;
};
