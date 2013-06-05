
#pragma once

class TextExplosion2D
{
public:
	TextExplosion2D();
	void explode(clan::GraphicContext &gc, const std::string &text, int x, int y, clan::Font font, clan::Colorf color);
	void render(clan::Canvas &canvas);

private:
	clan::Font font;
	int initial_x;
	int initial_y;
	std::string text;
	clan::Colorf color;

	std::vector<clan::Vec3f> letter_positions;
	std::vector<clan::Vec3f> letter_deltas;

	clan::GameTime timer;
};
