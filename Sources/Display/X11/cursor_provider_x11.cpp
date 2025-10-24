/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "cursor_provider_x11.h"
#include "API/Core/System/databuffer.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/sprite_description.h"
#include "x11_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_X11 Construction:

CL_CursorProvider_X11::CL_CursorProvider_X11(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
: handle(0)
{
	handle = create_cursor(sprite_description, hotspot);
}

CL_CursorProvider_X11::~CL_CursorProvider_X11()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_X11 Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_X11 Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_CursorProvider_X11 Implementation:

int CL_CursorProvider_X11::create_cursor(const CL_SpriteDescription &sprite_description, const CL_Point &hotspot) const
{
	// TODO: Implement me :)
	return 0;
}


