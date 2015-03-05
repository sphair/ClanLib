
#pragma once

class Program : public clan::Application
{
public:
	Program();
	bool update() override;
	
	static void exit_func();
	static bool exit;
};
