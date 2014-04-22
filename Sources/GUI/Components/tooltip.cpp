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
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/Components/tooltip.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/Core/System/timer.h"
#include "../gui_css_strings.h"
#include "API/Display/2D/canvas.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ToolTip_Impl Class:

class ToolTip_Impl
{
public:
	static GUITopLevelDescription create_description();
	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_show_delayed();
	void on_filter_message(std::shared_ptr<GUIMessage> &message);

	ToolTip *tooltip;
	Timer timer_show_delayed;
	std::string text;
};

/////////////////////////////////////////////////////////////////////////////
// ToolTip Construction:

ToolTip::ToolTip(GUIManager manager)
: GUIComponent(&manager, ToolTip_Impl::create_description(), CssStr::ToolTip::type_name), impl(new ToolTip_Impl)
{
	impl->tooltip = this;

	func_process_message().set(impl.get(), &ToolTip_Impl::on_process_message);
	func_render().set(impl.get(), &ToolTip_Impl::on_render);

	impl->timer_show_delayed.func_expired().set(impl.get(), &ToolTip_Impl::on_show_delayed);
    get_gui_manager().sig_filter_message().connect({impl.get(), &ToolTip_Impl::on_filter_message});
}

ToolTip::~ToolTip()
{
}

/////////////////////////////////////////////////////////////////////////////
// ToolTip Attributes:

ToolTip *ToolTip::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ToolTip *object = NULL;
	if (reference_component)
		object = dynamic_cast<ToolTip*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ToolTip named item: %1", id));

	return object;
}

std::string ToolTip::get_text() const
{
	return impl->text;
}

/////////////////////////////////////////////////////////////////////////////
// ToolTip Operations:

void ToolTip::set_text(const std::string &text)
{
	impl->text = text;
	Point top_left = get_geometry().get_top_left();

	Canvas canvas = get_canvas();
	Size text_size = get_render_text_size(canvas, impl->text);
	Rect rect(Point(0,0), text_size);
	set_geometry(Rect(top_left, impl->tooltip->get_size()));
}

void ToolTip::show(const Point &position)
{
	if (is_visible())
		return;
	set_geometry(Rect(position, get_size()));
	set_visible(true, false);
}

void ToolTip::show_delayed(const Point &position, int delay_time)
{
	if (is_visible())
		return;
	set_geometry(Rect(position, get_size()));
	impl->timer_show_delayed.start(delay_time, false);
}

void ToolTip::hide()
{
	impl->timer_show_delayed.stop();
	set_visible(false, false);
}

/////////////////////////////////////////////////////////////////////////////
// ToolTip Implementation:

GUITopLevelDescription ToolTip_Impl::create_description()
{
	GUITopLevelDescription desc;
	desc.set_title("Tooltip");
	desc.set_visible(false);
	desc.set_topmost(true);
	desc.set_drop_shadow(true);
	desc.set_tool_window(true);
	desc.show_caption(false);
	return desc;
}

void ToolTip_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
		tooltip->set_visible(false, false);
}

void ToolTip_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = tooltip->get_geometry();

	Rect content_rect = tooltip->get_content_box();

	Size text_size = tooltip->get_render_text_size(canvas, text);
	tooltip->render_text(canvas, text, 	content_rect.left, content_rect.top + content_rect.get_height()/2 + text_size.height/2 - 2 + tooltip->get_vertical_text_align(canvas).baseline);
}

void ToolTip_Impl::on_show_delayed()
{
	tooltip->set_visible(true, false);
}

void ToolTip_Impl::on_filter_message(std::shared_ptr<GUIMessage> &message)
{
	if (
		std::dynamic_pointer_cast<GUIMessage_FocusChange>(message) ||
		std::dynamic_pointer_cast<GUIMessage_ActivationChange>(message) ||
		std::dynamic_pointer_cast<GUIMessage_Pointer>(message) )
	{
		tooltip->hide();
	}
}

}
