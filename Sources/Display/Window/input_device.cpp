/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Display/Window/input_context.h"
#include "API/Display/Window/keys.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "input_device_impl.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice Construction:

CL_InputDevice::CL_InputDevice()
{
}

CL_InputDevice::CL_InputDevice(CL_InputDeviceProvider *provider)
: impl(new CL_InputDevice_Impl)
{
	impl->input_device = impl;
	impl->provider = provider;
	provider->init(&impl->sig_provider_event);
}

CL_InputDevice::CL_InputDevice(const CL_InputDevice &copy)
: impl(copy.impl)
{
}

CL_InputDevice::CL_InputDevice(CL_WeakPtr<CL_InputDevice_Impl> impl)
: impl(impl.to_sharedptr())
{
}

CL_InputDevice::~CL_InputDevice()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice Attributes:

CL_InputDeviceProvider *CL_InputDevice::get_provider() const
{
	if (impl.is_null())
		return 0;
	else
		return impl->provider;
}

CL_String CL_InputDevice::get_name() const
{
	if (impl->provider)
		return impl->provider->get_name();
	else
		return CL_String();
}

CL_String CL_InputDevice::get_device_name() const
{
	if (impl->provider)
		return impl->provider->get_device_name();
	else
		return CL_String();
}

CL_InputDevice::Type CL_InputDevice::get_type() const
{
	if (impl->provider)
		return impl->provider->get_type();
	else
		return unknown;
}

CL_String CL_InputDevice::get_key_name(int id) const
{
	if (impl->provider)
		return impl->provider->get_key_name(id);
	else
		return CL_String();
}

CL_String CL_InputDevice::keyid_to_string(int keycode) const
{
	if (impl->provider)
	{
		if (impl->provider->supports_keyid_mapping())
			return impl->provider->keyid_to_string(keycode);

		switch (keycode)
		{
		case CL_KEY_BACKSPACE: return cl_text("backspace");
		case CL_KEY_TAB: return cl_text("tab");
		case CL_KEY_CLEAR: return cl_text("clear");
		case CL_KEY_RETURN: return cl_text("return");
		#ifdef WIN32
		case CL_KEY_SHIFT: return cl_text("shift");
		case CL_KEY_CONTROL: return cl_text("control");
		#endif
		#if !defined(__CYGWIN__) && !defined(__MINGW32__)
		case CL_KEY_KANJI: return cl_text("kanji");
		case CL_KEY_CONVERT: return cl_text("convert");
		case CL_KEY_NONCONVERT: return cl_text("nonconvert");
		#endif
		case CL_KEY_MENU: return cl_text("menu");
		case CL_KEY_PAUSE: return cl_text("pause");
		case CL_KEY_ESCAPE: return cl_text("escape");
		case CL_KEY_SPACE: return cl_text("space");
		case CL_KEY_PRIOR: return cl_text("prior");
		case CL_KEY_NEXT: return cl_text("next");
		case CL_KEY_END: return cl_text("end");
		case CL_KEY_HOME: return cl_text("home");
		case CL_KEY_LEFT: return cl_text("left");
		case CL_KEY_UP: return cl_text("up");
		case CL_KEY_RIGHT: return cl_text("right");
		case CL_KEY_DOWN: return cl_text("down");
		case CL_KEY_SELECT: return cl_text("select");
		case CL_KEY_PRINT: return cl_text("print");
		case CL_KEY_EXECUTE: return cl_text("execute");
		case CL_KEY_INSERT: return cl_text("insert");
		case CL_KEY_DELETE: return cl_text("delete");
		case CL_KEY_HELP: return cl_text("help");
		case CL_KEY_0: return cl_text("0");
		case CL_KEY_1: return cl_text("1");
		case CL_KEY_2: return cl_text("2");
		case CL_KEY_3: return cl_text("3");
		case CL_KEY_4: return cl_text("4");
		case CL_KEY_5: return cl_text("5");
		case CL_KEY_6: return cl_text("6");
		case CL_KEY_7: return cl_text("7");
		case CL_KEY_8: return cl_text("8");
		case CL_KEY_9: return cl_text("9");
		case CL_KEY_A: return cl_text("a");
		case CL_KEY_B: return cl_text("b");
		case CL_KEY_C: return cl_text("c");
		case CL_KEY_D: return cl_text("d");
		case CL_KEY_E: return cl_text("e");
		case CL_KEY_F: return cl_text("f");
		case CL_KEY_G: return cl_text("g");
		case CL_KEY_H: return cl_text("h");
		case CL_KEY_I: return cl_text("i");
		case CL_KEY_J: return cl_text("j");
		case CL_KEY_K: return cl_text("k");
		case CL_KEY_L: return cl_text("l");
		case CL_KEY_M: return cl_text("m");
		case CL_KEY_N: return cl_text("n");
		case CL_KEY_O: return cl_text("o");
		case CL_KEY_P: return cl_text("p");
		case CL_KEY_Q: return cl_text("q");
		case CL_KEY_R: return cl_text("r");
		case CL_KEY_S: return cl_text("s");
		case CL_KEY_T: return cl_text("t");
		case CL_KEY_U: return cl_text("u");
		case CL_KEY_V: return cl_text("v");
		case CL_KEY_W: return cl_text("w");
		case CL_KEY_X: return cl_text("x");
		case CL_KEY_Y: return cl_text("y");
		case CL_KEY_Z: return cl_text("z");
		case CL_KEY_LWIN: return cl_text("lwin");
		case CL_KEY_RWIN: return cl_text("rwin");
		case CL_KEY_NUMPAD0: return cl_text("numpad0");
		case CL_KEY_NUMPAD1: return cl_text("numpad1");
		case CL_KEY_NUMPAD2: return cl_text("numpad2");
		case CL_KEY_NUMPAD3: return cl_text("numpad3");
		case CL_KEY_NUMPAD4: return cl_text("numpad4");
		case CL_KEY_NUMPAD5: return cl_text("numpad5");
		case CL_KEY_NUMPAD6: return cl_text("numpad6");
		case CL_KEY_NUMPAD7: return cl_text("numpad7");
		case CL_KEY_NUMPAD8: return cl_text("numpad8");
		case CL_KEY_NUMPAD9: return cl_text("numpad9");
		case CL_KEY_MULTIPLY: return cl_text("multiply");
		case CL_KEY_ADD: return cl_text("add");
		case CL_KEY_SEPARATOR: return cl_text("separator");
		case CL_KEY_SUBTRACT: return cl_text("subtract");
		case CL_KEY_DECIMAL: return cl_text("decimal");
		case CL_KEY_DIVIDE: return cl_text("divide");
		case CL_KEY_F1: return cl_text("f1");
		case CL_KEY_F2: return cl_text("f2");
		case CL_KEY_F3: return cl_text("f3");
		case CL_KEY_F4: return cl_text("f4");
		case CL_KEY_F5: return cl_text("f5");
		case CL_KEY_F6: return cl_text("f6");
		case CL_KEY_F7: return cl_text("f7");
		case CL_KEY_F8: return cl_text("f8");
		case CL_KEY_F9: return cl_text("f9");
		case CL_KEY_F10: return cl_text("f10");
		case CL_KEY_F11: return cl_text("f11");
		case CL_KEY_F12: return cl_text("f12");
		case CL_KEY_F13: return cl_text("f13");
		case CL_KEY_F14: return cl_text("f14");
		case CL_KEY_F15: return cl_text("f15");
		case CL_KEY_F16: return cl_text("f16");
		case CL_KEY_F17: return cl_text("f17");
		case CL_KEY_F18: return cl_text("f18");
		case CL_KEY_F19: return cl_text("f19");
		case CL_KEY_F20: return cl_text("f20");
		case CL_KEY_F21: return cl_text("f21");
		case CL_KEY_F22: return cl_text("f22");
		case CL_KEY_F23: return cl_text("f23");
		case CL_KEY_F24: return cl_text("f24");
		case CL_KEY_NUMLOCK: return cl_text("numlock");
		case CL_KEY_SCROLL: return cl_text("scroll");
		case CL_KEY_LSHIFT: return cl_text("lshift");
		case CL_KEY_RSHIFT: return cl_text("rshift");
		case CL_KEY_LCONTROL: return cl_text("lcontrol");
		case CL_KEY_RCONTROL: return cl_text("rcontrol");
		case CL_KEY_LMENU: return cl_text("lmenu");
		case CL_KEY_RMENU: return cl_text("rmenu");
		default:
			return cl_format(cl_text("key_%1"), keycode);
		}
	}
	else
	{
		return CL_String();
	}
}

int CL_InputDevice::string_to_keyid(const CL_String &str) const
{
	if (impl->provider)
	{
		if (impl->provider->supports_keyid_mapping())
			return impl->provider->string_to_keyid(str);

		if (str.substr(0, 4) == cl_text("key_"))
		{
			return CL_StringHelp::text_to_int(str.substr(4));
		}

		static std::map<CL_String8, int> string_to_id;
		static bool first_call = true;
		if (first_call)
		{
			first_call = false;
			string_to_id["backspace"] = CL_KEY_BACKSPACE;
			string_to_id["tab"] = CL_KEY_TAB;
			string_to_id["clear"] = CL_KEY_CLEAR;
			string_to_id["return"] = CL_KEY_RETURN;
			#ifdef WIN32
			string_to_id["shift"] = CL_KEY_SHIFT;
			string_to_id["control"] = CL_KEY_CONTROL;
			#endif
			#if !defined(__CYGWIN__) && !defined(__MINGW32__)
			string_to_id["kanji"] = CL_KEY_KANJI;
			string_to_id["convert"] = CL_KEY_CONVERT;
			string_to_id["nonconvert"] = CL_KEY_NONCONVERT;
			#endif
			string_to_id["menu"] = CL_KEY_MENU;
			string_to_id["pause"] = CL_KEY_PAUSE;
			string_to_id["escape"] = CL_KEY_ESCAPE;
			string_to_id["space"] = CL_KEY_SPACE;
			string_to_id["prior"] = CL_KEY_PRIOR;
			string_to_id["next"] = CL_KEY_NEXT;
			string_to_id["end"] = CL_KEY_END;
			string_to_id["home"] = CL_KEY_HOME;
			string_to_id["left"] = CL_KEY_LEFT;
			string_to_id["up"] = CL_KEY_UP;
			string_to_id["right"] = CL_KEY_RIGHT;
			string_to_id["down"] = CL_KEY_DOWN;
			string_to_id["select"] = CL_KEY_SELECT;
			string_to_id["print"] = CL_KEY_PRINT;
			string_to_id["execute"] = CL_KEY_EXECUTE;
			string_to_id["insert"] = CL_KEY_INSERT;
			string_to_id["delete"] = CL_KEY_DELETE;
			string_to_id["help"] = CL_KEY_HELP;
			string_to_id["0"] = CL_KEY_0;
			string_to_id["1"] = CL_KEY_1;
			string_to_id["2"] = CL_KEY_2;
			string_to_id["3"] = CL_KEY_3;
			string_to_id["4"] = CL_KEY_4;
			string_to_id["5"] = CL_KEY_5;
			string_to_id["6"] = CL_KEY_6;
			string_to_id["7"] = CL_KEY_7;
			string_to_id["8"] = CL_KEY_8;
			string_to_id["9"] = CL_KEY_9;
			string_to_id["a"] = CL_KEY_A;
			string_to_id["b"] = CL_KEY_B;
			string_to_id["c"] = CL_KEY_C;
			string_to_id["d"] = CL_KEY_D;
			string_to_id["e"] = CL_KEY_E;
			string_to_id["f"] = CL_KEY_F;
			string_to_id["g"] = CL_KEY_G;
			string_to_id["h"] = CL_KEY_H;
			string_to_id["i"] = CL_KEY_I;
			string_to_id["j"] = CL_KEY_J;
			string_to_id["k"] = CL_KEY_K;
			string_to_id["l"] = CL_KEY_L;
			string_to_id["m"] = CL_KEY_M;
			string_to_id["n"] = CL_KEY_N;
			string_to_id["o"] = CL_KEY_O;
			string_to_id["p"] = CL_KEY_P;
			string_to_id["q"] = CL_KEY_Q;
			string_to_id["r"] = CL_KEY_R;
			string_to_id["s"] = CL_KEY_S;
			string_to_id["t"] = CL_KEY_T;
			string_to_id["u"] = CL_KEY_U;
			string_to_id["v"] = CL_KEY_V;
			string_to_id["w"] = CL_KEY_W;
			string_to_id["x"] = CL_KEY_X;
			string_to_id["y"] = CL_KEY_Y;
			string_to_id["z"] = CL_KEY_Z;
			string_to_id["lwin"] = CL_KEY_LWIN;
			string_to_id["rwin"] = CL_KEY_RWIN;
			string_to_id["numpad0"] = CL_KEY_NUMPAD0;
			string_to_id["numpad1"] = CL_KEY_NUMPAD1;
			string_to_id["numpad2"] = CL_KEY_NUMPAD2;
			string_to_id["numpad3"] = CL_KEY_NUMPAD3;
			string_to_id["numpad4"] = CL_KEY_NUMPAD4;
			string_to_id["numpad5"] = CL_KEY_NUMPAD5;
			string_to_id["numpad6"] = CL_KEY_NUMPAD6;
			string_to_id["numpad7"] = CL_KEY_NUMPAD7;
			string_to_id["numpad8"] = CL_KEY_NUMPAD8;
			string_to_id["numpad9"] = CL_KEY_NUMPAD9;
			string_to_id["multiply"] = CL_KEY_MULTIPLY;
			string_to_id["add"] = CL_KEY_ADD;
			string_to_id["separator"] = CL_KEY_SEPARATOR;
			string_to_id["subtract"] = CL_KEY_SUBTRACT;
			string_to_id["decimal"] = CL_KEY_DECIMAL;
			string_to_id["divide"] = CL_KEY_DIVIDE;
			string_to_id["f1"] = CL_KEY_F1;
			string_to_id["f2"] = CL_KEY_F2;
			string_to_id["f3"] = CL_KEY_F3;
			string_to_id["f4"] = CL_KEY_F4;
			string_to_id["f5"] = CL_KEY_F5;
			string_to_id["f6"] = CL_KEY_F6;
			string_to_id["f7"] = CL_KEY_F7;
			string_to_id["f8"] = CL_KEY_F8;
			string_to_id["f9"] = CL_KEY_F9;
			string_to_id["f10"] = CL_KEY_F10;
			string_to_id["f11"] = CL_KEY_F11;
			string_to_id["f12"] = CL_KEY_F12;
			string_to_id["f13"] = CL_KEY_F13;
			string_to_id["f14"] = CL_KEY_F14;
			string_to_id["f15"] = CL_KEY_F15;
			string_to_id["f16"] = CL_KEY_F16;
			string_to_id["f17"] = CL_KEY_F17;
			string_to_id["f18"] = CL_KEY_F18;
			string_to_id["f19"] = CL_KEY_F19;
			string_to_id["f20"] = CL_KEY_F20;
			string_to_id["f21"] = CL_KEY_F21;
			string_to_id["f22"] = CL_KEY_F22;
			string_to_id["f23"] = CL_KEY_F23;
			string_to_id["f24"] = CL_KEY_F24;
			string_to_id["numlock"] = CL_KEY_NUMLOCK;
			string_to_id["scroll"] = CL_KEY_SCROLL;
			string_to_id["lshift"] = CL_KEY_LSHIFT;
			string_to_id["rshift"] = CL_KEY_RSHIFT;
			string_to_id["lcontrol"] = CL_KEY_LCONTROL;
			string_to_id["rcontrol"] = CL_KEY_RCONTROL;
			string_to_id["lmenu"] = CL_KEY_LMENU;
			string_to_id["rmenu"] = CL_KEY_RMENU;
		}

		return string_to_id[CL_StringHelp::text_to_local8(str)];
	}
	else
		return 0;
}

bool CL_InputDevice::get_keycode(int keycode) const
{
	if (impl->provider)
		return impl->provider->get_keycode(keycode);
	else
		return false;
}

CL_Point CL_InputDevice::get_position() const
{
	if (impl->provider)
		return CL_Point(impl->provider->get_x(), impl->provider->get_y());
	else
		return CL_Point(0,0);
}

int CL_InputDevice::get_x() const
{
	if (impl->provider)
		return impl->provider->get_x();
	else
		return 0;
}

int CL_InputDevice::get_y() const
{
	if (impl->provider)
		return impl->provider->get_y();
	else
		return 0;
}

double CL_InputDevice::get_axis(int index) const
{
	if (impl->provider)
		return impl->provider->get_axis(index);
	else
		return 0.0f;
}

int CL_InputDevice::get_axis_count() const
{
	if (impl->provider)
		return impl->provider->get_axis_count();
	else
		return 0;
}

int CL_InputDevice::get_button_count() const
{
	if (impl->provider)
		return impl->provider->get_button_count();
	else
		return 0;
}

bool CL_InputDevice::in_proximity() const
{
	if (impl->provider)
		return impl->provider->in_proximity();
	else
		return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice Operations:

CL_InputDevice &CL_InputDevice::operator =(const CL_InputDevice &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_InputDevice::set_position(int x, int y)
{
	if (impl->provider)
		impl->provider->set_position(x, y);
}

bool CL_InputDevice::poll(bool peek_only)
{
	if (impl->provider)
	{
		return impl->provider->poll(peek_only);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice Signals:

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &CL_InputDevice::sig_key_down()
{
	return impl->sig_key_down;
}

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &CL_InputDevice::sig_key_up()
{
	return impl->sig_key_up;
}

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &CL_InputDevice::sig_pointer_move()
{
	return impl->sig_pointer_move;
}

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &CL_InputDevice::sig_ball_move()
{
	return impl->sig_ball_move;
}

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &CL_InputDevice::sig_axis_move()
{
	return impl->sig_axis_move;
}

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &CL_InputDevice::sig_key_dblclk()
{
	return impl->sig_key_dblclk;
}

CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> & CL_InputDevice::sig_proximity_change()
{
	return impl->sig_proximity_change;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice Implementation:

