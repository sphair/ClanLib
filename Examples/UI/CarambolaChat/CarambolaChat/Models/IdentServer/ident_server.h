
#pragma once

class IdentServerConnection;

class IdentServer
{
public:
	IdentServer();
	~IdentServer();

	void start();
	void stop();

private:
	void worker_main();

	std::thread thread;
	std::mutex mutex;
	clan::NetworkConditionVariable change_event;
	bool stop_flag = true;
};
