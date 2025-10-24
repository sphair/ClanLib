/*
	$Id: server.cpp,v 1.4 2004/02/10 17:01:45 sphair Exp $
	
	Program to test how well sockets work in ClanLib.
	
	Author: Mark Robinson Feb 2003
*/
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/network.h>

class ClientConnection : public CL_Runnable
{
	CL_Socket conn;
	CL_Thread *th;
	
public:
	ClientConnection(CL_Socket sock)
	{	
		conn = sock;
		conn.sig_disconnected().connect(this, &ClientConnection::sock_close);
		th = new CL_Thread(this);
		th->start();
	}
	
	virtual ~ClientConnection()
	{
		// I guess we can stop our thread now
		th->terminate();
	}
	
	void sock_close()
	{
		std::cout << "Client has closed the socket" << std::endl;
		delete this;
		//th->terminate();
	}
	
	void run()
	{
		char *big_buf = new char[1024+1];
		std::cout << "Write on Socket" << std::endl;
		while(1)
		{
			conn.recv(big_buf, 1024);
			std::cout << big_buf << std::endl;
			memset(big_buf, 0, 1024+1);
			CL_System::keep_alive();
		}
		delete[] big_buf;
		CL_System::keep_alive();
	}

};

class SocketTest : public CL_ClanApplication
{

	CL_IPAddress *server_ip;
	CL_Socket *server_socket;
	CL_Socket connection;
	bool has_connected;

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
		
		has_connected = false;

		if(argv > 1)
		{
			if(strncmp(argc[1], "-6", 2) == 0)
			{
				server_ip = new CL_IPAddress("9000");
				server_socket = new CL_Socket(CL_Socket::tcp, CL_Socket::ipv6);
			}
			else if(strncmp(argc[1], "-4", 2) == 0)
			{
				server_ip = new CL_IPAddress("9000");
				server_socket = new CL_Socket(CL_Socket::tcp, CL_Socket::ipv4);
			}
			else
			{
				print_help();
				return 0;
			}
		}
		else
		{
			server_ip = new CL_IPAddress("9000");
			server_socket = new CL_Socket(CL_Socket::tcp, CL_Socket::ipv4);
		}

		server_socket->sig_read_triggered().connect(this, &SocketTest::sock_read);
		server_socket->sig_write_triggered().connect(this, &SocketTest::sock_write);
		server_socket->sig_exception_triggered().connect(this, &SocketTest::something);
		server_socket->sig_disconnected().connect(this, &SocketTest::something);

		try
		{
			server_socket->bind(*server_ip);
		}
		catch(CL_Error e)
		{
			std:: cout << e.message << std::endl;
			return 0;
		}
		server_socket->listen(1);

		while(1)
		{
			new ClientConnection(server_socket->accept());
			CL_System::keep_alive();
		}
		
		
		delete server_socket;
		delete server_ip;
		return 0;
	
	}
	
	void sock_read()
	{
		std::cout << "Read on Socket" << std::endl;
	}
	
	void sock_write()
	{
		std::cout << "Write on Socket" << std::endl;
	}
	
	void something()
	{
		std::cout << "Something Happened" << std::endl;
	}
	
	void doStuff()
	{
		CL_System::keep_alive();
	}


} app;
