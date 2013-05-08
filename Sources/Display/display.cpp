/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/display.h"
#include "API/Display/display_target.h"
#include "display_target_impl.h"
#include "API/Display/Window/display_window.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Display Implementation:

/////////////////////////////////////////////////////////////////////////////
// Display Attributes:

DisplayTarget Display::get_current_target()
{
	return DisplayTarget_Impl::get_current_target();
}

/////////////////////////////////////////////////////////////////////////////
// Display Operations:

void Display::set_current_target(const DisplayTarget &target)
{
	DisplayTarget_Impl::set_current_target(target);
}

void Display::remove_target(const DisplayTarget &target)
{
	DisplayTarget_Impl::remove_target(target);
}

}
