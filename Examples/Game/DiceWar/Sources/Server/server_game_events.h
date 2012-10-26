
#pragma once

class ServerGame;
class ServerGamePlayer;

class ServerGameEvents
{
public:
	ServerGameEvents(Server *server);
	~ServerGameEvents();

	NetGameEventDispatcher_v2<ServerGame *, ServerGamePlayer *> &get_dispatcher() { return game_events; }

private:
	void on_event_attack_area(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);
	void on_event_end_turn(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);
	void on_event_battle_view_over(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);
	void on_event_game_add_message(const NetGameEvent &e, ServerGame *game, ServerGamePlayer *player);

	NetGameEventDispatcher_v2<ServerGame *, ServerGamePlayer *> game_events;

	Server *server;
};
