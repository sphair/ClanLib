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
#include "API/Display/font.h"
#include "API/Display/surface.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"
#include "API/GUIStyleSilver/window_silver.h"
#include "API/GUIStyleSilver/button_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Window_Silver::CL_Window_Silver(
	CL_Window *window,
	CL_StyleManager_Silver *style) 
:
	CL_ComponentStyle(window),
	window(window),
	button_close(0),
	button_maximize(0),
	button_minimize(0),
	button_help(0),
	style(style),
	titlebar_height(22)
{
	CL_ResourceManager *resources = style->get_resources();
	
	font = new CL_Font("Window/font", resources);
	font_disabled = new CL_Font("Window/font_disabled", resources);
	
	font->set_color( CL_Color::black );
	font_disabled->set_color( CL_Color::gray );

	// Make the window movable by dragging titlebar
	move_handler = new CL_ComponentMoveHandler(window);

	// Make the window resizable by dragging corner
	resize_handler = new CL_ComponentResizeHandler(window);

	CL_Window::TitlebarButtons buttons = window->get_titlebar_buttons_enum();

	// Create a close button on the titlebar
	if(buttons & CL_Window::close_button)
	{
		button_close = add_titlebar_button("Window/surface_close", "Window/surface_close", "Window/surface_close_disabled", resources);
		slots.connect(button_close->sig_clicked(), this, &CL_Window_Silver::on_close_button_clicked);
	}

	// Create a maximize button on the titlebar
	if(buttons & CL_Window::maximize_button)
	{
		button_maximize = add_titlebar_button("Window/surface_maximize", "Window/surface_maximize", "Window/surface_maximize_disabled", resources);
		slots.connect(button_maximize->sig_clicked(), this, &CL_Window_Silver::on_maximize_button_clicked);
	}

	// Create a minimize button on the titlebar
	if(buttons & CL_Window::minimize_button)
	{
		button_minimize = add_titlebar_button("Window/surface_minimize", "Window/surface_minimize", "Window/surface_minimize_disabled", resources);
		slots.connect(button_minimize->sig_clicked(), this, &CL_Window_Silver::on_minimize_button_clicked);
	}

	// Create a help button on the titlebar
	if(buttons & CL_Window::help_button)
	{
		button_help = add_titlebar_button("Window/surface_help", "Window/surface_help", "Window/surface_help_disabled", resources);
		slots.connect(button_help->sig_clicked(), this, &CL_Window_Silver::on_help_button_clicked);
	}

	slots.connect(window->sig_paint(), this, &CL_Window_Silver::on_paint);
	slots.connect(window->sig_resize(), this, &CL_Window_Silver::on_resize);

	on_resize(0,0);
}

CL_Window_Silver::~CL_Window_Silver()
{
	delete font;
	delete font_disabled;
	delete resize_handler;
	delete move_handler;

	if(button_close)
		delete button_close;
	if(button_maximize)
		delete button_maximize;
	if(button_minimize)
		delete button_minimize;
	if(button_help)
		delete button_help;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_Window_Silver::on_paint()
{
	CL_Rectf rect = window->get_screen_rect();

	// Titlebar
	CL_Display::fill_rect(
		CL_Rectf(rect.left + 2, rect.top + 4, rect.right - 2, rect.top + titlebar_height - 3),
		CL_Color(206, 217, 228));

	CL_Display::draw_line(
		rect.left + 2,
		rect.top + 2,
		rect.right - 2,
		rect.top + 2,
		CL_Color(251, 253, 255));

	CL_Display::draw_line(
		rect.left + 2,
		rect.top + 3,
		rect.right - 2,
		rect.top + 3,
		CL_Color(236, 238, 240));

	CL_Display::draw_line(
		rect.left + 2,
		rect.top + titlebar_height - 3,
		rect.right - 2,
		rect.top + titlebar_height - 3,
		CL_Color(180, 202, 224));

	CL_Display::draw_line(
		rect.left + 2,
		rect.top + titlebar_height - 2,
		rect.right - 2,
		rect.top + titlebar_height - 2,
		CL_Color(144, 180, 218));

	CL_Display::draw_line(
		rect.left + 2,
		rect.top + titlebar_height - 1,
		rect.right - 2,
		rect.top + titlebar_height - 1,
		CL_Color(128, 142, 159));

	// Outer window frame
	CL_Display::draw_rect(
		rect,
		CL_Color(128, 142, 159));

	// Inner window frame
	CL_Display::draw_rect(
		CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
		CL_Color(161, 172, 185));

	// Client area
	CL_Display::fill_rect(
		CL_Rectf(rect.left + 2, rect.top + titlebar_height, rect.right - 2, rect.bottom - 2),
		CL_Gradient(
			CL_Color(255, 255, 255, 235),
			CL_Color(255, 255, 255, 235),
			CL_Color(224, 228, 232, 235),
			CL_Color(224, 228, 232, 235)));
		
	int y = (titlebar_height + 2 - font->get_height()) / 2;

	if(window->is_enabled() == false)
		font_disabled->draw(static_cast<int>(rect.left + 7),
								  static_cast<int>(rect.top + y), window->get_title());
	else
	{
		font->draw(static_cast<int>(rect.left + 7), 
					  static_cast<int>(rect.top + y), window->get_title());
		font->draw(static_cast<int>(rect.left + 8),
					  static_cast<int>(rect.top + y), window->get_title());
	}

/*	CL_Rect rect = window->get_screen_rect();

	int offx = window->get_screen_x();
	int offy = window->get_screen_y();

	int w_width = rect.get_width();
	int w_height = rect.get_height();

	// Vertical shadows
	CL_Display::draw_line(offx+w_width + 0, offy + 5, offx+w_width + 0, offy+w_height + 0, CL_Color(0, 0, 0, 64));
	CL_Display::draw_line(offx+w_width + 1, offy + 5, offx+w_width + 1, offy+w_height + 1, CL_Color(0, 0, 0, 51));
	CL_Display::draw_line(offx+w_width + 2, offy + 5, offx+w_width + 2, offy+w_height + 2, CL_Color(0, 0, 0, 38));
	CL_Display::draw_line(offx+w_width + 3, offy + 5, offx+w_width + 3, offy+w_height + 3, CL_Color(0, 0, 0, 25));
	CL_Display::draw_line(offx+w_width + 4, offy + 5, offx+w_width + 4, offy+w_height + 4, CL_Color(0, 0, 0, 13));

	// Horizontal shadows
	CL_Display::draw_line(offx + 5, offy+w_height + 0, offx+w_width + 1, offy+w_height + 0, CL_Color(0, 0, 0, 64));
	CL_Display::draw_line(offx + 5, offy+w_height + 1, offx+w_width + 2, offy+w_height + 1, CL_Color(0, 0, 0, 51));
	CL_Display::draw_line(offx + 5, offy+w_height + 2, offx+w_width + 3, offy+w_height + 2, CL_Color(0, 0, 0, 38));
	CL_Display::draw_line(offx + 5, offy+w_height + 3, offx+w_width + 4, offy+w_height + 3, CL_Color(0, 0, 0, 25));
	CL_Display::draw_line(offx + 5, offy+w_height + 4, offx+w_width + 5, offy+w_height + 4, CL_Color(0, 0, 0, 13));
*/
}

void CL_Window_Silver::on_resize(int old_width, int old_height)
{
	CL_Rect client_rect(3, 24, window->get_width() - 3, window->get_height() - 3);
	window->get_client_area()->set_position(client_rect);

	int xpos = window->get_width() - 20;

	if(button_close)
	{
		CL_Rect close_rect(xpos, 3, xpos + 17, 20);
		button_close->set_position(close_rect);
		xpos -= 18;
	}

	if(button_maximize)
	{
		CL_Rect close_rect(xpos, 3, xpos + 17, 20);
		button_maximize->set_position(close_rect);
		xpos -= 18;
	}

	if(button_minimize)
	{
		CL_Rect close_rect(xpos, 3, xpos + 17, 20);
		button_minimize->set_position(close_rect);
		xpos -= 18;
	}

	if(button_help)
	{
		CL_Rect close_rect(xpos, 3, xpos + 17, 20);
		button_help->set_position(close_rect);
	}

	CL_Rect move_rect(0, 0, window->get_width(), titlebar_height);
	move_handler->set_position(move_rect);
	
	CL_Rect resize_rect(window->get_width() - 3, window->get_height() - 3, window->get_width(), window->get_height());
	resize_handler->set_position(resize_rect);
}

void CL_Window_Silver::on_close_button_clicked()
{
	window->sig_close_button_clicked()();
}

void CL_Window_Silver::on_minimize_button_clicked()
{
	window->sig_minimize_button_clicked()();
}

void CL_Window_Silver::on_maximize_button_clicked()
{
	window->sig_maximize_button_clicked()();
}

void CL_Window_Silver::on_help_button_clicked()
{
	window->sig_help_button_clicked()();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

CL_Button *CL_Window_Silver::add_titlebar_button(
	const std::string &resource_normal, 
	const std::string &resource_down, 
	const std::string &resource_disabled, 
	CL_ResourceManager *resources)
{
	CL_Button *button = new CL_Button(window, style);

	CL_Button_Silver *button_style = (CL_Button_Silver *)button->get_style();

	CL_Surface *surface_button = new CL_Surface(resource_normal, resources);
	CL_Surface *surface_button_down = new CL_Surface(resource_down, resources);
	CL_Surface *surface_button_disabled = new CL_Surface(resource_disabled, resources);

	surface_button_down->set_alignment(origin_top_left, 1, -1);

	button_style->set_surface_up(surface_button, true);
	button_style->set_surface_down(surface_button_down, true);
	button_style->set_surface_disabled(surface_button_disabled, true);
	
	button_style->set_surface_draw_only(false);

	return button;
}
