
#pragma once

class IdentServer;

class IdentServerConnection
{
public:
	IdentServerConnection(IdentServer *server, clan::TCPConnection connection);
	void join();

private:
	void worker_main();
	void received_message(std::string message);

	IdentServer *server;
	clan::TCPConnection connection;
	std::thread thread;
};
