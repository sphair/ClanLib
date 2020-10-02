/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Math/cl_math.h"
#include "API/Display/2D/color_hsl.h"
#include "API/Display/2D/color.h"

namespace clan
{
	template<>
	ColorHSLx<float, Colorf>::ColorHSLx(const Colorf &color)
	{
		float red = color.get_red();
		float green = color.get_green();
		float blue = color.get_blue();

		const float min_value = min(red, min(blue, green));
		const float max_value = max(red, max(blue, green));
		a = color.get_alpha();
		l = (max_value + min_value) / 2.0f;

		if (min_value == max_value)
		{
			h = 0.0f;
			s = 0.0f;
			return;
		}

		if (l <= 0.5f)
		{
			s = (max_value - min_value) / (min_value + max_value);
		}
		else
		{
			s = (max_value - min_value) / (2.0f - (min_value + max_value));
		}

		if (max_value == red)
		{
			h = (60.0f * (green - blue)) / (max_value - min_value);
		}
		else if (max_value == green)
		{
			h = 120.0f + (60.0f * (blue - red)) / (max_value - min_value);
		}
		else
		{
			h = 240.0f + (60.0f * (red - green)) / (max_value - min_value);
		}
	}

	template<>
	ColorHSLx<int, Color>::ColorHSLx(const Color &color)
	{
		Colorf color_f(color);
		ColorHSLx<float, Colorf> hsl_f(color_f);
		h = (int)(hsl_f.h + 0.5f);
		s = (int)((hsl_f.s * 255.0f) + 0.5f);
		l = (int)((hsl_f.l * 255.0f) + 0.5f);
		a = (int)((hsl_f.a * 255.0f) + 0.5f);
	}

	template<>
	ColorHSLx<int, Color>::ColorHSLx(const Colorf &color)
	{
		ColorHSLx<float, Colorf> color_hsl_f(color);
		h = (int)color_hsl_f.h + 0.5f;
		s = (int)((255.0f * color_hsl_f.s) + 0.5f);
		l = (int)((255.0f * color_hsl_f.l) + 0.5f);
		a = (int)((255.0f * color_hsl_f.a) + 0.5f);
	}

	template<>
	ColorHSLx<float, Colorf>::ColorHSLx(const Color &color)
	{
		ColorHSLx<int, Color> color_hsl_i(color);
		h = (float)color_hsl_i.h;
		s = color_hsl_i.s / 255.0f;
		l = color_hsl_i.l / 255.0f;
		a = color_hsl_i.a / 255.0f;
	}

	template<>
	ColorHSLx<double, Colorf>::ColorHSLx(const Color &color)
	{
		ColorHSLx<int, Color> color_hsl_i(color);
		h = (double)color_hsl_i.h;
		s = color_hsl_i.s / 255.0;
		l = color_hsl_i.l / 255.0;
		a = color_hsl_i.a / 255.0;
	}

	template<>
	ColorHSLx<double, Colorf>::ColorHSLx(const Colorf &color)
	{
		ColorHSLx<float, Colorf> color_hsl_f(color);
		h = color_hsl_f.h;
		s = color_hsl_f.s;
		l = color_hsl_f.l;
		a = color_hsl_f.a;
	}

	template<>
	ColorHSLx<float, Colorf>::operator Colorf()
	{
		float hue = min(359.0f, max(0.0f, h)) / 360.0f;
		float saturation = min(1.0f, max(0.0f, s));
		float lightness = min(1.0f, max(0.0f, l));
		if (saturation == 0.0f)
		{
			return Colorf(lightness, lightness, lightness, a);
		}

		float q;
		if (lightness < 0.5f)
		{
			q = lightness * (1 + saturation);
		}
		else
		{
			q = lightness + saturation - lightness * saturation;
		}
		float p = 2.0f * lightness - q;
		float temp_rgb[3];
		temp_rgb[0] = hue + 1.0f / 3.0f;
		temp_rgb[1] = hue;
		temp_rgb[2] = hue - 1.0f / 3.0f;
		for (auto & elem : temp_rgb)
		{
			while (elem < 0.0f)
			{
				elem += 1.0f;
			}
			while (elem > 1.0f)
			{
				elem -= 1.0f;
			}

			if (elem < (1.0f / 6.0f))
			{
				elem = p + (q - p) * 6.0f * elem;
			}
			else if (elem < 0.5f)
			{
				elem = q;
			}
			else if (elem < (2.0f / 3.0f))
			{
				elem = p + (q - p) * 6.0f * ((2.0f / 3.0f) - elem);
			}
			else
			{
				elem = p;
			}
		}
		return Colorf(temp_rgb[0], temp_rgb[1], temp_rgb[2], a);
	}

	template<>
	ColorHSLx<int, Color>::operator Color()
	{
		ColorHSLf hslf((float)h, (float)s / 255.0f, (float)l / 255.0f, (float)a / 255.0f);
		Colorf colorf(hslf);
		return colorf;
	}

	// Explicit instantiate the versions we use:
	template class ColorHSLx<int, Color>;
	template class ColorHSLx<float, Colorf>;
}
