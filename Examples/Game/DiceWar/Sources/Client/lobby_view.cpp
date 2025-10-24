
#include "precomp.h"
#include "lobby_view.h"
#include "client.h"
#include "lobby_player.h"
#include "lobby_game.h"
#include "lobby_view_create_game.h"

LobbyView::LobbyView(Client *client)
: CL_GUIComponent(client->get_gui(), get_toplevel_description()), client(client)
{
	func_render().set(this, &LobbyView::on_render);
	func_process_message().set(this, &LobbyView::on_gui_message);

	sprite_background = CL_Sprite(get_gc(), "Resources/lobby_background1.png");
	
	create_frame_games();
	create_frame_create_game();
	create_frame_players();
	create_frame_chat();
	create_frame_toolbar();
}	

LobbyView::~LobbyView()
{
}

CL_GUITopLevelDescription LobbyView::get_toplevel_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("DiceWar Lobby");
	desc.set_position(CL_Rect(0, 0, 1024, 768), false);
	return desc;
}

void LobbyView::create_frame_create_game()
{
	view_create_game = new LobbyViewCreateGame(this, CL_Rect(30, 110, 740, 483), client);
	view_create_game->set_visible(false);
}

void LobbyView::create_frame_games()
{
	frame_games = new CL_Frame(this);
	frame_games->set_geometry(CL_Rect(30, 110, 740, 483));

	listview_games = new CL_ListView(frame_games);
	listview_games->set_geometry(CL_Rect(24, 8, 685, 290));
	CL_ListViewColumnHeader game_name_header = listview_games->get_header()->create_column("name", "Game name");
	game_name_header.set_width(250);
	CL_ListViewColumnHeader game_players_header = listview_games->get_header()->create_column("players", "Players");
	game_players_header.set_width(70);
	CL_ListViewColumnHeader game_map_header = listview_games->get_header()->create_column("map", "Map");
	game_map_header.set_width(180);
	CL_ListViewColumnHeader game_status_header = listview_games->get_header()->create_column("status", "Status");
	game_status_header.set_width(160);
	listview_games->get_header()->append(game_name_header);
	listview_games->get_header()->append(game_players_header);
	listview_games->get_header()->append(game_map_header);
	listview_games->get_header()->append(game_status_header);
	listview_games->func_selection_changed().set(this, &LobbyView::on_listview_games_selection_changed);

	button_create_game = new CL_PushButton(frame_games);
	button_create_game->set_geometry(CL_RectPS(29, 310, 200, 41));
	button_create_game->set_text("Create game");
	button_create_game->func_clicked().set(this, &LobbyView::on_create_game_clicked);

	button_join_game = new CL_PushButton(frame_games);
	button_join_game->set_geometry(CL_RectPS(250, 310, 200, 41));
	button_join_game->set_text("Join game");
	button_join_game->set_enabled(false);
	button_join_game->func_clicked().set(this, &LobbyView::on_join_game_clicked);
}

void LobbyView::create_frame_players()
{
	frame_players = new CL_Frame(this);
	frame_players->set_geometry(CL_Rect(766, 110, 1000, 483));

	listview_players = new CL_ListView(frame_players);
	listview_players->set_geometry(CL_Rect(24, 8, 205, 350));
	CL_ListViewColumnHeader player_name_header = listview_players->get_header()->create_column("name", "Connected players");
	listview_players->get_header()->append(player_name_header);
	listview_players->set_select_whole_row(true);
}

void LobbyView::create_frame_chat()
{
	frame_chat_text = new CL_Frame(this);
	frame_chat_text->set_geometry(CL_Rect(30, 505, 1000, 700));

	listview_chat = new CL_ListView(frame_chat_text);
	listview_chat->set_geometry(CL_Rect(24, 8, 955, 175));
	
	CL_ListViewColumnHeader chat_message_header = listview_chat->get_header()->create_column("message", "Message");
	listview_chat->get_header()->append(chat_message_header);
//	listview_chat->set_display_mode(listview_mode_list);
//	chat_message_header.set_visible(false);

	lineedit_chat = new CL_LineEdit(this);
	lineedit_chat->set_geometry(CL_Rect(30, 700, 1000, 750));
	lineedit_chat->func_enter_pressed().set(this, &LobbyView::on_lineedit_chat_enter_pressed);
}

void LobbyView::create_frame_toolbar()
{
	button_quit = new CL_PushButton(this);
	button_quit->set_geometry(CL_RectPS(955, 52, 45, 20));
	button_quit->set_class_name("textonly");
	button_quit->set_text("Quit");
	button_quit->func_clicked().set(this, &LobbyView::on_quit_clicked);
}

void LobbyView::on_gui_message(CL_GUIMessage &message)
{
	if (message.is_type(CL_GUIMessage_Close::get_type_name()))
		exit_with_code(0);
}

void LobbyView::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	sprite_background.draw(gc, CL_Rect(0, 0, get_width(), get_height()));
}

void LobbyView::on_quit_clicked()
{
	exit_with_code(0);
}

void LobbyView::on_create_game_clicked()
{
	client->request_create_game();
}

void LobbyView::on_join_game_clicked()
{
	CL_ListViewItem item = listview_games->get_selected_item();
	if (!item.is_null())
	{
		client->request_join_game(item.get_id());
	}
}

void LobbyView::on_listview_games_selection_changed(CL_ListViewSelection selection)
{
	CL_ListViewItem item = listview_games->get_selected_item();
	if (!item.is_null())
	{
		button_join_game->set_enabled(true);
	}
}

void LobbyView::on_lineedit_chat_enter_pressed()
{
	client->get_network_client()->send_event(CL_NetGameEvent("lobby-add-message", lineedit_chat->get_text()));
	lineedit_chat->set_text("");
}

void LobbyView::player_logged_in(LobbyPlayer *player)
{
	CL_ListViewItem item = listview_players->create_item();
	item.set_column_text("name", player->name);
	item.set_id(player->id);
	listview_players->get_document_item().append_child(item);

	add_info_message(cl_format("%1 logged on", player->name));
}

void LobbyView::player_logged_out(LobbyPlayer *player)
{
	CL_ListViewItem item = listview_players->find(player->id);
	if (!item.is_null())
		item.remove();

	add_info_message(cl_format("%1 logged off", player->name));
}

void LobbyView::player_joined_game(LobbyGame *game, LobbyPlayer *player)
{
	update_game(game);

	if(player->id == client->get_player_id())
	{
		view_create_game->set_active_game(game);
		view_create_game->set_visible(true);
		frame_games->set_visible(false);
	}
	view_create_game->update_view(game);

	add_info_message(cl_format("%1 joined %2", player->name, game->get_name()));
}

void LobbyView::player_left_game(LobbyGame *game, LobbyPlayer *player)
{
	update_game(game);

	if(player->id == client->get_player_id())
	{
		view_create_game->set_visible(false);
		frame_games->set_visible(true);
	}
	else
		view_create_game->update_view(game);

	add_info_message(cl_format("%1 left %2", player->name, game->get_name()));
}

void LobbyView::add_game(LobbyGame *game)
{
	CL_ListViewItem item = listview_games->create_item();
	item.set_id(game->get_id());
	listview_games->get_document_item().append_child(item);

	update_game(game);

	add_info_message(cl_format("New game available: %1", game->get_name()));
}

void LobbyView::create_game(LobbyGame *game)
{
	view_create_game->set_active_game(game);
	view_create_game->update_view(game);

	view_create_game->set_visible(true);
	frame_games->set_visible(false);
}

void LobbyView::remove_game(LobbyGame *game)
{
	CL_ListViewItem item = listview_games->find(game->get_id());
	if (!item.is_null())
		item.remove();

	if(view_create_game->get_active_game() == game)
	{
		view_create_game->set_visible(false);
		frame_games->set_visible(true);
	}

	add_info_message(cl_format("Game %1 was removed", game->get_name()));
}

void LobbyView::update_game(LobbyGame *game)
{
	CL_ListViewItem item = listview_games->find(game->get_id());
	if (!item.is_null())
	{
		item.set_column_text("players", cl_format("%1 / %2", game->get_player_count(), game->get_max_players()));
		item.set_column_text("name", game->get_name());
		item.set_column_text("map", game->get_map_name());

		if(game->get_state() == LobbyGame::lobby)
			item.set_column_text("status", "Waiting for players");
		else
			item.set_column_text("status", "Playing");
	}
}

void LobbyView::add_info_message(CL_String message)
{
	CL_ListViewItem item = listview_chat->create_item();
	item.set_column_text("message", message);
	listview_chat->get_document_item().append_child(item);
}

void LobbyView::add_chat_message(LobbyPlayer *player, CL_String message)
{
	CL_ListViewItem item = listview_chat->create_item();
	CL_String chat_message = cl_format("%1: %2", player->name, message);
	item.set_column_text("message", chat_message);
	listview_chat->get_document_item().append_child(item);
}
