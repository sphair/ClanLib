
#pragma once

class Client;
class LobbyPlayer;
class LobbyGame;
class LobbyViewCreateGame;

class LobbyView : public GUIComponent
{
public:
	LobbyView(Client *client);
	~LobbyView();

	void player_logged_in(LobbyPlayer *player);
	void player_logged_out(LobbyPlayer *player);

	void player_joined_game(LobbyGame *game, LobbyPlayer *player);
	void player_left_game(LobbyGame *game, LobbyPlayer *player);

	void add_game(LobbyGame *game);
	void create_game(LobbyGame *game);
	void remove_game(LobbyGame *game);
	void update_game(LobbyGame *game);

	void add_chat_message(LobbyPlayer *player, std::string message);
	void add_info_message(std::string message);

private:
	void create_frame_chat();
	void create_frame_players();
	void create_frame_games();
	void create_frame_create_game();
	void create_frame_toolbar();

	void on_render(GraphicContext &gc, const Rect &clip_rect);
	void on_gui_message(GUIMessage &message);
	void on_create_game_clicked();
	void on_join_game_clicked();
	void on_listview_games_selection_changed(ListViewSelection selection);
	void on_lineedit_chat_enter_pressed();
	void on_quit_clicked();

	Client *client;

	Sprite sprite_background;

	LobbyViewCreateGame *view_create_game;

	Frame *frame_games;
	PushButton *button_create_game;
	PushButton *button_join_game;
	ListView *listview_games;

	Frame *frame_players;
	ListView *listview_players;

	Frame *frame_chat_text;
	ListView *listview_chat;
	LineEdit *lineedit_chat;

	PushButton *button_quit;

	static GUITopLevelDescription get_toplevel_description();

	friend class LobbyViewCreateGame;
};
