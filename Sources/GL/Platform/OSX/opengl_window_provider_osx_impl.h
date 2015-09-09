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
 **    James Lammlein
 **    Magnus Norddahl
 */

#pragma once

#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Window/input_device.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_context_description.h"
#include "API/Core/Text/logger.h"
#include "GL/opengl_context_description_impl.h"
#include "GL/GL3/gl3_graphic_context_provider.h"

#import <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>

@class CocoaWindow;

namespace clan
{

class OpenGLWindowProvider_Impl
{
public:
	OpenGLWindowProvider_Impl(OpenGLWindowProvider *self, OpenGLContextDescription &opengl_desc);

	NSOpenGLContext *get_share_context();
	void on_input_event(NSEvent *theEvent);

	OpenGLWindowProvider *self;
	GraphicContext gc;
	DisplayWindowSite *site;
	OpenGLContextDescription opengl_desc;

	CocoaWindow *window;
    NSOpenGLContext *opengl_context;

private:
    void on_keyboard_event(NSEvent *theEvent);
    void on_mouse_event(NSEvent *theEvent);
};

}
