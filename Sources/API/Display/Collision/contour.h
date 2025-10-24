/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Magnus Norddahl
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_Collision clanDisplay Collision
/// \{

#pragma once

#include "../api_display.h"
#include <vector>
#include "outline_circle.h"

class CL_Contour_Impl
{
public:
	CL_Contour_Impl() : is_inside_contour(false) {};

	std::vector<CL_Pointf> points;

	bool is_inside_contour;

	std::vector<CL_OutlineCircle> sub_circles;
};

/// \brief Collision detection contour.
///
/// <p>A outline consists of contours, which hold the actual outline geometry</p>
/// \xmlonly !group=Display/Collision! !header=display.h! \endxmlonly
class CL_Contour
{
/// \name Construction
/// \{
public:
	/// \brief Construct a contour
	CL_Contour() : impl(new CL_Contour_Impl()) {};
	~CL_Contour() {};

/// \}
/// \name Attributes
/// \{
public:
	// Points forming the countour.
	std::vector<CL_Pointf> &get_points() { return impl->points; }
	const std::vector<CL_Pointf> &get_points() const { return impl->points; }

	// boolean specifying if this contour is inside-out (the inside of a hollow polygon)
	// if that is the case, then the collision-test will skip the inside_contour-test (because you can
	// be inside this one, without causing a collision)
	bool is_inside_contour() const { return impl->is_inside_contour; }

	/// \brief Set inside contour
	///
	/// \param is_inside = bool
	void set_inside_contour(bool is_inside) { impl->is_inside_contour = is_inside; }

	// Circles encapsulating a part of the outline.
	// If two circles arent intersecting, none of the lines inside them
	// collide either.
	std::vector<CL_OutlineCircle> &get_sub_circles() { return impl->sub_circles; }
	const std::vector<CL_OutlineCircle> &get_sub_circles() const { return impl->sub_circles; }
/// \}

/// \name Operators
/// \{
public:
	/// \brief Equality operator
	bool operator==(const CL_Contour &other) const { return impl==other.impl; }

	/// \brief Inequality operator
	bool operator!=(const CL_Contour &other) const { return impl!=other.impl; }

	/// \brief Less than operator
	bool operator<(const CL_Contour &other) const { return impl < other.impl; }
/// \}
	
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_Contour_Impl> impl;
/// \}
};

/// \}
