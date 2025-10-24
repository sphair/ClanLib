/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_triangle_params
#define header_triangle_params

class CL_Surface_Target;
class CL_Color;

struct CL_TriangleParams
{
	CL_TriangleParams() :
		count(0),
		fill(true),
		color_range_zero2one(false),
		uv_range_zero2one(false),
		vertices(0),
		color(0),
		uchar_color(0),
		uv(0),
		int_uv(0),
		texture(0),
		single_color(0)
	{
		return;
	}

	unsigned int count;
	bool fill;
	bool color_range_zero2one;
	bool uv_range_zero2one;
	double *vertices;
	double *color;
	unsigned char *uchar_color;
	double *uv;
	int *int_uv;
	CL_Surface_Target *texture;
	const CL_Color *single_color;
};

#endif
