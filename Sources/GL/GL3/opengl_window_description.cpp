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
**    Mark Page
*/

#include "GL/precomp.h"
#include "API/GL/opengl_window_description.h"
#include "opengl_window_description_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription construction:

OpenGLWindowDescription::OpenGLWindowDescription() : impl(new OpenGLWindowDescription_Impl)
{
}

OpenGLWindowDescription::~OpenGLWindowDescription()
{
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription attributes:

int OpenGLWindowDescription::get_version_major() const
{
	return impl->version_major;
}

int OpenGLWindowDescription::get_version_minor() const
{
	return impl->version_minor;
}

bool OpenGLWindowDescription::get_allow_lower_versions() const
{
	return impl->allow_lower_versions;
}

bool OpenGLWindowDescription::get_debug() const
{
	return impl->debug_flag;
}
bool OpenGLWindowDescription::get_forward_compatible() const
{
	return impl->forward_compatible_flag;
}
bool OpenGLWindowDescription::get_core_profile() const
{
	return impl->core_profile_flag;
}
bool OpenGLWindowDescription::get_compatibility_profile() const
{
	return impl->compatibility_profile_flag;
}

int OpenGLWindowDescription::get_layer_plane() const
{
	return impl->layer_plane;
}


/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription operations:

void OpenGLWindowDescription::set_version(int major, int minor, bool allow_lower_versions)
{
	impl->version_major = major;
	impl->version_minor = minor;
	impl->allow_lower_versions = allow_lower_versions;
	
}

void OpenGLWindowDescription::set_debug(bool enable)
{
	impl->debug_flag = enable;
}
void OpenGLWindowDescription::set_forward_compatible(bool enable)
{
	impl->forward_compatible_flag = enable;
}
void OpenGLWindowDescription::set_core_profile(bool enable)
{
	impl->core_profile_flag = enable;
}
void OpenGLWindowDescription::set_compatibility_profile(bool enable)
{
	impl->compatibility_profile_flag = enable;
}

void OpenGLWindowDescription::set_layer_plane(int value)
{
	impl->layer_plane = value;
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription implementation:

}
