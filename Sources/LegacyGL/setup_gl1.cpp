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
*/

#include "LegacyGL/precomp.h"
#include "API/LegacyGL/setup_legacy_gl.h"
#include "API/LegacyGL/legacy_gl_target.h"
#include "setup_gl1_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SetupLegacyGL Construction:

Mutex SetupLegacyGL_Impl::gl1_mutex;
int SetupLegacyGL_Impl::gl1_refcount = 0;
LegacyGLTarget *SetupLegacyGL_Impl::gl1_target = 0;

SetupLegacyGL::SetupLegacyGL()
{
	MutexSection mutex_lock(&SetupLegacyGL_Impl::gl1_mutex);
	if (SetupLegacyGL_Impl::gl1_refcount == 0)
		SetupLegacyGL_Impl::gl1_target = new LegacyGLTarget();
	SetupLegacyGL_Impl::gl1_refcount++;
}

SetupLegacyGL::~SetupLegacyGL()
{
	MutexSection mutex_lock(&SetupLegacyGL_Impl::gl1_mutex);
	SetupLegacyGL_Impl::gl1_refcount--;
	if (SetupLegacyGL_Impl::gl1_refcount == 0)
		delete SetupLegacyGL_Impl::gl1_target;
}

}
