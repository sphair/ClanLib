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

#include "GL/precomp.h"
#include "opengl_window_provider_agl.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl_window_description.h"
#include "API/Core/Text/logger.h"
//#include "Display/Cocoa/cursor_provider_cocoa.h"
#include "../../opengl_window_description_impl.h"
#include "../../opengl_graphic_context_provider.h"
#include "../opengl_target_provider.h"

namespace clan
{

void cl_agl_make_none_current()
{
    [EAGLContext setCurrentContext:nil];
}

OpenGLWindowProvider *cl_alloc_display_window_agl()
{
    return new OpenGLWindowProvider();
}

void cl_set_default_frame_buffer(DisplayWindowProvider *provider)
{
    CL_GL_DisplayWindowProvider_AGL *wptr = dynamic_cast<CL_GL_DisplayWindowProvider_AGL *>(provider);
    if (wptr)
        wptr->set_default_frame_buffer();
}

CL_GL_RenderWindowProvider_AGL::CL_GL_RenderWindowProvider_AGL(OpenGLWindowProvider & window, EAGLContext *agl_context, bool own_context)
	: window(window), agl_context(agl_context), own_context(own_context)
{
}

CL_GL_RenderWindowProvider_AGL::~CL_GL_RenderWindowProvider_AGL()
{
	if( agl_context && own_context)
	{
        [agl_context release];
        agl_context = 0;
	}
}

Size CL_GL_RenderWindowProvider_AGL::get_viewport_size() const
{
	return window.get_viewport().get_size();
}

void CL_GL_RenderWindowProvider_AGL::flip_buffers(int interval) const
{
	window.flip(interval);
}

void CL_GL_RenderWindowProvider_AGL::make_current() const
{
    [EAGLContext setCurrentContext:agl_context];
}

void CL_GL_RenderWindowProvider_AGL::set_default_frame_buffer()
{
    window.set_default_frame_buffer();
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Construction:

OpenGLWindowProvider::OpenGLWindowProvider()
: cocoa_window(), opengl_context(0), swap_interval(-1), default_framebuffer_handle(0), default_colorbuffer_handle(0), default_depthbuffer_handle(0)
{
	cocoa_window.func_on_resized() = bind_member(this, &OpenGLWindowProvider::on_window_resized);
}

OpenGLWindowProvider::~OpenGLWindowProvider()
{
	if (opengl_context)
	{
		if (!gc.is_null())
		{
			OpenGLGraphicContextProvider *gl_provider = dynamic_cast<OpenGLGraphicContextProvider*>(gc.get_provider());
			if (gl_provider)
				gl_provider->dispose();
		}

		// Delete the context
        OpenGL::set_active(NULL);
        [opengl_context release];
		opengl_context = 0;
	}

}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Operations:

void OpenGLWindowProvider::make_current() const
{
    [EAGLContext setCurrentContext:agl_context];
}

void OpenGLWindowProvider::create(DisplayWindowSite *new_site, const DisplayWindowDescription &desc)
{
	site = new_site;

	cocoa_window.create(site, desc);

	if (!opengl_context)
	{
        opengl_context = create_context();

		OpenGLWindowDescription gldesc(desc);
   		gc = GraphicContext(new OpenGLGraphicContextProvider(new CL_GL_RenderWindowProvider_AGL(*this, opengl_context, false), gldesc ));
        setup_default_framebuffer();
	}

	swap_interval = desc.get_swap_interval();
	if (swap_interval != -1)
	{
/*		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(swap_interval);
		}*/
	}
}

void OpenGLWindowProvider::on_window_resized()
{
	if (gc.get_provider())
		((OpenGLGraphicContextProvider *) gc.get_provider())->on_window_resized();
}

EAGLContext *OpenGLWindowProvider::create_context()
{
	EAGLContext *shared_context = NULL;

	GraphicContextProvider* gc_providers = SharedGCData::get_provider();
	if (gc_providers)
	{
		OpenGLGraphicContextProvider *gl_provider = dynamic_cast<OpenGLGraphicContextProvider*>(gc_providers);
		if (gl_provider)
		{
			const DisplayWindowProvider *rwp = &gl_provider->get_render_window();
			const CL_GL_DisplayWindowProvider_AGL *render_window_agl = dynamic_cast<const CL_GL_DisplayWindowProvider_AGL*>(rwp);
			if (render_window_agl)
				shared_context = render_window_agl->get_context();
		}
	}

    EAGLContext *context;
    if (shared_context)
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:shared_context.sharegroup];
    }
    else
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:nil];
    }

    return context;
}

void OpenGLWindowProvider::setup_default_framebuffer()
{
    //[EAGLContext setCurrentContext:context];
	OpenGL::set_active(gc);

    if (default_framebuffer_handle == 0)
        glGenFramebuffers(1, &default_framebuffer_handle);
    if (default_colorbuffer_handle == 0)
        glGenRenderbuffers(1, &default_colorbuffer_handle);
    if (default_depthbuffer_handle == 0)
        glGenRenderbuffers(1, &default_depthbuffer_handle);

    glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer_handle);

    glBindRenderbuffer(GL_RENDERBUFFER, default_depthbuffer_handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, get_viewport().get_width(), get_viewport().get_height());

    glBindRenderbuffer(GL_RENDERBUFFER, default_colorbuffer_handle);
    BOOL result = [opengl_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:cocoa_window.get_drawable()];
    if (!result)
    {
        throw Exception("EAGLContext.renderbufferStorage failed");
    }

    GLint width = 0, height = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, default_colorbuffer_handle);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, default_depthbuffer_handle);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw Exception("glCheckFramebufferStatus failed");
    }

    glViewport(0, 0, width, height);
}

void OpenGLWindowProvider::set_default_frame_buffer()
{
    OpenGL::set_active(gc);
    glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer_handle);
}

void OpenGLWindowProvider::flip(int interval)
{
	GraphicContext gc = get_gc();
	OpenGL::set_active(gc);
	glFlush();
/*
	if (interval != -1 && interval != swap_interval)
	{
		swap_interval = interval;
		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI(swap_interval);
		}
		else if (glXSwapIntervalMESA)
		{
			glXSwapIntervalMESA(swap_interval);
		}
	}
*/
    [opengl_context presentRenderbuffer:GL_RENDERBUFFER];

	OpenGL::check_error();
}

CursorProvider *OpenGLWindowProvider::create_cursor(const SpriteDescription &sprite_description, const Point &hotspot)
{
//	return new CursorProvider_Cocoa(sprite_description, hotspot);
    return 0;
}

void OpenGLWindowProvider::set_cursor(CursorProvider *cursor)
{
//	cocoa_window.set_cursor(static_cast<CursorProvider_Cocoa *>(cursor));
}

void OpenGLWindowProvider::set_large_icon(const PixelBuffer &image)
{
	cocoa_window.set_large_icon(image);
}

void OpenGLWindowProvider::set_small_icon(const PixelBuffer &image)
{
	cocoa_window.set_small_icon(image);
}

void OpenGLWindowProvider::enable_alpha_channel(const Rect &blur_rect)
{
	// Implement me, if possible
}

void OpenGLWindowProvider::extend_frame_into_client_area(int left, int top, int right, int bottom)
{
	// Implement me, if possible
}

ProcAddress *OpenGLWindowProvider::get_proc_address(const std::string& function_name) const
{
	//TODO: If "cl_gBundleRefOpenGL" not found, it is because it is located elsewhere, but where? (This function was moved)

	// Mac OS X doesn't have an OpenGL extension fetch function. Isn't that silly?
	if (cl_gBundleRefOpenGL == 0)
	{
		cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
		if (cl_gBundleRefOpenGL == 0)
        {
            cl_gBundleRefOpenGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengles"));
            if (cl_gBundleRefOpenGL == 0)
                throw Exception("Unable to find opengl bundle");
        }
    }

	return (ProcAddress *) CFBundleGetFunctionPointerForName(
		cl_gBundleRefOpenGL,
		CFStringCreateWithCStringNoCopy(
			0,
			function_name.c_str(),
			CFStringGetSystemEncoding(),
			0));

}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowProvider Implementation:

}

