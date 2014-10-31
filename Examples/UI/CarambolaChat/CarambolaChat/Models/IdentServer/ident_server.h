
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

	clan::Thread thread;
	clan::Event stop_event;
};
