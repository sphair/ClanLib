
#pragma once

class Application
{
public:
	Application();
	~Application();

	int main(const std::vector<CL_String> &args);

	static Application *instance;
};
