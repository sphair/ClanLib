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
#include "API/UI/Controller/window_manager.h"
#include "API/UI/TopLevel/top_level_window.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/2D/canvas.h"

namespace clan
{
	class WindowManagerImpl
	{
	public:
		std::map<WindowController *, std::shared_ptr<WindowController>> windows;
	};

	/////////////////////////////////////////////////////////////////////////

	class WindowControllerImpl
	{
	public:
		std::string title;
		std::shared_ptr<View> default_root_view = std::make_shared<View>();

		WindowManager *manager = nullptr;
		std::shared_ptr<TopLevelWindow> window;
		std::weak_ptr<View> modal_owner;
	};

	/////////////////////////////////////////////////////////////////////////

	WindowManager::WindowManager() : impl(new WindowManagerImpl)
	{
	}

	WindowManager::~WindowManager()
	{
	}

	void WindowManager::present_main(const std::shared_ptr<WindowController> &controller)
	{

	}

	void WindowManager::present_modal(View *owner, const std::shared_ptr<WindowController> &controller)
	{
		if (controller->impl->manager)
			return;

		Pointf screen_pos = owner->to_screen_pos(owner->geometry().content_box().get_center());

		DisplayWindowDescription desc;
		desc.set_dialog_window();
		desc.set_visible(false);
		desc.set_owner_window(owner->view_tree()->get_display_window());
		desc.set_title(controller->title());
		desc.show_minimize_button(false);
		desc.show_maximize_button(false);

		controller->impl->manager = this;
		controller->impl->window = std::make_shared<TopLevelWindow>(desc);
		controller->impl->window->set_root_view(controller->root_view());
		controller->slots.connect(controller->impl->window->get_display_window().sig_window_close(), bind_member(controller.get(), &WindowController::dismiss));

		impl->windows[controller.get()] = controller;

		Canvas canvas = controller->root_view()->get_canvas();
		float width = controller->root_view()->calculate_preferred_width(canvas);
		float height = controller->root_view()->calculate_preferred_height(canvas, width);
		Rectf content_box(screen_pos.x - width * 0.5f, screen_pos.y - height * 0.5f, screen_pos.x + width * 0.5f, screen_pos.y + height * 0.5f);
		Rectf margin_box = ViewGeometry::from_content_box(controller->root_view()->style_cascade(), content_box).margin_box();
		controller->impl->window->get_display_window().set_position(margin_box, true);

		controller->impl->window->show(WindowShowType::show);
		owner->view_tree()->get_display_window().set_enabled(false);
	}

	void WindowManager::present_popup(View *owner, const Pointf &pos, const std::shared_ptr<WindowController> &controller)
	{
		if (controller->impl->manager)
			return;

		Pointf screen_pos = owner->to_screen_pos(pos);

		DisplayWindowDescription desc;
		desc.set_popup_window();
		desc.set_visible(false);
		desc.set_topmost(true);
		desc.set_no_activate(true);
		desc.show_caption(false);
		desc.show_sysmenu(false);
		desc.show_minimize_button(false);
		desc.show_maximize_button(false);

		controller->impl->manager = this;
		controller->impl->window = std::make_shared<TopLevelWindow>(desc);
		controller->impl->window->set_root_view(controller->root_view());
		controller->slots.connect(owner->view_tree()->get_display_window().sig_lost_focus(), bind_member(controller.get(), &WindowController::dismiss));

		impl->windows[controller.get()] = controller;

		Canvas canvas = controller->root_view()->get_canvas();
		float width = controller->root_view()->calculate_preferred_width(canvas);
		float height = controller->root_view()->calculate_preferred_height(canvas, width);
		Rectf content_box(screen_pos.x, screen_pos.y, screen_pos.x + width, screen_pos.y + height);
		Rectf margin_box = ViewGeometry::from_content_box(controller->root_view()->style_cascade(), content_box).margin_box();
		controller->impl->window->get_display_window().set_position(margin_box, false);

		controller->impl->window->show(WindowShowType::show_no_activate);
	}

	/////////////////////////////////////////////////////////////////////////

	WindowController::WindowController() : impl(new WindowControllerImpl)
	{
	}

	WindowController::~WindowController()
	{
	}

	const std::shared_ptr<View> &WindowController::root_view() const
	{
		return impl->default_root_view;
	}

	const std::string &WindowController::title() const
	{
		return impl->title;
	}

	void WindowController::set_title(const std::string &title)
	{
		impl->title = title;
		if (impl->window)
			impl->window->get_display_window().set_title(title);
	}

	void WindowController::dismiss()
	{
		if (impl->manager)
		{
			auto modal_owner = impl->modal_owner.lock();
			if (modal_owner && modal_owner->view_tree())
				modal_owner->view_tree()->get_display_window().set_enabled(true);

			auto &windows = impl->manager->impl->windows;
			auto it = windows.find(this);
			if (it != windows.end())
				windows.erase(it);
		}

		impl->manager = nullptr;
		impl->window.reset();
		impl->modal_owner.reset();
	}
}
