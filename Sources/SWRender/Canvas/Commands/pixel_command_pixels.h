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
*/

#pragma once

#include "API/SWRender/pixel_command.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"

namespace clan
{

class PixelCommandPixels : public PixelCommand
{
public:
	PixelCommandPixels(const Rect &dest_rect, const PixelBuffer &image, const Rect &src_rect, const Colorf &primary_color);
	void run(PixelThreadContext *context);

private:
	void render_pixels_scale(PixelThreadContext *context, const Rect &box);
	void render_pixels_noscale(PixelThreadContext *context, const Rect &box);
	void render_pixels_noscale_white(PixelThreadContext *context, const Rect &box);
	Rect get_clipped_dest_rect(PixelThreadContext *context) const;

	Rect dest_rect;
	PixelBuffer image;
	Rect src_rect;
	Colorf primary_color;
};

}
