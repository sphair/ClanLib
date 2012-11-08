
#include "precomp.h"
#include "age_2d.h"
#include "game_world.h"

Age2D::Age2D(CL_GraphicContext &gc, GameWorld *game)
: game(game)
{
	CL_SpriteDescription left_pacman_desc;
	left_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 0, 0, 28, 32, 6, 1);
	pacman[0] = CL_Sprite(gc, left_pacman_desc);
	pacman[0].set_alignment(origin_center);
	pacman[0].set_play_pingpong();

	CL_SpriteDescription right_pacman_desc;
	right_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 28*6, 0, 28, 32, 6, 1);
	pacman[1] = CL_Sprite(gc, right_pacman_desc);
	pacman[1].set_alignment(origin_center);
	pacman[1].set_play_pingpong();

	CL_SpriteDescription up_pacman_desc;
	up_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 0, 32, 28, 32, 6, 1);
	pacman[2] = CL_Sprite(gc, up_pacman_desc);
	pacman[2].set_alignment(origin_center);
	pacman[2].set_play_pingpong();

	CL_SpriteDescription down_pacman_desc;
	down_pacman_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 28*6, 32, 28, 32, 6, 1);
	pacman[3] = CL_Sprite(gc, down_pacman_desc);
	pacman[3].set_alignment(origin_center);
	pacman[3].set_play_pingpong();

	CL_SpriteDescription green_ghost_desc;
	green_ghost_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 336, 0, 28, 32, 6, 1);
	ghost[0] = CL_Sprite(gc, green_ghost_desc);
	ghost[0].set_alignment(origin_center);
	ghost[0].set_play_pingpong();
	ghost[0].set_delay(100);

	CL_SpriteDescription red_ghost_desc;
	red_ghost_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 336, 32, 28, 32, 6, 1);
	ghost[1] = CL_Sprite(gc, red_ghost_desc);
	ghost[1].set_alignment(origin_center);
	ghost[1].set_play_pingpong();
	ghost[1].set_delay(100);

	CL_SpriteDescription tiles_desc;
	tiles_desc.add_gridclipped_frames(CL_PNGProvider::load("pacman.png"), 0, 64, 34, 38, 10, 2);
	tiles = CL_Sprite(gc, tiles_desc);

/*
    <font name="fnt_clansoft">
      <bitmap
        glyphs="Game/spr_clansoft" 
        spacelen="6" 
        letters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" />
    </font>
*/
}

void Age2D::render(CL_GraphicContext &gc)
{
	gc.clear();

	if (game->player)
		look_at = CL_Vec2f(game->player->get_x(), game->player->get_y());

	CL_Vec2i offset = CL_Vec2i(
		(int)(look_at.x * tiles.get_width() - gc.get_width() * 0.5f),
		(int)(look_at.y * tiles.get_height() - gc.get_height() * 0.5f));

	offset.x = -cl_clamp(offset.x, 0, game->map.get_width() * tiles.get_width() - gc.get_width());
	offset.y = -cl_clamp(offset.y, 0, game->map.get_height() * tiles.get_height() - gc.get_height());

	for (int y = 0; y < game->map.get_height(); y++)
	{
		for (int x = 0; x < game->map.get_width(); x++)
		{
			tiles.set_frame(game->map.get_tile(x, y));
			tiles.draw(gc, offset.x + x * tiles.get_width(), offset.y + y * tiles.get_height());
		}
	}

	for (int i = 0; i < 4; i++)
		pacman[i].update();
	for (int i = 0; i < 2; i++)
		ghost[i].update();

	int center_x = tiles.get_width() / 2 + 3;
	int center_y = tiles.get_height() / 2 + 2;

	if (game->player)
	{
		int direction = game->player->get_move_dir();
		pacman[direction].draw(gc, offset.x + center_x + (int)(game->player->get_x() * tiles.get_width()), offset.y + center_y + (int)(game->player->get_y() * tiles.get_height()));
	}

	for (size_t i = 0; i < game->ghosts.size(); i++)
	{
		int color = game->ghosts[i]->is_red_ghost() ? 1 : 0;
		ghost[color].draw(gc, offset.x + center_x + (int)(game->ghosts[i]->get_x() * tiles.get_width()), offset.y + center_y + (int)(game->ghosts[i]->get_y() * tiles.get_height()));
	}
}
