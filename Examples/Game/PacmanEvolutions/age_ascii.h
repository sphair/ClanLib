
#pragma once

class GameWorld;

class AgeAscii
{
public:
	AgeAscii(CL_GraphicContext &gc, GameWorld *game);
	void render(CL_GraphicContext &gc);

	void set_character(int x, int y, wchar_t symbol, const CL_Colorf &color);

private:
	wchar_t get_tile_character(int tile);
	CL_Colorf get_tile_color(int tile);

	GameWorld *game;
	CL_Font font;
	CL_Vec2f look_at;

	std::vector<wchar_t> chars;
	std::vector<CL_Colorf> colors;
};
