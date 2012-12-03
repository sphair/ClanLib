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

#include "GUI/precomp.h"
#include "API/GUI/gui_pseudo_component.h"
#include "API/GUI/gui_manager.h"
#include "API/CSSLayout/css_property_list.h"
#include "gui_pseudo_component_impl.h"
#include "gui_component_select_node.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"

namespace clan
{

GUIPseudoComponent::GUIPseudoComponent()
{
}

GUIPseudoComponent::GUIPseudoComponent(GUIComponent *parent, const std::string &pseudo_tag_name)
: impl(new GUIPseudoComponent_Impl(parent, pseudo_tag_name))
{
}

void GUIPseudoComponent::throw_if_null() const
{
	if (!impl)
		throw Exception("GUIPseudoComponent is null");
}


Font GUIPseudoComponent::get_font()
{
	return impl->component->get_font(get_css_properties());
}

const CSSBoxProperties &GUIPseudoComponent::get_css_properties() const
{
	return impl->css_properties;
}

CSSBoxProperties &GUIPseudoComponent::get_css_properties()
{
	return impl->css_properties;
}

void GUIPseudoComponent::update_style()
{
	impl->css_properties = CSSBoxProperties();

	GUIComponentSelectNode select_node(impl->component, impl->pseudo_tag_name);
	CSSPropertyList sheet_properties = impl->component->get_gui_manager().get_css_document().select(&select_node);
	impl->css_properties.apply_properties(sheet_properties);

	if (impl->component->get_parent_component())
		impl->css_properties.compute(&impl->component->get_parent_component()->get_css_properties(), &impl->component->impl->gui_manager_impl->resource_cache);
	else
		impl->css_properties.compute(0, &impl->component->impl->gui_manager_impl->resource_cache);
}

}
