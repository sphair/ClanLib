
#include "precomp.h"
#include "age_2d.h"
#include "game_world.h"

Age2D::Age2D(clan::Canvas &canvas, GameWorld *game)
: game(game)
{
	clan::Texture2D pacman_texture(canvas, "pacman.png");

	pacman[0] = clan::Sprite(canvas);
	pacman[0].add_gridclipped_frames(canvas, pacman_texture, 0, 0, 28, 32, 6, 1);
	pacman[0].set_alignment(clan::origin_center);
	pacman[0].set_play_pingpong();

	pacman[1] = clan::Sprite(canvas);
	pacman[1].add_gridclipped_frames(canvas, pacman_texture, 28*6, 0, 28, 32, 6, 1);
	pacman[1].set_alignment(clan::origin_center);
	pacman[1].set_play_pingpong();

	pacman[2] = clan::Sprite(canvas);
	pacman[2].add_gridclipped_frames(canvas, pacman_texture, 0, 32, 28, 32, 6, 1);
	pacman[2].set_alignment(clan::origin_center);
	pacman[2].set_play_pingpong();

	pacman[3] = clan::Sprite(canvas);
	pacman[3].add_gridclipped_frames(canvas, pacman_texture, 28*6, 32, 28, 32, 6, 1);
	pacman[3].set_alignment(clan::origin_center);
	pacman[3].set_play_pingpong();

	ghost[0] = clan::Sprite(canvas);
	ghost[0].add_gridclipped_frames(canvas, pacman_texture, 336, 0, 28, 32, 6, 1);
	ghost[0].set_alignment(clan::origin_center);
	ghost[0].set_play_pingpong();
	ghost[0].set_delay(100);

	ghost[1] = clan::Sprite(canvas);
	ghost[1].add_gridclipped_frames(canvas, pacman_texture, 336, 32, 28, 32, 6, 1);
	ghost[1].set_alignment(clan::origin_center);
	ghost[1].set_play_pingpong();
	ghost[1].set_delay(100);

	tiles = clan::Sprite(canvas);
	tiles.add_gridclipped_frames(canvas, pacman_texture, 0, 64, 34, 38, 10, 2);

/*
    <font name="fnt_clansoft">
      <bitmap
        glyphs="Game/spr_clansoft" 
        spacelen="6" 
        letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" />
    </font>
*/
}

void Age2D::render(clan::Canvas &canvas, int elapsed_time_ms)
{
	canvas.clear();

	if (game->player)
		look_at = clan::Vec2f(game->player->get_x(), game->player->get_y());

	clan::Vec2i offset = clan::Vec2i(
		(int)(look_at.x * tiles.get_width() - canvas.get_width() * 0.5f),
		(int)(look_at.y * tiles.get_height() - canvas.get_height() * 0.5f));

	offset.x = -clan::clamp(offset.x, 0, game->map.get_width() * tiles.get_width() - canvas.get_width());
	offset.y = -clan::clamp(offset.y, 0, game->map.get_height() * tiles.get_height() - canvas.get_height());

	for (int y = 0; y < game->map.get_height(); y++)
	{
		for (int x = 0; x < game->map.get_width(); x++)
		{
			tiles.set_frame(game->map.get_tile(x, y));
			tiles.draw(canvas, offset.x + x * tiles.get_width(), offset.y + y * tiles.get_height());
		}
	}

	for (int i = 0; i < 4; i++)
		pacman[i].update(elapsed_time_ms);
	for (int i = 0; i < 2; i++)
		ghost[i].update(elapsed_time_ms);

	int center_x = tiles.get_width() / 2 + 3;
	int center_y = tiles.get_height() / 2 + 2;

	if (game->player)
	{
		int direction = game->player->get_move_dir();
		pacman[direction].draw(canvas, offset.x + center_x + (int)(game->player->get_x() * tiles.get_width()), offset.y + center_y + (int)(game->player->get_y() * tiles.get_height()));
	}

	for (size_t i = 0; i < game->ghosts.size(); i++)
	{
		int color = game->ghosts[i]->is_red_ghost() ? 1 : 0;
		ghost[color].draw(canvas, offset.x + center_x + (int)(game->ghosts[i]->get_x() * tiles.get_width()), offset.y + center_y + (int)(game->ghosts[i]->get_y() * tiles.get_height()));
	}
}
