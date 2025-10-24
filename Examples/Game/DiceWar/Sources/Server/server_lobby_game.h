
#pragma once

class Server;
class ServerLobbyGamePlayerCollection;
class ServerLobbyPlayer;

class ServerLobbyGame
{
public:
	enum State
	{
		lobby,
		playing
	};

public:
	ServerLobbyGame(Server *server, ServerLobbyPlayer *owner, int id);
	~ServerLobbyGame();

	ServerLobbyGamePlayerCollection *get_player_collection() { return player_collection.get(); }

	State get_state() { return state; }
	void set_state(State state);

	int get_id() const { return id; }

	CL_String get_name() const { return name; }
	void set_name(CL_String name) { this->name = name; }

	int get_max_players() const { return max_players; }
	void set_max_players(int max_players);

	CL_String get_map_name() const { return map_name; }
	void set_map_name(CL_String map_name) { this->map_name = map_name; }

	void send_game_info();
	void send_game_info(ServerLobbyPlayer *destination_player);

private:
	Server *server;

	State state;

	std::auto_ptr<ServerLobbyGamePlayerCollection> player_collection;

	int id;
	CL_String name;
	CL_String map_name;
	int max_players;
};
