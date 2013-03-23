/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "SWRender/precomp.h"
#include "API/SWRender/swr_graphic_context.h"
#include "API/SWRender/swr_program_object.h"
#include "swr_program_object_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ProgramObject_SWRender Construction:

ProgramObject_SWRender::ProgramObject_SWRender(SoftwareProgram *program, bool is_sprite_program) : ProgramObject( new SWRenderProgramObjectProvider )
{
	SWRenderProgramObjectProvider *provider = get_provider();
	provider->set_program(program);
	provider->set_sprite_program(is_sprite_program);
}

ProgramObject_SWRender::ProgramObject_SWRender(const ProgramObject &program_object) : ProgramObject(program_object)
{
	if (!get_provider())
		throw Exception("Program Object is not from the SWRender target");

}

ProgramObject_SWRender::~ProgramObject_SWRender()
{
}

/////////////////////////////////////////////////////////////////////////////
// ProgramObject_SWRender Attributes:

SoftwareProgram *ProgramObject_SWRender::get_program() const
{
	return get_provider()->get_program();
}

	/// \brief Returns true if this program object was created as a sprite program
bool ProgramObject_SWRender::is_sprite_program() const
{
	return get_provider()->is_sprite_program();
}

SWRenderProgramObjectProvider *ProgramObject_SWRender::get_provider() const
{
	return static_cast <SWRenderProgramObjectProvider *> (ProgramObject::get_provider());
}

/////////////////////////////////////////////////////////////////////////////
// ProgramObject_SWRender Operations:

/////////////////////////////////////////////////////////////////////////////
// ProgramObject_SWRender Implementation:

}
