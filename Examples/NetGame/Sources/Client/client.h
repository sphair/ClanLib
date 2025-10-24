#pragma once

#include <ClanLib/core.h>
#include <ClanLib/network.h>

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

	void on_event_received(const CL_NetGameEvent &e);

	void on_event_login_success(const CL_NetGameEvent &e);
	void on_event_login_fail(const CL_NetGameEvent &e);
	void on_event_game_loadmap(const CL_NetGameEvent &e);
	void on_event_game_startgame(const CL_NetGameEvent &e);

private:
	CL_NetGameClient network_client;
	CL_SlotContainer slots;

	CL_NetGameEventDispatcher_v0 login_events;
	CL_NetGameEventDispatcher_v0 game_events;

	bool quit;

	bool logged_in;
};
