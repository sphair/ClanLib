
#pragma once

class ServerGame;
class ServerPlayer;
class NetGameEvent;

class ServerGamePlayer
{
public:
	ServerGamePlayer(ServerPlayer *player);

	virtual void send_event(const NetGameEvent &game_event);

public:
	ServerGame *current_game;

	ServerPlayer *player;

	int visual_id;

	bool in_battle_mode;
};
