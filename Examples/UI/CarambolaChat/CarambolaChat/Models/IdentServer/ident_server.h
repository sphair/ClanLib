
#pragma once

class IdentServerConnection;

class IdentServer
{
public:
	IdentServer();
	~IdentServer();

	void start();
	void stop();

	clan::Event &get_stop_event() { return stop_event; }

private:
	void worker_main();

	std::thread thread;
	clan::Event stop_event;
};
