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

#include "Display/precomp.h"
#include "API/Display/Window/keys.h"

namespace clan
{
	Key decode_ic(InputCode ic)
	{
		switch (ic)
		{
		case keycode_backspace: return Key::backspace;
		case keycode_tab: return Key::tab;
		case keycode_clear: return Key::clear;
		case keycode_scroll: return Key::scroll;
		case keycode_return: return Key::key_return;
		case keycode_select: return Key::select;
		case keycode_execute: return Key::execute;
		case keycode_escape: return Key::escape;
		case keycode_pause: return Key::pause;
		case keycode_print: return Key::print;
		case keycode_space: return Key::space;

		case keycode_prior: return Key::prior;
		case keycode_next: return Key::next;
		case keycode_home: return Key::home;
		case keycode_end: return Key::end;
		case keycode_left: return Key::left;
		case keycode_up: return Key::up;
		case keycode_right: return Key::right;
		case keycode_down: return Key::down;

#if !defined(__CYGWIN__) && !defined(__MINGW32__)
		case keycode_kanji: return Key::kanji;
		case keycode_convert: return Key::convert;
		case keycode_nonconvert: return Key::nonconvert;
#endif

		case keycode_help: return Key::help;

		case keycode_0: return Key::key_0;
		case keycode_1: return Key::key_1;
		case keycode_2: return Key::key_2;
		case keycode_3: return Key::key_3;
		case keycode_4: return Key::key_4;
		case keycode_5: return Key::key_5;
		case keycode_6: return Key::key_6;
		case keycode_7: return Key::key_7;
		case keycode_8: return Key::key_8;
		case keycode_9: return Key::key_9;

		case keycode_a: return Key::a;
		case keycode_b: return Key::b;
		case keycode_c: return Key::c;
		case keycode_d: return Key::d;
		case keycode_e: return Key::e;
		case keycode_f: return Key::f;
		case keycode_g: return Key::g;
		case keycode_h: return Key::h;
		case keycode_i: return Key::i;
		case keycode_j: return Key::j;
		case keycode_k: return Key::k;
		case keycode_l: return Key::l;
		case keycode_m: return Key::m;
		case keycode_n: return Key::n;
		case keycode_o: return Key::o;
		case keycode_p: return Key::p;
		case keycode_q: return Key::q;
		case keycode_r: return Key::r;
		case keycode_s: return Key::s;
		case keycode_t: return Key::t;
		case keycode_u: return Key::u;
		case keycode_v: return Key::v;
		case keycode_w: return Key::w;
		case keycode_x: return Key::x;
		case keycode_y: return Key::y;
		case keycode_z: return Key::z;

		case keycode_numpad0: return Key::numpad_0;
		case keycode_numpad1: return Key::numpad_1;
		case keycode_numpad2: return Key::numpad_2;
		case keycode_numpad3: return Key::numpad_3;
		case keycode_numpad4: return Key::numpad_4;
		case keycode_numpad5: return Key::numpad_5;
		case keycode_numpad6: return Key::numpad_6;
		case keycode_numpad7: return Key::numpad_7;
		case keycode_numpad8: return Key::numpad_8;
		case keycode_numpad9: return Key::numpad_9;

		case keycode_multiply: return Key::multiply;
		case keycode_add: return Key::add;
		case keycode_separator: return Key::separator;
		case keycode_subtract: return Key::subtract;
		case keycode_decimal: return Key::decimal;
		case keycode_divide: return Key::divide;

		case keycode_f1: return Key::f1;
		case keycode_f2: return Key::f2;
		case keycode_f3: return Key::f3;
		case keycode_f4: return Key::f4;
		case keycode_f5: return Key::f5;
		case keycode_f6: return Key::f6;
		case keycode_f7: return Key::f7;
		case keycode_f8: return Key::f8;
		case keycode_f9: return Key::f9;
		case keycode_f10: return Key::f10;
		case keycode_f11: return Key::f11;
		case keycode_f12: return Key::f12;
		case keycode_f13: return Key::f13;
		case keycode_f14: return Key::f14;
		case keycode_f15: return Key::f15;
		case keycode_f16: return Key::f16;
		case keycode_f17: return Key::f17;
		case keycode_f18: return Key::f18;
		case keycode_f19: return Key::f19;
		case keycode_f20: return Key::f20;
		case keycode_f21: return Key::f21;
		case keycode_f22: return Key::f22;
		case keycode_f23: return Key::f23;
		case keycode_f24: return Key::f24;

		case keycode_insert: return Key::insert;
		case keycode_delete: return Key::key_delete;

		case keycode_lshift: return Key::lshift;
		case keycode_rshift: return Key::rshift;

		case keycode_lcontrol: return Key::lcontrol;
		case keycode_rcontrol: return Key::rcontrol;

		case keycode_lmenu: return Key::lmenu;
		case keycode_rmenu: return Key::rmenu;

		case keycode_lwin: return Key::lsuper;
		case keycode_rwin: return Key::rsuper;

		// No Alt, ScrollLock or CapsLock on InputCode

		case keycode_numlock: return Key::numlock;

		// Platform-specific aliases
#if defined(WIN32)
		case keycode_shift: return Key::lshift;
		case keycode_control: return Key::lcontrol;
		case keycode_menu: return Key::lmenu;
#elif defined(__APPLE__)
		case keycode_shift  : return Key::lshift;
		case keycode_control: return Key::lcontrol;
		case keycode_menu   : return Key::lmenu;

		case keycode_apps   : return Key::apps;
#else

#if !defined __ANDROID__
		case keycode_numpad_enter: return Key::key_return;
#endif

#endif

		case keycode_unknown:
		default:
			return Key::none;
		}
	}
}
