
#pragma once

#include "elapsed_timer.h"

class AgeAscii;

class TextExplosionAscii
{
public:
	TextExplosionAscii();
	void explode(AgeAscii *console, const std::string &text, int x, int y, clan::Colorf color);
	void render(AgeAscii *console);

private:
	int initial_x;
	int initial_y;
	std::string text;
	clan::Colorf color;

	std::vector<clan::Vec3f> letter_positions;
	std::vector<clan::Vec3f> letter_deltas;
	std::vector<wchar_t> letter_symbols;

	ElapsedTimer timer;
};
