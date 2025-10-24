
#pragma once

class Server;
class ServerGame;
class ServerGamePlayer;

class ServerGamePlayerCollection
{
public:
	ServerGamePlayerCollection(Server *server, ServerGame *game);
	~ServerGamePlayerCollection();

	void add_player(ServerGamePlayer *player);
	void remove_player(ServerGamePlayer *player);
	bool contains_player(ServerGamePlayer *player);
	ServerGamePlayer *get_player(int player_id);

	std::vector<ServerGamePlayer *> &get_players() { return players; }

	int get_count() const { return players.size(); }

	void transfer_players();

	void send_event(const CL_NetGameEvent &game_event);

private:
	Server *server;
	ServerGame *game;
	std::vector<ServerGamePlayer *> players;

	int next_visual_id;
};
