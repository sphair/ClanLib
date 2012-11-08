
#pragma once

class ServerGamePlayerCollection;
class ServerGamePlayer;
class ServerPlayer;
class Server;

class ServerGame
{
public:
	ServerGame(Server *server, int id);
	~ServerGame();

	void add_player(ServerPlayer *player);
	void remove_player(ServerGamePlayer *player);

	void set_map_name(CL_String map_name) { this->map_name = map_name; }

	bool start();

	int get_id() const { return id; }
	
private:
	Server *server;

	std::auto_ptr<ServerGamePlayerCollection> players;

	ServerGamePlayer *active_player;

	std::vector<ServerPlayer *> players_ai;

	CL_String map_name;

	int id;

	int max_turns;
	int current_turn;
};
