
#pragma once

#include "game_map_selection.h"

class Client;
class MapArea;
class GameTerrain;
class GamePlayer;
class ModelAssimp;
class LightModel;
class GameViewPlayerList;
class GameViewBattle;
class GameViewEnd;
class GameViewChat;
class GameViewActionBar;

class GameView : public GUIComponent
{
public:
	GameView(Client *client);
	~GameView();

	void load_map();
	void player_joined_game(GamePlayer *player);
	void player_left_game(GamePlayer *player);
	void start_game();

	void update_map();

	void set_active_turn();
	void set_waiting_turn();

	void add_player_message(GamePlayer *player, const std::string &message);
	void add_system_message(const std::string &message);

	void invalid_attack();
	void attacked_area(int map_area_from_id, int map_area_to_id);
	void attack_dice_result(const std::vector<int> &dice_results);
	void defense_dice_result(const std::vector<int> &dice_results);
	void game_over(const std::string &title, const std::string &message);

	void battle_over();

	void end_turn();

	Client *get_client() { return client; }

private:
	bool is_active_turn;
	bool is_in_battle;

	void on_render(GraphicContext &gc, const Rect &clip_rect);
	void on_message(GUIMessage &message);

	void on_message_pointer_moved();
	void on_message_mouse_left_pressed();

	void render_map(GraphicContext &gc);
	void render_armies(GraphicContext &gc, const LightModel &light_model);
	void render_army(GraphicContext &gc, const LightModel &light_model, MapArea *area);

	Mat4f get_projection();
	Mat4f get_modelview();

	int get_area_id_at_mousepos(GraphicContext &gc, Mat4f &projection, Mat4f &modelview);

	void set_battle_state(bool in_battle);
	void set_playing_state(bool active_turn);

	void update_actionbar();

	Client *client;

	std::auto_ptr<GameTerrain> terrain;

	std::auto_ptr<ModelAssimp> army_model;

	std::auto_ptr<MapSelection> map_selection;

	Sprite sprite_waiting_for_turn;
	Font font_large;

	std::auto_ptr<GameViewPlayerList> view_playerlist;
	std::auto_ptr<GameViewBattle> view_battle;
	std::auto_ptr<GameViewEnd> view_gameend;
	std::auto_ptr<GameViewChat> view_chat;
	std::auto_ptr<GameViewActionBar> view_actionbar;

	float view_angle_x;
	float view_angle_y;
	float view_angle_z;
	float view_distance;
	bool camera_drag_in_progress;
	Point camera_drag_last_mouse_pos;

	static GUITopLevelDescription get_toplevel_description();
};
