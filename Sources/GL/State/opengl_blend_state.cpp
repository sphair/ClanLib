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

#include "GL/precomp.h"
#include "opengl_blend_state.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"

namespace clan
{

OpenGLBlendState::OpenGLBlendState() 
	: blend_color(Colorf::white), changed_desc(true), changed_blend_color(true)
{
}

void OpenGLBlendState::set(const BlendStateDescription &new_state, const Vec4f &new_blend_color)
{
	if (!(new_state == desc))
	{
		desc = new_state.clone();
		changed_desc = true;
	}

	if (new_blend_color != blend_color)
	{
		blend_color = new_blend_color;
		changed_blend_color = true;
	}
}

void OpenGLBlendState::set(const OpenGLBlendState &new_state)
{
	set(new_state.desc, new_state.blend_color);
}


void OpenGLBlendState::apply()
{
	if (changed_desc)
	{
		changed_desc = false;

		BlendEquation equation_color, equation_alpha;
		BlendFunc src, dest, src_alpha, dest_alpha;
		desc.get_blend_equation(equation_color, equation_alpha);
		desc.get_blend_function(src, dest, src_alpha, dest_alpha);

		desc.is_blending_enabled() ? glEnable(GL_BLEND) : glDisable(GL_BLEND);

		if (glColorMask)
		{
			bool red, green, blue, alpha;
			desc.get_color_write(red, green, blue, alpha);
			glColorMask(red,green,blue,alpha);
		}


		if (equation_color == equation_alpha)
		{
			if (glBlendEquation)
				glBlendEquation(OpenGL::to_enum(equation_color));
		}
		else
		{
			if (glBlendEquationSeparate)
			{
				glBlendEquationSeparate( OpenGL::to_enum(equation_color), OpenGL::to_enum(equation_alpha) );
			}
			else
			{
				if (glBlendEquation)
					glBlendEquation(OpenGL::to_enum(equation_color));
			}
		}

		if( src == src_alpha && dest == dest_alpha )
		{
			if (glBlendFunc)
				glBlendFunc(OpenGL::to_enum(src), OpenGL::to_enum(dest));
		}
		else
		{
			if (glBlendFuncSeparate)
			{
				glBlendFuncSeparate( OpenGL::to_enum(src), OpenGL::to_enum(dest), OpenGL::to_enum(src_alpha), OpenGL::to_enum(dest_alpha) );
			}
			else
			{
				if (glBlendFunc)
					glBlendFunc(OpenGL::to_enum(src), OpenGL::to_enum(dest));
			}
		}

		desc.is_logic_op_enabled() ? glEnable(GL_COLOR_LOGIC_OP) : glDisable(GL_COLOR_LOGIC_OP);

		if (glLogicOp)
			glLogicOp(OpenGL::to_enum(desc.get_logic_op()));

	}

	if (changed_blend_color)
	{
		changed_blend_color = false;
		if (glBlendColor)
				glBlendColor(blend_color.r, blend_color.g, blend_color.b, blend_color.a);
	}

}

}
