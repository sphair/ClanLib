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

#include "gui_css_box_visitor.h"

namespace clan
{

class GUICSSDisplayVisitor : public GUICSSBoxVisitor
{
public:
	void node(GUIComponent_Impl *node);

	void flex_node(GUIComponent_Impl *node);

	virtual void flex_horizontal_node(GUIComponent_Impl *node) = 0;
	virtual void flex_vertical_node(GUIComponent_Impl *node) = 0;

	void clan_grid_node(GUIComponent_Impl *node);
	void clan_stacked_node(GUIComponent_Impl *node);

protected:
	CSSUsedValue get_used_noncontent_width(const GUICSSUsedValues &values);
	CSSUsedValue get_used_noncontent_height(const GUICSSUsedValues &values);

	float get_css_relative_x(GUIComponent_Impl *node, float containing_width);
	float get_css_relative_y(GUIComponent_Impl *node, float containing_height);

	static bool is_normal_flow(GUIComponent_Impl *node);
};

}
