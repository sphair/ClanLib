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

#include "event.h"
#include "../../Core/Math/point.h"

namespace clan
{
	enum class PointerButton
	{
		none,
		left,
		right,
		middle,
		wheel_up,
		wheel_down,
		xbutton1,
		xbutton2,
		xbutton3,
		xbutton4,
		xbutton5,
		xbutton6
	};

	enum class PointerEventType
	{
		none,
		enter,
		leave,
		move,
		press,
		release,
		double_click,
		promixity_change
	};

	class PointerEvent : public EventUI
	{
	public:
		PointerEvent(PointerEventType type, PointerButton button, const Pointf &pos, bool alt_down, bool shift_down, bool ctrl_down, bool cmd_down) :
			_type(type), _button(button), _pos(pos), _alt_down(alt_down), _shift_down(shift_down), _ctrl_down(ctrl_down), _cmd_down(cmd_down)
		{
		}

		PointerEventType type() const { return _type; }
		PointerButton button() const { return _button; }
		Pointf pos(View *view) const;
		Pointf pos(const std::shared_ptr<View> &view) const;

		void set_pos(View *view, const Pointf &pos);

		bool alt_down() const { return _alt_down; }
		bool shift_down() const { return _shift_down; }
		bool ctrl_down() const { return _ctrl_down; }
		bool cmd_down() const { return _cmd_down; }

	private:
		PointerEventType _type = PointerEventType::enter;
		PointerButton _button = PointerButton::none;
		Pointf _pos;
		bool _alt_down = false;
		bool _shift_down = false;
		bool _ctrl_down = false;
		bool _cmd_down = false;
	};
}
