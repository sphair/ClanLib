/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "API/UI/View/view.h"

namespace clan
{
	class PositionedLayout
	{
	public:
		static void layout_children(Canvas &canvas, View *view);
		static ViewGeometry get_geometry(Canvas &canvas, View *view, const Rectf &containing_box);

	private:
		static void layout_from_containing_box(Canvas &canvas, View *view, const Rectf &containing_box);
		static float resolve_percentage(const StyleGetValue &computed_value, float size);
	};
}
