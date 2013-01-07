
#include "precomp.h"
#include "text_explosion_2d.h"

TextExplosion2D::TextExplosion2D()
: initial_x(), initial_y()
{
}

void TextExplosion2D::explode(clan::GraphicContext &gc, const std::string &new_text, int new_x, int new_y, clan::Font new_font, clan::Colorf new_color)
{
	letter_positions.clear();
	letter_deltas.clear();
	text = new_text;
	font = new_font;
	color = new_color;
	initial_x = new_x;
	initial_y = new_y;

	int total_length = font.get_text_size(gc, text).width;
	int x = initial_x - total_length/2;
	letter_positions.resize(text.length());
	letter_deltas.resize(text.length());
	for (size_t i = 0; i < text.length(); i++)
	{
		letter_positions[i] = clan::Vec3f((float)x, (float)initial_y, 1.0f);
		letter_deltas[i] = clan::Vec3f((rand()%2000-1000)/(float) 20, -(rand()%2000)/(float) 20, (rand()%2000-1000)/(float) 2000);
		x += font.get_text_size(gc, text.substr(i, 1)).width;
	}
	timer.reset();
}

void TextExplosion2D::render(clan::Canvas &canvas)
{
	if (!letter_positions.empty())
	{
		bool still_visible = false;
		float time_elapsed = timer.update();
		for (size_t i = 0; i < letter_positions.size(); i++)
		{
			font.draw_text(canvas, (int)letter_positions[i].x, (int)letter_positions[i].y, text.substr(i, 1), color);

			letter_positions[i] += letter_deltas[i] * time_elapsed * 5.0f;
			letter_deltas[i].y += time_elapsed * 20.0f;
			if (letter_deltas[i].y < canvas.get_height())
				still_visible = true;
		}
		if (!still_visible)
		{
			letter_positions.clear();
			letter_deltas.clear();
		}
	}
}
