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
**    Kenneth Gangstø
**    (if your name is missing here, please add it)
*/

#include "API/Display/display.h"
#include "API/Display/surface.h"
#include "API/Display/font.h"
#include "API/GUI/gui_manager.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"
#include "API/GUIStyleSilver/button_silver.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Math/rect.h"
#include "API/Core/XML/dom_element.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Button_Silver::CL_Button_Silver(
	CL_Button *button,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(button),
	button(button),
	style(style),
	font(0),
	font_disabled(0),
	delete_font(false),
	delete_font_disabled(false),
	sur_up(0), 
	sur_down(0),
	sur_highlighted(0),
	sur_disabled(0), 
	draw_only_surfaces(true),
	delete_sur_up(false),
	delete_sur_down(false),
	delete_sur_highlighted(false),
	delete_sur_disabled(false)
{
	resources = style->get_resources();

	if(resources->exists("Button/surface_up"))
		set_surface_up(new CL_Surface("Button/surface_up", resources), true);
	if(resources->exists("Button/surface_down"))
		set_surface_down(new CL_Surface("Button/surface_down", resources), true);
	if(resources->exists("Button/surface_highlighted"))
		set_surface_highlighted(new CL_Surface("Button/surface_highlighted", resources), true);
	if(resources->exists("Button/surface_disabled"))
		set_surface_disabled(new CL_Surface("Button/surface_disabled", resources), true);
	if(resources->exists("Button/font"))
		set_font(new CL_Font("Button/font", resources), true);
	if(resources->exists("Button/font_disabled"))
		set_font_disabled(new CL_Font("Button/font_disabled", resources), true);

	slot_set_options = button->sig_set_options().connect(
		this, &CL_Button_Silver::on_set_options);
	slot_paint = button->sig_paint().connect(
		this, &CL_Button_Silver::on_paint);
	slot_get_preferred_size = button->sig_get_preferred_size().connect(
		this, &CL_Button_Silver::on_get_preferred_size);

	if (font)
		font->set_color( CL_Color::black );
}

CL_Button_Silver::~CL_Button_Silver()
{
	if(delete_font && font)
		delete font;
	if(delete_font_disabled && font_disabled)
		delete font_disabled;
	if(delete_sur_up && sur_up)
		delete sur_up;
	if(delete_sur_down && sur_down)
		delete sur_down;
	if(delete_sur_highlighted && sur_highlighted)
		delete sur_highlighted;
	if(delete_sur_disabled && sur_disabled)
		delete sur_disabled;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Button_Silver::set_font(CL_Font *_font, bool _delete_font)
{
	if(delete_font && font)
		delete font;
	font = _font;
	if (font)
		font->set_color( CL_Color::black );
	delete_font = _delete_font;
}

void CL_Button_Silver::set_font_disabled(CL_Font *_font, bool _delete_font)
{
	if(delete_font_disabled && font_disabled)
		delete font_disabled;
	font_disabled = _font;
	font_disabled->set_color( CL_Color::gray );
	delete_font_disabled = _delete_font;
}

void CL_Button_Silver::set_surface_up(CL_Surface *surface, bool delete_surface)
{
	if(delete_sur_up && sur_up)
		delete sur_up;
	sur_up = surface;
	delete_sur_up = delete_surface;
}

void CL_Button_Silver::set_surface_down(CL_Surface *surface, bool delete_surface)
{
	if(delete_sur_down && sur_down)
		delete sur_down;
	sur_down = surface;
	delete_sur_down = delete_surface;
}

void CL_Button_Silver::set_surface_highlighted(CL_Surface *surface, bool delete_surface)
{
	if(delete_sur_highlighted && sur_highlighted)
		delete sur_highlighted;
	sur_highlighted = surface;
	delete_sur_highlighted = delete_surface;
}

void CL_Button_Silver::set_surface_disabled(CL_Surface *surface, bool delete_surface)
{
	if(delete_sur_disabled && sur_disabled)
		delete sur_disabled;
	sur_disabled = surface;
	delete_sur_disabled = delete_surface;
}

void CL_Button_Silver::set_surface_draw_only(bool enable)
{
	draw_only_surfaces = enable;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Button_Silver::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("surface_up"))
		set_surface_up(new CL_Surface(options.get_attribute("surface_up"), resources), true);
	if (options.has_attribute("surface_down"))
		set_surface_down(new CL_Surface(options.get_attribute("surface_down"), resources), true);
	if (options.has_attribute("surface_disabled"))
		set_surface_disabled(new CL_Surface(options.get_attribute("surface_disabled"), resources), true);
	if (options.has_attribute("surface_highlighted"))
		set_surface_highlighted(new CL_Surface(options.get_attribute("surface_highlighted"), resources), true);
	if (options.has_attribute("font"))
	{
		set_font(new CL_Font(options.get_attribute("font"), resources), true);
		font->set_color( CL_Color::black );
	}
	if (options.has_attribute("font_disabled"))
		set_font_disabled(new CL_Font(options.get_attribute("font_disabled"), resources), true);
}

void CL_Button_Silver::on_get_preferred_size(CL_Size &size)
{
	if(sur_up)
	{
		size.width = sur_up->get_width();
		size.height = sur_up->get_height();
	}
	else
	{
		if(font)
		{
			size.width = font->get_width(button->get_text()) + 8;
			size.height = font->get_height() + 4;
		}
	}
}

void CL_Button_Silver::on_paint()
{
	int text_width = 0;
	int text_height = 0;

	if(font)
	{
		text_width = font->get_width(button->get_text().c_str());
		text_height = font->get_height();
	}

	CL_Rectf rect = button->get_screen_rect();

	int font_xpos = static_cast<int>(rect.left + (rect.right - rect.left - text_width) / 2);
	int font_ypos = static_cast<int>(rect.top + (rect.bottom - rect.top - text_height) / 2);

	if(button->is_enabled() == false)
	{
		if(draw_only_surfaces == false || sur_disabled == NULL)
		{
			CL_Display::draw_rect(rect, CL_Color(203, 209, 216));
		}
		if(sur_disabled)
		{
			sur_disabled->draw(rect.left + (rect.get_width() - sur_disabled->get_width()) / 2, rect.top + (rect.get_height() - sur_disabled->get_height()) / 2);
		}

		if(font_disabled)
		{
			font_disabled->draw(font_xpos, font_ypos, button->get_text());
		}
		else if(font)
		{
			font->draw(font_xpos, font_ypos, button->get_text());
		}
	}
	else 
	{
		if(button->is_drawn_down())
		{
			if(draw_only_surfaces == false || sur_down == NULL)
			{
				// Main border
				CL_Display::draw_rect(
					rect, 
					CL_Color(128, 142, 159));

				// Dark inner border
				CL_Display::draw_rect(
					CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
					CL_Color(73, 94, 120));

				// Outer gradient
				CL_Display::fill_rect(
					CL_Rectf(rect.left + 2, rect.top + 2, rect.right - 1, rect.bottom - 1),
					CL_Gradient(CL_Color::white, CL_Color::white, CL_Color(230, 235, 240), CL_Color(230, 235, 240)));

				// Inner fill
				CL_Display::fill_rect(
					CL_Rectf(rect.left + 4, rect.top + 4, rect.right - 1, rect.bottom - 1),
					CL_Color(240, 242, 244));
			}

			if(sur_down)
			{
				sur_down->draw(rect.left + (rect.get_width() - sur_down->get_width()) / 2, rect.top + (rect.get_height() - sur_down->get_height()) / 2);
			}

			if(font)
			{
				font->draw(font_xpos + 1, font_ypos + 1, button->get_text());
			}
		}
		else
		{
			bool need_highlight = false;
			if (button->has_mouse_over())
			{
				if (!button->get_gui_manager() || (!button->get_gui_manager()->get_modal_component() ||	button->has_modal_parent()))
				{
					need_highlight = true;
				}
			}

			CL_Surface *sur = NULL;
			if(sur_highlighted && need_highlight)
			{
				sur = sur_highlighted;
			}
			else if(sur_up)
			{
				sur = sur_up;
			}

			if(draw_only_surfaces == false || sur == NULL)
			{
				// Highlight
				if(need_highlight)
				{
					// Main border
					CL_Display::draw_rect(
						rect, 
						CL_Color(209, 149, 32));

					// Highlight
					CL_Display::draw_rect(
						CL_Rectf(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1),
						CL_Color(250, 236, 204));

					// Outer gradient
					CL_Display::fill_rect(
						CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
						CL_Gradient(CL_Color::white, CL_Color::white, CL_Color(230, 235, 240), CL_Color(230, 235, 240)));

					// Inner fill
					CL_Display::fill_rect(
						CL_Rectf(rect.left + 3, rect.top + 3, rect.right - 3, rect.bottom - 3),
						CL_Color(240, 242, 244));
				}
				// Focus
				else if(button->has_focus())
				{
					// Main border
					CL_Display::draw_rect(
						rect, 
						CL_Color(119, 138, 187));

					// Highlight
					CL_Display::draw_rect(
						CL_Rectf(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1),
						CL_Color(206, 220, 233));

					// Outer gradient
					CL_Display::fill_rect(
						CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
						CL_Gradient(CL_Color::white, CL_Color::white, CL_Color(230, 235, 240), CL_Color(230, 235, 240)));

					// Inner fill
					CL_Display::fill_rect(
						CL_Rectf(rect.left + 3, rect.top + 3, rect.right - 3, rect.bottom - 3),
						CL_Color(240, 242, 244));
				}
				// Normal
				else
				{
					// Main border
					CL_Display::draw_rect(
						rect, 
						CL_Color(128, 142, 159));

					// Outer gradient
					CL_Display::fill_rect(
						CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
						CL_Gradient(CL_Color::white, CL_Color::white, CL_Color(230, 235, 240), CL_Color(230, 235, 240)));

					// Inner fill
					CL_Display::fill_rect(
						CL_Rectf(rect.left + 3, rect.top + 3, rect.right - 3, rect.bottom - 3),
						CL_Color(240, 242, 244));
				}
			}

			if(sur)
			{
				sur->draw(rect.left + (rect.get_width() - sur->get_width()) / 2, rect.top + (rect.get_height() - sur->get_height()) / 2);
			}

			if(font)
			{
				font->draw(font_xpos, font_ypos, button->get_text());
			}
		}
	}
}
