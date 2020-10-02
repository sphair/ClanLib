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
**    Mark Page
*/

#include "GL/precomp.h"

#ifdef WIN32
#include "../Platform/WGL/pbuffer_impl.h"
#elif defined(__ANDROID__)
#include "../Platform/Android/pbuffer_impl.h"
#elif __APPLE__
#include "../Platform/AGL/pbuffer_impl.h"
#else
#include "../Platform/GLX/pbuffer_impl.h"
#endif
#include "pbuffer.h"

#include "gl1_graphic_context_provider.h"

namespace clan
{
	PBuffer_GL1::PBuffer_GL1()
	{
	}

	PBuffer_GL1::PBuffer_GL1(GL1GraphicContextProvider *gc_provider) : impl(std::make_shared<PBuffer_GL1_Impl>(gc_provider))
	{
	}

	PBuffer_GL1::~PBuffer_GL1()
	{
	}

	void PBuffer_GL1::create(OpenGLWindowProvider &window_provider, Size &size)
	{
		impl->create(window_provider, size);
		set_active();

		glEnable(GL_POINT_SPRITE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void PBuffer_GL1::set_active()
	{
		OpenGL::set_active(impl.get());
	}

	void PBuffer_GL1::throw_if_null() const
	{
		if (!impl)
			throw Exception("is null");
	}
}
