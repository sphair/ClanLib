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

#include "../View/view.h"
#include "API/Display/Window/display_window.h"

namespace clan
{
	class WindowView : public View
	{
	public:
		WindowView(const DisplayWindowDescription &desc);

		void show(WindowShowType type = WindowShowType::show);
		void hide();

		void set_needs_render() override;

	private:
		void on_window_size_changed();
		void on_window_render(Canvas &canvas);
		void on_window_key_event(KeyEvent &e);
		void on_window_pointer_event(PointerEvent &e);
		void on_window_close();
		void on_window_activated();
		void on_window_deactivated();

		DisplayWindow window;

		std::shared_ptr<View> captured_view;
		int capture_down_counter = 0;

		std::shared_ptr<View> hot_view;
	};
}
