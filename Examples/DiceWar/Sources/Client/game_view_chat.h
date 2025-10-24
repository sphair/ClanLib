
#pragma once

class GameView;
class Client;
class GamePlayer;

class GameViewChat : public CL_GUIComponent
{
public:
	GameViewChat(GameView *view, Client *client);
	~GameViewChat();

	void add_player_message(GamePlayer *player, const CL_String &message);
	void add_system_message(const CL_String &message);

private:
	void on_lineedit_chat_enter_pressed();

private:
	Client *client;

	CL_ListView *listview_chat;
	CL_LineEdit *lineedit_chat;
};
