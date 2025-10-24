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
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/Core/Text/string_format.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/tooltip.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/Display/Font/font.h"
#include "API/Core/System/timer.h"
#include "../gui_css_strings.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ToolTip_Impl Class:

class CL_ToolTip_Impl
{
public:
	static CL_GUITopLevelDescription create_description();
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_show_delayed();
	void on_filter_message(CL_GUIMessage &message);

	CL_ToolTip *tooltip;
	CL_Timer timer_show_delayed;
	CL_String text;
	CL_Font font;
	CL_Colorf text_color;
	CL_GUIThemePart part_component;
	CL_GUIThemePartProperty prop_text_color;
	CL_Slot slot_filter_message;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ToolTip Construction:

CL_ToolTip::CL_ToolTip(CL_GUIManager manager)
: CL_GUIComponent(&manager, CL_ToolTip_Impl::create_description()), impl(new CL_ToolTip_Impl)
{
	set_type_name("tooltip");
	impl->tooltip = this;
	impl->prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, "black");

	func_process_message().set(impl.get(), &CL_ToolTip_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ToolTip_Impl::on_render);

	impl->part_component = CL_GUIThemePart(this);
	impl->font = impl->part_component.get_font();
	impl->text_color = impl->part_component.get_property(impl->prop_text_color);

	impl->timer_show_delayed.func_expired().set(impl.get(), &CL_ToolTip_Impl::on_show_delayed);
	impl->slot_filter_message = get_gui_manager().sig_filter_message().connect(impl.get(), &CL_ToolTip_Impl::on_filter_message);
}

CL_ToolTip::~CL_ToolTip()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolTip Attributes:

CL_ToolTip *CL_ToolTip::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_ToolTip *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_ToolTip*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_ToolTip named item: %1", id));

	return object;
}

CL_StringRef CL_ToolTip::get_text() const
{
	return impl->text;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolTip Operations:

void CL_ToolTip::set_text(const CL_StringRef &text)
{
	impl->text = text;
	CL_Point top_left = get_geometry().get_top_left();

	CL_GraphicContext &gc = get_gc();
	CL_Size text_size = impl->font.get_text_size(gc, impl->text);
	CL_Rect rect(CL_Point(0,0), text_size);
	CL_Rect client_rect = impl->part_component.get_render_box(rect);
	set_geometry(CL_Rect(top_left, client_rect.get_size()));
}

void CL_ToolTip::show(const CL_Point &position)
{
	if (is_visible())
		return;
	set_geometry(CL_Rect(position, get_size()));
	set_visible(true, false);
}

void CL_ToolTip::show_delayed(const CL_Point &position, int delay_time)
{
	if (is_visible())
		return;
	set_geometry(CL_Rect(position, get_size()));
	impl->timer_show_delayed.start(delay_time, false);
}

void CL_ToolTip::hide()
{
	impl->timer_show_delayed.stop();
	set_visible(false, false);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ToolTip Implementation:

CL_GUITopLevelDescription CL_ToolTip_Impl::create_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Tooltip");
	desc.set_visible(false);
	desc.set_topmost(true);
	desc.set_decorations(false);
	desc.set_drop_shadow(true);
	desc.set_tool_window(true);
	desc.show_caption(false);
	return desc;
}

void CL_ToolTip_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
		tooltip->set_visible(false, false);
}

void CL_ToolTip_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = tooltip->get_geometry();
	part_component.render_box(gc, rect.get_size(), update_rect);

	CL_Rect content_rect = part_component.get_content_box(rect.get_size());

	CL_Size text_size = font.get_text_size(gc, text);
	font.draw_text(gc,
		content_rect.left,
		content_rect.top + content_rect.get_height()/2 + text_size.height/2 - 2,
		text,
		text_color);
}

void CL_ToolTip_Impl::on_show_delayed()
{
	tooltip->set_visible(true, false);
}

void CL_ToolTip_Impl::on_filter_message(CL_GUIMessage &message)
{
	if (message.get_type() == CL_GUIMessage_FocusChange::get_type_name() ||
		message.get_type() == CL_GUIMessage_ActivationChange::get_type_name() ||
		message.get_type() == CL_GUIMessage_Pointer::get_type_name())
	{
		tooltip->hide();
	}
}
