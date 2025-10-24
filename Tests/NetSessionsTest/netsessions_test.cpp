#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/network.h>
#include <ClanLib/application.h>

#include <iostream>

class NetChannelDemo : public CL_ClanApplication
{
public:

	NetChannelDemo() {}
	~NetChannelDemo() {}

	virtual int main(int argc, char **argv)
	{
		CL_ConsoleWindow console("NetChannelDemo", 80, 200);
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupNetwork::init();

			connect_test();
			packet_test();
			stream_test();
			stress_test();
		}
		catch (CL_Error error)
		{
			std::cout << "Error: " << error.message.c_str() << std::endl;
		}

		console.display_close_message();
		return 0;
	}

	void connect_test()
	{
		std::cout << "Running connect/disconnect test.." << std::endl;

		std::cout << "Creating host.." << std::endl;
		host = new CL_NetSession("Hello");
		CL_Slot slot_connect = host->sig_computer_connected().connect(this, &NetChannelDemo::on_connect);
		CL_Slot slot_disconnect = host->sig_computer_disconnected().connect(this, &NetChannelDemo::on_disconnect);

		std::cout << "Starting listen on port 9999" << std::endl;
		host->start_listen("9999");
		std::cout << std::endl;

		// Try check if connect/disconnect works in the simplest case:
		std::cout << "Creating client.." << std::endl;
		client = new CL_NetSession("Hello");

		std::cout << "Connecting client.." << std::endl;
		client->connect(CL_IPAddress("localhost", "9999"));

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait a little to see if host notices the connect.

		std::cout << "Deleting client.." << std::endl;
		delete client;

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait a little to see if host notices the disconnect.

		std::cout << std::endl;

		// Try disconnect server from client test:
		std::cout << "Creating client.." << std::endl;
		client = new CL_NetSession("Hello");

		std::cout << "Connecting client async.." << std::endl;
		client->connect_async("localhost", "9999");

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait for connect.

		std::cout << "Disconnecting client.." << std::endl;
		client->get_all().front().disconnect();

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait for host to notice disconnect.

		std::cout << "Deleting client.." << std::endl;
		delete client;

		std::cout << "Entering 1 second loop (No disconnect should happen here)" << std::endl;
		CL_System::keep_alive(1000); // No disconnect should happen here.

		std::cout << std::endl;

		// Try disconnect client from server test:

		std::cout << "Creating client.." << std::endl;
		client = new CL_NetSession("Hello");
		CL_Slot slot_connect_client = client->sig_computer_connected().connect(this, &NetChannelDemo::on_connect_client);
		CL_Slot slot_disconnect_client = client->sig_computer_disconnected().connect(this, &NetChannelDemo::on_disconnect_client);

		std::cout << "Connecting client.." << std::endl;
		client->connect(CL_IPAddress("localhost", "9999"));

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait for connect.

		std::cout << "Disconnecting client from host.." << std::endl;
		host->get_all().front().disconnect();

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait for host to notice disconnect.

		std::cout << "Deleting client.." << std::endl;
		delete client;

		std::cout << "Entering 1 second loop (No disconnect should happen here)" << std::endl;
		CL_System::keep_alive(1000); // No disconnect should happen here.

		std::cout << std::endl;

		// Try connect client after we stopped listening:

		std::cout << "Stopping listen.." << std::endl;
		host->stop_listen();
		CL_System::keep_alive(1000);

		std::cout << "Creating client.." << std::endl;
		client = new CL_NetSession("Hello");

		std::cout << "Connecting client.." << std::endl;
		try
		{
			client->connect(CL_IPAddress("localhost", "9999"));
		}
		catch (CL_Error error)
		{
			std::cout << "Connect failed with error: " << error.message.c_str() << std::endl;
		}

		std::cout << "Deleting client.." << std::endl;
		delete client;

		std::cout << "Starting listen.." << std::endl;
		host->start_listen("9999");
		CL_System::keep_alive(1000);

		std::cout << std::endl;

		// Try delete server while a client is connected:

		std::cout << "Creating client.." << std::endl;
		client = new CL_NetSession("Hello");
		slot_connect_client = client->sig_computer_connected().connect(this, &NetChannelDemo::on_connect_client);
		slot_disconnect_client = client->sig_computer_disconnected().connect(this, &NetChannelDemo::on_disconnect_client);

		std::cout << "Connecting client.." << std::endl;
		client->connect(CL_IPAddress("localhost", "9999"));

		std::cout << "Entering 1 second loop.." << std::endl;
		CL_System::keep_alive(1000); // Wait for connect.

		std::cout << host->get_all().size() << " computers connected to host." << std::endl;

		std::cout << "Deleting host.." << std::endl;
		delete host;

		std::cout << "Entering 1 second loop" << std::endl;
		CL_System::keep_alive(1000);

		std::cout << "Deleting client.." << std::endl;
		delete client;

		std::cout << "End of connect/disconnect test." << std::endl;
	}

	void packet_test()
	{
	}

	void stream_test()
	{
	}

	void stress_test()
	{
	}

	void on_connect(CL_NetComputer &computer)
	{
		std::cout << "SERVER: Connect from " << computer.get_address().get_address() << std::endl;
	}

	void on_disconnect(CL_NetComputer &computer)
	{
		std::cout << "SERVER: Disconnect from " << computer.get_address().get_address() << std::endl;
	}

	void on_connect_client(CL_NetComputer &computer)
	{
		std::cout << "CLIENT: Connect from " << computer.get_address().get_address() << std::endl;
	}

	void on_disconnect_client(CL_NetComputer &computer)
	{
		std::cout << "CLIENT: Disconnect from " << computer.get_address().get_address() << std::endl;
	}

private:
	CL_NetSession *client;
	CL_NetSession *host;
} app;
