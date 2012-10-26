
#include "precomp.h"
#include "lobby_view_create_game.h"
#include "lobby_view.h"
#include "lobby_game.h"
#include "lobby_player.h"
#include "client.h"

LobbyViewCreateGame::LobbyViewCreateGame(LobbyView *lobby_view, const Rect &geometry, Client *client)
: Frame(lobby_view), active_game(0), client(client)
{
	set_geometry(geometry);

	label_players_info = new Label(this);
	label_players_info->set_geometry(RectPS(24, 8, 200, 32));
	label_players_info->set_text("Players:");

	label_players = new Label(this);
	label_players->set_geometry(RectPS(40, 45, 200, 32));

	button_start_game = new PushButton(this);
	button_start_game->set_geometry(RectPS(29, 310, 200, 41));
	button_start_game->set_text("Start game");
	button_start_game->func_clicked().set(this, &LobbyViewCreateGame::on_start_game_clicked);

	button_leave_game = new PushButton(this);
	button_leave_game->set_geometry(RectPS(250, 310, 200, 41));
	button_leave_game->set_text("Leave game");
	button_leave_game->func_clicked().set(this, &LobbyViewCreateGame::on_leave_game_clicked);

	menu_maps = new PopupMenu();
	menu_maps->insert_item("Island");
	menu_maps->insert_item("Map1");

	combo_maps = new ComboBox(this);
	combo_maps->set_geometry(RectPS(29, 100, 200, 41));
	combo_maps->set_editable(false);
	combo_maps->set_popup_menu(*menu_maps);
}	

LobbyViewCreateGame::~LobbyViewCreateGame()
{
}

void LobbyViewCreateGame::on_leave_game_clicked()
{
	client->request_leave_game();
}

void LobbyViewCreateGame::on_start_game_clicked()
{
	client->request_start_game();
}

void LobbyViewCreateGame::set_active_game(LobbyGame *game)
{
	active_game = game;
}

void LobbyViewCreateGame::update_view(LobbyGame *game)
{
	if(game == active_game)
	{
		std::string player_list;
		const std::vector<LobbyPlayer *> players = game->get_players();
		std::vector<LobbyPlayer *>::const_iterator it;
		for(it = players.begin(); it != players.end(); ++it)
		{
			if(it != players.begin())
				player_list += ", ";
			player_list += (*it)->name;
		}

		label_players->set_text(player_list);
	}
}

void LobbyViewCreateGame::set_visible(bool visible)
{
	if(visible == false)
		active_game = 0;

	Frame::set_visible(visible);
}
