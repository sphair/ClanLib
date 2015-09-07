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

#include "Display/precomp.h"
#include "API/Core/System/system.h"
#include "input_device_provider_x11keyboard.h"
#include "x11_window.h"
#include <X11/Xatom.h>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Clipboard_X11 construction:

Clipboard_X11::Clipboard_X11(X11Window *x11_window)
: x11_window(x11_window), atom_CLIPBOARD(None), clipboard_available(false)
{
}

Clipboard_X11::~Clipboard_X11()
{
}

/////////////////////////////////////////////////////////////////////////////
// Clipboard_X11 attributes:

std::string Clipboard_X11::get_clipboard_text() const
{
	auto handle = x11_window->get_handle();

	if (atom_CLIPBOARD == None)
	{
		// X Server does not have clipboard support
		return std::string();
	}

	XConvertSelection(handle.display, atom_CLIPBOARD, XA_STRING, atom_CLIPBOARD, handle.window, CurrentTime);
	XFlush(handle.display);

	XEvent event;

	auto start_time = System::get_time();
	while(true)
	{
		if (XCheckTypedWindowEvent(handle.display, handle.window, SelectionNotify, &event))
			break;

		// Allow target application half a second of time to respond to this event.
		// TODO What is the correct way to do this?
		if ((System::get_time() - start_time) >= 500)
		{
			return std::string();
		}
		System::sleep(50); // Sleep for 50ms
	}

	Atom actual_type;
	int  actual_format;
	unsigned long item_count;
	unsigned char *read_data = X11Atoms::get_property(handle.display, handle.window, atom_CLIPBOARD, actual_type, actual_format, item_count);
	if (actual_type != XA_STRING || actual_format != 8 || item_count <= 0 || read_data == nullptr)
	{
		log_event("debug", "Failed to get X11 CLIPBOARD text.");
		if (read_data != nullptr) XFree(read_data);
		return std::string();
	}

	std::string buffer((char *)read_data);
	XFree(read_data);
	return buffer;
}

bool Clipboard_X11::is_clipboard_text_available() const
{
	return get_clipboard_text().size() > 0;
}

bool Clipboard_X11::is_clipboard_image_available() const
{
	// TODO Support ICCCM PRIMARY selection and FreeDesktop clipboards-spec
#if DEBUG
	throw Exception("Clipboard_X11::is_clipboard_image_available() unimplemented.");
#else
	return false;
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Clipboard_X11 operations:

void Clipboard_X11::setup()
{
	clipboard_available = false;
	clipboard_current = std::string();
	atom_CLIPBOARD = XInternAtom(x11_window->get_display(), "CLIPBOARD", False);
}

void Clipboard_X11::event_selection_clear(XSelectionClearEvent &xselectionclear)
{
	clipboard_available = false;
	clipboard_current = std::string();
}

void Clipboard_X11::event_selection_notify()
{
	// Data was sent to use (this is handled in get_clipboard_text())
}

void Clipboard_X11::event_selection_request(XSelectionRequestEvent &xselectionrequest)
{
	auto handle = x11_window->get_handle();

	XSelectionRequestEvent *rptr = &xselectionrequest;
	if (rptr->requestor == handle.window) return; // Ignore request if from self

	XEvent new_event;
	new_event.xselection.type = SelectionNotify;
	new_event.xselection.display = rptr->display;
	new_event.xselection.requestor = rptr->requestor;
	new_event.xselection.selection = rptr->selection;
	new_event.xselection.target = rptr->target;
	new_event.xselection.property = None;
	new_event.xselection.time = rptr->time;

	Atom xa_targets  = XInternAtom(handle.display, "TARGETS", False);
	Atom xa_multiple = XInternAtom(handle.display, "MULTIPLE", False);

	struct AtomPair { Atom target; Atom property; } *multi_ptr = nullptr;

	unsigned char *data_ptr = NULL;
	int num_multi   =  0;
	int index_multi = -1;
	if (rptr->target == xa_multiple)
	{
		unsigned long item_count;
		data_ptr = X11Atoms::get_property(handle.display, rptr->requestor, rptr->property, item_count);

		if (data_ptr != NULL)
		{
			num_multi = item_count / 2;
			multi_ptr = (struct AtomPair *) data_ptr;
		}

		index_multi = 0;
	}

	while(index_multi < num_multi)
	{
		Window xtarget;
		Atom xproperty;
		new_event.xselection.property = None;
		if (multi_ptr)
		{
			xtarget = multi_ptr[index_multi].target;
			xproperty = multi_ptr[index_multi].property;
			index_multi++;
		}else
		{
			xtarget = rptr->target;
			xproperty = rptr->property;
		}

		if (xtarget == xa_targets)
		{
			long new_targets[1];
			new_targets[0] = XA_STRING;
			XChangeProperty(handle.display, rptr->requestor, xproperty, xa_targets, 32, PropModeReplace, (unsigned char *) new_targets, 1);
			new_event.xselection.property = xproperty;
		}else
		{

			if (xtarget == XA_STRING)
			{
				XChangeProperty(handle.display, rptr->requestor, xproperty, xtarget, 8, PropModeReplace, (const unsigned char*) clipboard_current.c_str(), clipboard_current.size());
				new_event.xselection.property = xproperty;

			}
		}
		XSendEvent(handle.display, rptr->requestor, False, 0, &new_event);
		if (!num_multi) break;
	}
	if (data_ptr)
	{
		XFree(data_ptr);
	}
}

void Clipboard_X11::set_clipboard_text(const std::string &text)
{
	// The clipboard only works while the message queue is being processed as
	// get_message() processes clipboard (selection) events.

	if (atom_CLIPBOARD == None)
	{
		// X Server does not have clipboard support
		return;
	}
	clipboard_current = text;
	clipboard_available = true;

	auto handle = x11_window->get_handle();

	XSetSelectionOwner(handle.display, XA_PRIMARY, handle.window, CurrentTime);
	XSetSelectionOwner(handle.display, atom_CLIPBOARD, handle.window, CurrentTime);
}

/////////////////////////////////////////////////////////////////////////////
// Clipboard_X11 implementation:

}
