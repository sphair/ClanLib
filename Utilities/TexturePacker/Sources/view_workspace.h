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

#pragma once

class View;

class ViewWorkspace : public clan::GUIComponent
{
public:
	ViewWorkspace(clan::GUIComponent *parent);
	~ViewWorkspace();

	void add_view(View *view);
	void remove_view(View *view);
	void show_view(View *view);
	void show_view(unsigned int index);
	unsigned int find_view_index(View *view) const;
	unsigned int get_current_view_index() const;
	void set_view_color(View *view, clan::Colorf &color);

	void try_show_view(unsigned int index);
	void next_view();
	void previous_view();

	clan::Callback_v1<View *> cb_view_close;

private:
	void on_render(clan::Canvas &canvas, const clan::Rect &clip_rect);
	void on_resized();
	void on_process_message(std::shared_ptr<clan::GUIMessage> &msg);
	void on_input_message(clan::GUIMessage_Input msg);

	void paint_tabs(clan::Canvas &canvas, const clan::Rect &clip_rect);
	clan::Rect get_workspace_area();

	struct ViewPage
	{
		ViewPage(const std::string &text, View *view) : text(text), color(clan::Colorf::black), view(view) { }
		bool operator ==(const ViewPage &other) const { return view == other.view; }

		std::string text;
		clan::Colorf color;
		clan::SpanLayout span;
		clan::Rect position;
		View *view;
	};

	bool hot;
	clan::Font font_tab;
	clan::Image image_cross, image_cross_hot, image_cross_empty;
	
	clan::GUIThemePart part_tab;
	std::vector<ViewPage> pages;
	unsigned int current_page_index;
};
