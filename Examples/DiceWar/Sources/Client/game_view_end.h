
#pragma once

class GameView;
class GameModel;

class GameViewEnd : public CL_GUIComponent
{
public:
	GameViewEnd(GameView *view);
	~GameViewEnd();

	void game_over(const CL_String &title, const CL_String &message);

private:
	void close();
	void on_button_close_clicked();

private:
	CL_Label *label_title;
	CL_Label *label_message;
	CL_PushButton *button_close;
};
