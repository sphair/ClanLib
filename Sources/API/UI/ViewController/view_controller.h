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

#include <vector>
#include <memory>
#include <functional>

namespace clan
{
	class View;
	class ViewControllerImpl;

	/// Controller for a view
	class ViewController : public std::enable_shared_from_this<ViewController>
	{
	public:
		ViewController();
		~ViewController();

		/// View the controller is managing
		std::shared_ptr<View> view;

		/// Slot container helping with automatic disconnection of connected slots when the controller is destroyed
		SlotContainer slots;

		/// Parent controller in the controller tree
		ViewController *parent_controller() const;

		/// Child controllers in the controller tree
		const std::vector<std::shared_ptr<ViewController>> &child_controllers() const;

		/// Adds a child controller to this controller
		void add_child_controller(const std::shared_ptr<ViewController> &view_controller);

		/// Removes the controller from its parent
		void remove_from_parent_controller();

	protected:
		/// Called when a child controller is added
		virtual void child_controller_added(const std::shared_ptr<ViewController> &view_controller);

		/// Called when a child controller is removed
		virtual void child_controller_removed(const std::shared_ptr<ViewController> &view_controller);

	private:
		ViewController(const ViewController &) = delete;
		ViewController &operator=(const ViewController &) = delete;

		std::unique_ptr<ViewControllerImpl> impl;

		friend class ViewControllerImpl;
	};
}
