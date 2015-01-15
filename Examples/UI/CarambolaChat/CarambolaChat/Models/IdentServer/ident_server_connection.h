
#pragma once

class IdentServer;

class IdentServerConnection
{
public:
	IdentServerConnection(IdentServer *server, clan::TCPConnection connection);
	~IdentServerConnection();

private:
	void worker_main();
	bool received_message(std::string message);

	IdentServer *server;
	clan::TCPConnection connection;
	clan::NetworkConditionVariable change_event;
	std::mutex mutex;
	bool stop_flag = false;
	std::thread thread;
};
