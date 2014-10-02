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

#include <Memory>

namespace clan
{
	class View;

	enum class EventUIPhase
	{
		none,
		capturing,
		at_target,
		bubbling
	};

	class EventUI
	{
	public:
		virtual ~EventUI() { }

		EventUIPhase phase() const { return _phase; }

		std::shared_ptr<View> target() { return _target; }
		std::shared_ptr<View> current_target() { return _current_target; }

		bool default_prevented() const { return _default_prevented; }
		void prevent_default() { _default_prevented = true; }

		//bool immediate_propagation_stopped() const { return _immediate_propagation_stopped; }
		//void stop_immediate_propagation() { _propagation_stopped = true; _immediate_propagation_stopped = true; }

		bool propagation_stopped() const { return _propagation_stopped; }
		void stop_propagation() { _propagation_stopped = true; }

		/// \brief Milliseconds since 1970
		long long timestamp() const { return _timestamp; }
		void set_timestamp(long long ts) { _timestamp = ts; }

	private:
		bool _default_prevented = false;
		bool _propagation_stopped = false;
		//bool _immediate_propagation_stopped = true;
		EventUIPhase _phase = EventUIPhase::none;
		std::shared_ptr<View> _target;
		std::shared_ptr<View> _current_target;
		long long _timestamp = 0;

		friend class View;
		friend class ViewImpl;
	};

	// To do: Signals do not support immediate propagation yet
}
