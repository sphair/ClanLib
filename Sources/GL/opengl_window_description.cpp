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

#include "GL/precomp.h"
#include "API/GL/opengl_window_description.h"
#include "opengl_window_description_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription construction:

OpenGLWindowDescription::OpenGLWindowDescription()
{
	impl_gl = std::dynamic_pointer_cast<OpenGLWindowDescription_Impl>(get_data("OpenGL"));
	if (!impl_gl)
	{
		impl_gl = std::shared_ptr<OpenGLWindowDescription_Impl>(new OpenGLWindowDescription_Impl);
		set_data("OpenGL", impl_gl);
	}
}

OpenGLWindowDescription::~OpenGLWindowDescription()
{
}

OpenGLWindowDescription::OpenGLWindowDescription(const DisplayWindowDescription &desc)
{
	DisplayWindowDescription::operator=(desc);
	impl_gl = std::dynamic_pointer_cast<OpenGLWindowDescription_Impl>(get_data("OpenGL"));
	if (!impl_gl)
	{
		impl_gl = std::shared_ptr<OpenGLWindowDescription_Impl>(new OpenGLWindowDescription_Impl);
		set_data("OpenGL", impl_gl);
	}
}

OpenGLWindowDescription &OpenGLWindowDescription::operator=(DisplayWindowDescription &desc)
{
	DisplayWindowDescription::operator=(desc);
	impl_gl = std::dynamic_pointer_cast<OpenGLWindowDescription_Impl>(get_data("OpenGL"));
	if (!impl_gl)
	{
		impl_gl = std::shared_ptr<OpenGLWindowDescription_Impl>(new OpenGLWindowDescription_Impl);
		set_data("OpenGL", impl_gl);
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription attributes:

int OpenGLWindowDescription::get_buffer_size() const
{
	return impl_gl->buffer_size;
}

bool OpenGLWindowDescription::get_doublebuffer() const
{
	return impl_gl->doublebuffer;
}

bool OpenGLWindowDescription::get_stereo() const
{
	return impl_gl->stereo;
}

int OpenGLWindowDescription::get_red_size() const
{
	return impl_gl->red_size;
}

int OpenGLWindowDescription::get_green_size() const
{
	return impl_gl->green_size;
}

int OpenGLWindowDescription::get_blue_size() const
{
	return impl_gl->blue_size;
}

int OpenGLWindowDescription::get_alpha_size() const
{
	return impl_gl->alpha_size;
}

int OpenGLWindowDescription::get_multisampling() const
{
	return impl_gl->multisampling;
}

int OpenGLWindowDescription::get_version_major() const
{
	return impl_gl->version_major;
}

int OpenGLWindowDescription::get_version_minor() const
{
	return impl_gl->version_minor;
}

bool OpenGLWindowDescription::get_allow_lower_versions() const
{
	return impl_gl->allow_lower_versions;
}

bool OpenGLWindowDescription::get_debug() const
{
	return impl_gl->debug_flag;
}
bool OpenGLWindowDescription::get_forward_compatible() const
{
	return impl_gl->forward_compatible_flag;
}
bool OpenGLWindowDescription::get_core_profile() const
{
	return impl_gl->core_profile_flag;
}
bool OpenGLWindowDescription::get_compatibility_profile() const
{
	return impl_gl->compatibility_profile_flag;
}

int OpenGLWindowDescription::get_layer_plane() const
{
	return impl_gl->layer_plane;
}


/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription operations:

void OpenGLWindowDescription::set_doublebuffer(bool value)
{
	impl_gl->doublebuffer = value;
}

void OpenGLWindowDescription::set_stereo(bool value)
{
	impl_gl->stereo = value;
}

void OpenGLWindowDescription::set_buffer_size(int value)
{
	impl_gl->buffer_size = value;
}

void OpenGLWindowDescription::set_red_size(int value)
{
	impl_gl->red_size = value;
}

void OpenGLWindowDescription::set_green_size(int value)
{
	impl_gl->green_size = value;
}

void OpenGLWindowDescription::set_blue_size(int value)
{
	impl_gl->blue_size = value;
}

void OpenGLWindowDescription::set_alpha_size(int value)
{
	impl_gl->alpha_size = value;
}

void OpenGLWindowDescription::set_multisampling(int value)
{
	impl_gl->multisampling = value;
}

void OpenGLWindowDescription::set_version(int major, int minor, bool allow_lower_versions)
{
	impl_gl->version_major = major;
	impl_gl->version_minor = minor;
	impl_gl->allow_lower_versions = allow_lower_versions;
	
}

void OpenGLWindowDescription::set_debug(bool enable)
{
	impl_gl->debug_flag = enable;
}
void OpenGLWindowDescription::set_forward_compatible(bool enable)
{
	impl_gl->forward_compatible_flag = enable;
}
void OpenGLWindowDescription::set_core_profile(bool enable)
{
	impl_gl->core_profile_flag = enable;
}
void OpenGLWindowDescription::set_compatibility_profile(bool enable)
{
	impl_gl->compatibility_profile_flag = enable;
}

void OpenGLWindowDescription::set_layer_plane(int value)
{
	impl_gl->layer_plane = value;
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLWindowDescription implementation:

}
