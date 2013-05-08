/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#pragma once

#include "API/SWRender/pixel_command.h"
#include "API/Core/Math/vec2.h"
#include "API/Core/Math/vec4.h"
#include "API/Core/Math/rect.h"

namespace clan
{


class PixelCommandSprite : public PixelCommand
{
public:
	PixelCommandSprite(const Vec2f init_points[3], const Vec4f init_primcolor, const Vec2f init_texcoords[3], int init_sampler);
	void run(PixelThreadContext *context);

private:
	struct Scanline
	{
		Scanline() : dest(0), src(0), src_width(0), src_height(0), x1(0.0f), x2(0.0f), tx1(0.0f), tx2(0.0f), ty1(0.0f), ty2(0.0f), startx(0), endx(0) { }

		unsigned int *dest;
		unsigned int *src;
		unsigned int src_width;
		unsigned int src_height;
		float x1, x2;
		float w1, w2;
		float tx1, tx2;
		float ty1, ty2;
		int startx, endx;
	};

	void render_sprite(PixelThreadContext *context);
	void render_sprite_rotated(PixelThreadContext *context);
	void render_sprite_scale(PixelThreadContext *context, const Rect &box);
	void render_sprite_scale_linear(PixelThreadContext *context, const Rect &box);
	void render_sprite_noscale(PixelThreadContext *context, const Rect &box);
	void render_sprite_noscale_white(PixelThreadContext *context, const Rect &box);
	void render_glyph_scale(PixelThreadContext *context, const Rect &box);
	void render_glyph_noscale(PixelThreadContext *context, const Rect &box);
	Rect get_dest_rect(PixelThreadContext *context) const;

	void render_linear_scanline(Scanline *d);

	Vec2f points[3];
	Vec4f primcolor;
	Vec2f texcoords[3];
	int sampler;
};

}
