
#pragma once

#include "elapsed_timer.h"

class TextExplosion2D
{
public:
	TextExplosion2D();
	void explode(CL_GraphicContext &gc, const CL_String &text, int x, int y, CL_Font font, CL_Colorf color);
	void render(CL_GraphicContext &gc);

private:
	CL_Font font;
	int initial_x;
	int initial_y;
	CL_String text;
	CL_Colorf color;

	std::vector<CL_Vec3f> letter_positions;
	std::vector<CL_Vec3f> letter_deltas;

	ElapsedTimer timer;
};
