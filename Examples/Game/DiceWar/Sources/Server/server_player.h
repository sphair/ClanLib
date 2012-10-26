
#pragma once

class ServerLobbyPlayer;
class ServerGamePlayer;
class NetGameConnection;
class NetGameEvent;

class ServerPlayer
{
public:
	ServerPlayer(NetGameConnection *connection);

	static ServerPlayer *get_player(NetGameConnection *connection);

	void send_event(const NetGameEvent &game_event);

public:
	ServerLobbyPlayer *lobby_player;
	ServerGamePlayer *game_player;

	std::string name;
	int id;

private:
	NetGameConnection *connection;
};
