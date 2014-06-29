#pragma once

#include <ClanLib/core.h>
#include <ClanLib/network.h>
using namespace clan;

class Client
{
public:
	Client();
	~Client();

	void exec();

private:
	void connect_to_server();

	void on_connected();
	void on_disconnected();

	void on_event_received(const NetGameEvent &e);

	void on_event_login_success(const NetGameEvent &e);
	void on_event_login_fail(const NetGameEvent &e);
	void on_event_game_loadmap(const NetGameEvent &e);
	void on_event_game_startgame(const NetGameEvent &e);

private:
	NetGameClient network_client;
	CallbackContainer cc;

	NetGameEventDispatcher<> login_events;
	NetGameEventDispatcher<> game_events;

	bool quit;

	bool logged_in;
};
