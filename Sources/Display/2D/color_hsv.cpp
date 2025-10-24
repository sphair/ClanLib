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
#include "API/Display/2D/color_hsv.h"
#include "API/Display/2D/color.h"

template<>
CL_ColorHSVx<int>::CL_ColorHSVx(const CL_Color &color)
{
	int red = color.get_red();
	int green = color.get_green();
	int blue = color.get_blue();

	const int min_value = cl_min(red,cl_min(blue, green));
	const int max_value = cl_max(red,cl_max(blue, green));
	a = color.get_alpha();
	
	if (min_value == max_value)
	{
		h = 0;
		s = 0;
		v = min_value;
		return;
	}

	s = max_value != 0 ? 255 * (max_value - min_value) / max_value : 0;
	v = max_value;
	
	float r = float(max_value - red) / (max_value - min_value);
	float g = float(max_value - green) / (max_value - min_value);
	float b = float(max_value - blue) / (max_value - min_value);

	if (max_value == red)
	{
		h = (60 * (b - g))+0.5f;
	}
	else if (max_value == green)
	{
		h = (60 * (2 + r - b))+0.5f;
	}
	else
	{
		h = (60 * (4 + g - r))+0.5f;
	}
}

template<>
CL_ColorHSVx<float>::CL_ColorHSVx(const CL_Colorf &color)
{
	float red = color.get_red();
	float green = color.get_green();
	float blue = color.get_blue();

	const float min_value = cl_min(red,cl_min(blue, green));
	const float max_value = cl_max(red,cl_max(blue, green));
	a = color.get_alpha();

	if (min_value == max_value)
	{
		h = 0.0f;
		s = 0.0f;
		v = min_value;
		return;
	}

	s = max_value != 0.0f ? (max_value - min_value) / max_value : 0.0f;
	v = max_value;
	
	float r = float(max_value - red) / (max_value - min_value);
	float g = float(max_value - green) / (max_value - min_value);
	float b = float(max_value - blue) / (max_value - min_value);

	if (max_value == red)
	{
		h = (60.0f * (b - g));
	}
	else if (max_value == green)
	{
		h = (60.0f * (2.0f + r - b));
	}
	else
	{
		h = (60.0f * (4.0f + g - r));
	}
}

template<>
CL_ColorHSVx<double>::CL_ColorHSVx(const CL_Colord &color)
{
	double red = color.get_red();
	double green = color.get_green();
	double blue = color.get_blue();

	const double min_value = cl_min(red,cl_min(blue, green));
	const double max_value = cl_max(red,cl_max(blue, green));
	a = color.get_alpha();

	if (min_value == max_value)
	{
		h = 0.0;
		s = 0.0;
		v = min_value;
		return;
	}

	s = max_value != 0.0 ? (max_value - min_value) / max_value : 0.0;
	v = max_value;
	
	double r = double(max_value - red) / (max_value - min_value);
	double g = double(max_value - green) / (max_value - min_value);
	double b = double(max_value - blue) / (max_value - min_value);

	if (max_value == red)
	{
		h = (60.0 * (b - g));
	}
	else if (max_value == green)
	{
		h = (60.0 * (2.0 + r - b));
	}
	else
	{
		h = (60.0 * (4.0 + g - r));
	}
}


template<>
CL_ColorHSVx<int>::CL_ColorHSVx(const CL_Colorf &color)
{
	CL_ColorHSVx<float> color_hsv_f(color);
	h = (int) color_hsv_f.h + 0.5f;
	s = (int) ((255.0f * color_hsv_f.s) + 0.5f);
	v = (int) ((255.0f * color_hsv_f.v) + 0.5f);
	a = (int) ((255.0f * color_hsv_f.a) + 0.5f);
}

template<>
CL_ColorHSVx<int>::CL_ColorHSVx(const CL_Colord &color)
{
	CL_ColorHSVx<double> color_hsv_d(color);
	h = (int) color_hsv_d.h + 0.5;
	s = (int) ((255.0 * color_hsv_d.s) + 0.5);
	v = (int) ((255.0 * color_hsv_d.v) + 0.5);
	a = (int) ((255.0 * color_hsv_d.a) + 0.5);
}

template<>
CL_ColorHSVx<float>::CL_ColorHSVx(const CL_Color &color)
{
	CL_ColorHSVx<int> color_hsv_i(color);
	h = (float) color_hsv_i.h ;
	s = color_hsv_i.s / 255.0f;
	v = color_hsv_i.v / 255.0f;
	a = color_hsv_i.a / 255.0f;
}

template<>
CL_ColorHSVx<float>::CL_ColorHSVx(const CL_Colord &color)
{
	CL_ColorHSVx<double> color_hsv_d(color);
	h = (float) color_hsv_d.h ;
	s = (float) color_hsv_d.s;
	v = (float) color_hsv_d.v;
	a = (float) color_hsv_d.a;
}

template<>
CL_ColorHSVx<double>::CL_ColorHSVx(const CL_Color &color)
{
	CL_ColorHSVx<int> color_hsv_i(color);
	h = (double) color_hsv_i.h ;
	s = color_hsv_i.s / 255.0;
	v = color_hsv_i.v / 255.0;
	a = color_hsv_i.a / 255.0;
}

template<>
CL_ColorHSVx<double>::CL_ColorHSVx(const CL_Colorf &color)
{
	CL_ColorHSVx<float> color_hsv_f(color);
	h = color_hsv_f.h ;
	s = color_hsv_f.s;
	v = color_hsv_f.v;
	a = color_hsv_f.a;
}

#include "API/Display/2D/color_hsv2.h"

CL_ColorHSVf::operator CL_Colorf()
{
	float hue = cl_min(359.0f,cl_max(0.0f,h));
	float saturation = cl_min(1.0f,cl_max(0.0f,s));	
	float value = cl_min(1.0f,cl_max(0.0f,v));
	if (saturation == 0.0f)
	{
		return CL_Colorf(value, value, value, a);
	}

	int section = (int)(hue / 60);
	float f = (hue / 60.0f) - ((float) section);
	float p = value * (1.0f - saturation);
	float q = value * (1.0f - saturation * f);
	float t = value * (1.0f - saturation * (1.0f - f));
	if (section == 1)
	{
		return CL_Colorf(q, value, p, a);
	}
	if (section == 2)
	{
		return CL_Colorf(p, value, t, a);
	}
	if (section == 3)
	{
		return CL_Colorf(p, q, value, a);
	}
	if (section == 4)
	{
		return CL_Colorf(t, p, value, a);
	}
	if (section == 5)
	{
		return CL_Colorf(value, p, q, a);
	}
	return CL_Colorf(value, t, p, a);
}

CL_ColorHSVd::operator CL_Colord()
{
	double hue = cl_min(359.0,cl_max(0.0,h));
	double saturation = cl_min(1.0,cl_max(0.0,s));	
	double value = cl_min(1.0,cl_max(0.0,v));
	if (saturation == 0.0f)
	{
		return CL_Colord(value, value, value, a);
	}

	int section = (int)(hue / 60);
	double f = (hue / 60.0) - ((double) section);
	double p = value * (1.0 - saturation);
	double q = value * (1.0 - saturation * f);
	double t = value * (1.0 - saturation * (1.0 - f));
	if (section == 1)
	{
		return CL_Colord(q, value, p, a);
	}
	if (section == 2)
	{
		return CL_Colord(p, value, t, a);
	}
	if (section == 3)
	{
		return CL_Colord(p, q, value, a);
	}
	if (section == 4)
	{
		return CL_Colord(t, p, value, a);
	}
	if (section == 5)
	{
		return CL_Colord(value, p, q, a);
	}
	return CL_Colord(value, t, p, a);
}

CL_ColorHSVi::operator CL_Color()
{
	int hue = cl_min(359,cl_max(0,h));
	int saturation = cl_min(255,cl_max(0,s));	
	int value = cl_min(255,cl_max(0,v));
	if (saturation == 0)
	{
		return CL_Color(value, value, value, a);
	}

	int section = (int)(hue / 60);
	int f = ((hue*256) / 60) - (section*256);
	int p = (value * (255 - saturation))/255;
	int q = (value * (255 - ((saturation * f) /256) ))/255;
	int t = (value * (255 - ((saturation * (256 - f)) / 256) ))/255;
	if (section == 1)
	{
		return CL_Color(q, value, p, a);
	}
	if (section == 2)
	{
		return CL_Color(p, value, t, a);
	}
	if (section == 3)
	{
		return CL_Color(p, q, value, a);
	}
	if (section == 4)
	{
		return CL_Color(t, p, value, a);
	}
	if (section == 5)
	{
		return CL_Color(value, p, q, a);
	}
	return CL_Color(value, t, p, a);
}

// Explicit instantiate the versions we use:
template class CL_ColorHSVx<int>;
template class CL_ColorHSVx<float>;
template class CL_ColorHSVx<double>;


