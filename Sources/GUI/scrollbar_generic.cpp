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

#include "precomp.h"
#include "scrollbar_generic.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/component.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/button.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/keys.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ScrollBar_Generic::CL_ScrollBar_Generic(
	CL_ScrollBar *self,
	int min,
	int max,
	int value,
	bool orientation,
	bool tracking)
:
	min_value(0),
	max_value(0),
	cur_value(0),
	tracking(tracking),
	fixed_length(false),
	dragging(false),
	scrollbar(self),
	capture_pos(0, 0),
	timer_scroll(250)
{
	vertical = !orientation;

	if(vertical)
		min_slider_length = scrollbar->get_height() / 6;
	else
		min_slider_length = scrollbar->get_width() / 6;
	if(min_slider_length < 20)
		min_slider_length = 20;

	set_min_value(min);
	set_max_value(max);
	set_value(value);

	CL_Rect rect(0, 0, scrollbar->get_width(), scrollbar->get_height());
	CL_Component *client_area = new CL_Component(rect, scrollbar);
	scrollbar->set_client_area(client_area);
	
	slots.connect(scrollbar->sig_set_options(), this, &CL_ScrollBar_Generic::on_set_options);
	slots.connect(client_area->sig_mouse_move(), this, &CL_ScrollBar_Generic::on_mouse_move);
	slots.connect(client_area->sig_mouse_down(), this, &CL_ScrollBar_Generic::on_mouse_down);
	slots.connect(client_area->sig_mouse_up(), this, &CL_ScrollBar_Generic::on_mouse_up);
	slots.connect(client_area->sig_resize(), this, &CL_ScrollBar_Generic::on_client_resize);
	slots.connect(timer_scroll.sig_timer(), this, &CL_ScrollBar_Generic::on_timer_scroll);
}

CL_ScrollBar_Generic::~CL_ScrollBar_Generic()
{
	delete scrollbar->get_client_area();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_ScrollBar_Generic::get_range() const
{
	return max_value - min_value + 1;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_ScrollBar_Generic::set_vertical(bool enable)
{
	throw CL_Error("CL_ScrollBar_Generic::set_vertical() is not implemented");
}

void CL_ScrollBar_Generic::set_range(int new_min_value, int new_max_value)
{
	min_value = new_min_value;
	max_value = new_max_value;

	if(max_value < min_value)
		max_value = min_value;
	
	if(cur_value < min_value)
		cur_value = min_value;
	if(cur_value > max_value)
		cur_value = max_value;

	calculate_slider();
}

void CL_ScrollBar_Generic::set_min_value(int value)
{
	min_value = value;
	if(max_value < min_value)
		max_value = min_value;
	
	if(cur_value < min_value)
		cur_value = min_value;

	calculate_slider();
}

void CL_ScrollBar_Generic::set_max_value(int value)
{
	max_value = value;
	if(max_value < min_value)
		max_value = min_value;
	
	if(cur_value > max_value)
		cur_value = max_value;

	calculate_slider();
}

void CL_ScrollBar_Generic::set_value(int value, bool using_slider)
{
	if(value < min_value) value = min_value;
	if(value > max_value) value = max_value;

	if(cur_value != value)
	{
		cur_value = value;

		if(using_slider)
			sig_slider_moved(cur_value);

		if((using_slider && tracking) || using_slider == false)
			sig_value_changed(cur_value);
	}

	if(using_slider == false)
		calculate_slider();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ScrollBar_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("orientation"))
	{
		std::string s = CL_String::to_lower(options.get_attribute("orientation"));

		if(s == "hor" || s == "horz" | s == "horizontal" || s == "1")
			vertical = false;
		else if(s == "ver" || s == "vert" | s == "vertical" || s == "0")
			vertical = true;
		else
			throw CL_Error("Invalid scrollbar orientation value");
	}
	else
		vertical = true;
	
	if (options.has_attribute("min"))
		set_min_value(CL_String::to_int(options.get_attribute("min")));
	else
		set_min_value(0);

	if (options.has_attribute("max"))
		set_max_value(CL_String::to_int(options.get_attribute("max")));
	else
		set_max_value(min_value);

	if (options.has_attribute("value"))
		set_value(CL_String::to_int(options.get_attribute("value")));
	else
		set_value(min_value);

	if (options.has_attribute("tracking"))
		tracking = CL_String::to_bool(options.get_attribute("tracking"));
	else
		tracking = false;
		
	scrollbar->sig_resize()(0,0);
}

void CL_ScrollBar_Generic::on_client_resize(int old_width, int old_height)
{
	calculate_slider();
}

void CL_ScrollBar_Generic::on_mouse_down(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_WHEEL_UP)
	{
		set_value(cur_value - 5);
		return;
	}
	if(key.id == CL_MOUSE_WHEEL_DOWN)
	{
		set_value(cur_value + 5);
		return;
	}
	if(key.id == CL_MOUSE_LEFT)
	{
		if(dragging == false)
		{
			if(key.mouse_pos.x > rect_slider.left && 
				key.mouse_pos.y > rect_slider.top && 
				key.mouse_pos.x < rect_slider.right && 
				key.mouse_pos.y < rect_slider.bottom)
			{
				dragging = true;
				scrollbar->get_client_area()->capture_mouse();

				capture_pos = key.mouse_pos;
				capture_slider = rect_slider;

				sig_slider_pressed();
			}
			else
			{
				int scrollbar_length;
				int slider_length;

				scroll_delta = 1;

				if(vertical)
				{
					scrollbar_length = scrollbar->get_client_area()->get_height();
					slider_length = rect_slider.get_height();

					if(key.mouse_pos.y < rect_slider.top)
						scroll_delta = -1;
				}
				else
				{
					scrollbar_length = scrollbar->get_client_area()->get_width();
					slider_length = rect_slider.get_width();

					if(key.mouse_pos.x < rect_slider.left)
						scroll_delta = -1;
				}

				float percent = slider_length / (float)(scrollbar_length - slider_length);
				scroll_delta *= (int)( get_range() * percent);

				on_timer_scroll();
				timer_scroll.enable();
				scrollbar->get_client_area()->capture_mouse();
			}
		}
	}
}

void CL_ScrollBar_Generic::on_mouse_up(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_LEFT)
	{
		if(dragging)
		{
			scrollbar->get_client_area()->release_mouse();
			dragging = false;

			set_value(cur_value);

			if(tracking == false)
				sig_value_changed(cur_value);
			sig_slider_released();
		}
		else if(timer_scroll.is_enabled())
		{
			timer_scroll.disable();
			scrollbar->get_client_area()->release_mouse();
		}
	}
}

void CL_ScrollBar_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if(dragging == false)
		return;

	CL_Point delta = key.mouse_pos - capture_pos;

	if(vertical)
	{
		int scrollbar_length = scrollbar->get_client_area()->get_height();
		int slider_length = rect_slider.get_height();

		rect_slider = capture_slider;
		rect_slider.top += delta.y;

		if(rect_slider.top < 0)
			rect_slider.top = 0;
		else if(rect_slider.top + slider_length > scrollbar_length)
			rect_slider.top = scrollbar_length - slider_length;

		rect_slider.bottom = rect_slider.top + slider_length;

		float percent = (float)rect_slider.top / (scrollbar_length - slider_length);
		float value = min_value + (get_range() * percent);
		set_value((int)value, true);
	} 
	else
	{
		int scrollbar_length = scrollbar->get_client_area()->get_width();
		int slider_length = rect_slider.get_width();

		rect_slider = capture_slider;
		rect_slider.left += delta.x;

		if(rect_slider.left < 0)
			rect_slider.left = 0;
		else if(rect_slider.left + slider_length > scrollbar_length)
			rect_slider.left = scrollbar_length - slider_length;

		rect_slider.right = rect_slider.left + slider_length;

		float percent = (float)rect_slider.left / (scrollbar_length - slider_length);
		float value = min_value + (get_range() * percent);
		set_value((int)value, true);
	}
}

void CL_ScrollBar_Generic::on_timer_scroll()
{
	set_value(cur_value + scroll_delta);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_ScrollBar_Generic::calculate_slider()
{
	int slider_offset;
	int slider_length;

	if(fixed_length)
	{
		throw CL_Error("fixed-length sliders not implemented");
	}
	else {
		int scrollbar_length;
		if(vertical)
			scrollbar_length = scrollbar->get_client_area()->get_height();
		else
			scrollbar_length = scrollbar->get_client_area()->get_width();
		
		int range = get_range();

		slider_length = scrollbar_length / range;
		if(slider_length < min_slider_length)
			slider_length = min_slider_length;

		float y = 0.0f;
		if(range > 1)
		{
			int available_area = scrollbar_length - slider_length;
			y = (float)available_area / (range - 1);
		}

		slider_offset = (int) (y * (cur_value - min_value));
	}

	if(vertical)
	{
		rect_slider.top = slider_offset;
		rect_slider.bottom = slider_offset + slider_length;
		rect_slider.left = 0;
		rect_slider.right = scrollbar->get_client_area()->get_width();
	}
	else
	{
		rect_slider.left = slider_offset;
		rect_slider.right = slider_offset + slider_length;
		rect_slider.top = 0;
		rect_slider.bottom = scrollbar->get_client_area()->get_height();
	}
}
