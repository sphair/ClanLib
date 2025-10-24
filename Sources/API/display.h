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

//: <p>ClanLib display and input library. This provide you with 2D display
//: support, and the input devices attached to that display.</p>
//! Global=Display

#ifndef header_display_include
#define header_display_include

#if _MSC_VER > 1000
#pragma once
#endif

#include "Display/blend_func.h"
#include "Display/color.h"
#include "Display/display.h"
#include "Display/display_iostream.h"
#include "Display/display_mode.h"
#include "Display/display_window.h"
#include "Display/display_window_description.h"
#include "Display/font.h"
#include "Display/frameratecounter.h"
#include "Display/glyph_buffer.h"
#include "Display/gradient.h"
#include "Display/graphic_context.h"
#include "Display/input_buffer.h"
#include "Display/input_button.h"
#include "Display/input_context.h"
#include "Display/input_device.h"
#include "Display/input_event.h"
#include "Display/joystick.h"
#include "Display/key_binding.h"
#include "Display/keyboard.h"
#include "Display/keys.h"
#include "Display/mouse.h"
#include "Display/palette.h"
#include "Display/pixel_buffer.h"
#include "Display/pixel_format.h"
#include "Display/pixel_format_type.h"
#include "Display/rle_surface.h"
#include "Display/setupdisplay.h"
#include "Display/sprite.h"
#include "Display/sprite_description.h"
#include "Display/sprite_packer.h"
#include "Display/surface.h"
#include "Display/canvas.h"
#include "Display/text_styler.h"
#include "Display/Providers/jpeg_provider.h"
#include "Display/Providers/pcx_provider.h"
#include "Display/Providers/bmp_provider.h"
#include "Display/Providers/png_provider.h"
#include "Display/Providers/targa_provider.h"
#include "Display/Providers/provider_factory.h"
#include "Display/Providers/provider_type.h"
#include "Display/Collision/collision_outline.h"
#include "Display/Collision/outline_accuracy.h"

#if defined (_MSC_VER)
#if !defined (_DEBUG)
#pragma comment(lib, "clanDisplay-static-mt.lib")
#pragma comment(lib, "libjpeg-static-mt.lib")
#pragma comment(lib, "libpng-static-mt.lib")
#else
#pragma comment(lib, "libjpeg-static-mt-debug.lib")
#pragma comment(lib, "clanDisplay-static-mt-debug.lib")
#pragma comment(lib, "libpng-static-mt-debug.lib")
#endif
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#endif

#endif
