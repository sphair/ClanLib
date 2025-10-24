
#pragma once

class LobbyView;
class LobbyGame;
class Client;

class LobbyViewCreateGame : public CL_Frame
{
public:
	LobbyViewCreateGame(LobbyView *lobby_view, const CL_Rect &geometry, Client *client);
	~LobbyViewCreateGame();

	void set_visible(bool visible = true);

	void update_view(LobbyGame *game);

	void set_active_game(LobbyGame *game);
	LobbyGame *get_active_game() { return active_game; }

private:
	void on_start_game_clicked();
	void on_leave_game_clicked();

	CL_Label *label_players_info;
	CL_Label *label_players;
	CL_PushButton *button_start_game;
	CL_PushButton *button_leave_game;
	CL_ComboBox *combo_maps;
	CL_PopupMenu *menu_maps;

	LobbyGame *active_game;

	Client *client;
};
