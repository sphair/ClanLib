
#pragma once

class Application
{
//! Construction:
public:
	Application();
	~Application();

	int main(const std::vector<CL_String> &args);

//! Attributes:
public:
	static Application *instance;

//! Operations:
public:

//! Implementation:
private:

};
