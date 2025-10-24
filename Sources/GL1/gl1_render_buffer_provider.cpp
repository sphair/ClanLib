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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "GL1/precomp.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1.h"
#include "gl1_render_buffer_provider.h"


CL_GL1RenderBufferProvider::CL_GL1RenderBufferProvider(CL_GL1GraphicContextProvider *gc_provider)
: gc_provider(gc_provider)
{
	throw CL_Exception("CL_GL1RenderBuffer is not supported");
}

CL_GL1RenderBufferProvider::~CL_GL1RenderBufferProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1RenderBufferProvider Attributes:

CLuint CL_GL1RenderBufferProvider::get_handle()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1RenderBufferProvider Operations:

void CL_GL1RenderBufferProvider::create(int width, int height, CL_TextureFormat internal_format, int multisample_samples)
{
}

void CL_GL1RenderBufferProvider::destroy()
{
	delete this;
}

