
#pragma once

class GameView;
class Client;

class GameViewPlayerList : public GUIComponent
{
public:
	GameViewPlayerList(GameView *view, Client *client);
	~GameViewPlayerList();

private:
	std::vector<Label *> label_players;

	void on_render(Canvas &canvas, const Rect &clip_rect);

	Client *client;
};
