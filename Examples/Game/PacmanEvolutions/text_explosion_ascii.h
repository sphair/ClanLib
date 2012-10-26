
#pragma once

#include "elapsed_timer.h"

class AgeAscii;

class TextExplosionAscii
{
public:
	TextExplosionAscii();
	void explode(AgeAscii *console, const CL_String &text, int x, int y, CL_Colorf color);
	void render(AgeAscii *console);

private:
	int initial_x;
	int initial_y;
	CL_String text;
	CL_Colorf color;

	std::vector<CL_Vec3f> letter_positions;
	std::vector<CL_Vec3f> letter_deltas;
	std::vector<wchar_t> letter_symbols;

	ElapsedTimer timer;
};
