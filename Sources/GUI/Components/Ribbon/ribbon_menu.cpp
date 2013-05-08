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
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/Components/ribbon_menu.h"
#include "API/Display/Window/input_event.h"

#include "ribbon_menu_impl.h"

namespace clan
{

RibbonMenu::RibbonMenu(GUIComponent *owner)
: GUIComponent(owner, RibbonMenu_Impl::create_toplevel_description(), "ribbon-menu"), impl(new RibbonMenu_Impl)
{
	impl->component = this;
	func_render().set(impl.get(), &RibbonMenu_Impl::on_render);
	func_resized().set(impl.get(), &RibbonMenu_Impl::on_resized);
	impl->part_menu_item = GUIThemePart(this, "menu-item");
	impl->part_menu_item_selected = GUIThemePart(this, "menu-item");
	impl->part_menu_item_selected.set_pseudo_class("selected", true);
	impl->part_menu_item_separator = GUIThemePart(this, "menu-item");
	impl->part_menu_item_separator.set_pseudo_class("separator", true);

	impl->slot_filter = owner->get_gui_manager().sig_filter_message().connect(impl.get(), &RibbonMenu_Impl::on_filter_message);
}

Callback_v0 &RibbonMenu::add_item(Image image, std::string text)
{
	impl->items.push_back(RibbonMenuItem(image, text));
	return impl->items.back().func_clicked;
}

void  RibbonMenu::add_separator()
{
	impl->items.push_back(RibbonMenuItem());
	impl->items.back().separator = true;
}


}
