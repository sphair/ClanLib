/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Math/rect.h"
#include "scrollbar_impl.h"
#include "../gui_css_strings.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ScrollBar Construction:

ScrollBar::ScrollBar(GUIComponent *parent)
: GUIComponent(parent, CssStr::ScrollBar::type_name), impl(new ScrollBar_Impl)
{
	impl->scrollbar = this;

	impl->create_parts();
	impl->update_part_positions();

	func_process_message().set(impl.get(), &ScrollBar_Impl::on_process_message);
	func_render().set(impl.get(), &ScrollBar_Impl::on_render);
	func_enablemode_changed().set(impl.get(), &ScrollBar_Impl::on_enablemode_changed);
	func_resized().set(impl.get(), &ScrollBar_Impl::on_resized);

	impl->mouse_down_timer.func_expired().set(impl.get(), &ScrollBar_Impl::on_timer_expired);
}

ScrollBar::~ScrollBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// ScrollBar Attributes:

ScrollBar *ScrollBar::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ScrollBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<ScrollBar*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ScrollBar named item: %1", id));

	return object;
}

bool ScrollBar::is_vertical() const
{
	return impl->vertical;
}

bool ScrollBar::is_horizontal() const
{
	return !impl->vertical;
}

int ScrollBar::get_min() const
{
	return impl->scroll_min;
}

int ScrollBar::get_max() const
{
	return impl->scroll_max;
}

int ScrollBar::get_line_step() const
{
	return impl->line_step;
}

int ScrollBar::get_page_step() const
{
	return impl->page_step;
}

int ScrollBar::get_position() const
{
	return impl->position;
}


float ScrollBar::get_preferred_content_width()
{
	//FIXME:
	return 100.0f;
}

float ScrollBar::get_preferred_content_height(float width)
{
	//FIXME:
	return 20.0f;
}

/////////////////////////////////////////////////////////////////////////////
// ScrollBar Operations:

void ScrollBar::set_vertical()
{
	impl->vertical = true;
	impl->create_parts();
	if(impl->update_part_positions())
		request_repaint();
}

void ScrollBar::set_horizontal()
{
	impl->vertical = false;
	impl->create_parts();
	if(impl->update_part_positions())
		request_repaint();
}

void ScrollBar::set_min(int new_scroll_min)
{
	set_ranges(new_scroll_min, impl->scroll_max, impl->line_step, impl->page_step);
}

void ScrollBar::set_max(int new_scroll_max)
{
	set_ranges(impl->scroll_min, new_scroll_max, impl->line_step, impl->page_step);
}

void ScrollBar::set_line_step(int step)
{
	set_ranges(impl->scroll_min, impl->scroll_max, step, impl->page_step);
}

void ScrollBar::set_page_step(int step)
{
	set_ranges(impl->scroll_min, impl->scroll_max, impl->line_step, step);
}

void ScrollBar::set_ranges(int scroll_min, int scroll_max, int line_step, int page_step)
{
	if (scroll_min >= scroll_max || line_step <= 0 || page_step <= 0)
		throw Exception("Scrollbar ranges out of bounds!");
	impl->scroll_min = scroll_min;
	impl->scroll_max = scroll_max;
	impl->line_step = line_step;
	impl->page_step = page_step;
	if (impl->position >= impl->scroll_max)
		impl->position = impl->scroll_max-1;
	if (impl->position < impl->scroll_min)
		impl->position = impl->scroll_min;
	if(impl->update_part_positions())
		request_repaint();
}

void ScrollBar::calculate_ranges(int view_size, int total_size)
{
	if (view_size <= 0 || total_size <= 0)
	{
		set_ranges(0, 1, 1, 1);
	}
	else
	{
		int scroll_max = max(1, total_size - view_size + 1);
		int page_step = max(1, view_size);
		set_ranges(0, scroll_max, 1, page_step);
	}
}

void ScrollBar::set_position(int pos)
{
	impl->position = pos;
	if (pos >= impl->scroll_max)
		impl->position = impl->scroll_max-1;
	if (pos < impl->scroll_min)
		impl->position = impl->scroll_min;

	if(impl->update_part_positions())
		request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// ScrollBar Events:

Callback<void()> &ScrollBar::func_scroll()
{
	return impl->func_scroll;
}

Callback<void()> &ScrollBar::func_scroll_min()
{
	return impl->func_scroll_min;
}

Callback<void()> &ScrollBar::func_scroll_max()
{
	return impl->func_scroll_max;
}

Callback<void()> &ScrollBar::func_scroll_line_decrement()
{
	return impl->func_scroll_line_decrement;
}

Callback<void()> &ScrollBar::func_scroll_line_increment()
{
	return impl->func_scroll_line_increment;
}

Callback<void()> &ScrollBar::func_scroll_page_decrement()
{
	return impl->func_scroll_page_decrement;
}

Callback<void()> &ScrollBar::func_scroll_page_increment()
{
	return impl->func_scroll_page_increment;
}

Callback<void()> &ScrollBar::func_scroll_thumb_release()
{
	return impl->func_scroll_thumb_release;
}

Callback<void()> &ScrollBar::func_scroll_thumb_track()
{
	return impl->func_scroll_thumb_track;
}

Callback<void()> &ScrollBar::func_scroll_end()
{
	return impl->func_scroll_end;
}

/////////////////////////////////////////////////////////////////////////////
// ScrollBar Implementation:

}
