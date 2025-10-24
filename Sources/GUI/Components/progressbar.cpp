/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_ProgressBar_Impl Class:

class CL_ProgressBar_Impl
{
public:
	CL_ProgressBar_Impl()
	: progress_min(0), progress_max(100), step_size(1), position(0), marquee_mode(false),
	  marquee_animation_speed(100), marquee_box_width(10), marquee_position(0), marquee_step_size(2)
	{
		marquee_timer.func_expired().set(this, &CL_ProgressBar_Impl::on_marquee_progress);
	}

	void check_range();

	void on_process_message(CL_GUIMessage &msg);

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	
	void on_marquee_progress();
	
	CL_ProgressBar *progressbar;

	int progress_min, progress_max, step_size, position;

	bool marquee_mode;

	int marquee_animation_speed;

	int marquee_box_width, marquee_position, marquee_step_size;
	
	CL_GUIThemePart part_component;
	CL_GUIThemePart part_progress;
	CL_GUIThemePart part_chunk;

	CL_Timer marquee_timer;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ProgressBar Construction:

CL_ProgressBar::CL_ProgressBar(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ProgressBar_Impl)
{
	set_type_name(CssStr::ProgressBar::type_name);

	impl->progressbar = this;
	impl->part_component = CL_GUIThemePart(this);
	impl->part_progress = CL_GUIThemePart(this, "progress");
	impl->part_chunk = CL_GUIThemePart(this, "chunk");

	impl->part_component.set_state(CssStr::normal, true);

	func_process_message().set(impl.get(), &CL_ProgressBar_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ProgressBar_Impl::on_render);
}

CL_ProgressBar::~CL_ProgressBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgressBar Attributes:

CL_ProgressBar *CL_ProgressBar::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_ProgressBar *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_ProgressBar*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_ProgressBar named item: %1", id));

	return object;
}

int CL_ProgressBar::get_min() const
{
	return impl->progress_min;
}

int CL_ProgressBar::get_max() const
{
	return impl->progress_max;
}

int CL_ProgressBar::get_step_size() const
{
	return impl->step_size;
}

int CL_ProgressBar::get_position() const
{
	return impl->position;
}

bool CL_ProgressBar::is_marquee_mode() const
{
	return impl->marquee_mode;
}

int CL_ProgressBar::get_marquee_animation_speed() const
{
	return impl->marquee_animation_speed;
}

int CL_ProgressBar::get_marquee_box_width() const
{
	return impl->marquee_box_width;
}

int CL_ProgressBar::get_marquee_step_size() const
{
	return impl->marquee_step_size;
}



/////////////////////////////////////////////////////////////////////////////
// CL_ProgressBar Operations:

void CL_ProgressBar::set_min(int progress_min)
{
	impl->progress_min = progress_min;

	impl->check_range();
	request_repaint();
}

void CL_ProgressBar::set_max(int progress_max)
{
	impl->progress_max = progress_max;

	impl->check_range();
	request_repaint();
}

void CL_ProgressBar::set_range(int progress_min, int progress_max)
{
	impl->progress_min = progress_min;
	impl->progress_max = progress_max;

	impl->check_range();
	request_repaint();
}

void CL_ProgressBar::set_step_size(int size)
{
	impl->step_size = size;
	request_repaint();
}

void CL_ProgressBar::set_position(int pos)
{
	impl->position = pos;

	impl->check_range();
	request_repaint();
}

void CL_ProgressBar::advance_position(int pos)
{
	impl->position += pos;

	impl->check_range();
	request_repaint();
}

void CL_ProgressBar::step_position()
{
	impl->position += impl->step_size;

	impl->check_range();
	request_repaint();
}

void CL_ProgressBar::set_marquee_mode(bool enable)
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

void CL_ProgressBar::set_marquee_animation_speed(int milliseconds)
{
	impl->marquee_animation_speed = milliseconds;

	if (impl->marquee_mode)
	{
		impl->marquee_timer.stop();
		impl->marquee_timer.start(impl->marquee_animation_speed, true);
	}
}

void CL_ProgressBar::set_marquee_box_width(int width)
{
	impl->marquee_box_width = width;
}

void CL_ProgressBar::set_marquee_step_size(int size)
{
	impl->marquee_step_size = size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgressBar Implementation:

void CL_ProgressBar_Impl::on_marquee_progress()
{
	marquee_position += marquee_step_size;

	if(marquee_position > progressbar->get_width())
		marquee_position = -marquee_box_width;

	progressbar->request_repaint();
}

void CL_ProgressBar_Impl::check_range()
{
	if(position < progress_min)
		position = progress_min;
	if(position > progress_max)
		position = progress_max;
}

void CL_ProgressBar_Impl::on_process_message(CL_GUIMessage &msg)
{
}

void CL_ProgressBar_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = progressbar->get_size();
	part_component.render_box(gc, rect, update_rect);

	if (marquee_mode)
	{
		if (rect.get_width() > 0)
		{
			CL_Rect content_box = part_component.get_content_box(rect);

			CL_Rect progress_rect;
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
			
			part_progress.render_box(gc, progress_rect, update_rect);
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
			CL_Rect content_box = part_component.get_content_box(rect);

			CL_Rect progress_rect;
			progress_rect.left = content_box.left;
			progress_rect.top = content_box.top;
			progress_rect.bottom = content_box.bottom;
			progress_rect.right = content_box.left + (position - progress_min) * content_box.get_width() / (progress_max - progress_min);

			part_progress.render_box(gc, progress_rect, update_rect);
		}
	}
}
