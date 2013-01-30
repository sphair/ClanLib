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

#include "ribbon_impl.h"

#ifdef WIN32
#define GLASS_EFFECT
#endif

#ifdef GLASS_EFFECT
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace clan
{

Ribbon::Ribbon(GUIComponent *container)
: GUIComponent(container, "ribbon"), impl(new Ribbon_Impl)
{
	impl->component = this;

	func_render().set(impl.get(), &Ribbon_Impl::on_render);
	func_resized().set(impl.get(), &Ribbon_Impl::on_resized);
	func_input_pressed().set(impl.get(), &Ribbon_Impl::on_input_pressed);
	func_input_released().set(impl.get(), &Ribbon_Impl::on_input_released);
	func_input_pointer_moved().set(impl.get(), &Ribbon_Impl::on_input_pointer_moved);

	impl->menu_button = new PushButton(this);
	impl->menu_button->set_class("menu");
	impl->menu_button->set_geometry(Rect(0, 0, impl->menu_button->get_css_values().get_box().width.length.value, impl->menu_button->get_css_values().get_box().height.length.value));
	impl->menu_button->func_clicked().set(impl.get(), &Ribbon_Impl::on_menu_button_clicked);

	impl->menu = new RibbonMenu(this);

	impl->part_tab = GUIThemePart(this, "tab");
	impl->part_tab_background = GUIThemePart(this, "tab-background");

#ifdef GLASS_EFFECT
	// To do: only blur the right area instead of everything.
	HWND hwnd = get_display_window().get_hwnd();
	DWM_BLURBEHIND blur_behind = { 0 };
	blur_behind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED;
	blur_behind.fEnable = TRUE;
	blur_behind.fTransitionOnMaximized = TRUE;
	blur_behind.hRgnBlur = 0;
	DwmEnableBlurBehindWindow(hwnd, &blur_behind);

	MARGINS margins = { 0 };
	margins.cyTopHeight = impl->part_tab_background.get_css_height() + 1;
	DwmExtendFrameIntoClientArea(hwnd, &margins);
#endif
}

Ribbon::~Ribbon()
{

}

RibbonMenu *Ribbon::get_menu()
{
	return impl->menu;
}


Size Ribbon::get_css_size() const
{
	return get_size();
}

}

