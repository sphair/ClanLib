
#pragma once

#include "Model/app_model.h"

class App : public clan::Application
{
public:
	App();

private:
	AppModel app_model;
	clan::ResourceManager resources;
	clan::UIThread ui_thread;
	clan::WindowManager window_manager;
};
