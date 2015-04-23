/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "../../Core/Math/rect.h"

namespace clan
{
	class StyleCascade;

	class ViewGeometry
	{
	public:
		ViewGeometry() { }
		ViewGeometry(const StyleCascade &style);

		static ViewGeometry from_margin_box(const StyleCascade &style, const Rectf &box);
		static ViewGeometry from_border_box(const StyleCascade &style, const Rectf &box);
		static ViewGeometry from_padding_box(const StyleCascade &style, const Rectf &box);
		static ViewGeometry from_content_box(const StyleCascade &style, const Rectf &box);

		Rectf margin_box() const;
		Rectf border_box() const;
		Rectf padding_box() const;
		Rectf content_box() const;

		float margin_left = 0.0f;
		float margin_top = 0.0f;
		float margin_right = 0.0f;
		float margin_bottom = 0.0f;

		float border_left = 0.0f;
		float border_top = 0.0f;
		float border_right = 0.0f;
		float border_bottom = 0.0f;

		float padding_left = 0.0f;
		float padding_top = 0.0f;
		float padding_right = 0.0f;
		float padding_bottom = 0.0f;

		Rectf content;
	};
}
