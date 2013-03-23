/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "API/Core/Math/rect.h"
#include "API/Display/Render/blend_mode.h"

namespace clan
{

class LineSegment2;
class Colorf;

class PixelLineRenderer
{
public:
	PixelLineRenderer();

	void draw_line(const LineSegment2 &line_dest, const Colorf &primary_color);
	void set_clip_rect(const Rect &clip_rect);
	void set_dest(unsigned int *data, int width, int height);
	void set_core(int core, int num_cores);
	void set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha);

private:
	int find_first_line_for_core(int y_start, int core, int num_cores);

private:

	unsigned int *dest;
	int dest_width;
	int dest_height;

	Rect clip_rect;
	int core;
	int num_cores;

};

}
