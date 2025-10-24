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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Window/cursor.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/TargetProviders/cursor_provider.h"
#include "API/Display/TargetProviders/display_window_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Cursor_Impl class:

class CL_Cursor_Impl
{
public:
	CL_Cursor_Impl() : provider(0) { }
	~CL_Cursor_Impl() { if (provider) provider->destroy(); }

	CL_CursorProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Cursor Construction:

CL_Cursor::CL_Cursor()
{
}

CL_Cursor::CL_Cursor(const CL_DisplayWindow &window, const CL_SpriteDescription &sprite_description, const CL_Point &hotspot)
: impl(new CL_Cursor_Impl)
{
	impl->provider = window.get_provider()->create_cursor(sprite_description, hotspot);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Cursor Attributes:

void CL_Cursor::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_Cursor is null");
}

CL_CursorProvider *CL_Cursor::get_provider() const
{
	if (impl)
		return impl->provider;
	else
		return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Cursor Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_Cursor Implementation:
