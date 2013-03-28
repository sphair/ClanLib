/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "API/CSSLayout/ComputedValues/css_computed_box.h"
#include "gui_element.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"
#include "Layout/gui_css_used_values.h"

namespace clan
{

class GUIComponent;

class GUIThemePart_Impl
{
public:
	GUIThemePart_Impl(GUIComponent *component)
	: component(component), current_box_generation(0), element(&component->impl->gui_manager_impl->resource_cache)
	{
	}

	GUIComponent *component;
	GUIElement element;

	GUICSSUsedValues &get_css_used_values(const Rect &border_box);
	Rect last_calculated_border_box;
private:
	GUICSSUsedValues css_used_values;
	int current_box_generation;
};

}
