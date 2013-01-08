
#include "precomp.h"
#include "game_view.h"
#include "game_model.h"
#include "game_player.h"
#include "game_view_playerlist.h"
#include "game_player_collection.h"
#include "client.h"

GameViewPlayerList::GameViewPlayerList(GameView *view, Client *client)
: GUIComponent(view, "playerlist"), client(client)
{
	set_geometry(Rect(0, 0, 200, 200));

	func_render().set(this, &GameViewPlayerList::on_render);
}	

GameViewPlayerList::~GameViewPlayerList()
{
}

void GameViewPlayerList::on_render(Canvas &canvas, const Rect &clip_rect)
{
	Rect rect = get_geometry();

	GameModel *model = client->get_game_model();
	GamePlayerCollection *players = model->get_players();

	int current_y = 0;
	std::vector<GamePlayer *>::iterator it;
	for(it = players->players.begin(); it != players->players.end(); ++it)
	{
		GamePlayer *player = (*it);
		render_text(canvas, player->name, rect.left, current_y + 20);
		current_y += 20;
	}
}
