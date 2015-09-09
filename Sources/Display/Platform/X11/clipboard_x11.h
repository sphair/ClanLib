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
**    Mark Page
*/

#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace clan
{
	class X11Window;

	class Clipboard_X11
	{
	public:
		Clipboard_X11(X11Window *x11_window);
		~Clipboard_X11();

		std::string get_clipboard_text() const;
		bool is_clipboard_text_available() const;
		bool is_clipboard_image_available() const;

		void set_clipboard_text(const std::string &text);
		void setup();
		void event_selection_clear(XSelectionClearEvent &xselectionclear);
		void event_selection_notify();
		void event_selection_request(XSelectionRequestEvent &xselectionrequest);

	private:
		X11Window *x11_window;

		Atom atom_CLIPBOARD;
		std::string clipboard_current;
		bool clipboard_available;
	};
}
