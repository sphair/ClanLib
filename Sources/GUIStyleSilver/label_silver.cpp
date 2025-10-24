/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "API/Display/font.h"
#include "API/Display/display.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/component_style.h"
#include "API/GUIStyleSilver/label_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Label_Silver::CL_Label_Silver(
	CL_Label *label,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(label),
	label(label)
{
	resources = style->get_resources();
	
	font = new CL_Font("Label/font", resources);
	font->set_color( CL_Color::black );
	
	slot_paint = label->sig_paint().connect(
		this, &CL_Label_Silver::on_paint);
	slot_get_preferred_size = label->sig_get_preferred_size().connect(
		this, &CL_Label_Silver::on_get_preferred_size);
	slot_font_change = label->sig_font_change().connect(
		this, &CL_Label_Silver::on_font_change);
}

CL_Label_Silver::~CL_Label_Silver()
{
	delete font;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Label_Silver::on_get_preferred_size(CL_Size &size)
{
	size.width = font->get_width(label->get_text());
	size.height = font->get_height(label->get_text());
}

void CL_Label_Silver::on_paint()
{
	const char *text = label->get_text().c_str();

	CL_Point pos = calc_origin(label->get_alignment(), CL_Size(label->get_width() / 2, label->get_height()));
	
	pos.x += label->get_screen_x();
	pos.y += label->get_screen_y();
	
	font->draw(
		CL_Rect(
			pos.x,
			pos.y,
			pos.x + label->get_width(),
			pos.y + label->get_height()),
		text);
}

void CL_Label_Silver::on_font_change()
{
	delete font;

	if (label->get_font() == NULL)
	{
		font = new CL_Font("Label/font", resources);
	}
	else
	{
		font = new CL_Font(*(label->get_font()));
	}
}
