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
**    Sean Heber
*/

#include "GL1/precomp.h"
#include "gl1_window_provider_agl.h"
#include "API/Core/Math/rect.h"
#include "API/Display/display_window_description.h"
#include "API/Display/input_event.h"
#include "API/Display/TargetProviders/render_window_provider.h"
#include "API/GL1/opengl1.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1_state.h"
#include "API/Core/Text/logger.h"
#include "../gl1_window_description_impl.h"
#include "../gl1_graphic_context_provider.h"
#include "input_device_provider_mackeyboard.h"
#include "input_device_provider_macmouse.h"

namespace
{
	class RenderWindowProvider_AGL: public CL_RenderWindowProvider
	{
	public:
		RenderWindowProvider_AGL(CL_GL1WindowProvider_AGL & window, AGLContext agl_context = AGL_NONE)
			: window(window), agl_context(agl_context)
		{
		}
		virtual ~RenderWindowProvider_AGL()
		{
			if( agl_context )
			{
				aglDestroyContext(agl_context);
			}
		}
		virtual int get_viewport_width() const
		{
			return window.get_viewport().get_width();
		}
		virtual int get_viewport_height() const
		{
			return window.get_viewport().get_height();
		}
		virtual void flip_buffers(int interval) const
		{
			window.flip(interval);
		}
		virtual void make_current() const
		{
			aglMakeCurrent(window.get_window(), window.get_opengl_context());
		}
		virtual const CL_RenderWindowProvider * new_worker_context() const
		{
			return new RenderWindowProvider_AGL(window, window.create_context());
		}
	private:
		CL_GL1WindowProvider_AGL & window;
		AGLContext agl_context;
	};
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_AGL Construction:

CL_GL1WindowProvider_AGL::CL_GL1WindowProvider_AGL()
: keyboard(NULL), mouse(NULL), site(NULL)
{
	keyboard = new CL_InputDeviceProvider_MacKeyboard(this);
	mouse = new CL_InputDeviceProvider_MacMouse(this);
	gc = CL_GraphicContext(new CL_GL1GraphicContextProvider(this));
}

CL_GL1WindowProvider_AGL::~CL_GL1WindowProvider_AGL()
{
	delete keyboard;
	delete mouse;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_AGL Attributes:

//std::list<HGLRC> CL_GL1WindowProvider_WGL::opengl_contexts;

//std::map<HWND, CL_GL1WindowProvider_WGL *> CL_GL1WindowProvider_WGL::window_map;

CL_Rect CL_GL1WindowProvider_AGL::get_geometry() const
{
	return CL_Rect();
}

CL_Rect CL_GL1WindowProvider_AGL::get_viewport() const
{
	return CL_Rect();
}

bool CL_GL1WindowProvider_AGL::is_fullscreen() const
{
	return false;
}

bool CL_GL1WindowProvider_AGL::has_focus() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_AGL Operations:

void CL_GL1WindowProvider_AGL::create(CL_DisplayWindowSite *new_site, const CL_DisplayWindowDescription &desc)
{
	site = new_site;
}

void CL_GL1WindowProvider_AGL::show_system_cursor()
{
}

void CL_GL1WindowProvider_AGL::hide_system_cursor()
{
}

void CL_GL1WindowProvider_AGL::set_title(const CL_StringRef &new_title)
{
}

void CL_GL1WindowProvider_AGL::set_position(const CL_Rect &pos)
{
}

void CL_GL1WindowProvider_AGL::set_size(int width, int height)
{
}

void CL_GL1WindowProvider_AGL::flip(int interval)
{
}

void CL_GL1WindowProvider_AGL::update(const CL_Rect &rect)
{
}

void CL_GL1WindowProvider_AGL::process_messages()
{
	get_ic().process_messages();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1WindowProvider_AGL Implementation:

