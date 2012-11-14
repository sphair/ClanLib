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
**    Magnus Norddahl
*/

#pragma once

#include "API/GUI/Components/scrollbar.h"

namespace clan
{

class TextEdit_Impl
{
public:
	TextEdit_Impl() :
		cursor_pos(0),
		max_length(-1),
		lowercase(false),
		uppercase(false),
		readonly(false),
		mouse_moves_left(false),
		mouse_selecting(false),
		selection_start(-1, 0),
		selection_length(0),
		cursor_blink_visible(true),
		clip_start_offset(0),
		clip_end_offset(0),
		ignore_mouse_events(false),
		cursor_drawing_enabled_when_parent_focused(false),
		select_all_on_focus_gain(false)
	{
		//prop_text_color = GUIThemePartProperty(CssStr::text_color, "black");		
		lines.resize(1);
	}

	~TextEdit_Impl()
	{
		timer.stop();
		scroll_timer.stop();
	}

	void layout_lines(Canvas &canvas);

	void on_process_message(std::shared_ptr<GUIMessage> &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_style_changed();
	void on_timer_expired();
	void on_resized();
	void on_scroll_timer_expired();
	void on_enable_changed();
	void create_parts();
	void create_components();
	void on_vertical_scroll();
	void update_vertical_scroll();
	void move_vertical_scroll();
	int  get_total_line_height();
	GUIThemePart::VerticalTextPosition get_vertical_text_align(Canvas &canvas, Font &font, const Rect &content_rect);

	Callback_v1<InputEvent &> func_before_edit_changed;
	Callback_v1<InputEvent &> func_after_edit_changed;
	Callback_v0 func_selection_changed;
	Callback_v0 func_focus_gained;
	Callback_v0 func_focus_lost;
	Callback_v0 func_enter_pressed;

	struct Line
	{
		Line() : invalidated(true) { }

		std::string text;
		SpanLayout layout;
		Rect box;
		bool invalidated;
	};

	TextEdit *textedit;
	ScrollBar *vert_scrollbar;
	Timer timer;
	std::vector<Line> lines;
	Vec2i cursor_pos;
	int max_length;
	bool mouse_selecting;
	bool lowercase;
	bool uppercase;
	bool readonly;
	Vec2i selection_start;
	int selection_length;
	Colorf text_color;
	std::string input_mask;
	bool cursor_drawing_enabled_when_parent_focused;

	static std::string break_characters;

	GUIComponent *part_selection;
	GUIComponent *part_cursor;

	void move(int steps, InputEvent &e);
	void insert_text(Vec2i pos, const std::string &str);
	void backspace();
	void del();
	Vec2i get_character_index(Point mouse_wincoords);
	Vec2i find_next_break_character(Vec2i pos);
	Vec2i find_previous_break_character(Vec2i pos);
	std::string get_visible_text_before_selection();
	std::string get_visible_text_after_selection();
	std::string get_visible_selected_text();
	Size get_visual_text_size(Canvas &canvas, Font &font, int pos, int npos) const;
	Size get_visual_text_size(Canvas &canvas, Font &font) const;
	Rect get_cursor_rect();
	Rect get_selection_rect();
	bool input_mask_accepts_input(Vec2i cursor_pos, const std::string &str);

	std::string::size_type to_offset(Vec2i pos) const;
	Vec2i from_offset(std::string::size_type offset) const;

	GUIThemePart::VerticalTextPosition vertical_text_align;
	Timer scroll_timer;

	bool mouse_moves_left;
	bool cursor_blink_visible;
	unsigned int blink_timer;
	int clip_start_offset;
	int clip_end_offset;
	bool ignore_mouse_events;

	struct UndoInfo
	{
		/* set undo text when:
  		   - added char after moving
		   - destructive block operation (del, cut etc)
		   - beginning erase
		*/

		UndoInfo() : first_erase(0), first_text_insert(0) {}
		std::string undo_text;
		bool first_erase;
		bool first_text_insert;
	} undo_info;

	bool select_all_on_focus_gain;
};

}
