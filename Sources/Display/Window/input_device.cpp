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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Display/Window/keys.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "input_device_impl.h"
#include <map>

namespace clan
{
	InputDevice::InputDevice()
	{
	}

	InputDevice::InputDevice(InputDeviceProvider *provider)
		: impl(std::make_shared<InputDevice_Impl>())
	{
		impl->provider.reset(provider);
	}

	InputDevice::InputDevice(std::weak_ptr<InputDevice_Impl> impl)
		: impl(impl.lock())
	{
	}

	InputDevice::~InputDevice()
	{
	}

	void InputDevice::throw_if_null() const
	{
		if (!impl)
			throw Exception("InputDevice is null");
	}

	InputDeviceProvider *InputDevice::get_provider() const
	{
		if (!impl)
			return nullptr;
		else
			return impl->provider.get();
	}

	std::string InputDevice::get_name() const
	{
		if (impl->provider)
			return impl->provider->get_name();
		else
			return std::string();
	}

	std::string InputDevice::get_device_name() const
	{
		if (impl->provider)
			return impl->provider->get_device_name();
		else
			return std::string();
	}

	InputDevice::Type InputDevice::get_type() const
	{
		if (impl->provider)
			return impl->provider->get_type();
		else
			return unknown;
	}

	std::string InputDevice::get_key_name(int id) const
	{
		if (impl->provider)
			return impl->provider->get_key_name(id);
		else
			return std::string();
	}

	std::string InputDevice::keyid_to_string(int keycode) const
	{
		if (impl->provider)
		{
			if (impl->provider->supports_keyid_mapping())
				return impl->provider->keyid_to_string(keycode);

			switch (keycode)
			{
			case keycode_backspace: return "backspace";
			case keycode_tab: return "tab";
			case keycode_clear: return "clear";
			case keycode_return: return "return";
#ifdef WIN32
			case keycode_shift: return "shift";
			case keycode_control: return "control";
#endif
#if !defined(__CYGWIN__) && !defined(__MINGW32__)
			case keycode_kanji: return "kanji";
			case keycode_convert: return "convert";
			case keycode_nonconvert: return "nonconvert";
#endif
			case keycode_menu: return "menu";
			case keycode_pause: return "pause";
			case keycode_escape: return "escape";
			case keycode_space: return "space";
			case keycode_prior: return "prior";
			case keycode_next: return "next";
			case keycode_end: return "end";
			case keycode_home: return "home";
			case keycode_left: return "left";
			case keycode_up: return "up";
			case keycode_right: return "right";
			case keycode_down: return "down";
			case keycode_select: return "select";
			case keycode_print: return "print";
			case keycode_execute: return "execute";
			case keycode_insert: return "insert";
			case keycode_delete: return "delete";
			case keycode_help: return "help";
			case keycode_0: return "0";
			case keycode_1: return "1";
			case keycode_2: return "2";
			case keycode_3: return "3";
			case keycode_4: return "4";
			case keycode_5: return "5";
			case keycode_6: return "6";
			case keycode_7: return "7";
			case keycode_8: return "8";
			case keycode_9: return "9";
			case keycode_a: return "a";
			case keycode_b: return "b";
			case keycode_c: return "c";
			case keycode_d: return "d";
			case keycode_e: return "e";
			case keycode_f: return "f";
			case keycode_g: return "g";
			case keycode_h: return "h";
			case keycode_i: return "i";
			case keycode_j: return "j";
			case keycode_k: return "k";
			case keycode_l: return "l";
			case keycode_m: return "m";
			case keycode_n: return "n";
			case keycode_o: return "o";
			case keycode_p: return "p";
			case keycode_q: return "q";
			case keycode_r: return "r";
			case keycode_s: return "s";
			case keycode_t: return "t";
			case keycode_u: return "u";
			case keycode_v: return "v";
			case keycode_w: return "w";
			case keycode_x: return "x";
			case keycode_y: return "y";
			case keycode_z: return "z";
			case keycode_lwin: return "lwin";
			case keycode_rwin: return "rwin";
			case keycode_numpad0: return "numpad0";
			case keycode_numpad1: return "numpad1";
			case keycode_numpad2: return "numpad2";
			case keycode_numpad3: return "numpad3";
			case keycode_numpad4: return "numpad4";
			case keycode_numpad5: return "numpad5";
			case keycode_numpad6: return "numpad6";
			case keycode_numpad7: return "numpad7";
			case keycode_numpad8: return "numpad8";
			case keycode_numpad9: return "numpad9";
#if !defined __ANDROID__ && ! defined __APPLE__ && ! defined WIN32
			case keycode_numpad_enter: return "numpad_enter";
#endif
			case keycode_multiply: return "multiply";
			case keycode_add: return "add";
			case keycode_separator: return "separator";
			case keycode_subtract: return "subtract";
			case keycode_decimal: return "decimal";
			case keycode_divide: return "divide";
			case keycode_f1: return "f1";
			case keycode_f2: return "f2";
			case keycode_f3: return "f3";
			case keycode_f4: return "f4";
			case keycode_f5: return "f5";
			case keycode_f6: return "f6";
			case keycode_f7: return "f7";
			case keycode_f8: return "f8";
			case keycode_f9: return "f9";
			case keycode_f10: return "f10";
			case keycode_f11: return "f11";
			case keycode_f12: return "f12";
			case keycode_f13: return "f13";
			case keycode_f14: return "f14";
			case keycode_f15: return "f15";
			case keycode_f16: return "f16";
			case keycode_f17: return "f17";
			case keycode_f18: return "f18";
			case keycode_f19: return "f19";
			case keycode_f20: return "f20";
			case keycode_f21: return "f21";
			case keycode_f22: return "f22";
			case keycode_f23: return "f23";
			case keycode_f24: return "f24";
			case keycode_numlock: return "numlock";
			case keycode_scroll: return "scroll";
			case keycode_lshift: return "lshift";
			case keycode_rshift: return "rshift";
			case keycode_lcontrol: return "lcontrol";
			case keycode_rcontrol: return "rcontrol";
			case keycode_lmenu: return "lmenu";
			case keycode_rmenu: return "rmenu";
			default:
				return string_format("key_%1", keycode);
			}
		}
		else
		{
			return std::string();
		}
	}

	int InputDevice::string_to_keyid(const std::string &str) const
	{
		if (impl->provider)
		{
			if (impl->provider->supports_keyid_mapping())
				return impl->provider->string_to_keyid(str);

			if (str.substr(0, 4) == "key_")
			{
				return StringHelp::text_to_int(str.substr(4));
			}

			static std::map<std::string, int> string_to_id;
			static bool first_call = true;
			if (first_call)
			{
				first_call = false;
				string_to_id["backspace"] = keycode_backspace;
				string_to_id["tab"] = keycode_tab;
				string_to_id["clear"] = keycode_clear;
				string_to_id["return"] = keycode_return;
#ifdef WIN32
				string_to_id["shift"] = keycode_shift;
				string_to_id["control"] = keycode_control;
#endif
#if !defined(__CYGWIN__) && !defined(__MINGW32__)
				string_to_id["kanji"] = keycode_kanji;
				string_to_id["convert"] = keycode_convert;
				string_to_id["nonconvert"] = keycode_nonconvert;
#endif
				string_to_id["menu"] = keycode_menu;
				string_to_id["pause"] = keycode_pause;
				string_to_id["escape"] = keycode_escape;
				string_to_id["space"] = keycode_space;
				string_to_id["prior"] = keycode_prior;
				string_to_id["next"] = keycode_next;
				string_to_id["end"] = keycode_end;
				string_to_id["home"] = keycode_home;
				string_to_id["left"] = keycode_left;
				string_to_id["up"] = keycode_up;
				string_to_id["right"] = keycode_right;
				string_to_id["down"] = keycode_down;
				string_to_id["select"] = keycode_select;
				string_to_id["print"] = keycode_print;
				string_to_id["execute"] = keycode_execute;
				string_to_id["insert"] = keycode_insert;
				string_to_id["delete"] = keycode_delete;
				string_to_id["help"] = keycode_help;
				string_to_id["0"] = keycode_0;
				string_to_id["1"] = keycode_1;
				string_to_id["2"] = keycode_2;
				string_to_id["3"] = keycode_3;
				string_to_id["4"] = keycode_4;
				string_to_id["5"] = keycode_5;
				string_to_id["6"] = keycode_6;
				string_to_id["7"] = keycode_7;
				string_to_id["8"] = keycode_8;
				string_to_id["9"] = keycode_9;
				string_to_id["a"] = keycode_a;
				string_to_id["b"] = keycode_b;
				string_to_id["c"] = keycode_c;
				string_to_id["d"] = keycode_d;
				string_to_id["e"] = keycode_e;
				string_to_id["f"] = keycode_f;
				string_to_id["g"] = keycode_g;
				string_to_id["h"] = keycode_h;
				string_to_id["i"] = keycode_i;
				string_to_id["j"] = keycode_j;
				string_to_id["k"] = keycode_k;
				string_to_id["l"] = keycode_l;
				string_to_id["m"] = keycode_m;
				string_to_id["n"] = keycode_n;
				string_to_id["o"] = keycode_o;
				string_to_id["p"] = keycode_p;
				string_to_id["q"] = keycode_q;
				string_to_id["r"] = keycode_r;
				string_to_id["s"] = keycode_s;
				string_to_id["t"] = keycode_t;
				string_to_id["u"] = keycode_u;
				string_to_id["v"] = keycode_v;
				string_to_id["w"] = keycode_w;
				string_to_id["x"] = keycode_x;
				string_to_id["y"] = keycode_y;
				string_to_id["z"] = keycode_z;
				string_to_id["lwin"] = keycode_lwin;
				string_to_id["rwin"] = keycode_rwin;
				string_to_id["numpad0"] = keycode_numpad0;
				string_to_id["numpad1"] = keycode_numpad1;
				string_to_id["numpad2"] = keycode_numpad2;
				string_to_id["numpad3"] = keycode_numpad3;
				string_to_id["numpad4"] = keycode_numpad4;
				string_to_id["numpad5"] = keycode_numpad5;
				string_to_id["numpad6"] = keycode_numpad6;
				string_to_id["numpad7"] = keycode_numpad7;
				string_to_id["numpad8"] = keycode_numpad8;
				string_to_id["numpad9"] = keycode_numpad9;
				string_to_id["numpad_enter"] = keycode_numpad_enter;
				string_to_id["multiply"] = keycode_multiply;
				string_to_id["add"] = keycode_add;
				string_to_id["separator"] = keycode_separator;
				string_to_id["subtract"] = keycode_subtract;
				string_to_id["decimal"] = keycode_decimal;
				string_to_id["divide"] = keycode_divide;
				string_to_id["f1"] = keycode_f1;
				string_to_id["f2"] = keycode_f2;
				string_to_id["f3"] = keycode_f3;
				string_to_id["f4"] = keycode_f4;
				string_to_id["f5"] = keycode_f5;
				string_to_id["f6"] = keycode_f6;
				string_to_id["f7"] = keycode_f7;
				string_to_id["f8"] = keycode_f8;
				string_to_id["f9"] = keycode_f9;
				string_to_id["f10"] = keycode_f10;
				string_to_id["f11"] = keycode_f11;
				string_to_id["f12"] = keycode_f12;
				string_to_id["f13"] = keycode_f13;
				string_to_id["f14"] = keycode_f14;
				string_to_id["f15"] = keycode_f15;
				string_to_id["f16"] = keycode_f16;
				string_to_id["f17"] = keycode_f17;
				string_to_id["f18"] = keycode_f18;
				string_to_id["f19"] = keycode_f19;
				string_to_id["f20"] = keycode_f20;
				string_to_id["f21"] = keycode_f21;
				string_to_id["f22"] = keycode_f22;
				string_to_id["f23"] = keycode_f23;
				string_to_id["f24"] = keycode_f24;
				string_to_id["numlock"] = keycode_numlock;
				string_to_id["scroll"] = keycode_scroll;
				string_to_id["lshift"] = keycode_lshift;
				string_to_id["rshift"] = keycode_rshift;
				string_to_id["lcontrol"] = keycode_lcontrol;
				string_to_id["rcontrol"] = keycode_rcontrol;
				string_to_id["lmenu"] = keycode_lmenu;
				string_to_id["rmenu"] = keycode_rmenu;
			}

			return string_to_id[StringHelp::text_to_local8(str)];
		}
		else
			return 0;
	}

	bool InputDevice::get_keycode(int keycode) const
	{
		if (impl->provider)
			return impl->provider->get_keycode(keycode);
		else
			return false;
	}

	Pointf InputDevice::get_position() const
	{
		if (impl->provider)
			return impl->provider->get_position();
		else
			return Pointf(0.f, 0.f);
	}

	Point InputDevice::get_device_position() const
	{
		if (impl->provider)
			return impl->provider->get_device_position();
		else
			return Point(0, 0);
	}

	float InputDevice::get_axis(int index) const
	{
		if (impl->provider)
			return impl->provider->get_axis(index);
		else
			return 0.0f;
	}

	std::vector<int> InputDevice::get_axis_ids() const
	{
		if (impl->provider)
			return impl->provider->get_axis_ids();
		else
			return std::vector<int>();
	}

	int InputDevice::get_hat(int index) const
	{
		if (impl->provider)
			return impl->provider->get_hat(index);
		else
			return 0;
	}

	int InputDevice::get_button_count() const
	{
		if (impl->provider)
			return impl->provider->get_button_count();
		else
			return 0;
	}

	InputDevice &InputDevice::operator =(const InputDevice &copy)
	{
		impl = copy.impl;
		return *this;
	}

	void InputDevice::set_position(float x, float y)
	{
		if (impl->provider)
			impl->provider->set_position(x, y);
	}

	void InputDevice::set_device_position(int x, int y)
	{
		if (impl->provider)
			impl->provider->set_device_position(x, y);
	}

	Signal<void(const InputEvent &)> &InputDevice::sig_key_down()
	{
		return impl->sig_key_down;
	}

	Signal<void(const InputEvent &)> &InputDevice::sig_key_up()
	{
		return impl->sig_key_up;
	}

	Signal<void(const InputEvent &)> &InputDevice::sig_pointer_move()
	{
		return impl->sig_pointer_move;
	}

	Signal<void(const InputEvent &)> &InputDevice::sig_axis_move()
	{
		return impl->sig_axis_move;
	}

	Signal<void(const InputEvent &)> &InputDevice::sig_key_dblclk()
	{
		return impl->sig_key_dblclk;
	}

	Signal<void(const InputEvent &)> & InputDevice::sig_proximity_change()
	{
		return impl->sig_proximity_change;
	}
}
