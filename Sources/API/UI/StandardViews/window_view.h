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

#pragma once

#include "../View/root_view.h"
#include "../../Display/Window/display_window.h"
#include "../../Display/Window/keys.h"

namespace clan
{
	enum class WindowShowType
	{
		hide,
		show,
		show_no_activate,
		normal,
		maximized,
		minimized,
		maximize,
		minimize,
		minimize_no_activate,
		restore,
		show_default
	};

	class InputEvent;
	class WindowView_Impl;

	class WindowView : public RootView
	{
	public:
		WindowView(const DisplayWindowDescription &desc);

		void show(WindowShowType type = WindowShowType::show);
		void hide();

		DisplayWindow get_display_window();

	protected:
		bool root_hidden() const override;
		void set_root_hidden(bool value) override;

		Canvas get_root_canvas() const override;

		void set_root_needs_render() override;
		void layout_root() override;

		Pointf root_to_screen_pos(const Pointf &pos) override;
		Pointf root_from_screen_pos(const Pointf &pos) override;

		void root_present_popup(const Pointf &pos, const std::shared_ptr<View> &popup) override;
		void root_dismiss_popup() override;

		void root_present_modal(const std::string &title, const std::shared_ptr<View> &modal) override;
		void root_dismiss_modal() override;

	private:
		std::shared_ptr<WindowView_Impl> impl;

		friend class WindowView_Impl;
	};
}
