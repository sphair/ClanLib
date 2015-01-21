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
**    Mark Page
*/

#include "GL/precomp.h"
#include "API/Display/display.h"
#include "API/GL/opengl_window_description.h"
#include "API/GL/opengl_target.h"
#include "opengl_target_provider.h"
#include "setup_gl_impl.h"
#include "setup_gl.h"
namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLTarget Construction:

OpenGLTarget::OpenGLTarget()
: DisplayTarget(new OpenGLTargetProvider)
{
}

OpenGLTarget::~OpenGLTarget()
{
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLTarget Attributes:

bool OpenGLTarget::is_current()
{
	DisplayTarget target = Display::get_current_target();
	DisplayTargetProvider *ptr = target.get_provider();
	if (!ptr)
		return false;

	OpenGLTargetProvider *provider = dynamic_cast<OpenGLTargetProvider*>(ptr);
	return (provider != nullptr);

}

/////////////////////////////////////////////////////////////////////////////
// OpenGLTarget Operations:

void OpenGLTarget::enable()
{
	SetupGL::start();
}

OpenGLWindowDescription OpenGLTarget::get_description()
{
	SetupGL::start();

	if (SetupGL_Impl::instance)
	{
		OpenGLTargetProvider *provider = dynamic_cast<OpenGLTargetProvider*>(SetupGL_Impl::instance->cl_opengl_target->get_provider());
		if (provider)
		{
			return provider->get_description();
		}
	}
	return OpenGLWindowDescription();
}

void OpenGLTarget::set_description(OpenGLWindowDescription &desc)
{
	SetupGL::start();
	if (SetupGL_Impl::instance)
	{
		OpenGLTargetProvider *provider = dynamic_cast<OpenGLTargetProvider*>(SetupGL_Impl::instance->cl_opengl_target->get_provider());
		if (provider)
		{
			provider->set_description(desc);
		}
	}
}

void OpenGLTarget::set_current()
{
	SetupGL::start();
	if (!SetupGL_Impl::instance)
		throw Exception("clanGL has not been initialised");
	SetupGL_Impl::instance->cl_opengl_target->DisplayTarget::set_current();
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLTarget Implementation:

}
