#pragma once

#include <ClanLib/core.h>
#include <ClanLib/network.h>

class ServerUser;

class Server
{
public:
	Server();
	~Server();

	void exec();

private:
	void on_client_connected(CL_NetGameConnection *connection);
	void on_client_disconnected(CL_NetGameConnection *connection);

	void on_event_received(CL_NetGameConnection *connection, const CL_NetGameEvent &e);

	void on_event_login(const CL_NetGameEvent &e, ServerUser *user);
	void on_event_game_requeststart(const CL_NetGameEvent &e, ServerUser *user);

private:
	CL_NetGameServer network_server;
	CL_SlotContainer slots;

	CL_NetGameEventDispatcher_v1<ServerUser*> login_events;
	CL_NetGameEventDispatcher_v1<ServerUser*> game_events;

	int next_user_id;
	bool game_running;
};
