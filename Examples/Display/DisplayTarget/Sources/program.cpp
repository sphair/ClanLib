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
**    Mark Page
*/

#include "precomp.h"
#include "program.h"
#include "target.h"

clan::ApplicationInstance<Program> clanapp;

Program::Program()
{
	// We support all display targets, in order listed here
	clan::D3DTarget::set_current();
	clan::OpenGLTarget::set_current();

	target = clan::make_unique<Target>(Target::RenderTarget::legacy_gl);
}

bool Program::update()
{
	Target::RenderTarget rt = target->render_target;
	target->run_demo();
	if (target->quit)
		return false;

	if (target->render_target != rt)
	{
		rt = target->render_target;
		target.reset();	// This is important. Multiply Display targets cannot co-exist
		target = clan::make_unique<Target>(rt);
	}
	return true;
}
