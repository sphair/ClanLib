/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "Platform/OSX/opengl_window_provider_osx.h"
#elif defined(WIN32)
#include "Platform/WGL/opengl_window_provider_wgl.h"
#elif defined(__ANDROID__)
#include "Platform/Android/opengl_window_provider_android.h"
#else
#include "Platform/GLX/opengl_window_provider_glx.h"
namespace clan { DisplayWindowProvider *newOpenGLWindowProvider(); }
#endif

namespace clan
{
	OpenGLTargetProvider::OpenGLTargetProvider()
	{
	}

	OpenGLTargetProvider::~OpenGLTargetProvider()
	{
	}

	std::unique_ptr<DisplayWindowProvider> OpenGLTargetProvider::alloc_display_window()
	{
		return std::make_unique<OpenGLWindowProvider>(description);
	}
}
