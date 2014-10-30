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

#include "box_layout.h"
#include "box_position.h"
#include "box_flex.h"
#include "box_margin.h"
#include "box_border.h"
#include "box_padding.h"
#include "box_content.h"
#include "box_background.h"
#include <functional>

namespace clan
{
	class Canvas;
	class ViewGeometry;

	class BoxStyleImpl
	{
	public:
		BoxStyleImpl &operator =(const BoxStyleImpl &that)
		{
			layout = that.layout;
			position = that.position;
			flex = that.flex;
			margin = that.margin;
			border = that.border;
			padding = that.padding;
			content = that.content;
			background = that.background;
			if (style_changed) style_changed();
			return *this;
		}

		BoxLayout layout = BoxLayout::block;
		BoxPosition position;
		BoxFlex flex;
		BoxMargin margin;
		BoxBorder border;
		BoxPadding padding;
		BoxContent content;
		BoxBackground background;
		std::function<void()> style_changed;

		void render(Canvas &canvas, const ViewGeometry &geometry) const;

	private:
		static float mix(float a, float b, float t);
	};
}