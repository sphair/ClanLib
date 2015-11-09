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
**    Harry Storbacka
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/display_target_provider.h"
#include "API/GL/opengl_context_description.h"

namespace clan
{
	class OpenGLTargetProvider : public DisplayTargetProvider
	{
	public:
		OpenGLTargetProvider();
		~OpenGLTargetProvider();

		OpenGLContextDescription get_description() { return description; }
		DisplayWindowProvider *alloc_display_window() override;
		void set_description(OpenGLContextDescription &desc) { description = desc; }

	private:
#ifdef WIN32
		friend class OpenGLWindowProvider;
#else
		friend class OpenGLWindowProvider;
#endif
		OpenGLContextDescription description;
	};
}
