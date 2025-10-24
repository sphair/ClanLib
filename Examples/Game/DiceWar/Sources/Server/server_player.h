
#pragma once

class ServerLobbyPlayer;
class ServerGamePlayer;
class CL_NetGameConnection;
class CL_NetGameEvent;

class ServerPlayer
{
public:
	ServerPlayer(CL_NetGameConnection *connection);

	static ServerPlayer *get_player(CL_NetGameConnection *connection);

	void send_event(const CL_NetGameEvent &game_event);

public:
	ServerLobbyPlayer *lobby_player;
	ServerGamePlayer *game_player;

	CL_String name;
	int id;

private:
	CL_NetGameConnection *connection;
};
