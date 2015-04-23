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

#include "UI/precomp.h"
#include "API/UI/View/view_geometry.h"
#include "API/UI/Style/style.h"

namespace clan
{
	ViewGeometry::ViewGeometry(const StyleCascade &style_cascade)
	{
		margin_left = style_cascade.computed_value("margin-left").number;
		margin_top = style_cascade.computed_value("margin-top").number;
		margin_right = style_cascade.computed_value("margin-right").number;
		margin_bottom = style_cascade.computed_value("margin-bottom").number;

		border_left = style_cascade.computed_value("border-left-width").number;
		border_top = style_cascade.computed_value("border-top-width").number;
		border_right = style_cascade.computed_value("border-right-width").number;
		border_bottom = style_cascade.computed_value("border-bottom-width").number;

		padding_left = style_cascade.computed_value("padding-left").number;
		padding_top = style_cascade.computed_value("padding-top").number;
		padding_right = style_cascade.computed_value("padding-right").number;
		padding_bottom = style_cascade.computed_value("padding-bottom").number;
	}

	ViewGeometry ViewGeometry::from_margin_box(const StyleCascade &style, const Rectf &box)
	{
		ViewGeometry geometry(style);

		geometry.content = Rectf::ltrb(
			box.left + geometry.margin_left + geometry.border_left + geometry.padding_left,
			box.top + geometry.margin_top + geometry.border_top + geometry.padding_top,
			box.right - geometry.margin_right - geometry.border_right - geometry.padding_right,
			box.bottom - geometry.margin_bottom - geometry.border_bottom - geometry.padding_bottom);

		return geometry;
	}

	ViewGeometry ViewGeometry::from_border_box(const StyleCascade &style, const Rectf &box)
	{
		ViewGeometry geometry(style);

		geometry.content = Rectf::ltrb(
			box.left + geometry.border_left + geometry.padding_left,
			box.top + geometry.border_top + geometry.padding_top,
			box.right - geometry.border_right - geometry.padding_right,
			box.bottom - geometry.border_bottom - geometry.padding_bottom);

		return geometry;
	}

	ViewGeometry ViewGeometry::from_padding_box(const StyleCascade &style, const Rectf &box)
	{
		ViewGeometry geometry(style);

		geometry.content = Rectf::ltrb(
			box.left + geometry.padding_left,
			box.top + geometry.padding_top,
			box.right - geometry.padding_right,
			box.bottom - geometry.padding_bottom);

		return geometry;
	}

	ViewGeometry ViewGeometry::from_content_box(const StyleCascade &style, const Rectf &box)
	{
		ViewGeometry geometry(style);
		geometry.content = box;
		return geometry;
	}

	Rectf ViewGeometry::margin_box() const
	{
		return Rectf::ltrb(
			content.left - margin_left - border_left - padding_left,
			content.top - margin_top - border_top - padding_top,
			content.right + margin_right + border_right + padding_right,
			content.bottom + margin_bottom + border_bottom + padding_bottom);
	}

	Rectf ViewGeometry::border_box() const
	{
		return Rectf::ltrb(
			content.left - border_left - padding_left,
			content.top - border_top - padding_top,
			content.right + border_right + padding_right,
			content.bottom + border_bottom + padding_bottom);
	}

	Rectf ViewGeometry::padding_box() const
	{
		return Rectf::ltrb(
			content.left - padding_left,
			content.top - padding_top,
			content.right + padding_right,
			content.bottom + padding_bottom);
	}

	Rectf ViewGeometry::content_box() const
	{
		return content;
	}
}
