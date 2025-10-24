
#pragma once

class Client;
class LobbyPlayer;
class LobbyGame;
class LobbyViewCreateGame;

class LobbyView : public CL_GUIComponent
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

	void add_chat_message(LobbyPlayer *player, CL_String message);
	void add_info_message(CL_String message);

private:
	void create_frame_chat();
	void create_frame_players();
	void create_frame_games();
	void create_frame_create_game();
	void create_frame_toolbar();

	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_gui_message(CL_GUIMessage &message);
	void on_create_game_clicked();
	void on_join_game_clicked();
	void on_listview_games_selection_changed(CL_ListViewSelection selection);
	void on_lineedit_chat_enter_pressed();
	void on_quit_clicked();

	Client *client;

	CL_Sprite sprite_background;

	LobbyViewCreateGame *view_create_game;

	CL_Frame *frame_games;
	CL_PushButton *button_create_game;
	CL_PushButton *button_join_game;
	CL_ListView *listview_games;

	CL_Frame *frame_players;
	CL_ListView *listview_players;

	CL_Frame *frame_chat_text;
	CL_ListView *listview_chat;
	CL_LineEdit *lineedit_chat;

	CL_PushButton *button_quit;

	static CL_GUITopLevelDescription get_toplevel_description();

	friend class LobbyViewCreateGame;
};
