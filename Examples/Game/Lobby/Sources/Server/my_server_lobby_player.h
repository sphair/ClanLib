#pragma once

#include "../Lobby/server_lobby_player_information.h"

class MyServerLobbyPlayerInformation : public ServerLobbyPlayerInformation
{
public:
	MyServerLobbyPlayerInformation() 
	{
	}

	CL_String get_name() const { return name; }
	void set_name(CL_String name) { this->name = name; }

	void append_network_game_info(CL_NetGameEvent &event)
	{
		event.add_argument(name);
	}

	void apply_network_game_info(const std::vector<CL_NetGameEventValue> &arguments)
	{
		if(arguments.size() != 1)
			throw CL_Exception("Invalid number of arguments");

		name = arguments[0];
	}

private:
	CL_String name;
};
