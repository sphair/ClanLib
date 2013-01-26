/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "pixel_converter_impl.h"

namespace clan
{

class PixelFilterRGBToYCrCb : public PixelFilter
{
public:
	void filter(Vec4f *pixels, int num_pixels)
	{
		for (int i = 0; i < num_pixels; i++)
		{
			float y = (0.299f * pixels[i].r) + (0.587f * pixels[i].g) + (0.114f * pixels[i].b);
			float cb = 0.5f - (0.168736f * pixels[i].r) - (0.331264f * pixels[i].g) + (0.5f * pixels[i].b);
			float cr = 0.5f + (0.5f * pixels[i].r) - (0.418688f * pixels[i].g) - (0.081312f * pixels[i].b);
			pixels[i] = Vec4f(y, cr, cb, pixels[i].a);
		}
	}
};

class PixelFilterYCrCbToRGB : public PixelFilter
{
public:
	void filter(Vec4f *pixels, int num_pixels)
	{
		for (int i = 0; i < num_pixels; i++)
		{
			// xyz = ycrcb
			float r = pixels[i].x + 1.402f * (pixels[i].y - 0.5f);
			float g = pixels[i].x - 0.34414f * (pixels[i].z - 0.5f) - 0.71414f * (pixels[i].y - 0.5f);
			float b = pixels[i].x + 1.772f * (pixels[i].z - 0.5f);
			pixels[i] = Vec4f(r, g, b, pixels[i].a);
		}
	}
};

}
