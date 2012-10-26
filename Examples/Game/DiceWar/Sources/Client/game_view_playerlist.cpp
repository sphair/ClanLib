
#include "precomp.h"
#include "game_view.h"
#include "game_model.h"
#include "game_player.h"
#include "game_view_playerlist.h"
#include "game_player_collection.h"
#include "client.h"

GameViewPlayerList::GameViewPlayerList(GameView *view, Client *client)
: GUIComponent(view), client(client)
{
	set_geometry(Rect(0, 0, 200, 200));
	set_type_name("playerlist");

	func_render().set(this, &GameViewPlayerList::on_render);
}	

GameViewPlayerList::~GameViewPlayerList()
{
}

void GameViewPlayerList::on_render(GraphicContext &gc, const Rect &clip_rect)
{
	Rect rect = get_geometry();

	GUIThemePart part(this);
	part.render_box(gc, rect, clip_rect);

	GameModel *model = client->get_game_model();
	GamePlayerCollection *players = model->get_players();

	int current_y = 0;
	std::vector<GamePlayer *>::iterator it;
	for(it = players->players.begin(); it != players->players.end(); ++it)
	{
		Rect text_rect = Rect(rect.left, current_y, rect.right, current_y + 20);
		GamePlayer *player = (*it);
		part.render_text(gc, player->name, text_rect, clip_rect);
		current_y += 20;
	}
}
