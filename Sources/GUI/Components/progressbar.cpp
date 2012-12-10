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
**    Kenneth Gangstoe
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/timer.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/progressbar.h"
#include "../gui_css_strings.h"
#include "API/Core/Math/rect.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ProgressBar_Impl Class:

class ProgressBar_Impl
{
public:
	ProgressBar_Impl()
	: progress_min(0), progress_max(100), step_size(1), position(0), marquee_mode(false),
	  marquee_animation_speed(100), marquee_box_width(10), marquee_position(0), marquee_step_size(2)
	{
		marquee_timer.func_expired().set(this, &ProgressBar_Impl::on_marquee_progress);
	}

	void check_range();

	void on_process_message(std::shared_ptr<GUIMessage> &msg);

	void on_render(Canvas &canvas, const Rect &update_rect);
	
	void on_marquee_progress();
	
	ProgressBar *progressbar;

	int progress_min, progress_max, step_size, position;

	bool marquee_mode;

	int marquee_animation_speed;

	int marquee_box_width, marquee_position, marquee_step_size;
	
	
	GUIThemePart part_progress;
	GUIThemePart part_chunk;

	Timer marquee_timer;
};

/////////////////////////////////////////////////////////////////////////////
// ProgressBar Construction:

ProgressBar::ProgressBar(GUIComponent *parent)
: GUIComponent(parent, CssStr::ProgressBar::type_name), impl(new ProgressBar_Impl)
{
	impl->progressbar = this;
	impl->part_progress = GUIThemePart(this, "progress");
	impl->part_chunk = GUIThemePart(this, "chunk");

	impl->progressbar->set_pseudo_class(CssStr::normal, true);

	func_process_message().set(impl.get(), &ProgressBar_Impl::on_process_message);
	func_render().set(impl.get(), &ProgressBar_Impl::on_render);
}

ProgressBar::~ProgressBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// ProgressBar Attributes:

ProgressBar *ProgressBar::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ProgressBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<ProgressBar*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ProgressBar named item: %1", id));

	return object;
}

int ProgressBar::get_min() const
{
	return impl->progress_min;
}

int ProgressBar::get_max() const
{
	return impl->progress_max;
}

int ProgressBar::get_step_size() const
{
	return impl->step_size;
}

int ProgressBar::get_position() const
{
	return impl->position;
}

bool ProgressBar::is_marquee_mode() const
{
	return impl->marquee_mode;
}

int ProgressBar::get_marquee_animation_speed() const
{
	return impl->marquee_animation_speed;
}

int ProgressBar::get_marquee_box_width() const
{
	return impl->marquee_box_width;
}

int ProgressBar::get_marquee_step_size() const
{
	return impl->marquee_step_size;
}



/////////////////////////////////////////////////////////////////////////////
// ProgressBar Operations:

void ProgressBar::set_min(int progress_min)
{
	impl->progress_min = progress_min;

	impl->check_range();
	request_repaint();
}

void ProgressBar::set_max(int progress_max)
{
	impl->progress_max = progress_max;

	impl->check_range();
	request_repaint();
}

void ProgressBar::set_range(int progress_min, int progress_max)
{
	impl->progress_min = progress_min;
	impl->progress_max = progress_max;

	impl->check_range();
	request_repaint();
}

void ProgressBar::set_step_size(int size)
{
	impl->step_size = size;
	request_repaint();
}

void ProgressBar::set_position(int pos)
{
	impl->position = pos;

	impl->check_range();
	request_repaint();
}

void ProgressBar::advance_position(int pos)
{
	impl->position += pos;

	impl->check_range();
	request_repaint();
}

void ProgressBar::step_position()
{
	impl->position += impl->step_size;

	impl->check_range();
	request_repaint();
}

void ProgressBar::set_marquee_mode(bool enable)
{
	if (enable != impl->marquee_mode)
	{
		impl->marquee_mode = enable;

		if (enable)
		{
			impl->marquee_position = 0;
			impl->marquee_timer.start(impl->marquee_animation_speed, true);
		}
		else
		{
			impl->marquee_timer.stop();
		}

		request_repaint();
	}
}

void ProgressBar::set_marquee_animation_speed(int milliseconds)
{
	impl->marquee_animation_speed = milliseconds;

	if (impl->marquee_mode)
	{
		impl->marquee_timer.stop();
		impl->marquee_timer.start(impl->marquee_animation_speed, true);
	}
}

void ProgressBar::set_marquee_box_width(int width)
{
	impl->marquee_box_width = width;
}

void ProgressBar::set_marquee_step_size(int size)
{
	impl->marquee_step_size = size;
}

/////////////////////////////////////////////////////////////////////////////
// ProgressBar Implementation:

void ProgressBar_Impl::on_marquee_progress()
{
	marquee_position += marquee_step_size;

	if(marquee_position > progressbar->get_width())
		marquee_position = -marquee_box_width;

	progressbar->request_repaint();
}

void ProgressBar_Impl::check_range()
{
	if(position < progress_min)
		position = progress_min;
	if(position > progress_max)
		position = progress_max;
}

void ProgressBar_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
}

void ProgressBar_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = progressbar->get_size();
	
	if (marquee_mode)
	{
		if (rect.get_width() > 0)
		{
			Rect content_box = progressbar->get_content_box();

			Rect progress_rect;
			progress_rect.left = content_box.left + marquee_position;
			progress_rect.top = content_box.top;
			progress_rect.bottom = content_box.bottom;
			progress_rect.right = content_box.left + marquee_position + marquee_box_width;

			if (progress_rect.left < content_box.left)
			{
				progress_rect.left = 0;
				progress_rect.right -= (content_box.left - progress_rect.left);
			}

			if (progress_rect.right > content_box.right)
			{
				progress_rect.right -= (progress_rect.right - content_box.right);
			}
			
			part_progress.render_box(canvas, progress_rect);
		}
	}
	else
	{
		if (progress_max >= progress_min && 
			position >= progress_min &&
			position <= progress_max &&
			progress_min != progress_max &&
			rect.get_width() > 0)
		{
			Rect content_box = progressbar->get_content_box();

			Rect progress_rect;
			progress_rect.left = content_box.left;
			progress_rect.top = content_box.top;
			progress_rect.bottom = content_box.bottom;
			progress_rect.right = content_box.left + (position - progress_min) * content_box.get_width() / (progress_max - progress_min);

			part_progress.render_box(canvas, progress_rect);
		}
	}
}

}
