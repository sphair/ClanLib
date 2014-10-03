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

#include "../../Display/2D/color.h"
#include <memory>
#include <string>
#include <functional>

namespace clan
{
	class Canvas;
	class Colorf;
	class ViewGeometry;
	class ViewStyleImpl;

	class ViewStyle
	{
	public:
		ViewStyle();
		ViewStyle(ViewStyle &&move);
		ViewStyle(const ViewStyle &copy);
		~ViewStyle();

		ViewStyle &operator =(const ViewStyle &that);

		void set_layout_none();
		void set_layout_block();
		void set_layout_line();
		void set_layout_vbox();
		void set_layout_hbox();
		bool is_layout_none() const;
		bool is_layout_block() const;
		bool is_layout_line() const;
		bool is_layout_vbox() const;
		bool is_layout_hbox() const;

		void set_flex_grow(float grow);
		void set_flex_shrink(float shrink);
		void set_flex_basis_auto();
		void set_flex_basis(float basis);
		void set_flex(float grow, float shrink);
		void set_flex(float grow, float shrink, float basis);
		void set_flex_none();
		float flex_grow() const;
		float flex_shrink() const;
		bool is_flex_basis_auto() const;
		float flex_basis() const;

		void set_background_none();
		void set_background(const Colorf &color);
		void set_background_gradient_to_bottom(const Colorf &top, const Colorf &bottom);
		void set_background_gradient_to_bottom(const Colorf &stop1, float t1, const Colorf &stop2, float t2, const Colorf &stop3, float t3, const Colorf &stop4, float t4);
		void set_background_image(const std::string &url);
		void set_background_size_contain();

		void set_margin(float left, float top, float right, float bottom);
		void set_margin(float width, float height);
		void set_margin(float size);
		bool is_margin_left_auto() const;
		bool is_margin_top_auto() const;
		bool is_margin_right_auto() const;
		bool is_margin_bottom_auto() const;
		float margin_left() const;
		float margin_top() const;
		float margin_right() const;
		float margin_bottom() const;

		void set_border_none();
		void set_border(const Colorf &color, float left, float top, float right, float bottom);
		void set_border(const Colorf &color, float width, float height);
		void set_border(const Colorf &color, float size);
		bool is_border_left_solid() const;
		bool is_border_top_solid() const;
		bool is_border_right_solid() const;
		bool is_border_bottom_solid() const;
		float border_left() const;
		float border_top() const;
		float border_right() const;
		float border_bottom() const;

		void set_border_radius(float size);
		void set_border_radius(float tl, float tr, float bl, float br);
		void set_box_shadow(const Colorf &color, float x, float y, float size);

		void set_padding(float left, float top, float right, float bottom);
		void set_padding(float width, float height);
		void set_padding(float size);
		float padding_left() const;
		float padding_top() const;
		float padding_right() const;
		float padding_bottom() const;

		void set_width(float width);
		void set_height(float height);
		bool is_width_auto() const;
		bool is_height_auto() const;
		float width() const;
		float height() const;

		void set_static();
		void set_absolute();
		void set_fixed();
		void set_left(float value);
		void set_top(float value);
		void set_right(float value);
		void set_bottom(float value);
		bool is_static() const;
		bool is_absolute() const;
		bool is_fixed() const;
		bool is_left_auto() const;
		bool is_top_auto() const;
		bool is_right_auto() const;
		bool is_bottom_auto() const;
		float left() const;
		float top() const;
		float right() const;
		float bottom() const;

		void render(Canvas &canvas, const ViewGeometry &geometry) const;

		void set_style_changed(const std::function<void()> &callback);

	private:
		std::unique_ptr<ViewStyleImpl> impl;
	};
}
