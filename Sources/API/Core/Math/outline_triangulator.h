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
**    Magnus Norddahl
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_OutlineTriangulator_Generic;

/// \brief Polygon Tesselator.
///
/// This class uses constrained delauney triangulation to convert polygon outlines into triangles.
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_API_CORE CL_OutlineTriangulator
{
/// \name Construction
/// \{

public:
	/// \brief Creates a tessellation object.
	CL_OutlineTriangulator();

	virtual ~CL_OutlineTriangulator();

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	/// \brief This function specifies a vertex on a polygon.
	void add_vertex(float x, float y, void *data);

	/// \brief Mark next contour in polygon path.
	void next_contour();

	/// \brief Mark next polygon.
	void next_polygon();

	/// \brief Converts passed polygons into triangles.
	void generate();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_OutlineTriangulator_Generic> impl;
/// \}
};

/// \}
