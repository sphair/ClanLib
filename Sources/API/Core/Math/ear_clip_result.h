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
#include <vector>

class CL_EarClipResult_Impl;
class CL_EarClipTriangulator_Triangle;

/// \brief Ear clipping triangulation result structure.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_API_CORE CL_EarClipResult
{
/// \name Construction
/// \{

public:
	/// \brief Constructs an ear clipping result structure.
	CL_EarClipResult(int num_triangles);

	virtual ~CL_EarClipResult();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the triangles for this result.
	std::vector<CL_EarClipTriangulator_Triangle> &get_triangles();

	/// \brief return a reference to a triangle in the triangulation.
	/** Memory  is preallocated for all triangles at creation. Use the returned reference to modify the triangles.*/
	CL_EarClipTriangulator_Triangle &get_triangle(int index);

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_EarClipResult_Impl> impl;
/// \}
};

/// \}
