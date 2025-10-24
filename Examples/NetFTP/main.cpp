#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/network.h>

#include <iostream>
#include <list>

#include "ftp.h"
#include "main.h"

char welcome[] = {'2','2','0','h','i','\r','\n'};

std::list<FTPD*> kill_daemon;
bool server_kill;

ClanFTP app;

int ClanFTP::main(int, char**)
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupNetwork setup_network;
		
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		CL_IPAddress ip;
		ip.set_port((std::string)"2021");

		ftp = new CL_Socket(CL_Socket::tcp);

		ftp->bind(ip);
		ftp->sig_read_triggered().connect(this,&ClanFTP::conn);
		ftp->sig_write_triggered().connect(this,&ClanFTP::conn);
		ftp->sig_exception_triggered().connect(this,&ClanFTP::conn);
		ftp->listen(1);

		std::cout << "FTP server running." << std::endl;

		while(1)
		{
			CL_System::sleep(10);
			CL_System::keep_alive();
		}
	}
	catch(CL_Error err) 
	{
		std::cout << err.message << std::endl;
	}
	catch(...)
	{
		std::cout << "A non-ClanLib based error occured.  Please provide clanlib-devel@clanlib.org with a back-trace/stack-trace." << std::endl;
	}

	return 0;
}

void ClanFTP::conn()
{
	CL_Socket accept = ftp->accept();
	accept.send(welcome,sizeof(welcome));
	FTPD *temp = new FTPD(accept);

	daemon_list.push_back(temp);

//	CL_IPAddress ip;
//	ip.set_port(2021);

//	ftp = new CL_Socket(CL_Socket::tcp);

//	ftp->bind(ip);
//	ftp->sig_read_triggered().connect(this,&ClanFTP::conn);
//	ftp->sig_write_triggered().connect(this,&ClanFTP::conn);
//	ftp->sig_exception_triggered().connect(this,&ClanFTP::conn);
//	ftp->listen(4);
}
