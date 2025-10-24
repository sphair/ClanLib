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
**    Mark Page
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"
#include "../Collision/collision_outline.h"

/// \brief Base for Collidable Sprite class. Not yet implemented, except for some helper functions
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_CollidableSprite
{
public:
	/// \brief Create the collision outlines from the sprites resource
	///
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	/// \param alpha_limit = Alpha limit for pixels considered solid (collidable)
	/// \param accuracy = Amount of optimization of the outline (default medium)
	/// \return The collision outlines
	static std::vector<CL_CollisionOutline> create_collision_outlines(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium);

	/// \brief Create the collision outlines from the sprites resource
	///
	/// \param resource_id = String Ref
	/// \param resources = Resource Manager
	/// \param alpha_limit = Alpha limit for pixels considered solid (collidable)
	/// \param accuracy = Amount of optimization of the outline (default medium)
	/// \return The collision outlines
	static CL_CollisionOutline create_collision_outline(CL_GraphicContext &gc, const CL_StringRef &resource_id, CL_ResourceManager *resources, int alpha_limit=128, CL_OutlineAccuracy accuracy=accuracy_medium);

};

/// \}
