
#pragma once

#include "server_game_player.h"

class Server;
class ServerGame;
class ServerPlayer;

class ServerGamePlayerAI : public ServerGamePlayer
{
public:
	ServerGamePlayerAI(Server *server, ServerGame *game, ServerPlayer *player);

	virtual void send_event(const NetGameEvent &game_event);

private:
	NetGameEventDispatcher_v0 game_events;

	void on_event_attacked_area(const NetGameEvent &e);
	void on_event_attack_done(const NetGameEvent &e);
	void on_event_player_turn_started(const NetGameEvent &e);
	void on_event_invalid_attack(const NetGameEvent &e);

	void perform_next_attack();

	Server *server;
	ServerGame *game;
};
