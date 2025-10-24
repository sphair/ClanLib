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

//! clanDisplay="Input"
//! header=display.h

#ifndef header_keys
#define header_keys

#if _MSC_VER > 1000
#pragma once
#endif

// Keyboard & mouse platform independence support.

#ifdef WIN32

#include <windows.h>

#define CL_MOUSE_LEFT 0
#define CL_MOUSE_RIGHT 1
#define CL_MOUSE_MIDDLE 2
#define CL_MOUSE_WHEEL_UP 3
#define CL_MOUSE_WHEEL_DOWN 4
#define CL_MOUSE_XBUTTON1 5
#define CL_MOUSE_XBUTTON2 6

#define CL_KEY_BACKSPACE VK_BACK
#define CL_KEY_TAB VK_TAB
#define CL_KEY_CLEAR VK_CLEAR
#define CL_KEY_RETURN VK_RETURN
#define CL_KEY_SHIFT VK_SHIFT
#define CL_KEY_CONTROL VK_CONTROL
#define CL_KEY_MENU VK_MENU
#define CL_KEY_PAUSE VK_PAUSE
#define CL_KEY_ESCAPE VK_ESCAPE

#if !defined(__CYGWIN__) && !defined(__MINGW32__)
#  define CL_KEY_KANJI      VK_KANJI
#  define CL_KEY_CONVERT    VK_CONVERT
#  define CL_KEY_NONCONVERT VK_NONCONVERT
#endif

#if _MSC_VER < 1300
//1300 = VC7, defines here for VC6 and smaller
#define VK_OEM_PLUS 0xBB
#define VK_OEM_MINUS 0xBD
#define VK_OEM_COMMA 0xBC
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_1 0xBA
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#define VK_OEM_8 0xDF
#endif

#define CL_KEY_SPACE VK_SPACE
#define CL_KEY_PRIOR VK_PRIOR
#define CL_KEY_NEXT VK_NEXT
#define CL_KEY_END VK_END
#define CL_KEY_HOME VK_HOME
#define CL_KEY_LEFT VK_LEFT
#define CL_KEY_UP VK_UP
#define CL_KEY_RIGHT VK_RIGHT
#define CL_KEY_DOWN VK_DOWN
#define CL_KEY_SELECT VK_SELECT
#define CL_KEY_PRINT VK_PRINT
#define CL_KEY_EXECUTE VK_EXECUTE
#define CL_KEY_INSERT VK_INSERT
#define CL_KEY_DELETE VK_DELETE
#define CL_KEY_HELP VK_HELP
#define CL_KEY_0 '0'
#define CL_KEY_1 '1'
#define CL_KEY_2 '2'
#define CL_KEY_3 '3'
#define CL_KEY_4 '4'
#define CL_KEY_5 '5'
#define CL_KEY_6 '6'
#define CL_KEY_7 '7'
#define CL_KEY_8 '8'
#define CL_KEY_9 '9'
#define CL_KEY_A 'A'
#define CL_KEY_B 'B'
#define CL_KEY_C 'C'
#define CL_KEY_D 'D'
#define CL_KEY_E 'E'
#define CL_KEY_F 'F'
#define CL_KEY_G 'G'
#define CL_KEY_H 'H'
#define CL_KEY_I 'I'
#define CL_KEY_J 'J'
#define CL_KEY_K 'K'
#define CL_KEY_L 'L'
#define CL_KEY_M 'M'
#define CL_KEY_N 'N'
#define CL_KEY_O 'O'
#define CL_KEY_P 'P'
#define CL_KEY_Q 'Q'
#define CL_KEY_R 'R'
#define CL_KEY_S 'S'
#define CL_KEY_T 'T'
#define CL_KEY_U 'U'
#define CL_KEY_V 'V'
#define CL_KEY_W 'W'
#define CL_KEY_X 'X'
#define CL_KEY_Y 'Y'
#define CL_KEY_Z 'Z'
#define CL_KEY_SUBTRACT VK_OEM_MINUS
#define CL_KEY_EQUALS VK_OEM_PLUS
#define CL_KEY_LWIN VK_LWIN
#define CL_KEY_RWIN VK_RWIN
#define CL_KEY_APPS VK_APPS
#define CL_KEY_PERIOD VK_OEM_PERIOD
#define CL_KEY_QUOTE			VK_OEM_7 // (may be US only)
#define CL_KEY_SEMICOLON		VK_OEM_1 // (may be US only)
#define CL_KEY_COMMA			VK_OEM_COMMA
#define CL_KEY_DIVIDE VK_OEM_2 // the / key (may be US only)
#define CL_KEY_GRAVE VK_OEM_3  //unshifted tilde (may be US only)

#define CL_KEY_NUMPAD0 VK_NUMPAD0
#define CL_KEY_NUMPAD1 VK_NUMPAD1
#define CL_KEY_NUMPAD2 VK_NUMPAD2
#define CL_KEY_NUMPAD3 VK_NUMPAD3
#define CL_KEY_NUMPAD4 VK_NUMPAD4
#define CL_KEY_NUMPAD5 VK_NUMPAD5
#define CL_KEY_NUMPAD6 VK_NUMPAD6
#define CL_KEY_NUMPAD7 VK_NUMPAD7
#define CL_KEY_NUMPAD8 VK_NUMPAD8
#define CL_KEY_NUMPAD9 VK_NUMPAD9
#define CL_KEY_NUMPAD_MULTIPLY VK_MULTIPLY
#define CL_KEY_NUMPAD_ADD VK_ADD
#define CL_KEY_NUMPAD_ENTER VK_SEPARATOR
#define CL_KEY_NUMPAD_SUBTRACT VK_SUBTRACT
#define CL_KEY_NUMPAD_DECIMAL VK_DECIMAL
#define CL_KEY_NUMPAD_DIVIDE VK_DIVIDE
#define CL_KEY_NUMPAD_EQUALS VK_SEPARATOR

#define CL_KEY_F1 VK_F1
#define CL_KEY_F2 VK_F2
#define CL_KEY_F3 VK_F3
#define CL_KEY_F4 VK_F4
#define CL_KEY_F5 VK_F5
#define CL_KEY_F6 VK_F6
#define CL_KEY_F7 VK_F7
#define CL_KEY_F8 VK_F8
#define CL_KEY_F9 VK_F9
#define CL_KEY_F10 VK_F10
#define CL_KEY_F11 VK_F11
#define CL_KEY_F12 VK_F12
#define CL_KEY_F13 VK_F13
#define CL_KEY_F14 VK_F14
#define CL_KEY_F15 VK_F15
#define CL_KEY_F16 VK_F16
#define CL_KEY_F17 VK_F17
#define CL_KEY_F18 VK_F18
#define CL_KEY_F19 VK_F19
#define CL_KEY_F20 VK_F20
#define CL_KEY_F21 VK_F21
#define CL_KEY_F22 VK_F22
#define CL_KEY_F23 VK_F23
#define CL_KEY_F24 VK_F24
#define CL_KEY_NUMLOCK VK_NUMLOCK
#define CL_KEY_SCROLL VK_SCROLL
#define CL_KEY_LSHIFT VK_LSHIFT
#define CL_KEY_RSHIFT VK_RSHIFT
#define CL_KEY_LCONTROL VK_LCONTROL
#define CL_KEY_RCONTROL VK_RCONTROL
#define CL_KEY_LMENU VK_LMENU
#define CL_KEY_RMENU VK_RMENU


#ifndef VK_CLOSE_BRACKET
	#define VK_CLOSE_BRACKET 0xDD
	#define VK_OPEN_BRACKET 0xDB
#endif

#define CL_KEY_LEFT_BRACKET VK_OPEN_BRACKET
#define CL_KEY_RIGHT_BRACKET VK_CLOSE_BRACKET

#else
#ifdef __APPLE__

#include <Carbon/carbon.h>

// Seems like this platform dont have keysyms, or their docs suck so much I
// can't find it.
//
// To solve this matter I've made my own virtual key numbering. If a key
// pressed does not match any of these, then it will return the actual
// MacOSX keycode in the high order word (keycode + 0x10000000).
//
// Naturally this require that they dont use keycode values above 0xffff,
// but if they do complain to Apple for their crappy keyboard support.
// Already kinda pissed that the keys for typing { and } are alt+shift+8 and
// alt+shift+9, plus that backslash is alt+shift+7. How hostile is that!?!?
// Oh well what can you expect from a company that write "Designed by Apple
// in California" with big letters when you open your box that the powerbook
// came in. Yes it fucking shows thats its designed in California!!
//
// (No offence to California though. Been there once and loved the place.)

#define CL_MOUSE_LEFT			0
#define CL_MOUSE_RIGHT			1
#define CL_MOUSE_MIDDLE			2
#define CL_MOUSE_WHEEL_UP		3
#define CL_MOUSE_WHEEL_DOWN		4
#define CL_MOUSE_XBUTTON1		5
#define CL_MOUSE_XBUTTON2		6

#define CL_KEY_BACKSPACE		10
#define CL_KEY_TAB				11
#define CL_KEY_CLEAR			12
#define CL_KEY_RETURN			13
#define CL_KEY_SHIFT			14
#define CL_KEY_CONTROL			15
#define CL_KEY_OPTION			16
#define CL_KEY_PAUSE			17
#define CL_KEY_KANJI			18
#define CL_KEY_ESCAPE			19
#define CL_KEY_CONVERT			20
#define CL_KEY_NONCONVERT		21
#define CL_KEY_SPACE			22
#define CL_KEY_PRIOR			23
#define CL_KEY_NEXT				24
#define CL_KEY_END				25
#define CL_KEY_HOME				26
#define CL_KEY_LEFT				27
#define CL_KEY_UP				28
#define CL_KEY_RIGHT			29
#define CL_KEY_DOWN				30
#define CL_KEY_SELECT			31
#define CL_KEY_PRINT			32
#define CL_KEY_EXECUTE			33
#define CL_KEY_INSERT			34
#define CL_KEY_DELETE			35
#define CL_KEY_HELP				36
#define CL_KEY_0				37
#define CL_KEY_1				38
#define CL_KEY_2				39
#define CL_KEY_3				40
#define CL_KEY_4				41
#define CL_KEY_5				42
#define CL_KEY_6				43
#define CL_KEY_7				44
#define CL_KEY_8				45
#define CL_KEY_9				46
#define CL_KEY_A				47
#define CL_KEY_B				48
#define CL_KEY_C				49
#define CL_KEY_D				50
#define CL_KEY_E				51
#define CL_KEY_F				52
#define CL_KEY_G				53
#define CL_KEY_H				54
#define CL_KEY_I				55
#define CL_KEY_J				56
#define CL_KEY_K				57
#define CL_KEY_L				58
#define CL_KEY_M				59
#define CL_KEY_N				60
#define CL_KEY_O				61
#define CL_KEY_P				62
#define CL_KEY_Q				63
#define CL_KEY_R				64
#define CL_KEY_S				65
#define CL_KEY_T				66
#define CL_KEY_U				67
#define CL_KEY_V				68
#define CL_KEY_W				69
#define CL_KEY_X				70
#define CL_KEY_Y				71
#define CL_KEY_Z				72
#define CL_KEY_LWIN				73
#define CL_KEY_RWIN				74
#define CL_KEY_APPS				75
#define CL_KEY_NUMPAD0			76
#define CL_KEY_NUMPAD1			77
#define CL_KEY_NUMPAD2			78
#define CL_KEY_NUMPAD3			79
#define CL_KEY_NUMPAD4			80
#define CL_KEY_NUMPAD5			81
#define CL_KEY_NUMPAD6			82
#define CL_KEY_NUMPAD7			83
#define CL_KEY_NUMPAD8			84
#define CL_KEY_NUMPAD9			85
#define CL_KEY_MULTIPLY			86
#define CL_KEY_ADD				87
#define CL_KEY_SUBTRACT			89
#define CL_KEY_PERIOD			90
#define CL_KEY_DIVIDE			91
#define CL_KEY_F1				92
#define CL_KEY_F2				93
#define CL_KEY_F3				94
#define CL_KEY_F4				95
#define CL_KEY_F5				96
#define CL_KEY_F6				97
#define CL_KEY_F7				98
#define CL_KEY_F8				99
#define CL_KEY_F9				100
#define CL_KEY_F10				101
#define CL_KEY_F11				102
#define CL_KEY_F12				103
#define CL_KEY_F13				104
#define CL_KEY_F14				105
#define CL_KEY_F15				106
#define CL_KEY_F16				107
#define CL_KEY_F17				108
#define CL_KEY_F18				109
#define CL_KEY_F19				110
#define CL_KEY_F20				111
#define CL_KEY_F21				112
#define CL_KEY_F22				113
#define CL_KEY_F23				114
#define CL_KEY_F24				115
#define CL_KEY_NUMLOCK			116
#define CL_KEY_SCROLL			117
#define CL_KEY_LSHIFT			118
#define CL_KEY_RSHIFT			119
#define CL_KEY_LCONTROL			120
#define CL_KEY_RCONTROL			121
#define CL_KEY_LMENU			122
#define CL_KEY_RMENU			123
#define CL_KEY_NUMPAD_DECIMAL	124
#define CL_KEY_NUMPAD_MULTIPLY	125
#define CL_KEY_NUMPAD_ADD		126
#define CL_KEY_NUMPAD_DIVIDE	127
#define CL_KEY_NUMPAD_ENTER		128
#define CL_KEY_NUMPAD_SUBTRACT	129
#define CL_KEY_NUMPAD_EQUALS	130
#define CL_KEY_QUOTE			131
#define CL_KEY_SEMICOLON		132
#define CL_KEY_COMMA			133
#define CL_KEY_GRAVE			134
#define CL_KEY_PAGE_UP			135
#define CL_KEY_PAGE_DOWN		136
#define CL_KEY_EQUALS			137
#define CL_KEY_CAPSLOCK			138
#define CL_KEY_COMMAND			139
#define CL_KEY_LEFT_BRACKET 140
#define CL_KEY_RIGHT_BRACKET 141

// remappings:
#define CL_KEY_NUMPAD_0			CL_KEY_NUMPAD0
#define CL_KEY_NUMPAD_1			CL_KEY_NUMPAD1
#define CL_KEY_NUMPAD_2			CL_KEY_NUMPAD2
#define CL_KEY_NUMPAD_3			CL_KEY_NUMPAD3
#define CL_KEY_NUMPAD_4			CL_KEY_NUMPAD4
#define CL_KEY_NUMPAD_5			CL_KEY_NUMPAD5
#define CL_KEY_NUMPAD_6			CL_KEY_NUMPAD6
#define CL_KEY_NUMPAD_7			CL_KEY_NUMPAD7
#define CL_KEY_NUMPAD_8			CL_KEY_NUMPAD8
#define CL_KEY_NUMPAD_9			CL_KEY_NUMPAD9
#define CL_KEY_MENU				CL_KEY_OPTION
#define CL_KEY_ALT				CL_KEY_OPTION
#define CL_KEY_SEPARATOR		CL_KEY_DIVIDE
#define CL_KEY_DECIMAL			CL_KEY_PERIOD

#else

#include <X11/keysym.h>

#define CL_MOUSE_LEFT 0
#define CL_MOUSE_RIGHT 1
#define CL_MOUSE_MIDDLE 2
#define CL_MOUSE_WHEEL_UP 3
#define CL_MOUSE_WHEEL_DOWN 4
#define CL_MOUSE_XBUTTON1 5
#define CL_MOUSE_XBUTTON2 6

#define CL_KEY_BACKSPACE XK_BackSpace
#define CL_KEY_TAB XK_Tab
#define CL_KEY_CLEAR XK_Clear
#define CL_KEY_RETURN XK_Return
#define CL_KEY_SHIFT XK_Shift_L
#define CL_KEY_CONTROL XK_Control_L
#define CL_KEY_MENU XK_Alt_L
#define CL_KEY_PAUSE XK_Pause
#define CL_KEY_KANJI XK_Kanji
#define CL_KEY_ESCAPE XK_Escape
#define CL_KEY_CONVERT XK_Henkan_Mode
#define CL_KEY_NONCONVERT XK_Muhenkan
#define CL_KEY_SPACE XK_space
#define CL_KEY_PRIOR XK_Prior // aka PAGEUP
#define CL_KEY_NEXT XK_Next // aka PAGEDOWN
#define CL_KEY_END XK_End
#define CL_KEY_HOME XK_Home
#define CL_KEY_LEFT XK_Left
#define CL_KEY_UP XK_Up
#define CL_KEY_RIGHT XK_Right
#define CL_KEY_DOWN XK_Down
#define CL_KEY_SELECT XK_Select
#define CL_KEY_PRINT XK_Print
#define CL_KEY_EXECUTE XK_Execute
#define CL_KEY_INSERT XK_Insert
#define CL_KEY_DELETE XK_Delete
#define CL_KEY_HELP XK_Help
#define CL_KEY_0 XK_0
#define CL_KEY_1 XK_1
#define CL_KEY_2 XK_2
#define CL_KEY_3 XK_3
#define CL_KEY_4 XK_4
#define CL_KEY_5 XK_5
#define CL_KEY_6 XK_6
#define CL_KEY_7 XK_7
#define CL_KEY_8 XK_8
#define CL_KEY_9 XK_9
#define CL_KEY_A XK_a
#define CL_KEY_B XK_b
#define CL_KEY_C XK_c
#define CL_KEY_D XK_d
#define CL_KEY_E XK_e
#define CL_KEY_F XK_f
#define CL_KEY_G XK_g
#define CL_KEY_H XK_h
#define CL_KEY_I XK_i
#define CL_KEY_J XK_j
#define CL_KEY_K XK_k
#define CL_KEY_L XK_l
#define CL_KEY_M XK_m
#define CL_KEY_N XK_n
#define CL_KEY_O XK_o
#define CL_KEY_P XK_p
#define CL_KEY_Q XK_q
#define CL_KEY_R XK_r
#define CL_KEY_S XK_s
#define CL_KEY_T XK_t
#define CL_KEY_U XK_u
#define CL_KEY_V XK_v
#define CL_KEY_W XK_w
#define CL_KEY_X XK_x
#define CL_KEY_Y XK_y
#define CL_KEY_Z XK_z
#define CL_KEY_LWIN XK_Super_L
#define CL_KEY_RWIN XK_Multi_key
#define CL_KEY_APPS XK_Menu
#define CL_KEY_NUMPAD0 XK_KP_0
#define CL_KEY_NUMPAD1 XK_KP_1
#define CL_KEY_NUMPAD2 XK_KP_2
#define CL_KEY_NUMPAD3 XK_KP_3
#define CL_KEY_NUMPAD4 XK_KP_4
#define CL_KEY_NUMPAD5 XK_KP_5
#define CL_KEY_NUMPAD6 XK_KP_6
#define CL_KEY_NUMPAD7 XK_KP_7
#define CL_KEY_NUMPAD8 XK_KP_8
#define CL_KEY_NUMPAD9 XK_KP_9
#define CL_KEY_NUMPAD_MULTIPLY XK_KP_Multiply
#define CL_KEY_NUMPAD_ADD XK_KP_Add
#define CL_KEY_NUMPAD_SEPARATOR XK_KP_Separator
#define CL_KEY_NUMPAD_SUBTRACT XK_KP_Subtract
#define CL_KEY_NUMPAD_DECIMAL XK_KP_Decimal
#define CL_KEY_NUMPAD_DIVIDE XK_KP_Divide
#define CL_KEY_F1 XK_F1
#define CL_KEY_F2 XK_F2
#define CL_KEY_F3 XK_F3
#define CL_KEY_F4 XK_F4
#define CL_KEY_F5 XK_F5
#define CL_KEY_F6 XK_F6
#define CL_KEY_F7 XK_F7
#define CL_KEY_F8 XK_F8
#define CL_KEY_F9 XK_F9
#define CL_KEY_F10 XK_F10
#define CL_KEY_F11 XK_F11
#define CL_KEY_F12 XK_F12
#define CL_KEY_F13 XK_F13
#define CL_KEY_F14 XK_F14
#define CL_KEY_F15 XK_F15
#define CL_KEY_F16 XK_F16
#define CL_KEY_F17 XK_F17
#define CL_KEY_F18 XK_F18
#define CL_KEY_F19 XK_F19
#define CL_KEY_F20 XK_F20
#define CL_KEY_F21 XK_F21
#define CL_KEY_F22 XK_F22
#define CL_KEY_F23 XK_F23
#define CL_KEY_F24 XK_F24
#define CL_KEY_NUMLOCK XK_Num_Lock
#define CL_KEY_SCROLL XK_Scroll_Lock
#define CL_KEY_LSHIFT XK_Shift_L
#define CL_KEY_RSHIFT XK_Shift_R
#define CL_KEY_LCONTROL XK_Control_L
#define CL_KEY_RCONTROL XK_Control_R
#define CL_KEY_LMENU XK_Meta_L
#define CL_KEY_RMENU XK_Meta_R
#define CL_KEY_GRAVE XK_grave
#define CL_KEY_SUBTRACT XK_minus
#define CL_KEY_EQUALS XK_equal
#define CL_KEY_SEMICOLON XK_semicolon
#define CL_KEY_QUOTE XK_apostrophe
#define CL_KEY_COMMA XK_comma
#define CL_KEY_PERIOD XK_period
#define CL_KEY_DIVIDE XK_slash

#define CL_KEY_LEFT_BRACKET XK_bracketleft
#define CL_KEY_RIGHT_BRACKET XK_bracketright


#endif
#endif

#define CL_KEY_ENTER CL_KEY_RETURN
#define CL_KEY_LAPPLE CL_KEY_LWIN
#define CL_KEY_RAPPLE CL_KEY_RWIN
#define CL_KEY_TILDE CL_KEY_GRAVE

#endif
