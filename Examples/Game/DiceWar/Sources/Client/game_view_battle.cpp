
#include "precomp.h"
#include "game_view_battle.h"
#include "game_view.h"
#include "game_map.h"
#include "game_model.h"
#include "game_player.h"
#include "game_player_collection.h"
#include "client.h"
#include "../Lib/map_area.h"

GameViewBattle::GameViewBattle(GameView *view, Client *client)
: GUIComponent(view, "battle"), view(view), client(client)
{
	set_geometry(Rect(view->get_width() - 400, 0, view->get_width(), 300));
	set_visible(false);

	func_render().set(this, &GameViewBattle::on_render);
	func_process_message().set(this, &GameViewBattle::on_message);

	timer_hide.func_expired().set(this, &GameViewBattle::on_timer_hide_expired);

	Texture2D texture_dice(get_canvas(), "Resources/dices.png", VirtualDirectory());
	SpriteDescription dice_description;
	dice_description.add_gridclipped_frames(texture_dice, 0, 0, 42, 45, 6, 1);
	sprite_dices = Sprite(get_canvas(), dice_description);

	font_large = Font(get_canvas(), "Accidental Presidency", -40);
	font_small = Font(get_canvas(), "Accidental Presidency", -30);
}

GameViewBattle::~GameViewBattle()
{
}

void GameViewBattle::attacked_area(int map_area_from_id, int map_area_to_id)
{
	attack_results.clear();
	defense_results.clear();
	current_dice_index = 0;

	GameModel *game_model = client->get_game_model();

	MapArea *area_from = game_model->get_map()->get_area_by_id(map_area_from_id);
	MapArea *area_to = game_model->get_map()->get_area_by_id(map_area_to_id);
	player_from = game_model->get_players()->get_player(area_from->player_id);
	player_to = game_model->get_players()->get_player(area_to->player_id);

	set_visible(true);

	timer_hide.start(500, true);
}

void GameViewBattle::attack_dice_result(const std::vector<int> &dice_results)
{
	attack_results = dice_results;
}

void GameViewBattle::defense_dice_result(const std::vector<int> &dice_results)
{
	defense_results = dice_results;
}

void GameViewBattle::on_render(Canvas &canvas, const Rect &clip_rect)
{
	std::string text = string_format("%1 vs %2", player_from->name, player_to->name);
	Size text_size = font_small.get_text_size(canvas, text);
	int xpos = (get_width() - text_size.width) / 2;
	int ypos = get_height() / 2 + 10;
	font_small.draw_text(canvas, xpos, ypos, text);

	int attack_strength = 0;
	int defense_strength = 0;

	if(attack_results.size() > 0 && defense_results.size() > 0)
	{
		int dice_count = attack_results.size();
		int dice_width = dice_count * 35;
		xpos = (get_width() - dice_width) / 2;
		ypos = 20;

		for(int i = 0; i < clan::min(dice_count, current_dice_index); i++)
		{
			attack_strength += attack_results[i];
			sprite_dices.set_frame(attack_results[i] - 1);
			sprite_dices.draw(canvas, (float)xpos, (float)ypos);

			xpos += 35;
		}

		dice_count = defense_results.size();
		dice_width = dice_count * 35;
		xpos = (get_width() - dice_width) / 2;
		ypos = get_height() - 45 - 20;

		for(int i = 0; i < clan::min(dice_count, current_dice_index); i++)
		{
			defense_strength += defense_results[i];
			sprite_dices.set_frame(defense_results[i] - 1);
			sprite_dices.draw(canvas, (float)xpos, (float)ypos);

			xpos += 35;
		}
	}

	text = StringHelp::int_to_text(attack_strength);
	text_size = font_large.get_text_size(canvas, text);
	xpos = (get_width() - text_size.width) / 2;
	ypos = 109;
	font_large.draw_text(canvas, xpos, ypos, text);

	text = StringHelp::int_to_text(defense_strength);
	text_size = font_large.get_text_size(canvas, text);
	xpos = (get_width() - text_size.width) / 2;
	ypos = get_height() - 80;
	font_large.draw_text(canvas, xpos, ypos, text);
}

void GameViewBattle::on_message(GUIMessage &message)
{
	if (message.is_type(GUIMessage_Input::get_type_name()))
	{
		GUIMessage_Input msg_input = message;
		InputEvent e = msg_input.get_event();
		if (e.type == InputEvent::pressed)
		{
			int max_dice = clan::max(attack_results.size(), defense_results.size());

			if(current_dice_index < max_dice)
				current_dice_index = max_dice;
			else
				close();
		}
	}
}

void GameViewBattle::close()
{
	timer_hide.stop();
	set_visible(false);
	view->battle_over();
}

void GameViewBattle::on_timer_hide_expired()
{
	if(attack_results.size() > 0 && defense_results.size() > 0)
	{
		current_dice_index++;

		int max_dice = clan::max(attack_results.size(), defense_results.size());
		if(current_dice_index > max_dice + 2)
			close();
	}
}
