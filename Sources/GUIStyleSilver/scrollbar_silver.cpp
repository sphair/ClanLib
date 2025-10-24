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
#include "API/Core/Resources/resource_manager.h"
#include "API/GUIStyleSilver/scrollbar_silver.h"
#include "API/GUIStyleSilver/button_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ScrollBar_Silver::CL_ScrollBar_Silver(
	CL_ScrollBar *scrollbar,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(scrollbar),
	timer_scroll(250),
	scrollbar(scrollbar),
	style(style)
{
	CL_Component *client_area = scrollbar->get_client_area();

	button_increase = new CL_Button("", scrollbar, style);
	button_decrease = new CL_Button("", scrollbar, style);

	slots.connect(client_area->sig_paint(), this, &CL_ScrollBar_Silver::on_paint);
	slots.connect(timer_scroll.sig_timer(), this, &CL_ScrollBar_Silver::on_timer_scroll);

	slots.connect(button_increase->sig_pressed(), this, &CL_ScrollBar_Silver::start_scroll, 1);
	slots.connect(button_decrease->sig_pressed(), this, &CL_ScrollBar_Silver::start_scroll, -1);
	slots.connect(button_increase->sig_released(), this, &CL_ScrollBar_Silver::stop_scroll);
	slots.connect(button_decrease->sig_released(), this, &CL_ScrollBar_Silver::stop_scroll);
	slots.connect(scrollbar->sig_resize(), this, &CL_ScrollBar_Silver::on_resize);
	slots.connect(scrollbar->sig_set_options(), this, &CL_ScrollBar_Silver::on_set_options);

	on_resize(0, 0);
}

CL_ScrollBar_Silver::~CL_ScrollBar_Silver()
{
	delete button_increase;
	delete button_decrease;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ScrollBar_Silver::on_set_options(const CL_DomElement &options)
{
}

void CL_ScrollBar_Silver::on_resize(int old_width, int old_height)
{
	CL_Rect rect_decrease;
	CL_Rect rect_increase;
	CL_Rect rect_client;

	if (scrollbar->is_vertical())
	{
		int button_size = scrollbar->get_width();
		rect_decrease = CL_Rect(0, 0, button_size, button_size);
		rect_increase = CL_Rect(0, scrollbar->get_height() - button_size, button_size, scrollbar->get_height());
		rect_client = CL_Rect(0, button_size, button_size, scrollbar->get_height() - button_size);
	}
	else
	{
		int button_size = scrollbar->get_height();
		rect_decrease = CL_Rect(0, 0, button_size, button_size);
		rect_increase = CL_Rect(scrollbar->get_width() - button_size, 0, scrollbar->get_width(), button_size);
		rect_client = CL_Rect(button_size, 0, scrollbar->get_width() - button_size, button_size);
	}

	button_decrease->set_position(rect_decrease);
	button_increase->set_position(rect_increase);
	scrollbar->get_client_area()->set_position(rect_client);
}

void CL_ScrollBar_Silver::on_timer_scroll()
{
	scrollbar->set_value(scrollbar->get_value() + scroll_delta);
}

void CL_ScrollBar_Silver::on_paint()
{
	CL_Rect rect = scrollbar->get_screen_rect();

	int offset_x = rect.left;
	int offset_y = rect.top;

	if (scrollbar->is_vertical())
	{
		offset_y += button_decrease->get_height();
	}
	else
	{
		offset_x += button_decrease->get_width();
	}

	CL_Rectf slider_rect = scrollbar->get_slider_rect();
	slider_rect.left += offset_x;
	slider_rect.top += offset_y;
	slider_rect.right += offset_x;
	slider_rect.bottom += offset_y;

	CL_Display::draw_rect(
		rect,
		CL_Color(128, 142, 159));

	CL_Display::fill_rect(
		CL_Rect(rect.left + 1, rect.top, rect.right - 1, rect.bottom),
		CL_Color(203, 215, 227));

		// Main border
	CL_Display::draw_rect(
		slider_rect, 
		CL_Color(128, 142, 159));

	// Outer gradient
	CL_Display::fill_rect(
		CL_Rectf(slider_rect.left + 1, slider_rect.top + 1, slider_rect.right - 1, slider_rect.bottom - 1),
		CL_Gradient(CL_Color::white, CL_Color::white, CL_Color(230, 235, 240), CL_Color(230, 235, 240)));

	// Inner gradient
	CL_Display::fill_rect(
		CL_Rectf(slider_rect.left + 3, slider_rect.top + 3, slider_rect.right - 3, slider_rect.bottom - 3),
		CL_Gradient(CL_Color(217, 222, 227), CL_Color(217, 222, 227), CL_Color::white, CL_Color::white));

/*	CL_Display::draw_line(
		offset_x + slider_rect.left,
		offset_y + slider_rect.top + 1,
		offset_x + slider_rect.left,
		offset_y + slider_rect.bottom - 1,
		CL_Color(128, 142, 159));

	CL_Display::draw_line(
		offset_x + slider_rect.right - 1,
		offset_y + slider_rect.top + 1,
		offset_x + slider_rect.right - 1,
		offset_y + slider_rect.bottom - 1,
		CL_Color(128, 142, 159));

	CL_Display::draw_line(
		offset_x + slider_rect.left + 1,
		offset_y + slider_rect.top,
		offset_x + slider_rect.right - 1,
		offset_y + slider_rect.top,
		CL_Color(128, 142, 159));

	CL_Display::draw_line(
		offset_x + slider_rect.left + 1,
		offset_y + slider_rect.bottom - 1,
		offset_x + slider_rect.right - 1,
		offset_y + slider_rect.bottom - 1,
		CL_Color(128, 142, 159));

	CL_Display::fill_rect(
		CL_Rect(
			offset_x + slider_rect.left + 1,
			offset_y + slider_rect.top + 1,
			offset_x + slider_rect.right - 1,
			offset_y + slider_rect.bottom - 1),
		CL_Gradient(
			CL_Color::white,
			CL_Color(230, 235, 240),
			CL_Color::white,
			CL_Color(230, 235, 240)));
*/
	CL_Display::draw_line(
		slider_rect.left + 5,
		slider_rect.top + slider_rect.get_height() / 2 - 2,
		slider_rect.right - 5,
		slider_rect.top + slider_rect.get_height() / 2 - 2, 
		CL_Color(128, 142, 159));

	CL_Display::draw_line(
		slider_rect.left + 5,
		slider_rect.top + slider_rect.get_height() / 2,
		slider_rect.right - 5,
		slider_rect.top + slider_rect.get_height() / 2, 
		CL_Color(128, 142, 159));

	CL_Display::draw_line(
		slider_rect.left + 5,
		slider_rect.top + slider_rect.get_height() / 2 + 2,
		slider_rect.right - 5,
		slider_rect.top + slider_rect.get_height() / 2 + 2, 
		CL_Color(128, 142, 159));
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_ScrollBar_Silver::start_scroll(int delta)
{
	scroll_delta = delta;
	timer_scroll.enable();
	on_timer_scroll();
}

void CL_ScrollBar_Silver::stop_scroll()
{
	timer_scroll.disable();
}

