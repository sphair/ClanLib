
#include "precomp.h"
#include "server_player.h"
#include "server_game_player.h"

ServerGamePlayer::ServerGamePlayer(ServerPlayer *player)
: player(player), current_game(0)
{
	in_battle_mode = false;
}

void ServerGamePlayer::send_event(const CL_NetGameEvent &game_event)
{
	player->send_event(game_event);
}
