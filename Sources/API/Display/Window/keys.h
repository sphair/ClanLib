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

// Keyboard & mouse platform independence support.

#ifdef WIN32
#include <windows.h>
#endif

namespace clan
{
/// \addtogroup clanDisplay_Input clanDisplay Input
/// \{

enum InputCode
{
	mouse_left=0,
	mouse_right=1,
	mouse_middle=2,
	mouse_wheel_up=3,
	mouse_wheel_down=4,
	mouse_xbutton1=5,
	mouse_xbutton2=6,

	tablet_x_axis = 0x10000,
	tablet_y_axis,
	tablet_z_axis,
	tablet_tilt_x,
	tablet_tilt_y,
	tablet_rotation_pitch,
	tablet_rotation_roll,
	tablet_rotation_yaw,
	tablet_proximity_enter,
	tablet_proximity_exit,
	tablet_key,		// "id_offset" set

	joystick_x = 0,
	joystick_y,
	joystick_z,
	joystick_rx,
	joystick_ry,
	joystick_rz,
	joystick_slider,
	joystick_dial,
	joystick_wheel,
	joystick_vx,
	joystick_vy,
	joystick_vz,
	joystick_vrx,
	joystick_vry,
	joystick_vrz,
	joystick_vslider0,
	joystick_vslider1,
	joystick_ax,
	joystick_ay,
	joystick_az,
	joystick_arx,
	joystick_ary,
	joystick_arz,
	joystick_aslider0,
	joystick_aslider1,
	joystick_fx,
	joystick_fy,
	joystick_fz,
	joystick_frx,
	joystick_fry,
	joystick_frz,
	joystick_fslider0,
	joystick_fslider1,

	joystick_button = 0,	// "id_offset" set
	joystick_hat = 0,		// "id_offset" set

#ifdef WIN32
	keycode_backspace=VK_BACK,
	keycode_tab=VK_TAB,
	keycode_clear=VK_CLEAR,
	keycode_return=VK_RETURN,
	keycode_shift=VK_SHIFT,
	keycode_control=VK_CONTROL,
	keycode_menu=VK_MENU,
	keycode_pause=VK_PAUSE,
	keycode_escape=VK_ESCAPE,

#if !defined(__CYGWIN__) && !defined(__MINGW32__)
	keycode_kanji=VK_KANJI,
	keycode_convert=VK_CONVERT,
	keycode_nonconvert=VK_NONCONVERT,
#endif

	keycode_space=VK_SPACE,
	keycode_prior=VK_PRIOR,
	keycode_next=VK_NEXT,
	keycode_end=VK_END,
	keycode_home=VK_HOME,
	keycode_left=VK_LEFT,
	keycode_up=VK_UP,
	keycode_right=VK_RIGHT,
	keycode_down=VK_DOWN,
	keycode_select=VK_SELECT,
	keycode_print=VK_PRINT,
	keycode_execute=VK_EXECUTE,
	keycode_insert=VK_INSERT,
	keycode_delete=VK_DELETE,
	keycode_help=VK_HELP,
	keycode_0='0',
	keycode_1='1',
	keycode_2='2',
	keycode_3='3',
	keycode_4='4',
	keycode_5='5',
	keycode_6='6',
	keycode_7='7',
	keycode_8='8',
	keycode_9='9',
	keycode_a='A',
	keycode_b='B',
	keycode_c='C',
	keycode_d='D',
	keycode_e='E',
	keycode_f='F',
	keycode_g='G',
	keycode_h='H',
	keycode_i='I',
	keycode_j='J',
	keycode_k='K',
	keycode_l='L',
	keycode_m='M',
	keycode_n='N',
	keycode_o='O',
	keycode_p='P',
	keycode_q='Q',
	keycode_r='R',
	keycode_s='S',
	keycode_t='T',
	keycode_u='U',
	keycode_v='V',
	keycode_w='W',
	keycode_x='X',
	keycode_y='Y',
	keycode_z='Z',
	keycode_lwin=VK_LWIN,
	keycode_rwin=VK_RWIN,
	keycode_apps=VK_APPS,
	keycode_numpad0=VK_NUMPAD0,
	keycode_numpad1=VK_NUMPAD1,
	keycode_numpad2=VK_NUMPAD2,
	keycode_numpad3=VK_NUMPAD3,
	keycode_numpad4=VK_NUMPAD4,
	keycode_numpad5=VK_NUMPAD5,
	keycode_numpad6=VK_NUMPAD6,
	keycode_numpad7=VK_NUMPAD7,
	keycode_numpad8=VK_NUMPAD8,
	keycode_numpad9=VK_NUMPAD9,
	keycode_numpad_enter=keycode_return,
	keycode_multiply=VK_MULTIPLY,
	keycode_add=VK_ADD,
	keycode_separator=VK_SEPARATOR,
	keycode_subtract=VK_SUBTRACT,
	keycode_decimal=VK_DECIMAL,
	keycode_divide=VK_DIVIDE,
	keycode_f1=VK_F1,
	keycode_f2=VK_F2,
	keycode_f3=VK_F3,
	keycode_f4=VK_F4,
	keycode_f5=VK_F5,
	keycode_f6=VK_F6,
	keycode_f7=VK_F7,
	keycode_f8=VK_F8,
	keycode_f9=VK_F9,
	keycode_f10=VK_F10,
	keycode_f11=VK_F11,
	keycode_f12=VK_F12,
	keycode_f13=VK_F13,
	keycode_f14=VK_F14,
	keycode_f15=VK_F15,
	keycode_f16=VK_F16,
	keycode_f17=VK_F17,
	keycode_f18=VK_F18,
	keycode_f19=VK_F19,
	keycode_f20=VK_F20,
	keycode_f21=VK_F21,
	keycode_f22=VK_F22,
	keycode_f23=VK_F23,
	keycode_f24=VK_F24,
	keycode_numlock=VK_NUMLOCK,
	keycode_scroll=VK_SCROLL,
	keycode_lshift=VK_LSHIFT,
	keycode_rshift=VK_RSHIFT,
	keycode_lcontrol=VK_LCONTROL,
	keycode_rcontrol=VK_RCONTROL,
	keycode_lmenu=VK_LMENU,
	keycode_rmenu=VK_RMENU,

#else
#ifdef __APPLE__

// Seems like this platform dont have keysyms, or their docs suck so much I
// can't find it.
//
// To solve this matter I've made my own virtual key numbering. If a key
// pressed does not match any of these, then it will return the actual
// MacOSX keycode in the high order word (keycode + 0x10000000).
//
// Naturally this require that they dont use keycode values above 0xffff,
// but if they do complain to Apple for their crappy keyboard support.
// Already kinda annoyed that the keys for typing { and } are alt+shift+8 and
// alt+shift+9, plus that backslash is alt+shift+7. How hostile is that!?!?
// Oh well what can you expect from a company that write "Designed by Apple
// in California" with big letters when you open your box that the powerbook
// came in. Yes it shows thats its designed in California!!
//
// (No offence to California though. Been there once and loved the place.)

    // New values for Cocoa.
    keycode_escape=0x35,
    keycode_left=0x7B,
	keycode_right=0x7C,
	keycode_down=0x7D,
    keycode_up=0x7E,
    keycode_0=0x1D,
	keycode_1=0x12,
	keycode_2=0x13,
	keycode_3=0x14,
	keycode_4=0x15,
	keycode_5=0x17,
	keycode_6=0x16,
	keycode_7=0x1A,
	keycode_8=0x1C,
	keycode_9=0x19,
	keycode_a=0x00,
	keycode_b=0x0B,
	keycode_c=0x08,
	keycode_d=0x02,
	keycode_e=0x0E,
	keycode_f=0x03,
	keycode_g=0x05,
	keycode_h=0x04,
	keycode_i=0x22,
	keycode_j=0x26,
	keycode_k=0x28,
	keycode_l=0x25,
	keycode_m=0x2E,
	keycode_n=0x2D,
	keycode_o=0x1F,
	keycode_p=0x23,
	keycode_q=0x0C,
	keycode_r=0x0F,
	keycode_s=0x01,
	keycode_t=0x11,
	keycode_u=0x20,
	keycode_v=0x09,
	keycode_w=0x0D,
	keycode_x=0x07,
	keycode_y=0x10,
	keycode_z=0x06,
    
    // TODO: Finish updating these.
    // Original values hacked with placeholders to prevent conflicts for now.
	keycode_backspace=10,
	keycode_tab=0xFF0C,         // 11, Conflicts.
	keycode_clear=0xFF0F,       // 12, Conflicts.
	keycode_return=0xFF0D,      // 13, Conflicts.
	keycode_shift=14,
	keycode_control=15,
	keycode_menu=0xFF0E,        // 16, Conflicts.
	keycode_pause=0xFF10,       // 17, Conflicts.
	keycode_kanji=0xFF01,       // 18, Conflicts.
	keycode_convert=0xFF02,     // 20, Conflicts.
	keycode_nonconvert=0xFF03,  // 21, Conflicts.
	keycode_space=0xFF04,       // 22, Conflicts.
	keycode_prior=0xFF05,       // 23, Conflicts.
	keycode_next=0xFF06,        // 24, Conflicts.
	keycode_end=0xFF07,         // 25, Conflicts.
	keycode_home=0xFF08,        // 26, Conflicts.
	keycode_select=0xFF11,      // 31, Conflicts.
	keycode_print=0xFF12,       // 32, Conflicts.
	keycode_execute=0xFF13,     // 33, Conflicts.
	keycode_insert=0xFF14,      // 34, Conflicts.
	keycode_delete=0xFF15,      // 35, Conflicts.
	keycode_help=0xFF16,        // 36, Conflicts.
	keycode_lwin=73,
	keycode_rwin=74,
	keycode_apps=75,
	keycode_numpad0=76,
	keycode_numpad1=77,
	keycode_numpad2=78,
	keycode_numpad3=79,
	keycode_numpad4=80,
	keycode_numpad5=81,
	keycode_numpad6=82,
	keycode_numpad7=83,
	keycode_numpad8=84,
	keycode_numpad9=85,
	keycode_numpad_enter=keycode_return,
	keycode_multiply=86,
	keycode_add=87,
	keycode_separator=88,
	keycode_subtract=89,
	keycode_decimal=90,
	keycode_divide=91,
	keycode_f1=92,
	keycode_f2=93,
	keycode_f3=94,
	keycode_f4=95,
	keycode_f5=96,
	keycode_f6=97,
	keycode_f7=98,
	keycode_f8=99,
	keycode_f9=100,
	keycode_f10=101,
	keycode_f11=102,
	keycode_f12=103,
	keycode_f13=104,
	keycode_f14=105,
	keycode_f15=106,
	keycode_f16=107,
	keycode_f17=108,
	keycode_f18=109,
	keycode_f19=110,
	keycode_f20=111,
	keycode_f21=112,
	keycode_f22=113,
	keycode_f23=114,
	keycode_f24=115,
	keycode_numlock=116,
	keycode_scroll=117,
	keycode_lshift=118,
	keycode_rshift=119,
	keycode_lcontrol=120,
	keycode_rcontrol=121,
	keycode_lmenu=122,
	keycode_rmenu=0xFF17,        // 123, Conflicts.

#else

#include <X11/keysym.h>

	keycode_backspace=XK_BackSpace,
	keycode_tab=XK_Tab,
	keycode_clear=XK_Clear,
	keycode_return=XK_Return,
	keycode_shift=XK_Shift_L,
	keycode_control=XK_Control_L,
	keycode_menu=XK_Menu, // there is no XK_Alt, only XK_Alt_L and XK_Alt_R. Maybe remove this key? -- mbn 30 sep 2003
	keycode_pause=XK_Pause,
	keycode_kanji=XK_Kanji,
	keycode_escape=XK_Escape,
	keycode_convert=XK_Henkan_Mode,
	keycode_nonconvert=XK_Muhenkan,
	keycode_space=XK_space,
	keycode_prior=XK_Prior,
	keycode_next=XK_Next,
	keycode_end=XK_End,
	keycode_home=XK_Home,
	keycode_left=XK_Left,
	keycode_up=XK_Up,
	keycode_right=XK_Right,
	keycode_down=XK_Down,
	keycode_select=XK_Select,
	keycode_print=XK_Print,
	keycode_execute=XK_Execute,
	keycode_insert=XK_Insert,
	keycode_delete=XK_Delete,
	keycode_help=XK_Help,
	keycode_0=XK_0,
	keycode_1=XK_1,
	keycode_2=XK_2,
	keycode_3=XK_3,
	keycode_4=XK_4,
	keycode_5=XK_5,
	keycode_6=XK_6,
	keycode_7=XK_7,
	keycode_8=XK_8,
	keycode_9=XK_9,
	keycode_a=XK_a,
	keycode_b=XK_b,
	keycode_c=XK_c,
	keycode_d=XK_d,
	keycode_e=XK_e,
	keycode_f=XK_f,
	keycode_g=XK_g,
	keycode_h=XK_h,
	keycode_i=XK_i,
	keycode_j=XK_j,
	keycode_k=XK_k,
	keycode_l=XK_l,
	keycode_m=XK_m,
	keycode_n=XK_n,
	keycode_o=XK_o,
	keycode_p=XK_p,
	keycode_q=XK_q,
	keycode_r=XK_r,
	keycode_s=XK_s,
	keycode_t=XK_t,
	keycode_u=XK_u,
	keycode_v=XK_v,
	keycode_w=XK_w,
	keycode_x=XK_x,
	keycode_y=XK_y,
	keycode_z=XK_z,
	keycode_lwin=XK_Super_L,
	keycode_rwin=XK_Multi_key,
	keycode_apps=XK_Menu,
	keycode_numpad0=XK_KP_0,
	keycode_numpad1=XK_KP_1,
	keycode_numpad2=XK_KP_2,
	keycode_numpad3=XK_KP_3,
	keycode_numpad4=XK_KP_4,
	keycode_numpad5=XK_KP_5,
	keycode_numpad6=XK_KP_6,
	keycode_numpad7=XK_KP_7,
	keycode_numpad8=XK_KP_8,
	keycode_numpad9=XK_KP_9,
	keycode_numpad_enter=XK_KP_Enter,
	keycode_multiply=XK_KP_Multiply,
	keycode_add=XK_KP_Add,
	keycode_separator=XK_KP_Separator,
	keycode_subtract=XK_KP_Subtract,
	keycode_decimal=XK_KP_Decimal,
	keycode_divide=XK_KP_Divide,
	keycode_f1=XK_F1,
	keycode_f2=XK_F2,
	keycode_f3=XK_F3,
	keycode_f4=XK_F4,
	keycode_f5=XK_F5,
	keycode_f6=XK_F6,
	keycode_f7=XK_F7,
	keycode_f8=XK_F8,
	keycode_f9=XK_F9,
	keycode_f10=XK_F10,
	keycode_f11=XK_F11,
	keycode_f12=XK_F12,
	keycode_f13=XK_F13,
	keycode_f14=XK_F14,
	keycode_f15=XK_F15,
	keycode_f16=XK_F16,
	keycode_f17=XK_F17,
	keycode_f18=XK_F18,
	keycode_f19=XK_F19,
	keycode_f20=XK_F20,
	keycode_f21=XK_F21,
	keycode_f22=XK_F22,
	keycode_f23=XK_F23,
	keycode_f24=XK_F24,
	keycode_numlock=XK_Num_Lock,
	keycode_scroll=XK_Scroll_Lock,
	keycode_lshift=XK_Shift_L,
	keycode_rshift=XK_Shift_R,
	keycode_lcontrol=XK_Control_L,
	keycode_rcontrol=XK_Control_R,
	keycode_lmenu=XK_Meta_L,
	keycode_rmenu=XK_Meta_R,
#endif
#endif

	keycode_enter=keycode_return,
	keycode_lapple=keycode_lwin,
	keycode_rapple=keycode_rwin,

	keycode_unknown=-1
};

}

/// \}
