/*
	$Id: client.cpp,v 1.3 2003/04/22 06:25:02 plasmoid Exp $
	
	Program to test how well sockets work in ClanLib.
	
	Author: Mark Robinson Feb 2003
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/network.h>

#include <iostream>

class SockClient : public CL_ClanApplication
{
	CL_IPAddress *client_ip;
	CL_Socket *client_socket;
	
public:
	void print_help()
	{
		std::cout << "client [-6|-4]" << std::endl;
		std::cout << "-6 Connect with IPv6" << std::endl;
		std::cout << "-4 Connect with IPv4(default)" << std::endl;
	}

	int main(int argv, char** argc)
	{

		CL_SetupCore::init();
		CL_SetupNetwork::init();

		std::string message = (std::string)"ping\n";

		if(argv > 1)
		{
			if(strncmp(argc[1], "-6", 2) == 0)
			{
				client_ip = new CL_IPAddress("::1", "9000");
				client_socket = new CL_Socket(CL_Socket::tcp, CL_Socket::ipv6);
			}
			else if(strncmp(argc[1], "-4", 2) == 0)
			{
				client_ip = new CL_IPAddress("127.0.0.1", "9000");
				client_socket = new CL_Socket(CL_Socket::tcp, CL_Socket::ipv4);
			}
			else
			{
				print_help();
				return 0;
			}
		}
		else
		{
			client_ip = new CL_IPAddress("127.0.0.1", "9000");
			client_socket = new CL_Socket(CL_Socket::tcp, CL_Socket::ipv4);
		}

		client_socket->connect(*client_ip);
		client_socket->send(message);

		for(int i = 0; i < 5; i++)
		{
			CL_System::keep_alive();
			client_socket->send(message);
			CL_System::sleep(1000);
		}

		delete client_ip;
		delete client_socket;
		
		return 0;
	
	
	}


} app;
