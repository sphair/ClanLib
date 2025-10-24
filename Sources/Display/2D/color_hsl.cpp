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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Core/Math/cl_math.h"
#include "API/Display/2D/color_hsl.h"
#include "API/Display/2D/color.h"

template<>
CL_ColorHSLx<float>::CL_ColorHSLx(const CL_Colorf &color)
{
	float red = color.get_red();
	float green = color.get_green();
	float blue = color.get_blue();

	const float min_value = cl_min(red,cl_min(blue, green));
	const float max_value = cl_max(red,cl_max(blue, green));
	a = color.get_alpha();
	l = (max_value + min_value)/2.0f;

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
		s = (max_value - min_value) / (2.0f-(min_value + max_value));
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
CL_ColorHSLx<int>::CL_ColorHSLx(const CL_Color &color)
{
	CL_Colorf color_f(color);
	CL_ColorHSLx<float> hsl_f(color_f);
	h = (int) (hsl_f.h + 0.5f);
	s = (int) ((hsl_f.s * 255.0f) + 0.5f);
	l = (int) ((hsl_f.l * 255.0f) + 0.5f);
	a = (int) ((hsl_f.a * 255.0f) + 0.5f);
}

template<>
CL_ColorHSLx<double>::CL_ColorHSLx(const CL_Colord &color)
{
	double red = color.get_red();
	double green = color.get_green();
	double blue = color.get_blue();

	const double min_value = cl_min(red,cl_min(blue, green));
	const double max_value = cl_max(red,cl_max(blue, green));
	a = color.get_alpha();
	l = (max_value + min_value)/2.0;

	if (min_value == max_value)
	{
		h = 0.0;
		s = 0.0;
		return;
	}

	if (l <= 0.5)
	{
		s = (max_value - min_value) / (min_value + max_value);
	}
	else
	{
		s = (max_value - min_value) / (2.0-(min_value + max_value));
	}

	if (max_value == red)
	{
		h = (60.0 * (green - blue)) / (max_value - min_value);
	}
	else if (max_value == green)
	{
		h = 120.0 + (60.0 * (blue - red)) / (max_value - min_value);
	}
	else
	{
		h = 240.0 + (60.0 * (red - green)) / (max_value - min_value);
	}
}


template<>
CL_ColorHSLx<int>::CL_ColorHSLx(const CL_Colorf &color)
{
	CL_ColorHSLx<float> color_hsl_f(color);
	h = (int) color_hsl_f.h + 0.5f;
	s = (int) ((255.0f * color_hsl_f.s) + 0.5f);
	l = (int) ((255.0f * color_hsl_f.l) + 0.5f);
	a = (int) ((255.0f * color_hsl_f.a) + 0.5f);
}

template<>
CL_ColorHSLx<int>::CL_ColorHSLx(const CL_Colord &color)
{
	CL_ColorHSLx<double> color_hsl_d(color);
	h = (int) color_hsl_d.h + 0.5;
	s = (int) ((255.0 * color_hsl_d.s) + 0.5);
	l = (int) ((255.0 * color_hsl_d.l) + 0.5);
	a = (int) ((255.0 * color_hsl_d.a) + 0.5);
}

template<>
CL_ColorHSLx<float>::CL_ColorHSLx(const CL_Color &color)
{
	CL_ColorHSLx<int> color_hsl_i(color);
	h = (float) color_hsl_i.h ;
	s = color_hsl_i.s / 255.0f;
	l = color_hsl_i.l / 255.0f;
	a = color_hsl_i.a / 255.0f;
}

template<>
CL_ColorHSLx<float>::CL_ColorHSLx(const CL_Colord &color)
{
	CL_ColorHSLx<double> color_hsl_d(color);
	h = (float) color_hsl_d.h ;
	s = (float) color_hsl_d.s;
	l = (float) color_hsl_d.l;
	a = (float) color_hsl_d.a;
}

template<>
CL_ColorHSLx<double>::CL_ColorHSLx(const CL_Color &color)
{
	CL_ColorHSLx<int> color_hsl_i(color);
	h = (double) color_hsl_i.h ;
	s = color_hsl_i.s / 255.0;
	l = color_hsl_i.l / 255.0;
	a = color_hsl_i.a / 255.0;
}

template<>
CL_ColorHSLx<double>::CL_ColorHSLx(const CL_Colorf &color)
{
	CL_ColorHSLx<float> color_hsl_f(color);
	h = color_hsl_f.h ;
	s = color_hsl_f.s;
	l = color_hsl_f.l;
	a = color_hsl_f.a;
}

#include "API/Display/2D/color_hsl2.h"

CL_ColorHSLf::operator CL_Colorf()
{
	float hue = cl_min(359.0f,cl_max(0.0f,h)) / 360.0f;
	float saturation = cl_min(1.0f,cl_max(0.0f,s));	
	float lightness = cl_min(1.0f,cl_max(0.0f,l));
	if (saturation == 0.0f)
	{
		return CL_Colorf(lightness, lightness, lightness, a);
	}

	float q;
	if (lightness < 0.5f)
	{
		q = lightness * (1+saturation);
	}
	else
	{
		q = lightness + saturation - lightness * saturation;
	}
	float p = 2.0f * lightness - q;
	float temp_rgb[3];
	temp_rgb[0]=hue + 1.0f / 3.0f;
	temp_rgb[1]=hue;
	temp_rgb[2]=hue - 1.0f / 3.0f;
	for (int i=0;i<3;i++)
	{
		while(temp_rgb[i] < 0.0f)
		{
			temp_rgb[i] += 1.0f;
		}
		while(temp_rgb[i] > 1.0f)
		{
			temp_rgb[i] -= 1.0f;
		}

		if (temp_rgb[i] < (1.0f/6.0f))
		{
			temp_rgb[i] = p + (q - p) * 6.0f * temp_rgb[i];
		}
		else if (temp_rgb[i] < 0.5f)
		{
			temp_rgb[i] = q;
		}
		else if (temp_rgb[i] < (2.0f/3.0f))
		{
			temp_rgb[i] = p + (q - p) * 6.0f * ((2.0f/3.0f) - temp_rgb[i]);
		}
		else
		{
			temp_rgb[i] = p;
		}
	}
	return CL_Colorf(temp_rgb[0], temp_rgb[1], temp_rgb[2], a);
}

CL_ColorHSLd::operator CL_Colord()
{
	double hue = cl_min(359.0,cl_max(0.0,h)) / 360.0;
	double saturation = cl_min(1.0,cl_max(0.0,s));	
	double lightness = cl_min(1.0,cl_max(0.0,l));
	if (saturation == 0.0)
	{
		return CL_Colord(lightness, lightness, lightness, a);
	}

	double q;
	if (lightness < 0.5)
	{
		q = lightness * (1+saturation);
	}
	else
	{
		q = lightness + saturation - lightness * saturation;
	}
	double p = 2.0 * lightness - q;
	double temp_rgb[3];
	temp_rgb[0]=hue + 1.0 / 3.0;
	temp_rgb[1]=hue;
	temp_rgb[2]=hue - 1.0 / 3.0;
	for (int i=0;i<3;i++)
	{
		while(temp_rgb[i] < 0.0)
		{
			temp_rgb[i] += 1.0;
		}
		while(temp_rgb[i] > 1.0)
		{
			temp_rgb[i] -= 1.0;
		}

		if (temp_rgb[i] < 1.0/6.0)
		{
			temp_rgb[i] = p + (q - p) * 6.0 * temp_rgb[i];
		}
		else if (temp_rgb[i] < 0.5)
		{
			temp_rgb[i] = q;
		}
		else if (temp_rgb[i] < 2.0/3.0)
		{
			temp_rgb[i] = p + (q - p) * 6.0 * (2.0/3.0 - temp_rgb[i]);
		}
		else
		{
			temp_rgb[i] = p;
		}
	}
	return CL_Colord(temp_rgb[0], temp_rgb[1], temp_rgb[2], a);

}

CL_ColorHSLi::operator CL_Color()
{
	CL_ColorHSLf hslf((float) h, (float) s / 255.0f, (float) l / 255.0f, (float) a / 255.0f);
	CL_Colorf colorf(hslf);
	return colorf;
}

// Explicit instantiate the versions we use:
template class CL_ColorHSLx<int>;
template class CL_ColorHSLx<float>;
template class CL_ColorHSLx<double>;

