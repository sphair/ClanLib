
#pragma once

class LobbyView;
class LobbyGame;
class Client;

class LobbyViewCreateGame : public Frame
{
public:
	LobbyViewCreateGame(LobbyView *lobby_view, const Rect &geometry, Client *client);
	~LobbyViewCreateGame();

	void set_visible(bool visible = true);

	void update_view(LobbyGame *game);

	void set_active_game(LobbyGame *game);
	LobbyGame *get_active_game() { return active_game; }

private:
	void on_start_game_clicked();
	void on_leave_game_clicked();

	Label *label_players_info;
	Label *label_players;
	PushButton *button_start_game;
	PushButton *button_leave_game;
	ComboBox *combo_maps;
	PopupMenu *menu_maps;

	LobbyGame *active_game;

	Client *client;
};
