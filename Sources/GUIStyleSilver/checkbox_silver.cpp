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
#include "API/GUIStyleSilver/checkbox_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_CheckBox_Silver::CL_CheckBox_Silver(
	CL_CheckBox *checkbox,
	CL_StyleManager_Silver *style)
: 
	CL_ComponentStyle(checkbox),
	checkbox(checkbox),
	style(style),
	font(0),
	font_disabled(0),
	delete_font(false),
	delete_font_disabled(false),
	sur_checked(0),
	sur_unchecked(0),
	sur_checked_disabled(0), 
	sur_unchecked_disabled(0),
	delete_sur_checked(false),
	delete_sur_unchecked(false),
	delete_sur_checked_disabled(false),
	delete_sur_unchecked_disabled(false)
{
	resources = style->get_resources();

	if(resources->exists("CheckBox/surface_checked"))
		set_surface_checked (new CL_Surface("CheckBox/surface_checked", resources), true);
	if(resources->exists("CheckBox/surface_unchecked"))
		set_surface_unchecked (new CL_Surface("CheckBox/surface_unchecked", resources), true);
	if(resources->exists("CheckBox/surface_checked_disabled"))
		set_surface_checked_disabled (new CL_Surface("CheckBox/surface_checked_disabled", resources), true);
	if(resources->exists("CheckBox/surface_unchecked_disabled"))
		set_surface_unchecked_disabled (new CL_Surface("CheckBox/surface_unchecked_disabled", resources), true);
	if(resources->exists("CheckBox/font"))
		set_font (new CL_Font("CheckBox/font", resources), true);
	if(resources->exists("CheckBox/font_disabled"))
		set_font_disabled (new CL_Font("CheckBox/font_disabled", resources), true);

	slot_set_options = checkbox->sig_set_options().connect(
		this, &CL_CheckBox_Silver::on_set_options);
	slot_paint = checkbox->sig_paint().connect_virtual(
		this, &CL_CheckBox_Silver::on_paint);
	slot_get_preferred_size = checkbox->sig_get_preferred_size().connect(
		this, &CL_CheckBox_Silver::on_get_preferred_size);
}

CL_CheckBox_Silver::~CL_CheckBox_Silver()
{
	set_font(0);
	set_font_disabled(0);
	set_surface_checked(0);
	set_surface_unchecked(0);
	set_surface_checked_disabled(0);
	set_surface_unchecked_disabled(0);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_CheckBox_Silver::set_font(CL_Font *_font, bool _delete_font)
{
	if(delete_font && font)
		delete font;
	font = _font;
	if (font)
		font->set_color( CL_Color::black );
	delete_font = _delete_font;
}

void CL_CheckBox_Silver::set_font_disabled(CL_Font *_font, bool _delete_font)
{
	if(delete_font_disabled && font_disabled)
		delete font_disabled;
	font_disabled = _font;
	if (font_disabled)
		font_disabled->set_color( CL_Color::gray );
	delete_font_disabled = _delete_font;
}

void CL_CheckBox_Silver::set_surface_checked(CL_Surface *_surface, bool _delete_surface)
{
	if(delete_sur_checked && sur_checked)
		delete sur_checked;
	sur_checked = _surface;
	delete_sur_checked = _delete_surface;
}

void CL_CheckBox_Silver::set_surface_unchecked(CL_Surface *_surface, bool _delete_surface)
{
	if(delete_sur_unchecked && sur_unchecked)
		delete sur_unchecked;
	sur_unchecked = _surface;
	delete_sur_unchecked = _delete_surface;
}

void CL_CheckBox_Silver::set_surface_checked_disabled(CL_Surface *_surface, bool _delete_surface)
{
	if(delete_sur_checked_disabled && sur_checked_disabled)
		delete sur_checked_disabled;
	sur_checked_disabled = _surface;
	delete_sur_checked_disabled = _delete_surface;
}

void CL_CheckBox_Silver::set_surface_unchecked_disabled(CL_Surface *_surface, bool _delete_surface)
{
	if(delete_sur_unchecked_disabled && sur_unchecked_disabled)
		delete sur_unchecked_disabled;
	sur_unchecked_disabled = _surface;
	delete_sur_unchecked_disabled = _delete_surface;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_CheckBox_Silver::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("surface_checked"))
		set_surface_checked(new CL_Surface(options.get_attribute("surface_checked"), resources), true);
	if (options.has_attribute("surface_unchecked"))
		set_surface_unchecked(new CL_Surface(options.get_attribute("surface_unchecked"), resources), true);
	if (options.has_attribute("surface_checked_disabled"))
		set_surface_checked_disabled(new CL_Surface(options.get_attribute("surface_checked_disabled"), resources), true);
	if (options.has_attribute("surface_unchecked_disabled"))
		set_surface_unchecked_disabled(new CL_Surface(options.get_attribute("surface_checked_disabled"), resources), true);
	if (options.has_attribute("font"))
		set_font(new CL_Font(options.get_attribute("font"), resources), true);
	if (options.has_attribute("font_disabled"))
		set_font_disabled(new CL_Font(options.get_attribute("font_disabled"), resources), true);
}

void CL_CheckBox_Silver::on_get_preferred_size(CL_Size &size)
{
	CL_Surface *s = sur_checked;
	size.width = s->get_width() + font->get_width(checkbox->get_text()) + 8;
	size.height = font->get_height() + 4;
	if(size.height < (int)s->get_height())
		size.height = s->get_height();
}

void CL_CheckBox_Silver::on_paint(CL_SlotParent_v0 &super)
{
	CL_Surface *show_surface;
	CL_Font *show_font;
	
	if (checkbox->is_enabled())
	{
		if (checkbox->is_checked())
			show_surface = sur_checked;
		else
			show_surface = sur_unchecked;

		show_font = font;
		// Focus
//		if(checkbox->has_focus())
//			style->fill_rect(0, 0, checkbox->get_width(), show_surface->get_height(), GUICOLOR_FOCUS);
	}
	else
	{
		if (checkbox->is_checked())
			show_surface = sur_checked_disabled;
		else
			show_surface = sur_unchecked_disabled;

		if(font_disabled)
			show_font = font_disabled;
		else
			show_font = font;
	}

	show_surface->draw((float)checkbox->get_screen_x(), (float)checkbox->get_screen_y());

	if (checkbox->get_text().length() > 0)
		show_font->draw(checkbox->get_screen_x() + 4 + show_surface->get_width(), checkbox->get_screen_y(), checkbox->get_text());
}
