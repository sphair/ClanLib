
#pragma once

class AppModel
{
public:
	AppModel();
	~AppModel();

	static AppModel *instance();

	std::string name = "Mr. MVC";
	std::string description = "This is a MVC example with fields that make no sense. :)";

private:
	static AppModel *instance_ptr;
};
