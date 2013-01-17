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
/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once


#include "../api_display.h"
#include "path.h"
#include <vector>
#include "../Render/primitives_array.h"
#include "color.h"

namespace clan
{

class GraphicContext;
class Canvas;
class PathGroup_Impl;

typedef std::vector<Vec2f> PathPrimitivesArray;
typedef std::vector< std::vector<Vec2f> > PathPrimitivesArrayOutline;
typedef std::vector<std::vector<Pointf> > PathPrimitivesJoinedOutlines; // for debugging triangulator hole support - don't remove!

class CL_API_DISPLAY PathGroup
{
/// \name Construction
/// \{

public:
	PathGroup();
	virtual ~PathGroup();


/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

	void add_path(Path &path);

	/// \brief triangulate
	void triangulate(PathPrimitivesArray &out_primitives_array);
	void triangulate(PathPrimitivesArrayOutline &out_primitives_array_outline);
	void triangulate(PathPrimitivesArray &out_primitives_array, PathPrimitivesArrayOutline &out_primitives_array_outline);
	void triangulate(PathPrimitivesJoinedOutlines &out_joined_outlines);	// For debugging


/// \}
/// \name Implementation
/// \{

private:

	std::shared_ptr<PathGroup_Impl> impl;


/// \}
};

}
/// \}
