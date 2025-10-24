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

#include "API/Display/surface.h"
#include "API/Display/font.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUIStyleSilver/radiobutton_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_RadioButton_Silver::CL_RadioButton_Silver(
	CL_RadioButton *radiobutton,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(radiobutton),
	radiobutton(radiobutton),
	style(style)
{
	resources = style->get_resources();

	sur_checked = new CL_Surface("RadioButton/surface_checked", resources);
	sur_unchecked = new CL_Surface("RadioButton/surface_unchecked", resources);
	sur_checked_disabled = new CL_Surface("RadioButton/surface_checked_disabled", resources);
	sur_unchecked_disabled = new CL_Surface("RadioButton/surface_unchecked_disabled", resources);

	font = new CL_Font("RadioButton/font", resources);
	font->set_color( CL_Color::black );

	slot_paint = radiobutton->sig_paint().connect_virtual(
		this, &CL_RadioButton_Silver::on_paint);

	slot_get_preferred_size = radiobutton->sig_get_preferred_size().connect(
		this, &CL_RadioButton_Silver::on_get_preferred_size);
}

CL_RadioButton_Silver::~CL_RadioButton_Silver()
{
	delete font;
	delete sur_checked;
	delete sur_unchecked;
	delete sur_checked_disabled;
	delete sur_unchecked_disabled;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_RadioButton_Silver::on_get_preferred_size(CL_Size &size)
{
	CL_Surface *s = sur_checked;
	size.width = s->get_width() + font->get_width(radiobutton->get_text()) + 8;
	size.height = font->get_height() + 4;
	if(size.height < (int)s->get_height())
		size.height = s->get_height();
}

void CL_RadioButton_Silver::on_paint(CL_SlotParent_v0 &super)
{
	CL_Surface *show_surface;

	if (radiobutton->is_enabled())
	{
		if (radiobutton->is_checked())
			show_surface = sur_checked;
		else
			show_surface = sur_unchecked;
	}
	else
	{
		if (radiobutton->is_checked())
			show_surface = sur_checked_disabled;
		else
			show_surface = sur_checked_disabled;
	}

	int xpos = radiobutton->get_screen_x();
	int ypos = radiobutton->get_screen_y();

	show_surface->draw(xpos, ypos);

	if (radiobutton->get_text().length() > 0)
	{
		int text_height = font->get_height();

		font->draw(
			xpos + 4 + show_surface->get_width(),
			ypos + (show_surface->get_height() - text_height) / 2,
			radiobutton->get_text());
	}
}
