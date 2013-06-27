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

#include "GL/precomp.h"
#include "opengl_window_provider_osx.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_window_description.h"
#include "API/Core/Text/logger.h"
#include "../opengl_window_description_impl.h"
#include "../GL3/gl3_graphic_context_provider.h"
#include <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>

namespace clan
{

// static CFBundleRef cl_gBundleRefOpenGL = 0;
	
class OpenGLWindowProvider_Impl
{
public:
	OpenGLWindowProvider_Impl(OpenGLWindowProvider *self, OpenGLWindowDescription &opengl_desc)
		: self(self), site(0), opengl_desc(opengl_desc)
	{
	}
	
	NSOpenGLContext *get_share_context()
	{
		NSOpenGLContext *share_context = nil;
		
		std::unique_ptr<MutexSection> mutex_section;
		GraphicContextProvider* gc_providers = SharedGCData::get_provider(mutex_section);
		if (gc_providers)
		{
			GL3GraphicContextProvider *gl3_provider = dynamic_cast<GL3GraphicContextProvider*>(gc_providers);
			if (gl3_provider)
			{
				const OpenGLWindowProvider *render_window_wgl = dynamic_cast<const OpenGLWindowProvider*>(&gl3_provider->get_render_window());
				if (render_window_wgl)
					share_context = render_window_wgl->impl->opengl_context;
			}
		}
		
		return share_context;
	}
	
	OpenGLWindowProvider *self;
	GraphicContext gc;
	InputContext ic;
	DisplayWindowSite *site;
	OpenGLWindowDescription opengl_desc;
	
	NSWindow *window;
	NSOpenGLContext *opengl_context;
	
	std::string window_title;
};

OpenGLWindowProvider::OpenGLWindowProvider(OpenGLWindowDescription &opengl_desc)
{
	impl.reset(new OpenGLWindowProvider_Impl(this, opengl_desc));
}

OpenGLWindowProvider::~OpenGLWindowProvider()
{
}

ProcAddress *OpenGLWindowProvider::get_proc_address(const std::string& function_name) const
{
	return (void (*)())0;
}

Rect OpenGLWindowProvider::get_geometry() const
{
	return Rect();
}

Rect OpenGLWindowProvider::get_viewport() const
{
	return Rect();
}

bool OpenGLWindowProvider::is_fullscreen() const
{
	return false;
}

bool OpenGLWindowProvider::has_focus() const
{
	return true;
}

bool OpenGLWindowProvider::is_minimized() const
{
	return false;
}

bool OpenGLWindowProvider::is_maximized() const
{
	return false;
}

bool OpenGLWindowProvider::is_visible() const
{
	return true;
}

Size OpenGLWindowProvider::get_minimum_size(bool client_area) const
{
	return Size();
}

Size OpenGLWindowProvider::get_maximum_size(bool client_area) const
{
	return Size(32768, 32768);
}

GraphicContext& OpenGLWindowProvider::get_gc()
{
	return impl->gc;
}
	
InputContext OpenGLWindowProvider::get_ic()
{
	return impl->ic;
}
	
std::string OpenGLWindowProvider::get_title() const
{
	return impl->window_title;
}

bool OpenGLWindowProvider::is_clipboard_text_available() const
{
	return false;
}

bool OpenGLWindowProvider::is_clipboard_image_available() const
{
	return false;
}

void OpenGLWindowProvider::make_current() const
{
	[impl->opengl_context makeCurrentContext];
}

Point OpenGLWindowProvider::client_to_screen(const Point &client)
{
	return client;
}

Point OpenGLWindowProvider::screen_to_client(const Point &screen)
{
	return screen;
}

void OpenGLWindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
{
	impl->site = new_site;

	NSRect frame = NSMakeRect(0, 0, 200, 200);
	NSUInteger styles = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
	impl->window = [[NSWindow alloc] initWithContentRect:frame styleMask:styles backing:NSBackingStoreBuffered defer:NO];
	if (impl->window == nil)
		throw Exception("Could not create window");
	
	std::vector<NSOpenGLPixelFormatAttribute> attributes;
	
	attributes.push_back(NSOpenGLPFADoubleBuffer);
	attributes.push_back(NSOpenGLPFAColorSize);
	attributes.push_back(24);
	attributes.push_back(NSOpenGLPFAAlphaSize);
	attributes.push_back(8);
	attributes.push_back(NSOpenGLPFADepthSize);
	attributes.push_back(desc.get_depth_size());
	attributes.push_back(NSOpenGLPFAStencilSize);
	attributes.push_back(desc.get_stencil_size());
	attributes.push_back(NSOpenGLPFAMinimumPolicy); // Color and depth must minimum match what we specified
	if (desc.is_fullscreen())
		attributes.push_back(NSOpenGLPFAFullScreen);
	if (desc.get_multisampling() > 0)
	{
		attributes.push_back(NSOpenGLPFAMultisample);
		attributes.push_back(NSOpenGLPFASampleBuffers);
		attributes.push_back(1);
		attributes.push_back(NSOpenGLPFASamples);
		attributes.push_back(desc.get_multisampling());
	}
	attributes.push_back(0);
	
	NSOpenGLPixelFormat *pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attributes[0]];
	if (pixel_format == nil)
		throw Exception("Could not create the requested OpenGL pixel format");
	
	impl->opengl_context = [[NSOpenGLContext alloc] initWithFormat:pixel_format shareContext:impl->get_share_context()];
	if (impl->opengl_context == nil)
		throw Exception("Could not create OpenGL context");
	
	[impl->opengl_context setView:impl->window.contentView];
	
	[impl->window makeKeyAndOrderFront:NSApp];
}

void OpenGLWindowProvider::show_system_cursor()
{
}

CursorProvider *OpenGLWindowProvider::create_cursor(const CursorDescription &cursor_description, const Point &hotspot)
{
	throw Exception("Custom cursors not supported yet by OS X target");
}

void OpenGLWindowProvider::set_cursor(CursorProvider *cursor)
{
}

void OpenGLWindowProvider::set_cursor(StandardCursor type)
{
}

void OpenGLWindowProvider::hide_system_cursor()
{
}

void OpenGLWindowProvider::set_title(const std::string &new_title)
{
	impl->window_title = new_title;
}

void OpenGLWindowProvider::set_position(const Rect &pos, bool client_area)
{
}

void OpenGLWindowProvider::set_size(int width, int height, bool client_area)
{
}

void OpenGLWindowProvider::set_minimum_size( int width, int height, bool client_area )
{
}

void OpenGLWindowProvider::set_maximum_size( int width, int height, bool client_area )
{
}

void OpenGLWindowProvider::set_enabled(bool enable)
{
}

void OpenGLWindowProvider::minimize()
{
}

void OpenGLWindowProvider::restore()
{
}

void OpenGLWindowProvider::maximize()
{
}

void OpenGLWindowProvider::show(bool activate)
{
}

void OpenGLWindowProvider::hide()
{
}

void OpenGLWindowProvider::bring_to_front()
{
	[impl->window makeKeyAndOrderFront:NSApp];
}

void OpenGLWindowProvider::flip(int interval)
{
	OpenGL::set_active(get_gc());
	OpenGL::check_error();
	
	[impl->opengl_context flushBuffer];
}

void OpenGLWindowProvider::update(const Rect &_rect)
{
	OpenGL::set_active(get_gc());
	
	// To do: copy rect from back to front buffer
	glFlush();
	
	OpenGL::check_error();
}

void OpenGLWindowProvider::capture_mouse(bool capture)
{
}

void OpenGLWindowProvider::set_clipboard_text(const std::string &text)
{
}

std::string OpenGLWindowProvider::get_clipboard_text() const
{
	return std::string();
}

void OpenGLWindowProvider::request_repaint(const Rect &rect)
{
}

void OpenGLWindowProvider::set_large_icon(const PixelBuffer &image)
{
}

void OpenGLWindowProvider::set_small_icon(const PixelBuffer &image)
{
}

void OpenGLWindowProvider::enable_alpha_channel(const Rect &blur_rect)
{
}

void OpenGLWindowProvider::extend_frame_into_client_area(int height)
{
}

void OpenGLWindowProvider::set_clipboard_image(const PixelBuffer &buf)
{
}

PixelBuffer OpenGLWindowProvider::get_clipboard_image() const
{
	return PixelBuffer();
}

}
