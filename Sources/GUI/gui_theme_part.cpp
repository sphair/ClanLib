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
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_manager.h"
#include "API/CSSLayout/css_property_list.h"
#include "gui_theme_part_impl.h"
#include "gui_component_select_node.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"

namespace clan
{

GUIThemePart::GUIThemePart()
{
}

GUIThemePart::GUIThemePart(GUIComponent *parent, const std::string &pseudo_tag_name)
: impl(new GUIThemePart_Impl(parent))
{
	impl->element.set_parent(&parent->impl->element);
	impl->element.set_tag_name(pseudo_tag_name);
}

void GUIThemePart::throw_if_null() const
{
	if (!impl)
		throw Exception("GUIThemePart is null");
}


Font GUIThemePart::get_font()
{
	return impl->component->get_font(get_css_properties());
}

const CSSBoxProperties &GUIThemePart::get_css_properties() const
{
	return impl->element.get_css_properties();
}

CSSBoxProperties &GUIThemePart::get_css_properties()
{
	return impl->element.get_css_properties();
}

std::string GUIThemePart::get_tag_name() const
{
	return impl->element.get_tag_name();
}

std::string GUIThemePart::get_id() const
{
	return impl->element.get_id();
}

std::string GUIThemePart::get_class() const
{
	return impl->element.get_class();
}

bool GUIThemePart::get_pseudo_class(const std::string &name) const
{
	return impl->element.get_pseudo_class(name);
}

std::vector<std::string> GUIThemePart::get_pseudo_classes() const
{
	return impl->element.get_pseudo_classes();
}

void GUIThemePart::set_tag_name(const std::string &name)
{
	impl->element.set_tag_name(name);
	update_style();
}
	
void GUIThemePart::set_class(const std::string &name)
{
	impl->element.set_class(name);
	update_style();
}

void GUIThemePart::set_id(const std::string &name)
{
	impl->element.set_id(name);
	update_style();
}

void GUIThemePart::set_pseudo_class(const std::string &name, bool enable)
{
	if (impl->element.set_pseudo_class(name, enable))
		update_style();
}

void GUIThemePart::update_style()
{
	CSSDocument document = impl->component->get_gui_manager().get_css_document();
	impl->element.update_style(&impl->component->impl->gui_manager_impl->resource_cache, document);
}

}
