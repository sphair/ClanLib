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
**    Harry Storbacka
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/GUI/Components/ribbon.h"
#include "API/GUI/Components/ribbon_page.h"
#include "API/GUI/Components/ribbon_section.h"
#include "API/GUI/Components/ribbon_menu.h"
#include "API/GUI/Components/push_button.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"
#include "../../gui_css_strings.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_window_manager.h"
#include "ribbon_impl.h"

namespace clan
{

Ribbon::Ribbon(GUIComponent *container)
: GUIComponent(container, "ribbon"), impl(new Ribbon_Impl)
{
	impl->component = this;

	impl->menu = new RibbonMenu(this);

	impl->tab_row = new GUIComponent(this, "ribbon-tab-row");

	impl->menu_button = new PushButton(impl->tab_row);
	impl->menu_button->set_class("menu", true);
	impl->menu_button->func_clicked().set(impl.get(), &Ribbon_Impl::on_menu_button_clicked);

	impl->page_area = new GUIComponent(this, "ribbon-page-area");

	if (get_gui_manager().get_window_manager().get_window_manager_type() == GUIWindowManager::cl_wm_type_system)
	{
		get_display_window().enable_alpha_channel(Rect());
		get_display_window().extend_frame_into_client_area(23/*impl->tab_row->get_css_values().get_box().height.length.value*/);
	}
}

Ribbon::~Ribbon()
{
}

RibbonMenu *Ribbon::get_menu()
{
	return impl->menu;
}


}

