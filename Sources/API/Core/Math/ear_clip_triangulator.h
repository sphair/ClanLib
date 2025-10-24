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
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "../Math/point.h"
#include <vector>

/// \brief CL_EarClipTriangulator_Triangle
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_EarClipTriangulator_Triangle
{
public:
	float x1,y1,x2,y2,x3,y3;
};

class CL_EarClipTriangulator_Impl;
class CL_EarClipResult;

/// \brief Polygon orientations.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
enum CL_PolygonOrientation
{
	cl_clockwise,
	cl_counter_clockwise
};

/// \brief Ear-clipping triangulator.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_EarClipTriangulator
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a triangulator.
	CL_EarClipTriangulator();

	virtual ~CL_EarClipTriangulator();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns a list of the vertices added to the triangulator.
	std::vector<CL_Pointf> get_vertices();

	/// \brief Returns the number of vertices in the triangulator.
	int get_vertice_count();

	/// \brief Determine the orientation of the vertices in the triangulator.
	CL_PolygonOrientation calculate_polygon_orientation();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Add the next vertex of the polygon to the triangulator.
	void add_vertex(float x, float y);

	/// \brief Add vertex
	///
	/// \param p = Pointf
	void add_vertex(const CL_Pointf &p);

	/// \brief Remove all vertices from triangulator.
	void clear();

	/// \brief Set polygon orientation.
	void set_orientation( CL_PolygonOrientation orientation );

	/// \brief Perform triangulation.
	CL_EarClipResult triangulate();

	/// \brief Mark beginning of a polygon hole.
	void begin_hole();

	/// \brief Mark ending of a polygon hole.
	void end_hole();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_EarClipTriangulator_Impl> impl;
/// \}
};

/// \}
