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


#include "Display/precomp.h"
#include "glyph_contour.h"
#include <vector>
#include "API/Display/Render/primitives_array.h"
#include "API/Display/2D/color.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/System/databuffer.h"

namespace clan
{

class GraphicContext;
class GlyphPrimitivesArray;
class GlyphPrimitivesArrayOutline;
class Canvas;
class GlyphOutline_Impl;

class GlyphPrimitivesArray
{
//! Construction:
public:

	/// \brief Constructs a GlyphPrimitivesArray
	///
	/// \param num_triangles = value
	GlyphPrimitivesArray(int num_triangles)
	{
		int size = num_triangles*3;
		vertex.resize(size);
	}

	~GlyphPrimitivesArray()
	{
	}

	std::vector<Vec2f> vertex;
};

class GlyphPrimitivesArrayOutline
{
//! Construction:
public:

	/// \brief Constructs a GlyphPrimitivesArrayOutline
	///
	/// \param num_lines = value
	GlyphPrimitivesArrayOutline(int num_lines)
	{
		vertex.resize(1);
		vertex[0].resize(num_lines+1);
	}
	
//! Operations:
public:

	/// \brief New subarray
	///
	/// \param num_lines = value
	void new_subarray(int num_lines)
	{
		std::vector<std::vector<Vec2f> >::size_type size = vertex.size();
		vertex.resize(size+1);

		vertex[size].resize(num_lines+1);
	}

	std::vector< std::vector<Vec2f> > vertex;
};

class GlyphOutline
{
/// \name Construction
/// \{

public:
	GlyphOutline();
	virtual ~GlyphOutline();


/// \}
/// \name Attributes
/// \{

public:
	GlyphPrimitivesArray &get_triarray();

	GlyphPrimitivesArrayOutline &get_outline();

	std::vector<std::vector<Pointf> > joined_outlines; // for debugging triangulator hole support - don't remove!

/// \}
/// \name Operations
/// \{

public:

	void add_contour(GlyphContour &contour);

	void triangulate();

	void draw_debug_outline(Canvas &canvas);


/// \}
/// \name Implementation
/// \{

private:

	std::shared_ptr<GlyphOutline_Impl> impl;


/// \}
};

}
