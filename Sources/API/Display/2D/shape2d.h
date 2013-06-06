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
**    Harry Storbacka
**    Mark Page
*/

#pragma once


#include "../api_display.h"
#include "path2d.h"
#include <vector>
#include "../Render/primitives_array.h"
#include "color.h"

namespace clan
{
/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

class GraphicContext;
class Canvas;
class Shape2D_Impl;
class Rectf;

class CL_API_DISPLAY Shape2D
{
/// \name Construction
/// \{

public:
	Shape2D();
	virtual ~Shape2D();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief triangulate
	void get_triangles(std::vector<Vec2f> &out_primitives_array, PolygonOrientation orientation = cl_clockwise) const;

	void get_outline(std::vector< std::vector<Vec2f> > &out_primitives_array_outline) const;

/// \}
/// \name Operations
/// \{

public:

	void add_path(Path2D &path);

	/// \brief Add a circle.
	void add_circle(float center_x, float center_y, float radius, bool reverse = false);

	/// \brief Add a ellispe.
	void add_ellipse(float center_x, float center_y, float radius_x, float radius_y, bool reverse = false);

	/// \brief Add a Circle
	///
	/// \param center = Pointf
	/// \param radius = value
	void add_circle(const Pointf &center, float radius_x, bool reverse = false);

	/// \brief Add a Circle
	///
	/// \param center = Pointf
	/// \param radius = value
	void add_ellipse(const Pointf &center, const Pointf &radius, bool reverse = false);

	void add_rounded_rect(const Pointf &origin, const Sizef &size, float cap_rounding, bool reverse = false);

	void add_rounded_rect(const Pointf &origin, const Sizef &size, float cap_rounding, const Angle &angle, bool reverse = false);

	void add_rect(const Rectf &rect, bool reverse = false);

	void add_rect(const Rectf &rect, const Angle &angle, bool reverse = false);

	void add_rounded_line(const Pointf &start, const Pointf &end, float line_width, float cap_rounding, bool reverse);

/// \}
/// \name Implementation
/// \{

private:

	std::shared_ptr<Shape2D_Impl> impl;


/// \}
};

}
/// \}
