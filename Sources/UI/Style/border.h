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
**    Magnus Norddahl
*/

#pragma once

#include "API/Display/2D/color.h"

namespace clan
{
	class ViewBorderValue
	{
	public:
		enum Type
		{
			type_none,
			type_solid
		};
		Type type = type_none;
		float width = 0.0f;
		Colorf color;
	};

	class ViewBorderRadiusCorner
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
	};

	class ViewBorder
	{
	public:
		ViewBorderValue left;
		ViewBorderValue top;
		ViewBorderValue right;
		ViewBorderValue bottom;

		ViewBorderRadiusCorner top_left_radius;
		ViewBorderRadiusCorner top_right_radius;
		ViewBorderRadiusCorner bottom_left_radius;
		ViewBorderRadiusCorner bottom_right_radius;
	};
}
