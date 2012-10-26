
#include "precomp.h"
#include "lobby_player_collection.h"
#include "lobby_player.h"

LobbyPlayerCollection::LobbyPlayerCollection()
{
}

LobbyPlayerCollection::~LobbyPlayerCollection()
{
}

LobbyPlayer *LobbyPlayerCollection::get_player(int player_id) const
{
	std::map<int, LobbyPlayer *>::const_iterator it = players.find(player_id);
	if (it != players.end())
		return it->second;
	else
		return 0;
}

LobbyPlayer *LobbyPlayerCollection::create_player(int player_id, std::string player_name)
{
	LobbyPlayer *player = new LobbyPlayer(player_name, player_id);
	players[player_id] = player;
	return player;
}

void LobbyPlayerCollection::remove_player(int player_id)
{
	LobbyPlayer *player = players[player_id];

	if (player)
	{
		players.erase(players.find(player_id));
		delete player;
	}
}
