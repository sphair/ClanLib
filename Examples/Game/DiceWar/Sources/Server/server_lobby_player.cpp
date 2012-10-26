
#include "precomp.h"
#include "server_player.h"
#include "server_lobby_player.h"

ServerLobbyPlayer::ServerLobbyPlayer(ServerPlayer *player)
: player(player), current_game(0)
{
	player->lobby_player = this;
}

void ServerLobbyPlayer::send_event(const NetGameEvent &game_event)
{
	player->send_event(game_event);
}
