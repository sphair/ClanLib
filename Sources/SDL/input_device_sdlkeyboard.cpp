/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "API/Display/input_device.h"
#include "API/Display/input_event.h"
#include "API/Core/System/clanstring.h"
#include "API/Display/keys.h"
#include "input_device_sdlkeyboard.h"
#include "display_window_sdl.h"
#include <map>

static std::map<int,int > sdl_to_clanlib_keys;

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLKeyboard construction:

CL_InputDevice_SDLKeyboard::CL_InputDevice_SDLKeyboard(CL_DisplayWindow_SDL *owner) :
	owner(owner)
{
	type = CL_InputDevice::keyboard;
	slot_sdlevent = owner->sig_sdl_keyboard_event.connect(this, &CL_InputDevice_SDLKeyboard::handle_keyboard_event);
}

CL_InputDevice_SDLKeyboard::~CL_InputDevice_SDLKeyboard()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLKeyboard attributes:

float CL_InputDevice_SDLKeyboard::get_axis(int index) const
{
	return 0.0f;
}

std::string CL_InputDevice_SDLKeyboard::get_name() const
{
	return "System Keyboard";
}

int CL_InputDevice_SDLKeyboard::get_axis_count() const
{
	return 0;
}

int CL_InputDevice_SDLKeyboard::get_button_count() const
{
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLKeyboard operations:

#ifdef WIN32
#define USE_CL_KEYS
#endif
#ifdef __APPLE__
#define USE_CL_KEYS
#endif

std::string CL_InputDevice_SDLKeyboard::get_key_name(int virtual_key) const
{
	static std::map<int, SDLKey> clanlib_to_sdl_keys;
	static bool first_call = true;

	if (first_call)
	{
		first_call = false;
#ifdef USE_CL_KEYS
		clanlib_to_sdl_keys[CL_KEY_BACKSPACE] = SDLK_BACKSPACE;
		clanlib_to_sdl_keys[CL_KEY_TAB] = SDLK_TAB;
		clanlib_to_sdl_keys[CL_KEY_CLEAR] = SDLK_CLEAR;
		clanlib_to_sdl_keys[CL_KEY_RETURN] = SDLK_RETURN;
		clanlib_to_sdl_keys[CL_KEY_PAUSE] = SDLK_PAUSE;
		clanlib_to_sdl_keys[CL_KEY_ESCAPE] = SDLK_ESCAPE;
		clanlib_to_sdl_keys[CL_KEY_SPACE] = SDLK_SPACE;
		clanlib_to_sdl_keys[CL_KEY_0] = SDLK_0;
		clanlib_to_sdl_keys[CL_KEY_1] = SDLK_1;
		clanlib_to_sdl_keys[CL_KEY_2] = SDLK_2;
		clanlib_to_sdl_keys[CL_KEY_3] = SDLK_3;
		clanlib_to_sdl_keys[CL_KEY_4] = SDLK_4;
		clanlib_to_sdl_keys[CL_KEY_5] = SDLK_5;
		clanlib_to_sdl_keys[CL_KEY_6] = SDLK_6;
		clanlib_to_sdl_keys[CL_KEY_7] = SDLK_7;
		clanlib_to_sdl_keys[CL_KEY_8] = SDLK_8;
		clanlib_to_sdl_keys[CL_KEY_9] = SDLK_9;
		clanlib_to_sdl_keys[CL_KEY_A] = SDLK_a;
		clanlib_to_sdl_keys[CL_KEY_B] = SDLK_b;
		clanlib_to_sdl_keys[CL_KEY_C] = SDLK_c;
		clanlib_to_sdl_keys[CL_KEY_D] = SDLK_d;
		clanlib_to_sdl_keys[CL_KEY_E] = SDLK_e;
		clanlib_to_sdl_keys[CL_KEY_F] = SDLK_f;
		clanlib_to_sdl_keys[CL_KEY_G] = SDLK_g;
		clanlib_to_sdl_keys[CL_KEY_H] = SDLK_h;
		clanlib_to_sdl_keys[CL_KEY_I] = SDLK_i;
		clanlib_to_sdl_keys[CL_KEY_J] = SDLK_j;
		clanlib_to_sdl_keys[CL_KEY_K] = SDLK_k;
		clanlib_to_sdl_keys[CL_KEY_L] = SDLK_l;
		clanlib_to_sdl_keys[CL_KEY_M] = SDLK_m;
		clanlib_to_sdl_keys[CL_KEY_N] = SDLK_n;
		clanlib_to_sdl_keys[CL_KEY_O] = SDLK_o;
		clanlib_to_sdl_keys[CL_KEY_P] = SDLK_p;
		clanlib_to_sdl_keys[CL_KEY_Q] = SDLK_q;
		clanlib_to_sdl_keys[CL_KEY_R] = SDLK_r;
		clanlib_to_sdl_keys[CL_KEY_S] = SDLK_s;
		clanlib_to_sdl_keys[CL_KEY_T] = SDLK_t;
		clanlib_to_sdl_keys[CL_KEY_U] = SDLK_u;
		clanlib_to_sdl_keys[CL_KEY_V] = SDLK_v;
		clanlib_to_sdl_keys[CL_KEY_W] = SDLK_w;
		clanlib_to_sdl_keys[CL_KEY_X] = SDLK_x;
		clanlib_to_sdl_keys[CL_KEY_Y] = SDLK_y;
		clanlib_to_sdl_keys[CL_KEY_Z] = SDLK_z;
		clanlib_to_sdl_keys[CL_KEY_DELETE] = SDLK_DELETE;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD0] = SDLK_KP0;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD1] = SDLK_KP1;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD2] = SDLK_KP2;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD3] = SDLK_KP3;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD4] = SDLK_KP4;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD5] = SDLK_KP5;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD6] = SDLK_KP6;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD7] = SDLK_KP7;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD8] = SDLK_KP8;
		clanlib_to_sdl_keys[CL_KEY_NUMPAD9] = SDLK_KP9;
#ifdef __APPLE__
		clanlib_to_sdl_keys[CL_KEY_DECIMAL] = SDLK_KP_PERIOD;
		clanlib_to_sdl_keys[CL_KEY_MULTIPLY] = SDLK_KP_MULTIPLY;
		clanlib_to_sdl_keys[CL_KEY_ADD] = SDLK_KP_PLUS;
#endif
		clanlib_to_sdl_keys[CL_KEY_DIVIDE] = SDLK_KP_DIVIDE;
		clanlib_to_sdl_keys[CL_KEY_SUBTRACT] = SDLK_KP_MINUS;
		clanlib_to_sdl_keys[CL_KEY_UP] = SDLK_UP;
		clanlib_to_sdl_keys[CL_KEY_DOWN] = SDLK_DOWN;
		clanlib_to_sdl_keys[CL_KEY_RIGHT] = SDLK_RIGHT;
		clanlib_to_sdl_keys[CL_KEY_LEFT] = SDLK_LEFT;
		clanlib_to_sdl_keys[CL_KEY_INSERT] = SDLK_INSERT;
		clanlib_to_sdl_keys[CL_KEY_HOME] = SDLK_HOME;
		clanlib_to_sdl_keys[CL_KEY_END] = SDLK_END;
		clanlib_to_sdl_keys[CL_KEY_PRIOR] = SDLK_PAGEUP;
		clanlib_to_sdl_keys[CL_KEY_NEXT] = SDLK_PAGEDOWN;
		clanlib_to_sdl_keys[CL_KEY_F1] = SDLK_F1;
		clanlib_to_sdl_keys[CL_KEY_F2] = SDLK_F2;
		clanlib_to_sdl_keys[CL_KEY_F3] = SDLK_F3;
		clanlib_to_sdl_keys[CL_KEY_F4] = SDLK_F4;
		clanlib_to_sdl_keys[CL_KEY_F5] = SDLK_F5;
		clanlib_to_sdl_keys[CL_KEY_F6] = SDLK_F6;
		clanlib_to_sdl_keys[CL_KEY_F7] = SDLK_F7;
		clanlib_to_sdl_keys[CL_KEY_F8] = SDLK_F8;
		clanlib_to_sdl_keys[CL_KEY_F9] = SDLK_F9;
		clanlib_to_sdl_keys[CL_KEY_F10] = SDLK_F10;
		clanlib_to_sdl_keys[CL_KEY_F11] = SDLK_F11;
		clanlib_to_sdl_keys[CL_KEY_F12] = SDLK_F12;
		clanlib_to_sdl_keys[CL_KEY_F13] = SDLK_F13;
		clanlib_to_sdl_keys[CL_KEY_F14] = SDLK_F14;
		clanlib_to_sdl_keys[CL_KEY_F15] = SDLK_F15;
		clanlib_to_sdl_keys[CL_KEY_NUMLOCK] = SDLK_NUMLOCK;
		clanlib_to_sdl_keys[CL_KEY_SCROLL] = SDLK_SCROLLOCK;
		clanlib_to_sdl_keys[CL_KEY_RSHIFT] = SDLK_RSHIFT;
		clanlib_to_sdl_keys[CL_KEY_LSHIFT] = SDLK_LSHIFT;
		clanlib_to_sdl_keys[CL_KEY_RCONTROL] = SDLK_RCTRL;
		clanlib_to_sdl_keys[CL_KEY_LCONTROL] = SDLK_LCTRL;
		clanlib_to_sdl_keys[CL_KEY_RMENU] = SDLK_RALT;
		clanlib_to_sdl_keys[CL_KEY_LMENU] = SDLK_LALT;
		clanlib_to_sdl_keys[CL_KEY_RWIN] = SDLK_RSUPER;
		clanlib_to_sdl_keys[CL_KEY_LWIN] = SDLK_LSUPER;
#else
		clanlib_to_sdl_keys[XK_VoidSymbol] = SDLK_UNKNOWN;
		clanlib_to_sdl_keys[XK_VoidSymbol] = SDLK_FIRST;
		clanlib_to_sdl_keys[XK_BackSpace] = SDLK_BACKSPACE;
		clanlib_to_sdl_keys[XK_Tab] = SDLK_TAB;
		clanlib_to_sdl_keys[XK_Clear] = SDLK_CLEAR;
		clanlib_to_sdl_keys[XK_Return] = SDLK_RETURN;
		clanlib_to_sdl_keys[XK_Pause] = SDLK_PAUSE;
		clanlib_to_sdl_keys[XK_Escape] = SDLK_ESCAPE;
		clanlib_to_sdl_keys[XK_space] = SDLK_SPACE;
		clanlib_to_sdl_keys[XK_exclam] = SDLK_EXCLAIM;
		clanlib_to_sdl_keys[XK_quotedbl] = SDLK_QUOTEDBL;
		clanlib_to_sdl_keys[XK_numbersign] = SDLK_HASH;
		clanlib_to_sdl_keys[XK_dollar] = SDLK_DOLLAR;
		clanlib_to_sdl_keys[XK_ampersand] = SDLK_AMPERSAND;
		clanlib_to_sdl_keys[XK_quoteright] = SDLK_QUOTE;
		clanlib_to_sdl_keys[XK_parenleft] = SDLK_LEFTPAREN;
		clanlib_to_sdl_keys[XK_parenright] = SDLK_RIGHTPAREN;
		clanlib_to_sdl_keys[XK_asterisk] = SDLK_ASTERISK;
		clanlib_to_sdl_keys[XK_plus] = SDLK_PLUS;
		clanlib_to_sdl_keys[XK_comma] = SDLK_COMMA;
		clanlib_to_sdl_keys[XK_minus] = SDLK_MINUS;
		clanlib_to_sdl_keys[XK_period] = SDLK_PERIOD;
		clanlib_to_sdl_keys[XK_slash] = SDLK_SLASH;
		clanlib_to_sdl_keys[XK_0] = SDLK_0;
		clanlib_to_sdl_keys[XK_1] = SDLK_1;
		clanlib_to_sdl_keys[XK_2] = SDLK_2;
		clanlib_to_sdl_keys[XK_3] = SDLK_3;
		clanlib_to_sdl_keys[XK_4] = SDLK_4;
		clanlib_to_sdl_keys[XK_5] = SDLK_5;
		clanlib_to_sdl_keys[XK_6] = SDLK_6;
		clanlib_to_sdl_keys[XK_7] = SDLK_7;
		clanlib_to_sdl_keys[XK_8] = SDLK_8;
		clanlib_to_sdl_keys[XK_9] = SDLK_9;
		clanlib_to_sdl_keys[XK_colon] = SDLK_COLON;
		clanlib_to_sdl_keys[XK_semicolon] = SDLK_SEMICOLON;
		clanlib_to_sdl_keys[XK_less] = SDLK_LESS;
		clanlib_to_sdl_keys[XK_equal] = SDLK_EQUALS;
		clanlib_to_sdl_keys[XK_greater] = SDLK_GREATER;
		clanlib_to_sdl_keys[XK_question] = SDLK_QUESTION;
		clanlib_to_sdl_keys[XK_at] = SDLK_AT;
		clanlib_to_sdl_keys[XK_bracketleft] = SDLK_LEFTBRACKET;
		clanlib_to_sdl_keys[XK_backslash] = SDLK_BACKSLASH;
		clanlib_to_sdl_keys[XK_bracketright] = SDLK_RIGHTBRACKET;
		clanlib_to_sdl_keys[XK_asciicircum] = SDLK_CARET;
		clanlib_to_sdl_keys[XK_underscore] = SDLK_UNDERSCORE;
		clanlib_to_sdl_keys[XK_quoteleft] = SDLK_BACKQUOTE;
		clanlib_to_sdl_keys[XK_a] = SDLK_a;
		clanlib_to_sdl_keys[XK_b] = SDLK_b;
		clanlib_to_sdl_keys[XK_c] = SDLK_c;
		clanlib_to_sdl_keys[XK_d] = SDLK_d;
		clanlib_to_sdl_keys[XK_e] = SDLK_e;
		clanlib_to_sdl_keys[XK_f] = SDLK_f;
		clanlib_to_sdl_keys[XK_g] = SDLK_g;
		clanlib_to_sdl_keys[XK_h] = SDLK_h;
		clanlib_to_sdl_keys[XK_i] = SDLK_i;
		clanlib_to_sdl_keys[XK_j] = SDLK_j;
		clanlib_to_sdl_keys[XK_k] = SDLK_k;
		clanlib_to_sdl_keys[XK_l] = SDLK_l;
		clanlib_to_sdl_keys[XK_m] = SDLK_m;
		clanlib_to_sdl_keys[XK_n] = SDLK_n;
		clanlib_to_sdl_keys[XK_o] = SDLK_o;
		clanlib_to_sdl_keys[XK_p] = SDLK_p;
		clanlib_to_sdl_keys[XK_q] = SDLK_q;
		clanlib_to_sdl_keys[XK_r] = SDLK_r;
		clanlib_to_sdl_keys[XK_s] = SDLK_s;
		clanlib_to_sdl_keys[XK_t] = SDLK_t;
		clanlib_to_sdl_keys[XK_u] = SDLK_u;
		clanlib_to_sdl_keys[XK_v] = SDLK_v;
		clanlib_to_sdl_keys[XK_w] = SDLK_w;
		clanlib_to_sdl_keys[XK_x] = SDLK_x;
		clanlib_to_sdl_keys[XK_y] = SDLK_y;
		clanlib_to_sdl_keys[XK_z] = SDLK_z;
		clanlib_to_sdl_keys[XK_Delete] = SDLK_DELETE;
		clanlib_to_sdl_keys[XK_nobreakspace] = SDLK_WORLD_0;
		clanlib_to_sdl_keys[XK_exclamdown] = SDLK_WORLD_1;
		clanlib_to_sdl_keys[XK_cent] = SDLK_WORLD_2;
		clanlib_to_sdl_keys[XK_sterling] = SDLK_WORLD_3;
		clanlib_to_sdl_keys[XK_currency] = SDLK_WORLD_4;
		clanlib_to_sdl_keys[XK_yen] = SDLK_WORLD_5;
		clanlib_to_sdl_keys[XK_brokenbar] = SDLK_WORLD_6;
		clanlib_to_sdl_keys[XK_section] = SDLK_WORLD_7;
		clanlib_to_sdl_keys[XK_diaeresis] = SDLK_WORLD_8;
		clanlib_to_sdl_keys[XK_copyright] = SDLK_WORLD_9;
		clanlib_to_sdl_keys[XK_ordfeminine] = SDLK_WORLD_10;
		clanlib_to_sdl_keys[XK_guillemotleft] = SDLK_WORLD_11;
		clanlib_to_sdl_keys[XK_notsign] = SDLK_WORLD_12;
		clanlib_to_sdl_keys[XK_hyphen] = SDLK_WORLD_13;
		clanlib_to_sdl_keys[XK_registered] = SDLK_WORLD_14;
		clanlib_to_sdl_keys[XK_macron] = SDLK_WORLD_15;
		clanlib_to_sdl_keys[XK_degree] = SDLK_WORLD_16;
		clanlib_to_sdl_keys[XK_plusminus] = SDLK_WORLD_17;
		clanlib_to_sdl_keys[XK_twosuperior] = SDLK_WORLD_18;
		clanlib_to_sdl_keys[XK_threesuperior] = SDLK_WORLD_19;
		clanlib_to_sdl_keys[XK_acute] = SDLK_WORLD_20;
		clanlib_to_sdl_keys[XK_mu] = SDLK_WORLD_21;
		clanlib_to_sdl_keys[XK_paragraph] = SDLK_WORLD_22;
		clanlib_to_sdl_keys[XK_periodcentered] = SDLK_WORLD_23;
		clanlib_to_sdl_keys[XK_cedilla] = SDLK_WORLD_24;
		clanlib_to_sdl_keys[XK_onesuperior] = SDLK_WORLD_25;
		clanlib_to_sdl_keys[XK_masculine] = SDLK_WORLD_26;
		clanlib_to_sdl_keys[XK_guillemotright] = SDLK_WORLD_27;
		clanlib_to_sdl_keys[XK_onequarter] = SDLK_WORLD_28;
		clanlib_to_sdl_keys[XK_onehalf] = SDLK_WORLD_29;
		clanlib_to_sdl_keys[XK_threequarters] = SDLK_WORLD_30;
		clanlib_to_sdl_keys[XK_questiondown] = SDLK_WORLD_31;
		clanlib_to_sdl_keys[XK_Agrave] = SDLK_WORLD_32;
		clanlib_to_sdl_keys[XK_Aacute] = SDLK_WORLD_33;
		clanlib_to_sdl_keys[XK_Acircumflex] = SDLK_WORLD_34;
		clanlib_to_sdl_keys[XK_Atilde] = SDLK_WORLD_35;
		clanlib_to_sdl_keys[XK_Adiaeresis] = SDLK_WORLD_36;
		clanlib_to_sdl_keys[XK_Aring] = SDLK_WORLD_37;
		clanlib_to_sdl_keys[XK_AE] = SDLK_WORLD_38;
		clanlib_to_sdl_keys[XK_Ccedilla] = SDLK_WORLD_39;
		clanlib_to_sdl_keys[XK_Egrave] = SDLK_WORLD_40;
		clanlib_to_sdl_keys[XK_Eacute] = SDLK_WORLD_41;
		clanlib_to_sdl_keys[XK_Ecircumflex] = SDLK_WORLD_42;
		clanlib_to_sdl_keys[XK_Ediaeresis] = SDLK_WORLD_43;
		clanlib_to_sdl_keys[XK_Igrave] = SDLK_WORLD_44;
		clanlib_to_sdl_keys[XK_Iacute] = SDLK_WORLD_45;
		clanlib_to_sdl_keys[XK_Icircumflex] = SDLK_WORLD_46;
		clanlib_to_sdl_keys[XK_Idiaeresis] = SDLK_WORLD_47;
		clanlib_to_sdl_keys[XK_ETH] = SDLK_WORLD_48;
		clanlib_to_sdl_keys[XK_Eth] = SDLK_WORLD_49;
		clanlib_to_sdl_keys[XK_Ntilde] = SDLK_WORLD_50;
		clanlib_to_sdl_keys[XK_Ograve] = SDLK_WORLD_51;
		clanlib_to_sdl_keys[XK_Oacute ] = SDLK_WORLD_52;
		clanlib_to_sdl_keys[XK_Ocircumflex] = SDLK_WORLD_53;
		clanlib_to_sdl_keys[XK_Otilde] = SDLK_WORLD_54;
		clanlib_to_sdl_keys[XK_Odiaeresis] = SDLK_WORLD_55;
		clanlib_to_sdl_keys[XK_multiply] = SDLK_WORLD_56;
		clanlib_to_sdl_keys[XK_Ooblique] = SDLK_WORLD_57;
		clanlib_to_sdl_keys[XK_Oslash] = SDLK_WORLD_58;
		clanlib_to_sdl_keys[XK_Ugrave] = SDLK_WORLD_59;
		clanlib_to_sdl_keys[XK_Uacute] = SDLK_WORLD_60;
		clanlib_to_sdl_keys[XK_Ucircumflex] = SDLK_WORLD_61;
		clanlib_to_sdl_keys[XK_Udiaeresis] = SDLK_WORLD_62;
		clanlib_to_sdl_keys[XK_Yacute] = SDLK_WORLD_63;
		clanlib_to_sdl_keys[XK_THORN] = SDLK_WORLD_64;
		clanlib_to_sdl_keys[XK_Thorn] = SDLK_WORLD_65;
		clanlib_to_sdl_keys[XK_ssharp] = SDLK_WORLD_66;
		clanlib_to_sdl_keys[XK_agrave] = SDLK_WORLD_67;
		clanlib_to_sdl_keys[XK_aacute] = SDLK_WORLD_68;
		clanlib_to_sdl_keys[XK_acircumflex] = SDLK_WORLD_69;
		clanlib_to_sdl_keys[XK_atilde] = SDLK_WORLD_70;
		clanlib_to_sdl_keys[XK_adiaeresis] = SDLK_WORLD_71;
		clanlib_to_sdl_keys[XK_aring] = SDLK_WORLD_72;
		clanlib_to_sdl_keys[XK_ae] = SDLK_WORLD_73;
		clanlib_to_sdl_keys[XK_ccedilla] = SDLK_WORLD_74;
		clanlib_to_sdl_keys[XK_egrave] = SDLK_WORLD_75;
		clanlib_to_sdl_keys[XK_eacute] = SDLK_WORLD_76;
		clanlib_to_sdl_keys[XK_ecircumflex] = SDLK_WORLD_77;
		clanlib_to_sdl_keys[XK_ediaeresis] = SDLK_WORLD_78;
		clanlib_to_sdl_keys[XK_igrave] = SDLK_WORLD_79;
		clanlib_to_sdl_keys[XK_iacute] = SDLK_WORLD_80;
		clanlib_to_sdl_keys[XK_icircumflex] = SDLK_WORLD_81;
		clanlib_to_sdl_keys[XK_idiaeresis] = SDLK_WORLD_82;
		clanlib_to_sdl_keys[XK_eth] = SDLK_WORLD_83;
		clanlib_to_sdl_keys[XK_ntilde] = SDLK_WORLD_84;
		clanlib_to_sdl_keys[XK_ograve] = SDLK_WORLD_85;
		clanlib_to_sdl_keys[XK_oacute] = SDLK_WORLD_86;
		clanlib_to_sdl_keys[XK_ocircumflex] = SDLK_WORLD_87;
		clanlib_to_sdl_keys[XK_otilde] = SDLK_WORLD_88;
		clanlib_to_sdl_keys[XK_odiaeresis] = SDLK_WORLD_89;
		clanlib_to_sdl_keys[XK_division] = SDLK_WORLD_90;
		clanlib_to_sdl_keys[XK_oslash] = SDLK_WORLD_91;
		clanlib_to_sdl_keys[XK_ooblique] = SDLK_WORLD_92;
		clanlib_to_sdl_keys[XK_ugrave] = SDLK_WORLD_93;
		clanlib_to_sdl_keys[XK_uacute] = SDLK_WORLD_94;
		clanlib_to_sdl_keys[XK_ucircumflex] = SDLK_WORLD_95;
		clanlib_to_sdl_keys[XK_KP_0] = SDLK_KP0;
		clanlib_to_sdl_keys[XK_KP_1] = SDLK_KP1;
		clanlib_to_sdl_keys[XK_KP_2] = SDLK_KP2;
		clanlib_to_sdl_keys[XK_KP_3] = SDLK_KP3;
		clanlib_to_sdl_keys[XK_KP_4] = SDLK_KP4;
		clanlib_to_sdl_keys[XK_KP_5] = SDLK_KP5;
		clanlib_to_sdl_keys[XK_KP_6] = SDLK_KP6;
		clanlib_to_sdl_keys[XK_KP_7] = SDLK_KP7;
		clanlib_to_sdl_keys[XK_KP_8] = SDLK_KP8;
		clanlib_to_sdl_keys[XK_KP_9] = SDLK_KP9;
		clanlib_to_sdl_keys[XK_KP_Insert] = SDLK_KP_PERIOD;
		clanlib_to_sdl_keys[XK_KP_Divide] = SDLK_KP_DIVIDE;
		clanlib_to_sdl_keys[XK_KP_Multiply] = SDLK_KP_MULTIPLY;
		clanlib_to_sdl_keys[XK_KP_Subtract] = SDLK_KP_MINUS;
		clanlib_to_sdl_keys[XK_KP_Add] = SDLK_KP_PLUS;
		clanlib_to_sdl_keys[XK_KP_Enter] = SDLK_KP_ENTER;
		clanlib_to_sdl_keys[XK_KP_Equal] = SDLK_KP_EQUALS;
		clanlib_to_sdl_keys[XK_Up] = SDLK_UP;
		clanlib_to_sdl_keys[XK_Down] = SDLK_DOWN;
		clanlib_to_sdl_keys[XK_Right] = SDLK_RIGHT;
		clanlib_to_sdl_keys[XK_Left] = SDLK_LEFT;
		clanlib_to_sdl_keys[XK_Insert] = SDLK_INSERT;
		clanlib_to_sdl_keys[XK_Home] = SDLK_HOME;
		clanlib_to_sdl_keys[XK_End] = SDLK_END;
		clanlib_to_sdl_keys[XK_Page_Up] = SDLK_PAGEUP;
		clanlib_to_sdl_keys[XK_Page_Down] = SDLK_PAGEDOWN;
		clanlib_to_sdl_keys[XK_F1] = SDLK_F1;
		clanlib_to_sdl_keys[XK_F2] = SDLK_F2;
		clanlib_to_sdl_keys[XK_F3] = SDLK_F3;
		clanlib_to_sdl_keys[XK_F4] = SDLK_F4;
		clanlib_to_sdl_keys[XK_F5] = SDLK_F5;
		clanlib_to_sdl_keys[XK_F6] = SDLK_F6;
		clanlib_to_sdl_keys[XK_F7] = SDLK_F7;
		clanlib_to_sdl_keys[XK_F8] = SDLK_F8;
		clanlib_to_sdl_keys[XK_F9] = SDLK_F9;
		clanlib_to_sdl_keys[XK_F10] = SDLK_F10;
		clanlib_to_sdl_keys[XK_F11] = SDLK_F11;
		clanlib_to_sdl_keys[XK_F12] = SDLK_F12;
		clanlib_to_sdl_keys[XK_F13] = SDLK_F13;
		clanlib_to_sdl_keys[XK_F14] = SDLK_F14;
		clanlib_to_sdl_keys[XK_F15] = SDLK_F15;
		clanlib_to_sdl_keys[XK_Num_Lock] = SDLK_NUMLOCK;
		clanlib_to_sdl_keys[XK_Caps_Lock] = SDLK_CAPSLOCK;
		clanlib_to_sdl_keys[XK_Scroll_Lock] = SDLK_SCROLLOCK;
		clanlib_to_sdl_keys[XK_Shift_R] = SDLK_RSHIFT;
		clanlib_to_sdl_keys[XK_Shift_L] = SDLK_LSHIFT;
		clanlib_to_sdl_keys[XK_Control_R] = SDLK_RCTRL;
		clanlib_to_sdl_keys[XK_Control_L] = SDLK_LCTRL;
		clanlib_to_sdl_keys[XK_Alt_R] = SDLK_RALT;
		clanlib_to_sdl_keys[XK_Alt_L] = SDLK_LALT;
		clanlib_to_sdl_keys[XK_Meta_R] = SDLK_RMETA;
		clanlib_to_sdl_keys[XK_Meta_L] = SDLK_LMETA;
		clanlib_to_sdl_keys[XK_Super_R] = SDLK_RSUPER;
		clanlib_to_sdl_keys[XK_Super_L] = SDLK_LSUPER;
		clanlib_to_sdl_keys[XK_Mode_switch] = SDLK_MODE;
		// clanlib_to_sdl_keys[??] = SDLK_COMPOSE; // i can't find a XK key code for that one
		clanlib_to_sdl_keys[XK_Help] = SDLK_HELP;
		clanlib_to_sdl_keys[XK_Print] = SDLK_PRINT;
		clanlib_to_sdl_keys[XK_Sys_Req] = SDLK_SYSREQ;
		clanlib_to_sdl_keys[XK_Break] = SDLK_BREAK;
		clanlib_to_sdl_keys[XK_Menu] = SDLK_MENU;
		// clanlib_to_sdl_keys[??] = SDLK_POWER; // i can't find a XK key code for that one
		clanlib_to_sdl_keys[XK_EuroSign] = SDLK_EURO;
		clanlib_to_sdl_keys[XK_Undo] = SDLK_UNDO;
#endif
	}

	char *sdl_name = SDL_GetKeyName(clanlib_to_sdl_keys[virtual_key]);
	if (sdl_name) return sdl_name;

	// Unknown. Return something at least :)
	return CL_String::format("Unknown %1", virtual_key);
}

bool CL_InputDevice_SDLKeyboard::get_keycode(int keysym) const
{
	if(!owner->has_focus())
		return false;

	state_const_iterator pos = state.find (keysym);
	return (pos != state.end());
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDevice_SDLKeyboard implementation:

void CL_InputDevice_SDLKeyboard::handle_keyboard_event(SDL_Event& event)
{
	static std::map<int, int> sdl_to_clanlib_keys;
	static bool first_call = true;

	if (first_call)
	{
		first_call = false;
#ifdef USE_CL_KEYS
		sdl_to_clanlib_keys[SDLK_UNKNOWN] = 0;
		sdl_to_clanlib_keys[SDLK_FIRST] = 0;
		sdl_to_clanlib_keys[SDLK_BACKSPACE] = CL_KEY_BACKSPACE;
		sdl_to_clanlib_keys[SDLK_TAB] = CL_KEY_TAB;
		sdl_to_clanlib_keys[SDLK_CLEAR] = CL_KEY_CLEAR;
		sdl_to_clanlib_keys[SDLK_RETURN] =CL_KEY_RETURN;
		sdl_to_clanlib_keys[SDLK_PAUSE] = CL_KEY_PAUSE;
		sdl_to_clanlib_keys[SDLK_ESCAPE] = CL_KEY_ESCAPE;
		sdl_to_clanlib_keys[SDLK_SPACE] = CL_KEY_SPACE;
		sdl_to_clanlib_keys[SDLK_EXCLAIM] = 0;
		sdl_to_clanlib_keys[SDLK_QUOTEDBL] = 0;
		sdl_to_clanlib_keys[SDLK_HASH] = 0;
		sdl_to_clanlib_keys[SDLK_DOLLAR] = 0;
		sdl_to_clanlib_keys[SDLK_AMPERSAND] = 0;
		sdl_to_clanlib_keys[SDLK_QUOTE] = 0;
		sdl_to_clanlib_keys[SDLK_LEFTPAREN] = 0;
		sdl_to_clanlib_keys[SDLK_RIGHTPAREN] = 0;
		sdl_to_clanlib_keys[SDLK_ASTERISK] = 0;
		sdl_to_clanlib_keys[SDLK_PLUS] = 0;
		sdl_to_clanlib_keys[SDLK_COMMA] = 0;
		sdl_to_clanlib_keys[SDLK_MINUS] = 0;
		sdl_to_clanlib_keys[SDLK_PERIOD] = 0;
		sdl_to_clanlib_keys[SDLK_SLASH] = 0;
		sdl_to_clanlib_keys[SDLK_0] = CL_KEY_0;
		sdl_to_clanlib_keys[SDLK_1] = CL_KEY_1;
		sdl_to_clanlib_keys[SDLK_2] = CL_KEY_2;
		sdl_to_clanlib_keys[SDLK_3] = CL_KEY_3;
		sdl_to_clanlib_keys[SDLK_4] = CL_KEY_4;
		sdl_to_clanlib_keys[SDLK_5] = CL_KEY_5;
		sdl_to_clanlib_keys[SDLK_6] = CL_KEY_6;
		sdl_to_clanlib_keys[SDLK_7] = CL_KEY_7;
		sdl_to_clanlib_keys[SDLK_8] = CL_KEY_8;
		sdl_to_clanlib_keys[SDLK_9] = CL_KEY_9;
		sdl_to_clanlib_keys[SDLK_COLON] = 0;
		sdl_to_clanlib_keys[SDLK_SEMICOLON] = 0;
		sdl_to_clanlib_keys[SDLK_LESS] = 0;
		sdl_to_clanlib_keys[SDLK_EQUALS] = 0;
		sdl_to_clanlib_keys[SDLK_GREATER] = 0;
		sdl_to_clanlib_keys[SDLK_QUESTION] = 0;
		sdl_to_clanlib_keys[SDLK_AT] = 0;
		sdl_to_clanlib_keys[SDLK_LEFTBRACKET] = 0;
		sdl_to_clanlib_keys[SDLK_BACKSLASH] = 0;
		sdl_to_clanlib_keys[SDLK_RIGHTBRACKET] = 0;
		sdl_to_clanlib_keys[SDLK_CARET] = 0;
		sdl_to_clanlib_keys[SDLK_UNDERSCORE] = 0;
		sdl_to_clanlib_keys[SDLK_BACKQUOTE] = 0;
		sdl_to_clanlib_keys[SDLK_a] = CL_KEY_A;
		sdl_to_clanlib_keys[SDLK_b] = CL_KEY_B;
		sdl_to_clanlib_keys[SDLK_c] = CL_KEY_C;
		sdl_to_clanlib_keys[SDLK_d] = CL_KEY_D;
		sdl_to_clanlib_keys[SDLK_e] = CL_KEY_E;
		sdl_to_clanlib_keys[SDLK_f] = CL_KEY_F;
		sdl_to_clanlib_keys[SDLK_g] = CL_KEY_G;
		sdl_to_clanlib_keys[SDLK_h] = CL_KEY_H;
		sdl_to_clanlib_keys[SDLK_i] = CL_KEY_I;
		sdl_to_clanlib_keys[SDLK_j] = CL_KEY_J;
		sdl_to_clanlib_keys[SDLK_k] = CL_KEY_K;
		sdl_to_clanlib_keys[SDLK_l] = CL_KEY_L;
		sdl_to_clanlib_keys[SDLK_m] = CL_KEY_M;
		sdl_to_clanlib_keys[SDLK_n] = CL_KEY_N;
		sdl_to_clanlib_keys[SDLK_o] = CL_KEY_O;
		sdl_to_clanlib_keys[SDLK_p] = CL_KEY_P;
		sdl_to_clanlib_keys[SDLK_q] = CL_KEY_Q;
		sdl_to_clanlib_keys[SDLK_r] = CL_KEY_R;
		sdl_to_clanlib_keys[SDLK_s] = CL_KEY_S;
		sdl_to_clanlib_keys[SDLK_t] = CL_KEY_T;
		sdl_to_clanlib_keys[SDLK_u] = CL_KEY_U;
		sdl_to_clanlib_keys[SDLK_v] = CL_KEY_V;
		sdl_to_clanlib_keys[SDLK_w] = CL_KEY_W;
		sdl_to_clanlib_keys[SDLK_x] = CL_KEY_X;
		sdl_to_clanlib_keys[SDLK_y] = CL_KEY_Y;
		sdl_to_clanlib_keys[SDLK_z] = CL_KEY_Z;
		sdl_to_clanlib_keys[SDLK_DELETE] = CL_KEY_DELETE;
		sdl_to_clanlib_keys[SDLK_WORLD_0] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_1] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_2] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_3] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_4] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_5] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_6] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_7] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_8] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_9] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_10] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_11] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_12] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_13] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_14] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_15] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_16] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_17] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_18] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_19] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_20] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_21] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_22] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_23] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_24] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_25] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_26] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_27] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_28] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_29] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_30] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_31] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_32] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_33] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_34] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_35] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_36] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_37] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_38] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_39] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_40] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_41] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_42] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_43] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_44] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_45] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_46] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_47] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_48] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_49] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_50] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_51] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_52] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_53] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_54] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_55] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_56] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_57] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_58] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_59] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_60] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_61] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_62] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_63] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_64] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_65] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_66] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_67] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_68] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_69] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_70] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_71] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_72] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_73] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_74] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_75] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_76] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_77] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_78] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_79] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_80] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_81] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_82] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_83] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_84] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_85] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_86] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_87] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_88] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_89] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_90] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_91] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_92] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_93] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_94] = 0;
		sdl_to_clanlib_keys[SDLK_WORLD_95] = 0;
		sdl_to_clanlib_keys[SDLK_KP0] = CL_KEY_NUMPAD0;
		sdl_to_clanlib_keys[SDLK_KP1] = CL_KEY_NUMPAD1;
		sdl_to_clanlib_keys[SDLK_KP2] = CL_KEY_NUMPAD2;
		sdl_to_clanlib_keys[SDLK_KP3] = CL_KEY_NUMPAD3;
		sdl_to_clanlib_keys[SDLK_KP4] = CL_KEY_NUMPAD4;
		sdl_to_clanlib_keys[SDLK_KP5] = CL_KEY_NUMPAD5;
		sdl_to_clanlib_keys[SDLK_KP6] = CL_KEY_NUMPAD6;
		sdl_to_clanlib_keys[SDLK_KP7] = CL_KEY_NUMPAD7;
		sdl_to_clanlib_keys[SDLK_KP8] = CL_KEY_NUMPAD8;
		sdl_to_clanlib_keys[SDLK_KP9] = CL_KEY_NUMPAD9;
#ifdef __APPLE__
		sdl_to_clanlib_keys[SDLK_KP_PERIOD] = CL_KEY_DECIMAL;
		sdl_to_clanlib_keys[SDLK_KP_MULTIPLY] = CL_KEY_MULTIPLY;
		sdl_to_clanlib_keys[SDLK_KP_PLUS] = CL_KEY_ADD;
#endif
		sdl_to_clanlib_keys[SDLK_KP_DIVIDE] = CL_KEY_DIVIDE;
		sdl_to_clanlib_keys[SDLK_KP_MINUS] = CL_KEY_SUBTRACT;
		sdl_to_clanlib_keys[SDLK_KP_ENTER] = 0;
		sdl_to_clanlib_keys[SDLK_KP_EQUALS] = 0;
		sdl_to_clanlib_keys[SDLK_UP] = CL_KEY_UP;
		sdl_to_clanlib_keys[SDLK_DOWN] = CL_KEY_DOWN;
		sdl_to_clanlib_keys[SDLK_RIGHT] = CL_KEY_RIGHT;
		sdl_to_clanlib_keys[SDLK_LEFT] = CL_KEY_LEFT;
		sdl_to_clanlib_keys[SDLK_INSERT] = CL_KEY_INSERT;
		sdl_to_clanlib_keys[SDLK_HOME] = CL_KEY_HOME;
		sdl_to_clanlib_keys[SDLK_END] = CL_KEY_END;
		sdl_to_clanlib_keys[SDLK_PAGEUP] = CL_KEY_PRIOR;
		sdl_to_clanlib_keys[SDLK_PAGEDOWN] = CL_KEY_NEXT;
		sdl_to_clanlib_keys[SDLK_F1] = CL_KEY_F1;
		sdl_to_clanlib_keys[SDLK_F2] = CL_KEY_F2;
		sdl_to_clanlib_keys[SDLK_F3] = CL_KEY_F3;
		sdl_to_clanlib_keys[SDLK_F4] = CL_KEY_F4;
		sdl_to_clanlib_keys[SDLK_F5] = CL_KEY_F5;
		sdl_to_clanlib_keys[SDLK_F6] = CL_KEY_F6;
		sdl_to_clanlib_keys[SDLK_F7] = CL_KEY_F7;
		sdl_to_clanlib_keys[SDLK_F8] = CL_KEY_F8;
		sdl_to_clanlib_keys[SDLK_F9] = CL_KEY_F9;
		sdl_to_clanlib_keys[SDLK_F10] = CL_KEY_F10;
		sdl_to_clanlib_keys[SDLK_F11] = CL_KEY_F11;
		sdl_to_clanlib_keys[SDLK_F12] = CL_KEY_F12;
		sdl_to_clanlib_keys[SDLK_F13] = CL_KEY_F13;
		sdl_to_clanlib_keys[SDLK_F14] = CL_KEY_F14;
		sdl_to_clanlib_keys[SDLK_F15] = CL_KEY_F15;
		sdl_to_clanlib_keys[SDLK_NUMLOCK] = CL_KEY_NUMLOCK;
		sdl_to_clanlib_keys[SDLK_CAPSLOCK] = 0;
		sdl_to_clanlib_keys[SDLK_SCROLLOCK] = CL_KEY_SCROLL;
		sdl_to_clanlib_keys[SDLK_RSHIFT] = CL_KEY_RSHIFT;
		sdl_to_clanlib_keys[SDLK_LSHIFT] = CL_KEY_LSHIFT;
		sdl_to_clanlib_keys[SDLK_RCTRL] = CL_KEY_RCONTROL;
		sdl_to_clanlib_keys[SDLK_LCTRL] = CL_KEY_LCONTROL;
		sdl_to_clanlib_keys[SDLK_RALT] = CL_KEY_RMENU;
		sdl_to_clanlib_keys[SDLK_LALT] = CL_KEY_LMENU;
		sdl_to_clanlib_keys[SDLK_RMETA] = 0;
		sdl_to_clanlib_keys[SDLK_LMETA] = 0;
		sdl_to_clanlib_keys[SDLK_RSUPER] = 0;
		sdl_to_clanlib_keys[SDLK_LSUPER] = 0;
		sdl_to_clanlib_keys[SDLK_MODE] = 0;
		sdl_to_clanlib_keys[SDLK_COMPOSE] = 0; // i can't find a XK key code for that one
		sdl_to_clanlib_keys[SDLK_HELP] = 0;
		sdl_to_clanlib_keys[SDLK_PRINT] = 0;
		sdl_to_clanlib_keys[SDLK_SYSREQ] = 0;
		sdl_to_clanlib_keys[SDLK_BREAK] = 0;
		sdl_to_clanlib_keys[SDLK_MENU] = 0;
		sdl_to_clanlib_keys[SDLK_POWER] = 0; // i can't find a XK key code for that one
		sdl_to_clanlib_keys[SDLK_EURO] = 0;
		sdl_to_clanlib_keys[SDLK_UNDO] = 0;
#else
		sdl_to_clanlib_keys[SDLK_UNKNOWN] = XK_VoidSymbol;
		sdl_to_clanlib_keys[SDLK_FIRST] = XK_VoidSymbol;
		sdl_to_clanlib_keys[SDLK_BACKSPACE] = XK_BackSpace;
		sdl_to_clanlib_keys[SDLK_TAB] = XK_Tab;
		sdl_to_clanlib_keys[SDLK_CLEAR] = XK_Clear;
		sdl_to_clanlib_keys[SDLK_RETURN] =XK_Return;
		sdl_to_clanlib_keys[SDLK_PAUSE] = XK_Pause;
		sdl_to_clanlib_keys[SDLK_ESCAPE] = XK_Escape;
		sdl_to_clanlib_keys[SDLK_SPACE] = XK_space;
		sdl_to_clanlib_keys[SDLK_EXCLAIM] = XK_exclam;
		sdl_to_clanlib_keys[SDLK_QUOTEDBL] = XK_quotedbl;
		sdl_to_clanlib_keys[SDLK_HASH] = XK_numbersign;
		sdl_to_clanlib_keys[SDLK_DOLLAR] = XK_dollar;
		sdl_to_clanlib_keys[SDLK_AMPERSAND] = XK_ampersand;
		sdl_to_clanlib_keys[SDLK_QUOTE] = XK_quoteright;
		sdl_to_clanlib_keys[SDLK_LEFTPAREN] = XK_parenleft;
		sdl_to_clanlib_keys[SDLK_RIGHTPAREN] = XK_parenright;
		sdl_to_clanlib_keys[SDLK_ASTERISK] = XK_asterisk;
		sdl_to_clanlib_keys[SDLK_PLUS] = XK_plus;
		sdl_to_clanlib_keys[SDLK_COMMA] = XK_comma;
		sdl_to_clanlib_keys[SDLK_MINUS] = XK_minus;
		sdl_to_clanlib_keys[SDLK_PERIOD] = XK_period;
		sdl_to_clanlib_keys[SDLK_SLASH] = XK_slash;
		sdl_to_clanlib_keys[SDLK_0] = XK_0;
		sdl_to_clanlib_keys[SDLK_1] = XK_1;
		sdl_to_clanlib_keys[SDLK_2] = XK_2;
		sdl_to_clanlib_keys[SDLK_3] = XK_3;
		sdl_to_clanlib_keys[SDLK_4] = XK_4;
		sdl_to_clanlib_keys[SDLK_5] = XK_5;
		sdl_to_clanlib_keys[SDLK_6] = XK_6;
		sdl_to_clanlib_keys[SDLK_7] = XK_7;
		sdl_to_clanlib_keys[SDLK_8] = XK_8;
		sdl_to_clanlib_keys[SDLK_9] = XK_9;
		sdl_to_clanlib_keys[SDLK_COLON] = XK_colon;
		sdl_to_clanlib_keys[SDLK_SEMICOLON] = XK_semicolon;
		sdl_to_clanlib_keys[SDLK_LESS] = XK_less;
		sdl_to_clanlib_keys[SDLK_EQUALS] = XK_equal;
		sdl_to_clanlib_keys[SDLK_GREATER] = XK_greater;
		sdl_to_clanlib_keys[SDLK_QUESTION] = XK_question;
		sdl_to_clanlib_keys[SDLK_AT] = XK_at;
		sdl_to_clanlib_keys[SDLK_LEFTBRACKET] = XK_bracketleft;
		sdl_to_clanlib_keys[SDLK_BACKSLASH] = XK_backslash;
		sdl_to_clanlib_keys[SDLK_RIGHTBRACKET] = XK_bracketright;
		sdl_to_clanlib_keys[SDLK_CARET] = XK_asciicircum;
		sdl_to_clanlib_keys[SDLK_UNDERSCORE] = XK_underscore;
		sdl_to_clanlib_keys[SDLK_BACKQUOTE] = XK_quoteleft;
		sdl_to_clanlib_keys[SDLK_a] = XK_a;
		sdl_to_clanlib_keys[SDLK_b] = XK_b;
		sdl_to_clanlib_keys[SDLK_c] = XK_c;
		sdl_to_clanlib_keys[SDLK_d] = XK_d;
		sdl_to_clanlib_keys[SDLK_e] = XK_e;
		sdl_to_clanlib_keys[SDLK_f] = XK_f;
		sdl_to_clanlib_keys[SDLK_g] = XK_g;
		sdl_to_clanlib_keys[SDLK_h] = XK_h;
		sdl_to_clanlib_keys[SDLK_i] = XK_i;
		sdl_to_clanlib_keys[SDLK_j] = XK_j;
		sdl_to_clanlib_keys[SDLK_k] = XK_k;
		sdl_to_clanlib_keys[SDLK_l] = XK_l;
		sdl_to_clanlib_keys[SDLK_m] = XK_m;
		sdl_to_clanlib_keys[SDLK_n] = XK_n;
		sdl_to_clanlib_keys[SDLK_o] = XK_o;
		sdl_to_clanlib_keys[SDLK_p] = XK_p;
		sdl_to_clanlib_keys[SDLK_q] = XK_q;
		sdl_to_clanlib_keys[SDLK_r] = XK_r;
		sdl_to_clanlib_keys[SDLK_s] = XK_s;
		sdl_to_clanlib_keys[SDLK_t] = XK_t;
		sdl_to_clanlib_keys[SDLK_u] = XK_u;
		sdl_to_clanlib_keys[SDLK_v] = XK_v;
		sdl_to_clanlib_keys[SDLK_w] = XK_w;
		sdl_to_clanlib_keys[SDLK_x] = XK_x;
		sdl_to_clanlib_keys[SDLK_y] = XK_y;
		sdl_to_clanlib_keys[SDLK_z] = XK_z;
		sdl_to_clanlib_keys[SDLK_DELETE] = XK_Delete;
		sdl_to_clanlib_keys[SDLK_WORLD_0] = XK_nobreakspace;
		sdl_to_clanlib_keys[SDLK_WORLD_1] = XK_exclamdown;
		sdl_to_clanlib_keys[SDLK_WORLD_2] = XK_cent;
		sdl_to_clanlib_keys[SDLK_WORLD_3] = XK_sterling;
		sdl_to_clanlib_keys[SDLK_WORLD_4] = XK_currency;
		sdl_to_clanlib_keys[SDLK_WORLD_5] = XK_yen;
		sdl_to_clanlib_keys[SDLK_WORLD_6] = XK_brokenbar;
		sdl_to_clanlib_keys[SDLK_WORLD_7] = XK_section;
		sdl_to_clanlib_keys[SDLK_WORLD_8] = XK_diaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_9] = XK_copyright;
		sdl_to_clanlib_keys[SDLK_WORLD_10] = XK_ordfeminine;
		sdl_to_clanlib_keys[SDLK_WORLD_11] = XK_guillemotleft;
		sdl_to_clanlib_keys[SDLK_WORLD_12] = XK_notsign;
		sdl_to_clanlib_keys[SDLK_WORLD_13] = XK_hyphen;
		sdl_to_clanlib_keys[SDLK_WORLD_14] = XK_registered;
		sdl_to_clanlib_keys[SDLK_WORLD_15] = XK_macron;
		sdl_to_clanlib_keys[SDLK_WORLD_16] = XK_degree;
		sdl_to_clanlib_keys[SDLK_WORLD_17] = XK_plusminus;
		sdl_to_clanlib_keys[SDLK_WORLD_18] = XK_twosuperior;
		sdl_to_clanlib_keys[SDLK_WORLD_19] = XK_threesuperior;
		sdl_to_clanlib_keys[SDLK_WORLD_20] = XK_acute;
		sdl_to_clanlib_keys[SDLK_WORLD_21] = XK_mu;
		sdl_to_clanlib_keys[SDLK_WORLD_22] = XK_paragraph;
		sdl_to_clanlib_keys[SDLK_WORLD_23] = XK_periodcentered;
		sdl_to_clanlib_keys[SDLK_WORLD_24] = XK_cedilla;
		sdl_to_clanlib_keys[SDLK_WORLD_25] = XK_onesuperior;
		sdl_to_clanlib_keys[SDLK_WORLD_26] = XK_masculine;
		sdl_to_clanlib_keys[SDLK_WORLD_27] = XK_guillemotright;
		sdl_to_clanlib_keys[SDLK_WORLD_28] = XK_onequarter;
		sdl_to_clanlib_keys[SDLK_WORLD_29] = XK_onehalf;
		sdl_to_clanlib_keys[SDLK_WORLD_30] = XK_threequarters;
		sdl_to_clanlib_keys[SDLK_WORLD_31] = XK_questiondown;
		sdl_to_clanlib_keys[SDLK_WORLD_32] = XK_Agrave;
		sdl_to_clanlib_keys[SDLK_WORLD_33] = XK_Aacute;
		sdl_to_clanlib_keys[SDLK_WORLD_34] = XK_Acircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_35] = XK_Atilde;
		sdl_to_clanlib_keys[SDLK_WORLD_36] = XK_Adiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_37] = XK_Aring;
		sdl_to_clanlib_keys[SDLK_WORLD_38] = XK_AE;
		sdl_to_clanlib_keys[SDLK_WORLD_39] = XK_Ccedilla;
		sdl_to_clanlib_keys[SDLK_WORLD_40] = XK_Egrave;
		sdl_to_clanlib_keys[SDLK_WORLD_41] = XK_Eacute;
		sdl_to_clanlib_keys[SDLK_WORLD_42] = XK_Ecircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_43] = XK_Ediaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_44] = XK_Igrave;
		sdl_to_clanlib_keys[SDLK_WORLD_45] = XK_Iacute;
		sdl_to_clanlib_keys[SDLK_WORLD_46] = XK_Icircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_47] = XK_Idiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_48] = XK_ETH;
		sdl_to_clanlib_keys[SDLK_WORLD_49] = XK_Eth;
		sdl_to_clanlib_keys[SDLK_WORLD_50] = XK_Ntilde;
		sdl_to_clanlib_keys[SDLK_WORLD_51] = XK_Ograve;
		sdl_to_clanlib_keys[SDLK_WORLD_52] = XK_Oacute ;
		sdl_to_clanlib_keys[SDLK_WORLD_53] = XK_Ocircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_54] = XK_Otilde;
		sdl_to_clanlib_keys[SDLK_WORLD_55] = XK_Odiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_56] = XK_multiply;
		sdl_to_clanlib_keys[SDLK_WORLD_57] = XK_Ooblique;
		sdl_to_clanlib_keys[SDLK_WORLD_58] = XK_Oslash;
		sdl_to_clanlib_keys[SDLK_WORLD_59] = XK_Ugrave;
		sdl_to_clanlib_keys[SDLK_WORLD_60] = XK_Uacute;
		sdl_to_clanlib_keys[SDLK_WORLD_61] = XK_Ucircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_62] = XK_Udiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_63] = XK_Yacute;
		sdl_to_clanlib_keys[SDLK_WORLD_64] = XK_THORN;
		sdl_to_clanlib_keys[SDLK_WORLD_65] = XK_Thorn;
		sdl_to_clanlib_keys[SDLK_WORLD_66] = XK_ssharp;
		sdl_to_clanlib_keys[SDLK_WORLD_67] = XK_agrave;
		sdl_to_clanlib_keys[SDLK_WORLD_68] = XK_aacute;
		sdl_to_clanlib_keys[SDLK_WORLD_69] = XK_acircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_70] = XK_atilde;
		sdl_to_clanlib_keys[SDLK_WORLD_71] = XK_adiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_72] = XK_aring;
		sdl_to_clanlib_keys[SDLK_WORLD_73] = XK_ae;
		sdl_to_clanlib_keys[SDLK_WORLD_74] = XK_ccedilla;
		sdl_to_clanlib_keys[SDLK_WORLD_75] = XK_egrave;
		sdl_to_clanlib_keys[SDLK_WORLD_76] = XK_eacute;
		sdl_to_clanlib_keys[SDLK_WORLD_77] = XK_ecircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_78] = XK_ediaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_79] = XK_igrave;
		sdl_to_clanlib_keys[SDLK_WORLD_80] = XK_iacute;
		sdl_to_clanlib_keys[SDLK_WORLD_81] = XK_icircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_82] = XK_idiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_83] = XK_eth;
		sdl_to_clanlib_keys[SDLK_WORLD_84] = XK_ntilde;
		sdl_to_clanlib_keys[SDLK_WORLD_85] = XK_ograve;
		sdl_to_clanlib_keys[SDLK_WORLD_86] = XK_oacute;
		sdl_to_clanlib_keys[SDLK_WORLD_87] = XK_ocircumflex;
		sdl_to_clanlib_keys[SDLK_WORLD_88] = XK_otilde;
		sdl_to_clanlib_keys[SDLK_WORLD_89] = XK_odiaeresis;
		sdl_to_clanlib_keys[SDLK_WORLD_90] = XK_division;
		sdl_to_clanlib_keys[SDLK_WORLD_91] = XK_oslash;
		sdl_to_clanlib_keys[SDLK_WORLD_92] = XK_ooblique;
		sdl_to_clanlib_keys[SDLK_WORLD_93] = XK_ugrave;
		sdl_to_clanlib_keys[SDLK_WORLD_94] = XK_uacute;
		sdl_to_clanlib_keys[SDLK_WORLD_95] = XK_ucircumflex;
		sdl_to_clanlib_keys[SDLK_KP0] = XK_KP_0;
		sdl_to_clanlib_keys[SDLK_KP1] = XK_KP_1;
		sdl_to_clanlib_keys[SDLK_KP2] = XK_KP_2;
		sdl_to_clanlib_keys[SDLK_KP3] = XK_KP_3;
		sdl_to_clanlib_keys[SDLK_KP4] = XK_KP_4;
		sdl_to_clanlib_keys[SDLK_KP5] = XK_KP_5;
		sdl_to_clanlib_keys[SDLK_KP6] = XK_KP_6;
		sdl_to_clanlib_keys[SDLK_KP7] = XK_KP_7;
		sdl_to_clanlib_keys[SDLK_KP8] = XK_KP_8;
		sdl_to_clanlib_keys[SDLK_KP9] = XK_KP_9;
		sdl_to_clanlib_keys[SDLK_KP_PERIOD] = XK_KP_Insert;
		sdl_to_clanlib_keys[SDLK_KP_DIVIDE] = XK_KP_Divide;
		sdl_to_clanlib_keys[SDLK_KP_MULTIPLY] = XK_KP_Multiply;
		sdl_to_clanlib_keys[SDLK_KP_MINUS] = XK_KP_Subtract;
		sdl_to_clanlib_keys[SDLK_KP_PLUS] = XK_KP_Add;
		sdl_to_clanlib_keys[SDLK_KP_ENTER] = XK_KP_Enter;
		sdl_to_clanlib_keys[SDLK_KP_EQUALS] = XK_KP_Equal;
		sdl_to_clanlib_keys[SDLK_UP] = XK_Up;
		sdl_to_clanlib_keys[SDLK_DOWN] = XK_Down;
		sdl_to_clanlib_keys[SDLK_RIGHT] = XK_Right;
		sdl_to_clanlib_keys[SDLK_LEFT] = XK_Left;
		sdl_to_clanlib_keys[SDLK_INSERT] = XK_Insert;
		sdl_to_clanlib_keys[SDLK_HOME] = XK_Home;
		sdl_to_clanlib_keys[SDLK_END] = XK_End;
		sdl_to_clanlib_keys[SDLK_PAGEUP] = XK_Page_Up;
		sdl_to_clanlib_keys[SDLK_PAGEDOWN] = XK_Page_Down;
		sdl_to_clanlib_keys[SDLK_F1] = XK_F1;
		sdl_to_clanlib_keys[SDLK_F2] = XK_F2;
		sdl_to_clanlib_keys[SDLK_F3] = XK_F3;
		sdl_to_clanlib_keys[SDLK_F4] = XK_F4;
		sdl_to_clanlib_keys[SDLK_F5] = XK_F5;
		sdl_to_clanlib_keys[SDLK_F6] = XK_F6;
		sdl_to_clanlib_keys[SDLK_F7] = XK_F7;
		sdl_to_clanlib_keys[SDLK_F8] = XK_F8;
		sdl_to_clanlib_keys[SDLK_F9] = XK_F9;
		sdl_to_clanlib_keys[SDLK_F10] = XK_F10;
		sdl_to_clanlib_keys[SDLK_F11] = XK_F11;
		sdl_to_clanlib_keys[SDLK_F12] = XK_F12;
		sdl_to_clanlib_keys[SDLK_F13] = XK_F13;
		sdl_to_clanlib_keys[SDLK_F14] = XK_F14;
		sdl_to_clanlib_keys[SDLK_F15] = XK_F15;
		sdl_to_clanlib_keys[SDLK_NUMLOCK] = XK_Num_Lock;
		sdl_to_clanlib_keys[SDLK_CAPSLOCK] = XK_Caps_Lock;
		sdl_to_clanlib_keys[SDLK_SCROLLOCK] = XK_Scroll_Lock;
		sdl_to_clanlib_keys[SDLK_RSHIFT] = XK_Shift_R;
		sdl_to_clanlib_keys[SDLK_LSHIFT] = XK_Shift_L;
		sdl_to_clanlib_keys[SDLK_RCTRL] = XK_Control_R;
		sdl_to_clanlib_keys[SDLK_LCTRL] = XK_Control_L;
		sdl_to_clanlib_keys[SDLK_RALT] = XK_Alt_R;
		sdl_to_clanlib_keys[SDLK_LALT] = XK_Alt_L;
		sdl_to_clanlib_keys[SDLK_RMETA] = XK_Meta_R;
		sdl_to_clanlib_keys[SDLK_LMETA] = XK_Meta_L;
		sdl_to_clanlib_keys[SDLK_RSUPER] = XK_Super_R;
		sdl_to_clanlib_keys[SDLK_LSUPER] = XK_Super_L;
		sdl_to_clanlib_keys[SDLK_MODE] = XK_Mode_switch;
		sdl_to_clanlib_keys[SDLK_COMPOSE] = 0; // i can't find a XK key code for that one
		sdl_to_clanlib_keys[SDLK_HELP] = XK_Help;
		sdl_to_clanlib_keys[SDLK_PRINT] = XK_Print;
		sdl_to_clanlib_keys[SDLK_SYSREQ] = XK_Sys_Req;
		sdl_to_clanlib_keys[SDLK_BREAK] = XK_Break;
		sdl_to_clanlib_keys[SDLK_MENU] = XK_Menu;
		sdl_to_clanlib_keys[SDLK_POWER] = 0; // i can't find a XK key code for that one
		sdl_to_clanlib_keys[SDLK_EURO] = XK_EuroSign;
		sdl_to_clanlib_keys[SDLK_UNDO] = XK_Undo;
#endif
	}

	CL_InputEvent cl_event;
	cl_event.id = sdl_to_clanlib_keys[event.key.keysym.sym];
	
	if (event.key.type == SDL_KEYDOWN) 
	{
		cl_event.type = CL_InputEvent::pressed;

		state_iterator pos = state.find (cl_event.id);
		if (pos == state.end())
		{
			state_pair key(cl_event.id, true);
			state.insert (key);
		}
	}
	else if (event.key.type == SDL_KEYUP) 
	{
		cl_event.type = CL_InputEvent::released;
		state_iterator pos = state.find (cl_event.id);
		if (pos != state.end()) state.erase(pos);
	}
	cl_event.device = owner->keyboard;
	cl_event.mouse_pos = CL_Point(owner->mouse.get_x(), owner->mouse.get_y());
	// todo: do a more clever convert from unicode.
	if (event.key.keysym.unicode != 0 && event.key.keysym.unicode < 128)
		cl_event.str = std::string(1, event.key.keysym.unicode);
	// todo: keep local track of repeat counts since SDL doesnt do that for us.
	
	if (cl_event.type == CL_InputEvent::pressed)
		owner->keyboard.sig_key_down().call(cl_event);
	if (cl_event.type == CL_InputEvent::released)
		owner->keyboard.sig_key_up().call(cl_event);
}
