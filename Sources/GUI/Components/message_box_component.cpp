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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_close.h"
#include "API/GUI/gui_message_activation_change.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_window_manager.h"
#include "API/GUI/Components/push_button.h"
#include "API/GUI/Components/window.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/sprite.h"
#include "../gui_css_strings.h"
#include "message_box_component.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// MessageBoxComponent Construction:

MessageBoxComponent::MessageBoxComponent(
	GUIComponent *parent,
	std::string detail_text,
	MessageBoxButtons buttons,
	MessageBoxIcon icon) 
:	
	GUIComponent(parent, CssStr::MessageBox::type_name),
	button_ok(0),
	button_yes(0),
	button_no(0),
	button_cancel(0),
	detail_text(detail_text),
	messagebox_buttons(buttons),
	messagebox_icon(icon)
{
	set_css_class();
	create_buttons();
	create_parts();

	func_render().set(this, &MessageBoxComponent::on_render);
}

MessageBoxComponent::~MessageBoxComponent()
{
}

/////////////////////////////////////////////////////////////////////////////
// MessageBoxComponent Attributes:

/////////////////////////////////////////////////////////////////////////////
// MessageBoxComponent Operations:

Size MessageBoxComponent::layout_content()
{
	Size size(0,0);
	Size icon_size = part_icon.get_preferred_size();
	Rect border_space = component->get_content_shrink_box();

	Size min_size = component->get_preferred_size();
	if (min_size == Size(0,0))
	{
		min_size = Size(300,80);
	}

	int button_area_height = component->get_property_int(CssStr::MessageBox::button_area_height, "40");

	int icon_margin_right = component->get_property_int("icon_text_gap", "7");

	rect_icon = part_icon.get_render_box(icon_size).get_size();
	rect_icon.translate(border_space.left, 0);


	Size text_size = component->get_text_size(get_canvas(), detail_text);
	rect_text = component->get_render_box(text_size);
	rect_text.left = rect_icon.right + icon_margin_right;
	rect_text.right = rect_text.left + text_size.width;

	size.width = border_space.left;
	size.width += rect_icon.get_width();
	size.width += icon_margin_right;
	size.width += text_size.width;
	size.width += border_space.right;

	size.height = border_space.top;
	size.height += max(icon_size.height, text_size.height);
	size.height += border_space.bottom;
	size.height += button_area_height;

	int detail_area_height = 0;
	detail_area_height = border_space.top + max(text_size.height, icon_size.height) + border_space.bottom;
	detail_area_height = max(detail_area_height, min_size.height - button_area_height);

	// Center the text & icon in the detail-area
	rect_text.top = detail_area_height/2 - text_size.height/2;
	rect_text.bottom = rect_text.top + text_size.height;

	rect_icon.top = detail_area_height/2 - icon_size.height/2;
	rect_icon.bottom = rect_icon.top + icon_size.height;

	Size final_size;
	final_size.width = max(min_size.width, size.width);
	final_size.height = max(min_size.height, size.height);

	// Set button geometries.
	switch (messagebox_buttons)
	{
		case mb_buttons_ok:
		{
			Size button_size = button_ok->get_preferred_size();
			if (button_size == Size(0,0))
			{
				button_size = Size(66,21);
			}

			Rect R;
			R.left = final_size.width - border_space.right - button_size.width;
			R.top = final_size.height - border_space.bottom - button_size.height;
			R.right = R.left + button_size.width;
			R.bottom = R.top + button_size.height;
			button_ok->set_geometry(R);
			break;
		}
		case mb_buttons_yes_no:
		{
			Size button_size = button_yes->get_preferred_size();
			if (button_size == Size(0,0))
			{
				button_size = Size(66,21);
			}

			Rect yes;
			yes.left = final_size.width - border_space.right - button_size.width;
			yes.top = final_size.height - border_space.bottom - button_size.height;
			yes.right = yes.left + button_size.width;
			yes.bottom = yes.top + button_size.height;
			button_yes->set_geometry(yes);

			Rect no = yes;
			no.translate(-button_size.width - 7, 0);
			button_no->set_geometry(no);
			break;
		}
		case mb_buttons_yes_no_cancel:
		{
			Size button_size = button_yes->get_preferred_size();
			if (button_size == Size(0,0))
			{
				button_size = Size(66,21);
			}

			Rect yes;
			yes.left = final_size.width - border_space.right - button_size.width;
			yes.top = final_size.height - border_space.bottom - button_size.height;
			yes.right = yes.left + button_size.width;
			yes.bottom = yes.top + button_size.height;
			button_yes->set_geometry(yes);

			Rect no = yes;
			no.translate(-button_size.width - 7, 0);
			button_no->set_geometry(no);

			Rect cancel = no;
			cancel.translate(-button_size.width - 7, 0);
			button_cancel->set_geometry(cancel);
			break;
		}
		case mb_buttons_ok_cancel:
		{
			Size button_size = button_ok->get_preferred_size();
			if (button_size == Size(0,0))
			{
				button_size = Size(66,21);
			}

			Rect ok;
			ok.left = final_size.width - border_space.right - button_size.width;
			ok.top = final_size.height - border_space.bottom - button_size.height;
			ok.right = ok.left + button_size.width;
			ok.bottom = ok.top + button_size.height;
			button_ok->set_geometry(ok);

			Rect cancel = ok;
			cancel.translate(-button_size.width - 7, 0);
			button_cancel->set_geometry(cancel);
			break;
		}
	}

	return final_size; 
}

/////////////////////////////////////////////////////////////////////////////
// MessageBoxComponent Implementation:

void MessageBoxComponent::create_buttons()
{
	switch (messagebox_buttons)
	{
	case mb_buttons_ok:
		button_ok = new PushButton(this);
		break;
	case mb_buttons_ok_cancel:
		button_ok = new PushButton(this);
		button_cancel = new PushButton(this);
		break;
	case mb_buttons_yes_no:
		button_yes = new PushButton(this);
		button_no = new PushButton(this);
		break;
	case mb_buttons_yes_no_cancel:
		button_yes = new PushButton(this);
		button_no = new PushButton(this);
		button_cancel = new PushButton(this);
		break;
	}

	if (button_ok)
	{
		button_ok->set_text("OK");
		button_ok->func_clicked().set(this, &MessageBoxComponent::on_button_clicked, mb_result_ok);
	}
	if (button_yes)
	{
		button_yes->set_text("Yes");
		button_yes->func_clicked().set(this, &MessageBoxComponent::on_button_clicked, mb_result_yes);
	}
	if (button_no)
	{
		button_no->set_text("No");
		button_no->func_clicked().set(this, &MessageBoxComponent::on_button_clicked, mb_result_no);
	}
	if (button_cancel)
	{
		button_cancel->set_text("Cancel");
		button_cancel->func_clicked().set(this, &MessageBoxComponent::on_button_clicked, mb_result_cancel);
	}
}

void MessageBoxComponent::set_css_class()
{
	Canvas canvas = get_canvas();

	std::string icon_class = "";

	switch (messagebox_icon)
	{
	case mb_icon_error:
		icon_class = "error";
		break; 
	case mb_icon_warning:
		icon_class = "warning";
		break;
	case mb_icon_info:
		icon_class = "info";
		break;
	case mb_icon_question:
		icon_class = "question";
		break;
	}

	set_class(icon_class);
}

void MessageBoxComponent::on_render(Canvas &canvas, const Rect &dirty_rect)
{
	part_icon.render_box(canvas, rect_icon, dirty_rect);
	component->render_text(canvas, detail_text, rect_text, dirty_rect);
}

void MessageBoxComponent::create_parts()
{
	part_icon = GUIThemePart(this, CssStr::MessageBox::part_icon);

}

void MessageBoxComponent::on_button_clicked(MessageBoxResult result)
{
	get_parent_component()->exit_with_code(result);		
}

bool MessageBoxComponent::on_window_close(Window *window)
{
	window->exit_with_code(mb_result_cancel);

	return true;
}

}
