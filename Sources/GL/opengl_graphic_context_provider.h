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
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/GL/opengl.h"

namespace clan
{
	class OpenGLGraphicContextProvider
	{
	public:
		virtual void get_opengl_version(int &version_major, int &version_minor) const = 0;
		virtual void get_opengl_version(int &version_major, int &version_minor, int &version_release) const = 0;
		virtual void get_opengl_shading_language_version(int &version_major, int &version_minor) const { version_major = 0; version_minor = 0; }
		virtual void make_current() const = 0;
		virtual ProcAddress *get_proc_address(const std::string& function_name) const = 0;
	};
}
