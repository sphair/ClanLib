/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#include "GL/precomp.h"
#include "opengl_target_provider.h"
#if defined(__APPLE__)
#include "GL3/AGL/gl3_window_provider_agl.h"
#elif defined(WIN32)
#include "GL3/WGL/gl3_window_provider_wgl.h"
#include "GL1/WGL/gl1_window_provider_wgl.h"
#else
#include "GL3/GLX/gl3_window_provider_glx.h"
namespace clan { DisplayWindowProvider *newGL1WindowProvider_GLX(); } // #include "GL1/GLX/gl1_window_provider_glx.h"
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLTargetProvider Construction:

OpenGLTargetProvider::OpenGLTargetProvider()
{
}

OpenGLTargetProvider::~OpenGLTargetProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLTargetProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// OpenGLTargetProvider Operations:

DisplayWindowProvider *OpenGLTargetProvider::alloc_display_window()
{
#if defined(__APPLE__)
	// description not supported on AGL at the moment
	return cl_alloc_display_window_agl();//new OpenGLWindowProvider_AGL;

#else

	int version_major = description.get_version_major();
	int version_minor = description.get_version_minor();

	// Do not attempt GL3, if not requested that version
	if ((version_major < 3) || ((version_major == 3) && (version_minor < 2)))
	{
		#if defined(WIN32)
			return new GL1WindowProvider_WGL();
		#else
			return newGL1WindowProvider_GLX();
		#endif
	}

	// If we do not allow lower versions, only attempt GL3
	if (!description.get_allow_lower_versions())
	{
		#if defined(WIN32)
			return new OpenGLWindowProvider_WGL(description);
		#else
			return new OpenGLWindowProvider_GLX(description);
		#endif
	}

	try
	{
		// Attempt GL3 first
		#if defined(WIN32)
			return new OpenGLWindowProvider_WGL(description);
		#else
			return new OpenGLWindowProvider_GLX(description);
		#endif
	}
	catch (...)
	{
		// Then attempt GL1
		#if defined(WIN32)
			return new GL1WindowProvider_WGL();
		#else
			return newGL1WindowProvider_GLX();
		#endif

	}

#endif
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLTargetProvider Implementation:

}
