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

#include "view.h"
#include "../Events/activation_change_event.h"

namespace clan
{
	/// Base class for views participating as a root view in the user interface
	class RootView : public View
	{
	public:
		
	protected:
		/// Layout root view
		void layout(Canvas &canvas);

		/// Renders view into the specified canvas
		void render(Canvas &canvas);

		/// Dispatch activation change event to all views
		void dispatch_activation_change(ActivationChangeType type);

		/// Test if view is set to hidden
		virtual bool root_hidden() const = 0;

		/// Hides a view from layout and rendering
		virtual void set_root_hidden(bool value) = 0;

		/// Gets the current canvas used to render this view
		///
		/// This function may return a null canvas if the view does not have a canvas attached to it yet.
		virtual Canvas get_root_canvas() const = 0;

		/// Signals this view needs to be rendered again
		virtual void set_root_needs_render() = 0;

		/// Layout local root
		virtual void layout_root() = 0;

		/// Map from local content to screen coordinates
		virtual Pointf root_to_screen_pos(const Pointf &pos) = 0;

		/// Map from screen to local content coordinates
		virtual Pointf root_from_screen_pos(const Pointf &pos) = 0;

		/// Shows a popup view at the given content coordinates
		virtual void root_present_popup(const Pointf &pos, const std::shared_ptr<View> &popup) = 0;

		/// Hides a popup, if visible
		virtual void root_dismiss_popup() = 0;

		/// Shows a modal view
		virtual void root_present_modal(const std::string &title, const std::shared_ptr<View> &modal) = 0;

		/// Hides a modal, if visible
		virtual void root_dismiss_modal() = 0;

	private:
		friend class View;
		friend class ViewImpl;
		friend class PositionedLayout;
	};
}
