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
**    Harry Storbacka
**    Magnus Norddahl
*/

#pragma once

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

namespace clan
{

class ListViewColumnHeader_Impl
{
public:
	ListViewColumnHeader_Impl()
	: align(ListViewColumnHeader::align_left), width(100)
	{
	}

	std::string column_id;

	std::string caption;

	ListViewColumnHeader::TextAlignment align;

	Rect rect;

	std::shared_ptr<ListViewColumnHeader_Impl> next_sibling;

	std::weak_ptr<ListViewColumnHeader_Impl> prev_sibling;

	Callback_v1<ListViewColumnHeader> func_size_changed;

	int width;
};

}
