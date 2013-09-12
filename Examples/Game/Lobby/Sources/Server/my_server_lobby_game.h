#pragma once

#include "../Lobby/server_lobby_game_information.h"

class MyServerLobbyGameInformation : public ServerLobbyGameInformation
{
public:
	MyServerLobbyGameInformation() 
	{
		set_max_players(6);
		set_map_name("Island");
	}

	std::string get_name() const { return game_name; }
	void set_name(const std::string &name) { this->game_name = name; }

	void set_max_players(int max_players)
	{
		if(max_players < 1 || max_players > 6)
			throw clan::Exception("Invalid number of max players");

		this->max_players = max_players;
	}

	std::string get_map_name() const { return map_name; }

	void set_map_name(const std::string &map_name)
	{ 
		if(map_name != "Island" || map_name != "Map1")
			throw clan::Exception("Invalid map name");

		this->map_name = map_name;
	}

	void append_network_game_info(clan::NetGameEvent &event)
	{
		event.add_argument(game_name);
		event.add_argument(map_name);
		event.add_argument(max_players);
	}

	void apply_network_game_info(const std::vector<clan::NetGameEventValue> &arguments)
	{
		if(arguments.size() != 3)
			throw clan::Exception("Invalid number of arguments");

		std::string game_name = arguments[0];
		std::string map_name = arguments[1];
		int max_players = arguments[2];

		set_name(game_name);
		set_map_name(map_name);
		set_max_players(max_players);
	}

private:
	std::string game_name;
	std::string map_name;
	int max_players;
};