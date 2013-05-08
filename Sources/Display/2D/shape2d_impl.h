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


#include "Display/precomp.h"
#include "API/Display/2D/shape2d.h"
#include "API/Display/2D/path2d.h"
#include "shape2d_impl.h"
#include <vector>
#include "API/Display/Render/primitives_array.h"
#include "API/Display/2D/color.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/System/databuffer.h"

namespace clan
{

class GraphicContext;
class Canvas;

class Shape2D_Impl
{
/// \name Construction
/// \{

public:
	Shape2D_Impl();
	virtual ~Shape2D_Impl();


/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

	void add_path(Path2D &path);

	void get_triangles(std::vector<Vec2f> &out_primitives_array, PolygonOrientation polygon_orientation) const;
	void get_outline(std::vector< std::vector<Vec2f> > &out_primitives_array_outline) const;

	static void add_rotated_curve(Path2D &path, const Pointf &center, const Angle &angle, Pointf point_1,  Pointf point_2,  Pointf point_3);


/// \}
/// \name Implementation
/// \{

private:

	std::vector< Path2D > contours;
/// \}
};

}
