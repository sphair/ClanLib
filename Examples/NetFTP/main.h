#ifndef _main_h
#define _main_h

#include <ClanLib/application.h>

class ClanFTP : public CL_ClanApplication
{
public:
	CL_Socket *ftp;
	std::list<FTPD*> daemon_list;

	int main(int, char**);

	void conn();
} extern app;

#endif
