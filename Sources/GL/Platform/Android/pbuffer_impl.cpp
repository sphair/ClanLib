/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#include "GL/precomp.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "pbuffer_impl.h"
#include "../../GL1/gl1_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"
#include "opengl_window_provider_android.h"

namespace clan
{

	PBuffer_GL1_Impl::PBuffer_GL1_Impl(GL1GraphicContextProvider *gc_provider) : gc_provider(gc_provider)
	{
		if (!gc_provider)
			throw Exception("Unexpected provider");

	}

	PBuffer_GL1_Impl::~PBuffer_GL1_Impl()
	{
		reset();
	}

	void PBuffer_GL1_Impl::reset()
	{
		OpenGL::set_active(gc_provider);

		if (window_provider)
		{
			if (surface != EGL_NO_SURFACE)
			{
				eglDestroySurface(window_provider->get_display(), surface);
				surface = EGL_NO_SURFACE;
			}
		}


		window_provider = nullptr;
		OpenGL::remove_active(this);
	}

	void PBuffer_GL1_Impl::create(OpenGLWindowProvider &gl_window_provider, const Size &size)
	{
		reset();

		window_provider = &gl_window_provider;

		auto display = window_provider->get_display();

		EGLint attribs[] =
		{
			EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};

		EGLint numConfigs;
		EGLConfig config;
		if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs))
			throw Exception("eglChooseConfig failed");
		if (numConfigs < 1)
			throw Exception("Found configs failed");

		EGLint attribList[] =
		{
			EGL_WIDTH, size.width,
			EGL_HEIGHT, size.height,
			EGL_NONE
		};

		surface = eglCreatePbufferSurface(display, config, attribList);
		if (surface == EGL_NO_SURFACE)
			throw Exception("eglCreateWindowSurface failed");


		OpenGL::set_active(gc_provider);
	}

	void PBuffer_GL1_Impl::make_current() const
	{
		if (window_provider && surface != EGL_NO_SURFACE)
			eglMakeCurrent(window_provider->get_display(), surface, surface, window_provider->get_context());
	}

	ProcAddress *PBuffer_GL1_Impl::get_proc_address(const std::string& function_name) const
	{
		return (void(*)())eglGetProcAddress(function_name.c_str());
	}

	void PBuffer_GL1_Impl::get_opengl_version(int &version_major, int &version_minor) const
	{
		gc_provider->get_opengl_version(version_major, version_minor);
	}

	void PBuffer_GL1_Impl::get_opengl_version(int &version_major, int &version_minor, int &version_release) const
	{
		gc_provider->get_opengl_version(version_major, version_minor, version_release);
	}

}

