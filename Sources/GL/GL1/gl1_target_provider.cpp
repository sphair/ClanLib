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
*/

#include "LegacyGL/precomp.h"
#include "gl1_target_provider.h"
#ifdef WIN32
#include "WGL/gl1_window_provider_wgl.h"
#else
#include "GLX/gl1_window_provider_glx.h"
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// LegacyGLTargetProvider Construction:

LegacyGLTargetProvider::LegacyGLTargetProvider()
{
}

LegacyGLTargetProvider::~LegacyGLTargetProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// LegacyGLTargetProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// LegacyGLTargetProvider Operations:

DisplayWindowProvider *LegacyGLTargetProvider::alloc_display_window()
{
#ifdef WIN32
	return new GL1WindowProvider_WGL();
#else
	return new GL1WindowProvider_GLX();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// LegacyGLTargetProvider Implementation:

}
