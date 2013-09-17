#pragma once

#include <ClanLib/core.h>
#include <ClanLib/network.h>
using namespace clan;

class ServerUser;

class Server
{
public:
	Server();
	~Server();

	void exec();

private:
	void on_client_connected(NetGameConnection *connection);
	void on_client_disconnected(NetGameConnection *connection, const std::string &message);

	void on_event_received(NetGameConnection *connection, const NetGameEvent &e);

	void on_event_login(const NetGameEvent &e, ServerUser *user);
	void on_event_game_requeststart(const NetGameEvent &e, ServerUser *user);

private:
	NetGameServer network_server;
	SlotContainer slots;

	NetGameEventDispatcher_v1<ServerUser*> login_events;
	NetGameEventDispatcher_v1<ServerUser*> game_events;

	int next_user_id;
	bool game_running;
};
