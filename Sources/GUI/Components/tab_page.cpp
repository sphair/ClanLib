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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/tab_page.h"
#include "API/Core/XML/dom_element.h"
#include "tab_page_impl.h"
#include "../gui_css_strings.h"
#include "API/Core/Math/rect.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// TabPage Construction:

TabPage::TabPage(GUIComponent *parent)
: GUIComponent(parent, CssStr::TabPage::type_name), impl(new TabPage_Impl)
{
	impl->component = this;
	func_process_message().set(impl.get(), &TabPage_Impl::on_process_message);
	func_render().set(impl.get(), &TabPage_Impl::on_render);
	//FIXME: sig_style_changed().set(impl.get(), &TabPage_Impl::on_style_changed);
	func_resized().set(impl.get(), &TabPage_Impl::on_resized);

	impl->on_style_changed();
}

TabPage::~TabPage()
{
}

/////////////////////////////////////////////////////////////////////////////
// TabPage Attributes:

TabPage *TabPage::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	TabPage *object = NULL;
	if (reference_component)
		object = dynamic_cast<TabPage*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find TabPage named item: %1", id));

	return object;
}

int TabPage::get_id() const
{
	return impl->id;
}

std::string TabPage::get_label() const
{
	return impl->label;
}

/////////////////////////////////////////////////////////////////////////////
// TabPage Operations:


/////////////////////////////////////////////////////////////////////////////
// TabPage Implementation:

void TabPage_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
}

void TabPage_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
}

void TabPage_Impl::on_style_changed()
{
}

void TabPage_Impl::on_resized()
{
}

}
