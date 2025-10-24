
#pragma once

class GameView;
class Client;

class GameViewPlayerList : public CL_GUIComponent
{
public:
	GameViewPlayerList(GameView *view, Client *client);
	~GameViewPlayerList();

private:
	std::vector<CL_Label *> label_players;

	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);

	Client *client;
};
