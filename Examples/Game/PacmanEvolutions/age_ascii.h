
#pragma once

class GameWorld;

class AgeAscii
{
public:
	AgeAscii(clan::Canvas &canvas, GameWorld *game);
	void render(clan::Canvas &canvas);

	void set_character(int x, int y, wchar_t symbol, const clan::Colorf &color);

private:
	wchar_t get_tile_character(int tile);
	clan::Colorf get_tile_color(int tile);

	GameWorld *game;
	clan::Font font;
	clan::Vec2f look_at;

	std::vector<wchar_t> chars;
	std::vector<clan::Colorf> colors;
};
