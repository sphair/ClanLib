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
**    Harry Storbacka
**    Magnus Norddahl
*/

#pragma once

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_theme_part.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

class CL_ListViewColumnHeader_Impl
{
public:
	CL_ListViewColumnHeader_Impl()
	: align(CL_ListViewColumnHeader::align_left), width(100)
	{
	}

	CL_String column_id;

	CL_String caption;

	CL_ListViewColumnHeader::TextAlignment align;

	CL_Rect rect;

	CL_GUIThemePart part;

	CL_SharedPtr<CL_ListViewColumnHeader_Impl> next_sibling;

	CL_WeakPtr<CL_ListViewColumnHeader_Impl> prev_sibling;

	CL_Callback_v1<CL_ListViewColumnHeader> func_size_changed;

	int width;
};



