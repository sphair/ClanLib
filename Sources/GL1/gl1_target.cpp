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

#include "GL1/precomp.h"
#include "API/GL1/opengl1_target.h"
#include "gl1_target_provider.h"
#include "setup_gl1_impl.h"
#include "API/Display/display.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GL1Target Construction:

GL1Target::GL1Target()
: DisplayTarget(new GL1TargetProvider)
{
}

GL1Target::~GL1Target()
{
}

/////////////////////////////////////////////////////////////////////////////
// GL1Target Attributes:

/////////////////////////////////////////////////////////////////////////////
// GL1Target Operations:

void GL1Target::set_current()
{
	MutexSection mutex_lock(&SetupGL1_Impl::gl1_mutex);
	if (!SetupGL1_Impl::gl1_target)
		throw Exception("clanGL1 has not been initialised");
	SetupGL1_Impl::gl1_target->DisplayTarget::set_current();
}

bool GL1Target::is_current()
{
	DisplayTarget target = Display::get_current_target();
	DisplayTargetProvider *ptr = target.get_provider();
	if (!ptr)
		return false;

	GL1TargetProvider *provider = dynamic_cast<GL1TargetProvider*>(ptr);
	return (provider != nullptr);
}

/////////////////////////////////////////////////////////////////////////////
// GL1Target Implementation:

}
