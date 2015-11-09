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
**    Mark Page
*/

#pragma once

#include "API/GL/opengl.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/signal.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/rasterizer_state_description.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Render/depth_stencil_state_description.h"
#include "API/Core/System/disposable_object.h"
#include <map>

namespace clan
{
	class GL3StandardPrograms_Impl;
	class GL3GraphicContextProvider;

	class GL3StandardPrograms
	{
	public:
		GL3StandardPrograms();
		GL3StandardPrograms(GL3GraphicContextProvider *provider);
		~GL3StandardPrograms();

		ProgramObject get_program_object(StandardProgram standard_program) const;

	private:
		std::shared_ptr<GL3StandardPrograms_Impl> impl;
	};
}
