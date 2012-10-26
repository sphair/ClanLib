
#pragma once

#include "server_lobby_game_player_collection.h"

class ServerLobbyPlayer;
class ServerLobbyGameInformation;

class ServerLobbyGame
{
public:
	enum State
	{
		lobby,
		playing
	};

public:
	ServerLobbyGame(CL_NetGameServer *server, ServerLobbyPlayer *owner_player, int game_id, ServerLobbyGameInformation *extra_information = 0);
	virtual ~ServerLobbyGame() {};

	void add_player(ServerLobbyPlayer *lobby_player);
	void remove_player(ServerLobbyPlayer *lobby_player);
	void send_available_players(ServerLobbyPlayer *destination_player);
	bool has_players() const;

	void modify(const std::vector<CL_NetGameEventValue> &extra_arguments);

	ServerLobbyPlayer *get_owner() const;

	State get_state() { return state; }
	void set_state(State state);

	int get_id() const { return id; }

	void send_game_info_to_all();
	void send_game_info(ServerLobbyPlayer *destination_player);

private:
	CL_NetGameServer *server;

	ServerLobbyGameInformation *extra_information;

	State state;

	std::auto_ptr<ServerLobbyGamePlayerCollection> player_collection;

	int id;
};
