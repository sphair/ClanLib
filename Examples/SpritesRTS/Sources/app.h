#ifndef _APP_H_
#define _APP_H_

#include <ClanLib/application.h>
#include <ClanLib/core.h>

class Application
{
public:
	Application() {};

	virtual int main(const std::vector<CL_String> &args);
};

#endif
