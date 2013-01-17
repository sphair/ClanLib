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
**    Harry Storbacka
**    Mark Page
*/

#pragma once


#include <vector>
#include "API/Core/Math/point.h"
#include "API/Core/Math/ear_clip_triangulator.h"

namespace clan
{

class BezierCurve;

class Path_Impl
{
/// \name Construction
/// \{

public:
	Path_Impl();

	virtual ~Path_Impl();


/// \}
/// \name Attributes
/// \{

public:

	bool is_hole();

	bool is_inside_contour(const Path &other) const;

	bool is_point_inside(const Pointf &point) const;

	const std::vector<Pointf> &get_contour_points() { return contour_points; }



/// \}
/// \name Operations
/// \{

public:

	void add_curve(BezierCurve &);

	void add_line_to(const Pointf &p);


/// \}
/// \name Implementation
/// \{

private:

	PolygonOrientation get_orientation();

	std::vector<Pointf> contour_points;

	bool holeness_cached;
	bool hole;
/// \}
};

}
