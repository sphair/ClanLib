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
#include "opengl_depth_stencil_state.h"
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"

namespace clan
{
	OpenGLDepthStencilState::OpenGLDepthStencilState()
		: changed_desc(true)
	{
	}

	void OpenGLDepthStencilState::set(const DepthStencilStateDescription &new_state)
	{
		if (!(new_state == desc))
		{
			desc = new_state.clone();
			changed_desc = true;
		}
	}
	void OpenGLDepthStencilState::set(const OpenGLDepthStencilState &new_state)
	{
		set(new_state.desc);
	}

	void OpenGLDepthStencilState::apply()
	{
		if (changed_desc)
		{
			changed_desc = false;

			if (glStencilFuncSeparate)
			{
				CompareFunction front; int front_ref; int front_mask;
				CompareFunction back; int back_ref; int back_mask;
				unsigned char front_facing_mask, back_facing_mask;

				StencilOp fail_front, pass_depth_fail_front, pass_depth_pass_front;
				StencilOp fail_back, pass_depth_fail_back, pass_depth_pass_back;
				desc.get_stencil_compare_front(front, front_ref, front_mask);
				desc.get_stencil_compare_back(back, back_ref, back_mask);
				desc.get_stencil_write_mask(front_facing_mask, back_facing_mask);
				desc.get_stencil_op_front(fail_front, pass_depth_fail_front, pass_depth_pass_front);
				desc.get_stencil_op_back(fail_back, pass_depth_fail_back, pass_depth_pass_back);

				desc.is_stencil_test_enabled() ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);

				glStencilFuncSeparate(GL_FRONT, OpenGL::to_enum(front), front_ref, front_mask);
				glStencilFuncSeparate(GL_BACK, OpenGL::to_enum(back), back_ref, back_mask);
				glStencilMaskSeparate(GL_FRONT, front_facing_mask);
				glStencilMaskSeparate(GL_BACK, back_facing_mask);

				glStencilOpSeparate(GL_FRONT, OpenGL::to_enum(fail_front), OpenGL::to_enum(pass_depth_fail_front), OpenGL::to_enum(pass_depth_pass_front));
				glStencilOpSeparate(GL_BACK, OpenGL::to_enum(fail_back), OpenGL::to_enum(pass_depth_fail_back), OpenGL::to_enum(pass_depth_pass_back));
			}
			desc.is_depth_test_enabled() ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			glDepthMask(desc.is_depth_write_enabled() ? 1 : 0);
			glDepthFunc(OpenGL::to_enum(desc.get_depth_compare_function()));

		}
	}
}
