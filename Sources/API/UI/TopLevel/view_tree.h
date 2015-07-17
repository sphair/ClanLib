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

#include "../View/view.h"
#include "../Events/activation_change_event.h"

namespace clan
{
	class DisplayWindow;
	class Canvas;
	class ViewTreeImpl;

	/// Base class for managing a tree of views
	class ViewTree
	{
	public:
		ViewTree();
		~ViewTree();
		
		/// The view receiving keyboard events or nullptr if no view has the focus
		View *focus_view() const;

		/// Gets the display window used
		virtual DisplayWindow get_display_window() = 0;

		/// Gets the current canvas used to render
		virtual Canvas get_canvas() const = 0;

	protected:
		/// Returns the root view node
		const std::shared_ptr<View> &root();

		/// Sets the root view node
		void set_root(const std::shared_ptr<View> &view);

		/// Set or clears the focus
		void set_focus_view(View *view);

		/// Renders view into the specified canvas
		void render(Canvas &canvas, const Rectf &margin_box);

		/// Dispatch activation change event to all views
		void dispatch_activation_change(ActivationChangeType type);

		/// Signals that the root view needs to be rendered again
		virtual void set_needs_render() = 0;

		/// Map from client to screen coordinates
		virtual Pointf client_to_screen_pos(const Pointf &pos) = 0;

		/// Map from screen to client coordinates
		virtual Pointf screen_to_client_pos(const Pointf &pos) = 0;

		/// Shows a popup view at the given content coordinates
		virtual void present_popup(const Pointf &pos, const std::shared_ptr<View> &popup) = 0;

		/// Hides a popup, if visible
		virtual void dismiss_popup() = 0;

		/// Shows a modal view
		virtual void present_modal(const std::string &title, const std::shared_ptr<View> &modal) = 0;

		/// Hides a modal, if visible
		virtual void dismiss_modal() = 0;

	private:
		ViewTree(const ViewTree &) = delete;
		ViewTree &operator=(const ViewTree &) = delete;

		std::unique_ptr<ViewTreeImpl> impl;

		friend class View;
		friend class ViewImpl;
		friend class PositionedLayout;
	};
}
