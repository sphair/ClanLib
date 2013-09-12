#pragma once

#include "../Lobby/server_lobby_player_information.h"

class MyServerLobbyPlayerInformation : public ServerLobbyPlayerInformation
{
public:
	MyServerLobbyPlayerInformation() 
	{
	}

	std::string get_name() const { return name; }
	void set_name(std::string name) { this->name = name; }

	void append_network_game_info(clan::NetGameEvent &event)
	{
		event.add_argument(name);
	}

	void apply_network_game_info(const std::vector<clan::NetGameEventValue> &arguments)
	{
		if(arguments.size() != 1)
			throw clan::Exception("Invalid number of arguments");

		name = arguments[0];
	}

private:
	std::string name;
};
