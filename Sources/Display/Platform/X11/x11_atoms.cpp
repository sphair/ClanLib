/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Chu Chin Kuan
*/

#include "x11_atoms.h"

namespace clan
{
	const std::vector< std::string > X11Atoms::_atoms_ =
	{
		"WM_PROTOCOLS",

		"WM_DELETE_WINDOW",
		"WM_STATE",


		"_NET_FRAME_EXTENTS",

		//! Requests the WM to calculate frame extents of the window at its current
		//! configuration. Some WMs have _NET_FRAME_EXTENTS set anyway and do not
		//! support this atom.
		"_NET_REQUEST_FRAME_EXTENTS",


		"_NET_WM_PING",

		"_NET_WM_STATE", //! Set by WM, lists the following atoms:
		"_NET_WM_STATE_HIDDEN",
		"_NET_WM_STATE_FULLSCREEN",
		"_NET_WM_STATE_MAXIMIZED_HORZ",
		"_NET_WM_STATE_MAXIMIZED_VERT",
		"_NET_WM_STATE_MODAL",

		"_NET_WM_FULLSCREEN_MONITORS",


		"_NET_WM_WINDOW_TYPE",

		"_NET_WM_WINDOW_TYPE_DESKTOP",
		"_NET_WM_WINDOW_TYPE_DOCK",
		"_NET_WM_WINDOW_TYPE_TOOLBAR",
		"_NET_WM_WINDOW_TYPE_MENU",
		"_NET_WM_WINDOW_TYPE_UTILITY",
		"_NET_WM_WINDOW_TYPE_SPLASH",
		"_NET_WM_WINDOW_TYPE_DIALOG",
		"_NET_WM_WINDOW_TYPE_DROPDOWN_MENU",
		"_NET_WM_WINDOW_TYPE_POPUP_MENU",
		"_NET_WM_WINDOW_TYPE_TOOLTIP",
		"_NET_WM_WINDOW_TYPE_NOTIFICATION",
		"_NET_WM_WINDOW_TYPE_COMBO",
		"_NET_WM_WINDOW_TYPE_DND",
		"_NET_WM_WINDOW_TYPE_NORMAL"
	};

	unsigned char *X11Atoms::get_property(::Display *display, Window window, Atom property, Atom &actual_type, int &actual_format, unsigned long &item_count)
	{
		unsigned long bytes_after_return = 0;
		unsigned char *read_data = nullptr;

		int result = XGetWindowProperty(display, window, property,
			0L,		// offset
			~0L,	// request all (in 32-bit units)
			False, AnyPropertyType,
			&actual_type, &actual_format,
			&item_count, &bytes_after_return,
			&read_data);

	    if (result != Success)
		{
	        actual_type = None;
	        actual_format = 0;
	        item_count = 0;
			return nullptr;
		}

		return read_data; // must be freed with XFree by caller
	}

	unsigned char *X11Atoms::get_property(::Display *display, Window window, Atom property, unsigned long &item_count)
	{
		Atom _actual_type;
		int  _actual_format;

		return X11Atoms::get_property(display, window, property, _actual_type, _actual_format, item_count);
	}

	std::vector<bool> X11Atoms::check_net_wm_state(Window window, std::vector<std::string> state_atoms) const
	{
		// Atom not in _NET_WM_STATE MUST be considered not set.
		std::vector< bool > states(state_atoms.size(), false);

		if ((*this)["_NET_WM_STATE"] == None)
		{
			log_event("debug", "clan::X11Window::check_net_wm_state(): _NET_WM_STATE not provided by WM.");
			return states;
		}

		// Get window states from WM
		unsigned long  item_count;
		unsigned char *data = get_property(window, "_NET_WM_STATE", item_count);
		if (data == NULL)
		{
			log_event("debug", "clan::X11Atoms::check_net_wm_state(): Failed to query _NET_WM_STATE.");
			return states;
		}

		unsigned long *items = (unsigned long *)data;

		// Map each state atom to state boolean.
		for (size_t i = 0; i < state_atoms.size(); i++)
		{
			const std::string &elem = state_atoms[i];
			Atom state = static_cast<unsigned long>((*this)[elem]);
			if (state == None)
			{
				log_event("debug", "clan::X11Atoms::check_net_wm_state(): %1 is not provided by WM.", elem);
				continue; // Unsupported states are not queried.
			}

			auto it = std::find(items, items + item_count, state);
			states[i] = (it != items + item_count);
		}

		XFree(data);
		return states;
	}

	bool X11Atoms::modify_net_wm_state(Window window, long action, const std::string &atom1, const std::string &atom2)
	{
		Atom _NET_WM_STATE = (*this)["_NET_WM_STATE"];

		if (_NET_WM_STATE == None)
			return false;

		XEvent xevent;
		memset(&xevent, 0, sizeof(xevent));
		xevent.xclient.type = ClientMessage;
		xevent.xclient.window = window;
		xevent.xclient.message_type = _NET_WM_STATE;
		xevent.xclient.format = 32;
		xevent.xclient.data.l[0] =
		xevent.xclient.data.l[1] = (*this)[atom1];
		xevent.xclient.data.l[2] = (*this)[atom2];
		xevent.xclient.data.l[3] = 0; // or 2

		Status ret = XSendEvent(
			_display_, DefaultRootWindow(_display_), False,
			SubstructureNotifyMask | SubstructureRedirectMask, &xevent
			);

		XFlush(_display_);

		if (ret == 0)
		{
			log_event("debug", "clan::X11Atoms::modify_net_wm_state(): XSendEvent failed.");
			return false;
		}
		else
		{
			return true;
		}
	}

}


