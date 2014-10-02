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
#include "API/Core/Math/point.h"
#include "API/Display/Image/pixel_buffer.h"
#include <vector>

namespace clan
{
	class ViewGradientStop
	{
	public:
		ViewGradientStop() {}
		ViewGradientStop(const Colorf &color, float position) : color(color), position(position) { }

		Colorf color;
		float position = 0.0f;
	};

	class ViewBackground
	{
	public:
		// Solid color
		Colorf color = Colorf(0.0f, 0.0f, 0.0f, 0.0f);

		// Linear gradient
		std::vector<ViewGradientStop> stops;
		float angle = 0.0f;

		// Image
		PixelBuffer image;

		// Box shadow:
		bool shadow_inset = false;
		Pointf shadow_offset;
		float shadow_blur_radius = 0.0f;
		float shadow_spread_distance = 0.0f;
		Colorf shadow_color;
	};
}
