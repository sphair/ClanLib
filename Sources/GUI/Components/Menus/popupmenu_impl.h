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
**    Harry Storbacka
**    Magnus Norddahl
*/

#pragma once

#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Image/pixel_buffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenu_Impl Class:

class CL_PopupMenu_Impl
{
public:
	CL_PopupMenu_Impl() : next_id(0), minimum_width(-1), maximum_height(-1), joiner_width(0) { }

	std::vector<CL_PopupMenuItem> items;
	int next_id;
	int minimum_width;
	int maximum_height;
	CL_String class_name;
	int joiner_width;
	CL_Callback_v0 func_close;
};
