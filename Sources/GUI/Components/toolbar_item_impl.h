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

#ifndef _header_toolbar_item_impl_
#define _header_toolbar_item_impl_

#include "API/Display/2D/sprite.h"

class CL_ToolBarItem_Impl
{
public:
	CL_ToolBarItem_Impl() : frame(0), id(-1), disabled(false), toggling(false), pressed(false) { }

	CL_Sprite icon;
	int frame;
	CL_String text;
	CL_Rect position;
	CL_Rect icon_pos;
	CL_Point text_pos;
	int id;
	bool disabled;
	bool toggling;
	bool pressed;
};

#endif
