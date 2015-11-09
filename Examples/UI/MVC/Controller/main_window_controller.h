
#pragma once

#include "View/form_view.h"

class MainWindowController : public clan::WindowController
{
public:
	MainWindowController();

	std::shared_ptr<FormView> form_view = std::make_shared<FormView>();
};
