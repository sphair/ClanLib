
#pragma once

class ServerLobbyPlayerInformation
{
public:
	ServerLobbyPlayerInformation() {};
	virtual ~ServerLobbyPlayerInformation() {};

	virtual void append_network_game_info(clan::NetGameEvent &event) = 0;
	virtual void apply_network_game_info(const std::vector<clan::NetGameEventValue> &arguments) = 0;
};
