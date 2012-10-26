
#pragma once

class GameView;

class GameViewActionBar : public GUIComponent
{
public:
	GameViewActionBar(GameView *view);
	~GameViewActionBar();

	void enable_end_turn();
	void disable_end_turn();

private:
	void on_button_endturn_clicked();

private:
	PushButton *button_endturn;
	GameView *view;
};
