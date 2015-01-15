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

#include "../../Display/2D/color.h"
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>

namespace clan
{
	class Canvas;
	class Colorf;
	class BoxGeometry;
	class BoxStyleImpl;

	class BoxStyle
	{
	public:
		BoxStyle();
		~BoxStyle();

		/*! Copy assignment operator overload.
		 *  \note Only the reference to the style is copied. Changes made to
		 *        one object will also be seen on the other object.
		 *        Use clone() if you want a new style object.
		 */
		BoxStyle &operator=(const BoxStyle &copy);

		/*! Creates a new copy of the box style object.
		 *  \note Creates a new box style object with the same attributes.
		 *        Changes made to one object will not affect the other object.
		 */
		BoxStyle clone() const;

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

		/*! Sets a linear gradient background (based on CSS spec) for the box.
		 *
		 *  \param gradient_stops An initializer list containing one or more
		 *                        `(Colorf, float)` pairs that denote the color
		 *                        and location of a color stop on the gradient.
		 *                        Stop location values are normalized between
		 *                        `0.0f` (0% in CSS) and `1.0f` (100%).
		 *  \param angle          Gradient vector angle at clock-wise rotation.
		 *                        Defaults to `0`, where the gradient points
		 *                        upwards (i.e. going from bottom to top).
		 *                        Setting this to `45` degrees will make the
		 *                        gradient run diagonally from bottom-left to
		 *                        the top-right.
		 *
		 *  Example usage:
		 *  ```cpp
		 *  // This makes the box background a white-to-black gradient going
		 *  // from the top to bottom.
		 *  set_background_gradient({{ clan::Colorf::white, 0.0f }, { clan::Colorf::black, 1.0f }}, 180.0f);
		 *
		 *  // This makes the box background a semi-transparent 6-color rainbow
		 *  // running diagonally.
		 *  set_background_gradient({
		 *          { {255,0,0,40}, 0.0f }, { {255,255,0,80}, 0.2f }, { {0,255,0,160}, 0.4f },
		 *          { {0,255,255,160}, 0.6f }, { {0,0,255,80}, 0.8f }, { {255,0,255,40}, 1.0f }
		 *          }, 45.0f);
		 *  ```
		 */
		void set_background_gradient(std::initializer_list<std::pair<Colorf,float>> gradient_stops, Angle angle = Angle::from_degrees(0.0f));

		void set_background_gradient_to_bottom(const Colorf &top, const Colorf &bottom);
		void set_background_gradient_to_right(const Colorf &left, const Colorf &right);
		void set_background_image(const std::string &url);
		void set_background_size_contain();

		void set_margin(float left, float top, float right, float bottom);
		void set_margin(float width, float height);
		void set_margin(float size);
		void set_margin_auto();
		void set_margin_left_auto();
		void set_margin_top_auto();
		void set_margin_right_auto();
		void set_margin_bottom_auto();
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

		void render(Canvas &canvas, const BoxGeometry &geometry) const;

		void set_style_changed(const std::function<void()> &callback);

	private:
		std::shared_ptr<BoxStyleImpl> impl;
	};
}
