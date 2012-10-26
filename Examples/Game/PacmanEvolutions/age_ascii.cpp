
#include "precomp.h"
#include "age_ascii.h"
#include "game_world.h"

AgeAscii::AgeAscii(CL_GraphicContext &gc, GameWorld *game)
: game(game)
{
	font = CL_Font(gc, "Courier New", -64);
	chars.resize(game->map.get_width() * game->map.get_height());
	colors.resize(game->map.get_width() * game->map.get_height());
}

void AgeAscii::render(CL_GraphicContext &gc)
{
	gc.clear();

	if (game->player)
		look_at = CL_Vec2f(game->player->get_x(), game->player->get_y());

	for (int y = 0; y < game->map.get_height(); y++)
	{
		for (int x = 0; x < game->map.get_width(); x++)
		{
			set_character(x, y, get_tile_character(game->map.get_tile(x, y)), get_tile_color(game->map.get_tile(x, y)));
		}
	}

	if (game->player)
	{
		set_character((int)game->player->get_x(), (int)game->player->get_y(), L'¤', CL_Colorf::yellow);
	}

	for (size_t i = 0; i < game->ghosts.size(); i++)
	{
		set_character((int)game->ghosts[i]->get_x(), (int)game->ghosts[i]->get_y(), L'@', CL_Colorf::red);
	}


	float char_width = (float)font.get_text_size(gc, CL_String("X")).width;
	float line_height = font.get_font_metrics().get_height() + font.get_font_metrics().get_external_leading();
	float baseline_offset = font.get_font_metrics().get_ascent();

	CL_Vec2i offset = CL_Vec2i(
		(int)(look_at.x * char_width - gc.get_width() * 0.5f),
		(int)(look_at.y * line_height - gc.get_height() * 0.5f));

	offset.x = -cl_clamp(offset.x, 0, (int)(game->map.get_width() * char_width - gc.get_width()));
	offset.y = -cl_clamp(offset.y, 0, (int)(game->map.get_height() * line_height - gc.get_height()));

	CL_String16 text16;
	for (int y = 0; y < game->map.get_height(); y++)
	{
		CL_Colorf color;
		int start_x = 0;
		text16.clear();
		for (int x = 0; x < game->map.get_width(); x++)
		{
			if (x == 0)
			{
				color = colors[x + y * game->map.get_width()];
			}
			else if (color != colors[x + y * game->map.get_width()] && chars[x + y * game->map.get_width()] != L' ')
			{
				CL_String text = CL_StringHelp::ucs2_to_utf8(text16);
				font.draw_text(gc, offset.x + char_width * start_x, offset.y + y * line_height + baseline_offset, text, color);
				start_x = x;
				text16.clear();
				color = colors[x + y * game->map.get_width()];
			}

			text16.push_back(chars[x + y * game->map.get_width()]);
		}

		if (!text16.empty())
		{
			CL_String text = CL_StringHelp::ucs2_to_utf8(text16);
			font.draw_text(gc, offset.x + char_width * start_x, offset.y + y * line_height + baseline_offset, text, color);
		}
	}
}

void AgeAscii::set_character(int x, int y, wchar_t symbol, const CL_Colorf &color)
{
	if (x >= 0 && y >= 0 && x < game->map.get_width() && y < game->map.get_height())
	{
		chars[x + y * game->map.get_width()] = symbol;
		colors[x + y * game->map.get_width()] = color;
	}
}

wchar_t AgeAscii::get_tile_character(int tile)
{
	switch (tile)
	{
	case 0: return L' ';
	case 1: return 0x00B7;
	case 2: return L'*';
	case 3: return L'~';
	case 4: return 0x256C; // wall
	case 4+1: return 0x2550; // left
	case 4+2: return 0x2550; // right
	case 4+3: return 0x2550; // left+right
	case 4+4: return 0x2551; // up
	case 4+5: return 0x255D; // up+left
	case 4+6: return 0x255A; // up+right
	case 4+7: return 0x2569; // up+left+right
	case 4+8: return 0x2551; // down
	case 4+9: return 0x2557; // down+left
	case 4+10: return 0x2554; // down+right
	case 4+11: return 0x2566; // down+left+right
	case 4+12: return 0x2551; // up+down
	case 4+13: return 0x2563; // up+down+left
	case 4+14: return 0x2560; // up+down+right
	case 4+15: return 0x256C; // up+down+left+right
	default: return L'?';
	}
}

CL_Colorf AgeAscii::get_tile_color(int tile)
{
	switch (tile)
	{
	case 1: // egg
		return CL_Colorf::mintcream;
	case 2: // powerup
		return CL_Colorf::skyblue;
	case 3: // trail
		return CL_Colorf::mintcream;
	case 4: // walls
	case 4+1:
	case 4+2:
	case 4+3:
	case 4+4:
	case 4+5:
	case 4+6:
	case 4+7:
	case 4+8:
	case 4+9:
	case 4+10:
	case 4+11:
	case 4+12:
	case 4+13:
	case 4+14:
	case 4+15:
		return CL_Colorf::mintcream;
	case 0:
	default:
		return CL_Colorf::mintcream;
	}
}
