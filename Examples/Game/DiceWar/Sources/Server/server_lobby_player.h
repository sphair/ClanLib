
#pragma once

class ServerLobbyGame;
class ServerPlayer;

class ServerLobbyPlayer
{
public:
	ServerLobbyPlayer(ServerPlayer *player);

	virtual void send_event(const NetGameEvent &game_event);

public:
	ServerLobbyGame *current_game;

	ServerPlayer *player;
};
