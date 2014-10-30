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

#include "UI/precomp.h"
#include "API/UI/Style/box_style.h"
#include "API/Core/Text/string_help.h"
#include "box_style_impl.h"

namespace clan
{
	BoxStyle::BoxStyle() : impl(new BoxStyleImpl())
	{
	}

	BoxStyle::~BoxStyle()
	{
	}


	BoxStyle &BoxStyle::operator =(const BoxStyle &copy)
	{
		impl = copy.impl;
		return *this;
	}

	BoxStyle BoxStyle::clone() const
	{
		BoxStyle copy;
		*copy.impl = *impl;
		return copy;
	}


	void BoxStyle::set_layout_none()
	{
		impl->layout = BoxLayout::none;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_layout_block()
	{
		impl->layout = BoxLayout::block;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_layout_line()
	{
		impl->layout = BoxLayout::line;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_layout_vbox()
	{
		impl->layout = BoxLayout::vbox;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_layout_hbox()
	{
		impl->layout = BoxLayout::hbox;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex_grow(float grow)
	{
		impl->flex.grow = grow;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex_shrink(float shrink)
	{
		impl->flex.shrink = shrink;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex_basis_auto()
	{
		impl->flex.basis = BoxFlexBasis();
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex_basis(float basis)
	{
		impl->flex.basis.type = BoxFlexBasis::type_length;
		impl->flex.basis.value = basis;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex(float grow, float shrink)
	{
		impl->flex.grow = grow;
		impl->flex.shrink = shrink;
		impl->flex.basis = BoxFlexBasis();
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex(float grow, float shrink, float basis)
	{
		impl->flex.grow = grow;
		impl->flex.shrink = shrink;
		impl->flex.basis.type = BoxFlexBasis::type_length;
		impl->flex.basis.value = basis;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_flex_none()
	{
		impl->flex.grow = 0.0f;
		impl->flex.shrink = 0.0f;
		impl->flex.basis = BoxFlexBasis();
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_none()
	{
		impl->background = BoxBackground();
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background(const Colorf &color)
	{
		impl->background.color = color;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_gradient_to_bottom(const Colorf &top, const Colorf &bottom)
	{
		impl->background.stops.clear();
		impl->background.stops.push_back(BoxGradientStop(top, 0.0f));
		impl->background.stops.push_back(BoxGradientStop(bottom, 1.0f));
		impl->background.angle = 180.0f;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_gradient_to_bottom(const Colorf &stop1, float t1, const Colorf &stop2, float t2, const Colorf &stop3, float t3, const Colorf &stop4, float t4)
	{
		impl->background.stops.clear();
		impl->background.stops.push_back(BoxGradientStop(stop1, t1));
		impl->background.stops.push_back(BoxGradientStop(stop2, t2));
		impl->background.stops.push_back(BoxGradientStop(stop3, t3));
		impl->background.stops.push_back(BoxGradientStop(stop4, t4));
		impl->background.angle = 180.0f;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_gradient_to_right(const Colorf &left, const Colorf &right)
	{
		impl->background.stops.clear();
		impl->background.stops.push_back(BoxGradientStop(left, 0.0f));
		impl->background.stops.push_back(BoxGradientStop(right, 1.0f));
		impl->background.angle = 0.0f;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_gradient_to_right(const Colorf &stop1, float t1, const Colorf &stop2, float t2, const Colorf &stop3, float t3, const Colorf &stop4, float t4)
	{
		impl->background.stops.clear();
		impl->background.stops.push_back(BoxGradientStop(stop1, t1));
		impl->background.stops.push_back(BoxGradientStop(stop2, t2));
		impl->background.stops.push_back(BoxGradientStop(stop3, t3));
		impl->background.stops.push_back(BoxGradientStop(stop4, t4));
		impl->background.angle = 0.0f;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_image(const std::string &url)
	{
		impl->background.image = PixelBuffer(url);
			if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_background_size_contain()
	{

	}

	void BoxStyle::set_margin(float left, float top, float right, float bottom)
	{
		impl->margin.left.type = BoxMarginValue::type_length;
		impl->margin.left.value = left;
		impl->margin.right.type = BoxMarginValue::type_length;
		impl->margin.right.value = right;
		impl->margin.top.type = BoxMarginValue::type_length;
		impl->margin.top.value = top;
		impl->margin.bottom.type = BoxMarginValue::type_length;
		impl->margin.bottom.value = bottom;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_margin(float width, float height)
	{
		set_margin(width, height, width, height);
	}

	void BoxStyle::set_margin(float size)
	{
		set_margin(size, size, size, size);
	}

	void BoxStyle::set_border_none()
	{

	}

	void BoxStyle::set_border(const Colorf &color, float left, float top, float right, float bottom)
	{
		impl->border.left.type = BoxBorderValue::type_solid;
		impl->border.left.color = color;
		impl->border.left.width = left;
		impl->border.right.type = BoxBorderValue::type_solid;
		impl->border.right.color = color;
		impl->border.right.width = right;
		impl->border.top.type = BoxBorderValue::type_solid;
		impl->border.top.color = color;
		impl->border.top.width = top;
		impl->border.bottom.type = BoxBorderValue::type_solid;
		impl->border.bottom.color = color;
		impl->border.bottom.width = bottom;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_border(const Colorf &color, float width, float height)
	{
		set_border(color, width, height, width, height);
	}

	void BoxStyle::set_border(const Colorf &color, float size)
	{
		set_border(color, size, size, size, size);
	}

	void BoxStyle::set_border_radius(float size)
	{
		set_border_radius(size, size, size, size);
	}

	void BoxStyle::set_border_radius(float tl, float tr, float bl, float br)
	{
		impl->border.top_left_radius.x = tl;
		impl->border.top_left_radius.y = tl;
		impl->border.top_right_radius.x = tr;
		impl->border.top_right_radius.y = tr;
		impl->border.bottom_left_radius.x = bl;
		impl->border.bottom_left_radius.y = bl;
		impl->border.bottom_right_radius.x = br;
		impl->border.bottom_right_radius.y = br;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_box_shadow(const Colorf &color, float x, float y, float size)
	{
		impl->background.shadow_inset = false;
		impl->background.shadow_color = color;
		impl->background.shadow_offset.x = x;
		impl->background.shadow_offset.y = y;
		impl->background.shadow_blur_radius = size;
		impl->background.shadow_spread_distance = 0.0f;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_padding(float left, float top, float right, float bottom)
	{
		impl->padding.left = left;
		impl->padding.right = right;
		impl->padding.top = top;
		impl->padding.bottom = bottom;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_padding(float width, float height)
	{
		set_padding(width, height, width, height);
	}

	void BoxStyle::set_padding(float size)
	{
		set_padding(size, size, size, size);
	}

	void BoxStyle::set_width(float width)
	{
		impl->content.width.type = BoxWidthValue::type_length;
		impl->content.width.value = width;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_height(float height)
	{
		impl->content.height.type = BoxWidthValue::type_length;
		impl->content.height.value = height;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_static()
	{
		impl->position.mode = BoxPositionMode::static_mode;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_absolute()
	{
		impl->position.mode = BoxPositionMode::absolute;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_fixed()
	{
		impl->position.mode = BoxPositionMode::fixed;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_left(float value)
	{
		impl->position.left.type = BoxPositionValue::type_length;
		impl->position.left.value = value;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_top(float value)
	{
		impl->position.top.type = BoxPositionValue::type_length;
		impl->position.top.value = value;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_right(float value)
	{
		impl->position.right.type = BoxPositionValue::type_length;
		impl->position.right.value = value;
		if (impl->style_changed) impl->style_changed();
	}

	void BoxStyle::set_bottom(float value)
	{
		impl->position.bottom.type = BoxPositionValue::type_length;
		impl->position.bottom.value = value;
		if (impl->style_changed) impl->style_changed();
	}

	bool BoxStyle::is_width_auto() const
	{
		return impl->content.width.type == BoxWidthValue::type_auto;
	}

	bool BoxStyle::is_height_auto() const
	{
		return impl->content.height.type == BoxWidthValue::type_auto;
	}

	float BoxStyle::width() const
	{
		return !is_width_auto() ? impl->content.width.value : 0.0f;
	}

	float BoxStyle::height() const
	{
		return !is_height_auto() ? impl->content.height.value : 0.0f;
	}

	bool BoxStyle::is_static() const
	{
		return impl->position.mode == BoxPositionMode::static_mode;
	}

	bool BoxStyle::is_absolute() const
	{
		return impl->position.mode == BoxPositionMode::absolute;
	}

	bool BoxStyle::is_fixed() const
	{
		return impl->position.mode == BoxPositionMode::fixed;
	}

	bool BoxStyle::is_left_auto() const
	{
		return impl->position.left.type == BoxPositionValue::type_auto;
	}

	bool BoxStyle::is_top_auto() const
	{
		return impl->position.top.type == BoxPositionValue::type_auto;
	}

	bool BoxStyle::is_right_auto() const
	{
		return impl->position.right.type == BoxPositionValue::type_auto;
	}

	bool BoxStyle::is_bottom_auto() const
	{
		return impl->position.bottom.type == BoxPositionValue::type_auto;
	}

	float BoxStyle::left() const
	{
		return !is_left_auto() ? impl->position.left.value : 0.0f;
	}

	float BoxStyle::top() const
	{
		return !is_top_auto() ? impl->position.top.value : 0.0f;
	}

	float BoxStyle::right() const
	{
		return !is_right_auto() ? impl->position.right.value : 0.0f;
	}

	float BoxStyle::bottom() const
	{
		return !is_bottom_auto() ? impl->position.bottom.value : 0.0f;
	}

	bool BoxStyle::is_margin_left_auto() const
	{
		return impl->margin.left.type == BoxMarginValue::type_auto;
	}

	bool BoxStyle::is_margin_top_auto() const
	{
		return impl->margin.top.type == BoxMarginValue::type_auto;
	}

	bool BoxStyle::is_margin_right_auto() const
	{
		return impl->margin.right.type == BoxMarginValue::type_auto;
	}

	bool BoxStyle::is_margin_bottom_auto() const
	{
		return impl->margin.bottom.type == BoxMarginValue::type_auto;
	}

	float BoxStyle::margin_left() const
	{
		return !is_margin_left_auto() ? impl->margin.left.value : 0.0f;
	}

	float BoxStyle::margin_top() const
	{
		return !is_margin_top_auto() ? impl->margin.top.value : 0.0f;
	}

	float BoxStyle::margin_right() const
	{
		return !is_margin_right_auto() ? impl->margin.right.value : 0.0f;
	}

	float BoxStyle::margin_bottom() const
	{
		return !is_margin_bottom_auto() ? impl->margin.bottom.value : 0.0f;
	}

	bool BoxStyle::is_border_left_solid() const
	{
		return impl->border.left.type == BoxBorderValue::type_solid;
	}

	bool BoxStyle::is_border_top_solid() const
	{
		return impl->border.top.type == BoxBorderValue::type_solid;
	}

	bool BoxStyle::is_border_right_solid() const
	{
		return impl->border.right.type == BoxBorderValue::type_solid;
	}

	bool BoxStyle::is_border_bottom_solid() const
	{
		return impl->border.bottom.type == BoxBorderValue::type_solid;
	}

	float BoxStyle::border_left() const
	{
		return impl->border.left.width;
	}

	float BoxStyle::border_top() const
	{
		return impl->border.top.width;
	}

	float BoxStyle::border_right() const
	{
		return impl->border.right.width;
	}

	float BoxStyle::border_bottom() const
	{
		return impl->border.bottom.width;
	}

	float BoxStyle::padding_left() const
	{
		return impl->padding.left;
	}

	float BoxStyle::padding_top() const
	{
		return impl->padding.top;
	}

	float BoxStyle::padding_right() const
	{
		return impl->padding.right;
	}

	float BoxStyle::padding_bottom() const
	{
		return impl->padding.bottom;
	}

	bool BoxStyle::is_layout_none() const
	{
		return impl->layout == BoxLayout::none;
	}

	bool BoxStyle::is_layout_block() const
	{
		return impl->layout == BoxLayout::block;
	}

	bool BoxStyle::is_layout_line() const
	{
		return impl->layout == BoxLayout::line;
	}

	bool BoxStyle::is_layout_vbox() const
	{
		return impl->layout == BoxLayout::vbox;
	}

	bool BoxStyle::is_layout_hbox() const
	{
		return impl->layout == BoxLayout::hbox;
	}

	float BoxStyle::flex_grow() const
	{
		return impl->flex.grow;
	}

	float BoxStyle::flex_shrink() const
	{
		return impl->flex.shrink;
	}

	bool BoxStyle::is_flex_basis_auto() const
	{
		return impl->flex.basis.type == BoxFlexBasis::type_auto;
	}

	float BoxStyle::flex_basis() const
	{
		return !is_flex_basis_auto() ? impl->flex.basis.value : 0.0f;
	}

	void BoxStyle::render(Canvas &canvas, const BoxGeometry &geometry) const
	{
		impl->render(canvas, geometry);
	}

	void BoxStyle::set_style_changed(const std::function<void()> &callback)
	{
		impl->style_changed = callback;
	}
}
