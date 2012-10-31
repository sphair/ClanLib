
#pragma once

class Program
{
public:
	static int main(const std::vector<std::string> &args);
	static bool on_close(clan::GUIComponent *component);
	static void on_resized(clan::GUIComponent *component);
};
