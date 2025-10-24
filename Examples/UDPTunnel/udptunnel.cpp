/*
	ClanLib UDP Tunnel example
	
	This program demonstrates how to use the CL_Socket and CL_IPAddress classes.
	It is also excellent to send UDP packets via a third host when playing UT. :)
*/

#include <ClanLib/core.h>
#include <ClanLib/network.h>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

std::map<CL_IPAddress, CL_Socket> forward_sockets;

int main(int argc, char **argv)
{
	std::cout << "ClanLib UDP Tunnel, copyright (c) 2001 Magnus Norddahl / Clansoft" << std::endl;
	std::cout << std::endl;
	
	if (argc < 4)
	{
		std::cout << "Syntax: " << argv[0] << " <listen port> <server ip> <server port>" << std::endl;
		std::cout << std::endl;
		return 255;
	}
	
	std::string listen_port = argv[1];
	std::string server_ip = argv[2];
	std::string server_port = argv[3];

	try
	{
		char data[64*1024];
	
#ifdef WIN32
		CL_SetupCore::set_instance(GetModuleHandle(NULL));
#endif
		CL_SetupCore setup_core;
		CL_SetupNetwork setup_network;
	
		CL_IPAddress dest(server_ip, server_port);

		CL_IPAddress listen_addr;
		listen_addr.set_port(listen_port);

		CL_Socket listen_sock(CL_Socket::udp);
		listen_sock.bind(listen_addr);

#ifdef WIN32 // we need more power on the warp engines for win32
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#endif

		std::cout << "Tunnel is ready." << std::endl;
		
		while (true)
		{
			fd_set rfds;
			FD_ZERO(&rfds);
			
			int highest_fd = listen_sock.get_socket();
			
			FD_SET(listen_sock.get_socket(), &rfds);
			
			std::map<CL_IPAddress, CL_Socket>::iterator it;
			for (it = forward_sockets.begin(); it != forward_sockets.end(); it++)
			{
				int sock = it->second.get_socket();
				if (sock > highest_fd) highest_fd = sock;
				
				FD_SET(sock, &rfds);
			}
			
			int result = select(highest_fd+1, &rfds, 0, 0, 0);
			if (result < 0) throw CL_Error("Select failed!");
			
			if (FD_ISSET(listen_sock.get_socket(), &rfds)) // incoming data from a client
			{
				try
				{
					// accept data:
					CL_IPAddress from;
					int data_size = listen_sock.recv(data, 64*1024, from);
			
					// first find out if we already know the client:
					it = forward_sockets.find(from);
					if (it == forward_sockets.end()) // new client, create new forward socket for it
					{
						std::cout << "New client connected!" << std::endl;
					
						CL_Socket new_sock(CL_Socket::udp);
						new_sock.bind(CL_IPAddress());
						forward_sockets[from] = new_sock;

						it = forward_sockets.find(from);
					}
				
					// forward the data:
					result = it->second.send(data, data_size, dest);
				}
				catch (CL_Error err)
				{
					std::cout << "Error while forwarding from client to server " << err.message.c_str() << std::endl;
				}
			}

			for (it = forward_sockets.begin(); it != forward_sockets.end(); it++)
			{
				const CL_IPAddress &src = it->first;
				CL_Socket &forward_sock = it->second;

				try
				{
					int sock = forward_sock.get_socket();
				
					if (FD_ISSET(sock, &rfds)) // incoming data from the server
					{
						// accept data:
						CL_IPAddress from;
						int data_size = forward_sock.recv(data, 64*1024, from);
					
						// forward to client:
						listen_sock.send(data, data_size, src);
					}
				}
				catch (CL_Error err)
				{
					std::cout << "Error while forwarding from server to client " << err.message.c_str() << std::endl;
				}
			}
		}
	}
	catch (CL_Error error)
	{
		std::cout << "Error: " << error.message.c_str() << std::endl;
	}
	
	return 0;
}
