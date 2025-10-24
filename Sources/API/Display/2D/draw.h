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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Ken Hirsch
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include "../api_display.h"
#include "../Render/graphic_context.h"

class CL_Colorf;
class CL_Pointf;
class CL_Gradient;
class CL_LineSegment2f;
class CL_LineSegment2;
class CL_Quadf;
class CL_Trianglef;
class CL_Triangle;

/// \brief Primitives drawing class.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_Draw
{
/// \name Operations
/// \{

public:
	/// \brief Draw a point.
	static void point(CL_GraphicContext &gc, float x1, float y1, const CL_Colorf &color);

	/// \brief Point
	///
	/// \param gc = Graphic Context
	/// \param point = Pointf
	/// \param color = Colorf
	static void point(CL_GraphicContext &gc, const CL_Pointf &point, const CL_Colorf &color);

	/// \brief Draw a line.
	static void line(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color);

	/// \brief Line
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param color = Colorf
	static void line(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Colorf &color);

	/// \brief Line
	///
	/// \param gc = Graphic Context
	/// \param line_segment = The line
	/// \param color = Colorf
	static void line(CL_GraphicContext &gc, const CL_LineSegment2f &line_segment, const CL_Colorf &color);

	/// \brief Draw a box / rectangle.
	static void box(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color);

	/// \brief Box
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param color = Colorf
	static void box(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Colorf &color);

	/// \brief Box
	///
	/// \param gc = Graphic Context
	/// \param rect = Rectf
	/// \param color = Colorf
	static void box(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color);

	/// \brief Draw a filled box / rectangle.
	static void fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color);

	/// \brief Fill
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param color = Colorf
	static void fill(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Colorf &color);

	/// \brief Fill
	///
	/// \param gc = Graphic Context
	/// \param rect = Rectf
	/// \param color = Colorf
	static void fill(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color);

	/// \brief Draw a textured rectangle with the selected texture at unit 0
	///
	/// Usage: gc.set_texture(0, texture); CL_Draw::texture(gc, rect, ...); gc.reset_texture(0); \n
	/// It is recommended to use CL_Image draw function instead of this function. CL_Image is faster because it internally batch draws
	static void texture(
		CL_GraphicContext &gc,
		const CL_Rectf &rect,
		const CL_Colorf &color = CL_Colorf::white,
		const CL_Rectf &texture_unit1_coords = CL_Rectf(0.0, 0.0, 1.0, 1.0));

	/// \brief Draw a textured rectangle
	///
	/// This is a convenience function. If using repeatedly, it is a lot faster to use CL_PrimitivesArray with gc.draw_primitives instead
	static void texture(
		CL_GraphicContext &gc,
		const CL_Texture &texture,
		const CL_Quadf &quad,
		const CL_Colorf &color = CL_Colorf::white,
		const CL_Rectf &texture_unit1_coords = CL_Rectf(0.0, 0.0, 1.0, 1.0));

	/// \brief Gradient fill
	///
	/// \param gc = Graphic Context
	/// \param x1 = value
	/// \param y1 = value
	/// \param x2 = value
	/// \param y2 = value
	/// \param color = Gradient
	static void gradient_fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Gradient &color);

	/// \brief Gradient fill
	///
	/// \param gc = Graphic Context
	/// \param start = Pointf
	/// \param end = Pointf
	/// \param gradient = Gradient
	static void gradient_fill(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Gradient &gradient);

	/// \brief Gradient fill
	///
	/// \param gc = Graphic Context
	/// \param rect = Rectf
	/// \param gradient = Gradient
	static void gradient_fill(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Gradient &gradient);

	/// \brief Draw a circle.
	static void circle(CL_GraphicContext &gc, float center_x, float center_y, float radius, const CL_Colorf &color);

	/// \brief Circle
	///
	/// \param gc = Graphic Context
	/// \param center = Pointf
	/// \param radius = value
	/// \param color = Colorf
	static void circle(CL_GraphicContext &gc, const CL_Pointf &center, float radius, const CL_Colorf &color);

	/// \brief Gradient circle
	///
	/// \param gc = Graphic Context
	/// \param center = Pointf
	/// \param radius = value
	/// \param gradient = Gradient
	static void gradient_circle(CL_GraphicContext &gc, const CL_Pointf &center, float radius, const CL_Gradient &gradient);

	/// \brief Gradient circle
	///
	/// \param gc = Graphic Context
	/// \param center = Pointf
	/// \param centergradient = Pointf
	/// \param radius = value
	/// \param gradient = Gradient
	static void gradient_circle(CL_GraphicContext &gc, const CL_Pointf &center, const CL_Pointf &centergradient, float radius, const CL_Gradient &gradient);

	/// \brief Draw a triangle.
	static void triangle(CL_GraphicContext &gc, const CL_Pointf &a, const CL_Pointf &b, const CL_Pointf &c, const CL_Colorf &color);

	/// \brief Draw a triangle.
	static void triangle(CL_GraphicContext &gc, const CL_Trianglef &dest_triangle, const CL_Colorf &color);

/// \}
};

/// \}
