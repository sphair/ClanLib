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
*/

#pragma once

#include "API/Display/Window/display_window_description.h"

namespace clan
{
	class OpenGLContextDescription_Impl
	{
	public:
		OpenGLContextDescription_Impl()
		{
			version_major = 4;
			version_minor = 3;
			allow_lower_versions = false;
			debug_flag = false;
			forward_compatible_flag = false;
			core_profile_flag = true;
			compatibility_profile_flag = false;
			layer_plane = 0;

		}

		int version_major;
		int version_minor;
		bool allow_lower_versions;

		bool debug_flag;
		bool forward_compatible_flag;
		bool core_profile_flag;
		bool compatibility_profile_flag;
		int layer_plane;
	};
}
