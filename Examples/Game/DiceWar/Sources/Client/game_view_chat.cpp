
#include "precomp.h"
#include "game_view_chat.h"
#include "game_view.h"
#include "game_player.h"
#include "client.h"
#include "../Lib/net_events_game.h"

GameViewChat::GameViewChat(GameView *view, Client *client)
: GUIComponent(view, "chat"), client(client)
{
	set_geometry(Rect(0, view->get_height() - 250, 250, view->get_height()));

	listview_chat = new ListView(this);
	listview_chat->set_geometry(Rect(0, 0, 450, 215));
	
	ListViewColumnHeader chat_message_header = listview_chat->get_header()->create_column("message", "Message");
	listview_chat->get_header()->append(chat_message_header);

	lineedit_chat = new LineEdit(this);
	lineedit_chat->set_geometry(Rect(0, 215, 450, 250));
	lineedit_chat->func_enter_pressed().set(this, &GameViewChat::on_lineedit_chat_enter_pressed);
}

GameViewChat::~GameViewChat()
{
}

void GameViewChat::add_player_message(GamePlayer *player, const std::string &message)
{
	ListViewItem item = listview_chat->create_item();
	std::string chat_message = string_format("%1: %2", player->name, message);
	item.set_column_text("message", chat_message);
	listview_chat->get_document_item().append_child(item);
}

void GameViewChat::add_system_message(const std::string &message)
{
	ListViewItem item = listview_chat->create_item();
	item.set_column_text("message", message);
	listview_chat->get_document_item().append_child(item);
}

void GameViewChat::on_lineedit_chat_enter_pressed()
{
	client->get_network_client()->send_event(NetGameEvent(CTS_GAME_ADD_MESSAGE, lineedit_chat->get_text()));
	lineedit_chat->set_text("");
}
