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
#include "opengl_rasterizer_state.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"

namespace clan
{

OpenGLRasterizerState::OpenGLRasterizerState() 
	: changed_desc(true)
{
}

void OpenGLRasterizerState::set(const RasterizerStateDescription &new_state)
{
	if (!(new_state == desc))
	{
		desc = new_state.clone();
		changed_desc = true;
	}
}
void OpenGLRasterizerState::set(const OpenGLRasterizerState &new_state)
{
	set(new_state.desc);
}

void OpenGLRasterizerState::apply()
{
	if (changed_desc)
	{
		changed_desc = false;

		desc.get_culled() ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		desc.get_enable_line_antialiasing() ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);
		switch (desc.get_face_cull_mode())
		{
			case cull_front:
				glCullFace(GL_FRONT);
				break;
			case cull_back:
				glCullFace(GL_BACK);
				break;
			case cull_front_and_back:
				glCullFace(GL_FRONT_AND_BACK);
				break;
		}
		if (glPolygonMode)
	        glPolygonMode(GL_FRONT_AND_BACK, OpenGL::to_enum(desc.get_face_fill_mode()));

		desc.get_front_face() == face_counter_clockwise ? glFrontFace(GL_CCW) : glFrontFace(GL_CW);

		// Note, enabled in GraphicContextProvider::set_scissor()
		if (!desc.get_enable_scissor())
			glDisable(GL_SCISSOR_TEST);
	}

}

}
