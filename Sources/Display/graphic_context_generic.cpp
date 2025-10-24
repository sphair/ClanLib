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

#include "Display/display_precomp.h"
#include "graphic_context_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_Generic construction:

CL_GraphicContext_Generic::CL_GraphicContext_Generic()
: ref_count(0)
{
	cliprects.push_front(CL_Rect(0,0,0,0));
	modelviews.push_front(CL_Matrix4x4(true));
}

CL_GraphicContext_Generic::~CL_GraphicContext_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_Generic operations:

void CL_GraphicContext_Generic::add_ref()
{
	ref_count++;
}

void CL_GraphicContext_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

void CL_GraphicContext_Generic::set_cliprect(const CL_Rect &rect)
{
	cliprects.front() = rect;
}

void CL_GraphicContext_Generic::push_cliprect(const CL_Rect &rect)
{
	cliprects.push_front(cliprects.front().calc_union(rect));
}

void CL_GraphicContext_Generic::push_cliprect()
{
	cliprects.push_front(cliprects.front());
}

void CL_GraphicContext_Generic::pop_cliprect()
{
	cliprects.pop_front();
}

void CL_GraphicContext_Generic::set_modelview(const CL_Matrix4x4 &matrix)
{
	modelviews.front() = matrix;
	modelview_changed();
}

void CL_GraphicContext_Generic::add_modelview(const CL_Matrix4x4 &matrix)
{
	set_modelview(modelviews.front().multiply(matrix));
}

void CL_GraphicContext_Generic::push_modelview()
{
	CL_Matrix4x4 m = modelviews.front();
	modelviews.push_front(m);
}

void CL_GraphicContext_Generic::set_translate(double x, double y, double z)
{
	CL_Matrix4x4 matrix(true);
	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	set_modelview(matrix);
}

void CL_GraphicContext_Generic::add_translate(double x, double y, double z)
{
	CL_Matrix4x4 matrix(true);
	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	add_modelview(matrix);
}

void CL_GraphicContext_Generic::push_translate(double x, double y, double z)
{
	push_modelview();
	add_translate(x, y, z);
}

void CL_GraphicContext_Generic::set_rotate(double angle, double x, double y, double z)
{
	double len2 = x*x+y*y+z*z;
	if (len2 != 1.0)
	{
		double len = sqrt(len2);
		x /= len;
		y /= len;
		z /= len;
	}

	double c = cos(angle*3.14159265/180);
	double s = sin(angle*3.14159265/180);

	CL_Matrix4x4 matrix(true);
	matrix[0] = x*x*(1-c)+c;
	matrix[1] = y*x*(1-c)+z*s;
	matrix[2] = x*z*(1-c)-y*s;

	matrix[4] = x*y*(1-c)-z*s;
	matrix[5] = y*y*(1-c)+c;
	matrix[6] = y*z*(1-c)+x*s;

	matrix[8] = x*z*(1-c)+y*s;
	matrix[9] = y*z*(1-c)-x*s;
	matrix[10] = z*z*(1-c)+c;

	set_modelview(matrix);
}

void CL_GraphicContext_Generic::add_rotate(double angle, double x, double y, double z)
{
	double len2 = x*x+y*y+z*z;
	if (len2 != 1.0)
	{
		double len = sqrt(len2);
		x /= len;
		y /= len;
		z /= len;
	}

	double c = cos(angle*3.14159265/180);
	double s = sin(angle*3.14159265/180);

	CL_Matrix4x4 matrix(true);
	matrix[0] = x*x*(1-c)+c;
	matrix[1] = y*x*(1-c)+z*s;
	matrix[2] = x*z*(1-c)-y*s;

	matrix[4] = x*y*(1-c)-z*s;
	matrix[5] = y*y*(1-c)+c;
	matrix[6] = y*z*(1-c)+x*s;

	matrix[8] = x*z*(1-c)+y*s;
	matrix[9] = y*z*(1-c)-x*s;
	matrix[10] = z*z*(1-c)+c;

	add_modelview(matrix);
}

void CL_GraphicContext_Generic::push_rotate(double angle, double x, double y, double z)
{
	push_modelview();
	add_rotate(angle, x, y, z);
}

void CL_GraphicContext_Generic::set_scale(double x, double y, double z)
{
	CL_Matrix4x4 matrix(true);
	matrix[0] = x;
	matrix[5] = y;
	matrix[10] = z;
	set_modelview(matrix);
}

void CL_GraphicContext_Generic::add_scale(double x, double y, double z)
{
	CL_Matrix4x4 matrix(true);
	matrix[0] = x;
	matrix[5] = y;
	matrix[10] = z;
	add_modelview(matrix);
}

void CL_GraphicContext_Generic::push_scale(double x, double y, double z)
{
	push_modelview();
	add_scale(x, y, z);
}

void CL_GraphicContext_Generic::pop_modelview()
{
	modelviews.pop_front();
	if (modelviews.empty())
	{
		modelviews.push_front(CL_Matrix4x4(true));
	}
	modelview_changed();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_Generic implementation:
