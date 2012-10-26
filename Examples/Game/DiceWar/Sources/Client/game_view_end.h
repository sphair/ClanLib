
#pragma once

class GameView;
class GameModel;

class GameViewEnd : public GUIComponent
{
public:
	GameViewEnd(GameView *view);
	~GameViewEnd();

	void game_over(const std::string &title, const std::string &message);

private:
	void close();
	void on_button_close_clicked();

private:
	Label *label_title;
	Label *label_message;
	PushButton *button_close;
};
