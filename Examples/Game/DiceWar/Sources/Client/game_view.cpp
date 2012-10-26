
#include "precomp.h"
#include "client.h"
#include "game_player.h"
#include "game_model.h"
#include "game_terrain.h"
#include "game_view.h"
#include "game_view_playerlist.h"
#include "game_view_battle.h"
#include "game_view_end.h"
#include "game_view_chat.h"
#include "game_view_actionbar.h"
#include "3D/model_assimp.h"
#include "3D/light_model.h"
#include "3D/camera.h"
#include "../Lib/map_area.h"
#include "../Lib/net_events_game.h"

GameView::GameView(Client *client)
: GUIComponent(client->get_gui(), get_toplevel_description()), 
  client(client),
  view_angle_x(180.0f), view_angle_y(0.0f), view_angle_z(45.0f), view_distance(500.0f),
  camera_drag_in_progress(false)
{
	set_type_name("gameview");

	func_render().set(this, &GameView::on_render);
	func_process_message().set(this, &GameView::on_message);

	view_playerlist.reset(new GameViewPlayerList(this, client));
	view_battle.reset(new GameViewBattle(this, client));
	view_chat.reset(new GameViewChat(this, client));
	view_gameend.reset(new GameViewEnd(this));
	view_actionbar.reset(new GameViewActionBar(this));

	sprite_waiting_for_turn = Sprite(get_gc(), "Resources/waiting_for_turn.png");

	font_large = Font(get_gc(), "Accidental Presidency", -40);

	GraphicContext gc = get_gc();
	army_model.reset(new ModelAssimp(gc, "Resources/army_unit.3ds"));

	set_playing_state(false);
	set_battle_state(false);

	set_constant_repaint(true);
}	

GameView::~GameView()
{
}

GUITopLevelDescription GameView::get_toplevel_description()
{
	GUITopLevelDescription desc;
	desc.set_title("DiceWar Game");
	desc.set_allow_resize(true);
	desc.set_depth_size(16);
	desc.set_position(Rect(0, 0, 1024, 768), false);
	return desc;
}

void GameView::load_map()
{
	ClientMap *map = client->get_game_model()->get_map();

	std::string path = string_format("Resources/Maps/%1/", map->get_mapname());

	GraphicContext gc = get_gc();
	terrain.reset(
		new GameTerrain(
			gc, 
			path + "heightmap.png", 
			path + "texture.png", 
			path + "areas.png", 
			path + "borders.png", 
			map->areas_size(),
			map->get_terrain_scale()));

	map_selection.reset(new MapSelection(client));
}

void GameView::start_game()
{
	view_chat->add_system_message("May Sphair win!");
}

void GameView::player_joined_game(GamePlayer *player)
{
	view_chat->add_system_message(string_format("%1 joined the game", player->name));
}

void GameView::player_left_game(GamePlayer *player)
{
	view_chat->add_system_message(string_format("%1 left the game", player->name));
}

void GameView::update_map()
{
	map_selection->update_areas_with_player_ownership();
	map_selection->clear_selection();
}

void GameView::set_active_turn()
{
	set_playing_state(true);
}

void GameView::set_waiting_turn()
{
	set_playing_state(false);
}

void GameView::invalid_attack()
{
	view_chat->add_system_message("Invalid attack");
}

void GameView::attacked_area(int map_area_from_id, int map_area_to_id)
{
	set_battle_state(true);

	view_battle->attacked_area(map_area_from_id, map_area_to_id);
}

void GameView::attack_dice_result(const std::vector<int> &dice_results)
{
	view_battle->attack_dice_result(dice_results);
}

void GameView::defense_dice_result(const std::vector<int> &dice_results)
{
	view_battle->defense_dice_result(dice_results);
}

void GameView::battle_over()
{
	set_battle_state(false);

	client->get_network_client()->send_event(NetGameEvent(CTS_GAME_BATTLE_VIEW_OVER));
}

void GameView::set_battle_state(bool in_battle)
{
	is_in_battle = in_battle;
	update_actionbar();
}

void GameView::set_playing_state(bool active_turn)
{
	is_active_turn = active_turn;
	update_actionbar();
}

void GameView::update_actionbar()
{
	if(is_active_turn && !is_in_battle)
		view_actionbar->enable_end_turn();
	else
		view_actionbar->disable_end_turn();
}

void GameView::game_over(const std::string &title, const std::string &message)
{
	view_gameend->game_over(title, message);
}

void GameView::on_message(GUIMessage &message)
{
	if (message.is_type(GUIMessage_Close::get_type_name()))
		exit_with_code(0);

	if (message.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input msg_input = message;
		InputEvent e = msg_input.get_event();

		if (e.id == MOUSE_LEFT && e.type == InputEvent::pressed)
		{
			on_message_mouse_left_pressed();
			request_repaint();
		}
		else if (e.id == KEY_SPACE && e.type == InputEvent::pressed)
		{
			end_turn();
			request_repaint();
		}
		else if (e.id == KEY_ESCAPE && e.type == InputEvent::pressed)
		{
			exit_with_code(0);		
		}
		else if (e.id == MOUSE_WHEEL_UP)
		{
			view_distance -= 10.0f;
			if (view_distance < 100.0f)
				view_distance = 100.0f;
		}
		else if (e.id == MOUSE_WHEEL_DOWN)
		{
			view_distance += 10.0f;
			if (view_distance > 1000.0f)
				view_distance = 1000.0f;
		}
		else if (e.id == MOUSE_RIGHT)
		{
			if (e.type == InputEvent::pressed)
			{
				camera_drag_in_progress = true;
				capture_mouse(true);
			}
			else
			{
				camera_drag_in_progress = false;
				capture_mouse(false);
			}
			camera_drag_last_mouse_pos = e.mouse_pos;
		}
		else if (e.type == InputEvent::pointer_moved)
		{
			if (camera_drag_in_progress)
			{
				Point move_delta = e.mouse_pos - camera_drag_last_mouse_pos;

				view_angle_y += move_delta.x * 0.5f;
				view_angle_z += move_delta.y * 0.5f;
				if (view_angle_z < 10.0f)
					view_angle_z = 10.0f;
				else if (view_angle_z > 90.0f)
					view_angle_z = 90.0f;
				while (view_angle_y < 0.0f)
					view_angle_y += 360.0f;
				while (view_angle_y > 360.0f)
					view_angle_y -= 360.0f;

				camera_drag_last_mouse_pos = e.mouse_pos;
			}
		}
	}
}

void GameView::end_turn()
{
	if(is_active_turn && !is_in_battle)
		client->request_end_turn();
}

void GameView::on_message_mouse_left_pressed()
{
	if(is_active_turn && !is_in_battle)
	{
		map_selection->select_highlighted_area();
		if(map_selection->is_complete())
		{
			client->request_attack_area(map_selection->get_selected_source_area_id(), map_selection->get_selected_destination_area_id());
			map_selection->clear_selection();
		}
	}
}

void GameView::on_render(GraphicContext &gc, const Rect &clip_rect)
{
	render_map(gc);

	if(is_active_turn == false)
		sprite_waiting_for_turn.draw(gc, 0, 0);
}

Mat4f GameView::get_projection()
{
	Rect client = get_geometry();

	Mat4f projection = Mat4f::perspective(45.0f, client.get_width()/(float) client.get_height(), 10.0f, 2000.0f);
	projection = projection * Mat4f::scale(1.0f, -1.0f, 1.0f); // turn it upside down because we render to Texture WM

	return projection;
}

Mat4f GameView::get_modelview()
{
	Orientation o;
	o.rotate(view_angle_x, view_angle_y, view_angle_z);
	Position p;
	p.translate(o, 0.0f, 0.0f, -view_distance);
	Camera camera;
	camera.set_orientation(o);
	camera.set_position(p);
	return camera.get_modelview();
}

// Get the area id on a specific map position based on mouse position
int GameView::get_area_id_at_mousepos(GraphicContext &gc, Mat4f &projection, Mat4f &modelview)
{
	Point cursor = get_ic().get_mouse().get_position();
	Point map_pos = terrain->from_screen_point(gc, cursor, projection, modelview, get_geometry());
	return terrain->get_area(map_pos);
}

void GameView::render_map(GraphicContext &gc)
{
	Rect client_area = get_geometry();

	Draw::fill(gc, 0.0f, 0.0f, (float)client_area.right - client_area.left, (float)client_area.bottom - client_area.top, Colorf::black);

	if (!terrain.get())
	{
		font_large.draw_text(gc, 300, 200, "Loading Map...");
		return;
	}

	Mat4f modelview = get_modelview();
	Mat4f projection = get_projection();

	gc.set_map_mode(cl_user_projection);
	gc.set_viewport(client_area);
	gc.set_projection(projection);
	gc.set_modelview(modelview);

	gc.enable_depth_test(true);
	gc.set_depth_compare_function(cl_comparefunc_lequal);
	gc.enable_depth_write(true);
	
	LightModel light_model;
	LightSource light0;
	light0.constant_attenuation = 0.004f;
	light0.position = get_modelview()*Vec4f(-60.0f, 125.0f, 350.0f, 1.0f);
	//light0.position = Vec4f(-60.0f, 125.0f, 350.0f, 1.0f);
	light_model.scene_ambient = Vec4f(0.4f, 0.4f, 0.4f, 1.0f);
	light_model.light_sources.push_back(light0);

	gc.clear_depth(1.0f);

	terrain->render(gc, light_model);

	render_armies(gc, light_model);

	if(is_active_turn && !is_in_battle)
	{
		map_selection->set_highlighted_area(get_area_id_at_mousepos(gc, projection, modelview));
	}

	if(map_selection->is_modified())
	{
		// Update terrain with new colors
		GraphicContext gc = get_gc();
		terrain->set_area_colors(gc, map_selection->get_result_colors());
	}

	gc.reset_buffer_control();

	gc.set_map_mode(cl_map_2d_upper_left);
	gc.set_viewport(Rect(0, 0, gc.get_width(), gc.get_height()));
	gc.set_modelview(Mat4f::identity());
}

void GameView::render_armies(GraphicContext &gc, const LightModel &light_model)
{
	ClientMap *map = client->get_game_model()->get_map();
	for(unsigned int i = 0; i < map->areas_size(); ++i)
	{
		render_army(gc, light_model, map->get_area(i));
	}
}

void GameView::render_army(GraphicContext &gc, const LightModel &light_model, MapArea *area)
{
	Position pos(area->position.x/4 - terrain->get_size().width*2.0f, 15, area->position.y/4 - terrain->get_size().height*2.0f);
	Orientation orientation = Orientation::look_at(0.0f, -90.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	orientation.rotate(90.0f, Vec3f(1.0f, 0.0f, 0.0f));

	if (area->army_strength == 1)
	{
		army_model->render(gc, light_model, pos, orientation);
	}
	else if (area->army_strength > 1)
	{
		for (int i = 0; i < area->army_strength; i++)
		{
			int angle = i * 360 / area->army_strength;
			float x = cos(angle * 3.1428f / 180);
			float z = sin(angle * 3.1428f / 180);

			Orientation orientation2 = orientation;
			orientation2.rotate((float)angle, Vec3f(0.0f, 1.0f, 0.0f));

			Position pos2 = pos;
			pos2.set_x(pos.get_x() + x * (6 + area->army_strength));
			pos2.set_z(pos.get_z() + z * (6 + area->army_strength));
			army_model->render(gc, light_model, pos2, orientation2);
		}
	}
}

void GameView::add_player_message(GamePlayer *player, const std::string &message)
{
	view_chat->add_player_message(player, message);
}

void GameView::add_system_message(const std::string &message)
{
	view_chat->add_system_message(message);
}
