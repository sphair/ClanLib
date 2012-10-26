
#pragma once

class GameView;
class Client;
class GamePlayer;

class GameViewChat : public GUIComponent
{
public:
	GameViewChat(GameView *view, Client *client);
	~GameViewChat();

	void add_player_message(GamePlayer *player, const std::string &message);
	void add_system_message(const std::string &message);

private:
	void on_lineedit_chat_enter_pressed();

private:
	Client *client;

	ListView *listview_chat;
	LineEdit *lineedit_chat;
};
