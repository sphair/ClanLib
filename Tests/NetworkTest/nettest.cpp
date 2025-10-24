#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/network.h>

class CL_NetworkTest : public CL_ClanApplication
{

	CL_Socket *client;
	CL_IPAddress *ip;
	bool quit;

	int main(int argv, char**)
	{
		quit = false;

		CL_SetupCore::init();
		CL_SetupNetwork::init();
		client = new CL_Socket(CL_Socket::tcp);
		ip = new CL_IPAddress("www.clanlib.org", "80");
		std::string request_msg = "GET index.html HTTP/1.0\n\n";

		client->connect(*ip);
		client->send(request_msg);

		CL_Slot read_client = client->sig_read_triggered().connect(this, &CL_NetworkTest::client_read);
		CL_Slot remote_disc = client->sig_disconnected().connect(this, &CL_NetworkTest::remote_disconnect);

		while(!quit)
		{
			CL_System::keep_alive();
		}


		delete client;
		delete ip;

		return 0;
	}

	void client_read()
	{
		char buffer[64*1024+1];
		int received = client->recv(buffer, 64*1024);
		
		if(received == 0) 
		{
			std::cout << "No data in the buffer yet client_read() was called" << std::endl;
			return;
		}

		buffer[received] = 0;

		std::cout << buffer << std::endl;
	}
	
	void remote_disconnect()
	{
		std::cout << "The remote side has closed the socket" << std::endl;
		quit = true;
	}

} app;
