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
#include <string>

namespace clan
{
	enum class Key
	{
		none = 0,
		backspace = 0x08,
		tab = 0x09,
		clear = 0x0C,
		key_return = 0x0D,
		shift = 0x10,
		control = 0x11,
		menu = 0x12,
		pause = 0x13,
		escape = 0x1B,
		kanji = 0x19,
		convert = 0x1C,
		nonconvert = 0x1D,
		space = 0x20,
		prior = 0x21,
		next = 0x22,
		end = 0x23,
		home = 0x24,
		left = 0x25,
		up = 0x26,
		right = 0x27,
		down = 0x28,
		select = 0x29,
		print = 0x2A,
		execute = 0x2B,
		insert = 0x2D,
		key_delete = 0x2E,
		help = 0x2F,
		key_0 = '0',
		key_1 = '1',
		key_2 = '2',
		key_3 = '3',
		key_4 = '4',
		key_5 = '5',
		key_6 = '6',
		key_7 = '7',
		key_8 = '8',
		key_9 = '9',
		a = 'A',
		b = 'B',
		c = 'C',
		d = 'D',
		e = 'E',
		f = 'F',
		g = 'G',
		h = 'H',
		i = 'I',
		j = 'J',
		k = 'K',
		l = 'L',
		m = 'M',
		n = 'N',
		o = 'O',
		p = 'P',
		q = 'Q',
		r = 'R',
		s = 'S',
		t = 'T',
		u = 'U',
		v = 'V',
		w = 'W',
		x = 'X',
		y = 'Y',
		z = 'Z',
		lwin = 0x5B,
		rwin = 0x5C,
		apps = 0x5D,
		numpad0 = 0x60,
		numpad1 = 0x61,
		numpad2 = 0x62,
		numpad3 = 0x63,
		numpad4 = 0x64,
		numpad5 = 0x65,
		numpad6 = 0x66,
		numpad7 = 0x67,
		numpad8 = 0x68,
		numpad9 = 0x69,
		multiply = 0x6A,
		add = 0x6B,
		separator = 0x6C,
		subtract = 0x6D,
		decimal = 0x6E,
		divide = 0x6F,
		f1 = 0x70,
		f2 = 0x71,
		f3 = 0x72,
		f4 = 0x73,
		f5 = 0x74,
		f6 = 0x75,
		f7 = 0x76,
		f8 = 0x77,
		f9 = 0x78,
		f10 = 0x79,
		f11 = 0x7A,
		f12 = 0x7B,
		f13 = 0x7C,
		f14 = 0x7D,
		f15 = 0x7E,
		f16 = 0x7F,
		f17 = 0x80,
		f18 = 0x81,
		f19 = 0x82,
		f20 = 0x83,
		f21 = 0x84,
		f22 = 0x85,
		f23 = 0x86,
		f24 = 0x87,
		numlock = 0x90,
		scroll = 0x91,
		lshift = 0xA0,
		rshift = 0xA1,
		lcontrol = 0xA2,
		rcontrol = 0xA3,
		lmenu = 0xA4,
		rmenu = 0xA5
	};

	enum class KeyEventType
	{
		none,
		press,
		release
	};

	class KeyEvent : public EventUI
	{
	public:
		KeyEvent(KeyEventType type, Key key, int repeat_count, const std::string &text, const Pointf &pointer_pos, bool alt_down, bool shift_down, bool ctrl_down, bool cmd_down) :
			_type(type), _key(key), _repeat_count(repeat_count), _text(text), _pointer_pos(pointer_pos), _alt_down(alt_down), _shift_down(shift_down), _ctrl_down(ctrl_down), _cmd_down(cmd_down)
		{
		}

		KeyEventType type() const { return _type; }

		Key key() const { return _key; }
		int repeat_count() const { return _repeat_count; }

		const std::string &text() const { return _text; }

		Pointf pointer_pos() const { return _pointer_pos; }

		bool alt_down() const { return _alt_down; }
		bool shift_down() const { return _shift_down; }
		bool ctrl_down() const { return _ctrl_down; }
		bool cmd_down() const { return _cmd_down; }

	private:
		KeyEventType _type = KeyEventType::none;
		Key _key = Key::none;
		int _repeat_count = 0;
		std::string _text;
		Pointf _pointer_pos;
		bool _alt_down = false;
		bool _shift_down = false;
		bool _ctrl_down = false;
		bool _cmd_down = false;
	};
}
