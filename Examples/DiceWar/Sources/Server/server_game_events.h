
#pragma once

class ServerGame;
class ServerGamePlayer;

class ServerGameEvents
{
public:
	ServerGameEvents(Server *server);
	~ServerGameEvents();

	CL_NetGameEventDispatcher_v2<ServerGame *, ServerGamePlayer *> &get_dispatcher() { return game_events; }

private:
	void on_event_attack_area(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);
	void on_event_end_turn(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);
	void on_event_battle_view_over(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);
	void on_event_game_add_message(const CL_NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);

	CL_NetGameEventDispatcher_v2<ServerGame *, ServerGamePlayer *> game_events;

	Server *server;
};
