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

	CL_String get_name() const { return game_name; }
	void set_name(const CL_String &name) { this->game_name = name; }

	void set_max_players(int max_players)
	{
		if(max_players < 1 || max_players > 6)
			throw CL_Exception("Invalid number of max players");

		this->max_players = max_players;
	}

	CL_String get_map_name() const { return map_name; }

	void set_map_name(const CL_String &map_name)
	{ 
		if(map_name != "Island" || map_name != "Map1")
			throw CL_Exception("Invalid map name");

		this->map_name = map_name;
	}

	void append_network_game_info(CL_NetGameEvent &event)
	{
		event.add_argument(game_name);
		event.add_argument(map_name);
		event.add_argument(max_players);
	}

	void apply_network_game_info(const std::vector<CL_NetGameEventValue> &arguments)
	{
		if(arguments.size() != 3)
			throw CL_Exception("Invalid number of arguments");

		CL_String game_name = arguments[0];
		CL_String map_name = arguments[1];
		int max_players = arguments[2];

		set_name(game_name);
		set_map_name(map_name);
		set_max_players(max_players);
	}

private:
	CL_String game_name;
	CL_String map_name;
	int max_players;
};