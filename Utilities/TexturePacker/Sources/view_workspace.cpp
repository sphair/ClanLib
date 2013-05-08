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
**    Magnus Norddahl
*/

#include "precomp.h"
#include "view_workspace.h"
#include "view.h"

ViewWorkspace::ViewWorkspace(GUIComponent *parent)
: GUIComponent(parent, "workspace"), hot(false), current_page_index(0)
{
	func_render().set(this, &ViewWorkspace::on_render);
	func_resized().set(this, &ViewWorkspace::on_resized);
	func_process_message().set(this, &ViewWorkspace::on_process_message);

	Canvas canvas = get_canvas();
	part_tab = GUIThemePart(this, "workspacetab");
	font_tab = Font(canvas, "Tahoma", -11);//part_background.get_font();
	image_cross = Image(canvas, "Resources/cross-small.png");
	image_cross_hot = Image(canvas, "Resources/cross-small-hot.png");
	image_cross_empty = Image(canvas, "Resources/cross-small-empty.png");
	on_resized();
}

ViewWorkspace::~ViewWorkspace()
{
	for (std::vector<ViewPage>::iterator it = pages.begin(); it != pages.end(); ++it)
		delete (*it).view;
}

void ViewWorkspace::add_view(View *view)
{
	view->set_visible(pages.empty());
	view->set_geometry(get_workspace_area());
	pages.push_back(ViewPage(view->get_title(), view));
}

unsigned int ViewWorkspace::find_view_index(View *view) const
{
	for (size_t i = 0; i < pages.size(); i++)
		if (pages[i].view == view)
			return (int)i;

	throw Exception("View is not a member of the workspace");
}

void ViewWorkspace::remove_view(View *view)
{
	unsigned int page_index = find_view_index(view);
	pages.erase(pages.begin()+page_index);
	if (current_page_index >= page_index && current_page_index > 0)
		current_page_index--;
	show_view(current_page_index);
}

void ViewWorkspace::show_view(View *view)
{
	show_view(find_view_index(view));
}

void ViewWorkspace::show_view(unsigned int index)
{
	current_page_index = index;
	if (!pages.empty())
	{
		for (size_t i = 0; i < pages.size(); i++)
			pages[i].view->set_visible(i == index);
		pages[index].color = Colorf::black;
	}
	request_repaint();
}

void ViewWorkspace::try_show_view(unsigned int index)
{
	if (index >= 0 && index < pages.size())
		show_view(index);
}

void ViewWorkspace::next_view()
{
	if (!pages.empty())
	{
		unsigned int next = current_page_index + 1;
		if (next >= pages.size())
			next = 0;
		show_view(next);
	}
}

void ViewWorkspace::previous_view()
{
	if (!pages.empty())
	{
		unsigned int new_index;
		if (current_page_index == 0)
			new_index = pages.size() - 1;
		else
			new_index = current_page_index - 1;

		show_view(new_index);
	}
}

void ViewWorkspace::set_view_color(View *view, Colorf &color)
{
	int page_index = find_view_index(view);
	if (page_index != current_page_index)
	{
		pages[page_index].color = color;
		request_repaint();
	}
}

unsigned int ViewWorkspace::get_current_view_index() const
{
	return current_page_index;
}

void ViewWorkspace::on_render(Canvas &canvas, const Rect &clip_rect)
{
	paint_tabs(canvas, clip_rect);
}

void ViewWorkspace::on_resized()
{
	Rect workspace_area = get_workspace_area();
	for (std::vector<ViewPage>::iterator it = pages.begin(); it != pages.end(); ++it)
		(*it).view->set_geometry(workspace_area);
	request_repaint();
}

Rect ViewWorkspace::get_workspace_area()
{
	Rect client_area = get_size();
	client_area.top += part_tab.get_css_height()+7;
	client_area.left += 7;
	client_area.right -= 7;
	client_area.bottom -= 9;
	return client_area;
}

void ViewWorkspace::paint_tabs(Canvas &canvas, const Rect &clip_rect)
{
	int tab_x = 15;
	for (std::vector<ViewPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
	{
		SpanLayout span;
		span.add_text(pages[page_index].text, font_tab, pages[page_index].color);
	
		if(pages[page_index].view->is_closable())
		{
			span.add_text("  ", font_tab, pages[page_index].color);

			if (page_index == current_page_index)
			{
				if (hot)
					span.add_image(image_cross_hot, 1, 0);
				else
					span.add_image(image_cross, 1, 0);
			}
			else
			{
				span.add_image(image_cross_empty, 1, 0);
			}
		}

		span.layout(canvas, 1000);
		int tab_width = clan::max(40, span.get_size().width);
		Rect current_tab(Point(tab_x, 0), Size(tab_width+20, part_tab.get_css_height()));
		if (page_index == current_page_index)
			part_tab.render_box(canvas, current_tab);
		span.set_position(Point(tab_x+10, current_tab.bottom-18));
		span.draw_layout(canvas);

		pages[page_index].span = span;
		pages[page_index].position = current_tab;
		tab_x = current_tab.right+2;
	}
}

void ViewWorkspace::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
		on_input_message(*input_msg);
}

void ViewWorkspace::on_input_message(GUIMessage_Input msg)
{
	Canvas canvas = get_canvas();
	InputEvent input_event = msg.input_event;
	if (input_event.type == InputEvent::pressed && input_event.id == mouse_left)
	{
		Point mouse_pos = input_event.mouse_pos;
		for (std::vector<ViewPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
		{
			if (pages[page_index].position.contains(mouse_pos))
			{
				SpanLayout::HitTestResult result = pages[page_index].span.hit_test(canvas, mouse_pos);
				if (page_index == current_page_index && result.type == SpanLayout::HitTestResult::inside && result.object_id == 0)
				{
					if (!cb_view_close.is_null())
						cb_view_close.invoke(pages[page_index].view);
				}
				else
				{
					show_view(page_index);
				}
				break;
			}
		}
		msg.consumed = true;
	}
	else if (input_event.type == InputEvent::released && input_event.id == mouse_left)
	{
		msg.consumed = true;
	}
	else if (input_event.type == InputEvent::pointer_moved)
	{
		bool new_hot_state = false;
		Point mouse_pos = input_event.mouse_pos;
		bool no_match = true;
		for (std::vector<ViewPage>::size_type page_index = 0; page_index < pages.size(); page_index++)
		{
			if (pages[page_index].position.contains(mouse_pos))
			{
				no_match = false;
				SpanLayout::HitTestResult result = pages[page_index].span.hit_test(canvas, mouse_pos);
				if (page_index == current_page_index && result.type == SpanLayout::HitTestResult::inside && result.object_id == 0)
				{
					new_hot_state = true;
				}
			}
		}

		if (new_hot_state != hot)
		{
			hot = new_hot_state;
			request_repaint();
		}
		msg.consumed = true;
	}
	else if (input_event.type == InputEvent::pressed && input_event.ctrl)
	{
		switch (input_event.id)
		{
		case keycode_tab:
			if (input_event.shift)
				previous_view();
			else
				next_view();
			msg.consumed = true;
			break;
		}
	}
}
