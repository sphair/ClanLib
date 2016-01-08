/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/GL/opengl_context_description.h"
#include "opengl_context_description_impl.h"

namespace clan
{
	OpenGLContextDescription::OpenGLContextDescription() : impl(std::make_shared<OpenGLContextDescription_Impl>())
	{
	}

	OpenGLContextDescription::~OpenGLContextDescription()
	{
	}

	int OpenGLContextDescription::get_version_major() const
	{
		return impl->version_major;
	}

	int OpenGLContextDescription::get_version_minor() const
	{
		return impl->version_minor;
	}

	bool OpenGLContextDescription::get_allow_lower_versions() const
	{
		return impl->allow_lower_versions;
	}

	bool OpenGLContextDescription::get_debug() const
	{
		return impl->debug_flag;
	}
	bool OpenGLContextDescription::get_forward_compatible() const
	{
		return impl->forward_compatible_flag;
	}
	bool OpenGLContextDescription::get_core_profile() const
	{
		return impl->core_profile_flag;
	}
	bool OpenGLContextDescription::get_compatibility_profile() const
	{
		return impl->compatibility_profile_flag;
	}

	int OpenGLContextDescription::get_layer_plane() const
	{
		return impl->layer_plane;
	}

	void OpenGLContextDescription::set_version(int major, int minor, bool allow_lower_versions)
	{
		impl->version_major = major;
		impl->version_minor = minor;
		impl->allow_lower_versions = allow_lower_versions;
	}

	void OpenGLContextDescription::set_debug(bool enable)
	{
		impl->debug_flag = enable;
	}
	void OpenGLContextDescription::set_forward_compatible(bool enable)
	{
		impl->forward_compatible_flag = enable;
	}
	void OpenGLContextDescription::set_core_profile(bool enable)
	{
		impl->core_profile_flag = enable;
	}
	void OpenGLContextDescription::set_compatibility_profile(bool enable)
	{
		impl->compatibility_profile_flag = enable;
	}

	void OpenGLContextDescription::set_layer_plane(int value)
	{
		impl->layer_plane = value;
	}
}
