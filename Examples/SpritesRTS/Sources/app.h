#ifndef _APP_H_
#define _APP_H_

#include <ClanLib/application.h>
#include <ClanLib/core.h>

class Application : public CL_ClanApplication
{
public:
	Application() {};

	virtual int main(int argc, char** argv);
};

extern Application app;

#endif
