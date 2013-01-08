
#pragma once

class GameView;
class GameModel;
class GamePlayer;
class Client;

class GameViewBattle : public GUIComponent
{
public:
	GameViewBattle(GameView *view, Client *client);
	~GameViewBattle();

	void attacked_area(int map_area_from_id, int map_area_to_id);
	void attack_dice_result(const std::vector<int> &dice_results);
	void defense_dice_result(const std::vector<int> &dice_results);

private:
	void close();

	void on_render(Canvas &canvas, const Rect &clip_rect);
	void on_message(GUIMessage &message);
	void on_timer_hide_expired();

private:
	Client *client;
	GameView *view;
	
	Timer timer_hide;

	Sprite sprite_dices;

	Font font_large;
	Font font_small;

	std::vector<int> attack_results;
	std::vector<int> defense_results;
	int current_dice_index;

	GamePlayer *player_from;
	GamePlayer *player_to;
};
