
#pragma once

class Server;
class ServerPlayer;

class ServerLoginEvents
{
public:
	ServerLoginEvents(Server *server);
	~ServerLoginEvents();

	NetGameEventDispatcher_v1<ServerPlayer *> &get_dispatcher() { return login_events; }

private:
	void on_event_login(const NetGameEvent &e, ServerPlayer *player);

	Server *server;

	NetGameEventDispatcher_v1<ServerPlayer *> login_events;
};
